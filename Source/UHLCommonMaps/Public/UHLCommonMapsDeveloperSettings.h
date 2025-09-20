// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UHLCommonMapsDeveloperSettings.generated.h"

USTRUCT(BlueprintType)
struct FUHLCommonMapCategory
{
	GENERATED_BODY()

public:
	FUHLCommonMapCategory() = default;
	explicit FUHLCommonMapCategory(FName CategoryName_In, const TArray<FSoftObjectPath>& InPaths)
		: Name(CategoryName_In), Maps({ InPaths }) {};
	
	UPROPERTY(EditAnywhere, Category="CommonMapCategory")
	FName Name = "";

	UPROPERTY(EditAnywhere, Category="CommonMapCategory")
	bool bAutoSearchMapsInFolder = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CommonMapCategory", meta=(ContentDir, EditCondition="bAutoSearchMapsInFolder", EditConditionHides))
	FDirectoryPath SearchFolder;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CommonMapCategory", meta=(EditCondition="bAutoSearchMapsInFolder", EditConditionHides))
	int32 SubmenuDepth = 1;
	
	UPROPERTY(EditAnywhere, Category="CommonMapCategory", meta=(AllowedClasses="/Script/Engine.World", TitleProperty="Name"))
	TArray<FSoftObjectPath> Maps = {};

	bool operator==(const FUHLCommonMapCategory& Other) const
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
class UHLCOMMONMAPS_API UUHLCommonMapsDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	virtual FName GetCategoryName() const override { return FName("Plugins"); }
	
#if WITH_EDITORONLY_DATA
	UPROPERTY(Config,EditAnywhere, Category="Common Maps", meta=(ForceInlineRow))
	TArray<FUHLCommonMapCategory> MapsCategories;
#endif
};
