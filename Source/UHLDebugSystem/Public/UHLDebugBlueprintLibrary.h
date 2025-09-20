// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "GameplayEffect.h"
#include "AssetRegistry/AssetData.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UHLDebugBlueprintLibrary.generated.h"

UCLASS()
class UHLDEBUGSYSTEM_API UUHLDebugBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** DebugSubsystem **/
	UFUNCTION(
		BlueprintPure, Category = "UnrealHelperLibrary|Debug", meta = (Categories = "UHL.DebugCategory,DebugCategory", WorldContext = "WorldContextObject", Keywords = "UnrealHelperLibrary debug"))
	static bool IsUHLDebugCategoryEnabled(UObject* WorldContextObject, FGameplayTag DebugCategoryGameplayTag);
	/** ~DebugSubsystem **/
};
