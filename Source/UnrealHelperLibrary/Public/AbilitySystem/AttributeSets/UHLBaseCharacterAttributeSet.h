// Copyright (c) 2024 Pavel Penkov

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "UHLAttributeSet.h"
#include "UHLBaseCharacterAttributeSet.generated.h"

/**
 *
 */
UCLASS(Category = "UnrealHelperLibrary")
class UNREALHELPERLIBRARY_API UUHLBaseCharacterAttributeSet : public UUHLAttributeSet
{
	GENERATED_BODY()

public:
	ATTRIBUTE_ACCESSORS(UUHLBaseCharacterAttributeSet, Health);
	ATTRIBUTE_ACCESSORS(UUHLBaseCharacterAttributeSet, MaxHealth);

protected:
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;

	// Helper function to proportionally adjust the value of an attribute when it's associated max attribute changes.
	// (i.e. When MaxHealth increases, Health increases by an amount that maintains the same percentage as before)
	void AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty);
    virtual void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const;

private:
    UPROPERTY(BlueprintReadOnly, Category = "Attributes | Health", Meta=(AllowPrivateAccess=true))
    FGameplayAttributeData Health;
    UPROPERTY(BlueprintReadOnly, Category = "Attributes | Health", Meta=(AllowPrivateAccess=true))
    FGameplayAttributeData MaxHealth;
};
