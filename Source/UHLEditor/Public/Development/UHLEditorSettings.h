// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UHLEditorSettings.generated.h"

USTRUCT(BlueprintType)
struct UHLEDITOR_API FUHLEditorCustomClassIconDescription
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CustomClassIconDescription")
    TSoftObjectPtr<UTexture2D> Texture2D;
	// deprecated, TODO: remove
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CustomClassIconDescription", meta=(DeprecatedProperty, DeprecationMessage="Deprecated use Classes"))
    TSubclassOf<UObject> Class;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CustomClassIconDescription")
	TArray<TSubclassOf<UObject>> Classes;
};

/**
 *
 */
UCLASS(Config="Editor", DefaultConfig, meta = (DisplayName="UHL Editor"))
class UHLEDITOR_API UUHLEditorSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
    UPROPERTY(config, EditAnywhere, Category="Custom Class Icons", meta=(FullyExpand))
    TArray<FUHLEditorCustomClassIconDescription> CustomClassIcons;

	UPROPERTY(config)
	bool bMigrationFromOldSettingsDone = false;
};
