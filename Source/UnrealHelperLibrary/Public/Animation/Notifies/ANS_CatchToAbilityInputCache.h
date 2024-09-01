// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_CatchToAbilityInputCache.generated.h"

class UUHLAbilitySystemComponent;

/**
 * Should end then anything that "BlockActions" end
 */
UCLASS()
class UNREALHELPERLIBRARY_API UANS_CatchToAbilityInputCache : public UAnimNotifyState
{
	GENERATED_BODY()

public:
#if WITH_EDITOR
    /** Override this to prevent firing this notify state type in animation editors */
    virtual bool ShouldFireInEditor() { return false; }
#endif

    virtual FLinearColor GetEditorColor() override { return FColor::FromHex("#FFB341"); };
    virtual FString GetNotifyName_Implementation() const override { return FString("[UHL] CatchTo AbilityInputCache"); };

    virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
    virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:
    UPROPERTY()
    TObjectPtr<UUHLAbilitySystemComponent> ASC;
};
