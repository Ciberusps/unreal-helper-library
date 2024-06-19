// Fill out your copyright notice in the Description page of Project Settings.


#include "Utils/UHLTraceUtilsBPL.h"

// #include "UnrealHelperLibraryBPLibrary.h"


bool UUHLTraceUtilsBPL::SweepCapsuleSingleByChannel(const UWorld* World, FHitResult& OutHit, const FVector& Start,
                                               const FVector& End, float Radius, float HalfHeight,
                                               const FQuat& Rot, ECollisionChannel TraceChannel,
                                               const FCollisionQueryParams& Params,
                                               const FCollisionResponseParams& ResponseParam, bool bDrawDebug,
                                               float DrawTime,
                                               FColor TraceColor, FColor HitColor)
{
	bool bResult = false;

	FCollisionShape CollisionShape = FCollisionShape::MakeCapsule(Radius, HalfHeight);
	bResult = World->SweepSingleByChannel(OutHit, Start, End, Rot, TraceChannel, CollisionShape, Params, ResponseParam);

#if ENABLE_DRAW_DEBUG
	if (bDrawDebug)
	{
		DrawDebugCapsule(World, Start, HalfHeight, Radius, Rot, TraceColor, false, DrawTime);
		DrawDebugCapsule(World, End, HalfHeight, Radius, Rot, TraceColor, false, DrawTime);
		DrawDebugLine(World, Start, End, TraceColor, false, DrawTime);

		if (bResult)
		{
		    float Thickness = FMath::Clamp(HalfHeight / 100, 1.25, 5);
		    // UUnrealHelperLibraryBPLibrary::DebugPrintStrings(FString::Printf(TEXT("%f"), Thickness));
			DrawDebugPoint(World, OutHit.ImpactPoint, 10.0f, HitColor, false, DrawTime, 0);
			DrawDebugCapsule(World, OutHit.Location, HalfHeight, Radius, Rot, TraceColor, false, DrawTime, 0, Thickness);
		}
	}
#endif

	return bResult;
}

bool UUHLTraceUtilsBPL::SweepCapsuleMultiByProfile(const UWorld* World, TArray<FHitResult>& OutHits, const FVector& Start,
                                               const FVector& End, float Radius, float HalfHeight,
                                               const FQuat& Rot, FName ProfileName,
                                               const FCollisionQueryParams& Params,
                                               bool bDrawDebug, float DrawTime,
                                               FColor TraceColor, FColor HitColor)
{
    bool bResult = false;

    FCollisionShape CollisionShape = FCollisionShape::MakeCapsule(Radius, HalfHeight);
    bResult = World->SweepMultiByProfile(OutHits, Start, End, Rot, ProfileName, CollisionShape, Params);

// TODO make debug, dont have time
// #if ENABLE_DRAW_DEBUG
//     if (bDrawDebug)
//     {
//         DrawDebugCapsule(World, Start, HalfHeight, Radius, Rot, TraceColor, false, DrawTime);
//         DrawDebugCapsule(World, End, HalfHeight, Radius, Rot, TraceColor, false, DrawTime);
//         DrawDebugLine(World, Start, End, TraceColor, false, DrawTime);
//
//         if (bResult)
//         {
//             DrawDebugCapsule(World, OutHit.Location, HalfHeight, Radius, Rot, HitColor, false, DrawTime);
//             DrawDebugPoint(World, OutHit.ImpactPoint, 10.0f, HitColor, false, DrawTime);
//         }
//     }
// #endif

    return bResult;
}

bool UUHLTraceUtilsBPL::SweepSphereSingleByChannel(const UWorld* World, FHitResult& OutHit, const FVector& Start,
	const FVector& End, float Radius, ECollisionChannel TraceChannel, const FCollisionQueryParams& Params,
	const FCollisionResponseParams& ResponseParam, bool bDrawDebug, float DrawTime, FColor TraceColor, FColor HitColor)
{
	bool bResult = false;

	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(Radius);
	bResult = World->SweepSingleByChannel(OutHit, Start, End, FQuat::Identity, TraceChannel, CollisionShape, Params, ResponseParam);

#if ENABLE_DRAW_DEBUG
	if (bDrawDebug)
	{
		FVector DebugCenter = (Start + End) * 0.5f;
		FVector TraceVector = End - Start;
		float DebugCapsuleHalfHeight = TraceVector.Size() * 0.5f;

		FQuat DebugCapsuleRotation = FRotationMatrix::MakeFromZ(TraceVector).ToQuat();

		DrawDebugCapsule(World, DebugCenter, DebugCapsuleHalfHeight, Radius, DebugCapsuleRotation, TraceColor, false, DrawTime);

		if (bResult)
		{
			DrawDebugSphere(World, OutHit.Location, Radius, 16, HitColor, false, DrawTime);
			DrawDebugPoint(World, OutHit.ImpactPoint, 10.0f, HitColor, false, DrawTime);
		}
	}
#endif

	return bResult;
}

bool UUHLTraceUtilsBPL::OverlapCapsuleAnyByProfile(const UWorld* World, const FVector& Pos, float Radius, float HalfHeight,
                                              FQuat Rot, FName ProfileName, const FCollisionQueryParams& QueryParams, bool bDrawDebug, float DrawTime,
                                              FColor HitColor)
{
	bool bResult = false;

	FCollisionShape CollisionShape = FCollisionShape::MakeCapsule(Radius, HalfHeight);
	bResult = World->OverlapAnyTestByProfile(Pos, Rot, ProfileName, CollisionShape, QueryParams);

#if ENABLE_DRAW_DEBUG
	if (bDrawDebug && bResult)
	{
		DrawDebugCapsule(World, Pos, HalfHeight, Radius, Rot, HitColor, false, DrawTime);
	}
#endif

	return bResult;
}

bool UUHLTraceUtilsBPL::OverlapBlockingTestByProfile(const UWorld* World, const FVector& Pos, float Radius, float HalfHeight,
											  FQuat Rot, FName ProfileName, const FCollisionQueryParams& QueryParams, bool bDrawDebug, float DrawTime,
											  FColor HitColor)
{
	bool bResult = false;

	FCollisionShape CollisionShape = FCollisionShape::MakeCapsule(Radius, HalfHeight);
	bResult = World->OverlapBlockingTestByProfile(Pos, Rot, ProfileName, CollisionShape, QueryParams);

#if ENABLE_DRAW_DEBUG
	if (bDrawDebug && bResult)
	{
		DrawDebugCapsule(World, Pos, HalfHeight, Radius, Rot, HitColor, false, DrawTime);
	}
#endif

	return bResult;
}
