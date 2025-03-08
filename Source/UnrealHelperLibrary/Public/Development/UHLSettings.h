// Pavel Penkov 2025 All Rights Reserved.

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
	UPROPERTY(config, EditAnywhere, Category="Ability System Defaults")
	FUHLAbilitySystemSettings AbilitySystemConfigDefaults = {};
	UPROPERTY(config, EditAnywhere, Category="Ability System Defaults")
	bool bUseAbilitySystemConfigDefaultsInASC = true;

protected:
//~UDeveloperSettings interface
	virtual FName GetCategoryName() const override { return TEXT("Plugins"); };
//~End of UDeveloperSettings interface
};
