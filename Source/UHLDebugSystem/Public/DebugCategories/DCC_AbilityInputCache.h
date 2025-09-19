// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UHLDebugCategoryComponent.h"
#include "DCC_AbilityInputCache.generated.h"

/**
 * Debug category component that toggles the Ability Input Cache debug via CVar
 */
UCLASS()
class UHLDEBUGSYSTEM_API UDCC_AbilityInputCache : public UUHLDebugCategoryComponent
{
	GENERATED_BODY()

public:
	virtual void Activate_Implementation(UObject* ContextObject) override;
	virtual void Deactivate_Implementation(UObject* ContextObject) override;
};


