// ConventionSpec.cpp

#include "Convention.h"
#include "Misc/AutomationTest.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFilemanager.h"
#include "HAL/FileManager.h"

#if WITH_DEV_AUTOMATION_TESTS

// 1) Define the spec
BEGIN_DEFINE_SPEC(ConventionSpec,
                  "ConventionKeeper.Convention.Spec",
                  EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
    
    /** Root folder for each test */
    FString RootDir;

    /** Placeholders to ignore */
    TMap<FString, FString> GlobalPlaceholders;

	/** File manager shortcut */
	IFileManager& FM = IFileManager::Get();

END_DEFINE_SPEC(ConventionSpec)

void ConventionSpec::Define()
{

    Describe("ExtractTemplatesFromPath", [this]()
    {
		BeforeEach([this]()
		{
			GlobalPlaceholders = {
				{TEXT("ProjectName"), "TestProjectName"},
				{TEXT("GameVersion"), "v1" }
			};
		});
    	
        It("returns an empty set when there are no {…} segments", [this]()
        {
            const FString Path = TEXT("/Game/Content/Static/Meshes");
            TSet<FString> Results = UConvention::ExtractTemplatesFromPath(Path, GlobalPlaceholders);

            TestTrue("Should be empty", Results.Num() == 0);
        });

        It("finds a single template and ignores placeholders", [this]()
        {
            const FString Path = TEXT("/{ProjectName}/{CharacterName}/Meshes");
            TSet<FString> Results = UConvention::ExtractTemplatesFromPath(Path, GlobalPlaceholders);

            TestTrue("Contains CharacterName", Results.Contains(TEXT("CharacterName")));
            TestFalse("Does not contain ProjectName", Results.Contains(TEXT("ProjectName")));
            TestEqual("Exactly one template", Results.Num(), 1);
        });

        It("finds multiple templates in one path", [this]()
        {
            const FString Path = TEXT("/UI/{WidgetType}/{SkinVariant}/Preview");
            TSet<FString> Results = UConvention::ExtractTemplatesFromPath(Path, GlobalPlaceholders);

            TestTrue("Contains WidgetType",   Results.Contains(TEXT("WidgetType")));
            TestTrue("Contains SkinVariant",  Results.Contains(TEXT("SkinVariant")));
            TestEqual("Two templates found",  Results.Num(), 2);
        });

        It("handles braces but empty name gracefully", [this]()
        {
            const FString Path = TEXT("/Data/{}/Values/{LOD}");
            TSet<FString> Results = UConvention::ExtractTemplatesFromPath(Path, GlobalPlaceholders);

            // "{}" is length <3 once trimmed, so should be ignored; only LOD
            TestTrue("Contains LOD",    Results.Contains(TEXT("LOD")));
            TestFalse("Does not contain empty", Results.Contains(TEXT("")));
            TestEqual("One template found", Results.Num(), 1);
        });

        It("handles trailing and leading slashes correctly", [this]()
        {
            const FString Path = TEXT("{Global}/Start/{End}/");
            TSet<FString> Results = UConvention::ExtractTemplatesFromPath(Path, GlobalPlaceholders);

            // "Global" is not in placeholder list, so should be returned
            TestTrue("Contains Global", Results.Contains(TEXT("Global")));
            TestTrue("Contains End",    Results.Contains(TEXT("End")));
            TestEqual("Two templates found", Results.Num(), 2);
        });
    });

	Describe("ResolveTemplatePaths", [this]()
    {
        BeforeEach([this]()
        {
            // Make a fresh, isolated temp folder
            RootDir = FPaths::Combine(FPaths::ProjectIntermediateDir(), TEXT("TemplateUtilsResolveSpec"));
            FM.DeleteDirectory(*RootDir, false, true);
            FM.MakeDirectory(*RootDir, true);

        	GlobalPlaceholders = {
				{TEXT("ProjectName"), "TestProjectName"},
				{TEXT("GameVersion"), "v1" }
			};
        });

        AfterEach([this]()
        {
            FM.DeleteDirectory(*RootDir, false, true);
        });

        It("returns empty when the literal base folder does not exist", [this]()
        {
            const FString Pattern = FPaths::Combine(RootDir, TEXT("DoesNotExist"), TEXT("{Tpl}"));
            TArray<FString> Results = UConvention::ResolveTemplatePaths(Pattern, GlobalPlaceholders);
            TestTrue(TEXT("No matches"), Results.Num() == 0);
        });

        It("resolves a single‐level template", [this]()
        {
            // /RootDir/RootFolder/A  and /RootDir/RootFolder/B
            const FString Base = FPaths::Combine(RootDir, TEXT("RootFolder"));
            FM.MakeDirectory(*Base, true);
            for (const TCHAR* Sub : { TEXT("A"), TEXT("B") })
            {
                FM.MakeDirectory(*FPaths::Combine(Base, Sub), true);
            }

            const FString Pattern = FPaths::Combine(Base, TEXT("{Tpl}"));
            TArray<FString> Results = UConvention::ResolveTemplatePaths(Pattern, GlobalPlaceholders);

            TestEqual(TEXT("Two matches expected"), Results.Num(), 2);
            for (const TCHAR* Sub : { TEXT("A"), TEXT("B") })
            {
                // The returned paths have a leading '/'
                const FString Expected = FString(TEXT("/")) + FPaths::Combine(Base, Sub);
                TestTrue(
                    *FString::Printf(TEXT("Contains %s"), Sub),
                    Results.Contains(Expected)
                );
            }
        });

        It("resolves nested templates at multiple depths", [this]()
        {
            // Build:
            //   /RootDir/Base/T1/Sub1
            //   /RootDir/Base/T1/Sub2
            //   /RootDir/Base/T2/Sub3
            const FString Base = FPaths::Combine(RootDir, TEXT("Base"));
            FM.MakeDirectory(*Base, true);

            const FString T1 = FPaths::Combine(Base, TEXT("T1"));
            FM.MakeDirectory(*T1, true);
            FM.MakeDirectory(*FPaths::Combine(T1, TEXT("Sub1")), true);
            FM.MakeDirectory(*FPaths::Combine(T1, TEXT("Sub2")), true);

            const FString T2 = FPaths::Combine(Base, TEXT("T2"));
            FM.MakeDirectory(*T2, true);
            FM.MakeDirectory(*FPaths::Combine(T2, TEXT("Sub3")), true);

            const FString Pattern = FPaths::Combine(Base, TEXT("{Tpl1}"), TEXT("{Tpl2}"));
            TArray<FString> Results = UConvention::ResolveTemplatePaths(Pattern, GlobalPlaceholders);

            // First‐level (T1, T2) + second‐level (Sub1, Sub2, Sub3) = 5
            TestEqual(TEXT("Five matches expected"), Results.Num(), 5);

            // Just check that each name appears in at least one returned path
            for (const TCHAR* Name : { TEXT("T1"), TEXT("T2"), TEXT("Sub1"), TEXT("Sub2"), TEXT("Sub3") })
            {
                bool bFound = false;
                for (const FString& R : Results)
                {
                    if (R.EndsWith(Name))
                    {
                        bFound = true;
                        break;
                    }
                }
                TestTrue(*FString::Printf(TEXT("Contains %s"), Name), bFound);
            }
        });

        It("treats global placeholders like literals", [this]()
        {
            // /RootDir/Root/{ProjectName}/X
            const FString Base = FPaths::Combine(RootDir, TEXT("Root"));
            FM.MakeDirectory(*FPaths::Combine(Base, TEXT("{ProjectName}")), true);
            FM.MakeDirectory(*FPaths::Combine(Base, TEXT("{ProjectName}"), TEXT("X")), true);

            const FString Pattern = FPaths::Combine(Base, TEXT("{ProjectName}"), TEXT("{Tpl}"));
            TArray<FString> Results = UConvention::ResolveTemplatePaths(Pattern, GlobalPlaceholders);

            // Only “X” should appear under the literal {ProjectName} folder
            TestEqual(TEXT("One match expected"), Results.Num(), 1);
            const FString Expected = FString(TEXT("/")) + FPaths::Combine(Base, TEXT("{ProjectName}"), TEXT("X"));
            TestTrue(TEXT("Contains X path"), Results.Contains(Expected));
        });
    });
}

#endif // WITH_DEV_AUTOMATION_TESTS
