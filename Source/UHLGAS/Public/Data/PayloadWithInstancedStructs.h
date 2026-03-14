// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StructUtils/InstancedStruct.h"
#include "PayloadWithInstancedStructs.generated.h"

UCLASS(Blueprintable)
class UPayloadWithInstancedStructs : public UObject
{
	GENERATED_BODY()

public:
	UPayloadWithInstancedStructs() = default;
	UPayloadWithInstancedStructs(TArray<FInstancedStruct> InstancedStructs_In);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FInstancedStruct> InstancedStructs = {};
};
