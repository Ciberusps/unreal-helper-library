// Pavel Penkov 2025 All Rights Reserved.


#include "AttributeSets/UHLAttributeSet.h"

#include "UHLAbilitySystemComponent.h"

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
