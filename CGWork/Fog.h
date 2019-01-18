#pragma once

enum FogBlendingFunction
{
	FOG_BLENDING_LINEAR,
	FOG_BLENDING_EXP,
	FOG_BLENDING_EXP_SQUARED
};

struct FogParams
{
	bool IsEnabled;
	int Color[3];
	double MinFogDistance;
	double MaxFogDistance;
	FogBlendingFunction BlendingFunction;
	double Exponent;
	bool IsPortal;
	double PortalThreshold;

	FogParams()
		: IsEnabled(false), MinFogDistance(10.0), MaxFogDistance(30.0),
		BlendingFunction(FOG_BLENDING_LINEAR), Exponent(0.33),
		IsPortal(false), PortalThreshold(0.5)
	{
		Color[0] = 100;
		Color[1] = 100;
		Color[2] = 100;
	}
};