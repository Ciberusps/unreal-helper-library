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
    FVector Origin;
    FVector BoxExtent;
    float SphereRadius = 0.0f;
    UKismetSystemLibrary::GetComponentBounds(Component, Origin, BoxExtent, SphereRadius);

    const FVector BoxMin = Origin - BoxExtent;
    const FVector BoxMax = Origin + BoxExtent;
    return FBox(BoxMin, BoxMax).Max;
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

