// Pavel Penkov 2025 All Rights Reserved.


#include "Core/UHLPlayerController.h"

#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"
#include "Subsystems/DebugSubsystem/UHLDebugSubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UHLPlayerController)

void AUHLPlayerController::BeginPlay()
{
    Super::BeginPlay();

    UUHLDebugSubsystem* UHLDebugSubsystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UUHLDebugSubsystem>();
    UHLDebugSubsystem->SetUpCategoriesThatRequiresPlayerController();
}
