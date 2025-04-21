// Pavel Penkov 2025 All Rights Reserved.


#include "Convention.h"

#include "ConventionKeeperBlueprintLibrary.h"
#include "Development/ConventionKeeperSettings.h"
#include "Misc/Paths.h"

#define LOCTEXT_NAMESPACE "FConventionKeeperEditorModule"

#include UE_INLINE_GENERATED_CPP_BY_NAME(Convention)

void FFolderStructure::ConvertAllPathsToRelativePaths()
{
	UConventionKeeperBlueprintLibrary::MakePathRelativeToProjectDir(FolderPath.Path);

	for (FDirectoryPath& Dir : RequiredFolders)
	{
		UConventionKeeperBlueprintLibrary::MakePathRelativeToProjectDir(Dir.Path);
	}
}

#if WITH_EDITOR
void UConvention::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// Loop over every struct in our array...
	for (FFolderStructure& FolderStruct : FolderStructures)
	{
		FolderStruct.ConvertAllPathsToRelativePaths();
	}
}
#endif

void UConvention::ValidateFolderStructures_Implementation()
{
	FMessageLog MyMessageLog = FMessageLog(TEXT("ConventionKeeper"));
	MyMessageLog.NewPage(FText::FromString("Starting a new logging session..."));

	const UConventionKeeperSettings* ConventionKeeperSettings = GetDefault<UConventionKeeperSettings>();
	TMap<FString, FString> Placeholders = ConventionKeeperSettings->Placeholders;
	
	for (const FFolderStructure& FolderStructure : FolderStructures)
	{
		// bool bExists = CheckAssetPathExists(TEXT("/Game/Bogatyr"));
		bool bExists = DoesDirectoryExist(FolderStructure.FolderPath.Path, Placeholders);
		MyMessageLog.Message(
				bExists ? EMessageSeverity::Type::Info : EMessageSeverity::Type::Error,
		FText::Format(LOCTEXT("ConventionKeeperEditor","Path {0} exists? {1}"), FText::FromString(FolderStructure.FolderPath.Path), bExists)
		);

		// validate that required folders exists
		for (const FDirectoryPath& RequiredFolder : FolderStructure.RequiredFolders)
		{
			bool bRequiredFOlderExists = DoesDirectoryExist(RequiredFolder.Path, Placeholders);
			MyMessageLog.Message(
				bRequiredFOlderExists ? EMessageSeverity::Type::Info : EMessageSeverity::Type::Error,
				FText::Format(LOCTEXT("ConventionKeeperEditor","Path {0} exists? {1}"), FText::FromString(RequiredFolder.Path), bRequiredFOlderExists)
			);
		}

		// validate that other folders not exists
		if (FolderStructure.bOtherFoldersNotAllowed)
		{
			TArray<FString> AllFoldersInThisPath = {};
			UConventionKeeperBlueprintLibrary::GetDiskFoldersRelativeToRoot(FolderStructure.FolderPath.Path, AllFoldersInThisPath);

			for (const FString& Folder : AllFoldersInThisPath)
			{
				bool bFolderAlreadyInRequiredFolders = false;
				for (const FDirectoryPath& RequiredFolder : FolderStructure.RequiredFolders)
				{
					bool bIsSameFolder = FPaths::IsSamePath(Folder, RequiredFolder.Path);
					if (bIsSameFolder)
					{
						bFolderAlreadyInRequiredFolders = true;
						break;
					};
				}

				if (!bFolderAlreadyInRequiredFolders)
				{
					MyMessageLog.Error(FText::Format(LOCTEXT("ConventionKeeperEditor","Other folders not allowed in {0}"), FText::FromString(FolderStructure.FolderPath.Path)));		
				}
			}
		}
	}

	MyMessageLog.Open();
}

bool UConvention::DoesDirectoryExist(const FString& DirectoryPath, const TMap<FString, FString>& Placeholders)
{
	FString ResultPath = DirectoryPath;
	for (TTuple<FString, FString> Placeholder : Placeholders)
	{
		ResultPath = ResultPath.Replace(*Placeholder.Key, *Placeholder.Value, ESearchCase::CaseSensitive);
	}
	
	FString AbsolutePath = FPaths::ProjectDir() / ResultPath;
	return FPaths::DirectoryExists(AbsolutePath);
}
