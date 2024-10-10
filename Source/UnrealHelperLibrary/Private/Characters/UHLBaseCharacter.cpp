// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/UHLBaseCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UHLBaseCharacter)

// Sets default values
AUHLBaseCharacter::AUHLBaseCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
    // smooth AI rotation
    bUseControllerRotationYaw = false;
    GetCharacterMovement()->bOrientRotationToMovement = 0;
    GetCharacterMovement()->bUseControllerDesiredRotation = true;
}

FTurnSettings AUHLBaseCharacter::GetTurnSettings_Implementation() const
{
	return TurnSettingsDataAsset->TurnSettings;
}

