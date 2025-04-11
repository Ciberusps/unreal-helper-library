// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UHLEditorCustomThumbnail.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, meta = (Blueprintable))
class UUHLEditorCustomThumbnail : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class UHLEDITOR_API IUHLEditorCustomThumbnail
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

/** IUHLEditorCustomThumbnail **/
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Custom Thumbnail")
    UTexture2D* GetCustomThumbnailIcon() const;
	virtual UTexture2D* GetCustomThumbnailIcon_Implementation() const;
/** ~IUHLEditorCustomThumbnail **/
};
