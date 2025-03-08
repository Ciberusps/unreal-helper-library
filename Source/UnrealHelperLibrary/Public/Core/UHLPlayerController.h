// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UHLPlayerController.generated.h"

/**
 *
 */
UCLASS()
class UNREALHELPERLIBRARY_API AUHLPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
    virtual void BeginPlay() override;
};
