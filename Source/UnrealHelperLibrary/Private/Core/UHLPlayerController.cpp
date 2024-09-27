// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/UHLPlayerController.h"

#include "Kismet/GameplayStatics.h"
#include "Subsystems/DebugSubsystem/UHLDebugSubsystem.h"

void AUHLPlayerController::BeginPlay()
{
    Super::BeginPlay();

    UUHLDebugSubsystem* UHLDebugSubsystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UUHLDebugSubsystem>();
    UHLDebugSubsystem->SetUpCategoriesThatRequiresPlayerController();
}
