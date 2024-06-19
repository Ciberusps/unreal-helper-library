# Unreal Helper Library [UHL]

## Install

From source:

- add git submodule to your plugins folder using - `git submodule add https://github.com/Ciberusps/unreal-helper-library.git ./Plugins/UnrealHelperLibrary`
- add note to install instruction(`README.md`) on how to update git submodules - `git submodule update --init --recursive`

From marketplace: later this year

## Documentation

> - GAS
>   - Components
>     - [AbilitySystemComponent](#UHLAbilitySystemComponent)
>   - Tasks
>     - InterpolateToPosition
> - AI
>   - Components
>     - [AIPerceptionComponent](#UHLAIPerceptionComponent)
>   - Composite
>     - RandomSelector
>   - Decorators
>     - CheckGASGameplayTagsOnActor
>     - InAngle
>     - InRange
>     - LoopRandomCount
>     - RandomChance
>     - TimeLimitRandom
>   - Services
>     - GameplayFocus
>   - Tasks
>     - SetBBValue
>     - DebugPrintBBValue
>     - DebugPrintString
>     - InvokeGameplayAbility
>     - PlayAnimMontage
> - UnrealHelperLibraryBPLibrary
>   - RelativeAngleToActor
>   - CreateGenericGASGameplayEffectSpec
>   - GetProjectVersion
>   - GetNamesOfComponentsOnObject
>   - GetAssetsOfClass

<a name="UHLAbilitySystemComponent"></a>

#### `UHLAbilitySystemComponent`

with ability to

- `InitialGameplayAttributes`
- `Abilities` given on start
- `InitialActiveAbilities`,
- `InitialGameplayTags`

<a name="UHLAIPerceptionComponent"></a>

#### `UHLAIPerceptionComponent`

with ability to subscribe on

- `OnSightStimulusExpired`

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

<a name="RelativeAngleToActor"></a>

#### `RelativeAngleToActor`

<a name="CreateGenericGASGameplayEffectSpec"></a>

#### `CreateGenericGASGameplayEffectSpec`

<a name="GetProjectVersion"></a>

#### `GetProjectVersion`

<a name="GetNamesOfComponentsOnObject"></a>

#### `GetNamesOfComponentsOnObject`

<a name="GetAssetsOfClass"></a>

#### `GetAssetsOfClass`
