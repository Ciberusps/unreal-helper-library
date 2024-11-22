// Copyright (c) 2024 Pavel Penkov

#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"
#include "CollisionQueryParams.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UHLTraceUtilsBPL.generated.h"

/**
 *
 */
UCLASS()
class UNREALHELPERLIBRARY_API UUHLTraceUtilsBPL : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
    static bool SweepCapsuleSingleByChannel(const UWorld* World, struct FHitResult& OutHit, const FVector& Start,
                                 const FVector& End, float Radius, float HalfHeight, const FQuat& Rot,
                                 ECollisionChannel TraceChannel, const FCollisionQueryParams& Params,
                                 const FCollisionResponseParams& ResponseParam, bool bDrawDebug = false,
                                 float DrawTime = -1.0f, FColor TraceColor = FColor::Black,
                                 FColor HitColor = FColor::Red, float FailDrawTime = -1.0f);
	static bool SweepCapsuleMultiByChannel(const UWorld* World, TArray<FHitResult>& OutHits, const FVector& Start,
								const FVector& End, float Radius, float HalfHeight, const FQuat& Rot,
								ECollisionChannel TraceChannel, const FCollisionQueryParams& Params,
								const FCollisionResponseParams& ResponseParam, bool bDrawDebug = false,
								float DrawTime = -1.0f, FColor TraceColor = FColor::Black,
								FColor HitColor = FColor::Red, float FailDrawTime = -1.0f);
	
    static bool SweepSphereSingleByChannel(const UWorld* World, struct FHitResult& OutHit, const FVector& Start,
                                     const FVector& End, float Radius,
                                     ECollisionChannel TraceChannel, const FCollisionQueryParams& Params,
                                     const FCollisionResponseParams& ResponseParam, bool bDrawDebug = false,
                                     float DrawTime = -1.0f, FColor TraceColor = FColor::Black,
                                     FColor HitColor = FColor::Red);
    static bool OverlapCapsuleAnyByProfile(const UWorld* World, const FVector& Pos, float Radius, float HalfHeight,
                                    FQuat Rot, FName ProfileName, const FCollisionQueryParams& QueryParams,
                                    bool bDrawDebug = false, float DrawTime = -1.0f,
                                    FColor HitColor = FColor::Red);
    static bool OverlapBlockingTestByProfile(const UWorld* World, const FVector& Pos, float Radius, float HalfHeight,
                                        FQuat Rot, FName ProfileName, const FCollisionQueryParams& QueryParams,
                                        bool bDrawDebug = false, float DrawTime = -1.0f,
                                        FColor HitColor = FColor::Red);
    static bool SweepCapsuleMultiByProfile(const UWorld* World, TArray<FHitResult>& OutHits, const FVector& Start,
                                   const FVector& End, float Radius, float HalfHeight,
                                   const FQuat& Rot, FName ProfileName,
                                   const FCollisionQueryParams& Params,
                                   bool bDrawDebug = false, float DrawTime = -1.0f,
                                   FColor TraceColor = FColor::Black, FColor HitColor = FColor::Red);
    // SweepCapsuleMultiByProfile
};
