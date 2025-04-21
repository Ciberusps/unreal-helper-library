// Copyright (c) 2024 NextGenium


#include "ConventionKeeperBlueprintLibrary.h"

void UConventionKeeperBlueprintLibrary::MakePathRelativeToProjectDir(FString& InPath)
{
	const FString ProjectRoot = FPaths::ProjectDir();
	if (FPaths::MakePathRelativeTo(InPath, *ProjectRoot))
	{
		// Ensure we still have a trailing slash
		if (!InPath.EndsWith(TEXT("/")))
		{
			InPath += TEXT("/");
		}
	}
}

void UConventionKeeperBlueprintLibrary::GetDiskFoldersRelativeToRoot(
	const FString& DirectoryPath, TArray<FString>& FolderPaths)
{
	const FString ProjectRoot = FPaths::ProjectDir();
	
	// Ensure InPath ends with a slash
    FString Path = ProjectRoot / DirectoryPath;
    if (!Path.EndsWith(TEXT("/")) && !Path.EndsWith(TEXT("\\")))
    {
        Path += TEXT("/");
    }

    // FindFiles parameters: (OutResult, Directory, Files, Directories)
    // Passing false for Files, true for Directories
    IFileManager::Get().FindFiles(FolderPaths, *Path, /*Files=*/false, /*Directories=*/true);

    // Each entry in OutFolders is just the folder name (no path). If you want full paths:
    for (int32 i = 0; i < FolderPaths.Num(); ++i)
    {
    	FString PathRelativeToRoot = Path / FolderPaths[i]; 
        FPaths::MakePathRelativeTo(PathRelativeToRoot, *ProjectRoot);
        FolderPaths[i] = PathRelativeToRoot;
    }
}