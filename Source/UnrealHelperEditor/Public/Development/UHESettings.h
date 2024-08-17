// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettingsBackedByCVars.h"
#include "UHESettings.generated.h"

USTRUCT(BlueprintType)
struct UNREALHELPEREDITOR_API FUHECustomClassIconDescription
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSoftObjectPtr<UTexture2D> Texture2D;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
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
    UPROPERTY(config, EditAnywhere, meta=(FullyExpand))
    TArray<FUHECustomClassIconDescription> CustomClassIcons;
};
