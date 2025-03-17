#include "UHEValueOrBBKeyDetails.h"

#include "GameplayTagContainer.h"
#include "IDetailChildrenBuilder.h"
#include "IDetailPropertyRow.h"
#include "SGameplayTagCombo.h"

TSharedRef<IPropertyTypeCustomization> FValueOrBBKeyDetails_GameplayTag::MakeInstance()
{
	return MakeShareable(new FValueOrBBKeyDetails_GameplayTag);
}

void FValueOrBBKeyDetails_GameplayTag::CustomizeChildren(
	TSharedRef<IPropertyHandle> StructPropertyHandle, IDetailChildrenBuilder& StructBuilder,
	IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	FValueOrBBKeyDetails::CustomizeChildren(StructPropertyHandle, StructBuilder, StructCustomizationUtils);
	StructBuilder.AddProperty(DefaultValueProperty.ToSharedRef())
	.IsEnabled(TAttribute<bool>(this, &FValueOrBBKeyDetails::CanEditDefaultValue));
}

TSharedRef<SWidget> FValueOrBBKeyDetails_GameplayTag::CreateDefaultValueWidget()
{
	return SNew(SGameplayTagCombo)
		// .Visibility(this, &SGraphPin::GetDefaultValueVisibility)
		// .Filter(FilterString)
		.Tag(this, &FValueOrBBKeyDetails_GameplayTag::GetGameplayTag)
		.OnTagChanged(this, &FValueOrBBKeyDetails_GameplayTag::OnGameplayTagChanged);
}

void FValueOrBBKeyDetails_GameplayTag::OnGameplayTagChanged(const FGameplayTag NewTag)
{
	DefaultValueProperty->SetValue(NewTag.GetTagName());
}

FGameplayTag FValueOrBBKeyDetails_GameplayTag::GetGameplayTag() const
{
	FName GameplayTagValue = NAME_None;
	DefaultValueProperty->GetValue(GameplayTagValue);
	return FGameplayTag::RequestGameplayTag(GameplayTagValue, false);
}
