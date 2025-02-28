// Copyright (c) 2024 Pavel Penkov

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/UHLAbilitySystemComponent.h"
#include "Characters/UHLBaseCharacter.h"
#include "Core/UHLAbilitySystemInterface.h"
#include "UHLBaseCharacterWithASC.generated.h"

class UAbilitySystemComponent;

/**
 * Extended version of UHLBaseCharacter but with AbilitySystem and ASC init on "PossessedBy"
 */
UCLASS()
class UNREALHELPERLIBRARY_API AUHLBaseCharacterWithASC : public AUHLBaseCharacter,
                                                         public IAbilitySystemInterface,
                                                         public IUHLAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AUHLBaseCharacterWithASC(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UHL BaseCharacter with ASC")
	bool bInitUHLAbilitySystemOnPosses = true;
	
	/** IAbilitySystemInterface **/
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; };
	/** ~IAbilitySystemInterface **/

	/** IUHLAbilitySystemInterface */
	virtual UUHLAbilitySystemComponent* GetUHLAbilitySystemComponent_Implementation() const override
	{
		return AbilitySystemComponent;
	};
	/** ~IUHLAbilitySystemInterface */
	
protected:
	/** GameplayAbilities */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="UHL BaseCharacter with ASC")
	UUHLAbilitySystemComponent* AbilitySystemComponent;
	/** ~GameplayAbilities */

	virtual void PossessedBy(AController* NewController) override;
};
