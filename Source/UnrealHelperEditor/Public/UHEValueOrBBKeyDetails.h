#pragma once

#include "GameplayTagContainer.h"
#include "ValueOrBBKeyDetails.h"

class FValueOrBBKeyDetails_GameplayTag : public FValueOrBBKeyDetails
{
public:
	/** Makes a new instance of this detail layout class for a specific detail view requesting it */
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();

	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils) override;

protected:
	virtual TSharedRef<SWidget> CreateDefaultValueWidget() override;

	void OnGameplayTagChanged(const FGameplayTag NewTag);
	FGameplayTag GetGameplayTag() const;
};

