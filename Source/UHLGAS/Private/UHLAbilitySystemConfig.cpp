// Pavel Penkov 2025 All Rights Reserved.


#include "UHLAbilitySystemConfig.h"

#include "Development/UHLGASSettings.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UHLAbilitySystemConfig)

UUHLAbilitySystemConfig::UUHLAbilitySystemConfig()
{
	const UUHLGASSettings* UHLSettings = GetDefault<UUHLGASSettings>();
	Settings = UHLSettings->AbilitySystemConfigDefaults;
}