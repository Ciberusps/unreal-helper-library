// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/UHLBaseCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/UHLAbilitySystemComponent.h"
#include "AbilitySystem/AttributeSets/UHLBaseCharacterAttributeSet.h"
#include "GameFramework/CharacterMovementComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UHLBaseCharacter)

// Sets default values
AUHLBaseCharacter::AUHLBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    AbilitySystemComponent = CreateDefaultSubobject<UUHLAbilitySystemComponent>(TEXT("UHLAbilitySystem"));

    // smooth AI rotation
    bUseControllerRotationYaw = false;
    GetCharacterMovement()->bOrientRotationToMovement = 0;
    GetCharacterMovement()->bUseControllerDesiredRotation = true;
}

// Called when the game starts or when spawned
void AUHLBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AUHLBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AUHLBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AUHLBaseCharacter::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    AbilitySystemComponent->InitAbilitySystem(NewController, this);
    if (IsValid(AbilitySystemComponent))
    {
        AbilitySystemTestAttributeSet = AbilitySystemComponent->AddSet<UUHLBaseCharacterAttributeSet>();
    }
    AbilitySystemComponent->InitAttributes();
    AbilitySystemComponent->ActivateInitialAbilities();
}

// void AUHLBaseCharacter::GetTurnSettings_Implementation(FTurnSettings& TurnSettings) const
// {
//     TurnSettings = TurnSettingsDataAsset->TurnSettings;
// }

