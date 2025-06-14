// Copyright (c) 2024 NextGenium

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UHLCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class UHLCHARACTER_API UUHLCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	UUHLCharacterMovementComponent();

	// not required?
	void SetRotationRate(FRotator RotationRate_In);
};
