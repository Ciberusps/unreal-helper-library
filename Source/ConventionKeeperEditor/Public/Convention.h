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
	// can be templated if started with "{Template_" prefix(explicitly)
	// but mostly we can identify that its template by just checking all ""
	// templated means that we will look for all folders in this folder
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
	// UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// bool bTemplatedFolder = false;

	void ConvertAllPathsToRelativePaths();
};


UCLASS(Blueprintable, BlueprintType, DefaultToInstanced, EditInlineNew)
class CONVENTIONKEEPEREDITOR_API UConvention : public UObject
{
	GENERATED_BODY()

public:
	// should introspect all templates recursively
	// e.g. we can have `/{ProjectName}/{CharacterName}/{SomethingName}`
	// {ProjectName} - placeholder, {CharacterName} and {SomethingName} are templates
	/**
	 * Scans a single path string for segments of the form "{Name}",
	 * ignores any in GlobalPlaceholders, and returns the rest.
	 */
	UFUNCTION()
	static TSet<FString> ExtractTemplatesFromPath(
		const FString& Path,
		const TMap<FString, FString>& GlobalPlaceholders
	);

	/**
	 * Given a “templated” path (absolute or relative) with placeholders like
	 * "{SomeTemplate}", this will:
	 *  1) split off the leading literal segments (e.g. "/RootFolder"),
	 *  2) for each placeholder segment, recurse into each matching subdirectory
	 *     of the current set of paths,
	 *  3) accumulate *all* matched folder‐paths at each placeholder level,
	 *  4) return that list.
	 *
	 * Example:
	 *   Pattern = "/RootFolder/{A}/{B}"
	 *   Disk contains:
	 *     /RootFolder/Foo
	 *     /RootFolder/Bar
	 *     /RootFolder/Foo/X
	 *     /RootFolder/Foo/Y
	 *
	 *   ResolveTemplatePaths(Pattern) ->
	 *     [ "/RootFolder/Foo",
	 *       "/RootFolder/Bar",
	 *       "/RootFolder/Foo/X",
	 *       "/RootFolder/Foo/Y" ]
	 */
	UFUNCTION()
	static TArray<FString> ResolveTemplatePaths(
		const FString& FullPattern,
		const TMap<FString, FString>& GlobalPlaceholders
	);
	
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
	
};
