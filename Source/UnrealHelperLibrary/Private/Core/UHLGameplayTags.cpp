// Pavel Penkov 2025 All Rights Reserved.


#include "Core/UHLGameplayTags.h"

namespace UHLGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_AbilityInputBlocked, "Gameplay.AbilityInputBlocked");
	UE_DEFINE_GAMEPLAY_TAG(TAG_UHL_Debug_CheckGASGameplayTagsOnActor, "UHL.Debug.CheckGASGameplayTagsOnActor");

	// Abilities
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_UHL_Abilities_Character_GlobalTimeDilation, "UHL.Abilities.Character.GlobalTimeDilation", "");

	// AbilityInputCache
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_UHL_AbilityInputCache_Catching, "UHL.AbilityInputCache.Catching", "Opened for catching actions to AbilityInputCache");

	// Debug Categories
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_UHL_DebugCategory_AbilitySystem, "UHL.DebugCategory.AbilitySystem", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_UHL_DebugCategory_AbilitySystem_Attributes, "UHL.DebugCategory.AbilitySystem.Attributes", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_UHL_DebugCategory_AbilitySystem_Effects, "UHL.DebugCategory.AbilitySystem.Effects", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_UHL_DebugCategory_AbilitySystem_Abilities, "UHL.DebugCategory.AbilitySystem.Abilities", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_UHL_DebugCategory_AbilitySystem_DebugBasicHUD, "UHL.DebugCategory.AbilitySystem.DebugBasicHUD", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_UHL_DebugCategory_AbilitySystem_DebugAbilityTags, "UHL.DebugCategory.AbilitySystem.DebugAbilityTags", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_UHL_DebugCategory_InputSystem_EnhancedInputSystem, "UHL.DebugCategory.InputSystem.EnhancedInputSystem", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_UHL_DebugCategory_InputSystem_AbilityInputCache, "UHL.DebugCategory.InputSystem.AbilityInputCache", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_UHL_DebugCategory_Collisions, "UHL.DebugCategory.Collisions", "");

	// Errors
	UE_DEFINE_GAMEPLAY_TAG(TAG_Error, "Error");
}

