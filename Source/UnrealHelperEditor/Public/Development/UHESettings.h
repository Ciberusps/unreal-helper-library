// Copyright (c) 2024 Pavel Penkov

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettingsBackedByCVars.h"
#include "UHESettings.generated.h"

USTRUCT(BlueprintType)
struct UNREALHELPEREDITOR_API FUHECustomClassIconDescription
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CustomClassIconDescription")
    TSoftObjectPtr<UTexture2D> Texture2D;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CustomClassIconDescription")
    TSubclassOf<UObject> Class;
};

/**
 *
 */
UCLASS(Config="Editor", DefaultConfig, meta = (DisplayName="UnrealHelperEditor Settings"))
class UNREALHELPEREDITOR_API UUHESettings : public UDeveloperSettingsBackedByCVars
{
	GENERATED_BODY()

public:
    UPROPERTY(config, EditAnywhere, Category="Custom Class Icons", meta=(FullyExpand))
    TArray<FUHECustomClassIconDescription> CustomClassIcons;
};
