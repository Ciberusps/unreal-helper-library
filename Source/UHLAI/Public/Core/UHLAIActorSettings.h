// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/TurnSettings.h"
#include "UObject/Interface.h"
#include "UHLAIActorSettings.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UUHLAIActorSettings : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class UHLAI_API IUHLAIActorSettings
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

/** IUHLActorSettings */
    UFUNCTION(Category="UHLActorSettings", BlueprintCallable, BlueprintNativeEvent)
    FTurnSettings GetTurnSettings() const;
/** ~IUHLActorSettings */
};
