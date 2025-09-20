// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "CoreGlobals.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "GameplayTagContainer.h"
#include "UHLDebugCategory.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UHLDebugSystemSubsystem.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUHLDebugCategoryChanged, FGameplayTag, DebugCategoryTag, bool, bEnabled);

UCLASS()
class UHLDEBUGSYSTEM_API UUHLDebugSystemSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
    UUHLDebugSystemSubsystem();

    FOnUHLDebugCategoryChanged OnDebugCategoryChanged;

    // TODO filter gameplayTags, allow using only values from DebugCategories `meta=(Categories="")`
    // probably "Godreaper.DebugCategories." required, or "ProjectName.DebugCategories"...
    UFUNCTION(Exec, BlueprintCallable, Category="UHLDebugSubsystem", meta=(Categories = "UHL.DebugCategory,DebugCategory"))
    bool IsCategoryEnabled(const FGameplayTag DebugCategoryTag) const;

	UFUNCTION(Exec, BlueprintCallable, Category="UHLDebugSubsystem", meta=(Categories = "UHL.DebugCategory,DebugCategory"))
    void EnableDebugCategory(const FGameplayTag DebugCategoryTag, bool bEnable);
    
    UFUNCTION(Exec, BlueprintCallable, Category="UHLDebugSubsystem", meta=(Categories = "UHL.DebugCategory,DebugCategory"))
    void ToggleDebugCategory(const FGameplayTag DebugCategoryTag);

	UFUNCTION(BlueprintCallable, Category="UHLDebugSubsystem", meta=(Categories = "UHL.DebugCategory,DebugCategory"))
	const FUHLDebugCategory& GetDebugCategoryByTag(const FGameplayTag DebugCategoryTag) const;
	
    const TArray<FUHLDebugCategory>& GetDebugCategories() const { return DebugCategories; }

protected:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

private:
    bool bSetupped = false;
    bool bSetUpCategoriesThatRequiresPlayerController = false;
    bool bIsSetuping = true;

    UPROPERTY()
    TArray<FUHLDebugCategory> DebugCategories = {};

	FDelegateHandle ActorSpawnedDelegateHandle;

	// waiting for world initialize to wait PlayerController spawned for InitDebugCategories
	void OnPostWorldInit(UWorld* InWorld, const UWorld::InitializationValues IVS);

	// waiting for world teardown to not waiting for PlayerController spawn until next world inited 
	UFUNCTION()
	void OnWorldBeginTearDown(UWorld* World);

	// waiting for PlayerController spawned to InitDebugCategories that requires PlayerController
	UFUNCTION()
	void OnActorSpawned(AActor* SpawnedActor);

	// should be called when player controller available, e.g. in PlayerController.BeginPlay
	void SetUpCategoriesThatRequiresPlayerController();
};
