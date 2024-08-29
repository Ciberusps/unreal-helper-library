// Copyright Epic Games, Inc. All Rights Reserved.

#include "Utils/UnrealHelperLibraryBPL.h"

#include "AbilitySystemComponent.h"
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
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/KismetSystemLibrary.h"


FString UUnrealHelperLibraryBPL::GetProjectVersion()
{
	FString ProjectVersion;
	GConfig->GetString(
		TEXT("/Script/EngineSettings.GeneralProjectSettings"),
		TEXT("ProjectVersion"),
		ProjectVersion,
		GGameIni
	);
	return ProjectVersion;
}

void UUnrealHelperLibraryBPL::DebugPrintStrings(const FString& A, const FString& B, const FString& C,
	const FString& D, const FString& E, const FString& F, const FString& G, const FString& H, const FString& I,
	const FString& J, float Duration, const FName Key, const bool bEnabled)
{
	FString StringResult;
	StringResult.Empty(A.Len() + B.Len() + C.Len() + D.Len() + E.Len() + 1); // adding one for the string terminator
	StringResult += A;
	StringResult += B;
	StringResult += C;
	StringResult += D;
	StringResult += E;

	if (!bEnabled) return;

	UKismetSystemLibrary::PrintString(
		nullptr, StringResult,true, true,
		FLinearColor(0, 0.66, 1), Duration, Key
	);
}

void UUnrealHelperLibraryBPL::DrawDebugBar()
{
}

FGameplayEffectSpec UUnrealHelperLibraryBPL::CreateGenericGASGameplayEffectSpec(TSubclassOf<UGameplayEffect> GameplayEffectClass, AActor* HitInstigator, AActor* InEffectCauser,
    const FHitResult& HitResult, const UObject* SourceObject)
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
    if (!ASC) return;

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

TArray<FString> UUnrealHelperLibraryBPL::GetNamesOfComponentsOnObject(UObject* OwnerObject, UClass* Class)
{
    TArray<FString> Result = {};

    UBlueprintGeneratedClass* BlueprintGeneratedClass = OwnerObject->IsA<UBlueprintGeneratedClass>()
        ? Cast<UBlueprintGeneratedClass>(OwnerObject)
        : Cast<UBlueprintGeneratedClass>(OwnerObject->GetClass());
    if (!BlueprintGeneratedClass) return Result;

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

float UUnrealHelperLibraryBPL::RelativeAngleToActor(AActor* ActorRelativeToWhomAngleCalculated,
	AActor* TargetActor)
{
    if (!IsValid(ActorRelativeToWhomAngleCalculated) || !IsValid(TargetActor)) return FLOAT_ERROR;
	return UKismetAnimationLibrary::CalculateDirection(
		ActorRelativeToWhomAngleCalculated->GetActorLocation() - TargetActor->GetActorLocation(),
		(ActorRelativeToWhomAngleCalculated->GetActorForwardVector() * -1).ToOrientationRotator()
	);
}

float UUnrealHelperLibraryBPL::RelativeAngleToVector(AActor* ActorRelativeToWhomAngleCalculated, FVector TargetVector)
{
    if (!IsValid(ActorRelativeToWhomAngleCalculated)) return FLOAT_ERROR;
    return UKismetAnimationLibrary::CalculateDirection(
        ActorRelativeToWhomAngleCalculated->GetActorLocation() - TargetVector,
        (ActorRelativeToWhomAngleCalculated->GetActorForwardVector() * -1).ToOrientationRotator()
    );
}

UActorComponent* UUnrealHelperLibraryBPL::GetActorComponentByName(AActor* Actor, FString Name)
{
    if (!IsValid(Actor)) return nullptr;

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
    if (!IsValid(Actor)) return nullptr;

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
        UKismetSystemLibrary::LineTraceSingle(Component->GetWorld(), RandomPoint, FVector(0, 0, -999999),
            TraceTypeQuery1, false, TArray<AActor*>(),
            bDrawDebug ? EDrawDebugTrace::Type::ForDuration : EDrawDebugTrace::Type::None, OutHit,
            true, FLinearColor::Red, FLinearColor::Green, DebugDrawTime
        );
        if (OutHit.IsValidBlockingHit())
        {
            RandomPoint = OutHit.Location;
        }
    }

    return RandomPoint;
}

FVector UUnrealHelperLibraryBPL::GetHighestPointInBox(const USceneComponent* Component)
{
    if (!IsValid(Component)) return VECTOR_ERROR;

    FVector Origin;
    FVector BoxExtent;
    float SphereRadius = 0.0f;
    UKismetSystemLibrary::GetComponentBounds(Component, Origin, BoxExtent, SphereRadius);

    const FVector BoxMin = Origin - BoxExtent;
    const FVector BoxMax = Origin + BoxExtent;
    return FBox(BoxMin, BoxMax).Max;
}

FVector UUnrealHelperLibraryBPL::GetPointAtRelativeAngle(const AActor* ActorIn, const float Angle, const float Distance, const bool bDebug, const float DebugLifetime, const FLinearColor DebugColor)
{
    if (!IsValid(ActorIn)) return VECTOR_ERROR;
    FVector Result = ActorIn->GetActorLocation() + ActorIn->GetActorForwardVector().RotateAngleAxis(Angle, FVector(0, 0, 1)) * Distance;
    if (bDebug)
    {
        DrawDebugString(ActorIn->GetWorld(), Result, FString::Printf(TEXT("Angle %.2f\nDistance %.2f"), Angle, Distance), 0, DebugColor.ToFColor(true), DebugLifetime, true, 1.0f);
        DrawDebugSphere(ActorIn->GetWorld(), Result, 10.0f, 12, DebugColor.ToFColor(true), true, DebugLifetime, 0, 1);
        FVector ArrowLineEnd = ActorIn->GetActorLocation() + ActorIn->GetActorForwardVector().RotateAngleAxis(Angle, FVector(0, 0, 1)) * (Distance - 10);
        DrawDebugDirectionalArrow(ActorIn->GetWorld(), ActorIn->GetActorLocation(), ArrowLineEnd, RELATIVE_POINT_ARROW_SIZE, FColor::White, true, DebugLifetime, 0, 2);
    }
    return Result;
}

void UUnrealHelperLibraryBPL::GetPointAtRelativeDirection(FVector& Point, FRotator& PointRotation, const AActor* ActorIn, const EUHLDirection Direction, const float Distance, const bool bDebug, const float DebugLifetime, const FLinearColor DebugColor, const FText DebugText)
{
    if (!IsValid(ActorIn)) return;

    float Angle = DirectionToAngle(Direction);
    Point = GetPointAtRelativeAngle(ActorIn, Angle, Distance);
    PointRotation = (Point - ActorIn->GetActorLocation()).ToOrientationRotator();

    if (bDebug)
    {
        const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EUHLDirection"), true);
        if (EnumPtr)
        {
	        DrawDebugString(ActorIn->GetWorld(), Point, FString::Printf(TEXT("%s \nDirection %s\nDistance %.2f"), *DebugText.BuildSourceString(), *EnumPtr->GetNameStringByValue((uint8)Direction), Distance), 0, DebugColor.ToFColor(true), DebugLifetime, true, 1.0f);
        }
        DrawDebugSphere(ActorIn->GetWorld(), Point, 10.0f, 12, DebugColor.ToFColor(true), true, DebugLifetime, 0, 1);
        FVector ArrowLineEnd = GetPointAtRelativeAngle(ActorIn, Angle, Distance - 10);
        DrawDebugDirectionalArrow(ActorIn->GetWorld(), ActorIn->GetActorLocation(), ArrowLineEnd, RELATIVE_POINT_ARROW_SIZE, FColor::White, true, DebugLifetime, 0, 2);
    }
}

FVector UUnrealHelperLibraryBPL::GetPointAtAngleRelativeToOtherActor(const AActor* Actor1, const AActor* Actor2, const float Angle, const float Distance, const bool bTakeZFromActor1, const bool bDebug, const float DebugLifetime, const FLinearColor DebugColor)
{
    if (!IsValid(Actor1) || !IsValid(Actor2)) return VECTOR_ERROR;
    const FVector Actor1Location = Actor1->GetActorLocation();
    const FVector Actor2Location = Actor2->GetActorLocation();
    const FVector DirectionBetweenActors = (Actor2Location - Actor1Location).GetSafeNormal();
    FVector Result = Actor1Location + (DirectionBetweenActors.RotateAngleAxis(Angle, FVector(0, 0, 1)) * Distance);
    Result.Z = bTakeZFromActor1 ? Actor1Location.Z : Actor2Location.Z;
    if (bDebug)
    {
        DrawDebugString(Actor1->GetWorld(), Result, FString::Printf(TEXT("Angle %.2f\nDistance %.2f"), Angle, Distance), 0, DebugColor.ToFColor(true), DebugLifetime, true, 1.0f);
        DrawDebugSphere(Actor1->GetWorld(), Result, 10.0f, 12, DebugColor.ToFColor(true), true, DebugLifetime, 0, 1);
        FVector ArrowLineEnd = Actor1Location + (DirectionBetweenActors.RotateAngleAxis(Angle, FVector(0, 0, 1)) * (Distance - 10));
        Result.Z = bTakeZFromActor1 ? Actor1Location.Z : Actor2Location.Z;
        DrawDebugDirectionalArrow(Actor1->GetWorld(), Actor1->GetActorLocation(), Actor2->GetActorLocation(), RELATIVE_POINT_ARROW_SIZE, FColor::White, true, DebugLifetime, 0, 1);
        DrawDebugDirectionalArrow(Actor1->GetWorld(), Actor1->GetActorLocation(), ArrowLineEnd, RELATIVE_POINT_ARROW_SIZE, FColor::White, true, DebugLifetime, 0, 2);
    }
    return Result;
}

FVector UUnrealHelperLibraryBPL::GetPointAtDirectionRelativeToOtherActor(const AActor* Actor1, const AActor* Actor2, const EUHLDirection Direction, const float Distance, const bool bTakeZFromActor1, const bool bDebug, const float DebugLifetime, const FLinearColor DebugColor)
{
    if (!IsValid(Actor1) || !IsValid(Actor2)) return VECTOR_ERROR;
    float Angle = DirectionToAngle(Direction);
    FVector Result = GetPointAtAngleRelativeToOtherActor(Actor1, Actor2, Angle, Distance, bTakeZFromActor1);
    if (bDebug)
    {
        const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EUHLDirection"), true);
        if (EnumPtr)
        {
            DrawDebugString(Actor1->GetWorld(), Result, FString::Printf(TEXT("Direction %s\nDistance %.2f"), *EnumPtr->GetNameStringByValue((uint8)Direction), Distance), 0, DebugColor.ToFColor(true), DebugLifetime, true, 1.0f);
        }
        DrawDebugSphere(Actor1->GetWorld(), Result, 10.0f, 12, DebugColor.ToFColor(true), true, DebugLifetime, 0, 1);
        FVector ArrowLineEnd = GetPointAtAngleRelativeToOtherActor(Actor1, Actor2, Angle, Distance, bTakeZFromActor1);
        DrawDebugDirectionalArrow(Actor1->GetWorld(), Actor1->GetActorLocation(), Actor2->GetActorLocation(), RELATIVE_POINT_ARROW_SIZE, FColor::White, true, DebugLifetime, 0, 1);
        DrawDebugDirectionalArrow(Actor1->GetWorld(), Actor1->GetActorLocation(), ArrowLineEnd, RELATIVE_POINT_ARROW_SIZE, FColor::White, true, DebugLifetime, 0, 2);
    }
    return Result;
}

float UUnrealHelperLibraryBPL::DirectionToAngle(const EUHLDirection DirectionIn)
{
    if (DirectionIn == EUHLDirection::Front) return 0.0f;
    if (DirectionIn == EUHLDirection::Back) return 180.0f;
    if (DirectionIn == EUHLDirection::Left) return -90.0f;
    if (DirectionIn == EUHLDirection::Right) return 90.0f;
    if (DirectionIn == EUHLDirection::FrontLeft) return -45.0f;
    if (DirectionIn == EUHLDirection::FrontRight) return 45.0f;
    if (DirectionIn == EUHLDirection::BackLeft) return -135.0f;
    if (DirectionIn == EUHLDirection::BackRight) return 135.0f;
    return 0.0f;
}

float UUnrealHelperLibraryBPL::ConvertPercentToMultiplier(float Percent)
{
    return (100.0f - Percent) / 100.0f;
}

bool UUnrealHelperLibraryBPL::IsPreviewWorld(UObject* WorldContextObject)
{
    return WorldContextObject->GetWorld()->IsPreviewWorld();
}

bool UUnrealHelperLibraryBPL::IsGameWorld(UObject* WorldContextObject)
{
    return WorldContextObject->GetWorld()->IsGameWorld();
}

bool UUnrealHelperLibraryBPL::IsEditorWorld(UObject* WorldContextObject)
{
    return WorldContextObject->GetWorld()->IsEditorWorld();
}

bool UUnrealHelperLibraryBPL::IsObjectInPreviewWorld(UObject* Object)
{
    return Object->GetWorld()->IsPreviewWorld();
}

bool UUnrealHelperLibraryBPL::IsObjectInEditorWorld(UObject* Object)
{
    return Object->GetWorld()->IsEditorWorld();
}

bool UUnrealHelperLibraryBPL::IsObjectInGameWorld(UObject* Object)
{
    return Object->GetWorld()->IsGameWorld();
}

bool UUnrealHelperLibraryBPL::IsOtherActorInAngle(AActor* Actor, AActor* OtherActor, TArray<FFloatRange> Ranges)
{
    float RelativeAngle = RelativeAngleToActor(Actor, OtherActor);
    bool bInAngle = false;
    for (FFloatRange Range : Ranges)
    {
        bInAngle = UKismetMathLibrary::InRange_FloatFloat(RelativeAngle, Range.GetLowerBoundValue(), Range.GetUpperBoundValue(), true, true);
        if (bInAngle) break;
    }
    return bInAngle;
}

bool UUnrealHelperLibraryBPL::IsOtherCharacterInRange(ACharacter* Character, ACharacter* OtherCharacter, FFloatRange Range, bool bIncludeSelfCapsuleRadius, bool bIncludeTargetCapsuleRadius)
{
    if (!Character || !OtherCharacter) return false;

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

EBBValueType UUnrealHelperLibraryBPL::BlackboardKeyToBBValueType(
	FBlackboardKeySelector BlackboardKey)
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

