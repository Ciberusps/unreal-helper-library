// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Tasks/AT_InterpolateToPosition.h"

#include "Curves/CurveVector.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(AT_InterpolateToPosition)

UAT_InterpolateToPosition::UAT_InterpolateToPosition(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    bTickingTask = true;
    bSimulatedTask = true;
    bIsFinished = false;
}

UAT_InterpolateToPosition* UAT_InterpolateToPosition::InterpolateToPosition(UGameplayAbility* OwningAbility, FName TaskInstanceName, FVector Location, FRotator Rotation, float Duration,
    UCurveFloat* OptionalInterpolationCurve, UCurveVector* OptionalVectorInterpolationCurve, AActor* OptionalActorToInterpolate)
{
    UAT_InterpolateToPosition* MyObj = NewAbilityTask<UAT_InterpolateToPosition>(OwningAbility, TaskInstanceName);

    MyObj->ActorToInterpolate = OptionalActorToInterpolate != nullptr ? OptionalActorToInterpolate : MyObj->GetAvatarActor();
    if (MyObj->ActorToInterpolate != nullptr)
    {
        MyObj->StartLocation = MyObj->ActorToInterpolate->GetActorLocation();
        MyObj->StartRotation = MyObj->ActorToInterpolate->GetActorRotation();
    }

    MyObj->TargetLocation = Location;
    MyObj->TargetRotation = Rotation;
    MyObj->DurationOfMovement = FMath::Max(Duration, 0.001f);		// Avoid negative or divide-by-zero cases
    MyObj->TimeMoveStarted = MyObj->GetWorld()->GetTimeSeconds();
    MyObj->TimeMoveWillEnd = MyObj->TimeMoveStarted + MyObj->DurationOfMovement;
    MyObj->LerpCurve = OptionalInterpolationCurve;
    MyObj->LerpCurveVector = OptionalVectorInterpolationCurve;

    return MyObj;

}

void UAT_InterpolateToPosition::InitSimulatedTask(UGameplayTasksComponent& InGameplayTasksComponent)
{
    Super::InitSimulatedTask(InGameplayTasksComponent);

    TimeMoveStarted = GetWorld()->GetTimeSeconds();
    TimeMoveWillEnd = TimeMoveStarted + DurationOfMovement;
}

void UAT_InterpolateToPosition::Activate()
{
    Super::Activate();
}

void UAT_InterpolateToPosition::TickTask(float DeltaTime)
{
    if (bIsFinished)
    {
        return;
    }

    Super::TickTask(DeltaTime);
    AActor* MyActor = ActorToInterpolate;
    if (MyActor)
    {
        ACharacter* MyCharacter = Cast<ACharacter>(MyActor);
        if (MyCharacter)
        {
            UCharacterMovementComponent* CharMoveComp = Cast<UCharacterMovementComponent>(MyCharacter->GetMovementComponent());
            if (CharMoveComp)
            {
                CharMoveComp->SetMovementMode(MOVE_Custom, 0);
            }
        }


        float CurrentTime = GetWorld()->GetTimeSeconds();

        if (CurrentTime >= TimeMoveWillEnd)
        {
            bIsFinished = true;

            // Teleport in attempt to find a valid collision spot
            MyActor->TeleportTo(TargetLocation, MyActor->GetActorRotation());
            if (!bIsSimulating)
            {
                MyActor->ForceNetUpdate();
                if (ShouldBroadcastAbilityTaskDelegates())
                {
                    OnTargetLocationReached.Broadcast();
                }
                EndTask();
            }
        }
        else
        {
            FVector NewLocation;
            FRotator NewRotation;

            float MoveFraction = (CurrentTime - TimeMoveStarted) / DurationOfMovement;
            if (LerpCurveVector)
            {
                const FVector ComponentInterpolationFraction = LerpCurveVector->GetVectorValue(MoveFraction);
                NewLocation = FMath::Lerp<FVector, FVector>(StartLocation, TargetLocation, ComponentInterpolationFraction);
                // TODO NewRotation =
            }
            else
            {
                if (LerpCurve)
                {
                    MoveFraction = LerpCurve->GetFloatValue(MoveFraction);
                }

                NewLocation = FMath::Lerp<FVector, float>(StartLocation, TargetLocation, MoveFraction);
                NewRotation = FMath::Lerp<FRotator, float>(StartRotation, TargetRotation, MoveFraction);
            }

            MyActor->SetActorLocation(NewLocation);
			MyActor->SetActorRotation(NewRotation);
        }
    }
    else
    {
        bIsFinished = true;
        EndTask();
    }
}

void UAT_InterpolateToPosition::PreDestroyFromReplication()
{
    bIsFinished = true;
    EndTask();
}

void UAT_InterpolateToPosition::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
    DOREPLIFETIME(UAT_InterpolateToPosition, StartLocation);
    DOREPLIFETIME(UAT_InterpolateToPosition, StartRotation);
    DOREPLIFETIME(UAT_InterpolateToPosition, TargetLocation);
    DOREPLIFETIME(UAT_InterpolateToPosition, StartRotation);
    DOREPLIFETIME(UAT_InterpolateToPosition, DurationOfMovement);
    DOREPLIFETIME(UAT_InterpolateToPosition, LerpCurve);
    DOREPLIFETIME(UAT_InterpolateToPosition, LerpCurveVector);
}

void UAT_InterpolateToPosition::OnDestroy(bool bInOwnerFinished)
{
    AActor* MyActor = ActorToInterpolate;
    if (MyActor)
    {
        ACharacter* MyCharacter = Cast<ACharacter>(MyActor);
        if (MyCharacter)
        {
            UCharacterMovementComponent* CharMoveComp = Cast<UCharacterMovementComponent>(MyCharacter->GetMovementComponent());
            if (CharMoveComp && CharMoveComp->MovementMode == MOVE_Custom)
            {
                CharMoveComp->SetMovementMode(MOVE_Walking);
            }
        }
    }

    Super::OnDestroy(bInOwnerFinished);
}
