// Pavel Penkov 2025 All Rights Reserved.


#include "Conventions/UHLConvention.h"
#include "NamingConventions/PascalCaseNamingConvention.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UHLConvention)

UUHLConvention::UUHLConvention()
{
	Name = "UHL Convention";
	NamingConvention = UPascalCaseNamingConvention::StaticClass();

	FFolderStructure ContentFolderStructure = {};
	ContentFolderStructure.FolderPath = FDirectoryPath("Content/");
	ContentFolderStructure.RequiredFolders.Add(FDirectoryPath("Content/3rdParty/"));
	ContentFolderStructure.RequiredFolders.Add(FDirectoryPath("Content/{ProjectName}/"));
	// Should be added only if we have FMOD enabled
	ContentFolderStructure.RequiredFolders.Add(FDirectoryPath("Content/FMOD/"));

	// move to optional folders or exclusions ???
	ContentFolderStructure.RequiredFolders.Add(FDirectoryPath("Content/Movies/"));
	FolderStructures.Add(ContentFolderStructure);

	FFolderStructure ProjectNameFolderFolderStructure = {};
	ProjectNameFolderFolderStructure.FolderPath = FDirectoryPath("Content/{ProjectName}/");
	ProjectNameFolderFolderStructure.RequiredFolders.Add(FDirectoryPath("Content/{ProjectName}/Characters"));
	ProjectNameFolderFolderStructure.RequiredFolders.Add(FDirectoryPath("Content/{ProjectName}/Maps"));
	ProjectNameFolderFolderStructure.RequiredFolders.Add(FDirectoryPath("Content/{ProjectName}/Core"));
	ProjectNameFolderFolderStructure.RequiredFolders.Add(FDirectoryPath("Content/{ProjectName}/SFX"));
	ProjectNameFolderFolderStructure.RequiredFolders.Add(FDirectoryPath("Content/{ProjectName}/VFX"));
	ProjectNameFolderFolderStructure.RequiredFolders.Add(FDirectoryPath("Content/{ProjectName}/AI"));
	FolderStructures.Add(ProjectNameFolderFolderStructure);

	FFolderStructure CharacterFolderStructure = {};
	CharacterFolderStructure.FolderPath = FDirectoryPath("Content/{ProjectName}/Characters/{CharacterName}/");
	CharacterFolderStructure.RequiredFolders.Add(FDirectoryPath("Content/{ProjectName}/Characters/{CharacterName}/AI"));
	CharacterFolderStructure.RequiredFolders.Add(FDirectoryPath("Content/{ProjectName}/Characters/{CharacterName}/Projectiles"));
	CharacterFolderStructure.RequiredFolders.Add(FDirectoryPath("Content/{ProjectName}/Characters/{CharacterName}/Animations"));
	CharacterFolderStructure.RequiredFolders.Add(FDirectoryPath("Content/{ProjectName}/Characters/{CharacterName}/Gyms"));
	CharacterFolderStructure.RequiredFolders.Add(FDirectoryPath("Content/{ProjectName}/Characters/{CharacterName}/Data"));
	CharacterFolderStructure.RequiredFolders.Add(FDirectoryPath("Content/{ProjectName}/Characters/{CharacterName}/Abilities"));
	CharacterFolderStructure.RequiredFolders.Add(FDirectoryPath("Content/{ProjectName}/Characters/{CharacterName}/Materials"));
	FolderStructures.Add(CharacterFolderStructure);

	FFolderStructure CoreFolderStructure = {};
	CoreFolderStructure.FolderPath = FDirectoryPath("Content/{ProjectName}/Core/AI/");
	FolderStructures.Add(CoreFolderStructure);
}