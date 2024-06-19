// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameplayEffect.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UnrealHelperLibrary/UnrealHelperLibraryTypes.h"
#include "UnrealHelperLibraryBPLibrary.generated.h"

struct FBlackboardKeySelector;

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
class UNREALHELPERLIBRARY_API UUnrealHelperLibraryBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// UFUNCTION(BlueprintCallable, meta = (Keywords = "UnrealHelperLibrary sample test testing"), Category = "UnrealHelperLibraryTesting")
	// static float UnrealHelperLibraryRandomWeight(TMap<FString, >);

/** Gameplay **/
	UFUNCTION(BlueprintCallable, Category = "UnrealHelperLibrary", meta = (Keywords = "UnrealHelperLibrary debug Development", AdvancedDisplay="D,E,F,G,H,I,J,Duration"))
	static float RelativeAngleToActor(AActor* ActorRelativeToWhomAngleCalculated, AActor* TargetActor);
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
    static void GetAssetsOfClass(TArray<T*>& OutArray);

    UFUNCTION(BlueprintPure, Category = "UnrealHelperLibrary", meta = (Keywords = "UnrealHelperLibrary bounds box extent"))
    static FVector GetHighestPoint(const USceneComponent* Component);
/** ~Utils **/

/** AI **/
	static EBBValueType BlackboardKeyToBBValueType(FBlackboardKeySelector BlackboardKey);
/** ~AI **/
};

template <typename T>
void UUnrealHelperLibraryBPLibrary::GetAssetsOfClass(TArray<T*>& OutArray)
{
    FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
    TArray<FAssetData> AssetData;
    AssetRegistryModule.Get().GetAssetsByClass(T::StaticClass()->GetFName(), AssetData);
    for (int i = 0; i < AssetData.Num(); i++) {
        T* Object = Cast<T>(AssetData[i].GetAsset());
        OutArray.Add(Object);
    }
}

