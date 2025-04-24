#pragma once

#include "IPropertyTypeCustomization.h"

class FCommonMapCategoryCustomization : public IPropertyTypeCustomization
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();

	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow,
								 IPropertyTypeCustomizationUtils& CustomizationUtils) override;

	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle,
									IDetailChildrenBuilder& ChildBuilder,
									IPropertyTypeCustomizationUtils& CustomizationUtils) override;
};