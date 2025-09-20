// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "Data/PayloadWithInstancedStructs.h"

UPayloadWithInstancedStructs::UPayloadWithInstancedStructs(
	TArray<FInstancedStruct> InstancedStructs_In)
{
	InstancedStructs = InstancedStructs_In;
}