// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UHLAbilitySystemConfig.h"
#include "Engine/DeveloperSettings.h"
#include "UHLGASSettings.generated.h"

/**
 * 
 */
UCLASS(Config="Game", DefaultConfig, DisplayName="UHL GAS Settings")
class UHLGAS_API UUHLGASSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category="Ability System Defaults")
	FUHLAbilitySystemSettings AbilitySystemConfigDefaults = {};

	UPROPERTY(Config, EditAnywhere, Category="Ability System Defaults")
	bool bUseAbilitySystemConfigDefaultsInASC = true;

protected:
//~UDeveloperSettings interface
	virtual FName GetCategoryName() const override { return FApp::GetProjectName(); };
//~End of UDeveloperSettings interface
};
