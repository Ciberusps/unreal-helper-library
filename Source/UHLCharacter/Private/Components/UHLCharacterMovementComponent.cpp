// Copyright (c) 2024 NextGenium


#include "Components/UHLCharacterMovementComponent.h"

UUHLCharacterMovementComponent::UUHLCharacterMovementComponent()
{
	bUseControllerDesiredRotation = true;
	bOrientRotationToMovement = false;
}

void UUHLCharacterMovementComponent::SetRotationRate(FRotator RotationRate_In)
{
	RotationRate = RotationRate_In;
}
