// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "NamingConvention.h"
#include "Convention.generated.h"

USTRUCT(BlueprintType)
struct FFolderStructure
{
	GENERATED_BODY()

public:
	// should be relative to ProjectDir
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDirectoryPath FolderPath = {};

	// should be relative to FolderPath
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(RelativePath))
	TArray<FDirectoryPath> RequiredFolders = {};

	// TODO: optional folders like "/Content/Movies"?????

	// should not exist 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(RelativePath))
	TArray<FDirectoryPath> BannedFolders = {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bOtherFoldersNotAllowed = true;

	// means that name of this folder is template
	// TODO: decide is it implicit behavior or explicit
	// if implicit when we detect {Template_Character} and mark it as templated folder
	// if explicit when we need to set this bool
	// but if have chain of templates probably that should be implicit by default
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bTemplatedFolder = false;

	void ConvertAllPathsToRelativePaths();
};


UCLASS(Blueprintable, BlueprintType, DefaultToInstanced, EditInlineNew)
class CONVENTIONKEEPEREDITOR_API UConvention : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FFolderStructure> FolderStructures = {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UNamingConvention> NamingConvention;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ValidateFolderStructures();
	void ValidateFolderStructures_Implementation();

	bool DoesDirectoryExist(const FString& DirectoryPath, const TMap<FString, FString>& Placeholders);

private:
	
};
