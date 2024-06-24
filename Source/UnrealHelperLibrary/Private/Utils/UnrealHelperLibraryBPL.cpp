// Copyright Epic Games, Inc. All Rights Reserved.

#include "Utils/UnrealHelperLibraryBPL.h"

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
#include "Engine/SCS_Node.h"
#include "Engine/SimpleConstructionScript.h"
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
	return UKismetAnimationLibrary::CalculateDirection(
		ActorRelativeToWhomAngleCalculated->GetActorLocation() - TargetActor->GetActorLocation(),
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

FVector UUnrealHelperLibraryBPL::GetHighestPoint(const USceneComponent* Component)
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

FVector UUnrealHelperLibraryBPL::GetPointAtRelativeAngle(const AActor* ActorIn, const float Angle, const float Distance)
{
    if (!IsValid(ActorIn)) return VECTOR_ERROR;
    return ActorIn->GetActorLocation() + ActorIn->GetActorForwardVector().RotateAngleAxis(Angle, FVector(0, 0, 1)) * Distance;
}

FVector UUnrealHelperLibraryBPL::GetPointAtRelativeDirection(const AActor* ActorIn, const EUHLDirection Direction, const float Distance)
{
    if (!IsValid(ActorIn)) return VECTOR_ERROR;
    float Angle = DirectionToAngle(Direction);
    return GetPointAtRelativeAngle(ActorIn, Angle, Distance);
}

FVector UUnrealHelperLibraryBPL::GetPointAtRelativeAngleBetweenActors(const AActor* Actor1, const AActor* Actor2, const float Angle, const float Distance, const bool bTakeZFromActor1, const bool bDebug)
{
    if (!IsValid(Actor1) || !IsValid(Actor2)) return VECTOR_ERROR;
    const FVector Actor1Location = Actor1->GetActorLocation();
    const FVector Actor2Location = Actor2->GetActorLocation();
    const FVector DirectionBetweenActors = (Actor2Location - Actor1Location).GetSafeNormal();
    FVector Result = Actor1Location + (DirectionBetweenActors.RotateAngleAxis(Angle, FVector(0, 0, 1)) * Distance);
    Result.Z = bTakeZFromActor1 ? Actor1Location.Z : Actor2Location.Z;
    if (bDebug)
    {
        DrawDebugLine(Actor1->GetWorld(), Actor1Location, Result, FColor::Blue, true, -1, 0, 1);
        DrawDebugSphere(Actor1->GetWorld(), Result, 5, 12, FColor::Red, true, - 1, 0, 1);
    }
    return Result;
}

FVector UUnrealHelperLibraryBPL::GetPointAtRelativeDirectionBetweenActors(const AActor* Actor1, const AActor* Actor2, const EUHLDirection Direction, const float Distance, const bool bTakeZFromActor1, const bool bDebug)
{
    if (!IsValid(Actor1) || !IsValid(Actor2)) return VECTOR_ERROR;
    float Angle = DirectionToAngle(Direction);
    FVector Result = GetPointAtRelativeAngleBetweenActors(Actor1, Actor2, Angle, Distance, bTakeZFromActor1, bDebug);
    if (bDebug)
    {
        DebugPrintStrings(FString::Printf(TEXT("Angle - %f"), Angle));
	    // DrawDebugString(Actor1->GetWorld(), Result, FString::Printf(TEXT("Direction %s")));
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

