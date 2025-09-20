// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "GameplayEffect.h"
#include "UHLAbilitySystemComponent.h"
#include "AssetRegistry/AssetData.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UHLGASBlueprintLibrary.generated.h"

class UWidget;
struct FBlackboardKeySelector;

// TODO improve
// - validations if Actor1/Actors2 is nullptr - DebugPrintString it
// - keywords for every method

/*
 *	Function library class.
 *	Each function in it is expected to be static and represents blueprint node that can be called in any blueprint.
 *
 *	When declaring function you can define metadata for the node. Key function specifiers will be BlueprintPure and BlueprintCallable.
 *	BlueprintPure - means the function does not affect the owning object in any way and thus creates a node without Exec pins.
 *	BlueprintCallable - makes a function which can be executed in Blueprints - Thus it has Exec pins.
 *	DisplayName - full name of the node, shown when you mouse over the node and in the blueprint drop down menu.
 *				Its lets you name the node using characters not allowed in C++ function names.
 *	CompactNodeTitle - the word(s) that appear on the node.
 *	Keywords -	the list of keywords that helps you to find node when you search for it using Blueprint drop-down menu.
 *				Good example is "Print String" node which you can find also by using keyword "log".
 *	Category -	the category your node will be under in the Blueprint drop-down menu.
 *
 *	For more info on custom blueprint nodes visit documentation:
 *	https://wiki.unrealengine.com/Custom_Blueprint_Node_Creation
 */
UCLASS()
class UHLGAS_API UUHLGASBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** Tries to find an ability system component on the actor, will use AbilitySystemInterface or fall back to a component search */
	UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary", Meta=(DefaultToSelf = "Actor"))
	static UUHLAbilitySystemComponent* GetUHLAbilitySystemComponent(AActor* Actor);

	UFUNCTION(BlueprintCallable, Category = "UnrealHelperLibrary|GAS", meta = (Keywords = "UnrealHelperLibrary debug GAS Development"))
	static FGameplayEffectSpec CreateGenericGASGameplayEffectSpec(
		TSubclassOf<UGameplayEffect> GameplayEffectClass, AActor* HitInstigator, AActor* InEffectCauser, const FHitResult& HitResult, const UObject* SourceObject);

	/**
	*   UUnrealHelperLibraryBPL::UpdateStateGameplayTags(ASC, GetMovementComponent()->Velocity.Length() > 0,
			UHLGameplayTags::TAG_Character_State_IsMoving,
			UHLGameplayTags::TAG_Character_State_IsIdling);
		bCondition = true ? TAG_Character_State_IsMoving : TAG_Character_State_IsIdling

		UUnrealHelperLibraryBPL::UpdateStateGameplayTags(ASC, CharacterMovementVector.Length() > 0.4f,
			UHLGameplayTags::TAG_Character_State_HasMoveInput,
			FGameplayTag::EmptyTag);
		bCondition = true ? TAG_Character_State_HasMoveInput : NONE
	 */
	UFUNCTION(BlueprintCallable, Category = "UnrealHelperLibrary|GAS", meta = (Keywords = "UnrealHelperLibrary ability"))
	static void UpdateStateGameplayTags(UAbilitySystemComponent* ASC, bool bCondition, FGameplayTag PositiveConditionTag, FGameplayTag NegativeConditionTag, bool bShouldReplicate = false);

	UFUNCTION(BlueprintCallable, Category = "UnrealHelperLibrary|GAS", meta = (Keywords = "UnrealHelperLibrary ability"))
	static bool IsAbilityActiveByTag(const UAbilitySystemComponent* ASC, FGameplayTag GameplayTag);

	UFUNCTION(BlueprintCallable, Category = "UnrealHelperLibrary|GAS", meta = (Keywords = "UnrealHelperLibrary ability"))
	static bool TryActivateAbilityWithTag(UAbilitySystemComponent* ASC, FGameplayTag GameplayTag, bool bAllowRemoteActivation);

	UFUNCTION(BlueprintCallable, Category = "UnrealHelperLibrary|GAS", meta = (Keywords = "UnrealHelperLibrary ability"))
	static bool TryCancelAbilityWithTag(UAbilitySystemComponent* ASC, FGameplayTag GameplayTag);

	UFUNCTION(BlueprintCallable, Category = "UnrealHelperLibrary|GAS", meta = (Keywords = "UnrealHelperLibrary ability"))
	static bool ToggleAbilityWithTag(UAbilitySystemComponent* ASC, FGameplayTag GameplayTag, bool bAllowRemoteActivation);

	UFUNCTION(BlueprintCallable, Category = "UnrealHelperLibrary|GAS", meta = (Keywords = "UnrealHelperLibrary ability"))
	static TArray<bool> TryCancelAbilitiesWithTags(UAbilitySystemComponent* ASC, TArray<FGameplayTag> GameplayTags);

	UFUNCTION(BlueprintCallable, Category = "UnrealHelperLibrary|GAS", meta = (Keywords = "UnrealHelperLibrary ability event fire"))
	static int32 FireGameplayEvent(UAbilitySystemComponent* ASC, FGameplayTag EventTag, const FGameplayEventData& Payload);

	// "FGameplayTag::RequestGameplayTag()" crashes build on start, use this to find tag
	// function from Lyra
	UFUNCTION(BlueprintCallable, Category = "UnrealHelperLibrary|GAS", meta = (Keywords = "UnrealHelperLibrary gameplaytag tag"))
	static FGameplayTag FindTagByString(const FString& TagString, bool bMatchPartialString = false);

	UFUNCTION(BlueprintCallable, Category = "UnrealHelperLibrary|GAS", meta = (Keywords = "UnrealHelperLibrary gameplaytag tag"))
	static void FindAbilitySpecByTag(const TArray<FGameplayAbilitySpec>& AbilitiesSpecsContainer, FGameplayTag TagToFind, FGameplayAbilitySpec& AbilitySpec_Out);

	UFUNCTION(BlueprintCallable, Category = "UnrealHelperLibrary|GAS", meta = (Keywords = "UnrealHelperLibrary gameplaytag tag"))
	static void FindAbilitySpecByTagUsingASC(UAbilitySystemComponent* ASC, FGameplayTag TagToFind, FGameplayAbilitySpec& AbilitySpec_Out);
};
