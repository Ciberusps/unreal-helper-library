// Copyright (c) 2024 Pavel Penkov

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/UHLAbilitySystemComponent.h"
#include "Characters/UHLBaseCharacter.h"
#include "UHLBaseCharacterWithASC.generated.h"

class UAbilitySystemComponent;

/**
 * Extended version of UHLBaseCharacter but with AbilitySystem and ASC init on "PossessedBy"
 */
UCLASS()
class UNREALHELPERLIBRARY_API AUHLBaseCharacterWithASC : public AUHLBaseCharacter,
	public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AUHLBaseCharacterWithASC(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bInitUHLAbilitySystemOnPosses = true;
	
	/** IAbilitySystemInterface **/
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; };
	/** ~IAbilitySystemInterface **/
	
protected:
	/** GameplayAbilities */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	UUHLAbilitySystemComponent* AbilitySystemComponent;
	/** ~GameplayAbilities */

	virtual void PossessedBy(AController* NewController) override;
};
