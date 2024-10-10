// Copyright Epic Games, Inc. All Rights Reserved.

#include "Input/UHLInputConfig.h"
// #include "LyraLogChannels.h"
// #include "InputMappingContext.h"
// #include "Settings/LyraSettingsLocal.h"
// #include "Player/LyraLocalPlayer.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UHLInputConfig)

UUHLInputConfig::UUHLInputConfig(const FObjectInitializer& ObjectInitializer)
{
}

// const UInputAction* UGCInputConfig::FindNativeInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
// {
// 	for (const FGCInputAction& Action : NativeInputActions)
// 	{
// 		if (Action.InputAction && (Action.InputTag == InputTag))
// 		{
// 			return Action.InputAction;
// 		}
// 	}
//
// 	if (bLogNotFound)
// 	{
// 		UE_LOG(LogTemp, Error, TEXT("Can't find NativeInputAction for InputTag [%s] on InputConfig [%s]."), *InputTag.ToString(), *GetNameSafe(this));
// 	}
//
// 	return nullptr;
// }

// const UInputAction* UGCInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
// {
// 	for (const FGCInputActionAbilities& Action : AbilityInputActions)
// 	{
// 		if (Action.InputAction && (Action.AbilityTag == InputTag))
// 		{
// 			return Action.InputAction;
// 		}
// 	}
//
// 	if (bLogNotFound)
// 	{
// 		UE_LOG(LogTemp, Error, TEXT("Can't find AbilityInputAction for InputTag [%s] on InputConfig [%s]."), *InputTag.ToString(), *GetNameSafe(this));
// 	}
//
// 	return nullptr;
// }
