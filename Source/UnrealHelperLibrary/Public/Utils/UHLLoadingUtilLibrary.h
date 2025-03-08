// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UHLLoadingUtilLibrary.generated.h"

class UObject;
struct FFrame;

/**
 * Mostly copy-pasted from Lyra
 */
UCLASS()
class UNREALHELPERLIBRARY_API UUHLLoadingUtilLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Loading", meta = (WorldContext = "WorldContextObject"))
    static void ApplyDefaultPriorityLoading(const UObject* WorldContextObject);

    UFUNCTION(BlueprintCallable, Category = "Loading", meta = (WorldContext = "WorldContextObject"))
    static void ApplyStreamingPriorityLoading(const UObject* WorldContextObject);

    UFUNCTION(BlueprintCallable, Category = "Loading", meta = (WorldContext = "WorldContextObject"))
    static void ApplyHighestPriorityLoading(const UObject* WorldContextObject);

    UFUNCTION(BlueprintCallable, Category = "Loading", meta = (WorldContext = "WorldContextObject"))
    static void ApplyCustomPriorityLoading(const UObject* WorldContextObject, bool UseHighPriorityLoading, float MaxAsyncLoadingMilliSeconds, float MaxActorUpdateMilliSeconds);

    UFUNCTION(BlueprintCallable, Category = "Loading")
    static void ForceGarbageCollection();

    UFUNCTION(BlueprintCallable, Category = "Loading", meta = (WorldContext = "WorldContextObject"))
    static void FlushLevelStreaming(const UObject* WorldContextObject);

private:
    static void CaptureDefaultLoadingSettings();
    static bool HasCapturedDefaults;
    static float DefaultLevelStreamingActorsUpdateTimeLimit;
    static float DefaultLevelStreamingComponentsRegistrationGranularity;
    static float DefaultAsyncLoadingTimeLimit;

};
