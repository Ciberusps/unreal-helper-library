// Pavel Penkov 2025 All Rights Reserved.


#include "DebugCategories/DCC_AbilityInputCache.h"

#include "HAL/IConsoleManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DCC_AbilityInputCache)

void UDCC_AbilityInputCache::Activate_Implementation(UObject* ContextObject)
{
	IConsoleVariable* Var = IConsoleManager::Get().FindConsoleVariable(TEXT("uhl.Debug.AbilityInputCache"));
	if (Var)
	{
		Var->Set(1);
	}
	return Super::Activate_Implementation(ContextObject);
}

void UDCC_AbilityInputCache::Deactivate_Implementation(UObject* ContextObject)
{
	IConsoleVariable* Var = IConsoleManager::Get().FindConsoleVariable(TEXT("uhl.Debug.AbilityInputCache"));
	if (Var)
	{
		Var->Set(0);
	}
	Super::Deactivate_Implementation(ContextObject);
}


