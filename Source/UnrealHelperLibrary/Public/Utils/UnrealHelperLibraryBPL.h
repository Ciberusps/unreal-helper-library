// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameplayEffect.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UnrealHelperLibrary/UnrealHelperLibraryTypes.h"
#include "UnrealHelperLibraryBPL.generated.h"

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
	UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary", meta = (Keywords = "UnrealHelperLibrary debug Development angle relative GetAngle RelativeTo"))
	static float RelativeAngleToActor(AActor* ActorRelativeToWhomAngleCalculated, AActor* TargetActor);
    UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary", meta = (Keywords = "UnrealHelperLibrary debug Development angle relative GetAngle RelativeTo"))
    static float RelativeAngleToVector(AActor* ActorRelativeToWhomAngleCalculated, FVector TargetVector);
/** Gameplay **/

/** Debug **/
    UFUNCTION(BlueprintCallable, Category = "UnrealHelperLibrary", meta = (Keywords = "UnrealHelperLibrary debug Development", AdvancedDisplay="D,E,F,G,H,I,J,Duration"))
    static void DebugPrintStrings(const FString& A, const FString& B = "", const FString& C = "", const FString& D = "", const FString& E = "", const FString& F = "", const FString& G = "", const FString& H = "", const FString& I = "", const FString& J = "", float Duration = 2.0f, const FName Key = NAME_None, const bool bEnabled = true);
    UFUNCTION(BlueprintCallable, Category = "UnrealHelperLibrary", meta = (Keywords = "UnrealHelperLibrary debug Development"))
    static void DrawDebugBar();
/** ~Debug **/

/** GAS **/
    UFUNCTION(BlueprintCallable, Category = "UnrealHelperLibrary", meta = (Keywords = "UnrealHelperLibrary debug GAS Development"))
    static FGameplayEffectSpec CreateGenericGASGameplayEffectSpec(TSubclassOf<UGameplayEffect> GameplayEffectClass, AActor* HitInstigator, AActor* InEffectCauser, const FHitResult& HitResult, const UObject* SourceObject);
    /**
    *   UUnrealHelperLibraryBPL::UpdateStateGameplayTags(ASC, GetMovementComponent()->Velocity.Length() > 0,
            UHLGameplayTags::TAG_Character_State_IsMoving,
            UHLGameplayTags::TAG_Character_State_IsIdling);
		bCondition = true ? TAG_Character_State_IsMoving : TAG_Character_State_IsIdling

        UUnrealHelperLibraryBPL::UpdateStateGameplayTags(ASC, CharacterMovementVector.Length() > 0.4f,
            UHLGameplayTags::TAG_Character_State_HasMoveInput,
            FGameplayTag::EmptyTag);
		bCondition = true ? TAG_Character_State_HasMoveInput : NONE
     */
    UFUNCTION(BlueprintCallable, Category = "UnrealHelperLibrary", meta = (Keywords = "UnrealHelperLibrary angle distance"))
    static void UpdateStateGameplayTags(UAbilitySystemComponent* ASC, bool bCondition, FGameplayTag PositiveConditionTag, FGameplayTag NegativeConditionTag);
/** ~GAS **/

/** Utils **/
    // Get project version from "Project Settings"
    UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary", meta = (Keywords = "UnrealHelperLibrary version"))
    static FString GetProjectVersion();
    UFUNCTION(BlueprintCallable, Category = "UnrealHelperLibrary", meta = (Keywords = "UnrealHelperLibrary debug Development"))
    static TArray<FString> GetNamesOfComponentsOnObject(UObject* OwnerObject, UClass* Class);

    // return all assets of specified class in template
    template<typename T>
    UFUNCTION(BlueprintPure)
    static void GetAssetsOfClass(TArray<T*>& OutArray)
    {
        FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
        TArray<FAssetData> AssetData;
        AssetRegistryModule.Get().GetAssetsByClass(T::StaticClass()->GetClassPathName(), AssetData);
        for (int i = 0; i < AssetData.Num(); i++) {
            T* Object = Cast<T>(AssetData[i].GetAsset());
            OutArray.Add(Object);
        }
    }

    UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary")
    static UActorComponent* GetActorComponentByName(AActor* Actor, FString Name);
    UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary")
    static USceneComponent* GetSceneComponentByName(AActor* Actor, FString Name);

    UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary", meta = (Keywords = "UnrealHelperLibrary bounds box extent"))
    static FVector GetRandomPointInBox(const USceneComponent* Component, bool bOnGround, bool bDrawDebug = false, float DebugDrawTime = 5.0f);
    UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary", meta = (Keywords = "UnrealHelperLibrary bounds box extent"))
    static FVector GetHighestPointInBox(const USceneComponent* Component);
    UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary", meta = (DefaultToSelf = "ActorIn", AdvancedDisplay="bDebug,DebugLifetime,DebugColor"))
    static FVector GetPointAtRelativeAngle(const AActor* ActorIn, const float Angle, const float Distance, const bool bDebug = false, const float DebugLifetime = -1, const FLinearColor DebugColor = FLinearColor::White);
    UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary", meta = (DefaultToSelf = "ActorIn", AdvancedDisplay="bDebug,DebugLifetime,DebugColor,DebugText"))
    static void GetPointAtRelativeDirection(FVector& Point, FRotator& PointRotation, const AActor* ActorIn, const EUHLDirection Direction, const float Distance, const bool bDebug = false, const float DebugLifetime = -1, const FLinearColor DebugColor = FLinearColor::White, const FText DebugText = FText());
    UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary", meta = (DefaultToSelf = "Actor1", AdvancedDisplay="bTakeZFromActor1,bDebug,DebugLifetime,DebugColor"))
    static FVector GetPointAtAngleRelativeToOtherActor(const AActor* Actor1, const AActor* Actor2, const float Angle, const float Distance, const bool bTakeZFromActor1 = true, const bool bDebug = false, const float DebugLifetime = -1, const FLinearColor DebugColor = FLinearColor::White);
    UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary", meta = (DefaultToSelf = "Actor1", AdvancedDisplay="bTakeZFromActor1,bDebug,DebugLifetime,DebugColor"))
    static FVector GetPointAtDirectionRelativeToOtherActor(const AActor* Actor1, const AActor* Actor2, const EUHLDirection Direction, const float Distance, const bool bTakeZFromActor1 = true, const bool bDebug = false, const float DebugLifetime = -1, const FLinearColor DebugColor = FLinearColor::White);
    UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary")
    static float DirectionToAngle(const EUHLDirection DirectionIn);
    // e.g. 60% -> x0.4, 40% -> x0.6, 100% -> x0.0, 0% -> x1.0
    UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary")
    static float ConvertPercentToMultiplier(float Percent);

    UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary", meta = (WorldContext = "WorldContextObject", Keywords = "UnrealHelperLibrary preview editor"))
    static bool IsPreviewWorld(UObject* WorldContextObject);
    UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary", meta = (WorldContext = "WorldContextObject", Keywords = "UnrealHelperLibrary preview editor"))
    static bool IsGameWorld(UObject* WorldContextObject);
    UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary", meta = (WorldContext = "WorldContextObject", Keywords = "UnrealHelperLibrary preview editor"))
    static bool IsEditorWorld(UObject* WorldContextObject);
    UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary", meta = (Keywords = "UnrealHelperLibrary preview editor"))
    static bool IsObjectInPreviewWorld(UObject* Object);
    UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary", meta = (Keywords = "UnrealHelperLibrary preview editor"))
    static bool IsObjectInEditorWorld(UObject* Object);
    UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary", meta = (Keywords = "UnrealHelperLibrary game preview editor"))
    static bool IsObjectInGameWorld(UObject* Object);
    // TODO: ???
    // UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary")
    // static EUHLDirection AngleToDirection(const float AngleIn);

    UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary", meta = (Keywords = "UnrealHelperLibrary angle distance"))
    static bool IsOtherActorInAngle(AActor* Actor, AActor* OtherActor, TArray<FFloatRange> Ranges);
    UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary", meta = (Keywords = "UnrealHelperLibrary angle distance"))
    static bool IsOtherCharacterInRange(ACharacter* Actor, ACharacter* OtherActor, FFloatRange Range, bool bIncludeSelfCapsuleRadius, bool bIncludeTargetCapsuleRadius);

    UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary", meta = (Keywords = "UnrealHelperLibrary file path"))
    static FString GetPathToFile(UObject* Object);
/** ~Utils **/

/** AI **/
	static EBBValueType BlackboardKeyToBBValueType(FBlackboardKeySelector BlackboardKey);
/** ~AI **/
};



