// CommonMapCategoryCustomization.cpp

#include "CommonMapCategoryCustomization.h"

#include "CommonMapsDeveloperSettings.h"
#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "Widgets/Input/SDirectoryPicker.h"
#include "Misc/Paths.h"
#include "PropertyHandle.h"

#define LOCTEXT_NAMESPACE "CommonMapCategoryCustomization"

TSharedRef<IPropertyTypeCustomization> FCommonMapCategoryCustomization::MakeInstance()
{
	return MakeShareable(new FCommonMapCategoryCustomization);
}

void FCommonMapCategoryCustomization::CustomizeHeader(
	TSharedRef<IPropertyHandle> PropertyHandle,
	FDetailWidgetRow& HeaderRow,
	IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	// You can leave the header empty or show the struct name
	HeaderRow.NameContent()[ PropertyHandle->CreatePropertyNameWidget() ];
}

void FCommonMapCategoryCustomization::CustomizeChildren(
	TSharedRef<IPropertyHandle> StructPropertyHandle,
	IDetailChildrenBuilder& ChildBuilder,
	IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	uint32 NumChildren = 0;
	StructPropertyHandle->GetNumChildren(NumChildren);

	for (uint32 i = 0; i < NumChildren; ++i)
	{
		TSharedPtr<IPropertyHandle> ChildHandle = StructPropertyHandle->GetChildHandle(i);
		if (!ChildHandle.IsValid()) continue;

		FName PropName = ChildHandle->GetProperty()->GetFName();
		if (PropName == GET_MEMBER_NAME_CHECKED(FCommonMapCategory, SearchFolder))
		{
			ChildBuilder.AddCustomRow(LOCTEXT("SearchFolder", "Search Folder"))
			.NameContent()[ ChildHandle->CreatePropertyNameWidget() ]
			.ValueContent()
			.MinDesiredWidth(400)
			[
				SNew(SDirectoryPicker)
				.Directory(FPaths::ProjectDir())
				.OnDirectoryChanged_Lambda([ChildHandle](const FString& PickedPath)
				{
					FString Relative = PickedPath;
					FPaths::MakePathRelativeTo(Relative, *FPaths::ProjectDir());
					ChildHandle->SetValue(Relative);
				})
			];
		}
		else
		{
			// Let other fields render normally
			ChildBuilder.AddProperty(ChildHandle.ToSharedRef());
		}
	}
}

#undef LOCTEXT_NAMESPACE
