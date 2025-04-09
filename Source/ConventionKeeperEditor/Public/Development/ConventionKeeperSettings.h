// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Convention.h"
#include "Conventions/UHLConvention.h"
#include "ConventionKeeperSettings.generated.h"


// USTRUCT(BlueprintType)
// struct UHLCONVENTIONKEEPEREDITOR_API FUHECustomClassIconDescription
// {
//     GENERATED_BODY()
//
//     UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CustomClassIconDescription")
//     TSoftObjectPtr<UTexture2D> Texture2D;
// 	// deprecated, TODO: remove
//     UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CustomClassIconDescription", meta=(DeprecatedProperty, DeprecationMessage="Deprecated use Classes"))
//     TSubclassOf<UObject> Class;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CustomClassIconDescription")
// 	TArray<TSubclassOf<UObject>> Classes;
// };

/**
 *
 */
UCLASS(Config="Editor", DefaultConfig, meta = (DisplayName="ConventionKeeper"))
class CONVENTIONKEEPEREDITOR_API UConventionKeeperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere)
	TSubclassOf<UConvention> Convention = UUHLConvention::StaticClass();

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, meta=(RelativePath))
	TArray<FString> ExceptionFolders = {
		"MyCustomFOlder2323",
		"RetardedFolderForMyDrawings1337",
	};

	//~UDeveloperSettings interface
	virtual FName GetCategoryName() const override { return FApp::GetProjectName(); };
	//~End of UDeveloperSettings interface
};
