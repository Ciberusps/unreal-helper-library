// Copyright (c) 2024 Pavel Penkov

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
