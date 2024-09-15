// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UHLDebugSubsystem.h"
#include "Engine/CancellableAsyncAction.h"
#include "AA_WaitDebugCategoryChange.generated.h"

/**
 * TODO test that it works after several DebugCategory changes
 */
UCLASS()
class UNREALHELPERLIBRARY_API UAA_WaitDebugCategoryChange : public UCancellableAsyncAction
{
	GENERATED_BODY()

public:
    /**
     * Start a delay.
     * @param DelayTime		How long the delay will last.
     * @param WorldContext	Object from which the world will be derived.
     */
    UFUNCTION(BlueprintCallable, DisplayName="WaitDebugCategoryChange", meta=(WorldContext="WorldContext", BlueprintInternalUseOnly="true"))
    static UAA_WaitDebugCategoryChange* WaitDebugCategoryChange(const UObject* WorldContext, FGameplayTag DebugCategory);

    UPROPERTY(BlueprintAssignable)
    FOnUHLDebugCategoryChanged OnChange;
    UPROPERTY(BlueprintAssignable)
    FOnUHLDebugCategoryChanged OnEnabled;
    UPROPERTY(BlueprintAssignable)
    FOnUHLDebugCategoryChanged OnDisabled;

    virtual void Activate() override;
    virtual void Cancel() override;

    // Start UObject Functions
    virtual UWorld* GetWorld() const override
    {
        return ContextWorld.IsValid() ? ContextWorld.Get() : nullptr;
    }
    // End UObject Functions

private:
    /** The context world of this action. */
    TWeakObjectPtr<UWorld> ContextWorld = nullptr;

    FGameplayTag DebugCategory = FGameplayTag::EmptyTag;

    UFUNCTION()
    void OnDebugCategoryChange(FGameplayTag DebugCategoryTag, bool bEnabled);
};
