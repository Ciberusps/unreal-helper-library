// Pavel Penkov 2025 All Rights Reserved.


#include "AbilitySystem/UHLAbilitySystemConfig.h"

#include "Development/UHLSettings.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UHLAbilitySystemConfig)

UUHLAbilitySystemConfig::UUHLAbilitySystemConfig()
{
	const UUHLSettings* UHLSettings = GetDefault<UUHLSettings>();
	Settings = UHLSettings->AbilitySystemConfigDefaults;
}