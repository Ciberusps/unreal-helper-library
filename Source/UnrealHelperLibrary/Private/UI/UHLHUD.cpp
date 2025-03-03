// Copyright (c) 2024 Pavel Penkov


#include "UI/UHLHUD.h"

#include "Engine/World.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void AUHLHUD::DrawHUD()
{
	Super::DrawHUD();

	if (!bEnabledDrawDebug) return;

	FVector2D ViewportSize = UWidgetLayoutLibrary::GetViewportSize(GetWorld());
	FVector2D ViewportSizeOffset = ViewportSize / 2;

	// FVector2f Test;
	// UUnrealHelperLibraryBPL::DrawDebugCrossHair(GetWorld(), 100.0f, 3.0f, 0.0f, Test, FLinearColor::Red);

	for (const FLineInfo& LineInfo : LineInfos)
	{
		FVector2D StartPos = LineInfo.Start;
		FVector2D EndPos = LineInfo.End;
		if (LineInfo.bRelativeToViewportCenter)
		{
			StartPos += ViewportSizeOffset;
			EndPos += ViewportSizeOffset;
		}
		DrawLine(
			StartPos.X, StartPos.Y,
			EndPos.X, EndPos.Y,
			LineInfo.Color,
			LineInfo.LineThickness
		);
		FVector2D TextPos = (EndPos - StartPos) + ViewportSizeOffset;

		DrawText(LineInfo.Text, FColor::Red, TextPos.X, TextPos.Y);
	}
	LineInfos.RemoveAll([=](const FLineInfo& LineInfo)
	{
		return !LineInfo.bPersistent;
	});
}

void AUHLHUD::AddOrUpdateLineInfoToDrawNextTick(FLineInfo LineInfo_In)
{
	LineInfos.Add(LineInfo_In);
}