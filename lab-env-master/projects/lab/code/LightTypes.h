#pragma once

#include "vector3D.h"

struct BaseLight
{
	vector3D Color;
	float AmbientIntensity;
	float DiffuseIntensity;

	BaseLight()
	{
		Color = vector3D(0.0f, 0.0f, 0.0f);
		AmbientIntensity = 0.0f;
		DiffuseIntensity = 0.0f;
	}
};

struct DirectionalLight : public BaseLight
{
	vector3D Direction;

	DirectionalLight()
	{
		Direction = vector3D(0.0f, 0.0f, 0.0f);
	}
};

struct PointLight : public BaseLight
{
	vector3D Position;

	struct
	{
		float Constant;
		float Linear;
		float Exp;
	} Attenuation;

	PointLight()
	{
		Position = vector3D(0.0f, 0.0f, 0.0f);
		Attenuation.Constant = 1.0f;
		Attenuation.Linear = 0.0f;
		Attenuation.Exp = 0.0f;
	}
};

struct SpotLight : public PointLight
{
	vector3D Direction;
	float Cutoff;

	SpotLight()
	{
		Direction = vector3D(0.0f, 0.0f, 0.0f);
		Cutoff = 0.0f;
	}
};