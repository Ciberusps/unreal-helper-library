// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/UHLAbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "UHLBaseCharacter.generated.h"

class UAbilitySystemTestAttributeSet;

UCLASS()
class UNREALHELPERLIBRARY_API AUHLBaseCharacter : public ACharacter,
    public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AUHLBaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    virtual void PossessedBy(AController* NewController) override;

/** IAbilitySystemInterface **/
    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; };
    // virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return nullptr; };
/** ~IAbilitySystemInterface **/

protected:
/** GameplayAbilities */
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
    UUHLAbilitySystemComponent* AbilitySystemComponent;
    UPROPERTY()
    const UAbilitySystemTestAttributeSet* AbilitySystemTestAttributeSet;
/** ~GameplayAbilities */

};
