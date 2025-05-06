// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonMapsDeveloperSettings.generated.h"

USTRUCT(BlueprintType)
struct FCommonMapCategory
{
	GENERATED_BODY()

public:
	FCommonMapCategory() = default;
	explicit FCommonMapCategory(FName CategoryName_In, const TArray<FSoftObjectPath>& InPaths)
		: Name(CategoryName_In), Maps({ InPaths }) {};
	
	UPROPERTY(EditAnywhere, Category="")
	FName Name = "";

	UPROPERTY(EditAnywhere, Category="")
	bool bAutoSearchMapsInFolder = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ContentDir, EditCondition="bAutoSearchMapsInFolder", EditConditionHides))
	FDirectoryPath SearchFolder;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(EditCondition="bAutoSearchMapsInFolder", EditConditionHides))
	int32 SubmenuDepth = 1;
	
	UPROPERTY(EditAnywhere, Category="", meta=(AllowedClasses="/Script/Engine.World", TitleProperty="Name"))
	TArray<FSoftObjectPath> Maps = {};

	bool operator==(const FCommonMapCategory& Other) const
	{
		return Name == Other.Name;
	}
	bool operator==(const FName& Other) const
	{
		return Name == Other;
	}
};

/**
 * TODO: add rescan folders button
 */
UCLASS(config="Game", DefaultConfig, DisplayName="Common Maps")
class COMMONMAPS_API UCommonMapsDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	virtual FName GetCategoryName() const override { return FName("Plugins"); }
	
#if WITH_EDITORONLY_DATA
	UPROPERTY(Config,EditAnywhere, Category="Common Maps", meta=(ForceInlineRow))
	TArray<FCommonMapCategory> MapsCategories;
#endif
};
