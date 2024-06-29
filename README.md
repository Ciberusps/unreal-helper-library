# Unreal Helper Library [UHL]

UHL - unreal helper library, toolset to help developers working with AI, GAS and so on.
Goal is to became a tool that insta-installed on new project creation. All tools are mostly tested on melee combat so if you have other background and think that something should work another way or have an idea on how to improve developer experience feel free to [discuss](https://github.com/Ciberusps/unreal-helper-library/discussions)

Support: tested `UE5.3 - UE5.4`

## Install

From source (recommended):

- add git submodule to your plugins folder using - `git submodule add https://github.com/Ciberusps/unreal-helper-library.git ./Plugins/UnrealHelperLibrary`
- add note to install instruction(`README.md`) on how to update git submodules - `git submodule update --init --recursive`
- to use in C++ add `"UnrealHelperLibrary"` to file `"<ProjectName>.Build.cs"` in `PublicDependencyModuleNames` section

From marketplace: later this year

## Update

From source:

- `git submodule update --remote` to update library from source

## Documentation

> - [GAS](#gas)
>   - Components
>     - [AbilitySystemComponent](#abilitysystemcomponent)
>   - Tasks
>     - [InterpolateToPosition](interpolatetoposition)
> - [AI](#ai)
>   - Components
>     - [AIPerceptionComponent](#uhlaiperceptioncomponent)
>   - Composite
>     - [RandomSelector](#btc_randomselector)
>   - Decorators
>     - [CheckGASGameplayTagsOnActor](#checkgasgameplaytagsonactor)
>     - [InAngle](#btd_inangle)
>     - [InRange](#btd_inrange)
>     - [LoopRandomCount](#looprandomcount)
>     - [RandomChance](#randomchance)
>     - [TimeLimitRandom](#timelimitrandom)
>   - Services
>     - [GameplayFocus](#setgameplayfocus)
>   - Tasks
>     - [SetBBValue](#setbbvalue)
>     - [DebugPrintBBValue](#debugprintbbvalue)
>     - [DebugPrintString](#debugprintstring)
>     - [InvokeGameplayAbility](#invokegameplayability)
>     - [PlayAnimMontage](#playanimmontage)
> - [UnrealHelperLibraryBPL](#unrealhelperlibrarybpl)
>   - RelativeAngles
>     - [RelativeAngleToActor](#relativeangletoactor)
>     - [GetPointAtRelativeAngle](#getpointatrelativeangle)
>     - [GetPointAtRelativeDirection](#getpointatrelativedirection)
>     - [GetPointAtRelativeAngleBetweenActors](#getpointatrelativeanglebetweenactors)
>     - [GetPointAtRelativeDirectionBetweenActors](#getpointatrelativedirectionbetweenactors)
>     - [DirectionToAngle](#directiontoangle)
>   - GAS
>     - [CreateGenericGASGameplayEffectSpec](#creategenericgasgameplayeffectspec)
>   - Misc
>     - [GetProjectVersion](#getprojectversion)
>     - [GetNamesOfComponentsOnObject](#getnamesofcomponentsonobject)
>     - [GetAssetsOfClass](#getassetsofclass)
>     - GetActorComponentByName
>     - GetSceneComponentByName
>   - Other
>     - [GetHighestPoint](#gethighestpoint)

---

### GAS

#### `AbilitySystemComponent`

![AbilitySystemComponent](https://github.com/Ciberusps/unreal-helper-library/assets/14001879/fc6b751f-fc5d-4394-b133-aea69c9034c5)

**UHLAbilitySystemComponent** - for quick start with GAS. You can nest from it on start and than turn off its functions when you ready to replace them with your custom solution.

With abilities to

- set `InitialGameplayAttributes`
- give `Abilities` on start
- activate `InitialActiveAbilities`
- apply `InitialGameplayTags`

#### `InterpolateToPosition`

**AT_InterpolateToPosition** - interpolate actor to specified position/rotation at a predetermined amount of time

![image](https://github.com/Ciberusps/unreal-helper-library/assets/14001879/764ddf72-595e-4316-9149-b7b0accc2b89)

#### `UHLAIPerceptionComponent`

<!-- with ability to subscribe on

- `OnSightStimulusExpired` -->

---

### AI

#### `BTC_RandomSelector`

Randoms between child node by weights

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
> Don't mess with `UBTDecorator_CheckGameplayTagsOnActor` - its only checks GameplayTags on actor itself not on AbilitySystem.

Requirements:

- actor should implement `IAbilitySystemInterface` to get AbilitySystemComponent

![image](https://github.com/Ciberusps/unreal-helper-library/assets/14001879/f1581009-b9cd-408f-84de-2475b43012ae)

#### `InAngle`

**BTD_InAngle** - decorator to check is enemy in of specified angle ranges. Useful for big enemies to detect what attack for example dragon should do if player under the right wing or leg, is player in front of dragon or behind and so on.

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

**BTS_SetGameplayFocus** - alternative for "Set default focus". SetGameplayFocus made right way.
One of most common problems that anybody stucks when starting developing AI - "focus dont work"/"focus works wrong". Prevents rotation jittering while enemy rotation

![image](https://github.com/Ciberusps/unreal-helper-library/assets/14001879/c0add45c-76ef-44bc-b97c-0c56901e6e03)
![GameplayFocus](https://github.com/Ciberusps/unreal-helper-library/assets/14001879/0126fc2b-8a20-4b61-93d8-b1ead6802057)

Requirements:

- turn on `UseControllerDesiredRotation`
- turn off
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

---

### UnrealHelperLibraryBPL

#### > RelativeAngles

#### `RelativeAngleToActor`

![image](https://github.com/Ciberusps/unreal-helper-library/assets/14001879/4a695df4-9583-451c-801b-98e63c8ad5c8)

#### `GetPointAtRelativeAngle`

![image](https://github.com/Ciberusps/unreal-helper-library/assets/14001879/4c2f5e47-9f5c-4e70-8d77-57cb7383290a)
![image](https://github.com/Ciberusps/unreal-helper-library/assets/14001879/c0026be0-ce3a-4ee5-84e6-b8e90474fdb9)

#### `GetPointAtRelativeDirection`

![image](https://github.com/Ciberusps/unreal-helper-library/assets/14001879/c1ea6dda-4ccf-4441-a7d0-dc83ac977a9c)
![image](https://github.com/Ciberusps/unreal-helper-library/assets/14001879/a33a93ad-f470-4dec-8c2c-6c76b275207f)

#### `GetPointAtRelativeAngleBetweenActors`

#### `GetPointAtRelativeDirectionBetweenActors`

#### `DirectionToAngle`

#### > GAS

#### `CreateGenericGASGameplayEffectSpec`

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
