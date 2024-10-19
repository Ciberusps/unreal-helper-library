// Copyright (c) 2024 Pavel Penkov


#include "AI/Data/TurnSettings.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TurnSettings)

void FTurnSettings::Cleanup()
{
    TurnRangesGroups.Empty();
}

void FTurnSettings::SetupPreset_Default_90_180()
{
    bTurnOnlyWithAnims = false;
    bStopMontageOnGoalReached = true;
    // order is important because ranges might overlap
    TurnRangesGroups = {
        {
            "180deg",
            {
                {
                    { "Turn 180 L", FFloatRange(-225, -115) },
                    { "Turn 180 R", FFloatRange(115, 225) },
                }
            }
        },
        {
            "90deg",
            {
				{
				    { "Turn 90 L", FFloatRange(-115, -45) },
                    { "Turn 90 R", FFloatRange(45, 115) },
                }
            }
        },
    };
}

void FTurnSettings::SetupPreset_BigEnemy_90_180()
{
    bTurnOnlyWithAnims = true;
    bStopMontageOnGoalReached = true;
    // order is important because ranges might overlap
    TurnRangesGroups = {
        {
            "180deg",
            {
                {
					{ "Turn 180 L", FFloatRange(-225, -135) },
					{ "Turn 180 R", FFloatRange(135, 225) },
                }
            }
        },
		{
			"90deg",
			{
				{
					{ "Turn 90 L", FFloatRange(-135, -45) },
					{ "Turn 90 R", FFloatRange(45, 135) },
				}
			}
		},
    };
}

void FTurnSettings::SetupPreset_45_90_180()
{
    bTurnOnlyWithAnims = true;
    bStopMontageOnGoalReached = true;
    // order is important because ranges might overlap
    TurnRangesGroups = {
        {
            "180deg",
            {
                {
                    { "Turn 180 L", FFloatRange(-225, -135) },
                    { "Turn 180 R", FFloatRange(135, 225) },
                }
            }
        },
        {
            "90deg",
            {
				{
				    { "Turn 90 L", FFloatRange(-135, -67.5) },
                    { "Turn 90 R", FFloatRange(67.5, 135) },
                }
            }
        },
		{
			"45deg",
			{
				{
					{ "Turn 45 L", FFloatRange(-67.5, -22.5) },
					{ "Turn 45 R", FFloatRange(22.5, 67.5) },
				}
			}
		},
    };
}

void FTurnSettings::SetupPreset_15_45_90_180()
{
    bTurnOnlyWithAnims = true;
    bStopMontageOnGoalReached = true;
    // order is important because ranges might overlap
    TurnRangesGroups = {
        {
            "180deg",
            {
                {
                    { "Turn 180 L", FFloatRange(-225, -135) },
                    { "Turn 180 R", FFloatRange(135, 225) },
                }
            }
        },
        {
            "90deg",
            {
				{
				    { "Turn 90 L", FFloatRange(-135, -67.5) },
                    { "Turn 90 R", FFloatRange(67.5, 135) },
                }
            }
        },
        {
            "45deg",
            {
				{
				    { "Turn 45 L", FFloatRange(-67.5, -30) },
                    { "Turn 45 R", FFloatRange(30, 67.5) },
                }
            }
        },
		{
			"15deg",
			{
				{
					{ "Turn 15 L", FFloatRange(-30, -7.5) },
					{ "Turn 15 R", FFloatRange(7.5, 30) },
				}
			}
		},
    };
}

void FTurnSettings::SetupPreset_15_30_45_90_180()
{
    bTurnOnlyWithAnims = true;
    bStopMontageOnGoalReached = true;
    TurnRangesGroups = {
            {
                "180deg",
                {
                    {
                        { "Turn 180 L", FFloatRange(-225, -135) },
                        { "Turn 180 R", FFloatRange(135, 225) },
                    }
                }
            },
            {
                "90deg",
                {
					{
						{ "Turn 90 L", FFloatRange(-135, -67.5) },
						{ "Turn 90 R", FFloatRange(67.5, 135) },
					}
                }
            },
            {
                "45deg",
                {
					{
						{ "Turn 45 L", FFloatRange(-67.5, -30) },
						{ "Turn 45 R", FFloatRange(30, 67.5) },
					}
                }
            },
			{
				"30deg",
				{
					{
						{ "Turn 30 L", FFloatRange(-37.5, -22.5) },
						{ "Turn 30 R", FFloatRange(22.5, 37.5) },
					}
				}
			},
    		{
    			"15deg",
    			{
					{
						{ "Turn 15 L", FFloatRange(-22.5, -7.5) },
						{ "Turn 15 R", FFloatRange(7.5, 22.5) },
					}
    			}
    		},
        };
}

void UTurnSettingsDataAsset::Cleanup()
{
    TurnSettings.Cleanup();
}

void UTurnSettingsDataAsset::SetupPreset_Default_90_180()
{
    TurnSettings.SetupPreset_Default_90_180();
}

void UTurnSettingsDataAsset::SetupPreset_BigEnemy_90_180()
{
    TurnSettings.SetupPreset_BigEnemy_90_180();
}

void UTurnSettingsDataAsset::SetupPreset_45_90_180()
{
    TurnSettings.SetupPreset_45_90_180();
}

void UTurnSettingsDataAsset::SetupPreset_15_45_90_180()
{
    TurnSettings.SetupPreset_15_45_90_180();
}

void UTurnSettingsDataAsset::SetupPreset_15_30_45_90_180()
{
    TurnSettings.SetupPreset_15_30_45_90_180();
}
