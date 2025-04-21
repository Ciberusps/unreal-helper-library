// Copyright (c) 2024 NextGenium

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ConventionKeeperBlueprintLibrary.generated.h"

/**
 * 
 */
UCLASS()
class CONVENTIONKEEPEREDITOR_API UConventionKeeperBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static void MakePathRelativeToProjectDir(FString& InPath);
	static void GetDiskFoldersRelativeToRoot(const FString& DirectoryPath, TArray<FString>& FolderPaths);
	static void ConvertToFullPath(FString& InPath);
};
