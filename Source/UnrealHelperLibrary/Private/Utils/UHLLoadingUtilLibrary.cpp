// Copyright (c) 2024 Pavel Penkov


#include "Utils/UHLLoadingUtilLibrary.h"

#include "Engine/CoreSettings.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "GameFramework/WorldSettings.h"
#include "Misc/AssertionMacros.h"
#include "UObject/Object.h"

#include <cfloat>

#include UE_INLINE_GENERATED_CPP_BY_NAME(UHLLoadingUtilLibrary)

bool UUHLLoadingUtilLibrary::HasCapturedDefaults = false;
float UUHLLoadingUtilLibrary::DefaultLevelStreamingComponentsRegistrationGranularity;
float UUHLLoadingUtilLibrary::DefaultLevelStreamingActorsUpdateTimeLimit;
float UUHLLoadingUtilLibrary::DefaultAsyncLoadingTimeLimit;

void UUHLLoadingUtilLibrary::ApplyDefaultPriorityLoading(const UObject* WorldContextObject)
{
	// Call first, just in case defaults have not been captured yet
	CaptureDefaultLoadingSettings();
	ApplyCustomPriorityLoading(WorldContextObject, false, DefaultAsyncLoadingTimeLimit, DefaultLevelStreamingActorsUpdateTimeLimit);
}

void UUHLLoadingUtilLibrary::ApplyStreamingPriorityLoading(const UObject* WorldContextObject)
{
	ApplyCustomPriorityLoading(WorldContextObject, false, 10.0f, 10.0f);
}

void UUHLLoadingUtilLibrary::ApplyHighestPriorityLoading(const UObject* WorldContextObject)
{
	ApplyCustomPriorityLoading(WorldContextObject, true, FLT_MAX, FLT_MAX);
}

void UUHLLoadingUtilLibrary::ApplyCustomPriorityLoading(const UObject* WorldContextObject, bool UseHighPriorityLoading, float MaxAsyncLoadingMilliSeconds, float MaxActorUpdateMilliSeconds)
{
	CaptureDefaultLoadingSettings();

	if (!ensure(WorldContextObject != nullptr))
	{
		return;
	}

	UWorld* World = WorldContextObject->GetWorld();

	if (!ensure(World != nullptr))
	{
		return;
	}

	AWorldSettings* WorldSettings = World->GetWorldSettings();

	if (!ensure(WorldSettings != nullptr))
	{
		return;
	}

	WorldSettings->bHighPriorityLoadingLocal = UseHighPriorityLoading;
	GLevelStreamingActorsUpdateTimeLimit = MaxActorUpdateMilliSeconds;
	GLevelStreamingComponentsRegistrationGranularity = DefaultLevelStreamingComponentsRegistrationGranularity;
	GAsyncLoadingUseFullTimeLimit = UseHighPriorityLoading;
	GAsyncLoadingTimeLimit = MaxAsyncLoadingMilliSeconds;
}

void UUHLLoadingUtilLibrary::FlushLevelStreaming(const UObject* WorldContextObject)
{
	if (!ensure(WorldContextObject != nullptr))
	{
		return;
	}

	UWorld* World = WorldContextObject->GetWorld();

	if (!ensure(World != nullptr))
	{
		return;
	}

	GEngine->BlockTillLevelStreamingCompleted(World);
}

void UUHLLoadingUtilLibrary::ForceGarbageCollection()
{
#if WITH_EDITOR
	GEngine->ForceGarbageCollection(false);
#else
	GEngine->ForceGarbageCollection(true);
#endif
}

void UUHLLoadingUtilLibrary::CaptureDefaultLoadingSettings()
{
	if (!HasCapturedDefaults)
	{
		DefaultLevelStreamingComponentsRegistrationGranularity = GLevelStreamingComponentsRegistrationGranularity;
		DefaultLevelStreamingActorsUpdateTimeLimit = GLevelStreamingActorsUpdateTimeLimit;
		DefaultAsyncLoadingTimeLimit = GAsyncLoadingTimeLimit;
		HasCapturedDefaults = true;
	}
}
