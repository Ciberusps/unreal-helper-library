// Copyright (c) 2024 Pavel Penkov

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/UHLAbilitySystemConfig.h"
#include "Engine/DeveloperSettings.h"
#include "UHLSettings.generated.h"

/**
 * 
 */
UCLASS(config="Game", defaultconfig)
class UNREALHELPERLIBRARY_API UUHLSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(config, EditAnywhere)
	FUHLAbilitySystemSettings AbilitySystemConfigDefaults = {};
	UPROPERTY(config, EditAnywhere)
	bool bUseAbilitySystemConfigDefaultsInASC = true;

protected:
//~UDeveloperSettings interface
	virtual FName GetCategoryName() const override { return TEXT("Plugins"); };
//~End of UDeveloperSettings interface
};
