// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/UHLAbilitySystemConfig.h"

#include "Development/UHLSettings.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UHLAbilitySystemConfig)

UUHLAbilitySystemConfig::UUHLAbilitySystemConfig()
{
	const UUHLSettings* UHLSettings = GetDefault<UUHLSettings>();
	Settings = UHLSettings->AbilitySystemConfigDefaults;
}