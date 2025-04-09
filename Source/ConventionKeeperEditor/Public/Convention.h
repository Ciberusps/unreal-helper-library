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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDirectoryPath FolderPath = {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(RelativePath))
	TArray<FDirectoryPath> RequiredFolders = {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bOtherFoldersNotAllowed = true;
};


UCLASS(BlueprintType, DefaultToInstanced, EditInlineNew)
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
};
