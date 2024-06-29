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

<a name="BTD_CheckGASGameplayTagsOnActor"></a>

#### `BTD_CheckGASGameplayTagsOnActor`

<a name="BTD_InAngle"></a>

#### `BTD_InAngle`

<a name="BTD_InRange"></a>

#### `BTD_InRange`

<a name="BTD_LoopRandomCount"></a>

#### `BTD_LoopRandomCount`

<a name="BTD_RandomChance"></a>

#### `BTD_RandomChance`

<a name="BTD_TimeLimitRandom"></a>

#### `BTD_TimeLimitRandom`

<a name="BTS_GameplayFocus"></a>

#### `BTS_GameplayFocus`

<a name="BTT_SetBBValue"></a>

#### `BTT_SetBBValue`

<a name="BTT_DebugPrintBBValue"></a>

#### `BTT_DebugPrintBBValue`

<a name="BTT_DebugPrintString"></a>

#### `BTT_DebugPrintString`

<a name="BTT_InvokeGameplayAbility"></a>

#### `BTT_InvokeGameplayAbility`

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

<a name="GetPointAtRelativeDirection"></a>

#### `GetPointAtRelativeDirection`

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
