// Copyright (c) 2024 Pavel Penkov

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UHECustomThumbnail.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, meta = (Blueprintable))
class UUHECustomThumbnail : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class UNREALHELPEREDITOR_API IUHECustomThumbnail
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

/** IUHECustomThumbnail **/
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Custom Thumbnail")
    UTexture2D* GetCustomThumbnailIcon();
/** ~IUHECustomThumbnail **/
};
