// Pavel Penkov 2025 All Rights Reserved.

#include "Components/UHLStateTreeAIComponent.h"

#include "Misc/EngineVersion.h" 
#include "Misc/EngineVersionComparison.h"
#include "StateTreeExecutionContext.h"
#include "StateTreeReference.h"

void UUHLStateTreeAIComponent::SetStateTreeReference(const FStateTreeReference& NewRef, const FStateTreeReferenceOverrides& NewOverrides)
{
	// Simply copy the whole struct (operator= is public)
	StateTreeRef = NewRef;

	LinkedStateTreeOverrides = NewOverrides;

	// If you want to ensure your parameters match the new tree
	StateTreeRef.SyncParameters();
}

#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION == 5
// FIXES LinkedStateTreeOverrides for StateTreeAI in UE5.5
bool UUHLStateTreeAIComponent::SetContextRequirements(
	FStateTreeExecutionContext& Context, bool bLogErrors)
{
	Context.SetLinkedStateTreeOverrides(&LinkedStateTreeOverrides);
	return Super::SetContextRequirements(Context, bLogErrors);
}
#endif
