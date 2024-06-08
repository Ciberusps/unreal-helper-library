// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Components/UHL_AIPerceptionComponent.h"

#include "Perception/AISense_Sight.h"

void UUHL_AIPerceptionComponent::HandleExpiredStimulus(FAIStimulus& StimulusStore)
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
