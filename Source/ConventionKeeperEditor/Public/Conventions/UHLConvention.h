// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Convention.h"
#include "UHLConvention.generated.h"


UCLASS(BlueprintType, DefaultToInstanced, EditInlineNew, AutoExpandCategories = ("Default"))
class CONVENTIONKEEPEREDITOR_API UUHLConvention : public UConvention
{
	GENERATED_BODY()

public:
	UUHLConvention();
};
