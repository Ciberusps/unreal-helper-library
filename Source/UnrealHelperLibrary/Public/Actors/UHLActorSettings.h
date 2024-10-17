// Copyright (c) 2024 Pavel Penkov

#pragma once

#include "CoreMinimal.h"
#include "AI/Data/TurnSettings.h"
#include "UObject/Interface.h"
#include "UHLActorSettings.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UUHLActorSettings : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class UNREALHELPERLIBRARY_API IUHLActorSettings
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

/** IUHLActorSettings */
    UFUNCTION(Category="UHLActorSettings", BlueprintCallable, BlueprintNativeEvent)
    FTurnSettings GetTurnSettings() const;
/** ~IUHLActorSettings */
};
