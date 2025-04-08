// Pavel Penkov 2025 All Rights Reserved.


#include "Conventions/UHLConvention.h"
#include "NamingConventions/PascalCaseNamingConvention.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UHLConvention)

UUHLConvention::UUHLConvention()
{
	Name = "UHL Convention";
	NamingConvention = UPascalCaseNamingConvention::StaticClass();
}