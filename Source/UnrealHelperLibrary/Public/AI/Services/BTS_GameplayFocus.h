// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_DefaultFocus.h"
#include "BTS_GameplayFocus.generated.h"

/**
 * Prevents rotation jittering while moving to enemy
 */
UCLASS(Category = "UnrealHelperLibrary")
class UNREALHELPERLIBRARY_API UBTS_GameplayFocus : public UBTService_DefaultFocus
{
	GENERATED_BODY()

public:
    UBTS_GameplayFocus(const FObjectInitializer& ObjectInitializer);

#if UE_VERSION_NEWER_THAN(5, 4, 0)
    virtual uint16 GetInstanceMemorySize() const override { return sizeof(FBTFocusMemory); }
    virtual void InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType) const override;
    virtual void CleanupMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryClear::Type CleanupType) const override;
#endif
};
