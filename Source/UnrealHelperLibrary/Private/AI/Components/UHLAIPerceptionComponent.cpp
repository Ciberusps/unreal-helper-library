// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Components/UHLAIPerceptionComponent.h"

#include "Perception/AISense_Sight.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UHLAIPerceptionComponent)

void UUHLAIPerceptionComponent::HandleExpiredStimulus(FAIStimulus& StimulusStore)
{
    Super::HandleExpiredStimulus(StimulusStore);

    TSubclassOf<UAISense> StimulusClass = UAIPerceptionSystem::GetSenseClassForStimulus(GetWorld(), StimulusStore);
    if (StimulusClass == UAISense_Sight::StaticClass())
    {
        if (OnSightStimulusExpired.IsBound())
        {
            OnSightStimulusExpired.Broadcast(StimulusStore);
        }
    }
}
