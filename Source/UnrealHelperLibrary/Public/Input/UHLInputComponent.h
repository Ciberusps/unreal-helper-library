// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UHLInputConfig.h"
#include "GameplayTagContainer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "UHLInputComponent.generated.h"

/**
 * WARNING: Copy-paste from "Lyra project" - /Source/Input/LyraInputConfig
 */

/**
 * UUHLInputComponent
 *
 *	Component used to manage input mappings and bindings using an input config data asset.
 */
UCLASS(Config = Input)
class UNREALHELPERLIBRARY_API UUHLInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
    UUHLInputComponent(const FObjectInitializer& ObjectInitializer);

    // void AddInputMappings(const UGCInputConfig* InputConfig, UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const;
    // void RemoveInputMappings(const UGCInputConfig* InputConfig, UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const;

    template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
    void BindAbilityActions(const UUHLInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles);

    // void RemoveBinds(TArray<uint32>& BindHandles);

    // TODO: вроде ненужно
    // void AddInputConfig(const FLoadedMappableConfigPair& ConfigPair, UEnhancedInputLocalPlayerSubsystem* InputSubsystem);
    // void RemoveInputConfig(const FLoadedMappableConfigPair& ConfigPair, UEnhancedInputLocalPlayerSubsystem* InputSubsystem);
};

template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
void UUHLInputComponent::BindAbilityActions(const UUHLInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles)
{
    check(InputConfig);

    for (const FUHLInputActionAbilities& Action : InputConfig->AbilityInputActions)
    {
        if (Action.InputAction && Action.bEnabled && !Action.AbilitiesToActivate.IsEmpty())
        {
            for (FGameplayTag AbilityTag : Action.AbilitiesToActivate)
            {
                if (PressedFunc)
                {
                    BindHandles.Add(BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, PressedFunc, AbilityTag).GetHandle());
                }

                if (ReleasedFunc)
                {
                    BindHandles.Add(BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, AbilityTag).GetHandle());
                    BindHandles.Add(BindAction(Action.InputAction, ETriggerEvent::Canceled, Object, ReleasedFunc, AbilityTag).GetHandle());
                }
            }
        }
    }
}
