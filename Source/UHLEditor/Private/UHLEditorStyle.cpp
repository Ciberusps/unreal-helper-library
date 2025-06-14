// Pavel Penkov 2025 All Rights Reserved.

#include "UHLEditorStyle.h"
#include "UHLEditor.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Development/UHLEditorSettings.h"
#include "Framework/Application/SlateApplication.h"
#include "Styling/SlateStyleRegistry.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FUHLEditorStyle::StyleInstance = nullptr;

void FUHLEditorStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FUHLEditorStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FUHLEditorStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("UHLEditorStyle"));
	return StyleSetName;
}


const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);

TSharedRef< FSlateStyleSet > FUHLEditorStyle::Create()
{
    TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("UHLEditorStyle"));
    Style->SetContentRoot(IPluginManager::Get().FindPlugin("UnrealHelperLibrary")->GetBaseDir() / TEXT("Resources"));
    // Style->SetContentRoot(FPaths::ProjectContentDir());

    const UUHLEditorSettings* DeveloperSettings = GetDefault<UUHLEditorSettings>();
    for (FUHLEditorCustomClassIconDescription CustomClassIcon : DeveloperSettings->CustomClassIcons)
    {
        // FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
        FString Path = CustomClassIcon.Texture2D.GetLongPackageName();
        UObject* IconImageObject = LoadObject<UObject>(nullptr, *Path);

        if (IsValid(IconImageObject))
        {
            UTexture2D* IconImage = Cast<UTexture2D>(IconImageObject);
            // FSlateDynamicImageBrush* DynamicImageBrush = new FSlateDynamicImageBrush(IconImage, Icon20x20, FName("CapsuleHitRegistrator"));
            FSlateImageBrush* ImageBrush = new FSlateImageBrush(IconImage, Icon20x20);

        	TArray<TSubclassOf<UObject>> AllClasses = CustomClassIcon.Classes;
        	// support deprecated value
        	if (IsValid(CustomClassIcon.Class))
        	{
        		AllClasses.Add(CustomClassIcon.Class);
        	}
        	for (const TSubclassOf<UObject> Class : AllClasses)
        	{
        		if (!Class) continue;
        		FString ClassName = Class->GetName();
        		// Modify the class icons to use our new awesome icons
        		FString IconStyleName = FString::Printf(TEXT("ClassIcon.%s"), *ClassName);
        		Style->Set(FName(IconStyleName), ImageBrush);
        	}
        }
    }

    Style->Set("UHLEditor.PluginAction", new IMAGE_BRUSH_SVG(TEXT("PlaceholderButtonIcon"), Icon20x20));
    return Style;
}

void FUHLEditorStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FUHLEditorStyle::Get()
{
	return *StyleInstance;
}
