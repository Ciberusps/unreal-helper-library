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

## ✨ Plugins and features

- [UHL](https://github.com/Ciberusps/UHL) - utility functions like `GetProjectVersion`, `GetAssetsOfClass`, `GetHighestPoint`. Editor - editor customization, e.g. `Custom thumnails`, `Custom class icon`
- [UHLGAS](https://github.com/Ciberusps/UHLGAS) -
- [UHLAI](https://github.com/Ciberusps/UHLAI) -
- [UHLBehaviorTrees](https://github.com/Ciberusps/UHLBehaviorTrees) & [UHLStateTree](https://github.com/Ciberusps/UHLStateTree) - AI nodes
  - `SetGameplayFocus` - set focus on actor
  - `InRange` - check distance to enemy
  - `InAngle` - check angle to enemy
  - `PlayAnimMontage` - play attack animation
  - `InvokeGameplayAbility` - activate/deactivate GAS Gameplay Ability by tag, with optional "wait for finishing"
  - `TurnTo` - turn enemy using turn animations
- [UHLCommonMaps](https://github.com/Ciberusps/UHLCommonMaps)
- [UHLDebugSystem](https://github.com/Ciberusps/UHLDebugSystem)


## 🚀 Install & ⬆️ Update

### From source (recommended)

```bash
// install as git submodule to your plugins folder
git submodule add https://github.com/Ciberusps/unreal-helper-library.git ./Plugins/UnrealHelperLibrary

// to update plugin
git submodule update --init --recursive --checkout
```

or install all plugins as independent plugins

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

### From marketplace

https://www.fab.com/listings/9f7d82e9-bc72-42ff-b302-b3d6562bd4c8

## 🧩 Modules

UHL consists of 3 modules:

- **UnrealHelperLibrary** - main module with GAS helper classes, AI behavior tree nodes, Blueprint Function Libraries. Most functionality can be tested in `Gyms`(maps for testing atomic/single gameplay mechanic), all `Gyms` located in `/Plugins/UnrealHelperLibrary/Content/Gyms`
- **UnrealHelperEditor** - optional module with editor customization, e.g. custom thumnails, custom class icons
- **UHL Utils (EditorUtilityWidget)** - widget with tools helping you make trivial things, like `ConvertToORM` quite often task when you want to combine 3 textures `Occlusion`, `Roughness`, `Metalic` in one ORM texture

## 📄 Documentation

**[🧠 AI](#-ai)**

AI nodes for behavior tree, based on `BehaviorTree` and `BehaviorTreeComponent` from `UE5.4` and `UE5.5` with some improvements and additional features

**[💪 GAS](#-gas-gameplay-ability-system)**

Gameplay Ability System - Lyra based inputs, ability system component, ability system config, input cache, attribute set, ability sets


**[🦸 Character](#character)**

Default character class with UHL interfaces implemented, so you don't need to do it by yourself


**[🪲 DebugSystem](#-debugsystem)**

Debug system for your game, in mid-size commands you always use limited set of debugging tools

**[📚 UnrealHelperLibrary](#unrealhelperlibrarybpl)**


**[⌨ Editor](#-uhl-editor)**

---

### Migration to UHL 2.0, NEW multi-plugins version

if already have plugin
- checkout main
- commit UnrealHelperLibrary change
- run command - `git submodule update --init --recursive`

- UHL CommonMaps - in `Config/DefaultGame` rename section from [/Script/CommonMaps.UHLCommonMapsDeveloperSettings] -> [/Script/UHLCommonMaps.UHLCommonMapsDeveloperSettings]

#### Install separately

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
- **TODO: draw dependency tree using schemas**
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
-

## Special Thanks

[@Ingarnm](https://github.com/Ingarnm), [@Vamp1rk0](https://github.com/Vamp1rk0) for feedback
