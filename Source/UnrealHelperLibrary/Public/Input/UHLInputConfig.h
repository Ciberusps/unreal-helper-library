// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "UHLInputConfig.generated.h"

/**
 * WARNING: Copy paste from "Lyra project" - /Source/Input/LyraInputConfig
 */

class UInputAction;
class UInputMappingContext;
// class ULyraLocalPlayer;

/**
 * FGCInputAction
 *
 *	Struct used to map a input action to a gameplay input tag.
 */
USTRUCT(BlueprintType)
struct FUHLInputAction
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "InputAction")
	const UInputAction* InputAction = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "InputAction", Meta = (Categories = "InputTag"))
	FGameplayTag InputTag;
};

/**
 * FGCInputActionAbilities
 *
 *	Struct used to map a input action to a gameplay abilities tags
 */
USTRUCT(BlueprintType)
struct FUHLInputActionAbilities
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "InputActionAbilities")
	bool bEnabled = true;
	UPROPERTY(EditDefaultsOnly, Category = "InputActionAbilities")
	const UInputAction* InputAction = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "InputActionAbilities")
	TArray<FGameplayTag> AbilitiesToActivate;
};

/**
 * ULyraInputConfig
 *
 *	Non-mutable data asset that contains input configuration properties.
 */
UCLASS(BlueprintType, Const)
class UNREALHELPERLIBRARY_API UUHLInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	UUHLInputConfig(const FObjectInitializer& ObjectInitializer);

	// const UInputAction* FindNativeInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = true) const;
	// const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = true) const;

public:
	// List of input actions used by the owner.  These input actions are mapped to a gameplay tag and must be manually bound.
	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "InputAction"))
	// TArray<FGCInputAction> NativeInputActions;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Native Input Actions")
	FUHLInputAction NativeInputAction_Move;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Native Input Actions")
	FUHLInputAction NativeInputAction_LookMouse;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Native Input Actions")
	FUHLInputAction NativeInputAction_LookStick;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Native Input Actions")
	FUHLInputAction NativeInputAction_Interact;

	// List of input actions used by the owner.  These input actions are mapped to a gameplay tag and are automatically bound to abilities with matching input tags.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilityInputActions", Meta = (TitleProperty = "InputAction"))
	TArray<FUHLInputActionAbilities> AbilityInputActions;
};
