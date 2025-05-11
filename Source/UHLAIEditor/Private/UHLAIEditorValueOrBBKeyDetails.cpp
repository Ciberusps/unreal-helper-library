// Pavel Penkov 2025 All Rights Reserved.

#include "UHLAIEditorValueOrBBKeyDetails.h"

#include "GameplayTagContainer.h"
#include "IDetailChildrenBuilder.h"
#include "SGameplayTagCombo.h"

TSharedRef<IPropertyTypeCustomization> FValueOrBBKeyDetails_GameplayTag::MakeInstance()
{
	return MakeShareable(new FValueOrBBKeyDetails_GameplayTag);
}

void FValueOrBBKeyDetails_GameplayTag::CustomizeChildren(TSharedRef<class IPropertyHandle> StructPropertyHandle, class IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	if (GameplayTagProperty && GameplayTagProperty->IsEditable())
	{
		StructBuilder.AddProperty(GameplayTagProperty.ToSharedRef());
	}
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
	if (DefaultValueProperty.IsValid())
	{
		FGameplayTag TagToSet = NewTag;
		FString TagAsString = TagToSet.ToString();
		const FPropertyAccess::Result Result = DefaultValueProperty->SetValueFromFormattedString(TagAsString);
		if (Result != FPropertyAccess::Success)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to set GameplayTag value on property handle."));
		}
	}
}

FGameplayTag FValueOrBBKeyDetails_GameplayTag::GetGameplayTag() const
{
	FGameplayTag* GameplayTag = nullptr;
	void* StructData = nullptr;
	if (DefaultValueProperty->GetValueData(StructData) == FPropertyAccess::Success)
	{
		GameplayTag = reinterpret_cast<FGameplayTag*>(StructData);
	}
	return *GameplayTag;
}
