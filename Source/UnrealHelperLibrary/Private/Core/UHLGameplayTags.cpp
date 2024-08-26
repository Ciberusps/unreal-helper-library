// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/UHLGameplayTags.h"

namespace UHLGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_AbilityInputBlocked, "Gameplay.AbilityInputBlocked");
	UE_DEFINE_GAMEPLAY_TAG(TAG_UHL_Debug_CheckGASGameplayTagsOnActor, "UHL.Debug.CheckGASGameplayTagsOnActor");

	// AbilityInputCache
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_UHL_AbilityInputCache_Add, "UHL.AbilityInputCache.Add", "Event for adding ability input cache, mostly used for melee combats");

	// Errors
	UE_DEFINE_GAMEPLAY_TAG(TAG_Error, "Error");
}

