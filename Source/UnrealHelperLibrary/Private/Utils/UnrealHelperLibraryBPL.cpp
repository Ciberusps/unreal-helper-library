// Copyright (c) 2024 Pavel Penkov

#include "Utils/UnrealHelperLibraryBPL.h"

#include "AbilitySystemComponent.h"
#include "GameplayTagsManager.h"
#include "KismetAnimationLibrary.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Class.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Enum.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Int.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Name.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_NativeEnum.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Rotator.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_String.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
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
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"
#include "GameFramework/HUD.h"
#include "Subsystems/DebugSubsystem/UHLDebugSubsystem.h"
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

FGameplayEffectSpec UUnrealHelperLibraryBPL::CreateGenericGASGameplayEffectSpec(
	TSubclassOf<UGameplayEffect> GameplayEffectClass, AActor* HitInstigator, AActor* InEffectCauser, const FHitResult& HitResult, const UObject* SourceObject)
{
	const UGameplayEffect* GameplayEffect = GameplayEffectClass->GetDefaultObject<UGameplayEffect>();
	FGameplayEffectContext* GameplayEffectContext = new FGameplayEffectContext(HitInstigator, InEffectCauser);
	FGameplayEffectContextHandle GameplayEffectContextHandle(GameplayEffectContext);
	GameplayEffectContextHandle.AddHitResult(HitResult);
	GameplayEffectContextHandle.AddSourceObject(SourceObject);
	FGameplayEffectSpec GameplayEffectSpec(GameplayEffect, GameplayEffectContextHandle);
	return GameplayEffectSpec;
}

void UUnrealHelperLibraryBPL::UpdateStateGameplayTags(UAbilitySystemComponent* ASC, bool bCondition, FGameplayTag PositiveConditionTag, FGameplayTag NegativeConditionTag)
{
	if (!ASC)
		return;

	if (bCondition)
	{
		if (!ASC->HasMatchingGameplayTag(PositiveConditionTag))
		{
			ASC->AddLooseGameplayTag(PositiveConditionTag);
		}
		if (NegativeConditionTag != FGameplayTag::EmptyTag)
		{
			ASC->RemoveLooseGameplayTag(NegativeConditionTag, 999999);
		}
	}
	else
	{
		if (NegativeConditionTag == FGameplayTag::EmptyTag)
		{
			ASC->RemoveLooseGameplayTag(PositiveConditionTag, 999999);
		}
		else
		{
			if (!ASC->HasMatchingGameplayTag(NegativeConditionTag))
			{
				ASC->AddLooseGameplayTag(NegativeConditionTag);
			}
			ASC->RemoveLooseGameplayTag(PositiveConditionTag, 999999);
		}
	}
}

bool UUnrealHelperLibraryBPL::TryActivateAbilityWithTag(UAbilitySystemComponent* ASC, FGameplayTag GameplayTag, bool bAllowRemoteActivation)
{
	if (!IsValid(ASC))
		return false;
	return ASC->TryActivateAbilitiesByTag(FGameplayTagContainer(GameplayTag), bAllowRemoteActivation);
}

bool UUnrealHelperLibraryBPL::TryCancelAbilityWithTag(UAbilitySystemComponent* ASC, FGameplayTag GameplayTag)
{
	if (!IsValid(ASC))
		return false;

	bool bResult = false;
	TArray<FGameplayAbilitySpec*> AbilitiesToCancel;
	ASC->GetActivatableGameplayAbilitySpecsByAllMatchingTags(FGameplayTagContainer(GameplayTag), AbilitiesToCancel, false);

	for (FGameplayAbilitySpec* AbilitySpec : AbilitiesToCancel)
	{
		TArray<UGameplayAbility*> AbilityInstances = AbilitySpec->GetAbilityInstances();
		for (UGameplayAbility* Ability : AbilityInstances)
		{
			if (Ability->IsActive())
			{
				Ability->K2_CancelAbility();
				bResult = true;
			}
		}
	}
	return bResult;
}

TArray<bool> UUnrealHelperLibraryBPL::TryCancelAbilitiesWithTags(UAbilitySystemComponent* ASC, TArray<FGameplayTag> GameplayTags)
{
	if (!IsValid(ASC))
		return {};

	TArray<bool> Result;
	for (auto GameplayTag : GameplayTags)
	{
		Result.Add(TryCancelAbilityWithTag(ASC, GameplayTag));
	}
	return Result;
}

int32 UUnrealHelperLibraryBPL::FireGameplayEvent(UAbilitySystemComponent* ASC, FGameplayTag EventTag, const FGameplayEventData& Payload) { return ASC->HandleGameplayEvent(EventTag, &Payload); }

FGameplayTag UUnrealHelperLibraryBPL::FindTagByString(const FString& TagString, bool bMatchPartialString)
{
	const UGameplayTagsManager& Manager = UGameplayTagsManager::Get();
	FGameplayTag Tag = Manager.RequestGameplayTag(FName(*TagString), false);

	if (!Tag.IsValid() && bMatchPartialString)
	{
		FGameplayTagContainer AllTags;
		Manager.RequestAllGameplayTags(AllTags, true);

		for (const FGameplayTag& TestTag : AllTags)
		{
			if (TestTag.ToString().Contains(TagString))
			{
				// UE_LOG(LogUnrealHelperLibrary, Display, TEXT("Could not find exact match for tag [%s] but found partial match on tag [%s]."), *TagString, *TestTag.ToString());
				Tag = TestTag;
				break;
			}
		}
	}

	return Tag;
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
		FVector TextLocation = (LineEnd - LineStart) / 2 + LineStart;

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

AActor* UUnrealHelperLibraryBPL::GetActorClosestToCenterOfScreen(UObject* WorldContextObject, const TArray<AActor*>& Actors, APlayerController* PlayerController, FVector WorldLocation, FVector2D& ScreenPosition, bool bPlayerViewportRelative, const bool bDebug, const float DebugLifetime)
{
	bool bRelativeToViewportCenter = false;
	AActor* Result = nullptr;
	FVector2D ResultScreenPosition = FVector2D(133700.322, 133700.322);
	float ResultDistance = FVector2D::Distance(ResultScreenPosition, FVector2D::ZeroVector);
	float ViewportScale = UWidgetLayoutLibrary::GetViewportScale(WorldContextObject);
	FVector2D ViewportSize = UWidgetLayoutLibrary::GetViewportSize(WorldContextObject);
	FVector2D ViewportSizeUnscaled = GetViewportSizeUnscaled(WorldContextObject);

	for (AActor* Actor : Actors)
	{
		FVector2D CurrentActorScreenPosition;
		UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(PlayerController, Actor->GetActorLocation(), CurrentActorScreenPosition, true);
		float CurrentDistance = FVector2D::Distance(CurrentActorScreenPosition, ViewportSizeUnscaled / 2);

		if (CurrentDistance < ResultDistance)
		{
			ResultDistance = CurrentDistance;
			Result = Actor;
		}

		if (bDebug)
		{
			FLineInfo LineInfo = {
				"TestLine" + Actor->GetName(),
				ViewportSize / 2,
				CurrentActorScreenPosition * ViewportScale,
				FColor::MakeRandomColor(),
				5,
				"" + Actor->GetName() + " " + FString::SanitizeFloat(CurrentDistance),
				FColor::Blue,
				bRelativeToViewportCenter,
				true,
			};
			DrawDebugLineOnCanvas(Actor->GetWorld(), LineInfo, bRelativeToViewportCenter);
		}
	}

	ScreenPosition = ResultScreenPosition;
	return Result;
}

AActor* UUnrealHelperLibraryBPL::GetMostDistantActor(const TArray<AActor*>& Actors, float& MaxDistance_Out, FVector Location, const bool bDebug, const float DebugLifetime)
{
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

float UUnrealHelperLibraryBPL::ConvertPercentToMultiplier(float Percent) { return (100.0f - Percent) / 100.0f; }

bool UUnrealHelperLibraryBPL::IsPreviewWorld(UObject* WorldContextObject) { return WorldContextObject->GetWorld()->IsPreviewWorld(); }

bool UUnrealHelperLibraryBPL::IsGameWorld(UObject* WorldContextObject) { return WorldContextObject->GetWorld()->IsGameWorld(); }

bool UUnrealHelperLibraryBPL::IsEditorWorld(UObject* WorldContextObject) { return WorldContextObject->GetWorld()->IsEditorWorld(); }

bool UUnrealHelperLibraryBPL::IsObjectInPreviewWorld(UObject* Object) { return Object->GetWorld()->IsPreviewWorld(); }

bool UUnrealHelperLibraryBPL::IsObjectInEditorWorld(UObject* Object) { return Object->GetWorld()->IsEditorWorld(); }

bool UUnrealHelperLibraryBPL::IsObjectInGameWorld(UObject* Object) { return Object->GetWorld()->IsGameWorld(); }

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

bool UUnrealHelperLibraryBPL::IsOtherCharacterInRange(ACharacter* Character, ACharacter* OtherCharacter, FFloatRange Range, bool bIncludeSelfCapsuleRadius, bool bIncludeTargetCapsuleRadius)
{
	if (!Character || !OtherCharacter)
		return false;

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

EBBValueType UUnrealHelperLibraryBPL::BlackboardKeyToBBValueType(FBlackboardKeySelector BlackboardKey)
{
	EBBValueType Result = EBBValueType::None;

	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Bool::StaticClass())
	{
		Result = EBBValueType::Bool;
	}
	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Int::StaticClass())
	{
		Result = EBBValueType::Int;
	}
	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Float::StaticClass())
	{
		Result = EBBValueType::Float;
	}
	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_String::StaticClass())
	{
		Result = EBBValueType::String;
	}
	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Name::StaticClass())
	{
		Result = EBBValueType::Name;
	}
	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
	{
		Result = EBBValueType::Vector;
	}
	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Rotator::StaticClass())
	{
		Result = EBBValueType::Rotator;
	}
	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Enum::StaticClass())
	{
		Result = EBBValueType::Enum;
	}
	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_NativeEnum::StaticClass())
	{
		Result = EBBValueType::NativeEnum;
	}
	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
	{
		Result = EBBValueType::Object;
	}
	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Class::StaticClass())
	{
		Result = EBBValueType::Class;
	}

	return Result;
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

bool UUnrealHelperLibraryBPL::IsUHLDebugCategoryEnabled(UObject* WorldContextObject, FGameplayTag DebugCategoryGameplayTag)
{
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
	if (!IsValid(GameInstance))
		return false;

	UUHLDebugSubsystem* UHLDebugSubsystem = GameInstance->GetSubsystem<UUHLDebugSubsystem>();
	if (!IsValid(UHLDebugSubsystem))
		return false;

	return UHLDebugSubsystem->IsCategoryEnabled(DebugCategoryGameplayTag);
}
