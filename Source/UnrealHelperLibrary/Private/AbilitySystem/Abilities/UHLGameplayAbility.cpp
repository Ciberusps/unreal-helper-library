// Copyright (c) 2024 Pavel Penkov


#include "AbilitySystem/Abilities/UHLGameplayAbility.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/UHLAbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UHLGameplayAbility)

UUHLAbilitySystemComponent* UUHLGameplayAbility::GetUHLAbilitySystemComponentFromActorInfo() const
{
    if (!ensure(CurrentActorInfo))
    {
        return nullptr;
    }
    checkf(CastChecked<UUHLAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent.Get()), TEXT("UUHLGameplayAbility::GetUHLAbilitySystemComponentFromActorInfo can be used only on characters with UHLAbilitySystemComponent"));
    return StaticCast<UUHLAbilitySystemComponent*>(CurrentActorInfo->AbilitySystemComponent.Get());
}

void UUHLGameplayAbility::TryActivateAbilityOnSpawn(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) const
{
	const bool bIsPredicting = (Spec.ActivationInfo.ActivationMode == EGameplayAbilityActivationMode::Predicting);

	// Try to activate if activation policy is on spawn.
	if (ActorInfo && !Spec.IsActive() && !bIsPredicting && (ActivationPolicy == EUHLAbilityActivationPolicy::OnSpawn))
	{
		UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
		const AActor* AvatarActor = ActorInfo->AvatarActor.Get();

		// If avatar actor is torn off or about to die, don't try to activate until we get the new one.
		if (ASC && AvatarActor && !AvatarActor->GetTearOff() && (AvatarActor->GetLifeSpan() <= 0.0f))
		{
			const bool bIsLocalExecution = (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::LocalPredicted) || (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::LocalOnly);
			const bool bIsServerExecution = (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::ServerOnly) || (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::ServerInitiated);

			const bool bClientShouldActivate = ActorInfo->IsLocallyControlled() && bIsLocalExecution;
			const bool bServerShouldActivate = ActorInfo->IsNetAuthority() && bIsServerExecution;

			if (bClientShouldActivate || bServerShouldActivate)
			{
				ASC->TryActivateAbility(Spec.Handle);
			}
		}
	}
}

void UUHLGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
    Super::OnGiveAbility(ActorInfo, Spec);

    K2_OnAbilityAdded();

    TryActivateAbilityOnSpawn(ActorInfo, Spec);
}

void UUHLGameplayAbility::OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
    K2_OnAbilityRemoved();
    Super::OnRemoveAbility(ActorInfo, Spec);
}

void UUHLGameplayAbility::OnPawnAvatarSet()
{
    K2_OnPawnAvatarSet();
}
