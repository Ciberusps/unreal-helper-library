// Pavel Penkov 2025 All Rights Reserved.

#include "UHLDebugBlueprintLibrary.h"

#include "GameplayTagsManager.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/ConfigCacheIni.h"
#include "Animation/AnimMontage.h"
#include "DrawDebugHelpers.h"
#include "UHLDebugSubsystem.h"
#include "Components/Widget.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UHLDebugBlueprintLibrary)

bool UUHLDebugBlueprintLibrary::IsUHLDebugCategoryEnabled(UObject* WorldContextObject, FGameplayTag DebugCategoryGameplayTag)
{
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
	if (!IsValid(GameInstance))
		return false;

	UUHLDebugSubsystem* UHLDebugSubsystem = GameInstance->GetSubsystem<UUHLDebugSubsystem>();
	if (!IsValid(UHLDebugSubsystem))
		return false;

	return UHLDebugSubsystem->IsCategoryEnabled(DebugCategoryGameplayTag);
}
