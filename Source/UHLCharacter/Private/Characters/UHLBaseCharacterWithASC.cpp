// Pavel Penkov 2025 All Rights Reserved.


#include "Characters/UHLBaseCharacterWithASC.h"

#include "GameFramework/Controller.h"
#include "UHLAbilitySystemComponent.h"
#include "AttributeSets/UHLBaseCharacterAttributeSet.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UHLBaseCharacterWithASC)

AUHLBaseCharacterWithASC::AUHLBaseCharacterWithASC(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilitySystemComponent = CreateDefaultSubobject<UUHLAbilitySystemComponent>(TEXT("UHLAbilitySystem"));

    // some attribute sets can be added here, but its not only option
    // you can use UHL Settings to set defaults for every AbilitySystemComponent and add default AttributeSet
    // AbilitySystemComponent->AttributeSets = { UUHLBaseCharacterAttributeSet::StaticClass() };
}

void AUHLBaseCharacterWithASC::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (bInitUHLAbilitySystemOnPosses)
	{
	    if (bDontInitOnSameControllerTwice)
	    {
	        if (NewController != PreviousController.Get())
	        {
	            AbilitySystemComponent->InitAbilitySystem(NewController, this);
	            PreviousController = NewController;
	        }
	    }
	    else
	    {
	        AbilitySystemComponent->InitAbilitySystem(NewController, this);
	    }

	    // Advanced setup if you want to make something after attributes set, but before abilities activated
	    // AbilitySystemComponent->InitAbilitySystem(NewController, this, false);
	    // AbilitySystemComponent->ActivateInitialAbilities();
	}
}
