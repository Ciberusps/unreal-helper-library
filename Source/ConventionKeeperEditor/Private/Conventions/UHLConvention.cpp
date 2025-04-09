// Pavel Penkov 2025 All Rights Reserved.


#include "Conventions/UHLConvention.h"
#include "NamingConventions/PascalCaseNamingConvention.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UHLConvention)

UUHLConvention::UUHLConvention()
{
	Name = "UHL Convention";
	NamingConvention = UPascalCaseNamingConvention::StaticClass();

	FFolderStructure FolderStructure = {};
	FolderStructure.FolderPath = FDirectoryPath("/Game");
	FolderStructure.RequiredFolders.Add(FDirectoryPath("/Game/3rdParty"));
	FolderStructure.RequiredFolders.Add(FDirectoryPath("/Game/{ProjectName}"));
	FolderStructure.RequiredFolders.Add(FDirectoryPath("/Game/Bogatyr"));
	FolderStructures.Add(FolderStructure);
}