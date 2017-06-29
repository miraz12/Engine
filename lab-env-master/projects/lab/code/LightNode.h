#pragma once
#include "vector3D.h"
#include "LightTypes.h"
#include "matrix3D.h"
#include "GL/glew.h"


class LightNode
{
public:
	static const unsigned int MAX_POINT_LIGHTS = 2;
	static const unsigned int MAX_SPOT_LIGHTS = 2;

	LightNode();
	~LightNode();

	vector3D getPos();
	void setPos(vector3D);

	vector3D getColor();
	void setColor(vector3D);

	float getIntensity();
	void setIntensity(float);

	PointLight* m_pLights;
	SpotLight* m_sLights;

	matrix3D m_matSpecularIntensityLocation;
	matrix3D m_matSpecularPowerLocation;
	int m_numPointLightsLocation;
	int m_numSpotLightsLocation;

private:


	vector3D color;
	vector3D pos;
	float intens;





};

