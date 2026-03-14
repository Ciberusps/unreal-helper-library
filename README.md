# Unreal Helper Library [UHL]

**UHL** - unreal helper library, toolset to help developers working with AI, GAS, customizing editor and so on.
Goal is to became a tool that insta-installed on new project creation.

**UPDATE**: With UHL v2.0 update its set of plugins that can be installed independently:
- [UHL](#-uhl)
- [UHLGAS](#-gas)
- [UHLAI](#-ai)
  - [UHLBehaviorTree](#-ai)
  - [UHLStateTree](#-ai)
- [UHLCharacter](#-character)
- [UHLCommonMaps](#️-commonmaps)
- [UHLDebugSystem](#-debugsystem)

Support: `UE5.6` (at least)

## 🚀 Install & ⬆️ Update

### Install this repository with all UHL plugins

```bash
// install as git submodule to your plugins folder
git submodule add https://github.com/Ciberusps/unreal-helper-library.git ./Plugins/UnrealHelperLibrary

// to update plugin
git submodule update --init --recursive --checkout
```

### Or install all UHL plugins as independent plugins

```bash
git submodule add https://github.com/Ciberusps/UHL.git ./Plugins/UHL
git submodule add https://github.com/Ciberusps/UHLGAS.git ./Plugins/UHLGAS
git submodule add https://github.com/Ciberusps/UHLAI.git ./Plugins/UHLAI
git submodule add https://github.com/Ciberusps/UHLStateTree.git ./Plugins/UHLStateTree
git submodule add https://github.com/Ciberusps/UHLBehaviorTree.git ./Plugins/UHLBehaviorTree
git submodule add https://github.com/Ciberusps/UHLCharacter.git ./Plugins/UHLCharacter
git submodule add https://github.com/Ciberusps/UHLCommonMaps.git ./Plugins/UHLCommonMaps
git submodule add https://github.com/Ciberusps/UHLDebugSystem.git ./Plugins/UHLDebugSystem

// to update plugins
git submodule update --init --remote --recursive
```

> [!NOTE]
> Don't forget to update your `README.md` with instructions on how to setup - `git submodule update --init --recursive` and how to update submodules/plugin(s) - `git submodule update --remote`

> [!NOTE]
> Add `Editor Preferences -> Force Compilation on Startup` in `Config/EditorPerProjectUserSettings.ini` your team don't want to recompile plugin manually 😉

### Or install from marketplace optionally

https://www.fab.com/listings/9f7d82e9-bc72-42ff-b302-b3d6562bd4c8

## 🧩 Plugins

Click on headers to know more about plugins

### [🧠 AI](#-ai)

Basic utils for creating AI that most projects needs have two version for BehaviorsTrees/StateTrees with same functionality contains AI nodes for [UHLBehaviorTrees](https://github.com/Ciberusps/UHLBehaviorTrees) & [UHLStateTree](https://github.com/Ciberusps/UHLStateTree)
  - `SetGameplayFocus` - set focus on actor (Move/Gameplay)
  - `InRange` - check distance to enemy
  - `InAngle` - check angle to enemy
  - `PlayAnimMontage` - play attack animation
  - `InvokeGameplayAbility` - activate/deactivate GAS Gameplay Ability by tag, with optional "wait for finishing"
  - `TurnTo` - turn enemy using turn animations

![AiModule](https://github.com/user-attachments/assets/4becb592-c02e-423a-bf80-fcfc629ce518)

### [💪 GAS](https://github.com/Ciberusps/UHLGAS)

Gameplay Ability System - [Lyra](https://dev.epicgames.com/documentation/en-us/unreal-engine/lyra-sample-game-in-unreal-engine?application_version=5.5) based plugin - Lyra inputs, input cache, attribute set, ability system config, ability sets. GAS features provides much smoother GAS experience mostly based on.
All GAS features designed in mind that they or their part can be added or dropped by you in development in any time and replaced by something custom that fits your project needs

![UHLAbilitySystemComponent](https://github.com/user-attachments/assets/fe56c7a2-43e6-484a-ac65-635e3670204e)

GAS scaled for medium+ teams by AbilitySets/AbilitySystemConfig separated

![image](https://github.com/user-attachments/assets/d0a47867-dacc-4fbf-bd52-dafa70c8939d)

### [🪲 DebugSystem](https://github.com/Ciberusps/UHLDebugSystem)

Gameplay DebugCategories ability-like System. Mostly limited set of gameplay debugging tools being used, so UHLDebugSystem makes it easier to creating/enabling your gameplay DebugCategories by providing ability-like system, you can control that one DebugCategory disables another category by tags.

### [🗺️ CommonMaps](https://github.com/Ciberusps/UHLCommonMaps)

Literaly button with common maps thats all. Based on Lyra CommonMaps(EditorPlugin) but extended with ability to select folder with maps

<img width="641" height="433" alt="image" src="https://github.com/user-attachments/assets/0b393391-c3fd-4543-93a7-41c8962d265e" />


### [📚 UHL](#UHL)
- **UnrealHelperLibrary** - main module with GAS helper classes, AI behavior tree nodes, Blueprint Function Libraries. Most functionality can be tested in `Gyms`(maps for testing atomic/single gameplay mechanic), all `Gyms` located in `/Plugins/UnrealHelperLibrary/Content/Gyms`
- [UHL](https://github.com/Ciberusps/UHL) - utility functions like `GetProjectVersion`, `GetAssetsOfClass`, `GetHighestPoint`. Editor - editor customization, e.g. `Custom thumnails`, `Custom class icon`
- **UHL Utils (EditorUtilityWidget)** - widget with tools helping you make trivial things, like `ConvertToORM` quite often task when you want to combine 3 textures `Occlusion`, `Roughness`, `Metalic` in one ORM texture
- **UnrealHelperEditor** - optional module with editor customization, e.g. custom thumnails, custom class icons
---

### [🦸 Character](#character)

Default character class with UHL interfaces implemented, so you don't need to do it by yourself

## 🔃 Migration to UHL 2.0 (NEW multi-plugins version)

if already have plugin
- checkout main
- commit UnrealHelperLibrary change
- run command - `git submodule update --init --recursive`

- UHL CommonMaps - in `Config/DefaultGame` rename section from [/Script/CommonMaps.UHLCommonMapsDeveloperSettings] -> [/Script/UHLCommonMaps.UHLCommonMapsDeveloperSettings]

### Install separately

```C++
git submodule add https://github.com/Ciberusps/UHL.git ./Plugins/UHL
git submodule add https://github.com/Ciberusps/UHLAI.git ./Plugins/UHLAI
git submodule add https://github.com/Ciberusps/UHLBehaviorTree.git ./Plugins/UHLBehaviorTree
git submodule add https://github.com/Ciberusps/UHLStateTree.git ./Plugins/UHLStateTree
git submodule add https://github.com/Ciberusps/UHLCharacter.git ./Plugins/UHLCharacter
git submodule add https://github.com/Ciberusps/UHLCommonMaps.git ./Plugins/UHLCommonMaps
git submodule add https://github.com/Ciberusps/UHLDebugSystem.git ./Plugins/UHLDebugSystem
git submodule add https://github.com/Ciberusps/UHLGAS.git ./Plugins/UHLGAS
```

### Changelog

- all modules moved to separate plugins and made as independent as possible now dependency tree looks like
- dependency tree:
  - UHLAI
    - UHLBehaviorTree
    - UHLStateTree
    - UHLCharacter
  - UHLGAS
    - UHLCharacter
  - UHL
    - UHLCharacter
  - UHLCommonMaps
  - UHLDebugSystem
  - UHLCharacter
- UHLSettings moved to UHLAISettings now UHLStateTree and UHLBehaviorTree independent from core "UHL" plugin
and can be used completly separatly from it. UHLStateTree and UHLBehaviorTree depends only on UHLAI

## Special Thanks

[@Ingarnm](https://github.com/Ingarnm), [@Vamp1rk0](https://github.com/Vamp1rk0) for feedback
