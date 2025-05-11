// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UHLAbilitySystemComponent.h"
#include "UObject/Interface.h"
#include "UHLAbilitySystemInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(Blueprintable, MinimalAPI)
class UUHLAbilitySystemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UHLGAS_API IUHLAbilitySystemInterface
{
	GENERATED_BODY()

public:
	/** Returns the ability system component to use for this actor. It may live on another actor, such as a Pawn using the PlayerState's component */
	UFUNCTION(Category="UHLAbilitySystemInterface", BlueprintNativeEvent, BlueprintCallable)
	UUHLAbilitySystemComponent* GetUHLAbilitySystemComponent() const;
};