// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonMapsDeveloperSettings.generated.h"

USTRUCT(BlueprintType)
struct FCommonMap
{
	GENERATED_BODY()

public:
	FCommonMap() = default;
	explicit FCommonMap(const TArray<FSoftObjectPath>& InPaths) : MapURL(InPaths) {}

	UPROPERTY(EditAnywhere, Category="", meta=(AllowedClasses="/Script/Engine.World"))
	TSet<FSoftObjectPath> MapURL;
};

USTRUCT(BlueprintType)
struct FCommonMapCategory
{
	GENERATED_BODY()

public:
	FCommonMapCategory() = default;
	explicit FCommonMapCategory(FName CategoryName_In, const TArray<FSoftObjectPath>& InPaths)
		: CategoryName(CategoryName_In), Maps({ FCommonMap(InPaths) }) {};
	
	UPROPERTY(EditAnywhere, Category="")
	FName CategoryName = "";

	UPROPERTY(EditAnywhere, Category="")
	bool bSearchMapsInFolder = false;
	
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="", meta=(RelativePath, EditCondition="bSearchMapsInFolder", EditConditionHides))
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(RelativePath))
	FDirectoryPath SearchFolder;

	UPROPERTY(EditAnywhere, Category="")
	TArray<FCommonMap> Maps = {};

	bool operator==(const FCommonMapCategory& Other) const
	{
		return CategoryName == Other.CategoryName;
	}
	bool operator==(const FName& Other) const
	{
		return CategoryName == Other;
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
	UPROPERTY(Config,EditAnywhere, Category="Common Maps|MapsCategories", meta=(ForceInlineRow))
	TArray<FCommonMapCategory> MapsCategories;
#endif
};
