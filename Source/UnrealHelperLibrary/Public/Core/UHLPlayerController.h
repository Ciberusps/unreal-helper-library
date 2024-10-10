// Fill out your copyright notice in the Description page of Project Settings.

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
