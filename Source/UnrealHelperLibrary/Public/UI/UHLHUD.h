// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UHLHUD.generated.h"

USTRUCT(BlueprintType)
struct FLineInfo
{
	GENERATED_BODY()

	FString Id = "";
	FVector2D Start = FVector2D::ZeroVector;
	FVector2D End = FVector2D::ZeroVector;
	FColor Color = FColor::White;
	float LineThickness = 1.0f;

	FString Text = "";
	FColor TextColor = FColor::White;
	
	bool bRelativeToViewportCenter = false;
	bool bPersistent = false;

// private:
// 	float TimePassed = 0.0f;
};

/**
 * 
 */
UCLASS()
class UNREALHELPERLIBRARY_API AUHLHUD : public AHUD
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEnabledDrawDebug = true;
	
	UFUNCTION(BlueprintCallable)
	void AddOrUpdateLineInfoToDrawNextTick(FLineInfo LineInfo_In);

protected:
	virtual void DrawHUD() override;
	
private:
	UPROPERTY()
	TArray<FLineInfo> LineInfos;
};