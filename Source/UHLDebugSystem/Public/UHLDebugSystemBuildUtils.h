// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UHLDebugSystemTypes.h"

static FORCEINLINE EUHLDebugSystemBuildType GetCurrentUHLBuildType()
{
#if WITH_EDITOR
	return EUHLDebugSystemBuildType::Editor;
#else
	#if UE_BUILD_DEBUG
		return EUHLDebugSystemBuildType::Debug;
	#elif UE_BUILD_DEVELOPMENT
		return EUHLDebugSystemBuildType::Development;
	#elif UE_BUILD_SHIPPING
		return EUHLDebugSystemBuildType::Shipping;
	#elif UE_BUILD_TEST
		return EUHLDebugSystemBuildType::Test;
	#else
		return EUHLDebugSystemBuildType::Development;
	#endif
#endif
}


