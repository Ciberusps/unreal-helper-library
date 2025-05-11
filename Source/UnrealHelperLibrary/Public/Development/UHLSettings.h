// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "Subsystems/EnemyTickManager/EnemyTickOptimizerSubsystem.h"
#include "UHLSettings.generated.h"

/**
 * 
 */
UCLASS(config="Game", defaultconfig)
class UNREALHELPERLIBRARY_API UUHLSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(config, EditAnywhere, Category="EnemyTickOptimizerSubsystemSettings")
	FEnemyTickOptimizerSubsystemSettings EnemyTickOptimizerSubsystemSettings;
	
protected:
//~UDeveloperSettings interface
	virtual FName GetCategoryName() const override { return TEXT("Plugins"); };
//~End of UDeveloperSettings interface
};
