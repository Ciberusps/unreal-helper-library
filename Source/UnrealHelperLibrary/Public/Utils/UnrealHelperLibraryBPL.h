// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "UnrealHelperLibraryTypes.h"
#include "AssetRegistry/AssetData.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "UI/UHLHUD.h"
#include "UnrealHelperLibraryBPL.generated.h"

class UWidget;
struct FBlackboardKeySelector;

const float RELATIVE_POINT_ARROW_SIZE = 200.0f;

// TODO improve
// - validations if Actor1/Actors2 is nullptr - DebugPrintString it
// - keywords for every method

/*
 *	Function library class.
 *	Each function in it is expected to be static and represents blueprint node that can be called in any blueprint.
 *
 *	When declaring function you can define metadata for the node. Key function specifiers will be BlueprintPure and BlueprintCallable.
 *	BlueprintPure - means the function does not affect the owning object in any way and thus creates a node without Exec pins.
 *	BlueprintCallable - makes a function which can be executed in Blueprints - Thus it has Exec pins.
 *	DisplayName - full name of the node, shown when you mouse over the node and in the blueprint drop down menu.
 *				Its lets you name the node using characters not allowed in C++ function names.
 *	CompactNodeTitle - the word(s) that appear on the node.
 *	Keywords -	the list of keywords that helps you to find node when you search for it using Blueprint drop-down menu.
 *				Good example is "Print String" node which you can find also by using keyword "log".
 *	Category -	the category your node will be under in the Blueprint drop-down menu.
 *
 *	For more info on custom blueprint nodes visit documentation:
 *	https://wiki.unrealengine.com/Custom_Blueprint_Node_Creation
 */
UCLASS()
class UNREALHELPERLIBRARY_API UUnrealHelperLibraryBPL : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// UFUNCTION(BlueprintCallable, meta = (Keywords = "UnrealHelperLibrary sample test testing"), Category = "UnrealHelperLibraryTesting")
	// static float UnrealHelperLibraryRandomWeight(TMap<FString, >);

	/** Gameplay **/
	// Get HitReactDirection - direction(front/back/left/right) opposite to character(SourceActorLocation) that made hit
	// TODO add
	// - GetHitSourceDirection
	// - bool bUse8Directions
	UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary")
	static EUHLDirection GetHitReactDirection(const FVector& SourceActorLocation, const AActor* TargetActor);

	UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary", meta = (WorldContext = "WorldContextObject", Keywords = "UnrealHelperLibrary ui widget editor viewport"))
	static FVector2D GetViewportSizeUnscaled(UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary", meta = (WorldContext = "WorldContextObject", Keywords = "UnrealHelperLibrary ui widget editor viewport"))
	static FVector2D GetWidgetCenterPosition(UObject* WorldContextObject, UWidget* Widget);
	// UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary")
	// static bool ProjectWorldLocationToWidgetPositionWithScales(APlayerController* PlayerController, FVector WorldLocation, FVector2D& ViewportPosition, bool bPlayerViewportRelative);
	UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary", meta = (WorldContext = "WorldContextObject"))
	static float GetActorDistanceToCenterOfScreen(UObject* WorldContextObject, const AActor* Actor, APlayerController* PlayerController, bool bPlayerViewportRelative = true, const bool bDebug = false, const float DebugLifetime = -1);
	UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary", meta = (WorldContext = "WorldContextObject"))
	static AActor* GetActorClosestToCenterOfScreen(UObject* WorldContextObject, const TArray<AActor*>& Actors, APlayerController* PlayerController, FVector2D& ScreenPosition, bool bPlayerViewportRelative = true, const bool bDebug = false, const float DebugLifetime = -1);

	UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary")
	static AActor* GetMostDistantActor(const TArray<AActor*>& Actors, float& MaxDistance_Out, FVector Location, const bool bDebug = false, const float DebugLifetime = -1);

	// TODO GetDistanceOperation - MostDistant/LeastDistant/Average/Medium/...
	UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary", meta = (WorldContext = "WorldContextObject"))
	static FVector GetMostDistantVector(const UObject* WorldContextObject,
		const TArray<FVector> Vectors, FVector Location, 
		float& MaxDistance_Out, int32& Index_Out,
		bool bUseNavigation = false,
		const bool bDebug = false, const float DebugLifetime = -1);
	
	UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary", meta = (WorldContext = "WorldContextObject"))
	static FVector GetMostDistantActorComponent(const UObject* WorldContextObject,
		const TArray<USceneComponent*> SceneComponents, FVector Location, 
		float& MaxDistance_Out, int32& Index_Out,
		bool bUseNavigation = false,
		const bool bDebug = false, const float DebugLifetime = -1);


	UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary")
	static void DrawDebugLineOnCanvas(
		UObject* WorldContextObject,
		const FLineInfo& LineInfo,
		const bool bRelativeToViewportCenter = false
	);
	UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary")
	static void DrawDebugCrossHair(
		UObject* WorldContextObject,
		const float CrossHairLineLength, const float LineThickness,
		const float AngleToRotate,
		const FVector2f& CrossHairCenterScreenSpace,
		const FLinearColor& LineColor,
		const bool bRelativeToViewportCenter = false
	);
	/** Gameplay **/

	/** Debug **/
	UFUNCTION(BlueprintCallable, Category = "UnrealHelperLibrary|Debug", meta = (Keywords = "UnrealHelperLibrary debug Development", AdvancedDisplay = "D,E,F,G,H,I,J,Duration"))
	static void DebugPrintStrings(const FString& A, const FString& B = "", const FString& C = "", const FString& D = "", const FString& E = "", const FString& F = "", const FString& G = "",
		const FString& H = "", const FString& I = "", const FString& J = "", float Duration = 2.0f, const FName Key = NAME_None, const bool bEnabled = true);
	UFUNCTION(Category = "UnrealHelperLibrary", meta = (WorldContext = "WorldContextObject", Keywords = "UnrealHelperLibrary debug Development", AdvancedDisplay = "D,E,F,G,H,I,J,Duration"))
	static void DebugPrintString(const UObject* WorldContextObject, const FString& A, float Duration = 2.0f, const FName Key = NAME_None, const bool bEnabled = true);
	UFUNCTION(BlueprintCallable, Category = "UnrealHelperLibrary|Debug", meta = (Keywords = "UnrealHelperLibrary debug Development"))
	static void DrawDebugBar();
	/** ~Debug **/

	/** Anims **/
	// TODO dont work correctly, fix
	// if SectionName not specified will be used first section
	UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary|Animations", meta = (Keywords = "UnrealHelperLibrary anim montage"))
	static float GetAnimMontageSectionLengthByName(UAnimMontage* AnimMontage, FName SectionName = NAME_None);
	/** ~Anims **/

	/** Angles **/
	/**
	 * Calculates relative angle to other actor [-180, 180]
	 * @param ActorRelativeToWhomAngleCalculated
	 * @param TargetActor
	 * @param bRelativeToActorBack sometimes you want to know actors that nearest to character backside
	 * @param bDebug
	 * @param DebugLifetime
	 * @param DebugColor
	 */
	UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary|Angles",
		meta = (DefaultToSelf = "ActorRelativeToWhomAngleCalculated", Keywords = "UnrealHelperLibrary debug Development angle relative GetAngle RelativeTo",
			AdvancedDisplay = "bDebug,DebugLifetime,DebugColor,bRelativeToActorBack"))
	static float RelativeAngleToActor(AActor* ActorRelativeToWhomAngleCalculated, AActor* TargetActor, bool bRelativeToActorBack = false, const bool bDebug = false, const float DebugLifetime = -1,
		const FLinearColor DebugColor = FLinearColor::White);
	UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary|Angles",
		meta = (DefaultToSelf = "ActorRelativeToWhomAngleCalculated", Keywords = "UnrealHelperLibrary debug Development angle relative GetAngle RelativeTo"))
	static float RelativeAngleToVector(AActor* ActorRelativeToWhomAngleCalculated, FVector TargetVector);
	UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary|Angles", meta = (Keywords = "UnrealHelperLibrary debug Development angle relative GetAngle RelativeTo"))
	static float RelativeAngleVectorToVector(FVector VectorRelativeToWhomAngleCalculated, FVector TargetVector);
	UFUNCTION(BlueprintCallable, Category = "UnrealHelperLibrary|Angles")
	static EUHLDirection GetOppositeDirection(EUHLDirection Direction_In);
	/** ~Angles **/

	/** Utils **/
	// Get project version from "Project Settings"
	UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary|Utils", meta = (Keywords = "UnrealHelperLibrary version"))
	static FString GetProjectVersion();
	UFUNCTION(BlueprintCallable, Category = "UnrealHelperLibrary|Utils", meta = (Keywords = "UnrealHelperLibrary debug Development"))
	static TArray<FString> GetNamesOfComponentsOnObject(UObject* OwnerObject, UClass* Class);

	// return all assets of specified class in template
	template <typename T>
	UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary|Utils")
	static void GetAssetsOfClass(TArray<T*>& OutArray)
	{
		FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
		TArray<FAssetData> AssetData;
		AssetRegistryModule.Get().GetAssetsByClass(T::StaticClass()->GetClassPathName(), AssetData);
		for (int i = 0; i < AssetData.Num(); i++)
		{
			T* Object = Cast<T>(AssetData[i].GetAsset());
			OutArray.Add(Object);
		}
	}

	UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary|Utils")
	static UActorComponent* GetActorComponentByName(AActor* Actor, FString Name);
	UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary|Utils")
	static USceneComponent* GetSceneComponentByName(AActor* Actor, FString Name);

	UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary|Utils", meta = (Keywords = "UnrealHelperLibrary bounds box extent"))
	static FVector GetRandomPointInBox(const USceneComponent* Component, bool bOnGround, bool bDrawDebug = false, float DebugDrawTime = 5.0f);
	UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary|Utils", meta = (Keywords = "UnrealHelperLibrary bounds box extent"))
	static FVector GetHighestPointInBox(const USceneComponent* Component);
	UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary|Utils", meta = (Keywords = "UnrealHelperLibrary bounds box extent"))
	static FVector GetCenterPointInBox(const USceneComponent* Component);
	UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary|Utils", meta = (Keywords = "UnrealHelperLibrary bounds box extent"))
	static FBox GetComponentBox(const USceneComponent* Component);

	UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary|Utils", meta = (WorldContext = "WorldContextObject", Keywords = "UnrealHelperLibrary vector center"))
	static FVector GetCenterPointBetweenVectors(const UObject* WorldContextObject, const FVector& PointA, const FVector& PointB,
		const bool bDebug = false, const float DebugLifetime = -1, const FLinearColor DebugColor = FLinearColor::White);
	
	UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary|Utils", meta = (DefaultToSelf = "ActorIn", AdvancedDisplay = "bDebug,DebugLifetime,DebugColor"))
	static void GetPointAtRelativeAngle(FVector& Point, FRotator& PointRotation, const AActor* ActorIn, const float Angle, const float Distance, const bool bDebug = false,
		const float DebugLifetime = -1, const FLinearColor DebugColor = FLinearColor::White);
	UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary|Utils", meta = (DefaultToSelf = "ActorIn", AdvancedDisplay = "bDebug,DebugLifetime,DebugColor,DebugText"))
	static void GetPointAtRelativeDirection(FVector& Point, FRotator& PointRotation, const AActor* ActorIn, const EUHLDirection Direction, const float Distance, const bool bDebug = false,
		const float DebugLifetime = -1, const FLinearColor DebugColor = FLinearColor::White, const FText DebugText = FText());
	UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary|Utils", meta = (DefaultToSelf = "Actor1", AdvancedDisplay = "bTakeZFromActor1,bDebug,DebugLifetime,DebugColor"))
	static void GetPointAtAngleRelativeToOtherActor(FVector& Point, FRotator& PointRotation, const AActor* Actor1, const AActor* Actor2, const float Angle, const float Distance,
		const bool bTakeZFromActor1 = true, const bool bDebug = false, const float DebugLifetime = -1, const FLinearColor DebugColor = FLinearColor::White);
	UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary|Utils", meta = (DefaultToSelf = "Actor1", AdvancedDisplay = "bTakeZFromActor1,bDebug,DebugLifetime,DebugColor"))
	static void GetPointAtDirectionRelativeToOtherActor(FVector& Point, FRotator& PointRotation, const AActor* Actor1, const AActor* Actor2, const EUHLDirection Direction, const float Distance,
		const bool bTakeZFromActor1 = true, const bool bDebug = false, const float DebugLifetime = -1, const FLinearColor DebugColor = FLinearColor::White);
	UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary|Utils")
	static float DirectionToAngle(const EUHLDirection DirectionIn);

	// TODO test not sure that it works like i wanted
	UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary|Directions")
	static EUHLDirection ConvertMovementInputVectorToDirection(const FVector& Vector);

	UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary|Directions")
	static EUHLDirection GetMovementDirection(const FVector& Velocity, const FRotator& ActorRotation, float DeadZone);

	/**
	 * Determines the enemy's movement direction relative to your character's facing,
	 * compensating for your own movement velocity. Supports both 8-way and 4-way output,
	 * and can draw debug visualization in world space.
	 *
	 * @param WorldContextObject      Context for drawing (usually 'self' in Blueprint)
	 * @param EnemyVelocity           World-space velocity of the enemy pawn
	 * @param MyVelocity              World-space velocity of your pawn
	 * @param ActorLocation           Your pawn’s world location (for debug drawing)
	 * @param ActorRotation           Your pawn’s world rotation (defines forward)
	 * @param DeadZone                Speed threshold below which returns None (<=0 uses KINDA_SMALL_NUMBER)
	 * @param AngleToleranceDeg       Degrees tolerance around boundaries to snap
	 * @param bFourWay                If true, collapse diagonals into nearest cardinal (4-way)
	 * @param bDebug                  If true, draw debug arrows, lines, and labels in world
	 * @param DebugDuration           Lifetime for debug drawings (0 = single frame)
	 * @param DebugThickness          Line thickness for debug drawings
	 * @return                        The calculated direction enum (EUHLDirection)
	 */
	UFUNCTION(BlueprintCallable, Category="Movement|Direction", meta=(WorldContext="WorldContextObject"))
	static EUHLDirection GetEnemyMovementDirectionRelativeToCharacter(
		UObject* WorldContextObject,
		const FVector& EnemyVelocity,
		const FVector& MyVelocity,
		const FVector& ActorLocation,
		const FRotator& ActorRotation,
		float DeadZone,
		float AngleToleranceDeg = 5.f,
		bool bFourWay = false,
		bool bDebug = false,
		float DebugDuration = 0.f,
		float DebugThickness = 2.f
	);
	
	
	// e.g. 60% -> x0.4, 40% -> x0.6, 100% -> x0.0, 0% -> x1.0
	UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary|Utils")
	static float ConvertPercentToMultiplier(float Percent);

	UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary", meta = (DefaultToSelf = "ActorIn", Keywords = "UnrealHelperLibrary attach"))
	static AActor* FindAttachedActorByTag(AActor* ActorIn, FName Tag);

	UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary|Utils", meta = (WorldContext = "WorldContextObject", Keywords = "UnrealHelperLibrary preview editor"))
	static bool IsPreviewWorld(UObject* WorldContextObject);
	UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary|Utils", meta = (WorldContext = "WorldContextObject", Keywords = "UnrealHelperLibrary"))
	static bool IsGameWorld(UObject* WorldContextObject);
	UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary|Utils", meta = (WorldContext = "WorldContextObject", Keywords = "UnrealHelperLibrary editor"))
	static bool IsEditorWorld(UObject* WorldContextObject);
	UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary|Utils", meta = (Keywords = "UnrealHelperLibrary preview editor"))
	static bool IsObjectInPreviewWorld(UObject* Object);
	UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary|Utils", meta = (Keywords = "UnrealHelperLibrary preview editor"))
	static bool IsObjectInEditorWorld(UObject* Object);
	UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary|Utils", meta = (Keywords = "UnrealHelperLibrary game preview editor"))
	static bool IsObjectInGameWorld(UObject* Object);
	UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary|Utils", meta = (WorldContext = "WorldContextObject", Keywords = "UnrealHelperLibrary"))
	static bool IsWorldTearingDown(UObject* WorldContextObject);
	// TODO: ???
	// UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary")
	// static EUHLDirection AngleToDirection(const float AngleIn);

	UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary|Angles", meta = (DefaultToSelf = "Actor", Keywords = "UnrealHelperLibrary angle distance"))
	static bool IsOtherActorInAngle(AActor* Actor, AActor* OtherActor, TArray<FFloatRange> Ranges);
	UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary|Angles", meta = (DefaultToSelf = "Character", Keywords = "UnrealHelperLibrary angle distance"))
	static bool InRangeToOtherCharacter(ACharacter* Character, ACharacter* OtherCharacter, FFloatRange Range, bool bIncludeSelfCapsuleRadius, bool bIncludeTargetCapsuleRadius);
	UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary|Angles", meta = (DefaultToSelf = "Character", Keywords = "UnrealHelperLibrary angle distance"))
	static bool InRangeToLocation(ACharacter* Character, FVector Location, FFloatRange Range, bool bIncludeSelfCapsuleRadius);

	UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary|Utils", meta = (Keywords = "UnrealHelperLibrary file path"))
	static FString GetPathToFile(UObject* Object);

	UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary|Utils", meta = (Keywords = "UnrealHelperLibrary build debug"))
	static bool IsDebugBuild();
	UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary|Utils", meta = (Keywords = "UnrealHelperLibrary build debug development"))
	static bool IsDevelopmentBuild();
	UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary|Utils", meta = (Keywords = "UnrealHelperLibrary build debug production release shipping"))
	static bool IsShippingBuild();
	UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary|Utils", meta = (Keywords = "UnrealHelperLibrary build debug test"))
	static bool IsTestBuild();
	UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary|Utils", meta = (Keywords = "UnrealHelperLibrary editor InEditor withEditor"))
	static bool IsInEditor();
	// Are in Debug/Development/Shipping/Test build or Editor. If we WITH_EDITOR returns Editor,
	// no matter what EBuildConfiguration is currently used(mostly WITH_EDITOR is Development)
	// if real build returns - FApp::GetBuildConfiguration() result, except Debug and DebugGame are same "Debug"
	UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary|Utils", meta = (Keywords = "UnrealHelperLibrary build debug test"))
	static EUHLBuildType GetBuildType();

	UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary|Random", meta = (Keywords = "UnrealHelperLibrary random interval"))
	static float RandomValueInInterval(FFloatInterval Range);

	
	UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary|World", meta = (Keywords = "UnrealHelperLibrary world map level"))
	static bool IsWorldPartitionEnabled(UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary|World", meta = (Keywords = "UnrealHelperLibrary world map level"))
	static bool IsLevelHasSublevels(UObject* WorldContextObject);
	/** ~Utils **/

	/** Colors **/
	// "Makes a random but quite nice color" - literally C++ FColor::MakeRandomColor
	// if seed >= 0 than FColor::MakeRandomSeededColor will be used
	UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary|Colors", meta = (Keywords = "UnrealHelperLibrary color random", AdvancedDisplay = "Seed"))
	static FColor RandomColor(int32 Seed = -1);
	// "Makes a random but quite nice color" - literally C++ FLinearColor::MakeRandomColor
	// if seed >= 0 than FLinearColor::MakeRandomSeededColor will be used
	UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary|Colors", meta = (Keywords = "UnrealHelperLibrary color random", AdvancedDisplay = "Seed"))
	static FLinearColor RandomLinearColor(int32 Seed = -1);
	/** ~Colors **/
};
