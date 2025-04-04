// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UHLDebugCategoryComponent.generated.h"

// TODO force activation with "MMB" click
UCLASS(Abstract, Blueprintable)
class UHLDEBUG_API UUHLDebugCategoryComponent : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "UHLDebugSubsystem")
    void Activate(UObject* ContextObject);
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "UHLDebugSubsystem")
    void Deactivate(UObject* ContextObject);
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "UHLDebugSubsystem")
    bool CanActivate(UObject* ContextObject) const;
};
