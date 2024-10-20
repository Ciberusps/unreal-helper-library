// Copyright (c) 2024 Pavel Penkov

#pragma once

#include "CoreMinimal.h"
#include "Misc/EngineVersionComparison.h"
#include "BehaviorTree/Services/BTService_DefaultFocus.h"
#include "BTS_GameplayFocus.generated.h"

/**
 * Prevents rotation jittering while moving to enemy
 * Requires to turned on "UseControllerDesiredRotation"
 * Requires to turn off "UseControllerRotationYaw"/"UseControllerRotationPitch"/"UseControllerRotationRoll"
 * Troubleshooting:
 * - Check that nothing "ClearFocus" when thinks that it don't work
 * - Check that MoveTo uses "AllowStafe"(low chances to get work, in tests it don't block from strafing)
 */
UCLASS(Category = "UnrealHelperLibrary")
class UNREALHELPERLIBRARY_API UBTS_GameplayFocus : public UBTService_DefaultFocus
{
	GENERATED_BODY()

public:
    UBTS_GameplayFocus(const FObjectInitializer& ObjectInitializer);

#if UE_VERSION_NEWER_THAN(5, 4, 0)
    virtual uint16 GetInstanceMemorySize() const override { return sizeof(FBTFocusMemory); }
    virtual void InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType) const override;
    virtual void CleanupMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryClear::Type CleanupType) const override;
#endif
};
