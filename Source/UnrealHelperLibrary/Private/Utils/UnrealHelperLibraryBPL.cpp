// Pavel Penkov 2025 All Rights Reserved.

#include "Utils/UnrealHelperLibraryBPL.h"

#include "KismetAnimationLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Engine/SCS_Node.h"
#include "Engine/SimpleConstructionScript.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Misc/ConfigCacheIni.h"
#include "Animation/AnimMontage.h"
#include "DrawDebugHelpers.h"
#include "NavigationSystem.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Widget.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"
#include "UI/UHLHUD.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UnrealHelperLibraryBPL)

static const int32 DEPTH_PRIORITY = -1;

FString UUnrealHelperLibraryBPL::GetProjectVersion()
{
	FString ProjectVersion;
	GConfig->GetString(TEXT("/Script/EngineSettings.GeneralProjectSettings"), TEXT("ProjectVersion"), ProjectVersion, GGameIni);
	return ProjectVersion;
}

void UUnrealHelperLibraryBPL::DebugPrintStrings(const FString& A, const FString& B, const FString& C, const FString& D, const FString& E, const FString& F, const FString& G, const FString& H,
	const FString& I, const FString& J, float Duration, const FName Key, const bool bEnabled)
{
	FString StringResult;
	StringResult.Empty(A.Len() + B.Len() + C.Len() + D.Len() + E.Len() + 1);  // adding one for the string terminator
	StringResult += A;
	StringResult += B;
	StringResult += C;
	StringResult += D;
	StringResult += E;

	if (!bEnabled)
		return;

	UKismetSystemLibrary::PrintString(nullptr, StringResult, true, true, FLinearColor(0, 0.66, 1), Duration, Key);
}

void UUnrealHelperLibraryBPL::DebugPrintString(const UObject* WorldContextObject, const FString& A, float Duration, const FName Key, const bool bEnabled)
{
	FString StringResult;
	StringResult.Empty(A.Len() + 1);  // adding one for the string terminator

	if (!bEnabled)
		return;

	UKismetSystemLibrary::PrintString(WorldContextObject, StringResult, true, true, FLinearColor(0, 0.66, 1), Duration, Key);
}

void UUnrealHelperLibraryBPL::DrawDebugBar() {}

float UUnrealHelperLibraryBPL::GetAnimMontageSectionLengthByName(UAnimMontage* AnimMontage, FName SectionName)
{
	float Result = -1;

	if (!IsValid(AnimMontage))
		return Result;
	int32 SectionIdx = SectionName.IsNone() ? 0 : AnimMontage->GetSectionIndex(SectionName);

	if (SectionIdx == INDEX_NONE)
		return Result;
	DebugPrintString(AnimMontage->GetWorld(), FString::Printf(TEXT("Section %s %i"), *SectionName.ToString(), SectionIdx), Result);
	Result = AnimMontage->GetSectionLength(SectionIdx);

	return Result;
}

EUHLDirection UUnrealHelperLibraryBPL::GetOppositeDirection(EUHLDirection Direction_In)
{
	switch (Direction_In)
	{
		case EUHLDirection::Left:
			return EUHLDirection::Right;
		case EUHLDirection::Right:
			return EUHLDirection::Left;
		case EUHLDirection::Front:
			return EUHLDirection::Back;
		case EUHLDirection::Back:
			return EUHLDirection::Front;
		default:
			return EUHLDirection::None;
	}
}

TArray<FString> UUnrealHelperLibraryBPL::GetNamesOfComponentsOnObject(UObject* OwnerObject, UClass* Class)
{
	TArray<FString> Result = {};

	UBlueprintGeneratedClass* BlueprintGeneratedClass =
		OwnerObject->IsA<UBlueprintGeneratedClass>() ? Cast<UBlueprintGeneratedClass>(OwnerObject) : Cast<UBlueprintGeneratedClass>(OwnerObject->GetClass());
	if (!BlueprintGeneratedClass)
		return Result;

	TArray<UObject*> DefaultObjectSubobjects;
	BlueprintGeneratedClass->GetDefaultObjectSubobjects(DefaultObjectSubobjects);

	// Search for ActorComponents created from C++
	for (UObject* DefaultSubObject : DefaultObjectSubobjects)
	{
		if (DefaultSubObject->IsA(Class))
		{
			Result.Add(DefaultSubObject->GetName());
		}
	}

	// Search for ActorComponents created in Blueprint
	for (USCS_Node* Node : BlueprintGeneratedClass->SimpleConstructionScript->GetAllNodes())
	{
		if (Node->ComponentClass->IsChildOf(Class))
		{
			Result.Add(Node->GetVariableName().ToString());
		}
	}

	return Result;
}

float UUnrealHelperLibraryBPL::RelativeAngleToActor(
	AActor* ActorRelativeToWhomAngleCalculated, AActor* TargetActor, bool bRelativeToActorBack, const bool bDebug, const float DebugLifetime, const FLinearColor DebugColor)
{
	if (!IsValid(ActorRelativeToWhomAngleCalculated) || !IsValid(TargetActor))
		return FLOAT_ERROR;

	float Multiplier = bRelativeToActorBack ? 1 : -1;
	float Result = UKismetAnimationLibrary::CalculateDirection(
		ActorRelativeToWhomAngleCalculated->GetActorLocation() - TargetActor->GetActorLocation(), (ActorRelativeToWhomAngleCalculated->GetActorForwardVector() * Multiplier).ToOrientationRotator());

	if (bDebug)
	{
		UWorld* DebugWorld = ActorRelativeToWhomAngleCalculated->GetWorld();
		FColor DebugCol = DebugColor.ToFColor(true);
		FVector LineStart = ActorRelativeToWhomAngleCalculated->GetActorLocation();
		FVector LineEnd = TargetActor->GetActorLocation();
		FVector TextLocation = GetCenterPointBetweenVectors(ActorRelativeToWhomAngleCalculated->GetWorld(), LineStart, LineEnd);

		DrawDebugString(DebugWorld, TextLocation, FString::Printf(TEXT("RelativeAngle: %.2f°"), Result), 0, DebugCol, DebugLifetime, true, 1.0f);
		DrawDebugDirectionalArrow(DebugWorld, LineStart, LineEnd, RELATIVE_POINT_ARROW_SIZE, DebugCol, true, DebugLifetime, DEPTH_PRIORITY, 2);
	}

	return Result;
}

float UUnrealHelperLibraryBPL::RelativeAngleToVector(AActor* ActorRelativeToWhomAngleCalculated, FVector TargetVector)
{
	if (!IsValid(ActorRelativeToWhomAngleCalculated))
		return FLOAT_ERROR;
	return UKismetAnimationLibrary::CalculateDirection(
		ActorRelativeToWhomAngleCalculated->GetActorLocation() - TargetVector, (ActorRelativeToWhomAngleCalculated->GetActorForwardVector() * -1).ToOrientationRotator());
}

float UUnrealHelperLibraryBPL::RelativeAngleVectorToVector(FVector VectorRelativeToWhomAngleCalculated, FVector TargetVector)
{
	return UKismetAnimationLibrary::CalculateDirection(VectorRelativeToWhomAngleCalculated - TargetVector, (VectorRelativeToWhomAngleCalculated * -1).ToOrientationRotator());
}

EUHLDirection UUnrealHelperLibraryBPL::GetHitReactDirection(const FVector& SourceActorLocation, const AActor* TargetActor)
{
	const FVector& ActorLocation = TargetActor->GetActorLocation();
	// PointPlaneDist is super cheap - 1 vector subtraction, 1 dot product.
	float DistanceToFrontBackPlane = FVector::PointPlaneDist(SourceActorLocation, ActorLocation, TargetActor->GetActorRightVector());
	float DistanceToRightLeftPlane = FVector::PointPlaneDist(SourceActorLocation, ActorLocation, TargetActor->GetActorForwardVector());

	if (FMath::Abs(DistanceToFrontBackPlane) <= FMath::Abs(DistanceToRightLeftPlane))
	{
		// Determine if Front or Back

		// Can see if it's left or right of Left/Right plane which would determine Front or Back
		if (DistanceToRightLeftPlane >= 0)
		{
			return EUHLDirection::Front;
		}
		return EUHLDirection::Back;
	}
	// Determine if Right or Left

	if (DistanceToFrontBackPlane >= 0)
	{
		return EUHLDirection::Right;
	}
	return EUHLDirection::Left;
}

FVector2D UUnrealHelperLibraryBPL::GetViewportSizeUnscaled(UObject* WorldContextObject)
{
	FVector2D ViewportSize = UWidgetLayoutLibrary::GetViewportSize(WorldContextObject);
	float ViewportScale = UWidgetLayoutLibrary::GetViewportScale(WorldContextObject);
	FVector2D ViewportSizeUnscaled = ViewportSize / ViewportScale;
	return ViewportSizeUnscaled;
}

FVector2D UUnrealHelperLibraryBPL::GetWidgetCenterPosition(UObject* WorldContextObject, UWidget* Widget)
{
	FVector2D Result;
	
	FGeometry WidgetGeometry = Widget->GetCachedGeometry();
	FVector2D WidgetCenterAbsolutePosition = WidgetGeometry.GetAbsolutePositionAtCoordinates(FVector2f(0.5, 0.5));
	
	FVector2D PixelPos;
	FVector2D ViewportPos;
	USlateBlueprintLibrary::AbsoluteToViewport(WorldContextObject, WidgetCenterAbsolutePosition, PixelPos, ViewportPos);

	Result = PixelPos;
	
	return Result;
}

float UUnrealHelperLibraryBPL::GetActorDistanceToCenterOfScreen(UObject* WorldContextObject, const AActor* Actor, APlayerController* PlayerController, bool bPlayerViewportRelative, const bool bDebug, const float DebugLifetime)
{
	float Result = 9999999.0f;
	FVector2D ViewportSizeUnscaled = GetViewportSizeUnscaled(WorldContextObject);
	
	FVector2D CurrentActorScreenPosition;
	UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(PlayerController, Actor->GetActorLocation(), CurrentActorScreenPosition, true);
	Result = FVector2D::Distance(CurrentActorScreenPosition, ViewportSizeUnscaled / 2);

	// same as in "GetActorClosestToCenterOfScreen"
	if (bDebug)
	{
		bool bRelativeToViewportCenter = false;
		float ViewportScale = UWidgetLayoutLibrary::GetViewportScale(WorldContextObject);
		FVector2D ViewportSize = UWidgetLayoutLibrary::GetViewportSize(WorldContextObject);
		FLineInfo LineInfo = {
			"TestLine" + Actor->GetName(),
			ViewportSize / 2,
			CurrentActorScreenPosition * ViewportScale,
			FColor::MakeRandomColor(),
			5,
			"" + Actor->GetName() + " " + FString::SanitizeFloat(Result),
			FColor::Blue,
			bRelativeToViewportCenter,
			true,
		};
		DrawDebugLineOnCanvas(Actor->GetWorld(), LineInfo, bRelativeToViewportCenter);
	}
	
	return Result;
}

AActor* UUnrealHelperLibraryBPL::GetActorClosestToCenterOfScreen(UObject* WorldContextObject, const TArray<AActor*>& Actors, APlayerController* PlayerController, FVector2D& ScreenPosition, bool bPlayerViewportRelative, const bool bDebug, const float DebugLifetime)
{
	AActor* Result = nullptr;
	FVector2D ResultScreenPosition = FVector2D(133700.322, 133700.322);
	float ResultDistance = FVector2D::Distance(ResultScreenPosition, FVector2D::ZeroVector);

	for (AActor* Actor : Actors)
	{
		float CurrentDistance = GetActorDistanceToCenterOfScreen(Actor, Actor, PlayerController, true, bDebug, DebugLifetime);

		if (CurrentDistance < ResultDistance)
		{
			ResultDistance = CurrentDistance;
			Result = Actor;
		}
	}

	ScreenPosition = ResultScreenPosition;
	return Result;
}

AActor* UUnrealHelperLibraryBPL::GetMostDistantActor(const TArray<AActor*>& Actors, float& MaxDistance_Out, FVector Location, const bool bDebug, const float DebugLifetime)
{
	// TODO use GetMostDistantVector if possible
	AActor* Result = nullptr;
	float GreatestDistance = -9999999;
	TMap<AActor*, float> ActorsAndDistances = {};
	
	for (AActor* Actor : Actors)
	{
		float Distance = FVector::Distance(Actor->GetActorLocation(), Location);
		ActorsAndDistances.Add(Actor, Distance);
		if (Distance > GreatestDistance)
		{
			GreatestDistance = Distance;
			Result = Actor;
			MaxDistance_Out = Distance;
		}
	}
	
	if (bDebug)
	{
		for (const TTuple<AActor*, float>& ActorWithDist : ActorsAndDistances)
		{
			bool bMostDistant = Result == ActorWithDist.Key;
			DrawDebugLine(ActorWithDist.Key->GetWorld(), ActorWithDist.Key->GetActorLocation(), Location, bMostDistant ? FColor::Green : FColor::Red, false, DebugLifetime, -1, 2.0f);
			DrawDebugString(ActorWithDist.Key->GetWorld(), FVector::ZeroVector, FString::Printf(TEXT("Distance: %.2f"), ActorWithDist.Value), ActorWithDist.Key, bMostDistant ? FColor::Green : FColor::Red, 0, true, 1);
		}
	}

	return Result;
}

FVector UUnrealHelperLibraryBPL::GetMostDistantVector(
	const UObject* WorldContextObject,
	const TArray<FVector> Vectors, FVector Location,
	float& MaxDistance_Out, int32& Index_Out, bool bUseNavigation,
	const bool bDebug, const float DebugLifetime)
{
	FVector Result = VECTOR_ERROR;

	TArray<float> Distances = {}; 
	float GreatestDistance = -9999999;
	
	for (int32 i = 0; i < Vectors.Num(); i++)
	{
		float Distance = FLOAT_ERROR;
		if (bUseNavigation)
		{
			double DistanceInDouble;
			UNavigationSystemV1::GetPathLength(WorldContextObject->GetWorld(), Vectors[i], Location, DistanceInDouble);
			Distance = DistanceInDouble;
		}
		else
		{
			Distance = FVector::Distance(Vectors[i], Location);
		}
		
		Distances[i] = Distance;

		if (Distance > GreatestDistance)
		{
			GreatestDistance = Distance;
			Result = Vectors[i];
			MaxDistance_Out = Distance;
			Index_Out = i;
		}
	}
	
	if (bDebug)
	{
		for (int32 i = 0; i < Vectors.Num(); i++)
		{
			bool bMostDistant = i == Index_Out;
			DrawDebugLine(WorldContextObject->GetWorld(), Vectors[i], Location, bMostDistant ? FColor::Green : FColor::Red, false, DebugLifetime, -1, 2.0f);
			DrawDebugString(WorldContextObject->GetWorld(), FVector::ZeroVector, FString::Printf(TEXT("Distance: %.2f"), Distances[i]), nullptr, bMostDistant ? FColor::Green : FColor::Red, 0, true, 1);
		}
	}

	return Result;
}

FVector UUnrealHelperLibraryBPL::GetMostDistantActorComponent(
	const UObject* WorldContextObject, const TArray<USceneComponent*> SceneComponents,
	FVector Location, float& MaxDistance_Out, int32& Index_Out, bool bUseNavigation,
	const bool bDebug, const float DebugLifetime)
{
	TArray<FVector> Vectors = {};
	for (int32 i = 0; i < SceneComponents.Num(); i++)
	{
		if (IsValid(SceneComponents[i]))
		{
			Vectors.Add(SceneComponents[i]->GetComponentToWorld().GetLocation());
		}
		else
		{
			Vectors.Add(VECTOR_ERROR);
		}
	}
	return GetMostDistantVector(WorldContextObject,
		Vectors, Location, 
		MaxDistance_Out, Index_Out, bUseNavigation,
		bDebug, DebugLifetime);
}

void UUnrealHelperLibraryBPL::DrawDebugLineOnCanvas(UObject* WorldContextObject, const FLineInfo& LineInfo, const bool bRelativeToViewportCenter)
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
	const APlayerController* PlayerController = GameInstance->GetFirstLocalPlayerController(WorldContextObject->GetWorld());
	AUHLHUD* HUD = PlayerController->GetHUD<AUHLHUD>();
	if (!IsValid(HUD)) return;

	HUD->AddOrUpdateLineInfoToDrawNextTick(LineInfo);
}

void UUnrealHelperLibraryBPL::DrawDebugCrossHair(
	UObject* WorldContextObject,
	const float CrossHairLineLength,
	const float LineThickness,
	const float AngleToRotate,
	const FVector2f& CrossHairCenterScreenSpace,
	const FLinearColor& LineColor,
	const bool bRelativeToViewportCenter
	)
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
	const APlayerController* PlayerController = GameInstance->GetFirstLocalPlayerController(WorldContextObject->GetWorld());
	AUHLHUD* HUD = PlayerController->GetHUD<AUHLHUD>();
	if (!IsValid(HUD)) return;

	FLineInfo LineInfo = {
		"TestLine",
		FVector2D(0, 0),
		FVector2D(200, 500),
		FColor::Red,
		2,
		"Test123",
		FColor::Blue,
		bRelativeToViewportCenter,
		true,
	};
	DrawDebugLineOnCanvas(WorldContextObject, LineInfo, bRelativeToViewportCenter);
}

UActorComponent* UUnrealHelperLibraryBPL::GetActorComponentByName(AActor* Actor, FString Name)
{
	if (!IsValid(Actor))
		return nullptr;

	for (UActorComponent* Component : Actor->GetComponents())
	{
		if (Component->GetName() == Name)
		{
			return Component;
		}
	}
	return nullptr;
}

USceneComponent* UUnrealHelperLibraryBPL::GetSceneComponentByName(AActor* Actor, FString Name)
{
	if (!IsValid(Actor))
		return nullptr;

	return Cast<USceneComponent>(GetActorComponentByName(Actor, Name));
}

FVector UUnrealHelperLibraryBPL::GetRandomPointInBox(const USceneComponent* Component, bool bOnGround, bool bDrawDebug, float DebugDrawTime)
{
	FVector Origin;
	FVector BoxExtent;
	float SphereRadius;

	UKismetSystemLibrary::GetComponentBounds(Component, Origin, BoxExtent, SphereRadius);
	FVector RandomPoint = UKismetMathLibrary::RandomPointInBoundingBox(Origin, BoxExtent);

	if (bOnGround)
	{
		RandomPoint.Z = GetHighestPointInBox(Component).Z;
		FHitResult OutHit;
		UKismetSystemLibrary::LineTraceSingle(Component->GetWorld(), RandomPoint, FVector(0, 0, -999999), TraceTypeQuery1, false, TArray<AActor*>(),
			bDrawDebug ? EDrawDebugTrace::Type::ForDuration : EDrawDebugTrace::Type::None, OutHit, true, FLinearColor::Red, FLinearColor::Green, DebugDrawTime);
		if (OutHit.IsValidBlockingHit())
		{
			RandomPoint = OutHit.Location;
		}
	}

	return RandomPoint;
}

FVector UUnrealHelperLibraryBPL::GetHighestPointInBox(const USceneComponent* Component)
{
	if (!IsValid(Component))
		return VECTOR_ERROR;

	FVector Origin;
	FVector BoxExtent;
	float SphereRadius = 0.0f;
	UKismetSystemLibrary::GetComponentBounds(Component, Origin, BoxExtent, SphereRadius);

	const FVector BoxMin = Origin - BoxExtent;
	const FVector BoxMax = Origin + BoxExtent;
	return FBox(BoxMin, BoxMax).Max;
}

FVector UUnrealHelperLibraryBPL::GetCenterPointInBox(const USceneComponent* Component)
{
	if (!IsValid(Component))
	{
		return VECTOR_ERROR;
	}

	FVector Origin;
	FVector BoxExtent;
	float SphereRadius = 0.0f;
	UKismetSystemLibrary::GetComponentBounds(Component, Origin, BoxExtent, SphereRadius);

	const FVector BoxMin = Origin - BoxExtent;
	const FVector BoxMax = Origin + BoxExtent;
	return FBox(BoxMin, BoxMax).GetCenter();
}

FBox UUnrealHelperLibraryBPL::GetComponentBox(const USceneComponent* Component)
{
	if (!IsValid(Component))
	{
		return FBox();
	}

	FVector Origin;
	FVector BoxExtent;
	float SphereRadius = 0.0f;
	UKismetSystemLibrary::GetComponentBounds(Component, Origin, BoxExtent, SphereRadius);

	const FVector BoxMin = Origin - BoxExtent;
	const FVector BoxMax = Origin + BoxExtent;
	return FBox(BoxMin, BoxMax);
}

FVector UUnrealHelperLibraryBPL::GetCenterPointBetweenVectors(
	const UObject* WorldContextObject, const FVector& PointA, const FVector& PointB,
	const bool bDebug, const float DebugLifetime, const FLinearColor DebugColor)
{
	FVector Result = (PointB - PointA) / 2 + PointA;

	if (bDebug)
	{
		UWorld* DebugWorld = WorldContextObject->GetWorld();
		DrawDebugSphere(DebugWorld, Result, 10.0f, 12, DebugColor.ToFColor(true), true, DebugLifetime, DEPTH_PRIORITY, 1);
	}

	return Result;
}

void UUnrealHelperLibraryBPL::GetPointAtRelativeAngleUsingVector(
	FVector& Point, FRotator& PointRotation, const UObject* WorldContextObject, const FVector& Location, const FVector& ForwardVector, const float Angle, const float Distance, const float OverrideZ, const bool bDebug, const float DebugLifetime, const FLinearColor DebugColor)
{
	if (!WorldContextObject)
	{
		Point = VECTOR_ERROR;
		return;
	}

	FVector LocationLocal = Location;
	// Override Z if specified (using -999999.f as sentinel value to indicate no override)
	if (OverrideZ > -999999.f)
	{
		LocationLocal.Z = OverrideZ;
	}

	FVector NormalizedForward = ForwardVector.GetSafeNormal();
	FVector Result = LocationLocal + NormalizedForward.RotateAngleAxis(Angle, FVector(0, 0, 1)) * Distance;

	if (bDebug)
	{
		UWorld* DebugWorld = WorldContextObject->GetWorld();
		FString DebugText = FString::Printf(TEXT("Angle %.2f°\nDistance %.2f"), Angle, Distance);
		if (OverrideZ > -999999.f)
		{
			DebugText += FString::Printf(TEXT("\nZ Override: %.2f"), OverrideZ);
		}
		DrawDebugString(DebugWorld, Result, DebugText, 0, DebugColor.ToFColor(true), DebugLifetime, true, 1.0f);
		DrawDebugSphere(DebugWorld, Result, 10.0f, 12, DebugColor.ToFColor(true), true, DebugLifetime, DEPTH_PRIORITY, 1);
		FVector ArrowLineEnd = LocationLocal + NormalizedForward.RotateAngleAxis(Angle, FVector(0, 0, 1)) * (Distance - 10);
		if (OverrideZ > -999999.f)
		{
			ArrowLineEnd.Z = OverrideZ;
		}
		DrawDebugDirectionalArrow(DebugWorld, LocationLocal, ArrowLineEnd, RELATIVE_POINT_ARROW_SIZE, FColor::White, true, DebugLifetime, DEPTH_PRIORITY, 2);
	}

	Point = Result;
	PointRotation = (Point - LocationLocal).ToOrientationRotator();
}

void UUnrealHelperLibraryBPL::GetPointAtRelativeAngle(
	FVector& Point, FRotator& PointRotation, const AActor* ActorIn, const float Angle, const float Distance, const bool bDebug, const float DebugLifetime, const FLinearColor DebugColor)
{
	if (!IsValid(ActorIn))
	{
		Point = VECTOR_ERROR;
		return;
	}
	FVector Result = ActorIn->GetActorLocation() + ActorIn->GetActorForwardVector().RotateAngleAxis(Angle, FVector(0, 0, 1)) * Distance;
	if (bDebug)
	{
		DrawDebugString(ActorIn->GetWorld(), Result, FString::Printf(TEXT("Angle %.2f°\nDistance %.2f"), Angle, Distance), 0, DebugColor.ToFColor(true), DebugLifetime, true, 1.0f);
		DrawDebugSphere(ActorIn->GetWorld(), Result, 10.0f, 12, DebugColor.ToFColor(true), true, DebugLifetime, DEPTH_PRIORITY, 1);
		FVector ArrowLineEnd = ActorIn->GetActorLocation() + ActorIn->GetActorForwardVector().RotateAngleAxis(Angle, FVector(0, 0, 1)) * (Distance - 10);
		DrawDebugDirectionalArrow(ActorIn->GetWorld(), ActorIn->GetActorLocation(), ArrowLineEnd, RELATIVE_POINT_ARROW_SIZE, FColor::White, true, DebugLifetime, DEPTH_PRIORITY, 2);
	}
	Point = Result;
	PointRotation = (Point - ActorIn->GetActorLocation()).ToOrientationRotator();
}

void UUnrealHelperLibraryBPL::GetPointAtRelativeDirection(FVector& Point, FRotator& PointRotation, const AActor* ActorIn, const EUHLDirection Direction, const float Distance, const bool bDebug,
	const float DebugLifetime, const FLinearColor DebugColor, const FText DebugText)
{
	if (!IsValid(ActorIn))
		return;

	float Angle = DirectionToAngle(Direction);
	GetPointAtRelativeAngle(Point, PointRotation, ActorIn, Angle, Distance);

	if (bDebug)
	{
		const UEnum* EnumPtr = FindObject<UEnum>(nullptr, TEXT("EUHLDirection"), true);
		if (EnumPtr)
		{
			DrawDebugString(ActorIn->GetWorld(), Point,
				FString::Printf(TEXT("%s \nDirection %s\nDistance %.2f"), *DebugText.BuildSourceString(), *EnumPtr->GetNameStringByValue((uint8)Direction), Distance), 0, DebugColor.ToFColor(true),
				DebugLifetime, true, 1.0f);
		}
		DrawDebugSphere(ActorIn->GetWorld(), Point, 10.0f, 12, DebugColor.ToFColor(true), true, DebugLifetime, DEPTH_PRIORITY, 1);
		FVector ArrowLineEnd;
		FRotator ArrowLineEndRotation;
		GetPointAtRelativeAngle(ArrowLineEnd, ArrowLineEndRotation, ActorIn, Angle, Distance - 10);
		DrawDebugDirectionalArrow(ActorIn->GetWorld(), ActorIn->GetActorLocation(), ArrowLineEnd, RELATIVE_POINT_ARROW_SIZE, FColor::White, true, DebugLifetime, DEPTH_PRIORITY, 2);
	}
}

void UUnrealHelperLibraryBPL::GetPointAtAngleRelativeToOtherActor(FVector& Point, FRotator& PointRotation, const AActor* Actor1, const AActor* Actor2, const float Angle, const float Distance,
	const bool bTakeZFromActor1, const bool bDebug, const float DebugLifetime, const FLinearColor DebugColor)
{
	if (!IsValid(Actor1) || !IsValid(Actor2))
		return;

	const FVector Actor1Location = Actor1->GetActorLocation();
	const FVector Actor2Location = Actor2->GetActorLocation();
	const FVector DirectionBetweenActors = (Actor2Location - Actor1Location).GetSafeNormal();
	Point = Actor1Location + (DirectionBetweenActors.RotateAngleAxis(Angle, FVector(0, 0, 1)) * Distance);
	Point.Z = bTakeZFromActor1 ? Actor1Location.Z : Actor2Location.Z;
	PointRotation = (Point - Actor1Location).ToOrientationRotator();

	if (bDebug)
	{
		UWorld* DebugWorld = Actor1->GetWorld();
		DrawDebugString(DebugWorld, Point, FString::Printf(TEXT("Angle %.2f°\nDistance %.2f"), Angle, Distance), 0, DebugColor.ToFColor(true), DebugLifetime, true, 1.0f);
		DrawDebugSphere(DebugWorld, Point, 10.0f, 12, DebugColor.ToFColor(true), true, DebugLifetime, DEPTH_PRIORITY, 1);
		FVector ArrowLineEnd = Actor1Location + (DirectionBetweenActors.RotateAngleAxis(Angle, FVector(0, 0, 1)) * (Distance - 10));
		Point.Z = bTakeZFromActor1 ? Actor1Location.Z : Actor2Location.Z;
		DrawDebugDirectionalArrow(DebugWorld, Actor1Location, Actor2Location, RELATIVE_POINT_ARROW_SIZE, FColor::White, true, DebugLifetime, DEPTH_PRIORITY, 1);
		DrawDebugDirectionalArrow(DebugWorld, Actor1Location, ArrowLineEnd, RELATIVE_POINT_ARROW_SIZE, FColor::White, true, DebugLifetime, DEPTH_PRIORITY, 2);
	}
}

void UUnrealHelperLibraryBPL::GetPointAtDirectionRelativeToOtherActor(FVector& Point, FRotator& PointRotation, const AActor* Actor1, const AActor* Actor2, const EUHLDirection Direction,
	const float Distance, const bool bTakeZFromActor1, const bool bDebug, const float DebugLifetime, const FLinearColor DebugColor)
{
	if (!IsValid(Actor1) || !IsValid(Actor2))
		return;

	float Angle = DirectionToAngle(Direction);
	GetPointAtAngleRelativeToOtherActor(Point, PointRotation, Actor1, Actor2, Angle, Distance, bTakeZFromActor1);

	if (bDebug)
	{
		const UEnum* EnumPtr = FindObject<UEnum>(nullptr, TEXT("EUHLDirection"), true);
		if (EnumPtr)
		{
			DrawDebugString(Actor1->GetWorld(), Point, FString::Printf(TEXT("Direction %s\nDistance %.2f"), *EnumPtr->GetNameStringByValue((uint8)Direction), Distance), 0, DebugColor.ToFColor(true),
				DebugLifetime, true, 1.0f);
		}
		DrawDebugSphere(Actor1->GetWorld(), Point, 10.0f, 12, DebugColor.ToFColor(true), true, DebugLifetime, DEPTH_PRIORITY, 1);
		FVector ArrowLineEnd;
		FRotator ArrowLineEndRotation;
		GetPointAtAngleRelativeToOtherActor(ArrowLineEnd, ArrowLineEndRotation, Actor1, Actor2, Angle, Distance, bTakeZFromActor1);
		DrawDebugDirectionalArrow(Actor1->GetWorld(), Actor1->GetActorLocation(), Actor2->GetActorLocation(), RELATIVE_POINT_ARROW_SIZE, FColor::White, true, DebugLifetime, DEPTH_PRIORITY, 1);
		DrawDebugDirectionalArrow(Actor1->GetWorld(), Actor1->GetActorLocation(), ArrowLineEnd, RELATIVE_POINT_ARROW_SIZE, FColor::White, true, DebugLifetime, DEPTH_PRIORITY, 2);
	}
}

void UUnrealHelperLibraryBPL::GetPointAtDirectionRelativeToOtherVector(FVector& Point, FRotator& PointRotation, const FVector& Vector1, const FVector& Vector2, const EUHLDirection Direction,
	const float Distance, const bool bTakeZFromVector1, const bool bDebug, const float DebugLifetime, const FLinearColor DebugColor)
{
	float Angle = DirectionToAngle(Direction);
	const FVector DirectionBetweenVectors = (Vector2 - Vector1).GetSafeNormal();
	Point = Vector1 + (DirectionBetweenVectors.RotateAngleAxis(Angle, FVector(0, 0, 1)) * Distance);
	Point.Z = bTakeZFromVector1 ? Vector1.Z : Vector2.Z;
	PointRotation = (Point - Vector1).ToOrientationRotator();

	if (bDebug)
	{
		UWorld* DebugWorld = GEngine->GetWorld();
		if (!DebugWorld) return;

		const UEnum* EnumPtr = FindObject<UEnum>(nullptr, TEXT("EUHLDirection"), true);
		if (EnumPtr)
		{
			DrawDebugString(DebugWorld, Point, FString::Printf(TEXT("Direction %s\nDistance %.2f"), *EnumPtr->GetNameStringByValue((uint8)Direction), Distance), 0, DebugColor.ToFColor(true),
				DebugLifetime, true, 1.0f);
		}
		DrawDebugSphere(DebugWorld, Point, 10.0f, 12, DebugColor.ToFColor(true), true, DebugLifetime, DEPTH_PRIORITY, 1);
		FVector ArrowLineEnd = Vector1 + (DirectionBetweenVectors.RotateAngleAxis(Angle, FVector(0, 0, 1)) * (Distance - 10));
		Point.Z = bTakeZFromVector1 ? Vector1.Z : Vector2.Z;
		DrawDebugDirectionalArrow(DebugWorld, Vector1, Vector2, RELATIVE_POINT_ARROW_SIZE, FColor::White, true, DebugLifetime, DEPTH_PRIORITY, 1);
		DrawDebugDirectionalArrow(DebugWorld, Vector1, ArrowLineEnd, RELATIVE_POINT_ARROW_SIZE, FColor::White, true, DebugLifetime, DEPTH_PRIORITY, 2);
	}
}

float UUnrealHelperLibraryBPL::DirectionToAngle(const EUHLDirection DirectionIn)
{
	if (DirectionIn == EUHLDirection::Front)
		return 0.0f;
	if (DirectionIn == EUHLDirection::Back)
		return 180.0f;
	if (DirectionIn == EUHLDirection::Left)
		return -90.0f;
	if (DirectionIn == EUHLDirection::Right)
		return 90.0f;
	if (DirectionIn == EUHLDirection::FrontLeft)
		return -45.0f;
	if (DirectionIn == EUHLDirection::FrontRight)
		return 45.0f;
	if (DirectionIn == EUHLDirection::BackLeft)
		return -135.0f;
	if (DirectionIn == EUHLDirection::BackRight)
		return 135.0f;
	return 0.0f;
}

EUHLDirection UUnrealHelperLibraryBPL::ConvertMovementInputVectorToDirection(const FVector& InputVector)
{
	// Project to 2D
	float X = InputVector.X;
	float Y = InputVector.Y;
    
	// atan2 returns radians from -π to +π
	float Radians = FMath::Atan2(Y, X);
	float Degrees = FMath::RadiansToDegrees(Radians);
    
	// Normalize into [0, 360)
	if (Degrees < 0.f)
	{
		Degrees += 360.f;
	}

	// Divide into 8 slices, round to nearest integer [0..7]
	int32 Slice = FMath::RoundToInt(Degrees / 45.f) % 8;

	switch (Slice)
	{
		case 0: return EUHLDirection::Right;
		case 1: return EUHLDirection::FrontRight;
		case 2: return EUHLDirection::Front;
		case 3: return EUHLDirection::FrontLeft;
		case 4: return EUHLDirection::Left;
		case 5: return EUHLDirection::BackLeft;
		case 6: return EUHLDirection::Back;
		case 7: return EUHLDirection::BackRight;
		default: return EUHLDirection::Front;  // fall-back
	}
}

EUHLDirection UUnrealHelperLibraryBPL::GetMovementDirection(
	const FVector& Velocity, const FRotator& ActorRotation, float DeadZone)
{
	// 1) Flatten to horizontal plane
	FVector FlatVel = Velocity;
	FlatVel.Z = 0.f;

	// 2) Early-out if below threshold
	if (FlatVel.SizeSquared() <= FMath::Square(DeadZone))
	{
		return EUHLDirection::None;
	}

	// 3) Compute world-space yaw of movement
	const float MovementYaw = FlatVel.Rotation().Yaw;

	// 4) Your actor's facing yaw
	const float ForwardYaw = ActorRotation.Yaw;

	// 5) Signed difference in [-180, +180]
	float DeltaYaw = FMath::UnwindDegrees(MovementYaw - ForwardYaw);

	// 6) Snap into 8 slices of 45° each
	float SliceF = DeltaYaw / 45.f;
	int32 SliceI = FMath::RoundToInt(SliceF);

	// 7) Wrap the exact 180° case: -4 → +4
	if (SliceI == -4) SliceI = 4;

	// 8) Ensure index in [0..7]
	SliceI = (SliceI + 8) % 8;

	// 9) Map to your enum
	switch (SliceI)
	{
		case 0:  return EUHLDirection::Front;
		case 1:  return EUHLDirection::FrontRight;
		case 2:  return EUHLDirection::Right;
		case 3:  return EUHLDirection::BackRight;
		case 4:  return EUHLDirection::Back;
		case 5:  return EUHLDirection::BackLeft;
		case 6:  return EUHLDirection::Left;
		case 7:  return EUHLDirection::FrontLeft;
		default: return EUHLDirection::None; // should never hit this
	}
}

EUHLDirection UUnrealHelperLibraryBPL::GetEnemyMovementDirectionRelativeToCharacter(
    UObject* WorldContextObject,
    const FVector& EnemyVelocity,
    const FVector& MyVelocity,
    const FVector& ActorLocation,
    const FRotator& ActorRotation,
    float DeadZone,
    float AngleToleranceDeg,
    bool bFourWay,
    bool bDebug,
    float DebugDuration,
    float DebugThickness
)
{
    // If enemy isn't moving, return None immediately
    if (EnemyVelocity.IsNearlyZero(KINDA_SMALL_NUMBER))
    {
        if (bDebug && WorldContextObject)
        {
            UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
            DrawDebugString(World, ActorLocation + FVector(0,0,50), TEXT("None"), nullptr, FColor::White, DebugDuration, true);
        }
        return EUHLDirection::None;
    }

    // Compute relative movement and flatten to XY
    FVector RelVel = EnemyVelocity - MyVelocity;
    RelVel.Z = 0.f;

    // Effective dead zone threshold
    const float EffectiveDeadZone = (DeadZone > 0.f) ? DeadZone : KINDA_SMALL_NUMBER;
    if (RelVel.SizeSquared() <= FMath::Square(EffectiveDeadZone))
    {
        if (bDebug && WorldContextObject)
        {
            UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
            DrawDebugString(World, ActorLocation + FVector(0,0,50), TEXT("None"), nullptr, FColor::White, DebugDuration, true);
        }
        return EUHLDirection::None;
    }

    // Compute yaw difference
    const float MovementYaw = RelVel.Rotation().Yaw;
    const float ForwardYaw  = ActorRotation.Yaw;
    float DeltaYaw = FMath::UnwindDegrees(MovementYaw - ForwardYaw);

    // Determine octant index with tolerance
    const float SliceF = DeltaYaw / 45.f;
    const float TolSlice = AngleToleranceDeg / 45.f;
    int32 SliceI = INDEX_NONE;
    for (int32 i = 0; i < 8; ++i)
    {
        if (FMath::Abs(SliceF - i) <= TolSlice)
        {
            SliceI = i;
            break;
        }
    }
    if (SliceI == INDEX_NONE)
    {
        SliceI = FMath::RoundToInt(SliceF);
    }
    if (SliceI == -4) SliceI = 4;
    SliceI = (SliceI + 8) % 8;

    // Map index to enum
    EUHLDirection DirEnum;
    switch (SliceI)
    {
        case 0:  DirEnum = EUHLDirection::Front;      break;
        case 1:  DirEnum = EUHLDirection::FrontRight; break;
        case 2:  DirEnum = EUHLDirection::Right;      break;
        case 3:  DirEnum = EUHLDirection::BackRight;  break;
        case 4:  DirEnum = EUHLDirection::Back;       break;
        case 5:  DirEnum = EUHLDirection::BackLeft;   break;
        case 6:  DirEnum = EUHLDirection::Left;       break;
        case 7:  DirEnum = EUHLDirection::FrontLeft;  break;
        default: DirEnum = EUHLDirection::None;       break;
    }

    // Collapse to 4-way if requested
    if (bFourWay)
    {
        if (DirEnum == EUHLDirection::FrontLeft || DirEnum == EUHLDirection::FrontRight)
            DirEnum = EUHLDirection::Front;
        else if (DirEnum == EUHLDirection::BackLeft || DirEnum == EUHLDirection::BackRight)
            DirEnum = EUHLDirection::Back;
    }

    // Debug visuals
    if (bDebug && WorldContextObject)
    {
        UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
        const float DebugLen = 200.f;

        // Draw octant boundaries
        for (int32 i = 0; i < 8; ++i)
        {
            float Yaw = ForwardYaw + i * 45.f;
            FVector DirVec = FRotator(0.f, Yaw, 0.f).Vector() * DebugLen;
            DrawDebugLine(World, ActorLocation, ActorLocation + DirVec, FColor::Silver, false, DebugDuration, 0, DebugThickness * 0.5f);
        }

        // Draw relative movement arrow (cyan)
        DrawDebugDirectionalArrow(World, ActorLocation, ActorLocation + RelVel.GetSafeNormal() * DebugLen,
            50.f, FColor::Cyan, false, DebugDuration, 0, DebugThickness);

        // Draw forward axis (green)
        FVector FwdVec = ActorRotation.Vector() * DebugLen;
        DrawDebugLine(World, ActorLocation, ActorLocation + FwdVec, FColor::Green, false, DebugDuration, 0, DebugThickness);

        // Draw chosen direction arrow (red)
        float ChosenYaw = ForwardYaw + SliceI * 45.f;
        FVector ChosenVec = FRotator(0.f, ChosenYaw, 0.f).Vector() * DebugLen;
        DrawDebugLine(World, ActorLocation, ActorLocation + ChosenVec, FColor::Red, false, DebugDuration, 0, DebugThickness);

        // Label result using clean name
        UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EUHLDirection"), true);
        FString Name = EnumPtr ? EnumPtr->GetNameStringByValue((int64)DirEnum) : TEXT("None");
        DrawDebugString(World, ActorLocation + FVector(0,0,50), Name, nullptr, FColor::White, DebugDuration, true);
    }

    return DirEnum;
}


float UUnrealHelperLibraryBPL::ConvertPercentToMultiplier(float Percent) { return (100.0f - Percent) / 100.0f; }

AActor* UUnrealHelperLibraryBPL::FindAttachedActorByTag(AActor* ActorIn, FName Tag)
{
	TArray<AActor*> OutActors;
	ActorIn->GetAttachedActors(OutActors, true, true);

	AActor** ActorSearchResult = OutActors.FindByPredicate([Tag](const AActor* Actor)
	{
		return Actor->ActorHasTag(Tag);
	});

	if (ActorSearchResult)
	{
		return *ActorSearchResult;
	}
	return nullptr;
}

bool UUnrealHelperLibraryBPL::IsPreviewWorld(UObject* WorldContextObject) { return WorldContextObject->GetWorld()->IsPreviewWorld(); }

bool UUnrealHelperLibraryBPL::IsGameWorld(UObject* WorldContextObject) { return WorldContextObject->GetWorld()->IsGameWorld(); }

bool UUnrealHelperLibraryBPL::IsEditorWorld(UObject* WorldContextObject) { return WorldContextObject->GetWorld()->IsEditorWorld(); }

bool UUnrealHelperLibraryBPL::IsObjectInPreviewWorld(UObject* Object) { return Object->GetWorld()->IsPreviewWorld(); }

bool UUnrealHelperLibraryBPL::IsObjectInEditorWorld(UObject* Object) { return Object->GetWorld()->IsEditorWorld(); }

bool UUnrealHelperLibraryBPL::IsObjectInGameWorld(UObject* Object) { return Object->GetWorld()->IsGameWorld(); }

bool UUnrealHelperLibraryBPL::IsWorldTearingDown(UObject* WorldContextObject) { return WorldContextObject->GetWorld()->bIsTearingDown; }

bool UUnrealHelperLibraryBPL::IsOtherActorInAngle(AActor* Actor, AActor* OtherActor, TArray<FFloatRange> Ranges)
{
	float RelativeAngle = RelativeAngleToActor(Actor, OtherActor);
	bool bInAngle = false;
	for (FFloatRange Range : Ranges)
	{
		bInAngle = UKismetMathLibrary::InRange_FloatFloat(RelativeAngle, Range.GetLowerBoundValue(), Range.GetUpperBoundValue(), true, true);
		if (bInAngle)
			break;
	}
	return bInAngle;
}

bool UUnrealHelperLibraryBPL::InRangeToOtherCharacter(ACharacter* Character, ACharacter* OtherCharacter, FFloatRange Range, bool bIncludeSelfCapsuleRadius, bool bIncludeTargetCapsuleRadius)
{
	if (!Character || !OtherCharacter)
	{
		return false;
	}

	float CurrentDistance = Character->GetDistanceTo(OtherCharacter);

	if (bIncludeSelfCapsuleRadius)
	{
		CurrentDistance -= Character->GetCapsuleComponent()->GetScaledCapsuleRadius();
	}
	if (bIncludeTargetCapsuleRadius)
	{
		CurrentDistance -= OtherCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius();
	}

	bool bInRange = UKismetMathLibrary::InRange_FloatFloat(CurrentDistance, Range.GetLowerBoundValue(), Range.GetUpperBoundValue());
	return bInRange;
}

bool UUnrealHelperLibraryBPL::InRangeToLocation(
	ACharacter* Character, FVector Location, FFloatRange Range, bool bIncludeSelfCapsuleRadius)
{
	if (!Character)
	{
		return false;
	}

	float CurrentDistance = FVector::Dist2D(Character->GetActorLocation(), Location);

	if (bIncludeSelfCapsuleRadius)
	{
		CurrentDistance -= Character->GetCapsuleComponent()->GetScaledCapsuleRadius();
	}

	bool bInRange = UKismetMathLibrary::InRange_FloatFloat(CurrentDistance, Range.GetLowerBoundValue(), Range.GetUpperBoundValue());
	return bInRange;
}

FString UUnrealHelperLibraryBPL::GetPathToFile(UObject* Object)
{
	FString NameAndWithSlash = FString::Printf(TEXT("/%s"), *Object->GetName());
	return Object->GetPathName(NULL).Replace(*Object->GetName(), TEXT(""));
}

bool UUnrealHelperLibraryBPL::IsDebugBuild()
{
	EBuildConfiguration BuildConfiguration = FApp::GetBuildConfiguration();
	if (BuildConfiguration == EBuildConfiguration::Debug || BuildConfiguration == EBuildConfiguration::DebugGame)
	{
		return true;
	}
	return false;
}

bool UUnrealHelperLibraryBPL::IsDevelopmentBuild()
{
	EBuildConfiguration BuildConfiguration = FApp::GetBuildConfiguration();
	return BuildConfiguration == EBuildConfiguration::Development;
}

bool UUnrealHelperLibraryBPL::IsShippingBuild()
{
	EBuildConfiguration BuildConfiguration = FApp::GetBuildConfiguration();
	return BuildConfiguration == EBuildConfiguration::Shipping;
}

bool UUnrealHelperLibraryBPL::IsTestBuild()
{
	EBuildConfiguration BuildConfiguration = FApp::GetBuildConfiguration();
	return BuildConfiguration == EBuildConfiguration::Test;
}

bool UUnrealHelperLibraryBPL::IsInEditor()
{
#if WITH_EDITOR
	return true;
#else
	return false;
#endif
}

EUHLBuildType UUnrealHelperLibraryBPL::GetBuildType()
{
	if (IsInEditor())
		return EUHLBuildType::Editor;

	EBuildConfiguration BuildConfiguration = FApp::GetBuildConfiguration();
	switch (BuildConfiguration)
	{
		case EBuildConfiguration::Debug:
			return EUHLBuildType::Debug;
			break;
		case EBuildConfiguration::DebugGame:
			return EUHLBuildType::Debug;
			break;
		case EBuildConfiguration::Development:
			return EUHLBuildType::Development;
			break;
		case EBuildConfiguration::Shipping:
			return EUHLBuildType::Shipping;
			break;
		case EBuildConfiguration::Test:
			return EUHLBuildType::Test;
			break;
		default:
			return IsInEditor() ? EUHLBuildType::Editor : EUHLBuildType::None;
			break;
	}
}

float UUnrealHelperLibraryBPL::RandomValueInInterval(FFloatInterval Range)
{
	// Ensure correct ordering
	const float Lower = FMath::Min(Range.Min, Range.Max);
	const float Upper = FMath::Max(Range.Min, Range.Max);

	return FMath::FRandRange(Lower, Upper);
}

bool UUnrealHelperLibraryBPL::IsWorldPartitionEnabled(UObject* WorldContextObject)
{
	if (!WorldContextObject) return false;

	UWorld* World = WorldContextObject->GetWorld();
	return World && World->GetWorldPartition() != nullptr;
}

bool UUnrealHelperLibraryBPL::IsLevelHasSublevels(UObject* WorldContextObject)
{	
	UWorld* World = WorldContextObject->GetWorld();
	const TArray<ULevelStreaming*>& StreamingLevels = World->GetStreamingLevels();

	return StreamingLevels.Num() > 0;
}

void UUnrealHelperLibraryBPL::GetAllStreamingLevels(
	UObject* WorldContextObject, TArray<ULevelStreaming*>& OutStreamingLevels)
{
	OutStreamingLevels.Empty();

	if (!WorldContextObject)
	{
		UE_LOG(LogTemp, Warning, TEXT("UUnrealHelperLibraryBPL::GetAllStreamingLevels: Invalid WorldContextObject"));
		return;
	}

	UWorld* World = GEngine
		? GEngine->GetWorldFromContextObjectChecked(WorldContextObject)
		: nullptr;

	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("UUnrealHelperLibraryBPL::GetAllStreamingLevels: Could not resolve UWorld from context"));
		return;
	}

	// Grab the streaming levels array
	OutStreamingLevels = World->GetStreamingLevels();
}

void UUnrealHelperLibraryBPL::GetAllSubLevels(UObject* WorldContextObject, TArray<ULevel*>& OutSubLevels)
{
	OutSubLevels.Empty();

	if (!WorldContextObject)
	{
		UE_LOG(LogTemp, Warning, TEXT("GetAllSubLevels: Invalid WorldContextObject"));
		return;
	}

	// Resolve the world from the context object
	UWorld* World = GEngine
		? GEngine->GetWorldFromContextObjectChecked(WorldContextObject)
		: nullptr;

	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("GetAllSubLevels: Could not resolve UWorld from context"));
		return;
	}

	// World->GetLevels() returns every ULevel* currently loaded:
	//   - The Persistent Level
	//   - Any Streaming Levels that are loaded into memory
	OutSubLevels = World->GetLevels();
}

bool UUnrealHelperLibraryBPL::IsMapPackageName(const FString& PackageName)
{
	// Convert to filename to check extension
	FString Filename;
	if (FPackageName::TryConvertLongPackageNameToFilename(PackageName, Filename, FPackageName::GetMapPackageExtension()))
	{
		// Ensure the file actually exists on disk (optional)
		return FPaths::FileExists(Filename);
	}
	return false;
}

FString UUnrealHelperLibraryBPL::GetCleanLevelName(const FString& PackageName)
{
	// Turn "/Game/Maps/MyMap" into "MyMap" (or "UEDPIE_0_MyMap" if in PIE)
	const FString ShortName = FPackageName::GetShortName(PackageName);

	// If PIE, find the first "_" and strip off the prefix
	if (ShortName.StartsWith(TEXT("UEDPIE_")))
	{
		int32 UnderscoreIndex = INDEX_NONE;
		// Finds the first underscore after "UEDPIE"
		if (ShortName.FindChar(TEXT('_'), UnderscoreIndex))
		{
			// Chop off everything up to (and including) that underscore
			return ShortName.Mid(UnderscoreIndex + 1);
		}
	}

	// Otherwise, just return the plain short name
	return ShortName;
}

void UUnrealHelperLibraryBPL::GetAllSubLevelPackageNames(UObject* WorldContextObject, TArray<FString>& OutLevelPackageNames)
{
	OutLevelPackageNames.Empty();

	if (!WorldContextObject) { return; }

	UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
	if (!World) { return; }

	// 1) Persistent Level
	// if (ULevel* Persistent = World->PersistentLevel)
	// {
	// 	const FString Raw = Persistent->GetOutermost()->GetName();
	// 	if (IsMapPackageName(Raw))
	// 	{
	// 		OutLevelPackageNames.Add(Raw);
	// 	}
	// }

	// 2) All streaming levels (loaded or not)
	for (ULevelStreaming* StreamingLevel : World->GetStreamingLevels())
	{
		if (!StreamingLevel) continue;

		const FString Raw = StreamingLevel->GetWorldAssetPackageName();
		const FString CleanName = GetCleanLevelName(Raw);

		// Only include if it’s actually a .umap in your project
		if (IsMapPackageName(Raw))
		{
			OutLevelPackageNames.Add(CleanName);
		}
	}
}


FColor UUnrealHelperLibraryBPL::RandomColor(int32 Seed)
{
	if (Seed >= 0)
	{
		return FColor::MakeRandomSeededColor(Seed);
	}
	return FColor::MakeRandomColor();
}

FLinearColor UUnrealHelperLibraryBPL::RandomLinearColor(int32 Seed)
{
	if (Seed >= 0)
	{
		return FLinearColor::MakeRandomSeededColor(Seed);
	}
	return FLinearColor::MakeRandomColor();
}
