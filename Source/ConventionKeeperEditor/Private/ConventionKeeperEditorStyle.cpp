// Pavel Penkov 2025 All Rights Reserved.

#include "ConventionKeeperEditorStyle.h"
#include "ConventionKeeperEditor.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Framework/Application/SlateApplication.h"
#include "Styling/SlateStyleRegistry.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FConventionKeeperEditorStyle::StyleInstance = nullptr;

void FConventionKeeperEditorStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FConventionKeeperEditorStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FConventionKeeperEditorStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("UnrealHelperEditorStyle"));
	return StyleSetName;
}


const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);

TSharedRef< FSlateStyleSet > FConventionKeeperEditorStyle::Create()
{
    TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("UnrealHelperEditorStyle"));
    Style->SetContentRoot(IPluginManager::Get().FindPlugin("UnrealHelperLibrary")->GetBaseDir() / TEXT("Resources"));
    // Style->SetContentRoot(FPaths::ProjectContentDir());

    // const UUHESettings* DeveloperSettings = GetDefault<UUHESettings>();
    // for (FUHECustomClassIconDescription CustomClassIcon : DeveloperSettings->CustomClassIcons)
    // {
    //     // FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
    //     FString Path = CustomClassIcon.Texture2D.GetLongPackageName();
    //     UObject* IconImageObject = LoadObject<UObject>(nullptr, *Path);
    //
    //     if (IsValid(IconImageObject))
    //     {
    //         UTexture2D* IconImage = Cast<UTexture2D>(IconImageObject);
    //         // FSlateDynamicImageBrush* DynamicImageBrush = new FSlateDynamicImageBrush(IconImage, Icon20x20, FName("CapsuleHitRegistrator"));
    //         FSlateImageBrush* ImageBrush = new FSlateImageBrush(IconImage, Icon20x20);
    //
    //     	TArray<TSubclassOf<UObject>> AllClasses = CustomClassIcon.Classes;
    //     	// support deprecated value
    //     	if (IsValid(CustomClassIcon.Class))
    //     	{
    //     		AllClasses.Add(CustomClassIcon.Class);
    //     	}
    //     	for (const TSubclassOf<UObject> Class : AllClasses)
    //     	{
    //     		FString ClassName = Class->GetName();
    //     		// Modify the class icons to use our new awesome icons
    //     		FString IconStyleName = FString::Printf(TEXT("ClassIcon.%s"), *ClassName);
    //     		Style->Set(FName(IconStyleName), ImageBrush);
    //     	}
    //     }
    // }

    Style->Set("UnrealHelperEditor.PluginAction", new IMAGE_BRUSH_SVG(TEXT("PlaceholderButtonIcon"), Icon20x20));
    return Style;
}

void FConventionKeeperEditorStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FConventionKeeperEditorStyle::Get()
{
	return *StyleInstance;
}
