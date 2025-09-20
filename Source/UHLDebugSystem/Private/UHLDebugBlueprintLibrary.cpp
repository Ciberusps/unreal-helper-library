// Pavel Penkov 2025 All Rights Reserved.

#include "UHLDebugBlueprintLibrary.h"

#include "GameplayTagsManager.h"
#include "Misc/ConfigCacheIni.h"
#include "Animation/AnimMontage.h"
#include "DrawDebugHelpers.h"
#include "UHLDebugSystemSubsystem.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UHLDebugBlueprintLibrary)

bool UUHLDebugBlueprintLibrary::IsUHLDebugCategoryEnabled(UObject* WorldContextObject, FGameplayTag DebugCategoryGameplayTag)
{
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
	if (!IsValid(GameInstance))
	{
		return false;
	}

	UUHLDebugSystemSubsystem* UHLDebugSubsystem = GameInstance->GetSubsystem<UUHLDebugSystemSubsystem>();
	if (!IsValid(UHLDebugSubsystem))
	{
		return false;
	}

	return UHLDebugSubsystem->IsCategoryEnabled(DebugCategoryGameplayTag);
}
