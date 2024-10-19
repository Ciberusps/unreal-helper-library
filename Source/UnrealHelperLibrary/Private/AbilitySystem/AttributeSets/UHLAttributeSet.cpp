// Copyright (c) 2024 Pavel Penkov


#include "AbilitySystem/AttributeSets/UHLAttributeSet.h"

#include "AbilitySystem/UHLAbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UHLAttributeSet)

UUHLAttributeSet::UUHLAttributeSet()
{
}

UWorld* UUHLAttributeSet::GetWorld() const
{
    const UObject* Outer = GetOuter();
    check(Outer);

    return Outer->GetWorld();
}

UUHLAbilitySystemComponent* UUHLAttributeSet::GetUHLAbilitySystemComponent() const
{
    return Cast<UUHLAbilitySystemComponent>(GetOwningAbilitySystemComponent());
}
