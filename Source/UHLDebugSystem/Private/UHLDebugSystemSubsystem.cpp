// Pavel Penkov 2025 All Rights Reserved.


#include "UHLDebugSystemSubsystem.h"

#include "UnrealHelperLibrary.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"
#include "GameFramework/PlayerController.h"
#include "Development/UHLDebugSystemSettings.h"
#include "UnrealHelperLibraryTypes.h"
#include "Utils/UnrealHelperLibraryBPL.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UHLDebugSystemSubsystem)

UUHLDebugSystemSubsystem::UUHLDebugSystemSubsystem()
{
}

void UUHLDebugSystemSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

	if (bSetupped) return;
	bSetupped = true;
    UE_LOG(LogUnrealHelperLibrary, Warning, TEXT("[UUHLDebugSubsystem::Initialize] Start"));

	const UUHLDebugSystemSettings* DeveloperSettings = GetDefault<UUHLDebugSystemSettings>();
	DebugCategories = DeveloperSettings->DebugCategories;

	for (const TTuple<FGameplayTag, bool>& EnabledDebugCategory : DeveloperSettings->EnabledDebugCategories)
	{
		const FUHLDebugCategory* UHLDebugCategory = DebugCategories.FindByPredicate([=](const FUHLDebugCategory& DebugCategory)
		{
			return DebugCategory.Tags.HasAnyExact(FGameplayTagContainer(EnabledDebugCategory.Key));
		});
		if (EnabledDebugCategory.Value == true && UHLDebugCategory != nullptr && !UHLDebugCategory->bRequiresPlayerControllerToEnable)
		{
			EnableDebugCategory(EnabledDebugCategory.Key, EnabledDebugCategory.Value);
		}
	};

	EUHLBuildType BuildType = UUnrealHelperLibraryBPL::GetBuildType();
	if (BuildType != EUHLBuildType::Editor)
	{
        UE_LOG(LogUnrealHelperLibrary, Warning, TEXT("[UUHLDebugSubsystem::Initialize] BuildType not editor so check DebugCategories that should be enabled"));
		for (const FUHLDebugCategory& DebugCategory : DebugCategories)
		{
			UE_LOG(LogUnrealHelperLibrary, Warning, TEXT("[UUHLDebugSubsystem::Initialize] EnableDebugCategory %s"), *DebugCategory.Tags.First().ToString());
			if (DebugCategory.ByDefaultEnabledInBuildTypes.Contains(BuildType))
			{
				EnableDebugCategory(DebugCategory.Tags.First(), true);
			}
		}
	}
    UE_LOG(LogUnrealHelperLibrary, Warning, TEXT("[UUHLDebugSubsystem::SetUp] Finish"));

	FWorldDelegates::OnPostWorldInitialization.AddUObject(this, &ThisClass::OnPostWorldInit);
	FWorldDelegates::OnWorldBeginTearDown.AddUObject(this, &ThisClass::OnWorldBeginTearDown);
}

void UUHLDebugSystemSubsystem::Deinitialize()
{
	FWorldDelegates::OnPostWorldInitialization.RemoveAll(this);
	
	if (UWorld* World = GetWorld())
	{
		if (ActorSpawnedDelegateHandle.IsValid())
		{
			World->RemoveOnActorSpawnedHandler(ActorSpawnedDelegateHandle);
		}
	}
	
    for (FUHLDebugCategory& DebugCategory : DebugCategories)
    {
        // if (DebugCategory.bForceComponentsDeactivateOnEnd)
        if (DebugCategory.GetIsEnabled())
        {
            DebugCategory.TryDisable(this);
        }
    }
	
    Super::Deinitialize();
}

void UUHLDebugSystemSubsystem::OnActorSpawned(AActor* SpawnedActor)
{
	if (AController* Controller = Cast<AController>(SpawnedActor))
	{
		// Pawn spawned, check if it’s controlled by a player
		if (APlayerController* PC = Cast<APlayerController>(Controller))
		{
			UE_LOG(LogTemp, Log, TEXT("Pawn %s began play, controlled by PlayerController %s"), *Controller->GetName(), *PC->GetName());
			// Handle the player/pawn here (e.g., subscribe to further events, spawn effects, etc.)

			// unsubscribe as fast as possible
			if (UWorld* World = GetWorld())
			{
				if (ActorSpawnedDelegateHandle.IsValid())
				{
					World->RemoveOnActorSpawnedHandler(ActorSpawnedDelegateHandle);
				}
			}
			SetUpCategoriesThatRequiresPlayerController();
		}
	}
}

void UUHLDebugSystemSubsystem::SetUpCategoriesThatRequiresPlayerController()
{
    if (bSetUpCategoriesThatRequiresPlayerController) return;
    UE_LOG(LogUnrealHelperLibrary, Warning, TEXT("[UUHLDebugSubsystem::SetUpCategoriesThatRequiresPlayerController] Start"));
    bSetUpCategoriesThatRequiresPlayerController = true;

    const UUHLDebugSystemSettings* DeveloperSettings = GetDefault<UUHLDebugSystemSettings>();

    for (const TTuple<FGameplayTag, bool>& EnabledDebugCategory : DeveloperSettings->EnabledDebugCategories)
    {
        const FUHLDebugCategory* UHLDebugCategory = DebugCategories.FindByPredicate([=](const FUHLDebugCategory& DebugCategory)
        {
            return DebugCategory.Tags.HasAnyExact(FGameplayTagContainer(EnabledDebugCategory.Key));
        });
        if (EnabledDebugCategory.Value == true && UHLDebugCategory != nullptr && UHLDebugCategory->bRequiresPlayerControllerToEnable)
        {
            EnableDebugCategory(EnabledDebugCategory.Key, EnabledDebugCategory.Value);
        }
    };

	// activate debug categories that "bSetUpCategoriesThatRequiresPlayerController" and should be enabled in BuildType
	EUHLBuildType BuildType = UUnrealHelperLibraryBPL::GetBuildType();
	if (BuildType != EUHLBuildType::Editor)
	{
		UE_LOG(LogUnrealHelperLibrary, Warning, TEXT("[UUHLDebugSubsystem::SetUpCategoriesThatRequiresPlayerController] BuildType not editor so check DebugCategories that should be enabled"));
		for (const FUHLDebugCategory& DebugCategory : DebugCategories)
		{
			if (DebugCategory.ByDefaultEnabledInBuildTypes.Contains(BuildType))
			{
				UE_LOG(LogUnrealHelperLibrary, Warning, TEXT("[UUHLDebugSubsystem::SetUpCategoriesThatRequiresPlayerController] EnableDebugCategory %s"), *DebugCategory.Tags.First().ToString());
				EnableDebugCategory(DebugCategory.Tags.First(), true);
			}
		}
	}

	bIsSetuping = false;
	UE_LOG(LogUnrealHelperLibrary, Warning, TEXT("[UUHLDebugSubsystem::SetUpCategoriesThatRequiresPlayerController] Finish"));
}

bool UUHLDebugSystemSubsystem::IsCategoryEnabled(const FGameplayTag DebugCategoryTag) const
{
    const FUHLDebugCategory* UHLDebugCategory = DebugCategories.FindByPredicate([=](const FUHLDebugCategory& DebugCategory)
    {
        return DebugCategory.Tags.HasAnyExact(FGameplayTagContainer(DebugCategoryTag));
    });
    if (UHLDebugCategory != nullptr)
    {
        return UHLDebugCategory->GetIsEnabled();
    }
    return false;
}

void UUHLDebugSystemSubsystem::EnableDebugCategory(const FGameplayTag DebugCategoryTag, bool bEnable)
{
    bool bEnabled = bEnable;

    FUHLDebugCategory* UHLDebugCategory = DebugCategories.FindByPredicate([=](const FUHLDebugCategory& DebugCategory)
    {
        return DebugCategory.Tags.HasAnyExact(FGameplayTagContainer(DebugCategoryTag));
    });
    if (UHLDebugCategory == nullptr) return;
    if (UHLDebugCategory->bRequiresPlayerControllerToEnable && !bSetUpCategoriesThatRequiresPlayerController) return;
    if (UHLDebugCategory->GetIsEnabled() && bEnable) return;
    if (!UHLDebugCategory->GetIsEnabled() && !bEnable) return;

    if (bEnable)
    {
        // Disable blocked DebugCategories
        for (const FUHLDebugCategory& DebugCategory : DebugCategories)
        {
            if (DebugCategory != *UHLDebugCategory
            	&& !UHLDebugCategory->Blocks.IsEmpty()
                && DebugCategory.Tags.HasAny(UHLDebugCategory->Blocks))
            {
                EnableDebugCategory(DebugCategory.Tags.First(), false);
            }
        }

        bEnabled = UHLDebugCategory->TryEnable(this);
    }
    else
    {
        if (!bIsSetuping)
        // TODO remove
            // || (bIsSetuping && UHLDebugCategory->bForceComponentsDeactivateOnEnd))
        {
            UHLDebugCategory->TryDisable(this);
        }
    }

    for (FGameplayTag GameplayTag : UHLDebugCategory->Tags.GetGameplayTagArray())
    {
        OnDebugCategoryChanged.Broadcast(GameplayTag, bEnabled);
    }
}

void UUHLDebugSystemSubsystem::ToggleDebugCategory(const FGameplayTag DebugCategoryTag)
{
    EnableDebugCategory(DebugCategoryTag, !IsCategoryEnabled(DebugCategoryTag));
}

const FUHLDebugCategory& UUHLDebugSystemSubsystem::GetDebugCategoryByTag(const FGameplayTag DebugCategoryTag) const
{
	const FUHLDebugCategory* UHLDebugCategory = DebugCategories.FindByPredicate([=](const FUHLDebugCategory& DebugCategory)
	{
		return DebugCategory.Tags.HasAnyExact(FGameplayTagContainer(DebugCategoryTag));
	});
	return *UHLDebugCategory;
}

void UUHLDebugSystemSubsystem::OnPostWorldInit(UWorld* InWorld, const UWorld::InitializationValues IVS)
{
	if (InWorld->IsGameWorld())
	{
		FOnActorSpawned::FDelegate ActorSpawnedDelegate = FOnActorSpawned::FDelegate::CreateUObject(this, &UUHLDebugSystemSubsystem::OnActorSpawned);
		ActorSpawnedDelegateHandle = InWorld->AddOnActorSpawnedHandler(ActorSpawnedDelegate);
	}
}

void UUHLDebugSystemSubsystem::OnWorldBeginTearDown(UWorld* World)
{
	World->RemoveOnActorSpawnedHandler(ActorSpawnedDelegateHandle);
}