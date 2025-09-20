// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/Notifies/AN_UHL_Base.h"
#include "AN_FireGameplayEvent.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, Category="UnrealHelperLibrary")
class UHLGAS_API UAN_FireGameplayEvent : public UAN_UHL_Base
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category="FireGameplayEvent")
	FGameplayTag EventTag = FGameplayTag::EmptyTag;

	// TODO use "UPayloadWithInstancedStructs" from "UAA_TryActivateAbilityAndWait"
	UPROPERTY(EditAnywhere, Category="FireGameplayEvent", Instanced)
	UObject* OptionalObject = nullptr;

	UPROPERTY(EditAnywhere, Category="FireGameplayEvent", Instanced)
	UObject* OptionalObject2 = nullptr;

	// tags that required on instigator to fire event ??
	UPROPERTY(EditAnywhere, Category="FireGameplayEvent")
	FGameplayTagContainer InstigatorTags = {};

	// tags that required on target to fire event ??
	UPROPERTY(EditAnywhere, Category="FireGameplayEvent")
	FGameplayTagContainer TargetTags = {};
	
	UPROPERTY(EditAnywhere, Category="FireGameplayEvent")
	float EventMagnitude = 0.0f;

protected:
#if WITH_EDITOR
	/** Override this to prevent firing this notify state type in animation editors */
	virtual bool ShouldFireInEditor() { return false; }
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	virtual FLinearColor GetEditorColor() override { return FColor::FromHex("#FF7DE7"); };
	virtual FString GetNotifyName_Implementation() const override;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
