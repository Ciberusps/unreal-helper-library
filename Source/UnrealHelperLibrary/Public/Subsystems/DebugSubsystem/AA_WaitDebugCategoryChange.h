// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AA_WaitDebugCategoryChange.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitDebugCategoryChanged, bool, bEnabled);

/**
 * TODO test that it works after several DebugCategory changes
 */
UCLASS(BlueprintType, meta=(ExposedAsyncProxy = AsyncAction))
class UNREALHELPERLIBRARY_API UAA_WaitDebugCategoryChange : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
    // TODO add updating bool value from Blueprint by reference
    // UFUNCTION(BlueprintCallable, DisplayName="WaitDebugCategoryChange", meta=(WorldContext="WorldContext", BlueprintInternalUseOnly="true", AutoCreateRefTerm="bIsDebugEnabledValueRef", AdvancedDisplay="bIsDebugEnabledValueRef"))
    // static UAA_WaitDebugCategoryChange* WaitDebugCategoryChange(const UObject* WorldContext, FGameplayTag DebugCategoryTag, bool bCheckOnStart, const bool& bIsDebugEnabledValueRef);
    UFUNCTION(BlueprintCallable, Category="UnrealHelperLibrary", meta=(WorldContext="WorldContext", BlueprintInternalUseOnly="true", Keywords = "UnrealHelperLibrary debug UHL debugCategory debugging"))
    static UAA_WaitDebugCategoryChange* WaitDebugCategoryChange(const UObject* WorldContext, UPARAM(meta=(Categories="UHL.DebugCategory"))FGameplayTag DebugCategoryTag, bool bCheckOnStart = true);

    UPROPERTY(BlueprintAssignable)
    FWaitDebugCategoryChanged OnChange;
    UPROPERTY(BlueprintAssignable)
    FWaitDebugCategoryChanged OnEnabled;
    UPROPERTY(BlueprintAssignable)
    FWaitDebugCategoryChanged OnDisabled;

    virtual void Activate() override;

    virtual UWorld* GetWorld() const override
    {
        return ContextWorld.IsValid() ? ContextWorld.Get() : nullptr;
    }

private:
    TWeakObjectPtr<UWorld> ContextWorld = nullptr;

    FGameplayTag DebugCategoryTagInternal = FGameplayTag::EmptyTag;
    bool bCheckOnStart = true;

    UFUNCTION()
    void OnDebugCategoryChange(FGameplayTag DebugCategoryTag, bool bEnabled);
};
