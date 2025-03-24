// Pavel Penkov 2025 All Rights Reserved.


#include "Subsystems/EnemyTickManager/EnemyTickOptimizerSubsystem.h"

#include "Development/UHLSettings.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

void UEnemyTickOptimizerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const UUHLSettings* UHLSettings = GetDefault<UUHLSettings>();
	Settings = UHLSettings->EnemyTickOptimizerSubsystemSettings;

	if (Settings.bEnable)
	{
		// Set up a timer to call UpdateTickIntervals every 0.5 seconds
		GetWorld()->GetTimerManager().SetTimer(
			TickOptimizerTimerHandle,
			this,
			&UEnemyTickOptimizerSubsystem::UpdateTickIntervals,
			Settings.UpdateInterval,
			true
		);	
	}
}

void UEnemyTickOptimizerSubsystem::Deinitialize()
{
	// Clear the timer when the subsystem is shut down
	GetWorld()->GetTimerManager().ClearTimer(TickOptimizerTimerHandle);
	
	Super::Deinitialize();
}

void UEnemyTickOptimizerSubsystem::RegisterEnemy(ACharacter* Enemy)
{
	if (Enemy && !RegisteredEnemies.Contains(Enemy))
	{
		RegisteredEnemies.Add(Enemy);
	}
}

void UEnemyTickOptimizerSubsystem::UnregisterEnemy(ACharacter* Enemy)
{
	RegisteredEnemies.Remove(Enemy);
}

void UEnemyTickOptimizerSubsystem::UpdateTickIntervals()
{
	// Get the player character
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (!PlayerCharacter)
	{
		return;
	}

	FVector PlayerLocation = PlayerCharacter->GetActorLocation();

	// Iterate through all registered enemies and adjust their tick intervals
	for (ACharacter* Enemy : RegisteredEnemies)
	{
		if (Enemy)
		{
			float Distance = FVector::Dist(PlayerLocation, Enemy->GetActorLocation());
			float TickInterval;

			if (Distance < Settings.CloseDistance)
			{
				TickInterval = Settings.CloseTickInterval; // e.g., 0.0f (every frame)
			}
			else if (Distance < Settings.MediumDistance)
			{
				TickInterval = Settings.MediumTickInterval; // e.g., 0.1f
			}
			else if (Distance < Settings.FarDistance)
			{
				TickInterval = Settings.FarTickInterval; // e.g., 0.5f
			}
			else
			{
				TickInterval = Settings.DefaultEnemyTickInterval; // e.g., 1.0f
			}

			if (Settings.bDontTickIfNotRenderedRecently &&
				!Enemy->WasRecentlyRendered()
				&& Distance > Settings.MediumDistance)
			{
				TickInterval = Settings.RecentlyNotRenderedTickInterval;
			}

			// Set tick interval for the enemy actor
			Enemy->SetActorTickInterval(TickInterval);
			// Enemy->GetMesh()->SetComponentTickInterval(TickInterval);
			if (Settings.bSetTickOnActorComponentsAlso)
			{
				for (UActorComponent* Component : Enemy->GetComponents())
				{
					if (Component->IsComponentTickEnabled())
					{
						Component->SetComponentTickInterval(TickInterval);
					}
				}	
			}

			// Set tick interval for the enemy's controller, if it exists
			if (AController* Controller = Enemy->GetController())
			{
				Controller->PrimaryActorTick.TickInterval = TickInterval;
			}
		}
	}
}
