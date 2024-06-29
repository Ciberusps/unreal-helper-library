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

## Documentation

> - [GAS](#gas)
>   - Components
>     - [AbilitySystemComponent](#uhlabilitysystemcomponent)
>   - Tasks
>     - [InterpolateToPosition](interpolatetoposition)
> - [AI](#ai)
>   - Components
>     - [AIPerceptionComponent](#uhlaiperceptioncomponent)
>   - Composite
>     - [RandomSelector](#btc_randomselector)
>   - Decorators
>     - [CheckGASGameplayTagsOnActor](#btd_checkgasgameplaytagsonactor)
>     - [InAngle](#btd_inangle)
>     - [InRange](#btd_inrange)
>     - [LoopRandomCount](#btd_looprandomcount)
>     - [RandomChance](#btd_randomchance)
>     - [TimeLimitRandom](#btd_timelimitrandom)
>   - Services
>     - [GameplayFocus](#bts_gameplayfocus)
>   - Tasks
>     - [SetBBValue](#btt_setbbvalue)
>     - [DebugPrintBBValue](#btt_debugprintbbvalue)
>     - [DebugPrintString](#btt_debugprintstring)
>     - [InvokeGameplayAbility](#btt_invokegameplayability)
>     - [PlayAnimMontage](#btt_playanimmontage)
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

<a name="GAS"></a>

### GAS

<a name="UHLAbilitySystemComponent"></a>

#### `UHLAbilitySystemComponent`

with ability to

- `InitialGameplayAttributes`
- `Abilities` given on start
- `InitialActiveAbilities`,
- `InitialGameplayTags`

<a name="InterpolateToPosition"></a>

#### `InterpolateToPosition`

AT_InterpolateToPosition

<a name="UHLAIPerceptionComponent"></a>

#### `UHLAIPerceptionComponent`

with ability to subscribe on

- `OnSightStimulusExpired`

---

<a name="AI"></a>

### AI

<a name="BTC_RandomSelector"></a>

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

<a name="BTD_CheckGASGameplayTagsOnActor"></a>

#### `BTD_CheckGASGameplayTagsOnActor`

<a name="BTD_InAngle"></a>

#### `BTD_InAngle`

<a name="BTD_InRange"></a>

#### `BTD_InRange`

Decorator to check distance between actors. Compliant with "MoveTo" node have same settings `bIncludeSelfCapsuleRadius` and `bIncludeTargetCapsuleRadius` to check distance excluding capsules radiuses

![InRange](https://github.com/Ciberusps/unreal-helper-library/assets/14001879/e32b5d05-de82-4dfb-80d1-539c866008ff)

<a name="BTD_LoopRandomCount"></a>

#### `BTD_LoopRandomCount`

<a name="BTD_RandomChance"></a>

#### `BTD_RandomChance`

<a name="BTD_TimeLimitRandom"></a>

#### `BTD_TimeLimitRandom`

<a name="BTS_GameplayFocus"></a>

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

<a name="BTT_SetBBValue"></a>

#### `SetBBValue`

**BTT_SetBBValue** - helps settings values in blackboard, supports all blackboard types and for some values event provides opportunity to make calculations like `int`

![image](https://github.com/Ciberusps/unreal-helper-library/assets/14001879/baf4ea25-5d19-482d-a60c-799663def759)

<a name="BTT_DebugPrintBBValue"></a>

#### `DebugPrintBBValue`

**BTT_DebugPrintBBValue** -

![image](https://github.com/Ciberusps/unreal-helper-library/assets/14001879/d138c011-fc9a-438e-bd39-658480cd95bf)

<a name="BTT_DebugPrintString"></a>

#### `DebugPrintString`

**BTT_DebugPrintString** - simple task for printing debug info on screen

![image](https://github.com/Ciberusps/unreal-helper-library/assets/14001879/510e9766-37be-4f43-a60f-e0e012521841)

<a name="BTT_InvokeGameplayAbility"></a>

#### `InvokeGameplayAbility`



<a name="BTT_PlayAnimMontage"></a>

#### `BTT_PlayAnimMontage`


<a name="UnrealHelperLibraryBPL"></a>

---

### UnrealHelperLibraryBPL

#### > RelativeAngles

<a name="RelativeAngleToActor"></a>

#### `RelativeAngleToActor`

<a name="GetPointAtRelativeAngle"></a>

#### `GetPointAtRelativeAngle`

![image](https://github.com/Ciberusps/unreal-helper-library/assets/14001879/4c2f5e47-9f5c-4e70-8d77-57cb7383290a)
![image](https://github.com/Ciberusps/unreal-helper-library/assets/14001879/c0026be0-ce3a-4ee5-84e6-b8e90474fdb9)

<a name="GetPointAtRelativeDirection"></a>

#### `GetPointAtRelativeDirection`

![image](https://github.com/Ciberusps/unreal-helper-library/assets/14001879/c1ea6dda-4ccf-4441-a7d0-dc83ac977a9c)
![image](https://github.com/Ciberusps/unreal-helper-library/assets/14001879/a33a93ad-f470-4dec-8c2c-6c76b275207f)

<a name="GetPointAtRelativeAngleBetweenActors"></a>

#### `GetPointAtRelativeAngleBetweenActors`



<a name="GetPointAtRelativeDirectionBetweenActors"></a>

#### `GetPointAtRelativeDirectionBetweenActors`

<a name="DirectionToAngle"></a>

#### `DirectionToAngle`

#### > GAS

<a name="CreateGenericGASGameplayEffectSpec"></a>

#### `CreateGenericGASGameplayEffectSpec`

<a name="GetProjectVersion"></a>

#### > Misc

#### `GetProjectVersion`

<a name="GetNamesOfComponentsOnObject"></a>

#### `GetNamesOfComponentsOnObject`

<a name="GetAssetsOfClass"></a>

#### `GetAssetsOfClass`

#### > Other

<a name="GetHighestPoint"></a>

#### `GetHighestPoint`
