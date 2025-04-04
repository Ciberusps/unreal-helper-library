// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actors/UHLActorSettings.h"
#include "GameFramework/Character.h"
#include "UHLBaseCharacter.generated.h"

/**
 * Character with base interfaces and features implemented,
 * except AbilitySystem implementation cause its may vary on project setup
 * ASC can be inited on PlayerState/Character
 */
UCLASS()
class UHLCHARACTER_API AUHLBaseCharacter : public ACharacter,
	public IUHLActorSettings
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AUHLBaseCharacter(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TurnSettings")
	UTurnSettingsDataAsset* TurnSettingsDataAsset;
	
/** IUHLActorSettings */
	virtual FTurnSettings GetTurnSettings_Implementation() const override;
/** ~IUHLActorSettings */
};
