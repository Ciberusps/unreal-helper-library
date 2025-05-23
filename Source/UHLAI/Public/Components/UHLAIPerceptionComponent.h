// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "UHLAIPerceptionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSightStimulusExpired, const FAIStimulus&, StimulusStore);

/**
 *
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UHLAI_API UUHLAIPerceptionComponent : public UAIPerceptionComponent
{
	GENERATED_BODY()

public:
    virtual void HandleExpiredStimulus(FAIStimulus& StimulusStore) override;

    UPROPERTY(BlueprintAssignable, Category="UHL AIPerceptionComponent")
    FOnSightStimulusExpired OnSightStimulusExpired;
};
