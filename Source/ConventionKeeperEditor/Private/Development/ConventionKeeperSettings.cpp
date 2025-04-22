// Pavel Penkov 2025 All Rights Reserved.


#include "Development/ConventionKeeperSettings.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ConventionKeeperSettings)

TMap<FString, FString> UConventionKeeperSettings::GetPlaceholders() const
{
	TMap<FString, FString> InternalPlaceholders = {
		{ "{ProjectName}", ProjectNameFolder }
	};
	InternalPlaceholders.Append(Placeholders);
	return InternalPlaceholders;
}