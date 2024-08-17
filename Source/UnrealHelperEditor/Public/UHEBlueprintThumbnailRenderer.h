// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ThumbnailRendering/BlueprintThumbnailRenderer.h"
#include "UHEBlueprintThumbnailRenderer.generated.h"

/**
 *
 */
UCLASS()
class UNREALHELPEREDITOR_API UUHEBlueprintThumbnailRenderer : public UBlueprintThumbnailRenderer
{
    GENERATED_BODY()
protected:

    UUHEBlueprintThumbnailRenderer(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer)
    {}

    // UThumbnailRenderer implementation
    virtual void GetThumbnailSize(UObject* Object, float Zoom, uint32& OutWidth, uint32& OutHeight) const override;
    virtual void Draw(UObject* Object, int32 X, int32 Y, uint32 Width, uint32 Height, FRenderTarget*, FCanvas* Canvas, bool bAdditionalViewFamily) override;
    virtual bool CanVisualizeAsset(UObject* Object) override;
protected:

    UTexture2D* GetTextureFromGeneratedClass(UClass* Class) const;
};
