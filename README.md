# Unreal Helper Library [UHL]

**UHL** - unreal helper library, toolset to help developers working with AI, GAS, customizing editor and so on.
Goal is to became a tool that insta-installed on new project creation.
All tools are mostly tested on melee combat so if you have other background and think that something should
work another way or have an idea on how to improve developer experience feel free to [discuss](https://github.com/Ciberusps/unreal-helper-library/discussions).

**GAS things not required to be used at all, you can use library only for AI things**. GAS features provides
much smoother GAS experience mostly based on [Lyra](https://dev.epicgames.com/documentation/en-us/unreal-engine/lyra-sample-game-in-unreal-engine?application_version=5.5) features.
All GAS features designed in mind that they or their part can be added or dropped by you in development in any time
and replaced by something custom that fits your project needs

Support: `UE5.5 (main)`, `UE5.4 (branch UE5.4)`

![AiModule](https://github.com/user-attachments/assets/4becb592-c02e-423a-bf80-fcfc629ce518)

## ✨ Features

- AI nodes
  - `GameplayFocus` - set focus on actor
  - `RandomChance` - randomize actions
  - `InRange` - check distance to enemy
  - `InAngle` - check angle to enemy
  - `PlayAnimMontage` - play attack animation
  - `SetBBValue` - set blackboard value
  - `DebugPrintBBValue` - print blackboard value
  - `DebugPrintString` - print string on screen
  - `InvokeGameplayAbility` - activate/deactivate GAS Gameplay Ability by tag, with optional "wait for finishing"
  - `TurnTo` - turn enemy using turn animations
- GAS - Lyra based inputs, ability system component, ability system config, input cache, attribute set, ability sets
- **GAS** strong Lyra based toolset for quick start with GAS and scalable for big projects
- GAS abilities input binding using tags based on Lyra and enhanced with real project needs
- GameplayTags based **DebugSystem**
- Utils - utility functions for your game, e.g. `GetProjectVersion`, `GetAssetsOfClass`, `GetHighestPoint`
- Editor - editor customization, e.g. `Custom thumnails`, `Custom class icon`

## 🚀 Install

#### From source (recommended):

- add git submodule to your plugins folder

    ```bash
    git submodule add https://github.com/Ciberusps/unreal-helper-library.git ./Plugins/UnrealHelperLibrary
    ```

- <details>
  <summary>C++ setup, old engine versions</summary>

  - to use C++ things add code to file `<ProjectName>.Build.cs`

  ```C#
      // <ProjectName>.Build.cs
      public GameName(ReadOnlyTargetRules Target) : base(Target)
      {
          PublicDependencyModuleNames.AddRange(new string[] {
              // add "UnrealHelperLibrary" to use it in C++
              "UnrealHelperLibrary",
          });

          // OPTIONALLY add "UnrealHelperEditor" module to use custom unreal engine editor features
          if (Target.bBuildEditor)
          {
              PrivateDependencyModuleNames.AddRange(new string[] { "UnrealHelperEditor" });
          }
      }
  ```
  
  - to use specific engine version specify branch e.g. `-b UE5.4`

    ```bash
    git submodule add -b UE5.4 https://github.com/Ciberusps/unreal-helper-library.git ./Plugins/UnrealHelperLibrary
    ```

  - to use specific engine version specify branch e.g. `-b UE5.4`

    ```bash
    git submodule add -b UE5.4 https://github.com/Ciberusps/unreal-helper-library.git ./Plugins/UnrealHelperLibrary
    ```

  > [!NOTE]
  > Don't forget to update your `README.md` with instructions on how to setup - `git submodule update --init --recursive` and how to update submodules/plugin(s) - `git submodule update --remote`

  > [!NOTE]
  > Add `Editor Preferences -> Force Compilation on Startup` in `Config/EditorPerProjectUserSettings.ini` your team don't want to recompile plugin manually 😉

</details>

#### From marketplace:

https://www.fab.com/listings/9f7d82e9-bc72-42ff-b302-b3d6562bd4c8

## ⬆️ Update

From source:

- `git submodule update --remote` to update plugin from source

## 🧩 Modules

UHL consists of 3 modules:

- **UnrealHelperLibrary** - main module with GAS helper classes, AI behavior tree nodes, Blueprint Function Libraries. Most functionality can be tested in `Gyms`(maps for testing atomic/single gameplay mechanic), all `Gyms` located in `/Plugins/UnrealHelperLibrary/Content/Gyms`
- **UnrealHelperEditor** - optional module with editor customization, e.g. custom thumnails, custom class icons
- **UHL Utils (EditorUtilityWidget)** - widget with tools helping you make trivial things, like `ConvertToORM` quite often task when you want to combine 3 textures `Occlusion`, `Roughness`, `Metalic` in one ORM texture

## 📄 Documentation

**[🧠 AI](#-ai)**

AI nodes for behavior tree, based on `BehaviorTree` and `BehaviorTreeComponent` from `UE5.4` and `UE5.5` with some improvements and additional features

> - Components
>   - [AIPerceptionComponent](#uhlaiperceptioncomponent)
> - Composite
>   - [RandomSelector](#btc_randomselector)
> - Services
>   - [GameplayFocus](#setgameplayfocus)
> - Decorators
>   - [CheckGASGameplayTagsOnActor](#checkgasgameplaytagsonactor)
>   - [InAngle](#inangle)
>   - [InRange](#inrange)
>   - [LoopRandomCount](#looprandomcount)
>   - [RandomChance](#randomchance)
>   - [TimeLimitRandom](#timelimitrandom)
> - Tasks
>   - [SetBBValue](#setbbvalue)
>   - [DebugPrintBBValue](#debugprintbbvalue)
>   - [DebugPrintString](#debugprintstring)
>   - [InvokeGameplayAbility](#invokegameplayability)
>   - [PlayAnimMontage](#playanimmontage)
>   - [TurnTo](#turnto)

**[💪 GAS](#-gas-gameplay-ability-system)**

Gameplay Ability System - Lyra based inputs, ability system component, ability system config, input cache, attribute set, ability sets

> - Components
>   - [AbilitySystemComponent](#abilitysystemcomponent)
>   - [InputConfig (GAS abilities input binding)](#inputconfig-gas-abilities-input-binding)
>     - [AbilityInputCache (beta)](#abilityinputcache)
>   - [GameplayAbility](#gameplayability)
>   - [AttributeSet](#attributeset)
>   - [AbilitySet](#abilityset)
>   - [AbilitySystem Config](#abilitysystem-config)
> - Tasks
>   - [InterpolateToPosition](#interpolatetoposition)
> - AbilityAsync tasks (AbilityAsync versions of GAS UAbilityTask's, AbilityAsync available in blueprints)
>   - WaitForAbilityActivate
>   - WaitForAbilityActivateWithTagRequirements
>   - WaitForAbilityActivate_Query
> - [UHLGASBlueprintLibrary](#uhlgasblueprintlibrary)
>   - [TryActivateAbilityWithTag](#tryactivateabilitywithtag)
>   - [TryCancelAbilityWithTag](#trycancelabilitywithtag)
>   - [TryCancelAbilitiesWithTags](#trycancelabilitieswithtags)
>   - [FireGameplayEvent](#firegameplayevent)
>   - [UpdateStateGameplayTags](#updatestategameplaytags)
>   - [FindTagByString](#findtagbystring)
>   - [GetUHLAbilitySystemComponent](#getuhlabilitysystemcomponent)
>   - [CreateGenericGASGameplayEffectSpec](#creategenericgasgameplayeffectspec)

**[🦸 Character](#character)**

Default character class with UHL interfaces implemented, so you don't need to do it by yourself

> - [BaseCharacterWithASC](#basecharacterwithasc) (recommended for start)
> - [BaseCharacter](#basecharacter)

**[🪲 DebugSystem](#-debugsystem)**

Debug system for your game, in mid-size commands you always use limited set of debugging tools

> - Settings
>   - [DebugSystemSettings](#debugsystemsettings)
> - Subsystem
>   - [UHLDebugSystemSubsystem](#uhldebugsystemsubsystem)
> - AsyncActions
>   - [WaitDebugCategoryChange](#aa_waitdebugcategorychange)
> - [DebugBlueprintLibrary](#debugblueprintlibrary)
>   - [IsUHLDebugCategoryEnabled](#isuhldebugcategoryenabled)

**[📚 UnrealHelperLibrary](#unrealhelperlibrarybpl)**

> - [UnrealHelperLibraryBPL](#unrealhelperlibrarybpl)
>   - Gameplay
>     - GetActorClosestToCenterOfScreen
>     - GetMostDistantActor
>    <!--  - CreateGenericGASGameplayEffectSpec -->
>   - RelativeAngles
>     - [RelativeAngleToActor](#relativeangletoactor)
>     - [GetPointAtRelativeAngle](#getpointatrelativeangle)
>     - [GetPointAtRelativeDirection](#getpointatrelativedirection)
>     - [GetPointAtAngleRelativeToOtherActor](#getpointatanglerelativetootheractor)
>     - [GetPointAtDirectionRelativeToOtherActor](#getpointatdirectionrelativetootheractor)
>     - [DirectionToAngle](#directiontoangle)
>   - UI/Screen
>     - GetViewportSizeUnscaled
>   - Misc
>     - [GetProjectVersion](#getprojectversion)
>     - [GetNamesOfComponentsOnObject](#getnamesofcomponentsonobject)
>     - [GetAssetsOfClass](#getassetsofclass)
>     - GetBuildType
>    <!--  - GetActorComponentByName -->
>    <!--  - GetSceneComponentByName -->
>   - Debug
>     - DrawDebugLineOnCanvas
>   - Other
>     - [GetHighestPoint](#gethighestpoint)
> - [LoadingUtilLibrary](#loadingutillibrary)
>   - [ApplyDefaultPriorityLoading](#applydefaultpriorityloading)
>   - [ApplyStreamingPriorityLoading](#applystreamingpriorityloading)
>   - [ApplyHighestPriorityLoading](#applyhighestpriorityloading)
>   - [ApplyCustomPriorityLoading](#applycustompriorityloading)
>   - [ForceGarbageCollection](#forcegarbagecollection)
>   - [FlushLevelStreaming](#flushlevelstreaming)
> - [TraceUtilsBPL](#traceutilsbpl)
>   - SweepCapsuleSingleByChannel
> - [Settings](#settings)
>   - [UHL Settings](#uhl-settings)
> - [Subsystems](#subsystems)
>   - [UHLHUD](#uhlhud)
> - AnimNotifyState (ANS)
>   - [ANS_UHL_Base](#ans_uhl_base)
>   - [ANS_ActivateAbility](#ans_activateability)

**[⌨ Editor](#-uhl-editor)**

> - [Custom thumnails](#custom-thumnails)
> - [Custom class icon](#custom-class-icon)
> - [UHL Utils (Editor Utility Widget)](#uhl-utils-editor-utility-widget)
>   - [ConvertToORM](#converttoorm)

---

### 🧠 AI

UHL provides most needed AI nodes toolset for developing at least 3d-action AI - GameplayFocus, Random choices using RandomChance and RandomSelector, PlayAnimMontage to play attacks animations, InRange and InAngle to check distance to enemy and required angle
![AI_nodes](https://github.com/user-attachments/assets/1a00afdf-ab36-4e1c-9bd1-1b29c46cd8ac)

#### `UHLAIPerceptionComponent`

⚒️ InProgress

#### `BTC_RandomSelector`

Select random child node using weights

![image](https://github.com/Ciberusps/unreal-helper-library/assets/14001879/ad19828d-032f-4938-9106-2763ec6fb1fd)

<details>
  <summary>With cool validations</summary>

#### Warns if summary of weights > 1

![image](https://github.com/Ciberusps/unreal-helper-library/assets/14001879/78c62bb9-1b1e-4f5a-89b2-68ea4b445ec2)

#### Warns if chances array have more items then child nodes

![image](https://github.com/Ciberusps/unreal-helper-library/assets/14001879/8dc579a0-7f89-4f27-8a1b-a43fa9889496)

#### Shows error if child nodes count > than chances count

![image](https://github.com/Ciberusps/unreal-helper-library/assets/14001879/b8416859-a557-4378-85e3-27091f631b54)

</details>

#### `CheckGASGameplayTagsOnActor`

**BTD_CheckGASGameplayTagsOnActor** - checks that actor has GAS gameplay tags specified.

> [!WARNING]
> Don't mess with `UBTDecorator_CheckGameplayTagsOnActor` - its only checks `GameplayTags` on actor itself not on `AbilitySystem`.

Requirements:

- actor should implement `IAbilitySystemInterface` to get `AbilitySystemComponent`

![image](https://github.com/Ciberusps/unreal-helper-library/assets/14001879/f1581009-b9cd-408f-84de-2475b43012ae)

#### `InAngle`

**BTD_InAngle** - decorator to check is enemy in one of specified angle ranges. Useful in developing big enemies, for example we developing dragon we want to know is player under the right wing or leg, is player in front of dragon or behind and so on.

![image](https://github.com/Ciberusps/unreal-helper-library/assets/14001879/20f95715-a013-4b5f-8107-cd72d3163f4a)

#### `InRange`

**BTD_InRange** - decorator to check distance between actors. Compliant with "MoveTo" node have same settings `bIncludeSelfCapsuleRadius` and `bIncludeTargetCapsuleRadius` to check distance excluding capsules radiuses

![image](https://github.com/Ciberusps/unreal-helper-library/assets/14001879/5ee47572-0fdb-4afa-bbd1-d18eafb86807)
![InRange](https://github.com/Ciberusps/unreal-helper-library/assets/14001879/e32b5d05-de82-4dfb-80d1-539c866008ff)

#### `LoopRandomCount`

**BTD_LoopRandomCount** - randomized version of decorator `Loop`

![image](https://github.com/Ciberusps/unreal-helper-library/assets/14001879/9a97dd83-d5d3-4cfd-a9bb-830ce7f4c450)

#### `TimeLimitRandom`

**BTD_TimeLimitRandom** - randomized version of decorator `TimeLimit`

![image](https://github.com/Ciberusps/unreal-helper-library/assets/14001879/4fd5b54f-5066-4b47-9425-fac0f92b1b07)

#### `RandomChance`

**BTD_RandomChance** - commonly used decorator to randomize actions. Fine for single child node, extra bad for multiple nodes due to chance regression, for randomization between multiple child nodes better to use [RandomSelector](#btc_randomselector)

![image](https://github.com/Ciberusps/unreal-helper-library/assets/14001879/b469a200-5655-440a-a754-8f8f544a38a2)

#### `SetGameplayFocus`

**BTS_SetGameplayFocus** - alternative for "Set default focus". SetGameplayFocus made right way - prevents rotation jittering while enemy rotation.
One of most common problems that anybody stucks when starting developing AI - "focus dont work"/"focus works wrong".

![image](https://github.com/Ciberusps/unreal-helper-library/assets/14001879/c0add45c-76ef-44bc-b97c-0c56901e6e03)
![GameplayFocus](https://github.com/Ciberusps/unreal-helper-library/assets/14001879/0126fc2b-8a20-4b61-93d8-b1ead6802057)

Requirements:

- turn on `UseControllerDesiredRotation`
- turn off
  - `bOrientRotationToMovement`
  - `UseControllerRotationYaw`
  - `UseControllerRotationPitch`
  - `UseControllerRotationRoll`


Troubleshooting:

- check that nothing "ClearFocus"
- check that MoveTo uses "AllowStafe"

#### `SetBBValue`

**BTT_SetBBValue** - helps settings values in blackboard, supports all blackboard types and for some values event provides opportunity to make calculations like `int`

![image](https://github.com/Ciberusps/unreal-helper-library/assets/14001879/baf4ea25-5d19-482d-a60c-799663def759)

#### `DebugPrintBBValue`

**BTT_DebugPrintBBValue** - prints BB value of any type

![image](https://github.com/Ciberusps/unreal-helper-library/assets/14001879/d138c011-fc9a-438e-bd39-658480cd95bf)

#### `DebugPrintString`

**BTT_DebugPrintString** - simple task for printing debug info on screen

![image](https://github.com/Ciberusps/unreal-helper-library/assets/14001879/510e9766-37be-4f43-a60f-e0e012521841)

#### `InvokeGameplayAbility`

**BTT_InvokeGameplayAbility** - activate/deactivate GAS Gameplay Ability by tag, with optional "wait for finishing"

![image](https://github.com/Ciberusps/unreal-helper-library/assets/14001879/1916c411-7068-43db-9e69-3e6190874de5)

#### `PlayAnimMontage`

**BTT_PlayAnimMontage** - play anim montage with option to customize PlayRate, Starting Position, Start Section Name and stopping montage on task abort

![image](https://github.com/Ciberusps/unreal-helper-library/assets/14001879/5026ebdc-689d-4dba-a168-22ae8d6850bf)

#### `TurnTo`

**BTT_TurnTo** - turn to enemy using turn animations
Drop in replacement for "RotateToFaceBBEntry" but with option to "RotateTo" with animations
To get settings from actor requires `IUHLActorSettings` to be implemented on character

---

### 💪 GAS (Gameplay Ability System)

Many GAS-related things based on "Lyra" sample project.

#### `AbilitySystemComponent`

![UHLAbilitySystemComponent](https://github.com/user-attachments/assets/fe56c7a2-43e6-484a-ac65-635e3670204e)

**UHLAbilitySystemComponent** - for quick start with GAS. You can nest from it on start and than turn off its functions when you ready to replace them with your custom solution.

Features:

- set `InitialAttributes`
- give `Abilities` on start
- activate `InitialActiveAbilities`
- apply `InitialGameplayTags`
- "Lyra"-like "InputConfig", GAS abilities input binding

Setup:

##### Option 1 - zero setup

Easy way with zero setup, just nest your character from `AUHLBaseCharacterWithASC`, fits new projects
there you don't want to waste time at all.

##### Option 2 - BP way

Easy way - just add `UHLAbilitySystemComponent` to your character and call `InitAbilitySystem` on `BeginPlay`/`Possessed`

![image](https://github.com/user-attachments/assets/781e3b92-7af2-42db-a47a-b19f6bdd1b71)

##### Option 3 - C++

A bit harder and requires small C++ work, fits for projects with GAS already integrated.
Follow instructions below or just check `AUHLBaseCharacterWithASC` example

```C++
AUHLBaseCharacterWithASC::AUHLBaseCharacterWithASC(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    // create AbilitySystemComponent if you don't have it
    AbilitySystemComponent = CreateDefaultSubobject<UUHLAbilitySystemComponent>(TEXT("UHLAbilitySystem"));
}

void AUHLBaseCharacterWithASC::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    // init AbilitySystem
    AbilitySystemComponent->InitAbilitySystem(NewController, this);
}
```

If you want custom attributes init you can do it

- by overriding `InitAttributes_Implementation` - recommended
- or just don't activate abilities `AbilitySystemComponent->InitAbilitySystem(NewController, this, false)` and make your own attributes init, and then call `AbilitySystemComponent->ActivateInitialAbilities()`

You have 3 levels of advancement using GAS with UHL

1) entry - just using abilities/attributes in your character on start of a project
2) when you understand that you want to share some abilities to other characters - use [AbilitySets](#abilityset)
3) when your team grows and you understand that locking whole character just to add ability is or change some ability system settings is too much - use [AbilitySystem Config](#abilitysystem-config) and optionally defaults in UHLSettings

![image](https://github.com/user-attachments/assets/d0a47867-dacc-4fbf-bd52-dafa70c8939d)


#### InputConfig (GAS abilities input binding)

Binding InputActions to GameplayAbilities using tags, based on `Lyra` but enhanced and adopted for 3D action game.

![image](https://github.com/user-attachments/assets/78355f0a-bd98-430d-acda-c98405f4017d)

##### Setup

- turn on `bUseInputConfig` on `UHLAbilitySystemComponent`
- create `InputConfig` - `DataAsset` nested from `UHLInputConfig`
- add `InputConfig` to your character `UHLAbilitySystemComponent`
- in `Project Settings -> Input -> Default Input Component Class` -> set `UHLInputComponent`
- in your PlayerCharacter class add lines in `SetupPlayerInputComponent` for binding actions from `InputConfig`

For now only C++ setup tested (blueprint option will be later)

```C++
// Your PlayerCharacter class
void AUHLPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    UUHLInputComponent* UHLInputComponent = CastChecked<UUHLInputComponent>(PlayerInputComponent);
    UUHLInputConfig* UHLInputConfig = AbilitySystemComponent->InputConfig;
    TArray<uint32> BindHandles;
    UHLInputComponent->BindAbilityActions(UHLInputConfig, AbilitySystemComponent, &UUHLAbilitySystemComponent::AbilityInputTagPressed, &UUHLAbilitySystemComponent::AbilityInputTagReleased, BindHandles);

    // optional
    // if (UHLInputComponent)
    // {
    //     UHLInputComponent->BindAction(UHLInputConfig->NativeInputAction_Move.InputAction, ETriggerEvent::Triggered, this, &AUHLPlayerCharacter::InputMove);
    //     UHLInputComponent->BindAction(UHLInputConfig->NativeInputAction_Move.InputAction, ETriggerEvent::Completed, this, &AUHLPlayerCharacter::InputStopMove);

    //     UHLInputComponent->BindAction(UHLInputConfig->NativeInputAction_LookMouse.InputAction, ETriggerEvent::Triggered, this, &AUHLPlayerCharacter::InputLook);
    //     UHLInputComponent->BindAction(UHLInputConfig->NativeInputAction_LookStick.InputAction, ETriggerEvent::Triggered, this, &AUHLPlayerCharacter::InputLook);
    // }
}
```

- in your PlayerController class add

```c++
// Your PlayerController.cpp
void AUHLPlayerController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    CachedPlayerCharacter = Cast<AUHLPlayerCharacter>(InPawn);
}

void AUHLPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
    Super::PostProcessInput(DeltaTime, bGamePaused);

    if (!CachedPlayerCharacter.IsValid()) return;

    if (UUHLAbilitySystemComponent* ASC = CachedPlayerCharacter.Get()->GetUHLAbilitySystemComponent())
    {
        ASC->ProcessAbilityInput(DeltaTime, bGamePaused);
    }
}


// Your PlayerController.h
UCLASS()
class UHL_API AUHLPlayerController : public APlayerController
{
  GENERATED_BODY()

protected:
    virtual void OnPossess(APawn* InPawn) override;
    virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;

private:
    TWeakObjectPtr<AUHLPlayerCharacter> CachedPlayerCharacter;
};
```

- now create `InputAction`(IA) and map it in your `InputMappingContext`(IMC)
- add `InputAction` to created `InputConfig` and map it to `AbilityTags`
- now Abilities associated with `AbilityTags` will activates when `InputAction` triggered
- WARN! abilities should nest from `UHLGameplayAbility` for `ActivationPolicy` work correctly
- to controll ability activation, every `UHLGameplayAbility` have `ActivationPolicy`
  - `OnInputTriggered` - will activate when InputAction triggered
  - `WhileInputActive` - activates ability when input pressed and deactivates when input released
  - `OnSpawn` - activates ability when it gived(granted) to character

#### AbilityInputCache

`AbilityInputCache` (beta) - caches abilities activation. If you want to have input quality like in AAA games when you need cache some inputs and fire whem when its available. Abilities/Inputs to cache may vary depending on project e.g. for 3D actions(souls-likes, slashers) its critical, for shooters less important

> [!WARN]
> dont work without [UHLAbilitySystemComponent](#abilitysystemcomponent) and [InputConfig](#inputconfig-gas-abilities-input-binding) enabled

![image](https://github.com/user-attachments/assets/ecc013bf-cc5a-4211-a51c-ced5fe557ec2)

##### Setup:

Instructions here provided with souls-like developing background. Remember you can control `AbilityInputCache` wherever `ASC(AbilitySystemComponent)` is available just take `AbilityInputCache` from `ASC` and call `AddTagToCache`, `CheckCache`, `ClearCache`.... If you need "input window" just add `UHL.AbilityInputCache.Catching` on your character by hand and remove when window not required

- activate `bUseAbilityInputCache` in `UHLAbilitySystemComponent`
- [optionaly] strongly recommended to activate `bUseInputCacheWindows` also. If `bUseInputCacheWindows` not activated any GameplayAbility marked with `bInputCache` that you try to activate in any time on fail will be added to `AbilityInputCache` thats not what you want in 3D action game
- in `GameplayAbility` that you want to cache activate `bInputCache` and fill if required
  - `AddingToCacheInputRequiredTags` - tags that required to be on your character for this ability to be added to `AbilityInputCache`
  - `AddingToCacheInputBlockedTags` - tags that blocks adding this ability to `AbilityInputCache`
- prepare you attack animation - add anim notifies
  - `ANS_CatchToAbilityInputCache` - to mark when its possible to cache ability. Best practice - leave some frames on start(5-10frames at least) and finish when your "BlockAction" end
  - `ANS_CheckAbilityInputCache` - when you want to check cache and activate ability. Best practice - on end of "BlockAction" with 5-10frames duration

##### Debug:

- activate `AbilityInputCache` debug category in [DebugSystem](#-debugsystem) from
  - `ProjectSettings -> UHL DebugSubsystem Settings`
  - or in runtime via `UHLDebugCategoriesListWidget`
- write in console `ToggleAbilityInputDebug`, don't forget to add `ProcessConsoleExec` to your `GameInstance` or it won't work

#### AttributeSet

Just class with default things that every `AttributeSet` wants like `ATTRIBUTE_ACCESSORS`. Nest your `AttributeSets` from it to not duplicate same things other and other again.

#### AbilitySet

"Lyra"-like set of `Abilities, AttributeSets, GameplayEffects`, that can be added to character and removed later by tag

![image](https://github.com/user-attachments/assets/7f7a54dc-3276-4bc3-a866-ba992f506efc)

AbilitySet - is second level of advancement of using GAS in UHL, when you understand that you want to
share some abilities to other characters - use AbilitySets

Use cases:

- gived by external source using  or `AbilitySet->GiveToAbilitySystem` and removed by external source via `AbilitySetGrantedHandles.TakeFromAbilitySystem`
- gived by external source using `ASC->GiveAbilitySet(UUHLAbilitySet* AbilitySet)` and removed by tag calling `ASC->RemoveAbilitySetByTag()`, of course if tag associated with set by defining `AbilitySetTags` in `AbilitySet`

#### AbilitySystem Config

DataAsset - option for teams to edit `AbilitySystem` config externally to not locking character
for just changing abilities/initial attributes

![image](https://github.com/user-attachments/assets/1cfe4ebd-da39-44b3-9955-73ff6e4708ac)

Defaults can be changed in `ProjectSettings -> UHL Settings`

#### GameplayAbility

Additional events - OnSpawn

Activation policy - InputTriggered, WhileInputActive, OnSpawn

InputCache - to use it required to nest from GameplayAbility

#### `InterpolateToPosition`

**AT_InterpolateToPosition** - interpolate actor to specified position/rotation at a predetermined amount of time

![image](https://github.com/Ciberusps/unreal-helper-library/assets/14001879/764ddf72-595e-4316-9149-b7b0accc2b89)

#### `UHLGASBlueprintLibrary`

##### TryActivateAbilityWithTag

##### TryCancelAbilityWithTag

##### TryCancelAbilitiesWithTags

##### FireGameplayEvent

##### UpdateStateGameplayTags

##### FindTagByString

##### GetUHLAbilitySystemComponent

##### CreateGenericGASGameplayEffectSpec

#### `ANS_ActivateAbility`

**ANS_ActivateAbility** - commonly used ANS that just activate ability on start and deactivate on end

![image](https://github.com/user-attachments/assets/a1212bb9-dc09-45a7-8f3d-354ac8f2afa6)

- `GameplayAbilityTag` - tag associated with ability to activate
- `bDeactivateOnMontageBlendingOut` - should ability deactivates on montage blends out
- `bAllowRemoteActivation` - you can allow remote activation

<!-- with ability to subscribe on

- `OnSightStimulusExpired` -->

---

### 🦸 Character

Default characters classess with UHL interfaces implemented, so you don't need to do it by yourself

#### `BaseCharacterWithASC`

**UHLBaseCharacterWithASC** - recommended BaseCharacter for start - ASC created on start and
inited on `PossessedBy`. Can be turned off by disabling `bInitUHLAbilitySystemOnPosses`

#### `BaseCharacter`

**UHLBaseCharacter** - simplest BaseCharacter with only UHL interfaces implemented, so you don't need to do it by yourself

---

### UnrealHelperLibraryBPL

#### > RelativeAngles

#### `RelativeAngleToActor`

for most cases you want to use "InRange" like `IsOtherActorInAngle` (or `IsOtherCharacterInRange` if you want to check distance)

![image](https://github.com/Ciberusps/unreal-helper-library/assets/14001879/4a695df4-9583-451c-801b-98e63c8ad5c8)

#### `GetPointAtRelativeAngle`

![image](https://github.com/Ciberusps/unreal-helper-library/assets/14001879/4c2f5e47-9f5c-4e70-8d77-57cb7383290a)
![image](https://github.com/Ciberusps/unreal-helper-library/assets/14001879/c0026be0-ce3a-4ee5-84e6-b8e90474fdb9)

#### `GetPointAtRelativeDirection`

![image](https://github.com/Ciberusps/unreal-helper-library/assets/14001879/c1ea6dda-4ccf-4441-a7d0-dc83ac977a9c)
![image](https://github.com/Ciberusps/unreal-helper-library/assets/14001879/a33a93ad-f470-4dec-8c2c-6c76b275207f)

#### `GetPointAtAngleRelativeToOtherActor`

#### `GetPointAtDirectionRelativeToOtherActor`

#### `DirectionToAngle`

#### > Misc

#### `GetProjectVersion`

Get project version from "Project Settings"

![image](https://github.com/Ciberusps/unreal-helper-library/assets/14001879/c15d4c68-48d2-4d49-bee6-438f18064f9f)

![image](https://github.com/Ciberusps/unreal-helper-library/assets/14001879/d9e7e53c-b0ba-47cc-b63a-099423a89346)

#### `GetNamesOfComponentsOnObject`

Get names of actor components on object, usefull for [`GetOptions` UPROPERTY](https://benui.ca/unreal/uproperty/#getoptions)

#### `GetAssetsOfClass`

#### > Other

#### `GetHighestPoint`

#### `ANS_UHL_Base`

**ANS_UHL_Base** - base `AnimNotifyState` class with commonly used features like

- subscribing `OnMontageBlendingOut` by overriding `OnMontageBlendingOut` can be disabled by `bUseOnMontageBlendingOut=false(true by default)`
- more come later

### 🪲 DebugSystem

Powerful debug system for gameplay based on GameplayTags. So any game needs debug system, in mid-size commands you always use limited set of debugging tools
more always than others, so **DebugSystem** is as tool for creating your debug system as fast as possible by providing `DebugCategories`(gameplaytags) that can be enabled/disabled

Use case:
I want to have debug for AbilitySystem, it should turn on/off, available in editor between sessions and.....

Components:

##### DebugSystemSettings

- add new categories, turn on/off default state, every debug category is a tag
- `DebugCategoryComponents` (DCC)

##### Check is category enabled/subscribe on debug category state change

- `WaitDebugCategoryChange` - blueprint node to easier check `isDebugCategoryEnabled` or not and wait for its changes
- `IsUHLDebugSystemEnabled` - blueprint node
- `DebugCategoriesList` - UI component for quick integration in your debug menu

**Features:**

- you can create DebugCategoryComponents that activate/deactivate console commands, event in blueprints like GAS abilities
- you can even compose DebugCategoryComponents e.g. you want Collisions + HitBoxes, so you can create combined DebugCategory and add "DCC_Collisions" and "DCC_HitBoxes"
- you can "Block" other DebugCategories by tag
- WaitDebugCategoryChange

**How to add DebugCategory:**
1) ...
2) ...

How to subscribe on debug category change in C++

```c++
    UAA_WaitDebugCategoryChange* WaitDebugCategoryChangeTask = UAA_WaitDebugCategoryChange::WaitDebugCategoryChange(
        Actor->GetWorld(),
        YourGameplayTags::TAG_DebugCategory_Combat // same as FGameplayTag("DebugCategory.Something")
    );
    WaitDebugCategoryChangeTask->OnChange.AddUniqueDynamic(this, &UCombatSubsystem::OnDebugCategoryChanged);
    // on activation "OnDebugCategoryChanged" will be fired
    WaitDebugCategoryChangeTask->Activate();
```

##### DebugSystemSubsystem

##### `UHLDebugSystemSubsystem`

**UHLDebugSystemSubsystem** - subsystem that provides interaction with debug categories

##### DebugBlueprintLibrary

###### `IsUHLDebugCategoryEnabled`

Check is debug category enabled or not

### Subsystems

#### UHLHUD

HUD with debugging abilities, for now used to display debug bars(e.g. HP/hidden attributes)

### 🔃 LoadingUtilLibrary

**UHLLoadingUtilLibrary** - loading utils from Lyra

#### ApplyDefaultPriorityLoading

#### ApplyStreamingPriorityLoading

#### ApplyHighestPriorityLoading

#### ApplyCustomPriorityLoading

#### ForceGarbageCollection

#### FlushLevelStreaming

### 🎯 TraceUtilsBPL

**UHLTraceUtilsBPL** - trace utils

### ⚙️ Settings

#### UHL Settings

- You can set defaults for all [AbilitySystem](#abilitysystemcomponent) and [AbilitySystem Config](#abilitysystem-config) in your project its can be usefull
if you don't want to copy paste your `AttributeSets`

### ⌨ UHL Editor

**UHL Editor** - optional module with editor customization, e.g. custom thumnails, custom class icons

#### `Custom thumnails`

![image](https://github.com/user-attachments/assets/c24fd8bb-0ffe-4666-afd5-8800df650c35)

**Custom thumnails** - to override thumbnail by your own, just implement `IUHLEditorCustomThumbnail` interface and define your own icon using `GetCustomThumbnailIcon()`

> [!WARNING]
> ⚠️ NOT sure that blueprints supported for now

```C++
// UInventoryItem.h
#if WITH_EDITOR
#include "UHLEditorCustomThumbnail.h"
#endif

// IUHLEditorCustomThumbnail not available in production build
#if !WITH_EDITOR
class IUHLEditorCustomThumbnail {};
#endif

class GAMECODE_API UInventoryItem : public UObject,
    public IUHLEditorCustomThumbnail
{
/** IUHLEditorCustomThumbnail **/
#if WITH_EDITOR
    virtual UTexture2D* GetCustomThumbnailIcon_Implementation() const override;
#endif
/** ~IUHLEditorCustomThumbnail **/
}

------------------------------------------------------------------

// UInventoryItem.cpp
#if WITH_EDITOR
UTexture2D* UInventoryItem::GetCustomThumbnailIcon_Implementation()
{
    return Description.Icon;
}
#endif
```

Thanks to [this post](https://forums.unrealengine.com/t/custom-thumbnail-not-display-asset-is-never-loaded/143155/2?u=ciberus) and [this](https://forums.unrealengine.com/t/custom-thumbnail-on-blueprint/337532/3?u=ciberus)

#### `Custom class icon`

**Custom class icon** - to override classes icons on your own, just set settings in `Project Settings -> Editor -> UnrealHelperEditor Settings`

![image](https://github.com/user-attachments/assets/da940018-2120-4b81-84da-5237e97e9c86)

[List of default Unreal Engine Editor icons](https://github.com/EpicKiwi/unreal-engine-editor-icons)

Thanks to [this post](https://www.quodsoler.com/blog/customize-your-unreal-class-icons) and [this](https://forums.unrealengine.com/t/how-to-load-a-font-uasset-and-use-it-for-fslatefontinfo/1548466/3?u=ciberus)

### UHL Utils (Editor Utility Widget)

⚒️ InProgress

### ConvertToORM

Combines separate `Occlusion`, `Roughness`, `Metalic` textures into one `ORM`

TODO check ref - https://github.com/Atulin/ChannelMerger

## Special Thanks

[@Ingarnm](https://github.com/Ingarnm), [@Vamp1rk0](https://github.com/Vamp1rk0) for feedback
