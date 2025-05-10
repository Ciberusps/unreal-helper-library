// Pavel Penkov 2025 All Rights Reserved.

#include "Core/UHLTagCooldowns.h"

void FUHLTagCooldowns::AddCooldownTagDuration(const UObject* Context, const FGameplayTag& CooldownTag, float Duration, bool bAddToExistingDuration)
{
	if (!Context) return;
	if (!CooldownTag.IsValid()) return;
	if (!ensure(Duration > 0.f)) return;

	// const float Now = Context->GetWorld()->GetTimeSeconds();
	// FUHLTagCooldown& CD = CooldownTagsMap.FindOrAdd(CooldownTag);
	// CD.ExpireTime = Now + Duration;

	if (CooldownTag.IsValid())
	{
		double* CurrentEndTime = CooldownTagsMap.Find(CooldownTag);

		// If we are supposed to add to an existing duration, do that, otherwise we set a new value.
		if (bAddToExistingDuration && (CurrentEndTime != nullptr))
		{
			*CurrentEndTime += Duration;
		}
		else
		{
			CooldownTagsMap.Add(CooldownTag, (Context->GetWorld()->GetTimeSeconds() + Duration));
		}
	}
}

bool FUHLTagCooldowns::HasCooldownFinished(const UObject* Context, const FGameplayTag& CooldownTag) const
{
	if (!Context) return false;
	if (!CooldownTag.IsValid()) return false;

	const double TagCooldownEndTime = GetTagCooldownEndTime(Context, CooldownTag);
	if (TagCooldownEndTime == 0.)
	{
		// special case, we don't have an end time yet for this cooldown tag
		return true;
	}
	return (Context->GetWorld()->GetTimeSeconds() >= TagCooldownEndTime);
}

double FUHLTagCooldowns::GetTagCooldownEndTime(
	const UObject* Context, FGameplayTag CooldownTag) const
{
	const double CooldownEndTime = CooldownTagsMap.FindRef(CooldownTag);
	return CooldownEndTime;
}