// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/UHLBaseCharacterWithASC.h"

#include "AbilitySystem/UHLAbilitySystemComponent.h"
#include "AbilitySystem/AttributeSets/UHLBaseCharacterAttributeSet.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UHLBaseCharacterWithASC)

AUHLBaseCharacterWithASC::AUHLBaseCharacterWithASC(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilitySystemComponent = CreateDefaultSubobject<UUHLAbilitySystemComponent>(TEXT("UHLAbilitySystem"));
}

void AUHLBaseCharacterWithASC::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (bInitUHLAbilitySystemOnPosses)
	{
		AbilitySystemComponent->InitAbilitySystem(NewController, this);
		AbilitySystemComponent->InitAttributes();
		AbilitySystemComponent->ActivateInitialAbilities();
	}
}