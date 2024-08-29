// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/UHLAbilitySystemComponent.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_CheckAbilityInputCache.generated.h"

/**
 * Should start after anything that "BlockActions" end
 * by default clears AbilityInputCache on end
 */
UCLASS(Category="UnrealHelperLibrary")
class UNREALHELPERLIBRARY_API UANS_CheckAbilityInputCache : public UAnimNotifyState
{
	GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bClearCacheOnEnd = true;

#if WITH_EDITOR
    /** Override this to prevent firing this notify state type in animation editors */
    virtual bool ShouldFireInEditor() { return false; }
#endif

    virtual FLinearColor GetEditorColor() override { return FColor::FromHex("#FFA420"); };
    virtual FString GetNotifyName_Implementation() const override { return FString("[UHL] Check AbilityInputCache"); };

    virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
    virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
    virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:
    UPROPERTY()
    TObjectPtr<UUHLAbilitySystemComponent> ASC;
};
