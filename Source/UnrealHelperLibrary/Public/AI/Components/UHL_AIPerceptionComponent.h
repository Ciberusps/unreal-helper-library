// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "UHL_AIPerceptionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSightStimulusExpired, FAIStimulus&, StimulusStore);

/**
 *
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALHELPERLIBRARY_API UUHL_AIPerceptionComponent : public UAIPerceptionComponent
{
	GENERATED_BODY()

public:
    virtual void HandleExpiredStimulus(FAIStimulus& StimulusStore) override;

    UPROPERTY(BlueprintAssignable)
    FOnSightStimulusExpired OnSightStimulusExpired;
};
