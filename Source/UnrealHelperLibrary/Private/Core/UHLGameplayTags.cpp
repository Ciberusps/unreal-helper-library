// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/UHLGameplayTags.h"

namespace UHLGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_AbilityInputBlocked, "Gameplay.AbilityInputBlocked");
	UE_DEFINE_GAMEPLAY_TAG(TAG_UHL_Debug_CheckGASGameplayTagsOnActor, "UHL.Debug.CheckGASGameplayTagsOnActor");

	// AbilityInputCache
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_UHL_AbilityInputCache_Catching, "UHL.AbilityInputCache.Catching", "Opened for catching actions to AbilityInputCache");

	// Errors
	UE_DEFINE_GAMEPLAY_TAG(TAG_Error, "Error");
}

