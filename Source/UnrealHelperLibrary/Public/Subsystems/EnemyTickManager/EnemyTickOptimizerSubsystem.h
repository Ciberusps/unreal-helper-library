// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "EnemyTickOptimizerSubsystem.generated.h"


USTRUCT(BlueprintType)
struct FEnemyTickOptimizerSubsystemSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Tick Optimization")
	bool bEnable = false;
	
	// How often we update enemies ticks rate
	UPROPERTY(EditAnywhere, Category = "Tick Optimization")
	float UpdateInterval = 0.5f;
	
	UPROPERTY(EditAnywhere, Category = "Tick Optimization")
	float DefaultEnemyTickInterval = 1.0f;
	
	UPROPERTY(EditAnywhere, Category = "Tick Optimization")
	float CloseDistance = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "Tick Optimization")
	float MediumDistance = 3000.0f;

	UPROPERTY(EditAnywhere, Category = "Tick Optimization")
	float FarDistance = 5000.0f;

	UPROPERTY(EditAnywhere, Category = "Tick Optimization")
	float CloseTickInterval = 0.0f; // Every frame

	UPROPERTY(EditAnywhere, Category = "Tick Optimization")
	float MediumTickInterval = 0.1f;

	UPROPERTY(EditAnywhere, Category = "Tick Optimization")
	float FarTickInterval = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Tick Optimization")
	bool bSetTickOnActorComponentsAlso = true;
	
	UPROPERTY(EditAnywhere, Category = "Tick Optimization")
	bool bDontTickIfNotRenderedRecently = true;
	
	UPROPERTY(EditAnywhere, Category = "Tick Optimization")
	float RecentlyNotRenderedTickInterval = 1.0f;
};

/**
 * 
 */
UCLASS()
class UNREALHELPERLIBRARY_API UEnemyTickOptimizerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// Register an enemy with the subsystem
	void RegisterEnemy(ACharacter* Enemy);

	// Unregister an enemy from the subsystem
	void UnregisterEnemy(ACharacter* Enemy);

private:
	// Array to store all registered enemies
	TArray<ACharacter*> RegisteredEnemies;

	// Timer handle for periodic tick interval updates
	FTimerHandle TickOptimizerTimerHandle;

	// Function to update tick intervals based on distance to the player
	void UpdateTickIntervals();

	// Configurable properties for distance thresholds and tick intervals
	FEnemyTickOptimizerSubsystemSettings Settings;
	
	bool bTickAllowed = false;
	float AccumulatedTime = 0.0f;
	float DesiredInterval = 0.5f;
};
