// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/UHLAbilitySystemComponent.h"
#include "Actors/UHLActorSettings.h"
#include "GameFramework/Character.h"
#include "UHLBaseCharacter.generated.h"

class UUHLBaseCharacterAttributeSet;

UCLASS()
class UNREALHELPERLIBRARY_API AUHLBaseCharacter : public ACharacter,
    public IAbilitySystemInterface,
	public IUHLActorSettings
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AUHLBaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTurnSettingsDataAsset* TurnSettingsDataAsset;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    virtual void PossessedBy(AController* NewController) override;

/** IAbilitySystemInterface **/
    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; };
    // virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return nullptr; };
/** ~IAbilitySystemInterface **/

/** IUHLActorSettings */
    // virtual void GetTurnSettings_Implementation(FTurnSettings& TurnSettings) const override;
/** ~IUHLActorSettings */

protected:
/** GameplayAbilities */
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
    UUHLAbilitySystemComponent* AbilitySystemComponent;
    UPROPERTY()
    const UUHLBaseCharacterAttributeSet* AbilitySystemTestAttributeSet;
/** ~GameplayAbilities */

};
