// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AttributeSets/UHLAttributeSet.h"

#include "AbilitySystem/UHLAbilitySystemComponent.h"

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
