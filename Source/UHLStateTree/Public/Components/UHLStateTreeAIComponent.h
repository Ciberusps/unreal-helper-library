// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StateTreeReference.h"
#include "Components/StateTreeAIComponent.h"
#include "Core/UHLTagCooldowns.h"
#include "Misc/EngineVersion.h" 
#include "Misc/EngineVersionComparison.h"
#include "UHLStateTreeAIComponent.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, meta=(BlueprintSpawnableComponent))
class UHLSTATETREE_API UUHLStateTreeAIComponent : public UStateTreeAIComponent
{
	GENERATED_BODY()


public:
	/** Swap in *any* FStateTreeReference at runtime */
	void SetStateTreeReference(const FStateTreeReference& NewRef, const FStateTreeReferenceOverrides& NewOverrides);

#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION == 5
	virtual bool SetContextRequirements(FStateTreeExecutionContext& Context, bool bLogErrors = false) override;
#endif

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FUHLTagCooldowns TagCooldowns = {};
};
