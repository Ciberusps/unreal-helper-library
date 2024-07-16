// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Data/TurnSettings.h"

void FTurnSettings::Cleanup()
{
    TurnRangesGroups.Empty();
}

void FTurnSettings::SetupPreset_90_180()
{
    // order is important because ranges might overlap
    TurnRangesGroups = {
        {
            "180deg",
            {
                {
					{ "Turn 180 L", FFloatRange(-225, -135), nullptr },
					{ "Turn 180 R", FFloatRange(135, 225), nullptr },
                }
            }
        },
		{
			"90deg",
			{
				{
					{ "Turn 90 L", FFloatRange(-135, -45), nullptr },
					{ "Turn 90 R", FFloatRange(45, 135), nullptr },
				}
			}
		},
    };
}

void FTurnSettings::SetupPreset_45_90_180()
{
    // order is important because ranges might overlap
    TurnRangesGroups = {
        {
            "180deg",
            {
                {
                    { "Turn 180 L", FFloatRange(-225, -135), nullptr },
                    { "Turn 180 R", FFloatRange(135, 225), nullptr },
                }
            }
        },
        {
            "90deg",
            {
				{
				    { "Turn 90 L", FFloatRange(-135, -67.5), nullptr },
                    { "Turn 90 R", FFloatRange(67.5, 135), nullptr },
                }
            }
        },
		{
			"45deg",
			{
				{
					{ "Turn 45 L", FFloatRange(-67.5, -22.5), nullptr },
					{ "Turn 45 R", FFloatRange(22.5, 67.5), nullptr },
				}
			}
		},
    };
}

void FTurnSettings::SetupPreset_15_45_90_180()
{
    // order is important because ranges might overlap
    TurnRangesGroups = {
        {
            "180deg",
            {
                    {
                        { "Turn 180 L", FFloatRange(-225, -135), nullptr },
                        { "Turn 180 R", FFloatRange(135, 225), nullptr },
                    }
            }
        },
        {
            "90deg",
            {
				    {
				        { "Turn 90 L", FFloatRange(-135, -67.5), nullptr },
                        { "Turn 90 R", FFloatRange(67.5, 135), nullptr },
                    }
            }
        },
        {
            "45deg",
            {
				    {
				        { "Turn 45 L", FFloatRange(-67.5, -30), nullptr },
                        { "Turn 45 R", FFloatRange(30, 67.5), nullptr },
                    }
            }
        },
		{
			"15deg",
			{
					{
						{ "Turn 15 L", FFloatRange(-30, -7.5), nullptr },
						{ "Turn 15 R", FFloatRange(7.5, 30), nullptr },
					}
			}
		},
    };
}

void FTurnSettings::SetupPreset_15_30_45_90_180()
{
    TurnRangesGroups = {
            {
                "180deg",
                {
                        {
                            { "Turn 180 L", FFloatRange(-225, -135), nullptr },
                            { "Turn 180 R", FFloatRange(135, 225), nullptr },
                        }
                }
            },
            {
                "90deg",
                {
    				    {
    				        { "Turn 90 L", FFloatRange(-135, -67.5), nullptr },
                            { "Turn 90 R", FFloatRange(67.5, 135), nullptr },
                        }
                }
            },
            {
                "45deg",
                {
    				    {
    				        { "Turn 45 L", FFloatRange(-67.5, -30), nullptr },
                            { "Turn 45 R", FFloatRange(30, 67.5), nullptr },
                        }
                }
            },
			{
				"30deg",
				{
						{
							{ "Turn 30 L", FFloatRange(-37.5, -22.5), nullptr },
							{ "Turn 30 R", FFloatRange(22.5, 37.5), nullptr },
						}
				}
			},
    		{
    			"15deg",
    			{
    					{
    						{ "Turn 15 L", FFloatRange(-22.5, -7.5), nullptr },
    						{ "Turn 15 R", FFloatRange(7.5, 22.5), nullptr },
    					}
    			}
    		},
        };
}

void UTurnToAnimationsDataAsset::Cleanup()
{
    TurnSettings.Cleanup();
}

void UTurnToAnimationsDataAsset::SetupPreset_Recommended_90_180()
{
    TurnSettings.SetupPreset_90_180();
}

void UTurnToAnimationsDataAsset::SetupPreset_45_90_180()
{
    TurnSettings.SetupPreset_45_90_180();
}

void UTurnToAnimationsDataAsset::SetupPreset_15_45_90_180()
{
    TurnSettings.SetupPreset_15_45_90_180();
}

void UTurnToAnimationsDataAsset::SetupPreset_15_30_45_90_180()
{
    TurnSettings.SetupPreset_15_30_45_90_180();
}
