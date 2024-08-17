// Fill out your copyright notice in the Description page of Project Settings.


#include "UHEBlueprintThumbnailRenderer.h"

#include "CanvasItem.h"
#include "CanvasTypes.h"
#include "UHECustomThumbnail.h"
#include "ThumbnailRendering/ThumbnailManager.h"

void UUHEBlueprintThumbnailRenderer::GetThumbnailSize(UObject* Object, float Zoom, uint32& OutWidth, uint32& OutHeight) const
{
    UBlueprint* Blueprint = Cast<UBlueprint>(Object);

    if (Blueprint)
    {
        if (UTexture2D* Texture = GetTextureFromGeneratedClass(Blueprint->GeneratedClass))
        {
            OutWidth = FMath::TruncToInt(Zoom * (float)Texture->GetSurfaceWidth());
            OutHeight = FMath::TruncToInt(Zoom * (float)Texture->GetSurfaceHeight());
        }
    }
    Super::GetThumbnailSize(Object, Zoom, OutWidth, OutHeight);
}

void UUHEBlueprintThumbnailRenderer::Draw(UObject* Object, int32 X, int32 Y, uint32 Width, uint32 Height, FRenderTarget* RenderTarget, FCanvas* Canvas, bool bAdditionalViewFamily)
{
    UBlueprint* Blueprint = Cast<UBlueprint>(Object);
    if (Blueprint)
    {
        if (UTexture2D* Texture2D = GetTextureFromGeneratedClass(Blueprint->GeneratedClass))
        {
            const bool bUseTranslucentBlend = Texture2D && Texture2D->HasAlphaChannel() && ((Texture2D->LODGroup == TEXTUREGROUP_UI) || (Texture2D->LODGroup == TEXTUREGROUP_Pixels2D));
            TRefCountPtr<FBatchedElementParameters> BatchedElementParameters;
            if (bUseTranslucentBlend)
            {
                // If using alpha, draw a checkerboard underneath first.
                const int32 CheckerDensity = 8;
                TObjectPtr<UTexture2D> Checker = UThumbnailManager::Get().CheckerboardTexture;
                Canvas->DrawTile(
                    0.0f, 0.0f, Width, Height, // Dimensions
                    0.0f, 0.0f, CheckerDensity, CheckerDensity, // UVs
                    FLinearColor::White, Checker->GetResource()); // Tint & Texture
            }
            // Use A canvas tile item to draw
            FCanvasTileItem CanvasTile(FVector2D(X, Y), Texture2D->GetResource(), FVector2D(Width, Height), FLinearColor::White);
            CanvasTile.BlendMode = bUseTranslucentBlend ? SE_BLEND_Translucent : SE_BLEND_Opaque;
            CanvasTile.BatchedElementParameters = BatchedElementParameters;
            CanvasTile.Draw(Canvas);
            if (Texture2D && Texture2D->IsCurrentlyVirtualTextured())
            {
                auto VTChars = TEXT("VT");
                int32 VTWidth = 0;
                int32 VTHeight = 0;
                StringSize(GEngine->GetLargeFont(), VTWidth, VTHeight, VTChars);
                float PaddingX = Width / 128.0f;
                float PaddingY = Height / 128.0f;
                float ScaleX = Width / 64.0f; //Text is 1/64'th of the size of the thumbnails
                float ScaleY = Height / 64.0f;
                // VT overlay
                FCanvasTextItem TextItem(FVector2D(Width - PaddingX - VTWidth * ScaleX, Height - PaddingY - VTHeight * ScaleY), FText::FromString(VTChars), GEngine->GetLargeFont(),
                    FLinearColor::White);
                TextItem.EnableShadow(FLinearColor::Black);
                TextItem.Scale = FVector2D(ScaleX, ScaleY);
                TextItem.Draw(Canvas);
            }
            return;
        }
    }
    Super::Draw(Object, X, Y, Width, Height, RenderTarget, Canvas, bAdditionalViewFamily);
}

bool UUHEBlueprintThumbnailRenderer::CanVisualizeAsset(UObject* Object)
{
    UBlueprint* Blueprint = Cast<UBlueprint>(Object);

    if (Blueprint && GetTextureFromGeneratedClass(Blueprint->GeneratedClass) != nullptr)
    {
        return true;
    }
    return Super::CanVisualizeAsset(Object);
}

UTexture2D* UUHEBlueprintThumbnailRenderer::GetTextureFromGeneratedClass(UClass* Class) const
{
    if (Class)
    {
        if (Class->ImplementsInterface(UUHECustomThumbnail::StaticClass()))
        {
            if (UObject* CDO = Class->GetDefaultObject())
            {
                IUHECustomThumbnail* CustomThumbnailInterface = Cast<IUHECustomThumbnail>(CDO);
                if (CustomThumbnailInterface)
                {
                    return CustomThumbnailInterface->Execute_GetCustomThumbnailIcon(CDO);
                }
            }
        }
        // else if (Class->IsChildOf(UBuff::StaticClass()))
        // {
        //     if (UBuff* CDO = Class->GetDefaultObject<UBuff>())
        //     {
        //         return CDO->GetIcon();
        //     }
        // }
    }
    return nullptr;
}
