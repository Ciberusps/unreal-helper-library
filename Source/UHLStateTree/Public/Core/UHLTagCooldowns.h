// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/StateTreeAIComponent.h"
#include "UHLTagCooldowns.generated.h"

// USTRUCT()
// struct FUHLTagCooldown
// {
// 	GENERATED_BODY()
//
// 	/** World-time at which this tag is no longer on cooldown */
// 	UPROPERTY()
// 	float ExpireTime = 0.f;
// };

// implementation of tags cooldowns similar to BehaviorTreeComponent
USTRUCT(BlueprintType)
struct FUHLTagCooldowns
{
	GENERATED_BODY()

	/**  
	 * Map from gameplay-tag → its cooldown data  
	 */
	UPROPERTY()
	TMap<FGameplayTag, double> CooldownTagsMap;

	/**  
	* Put Tag on cooldown for Duration seconds (from now)  
	*/
	void AddCooldownTagDuration(const UObject* Context, const FGameplayTag& CooldownTag, float Duration, bool bAddToExistingDuration);

	/**  
	 * True if Tag is still cooling down (ExpireTime > now)  
	 */
	bool HasCooldownFinished(const UObject* Context, const FGameplayTag& CooldownTag) const;

	double GetTagCooldownEndTime(const UObject* Context, FGameplayTag CooldownTag) const;
};
