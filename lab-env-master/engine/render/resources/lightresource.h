#pragma once
#include "core/math/vector3D.h"
#include "render/resources/lighttypes.h"
#include "core/math/matrix3D.h"
#include "GL/glew.h"
#include <vector>


class LightResource
{
public:
	static const unsigned int MAX_POINT_LIGHTS = 2;
	static const unsigned int MAX_SPOT_LIGHTS = 2;

	LightResource();
	~LightResource();

	vector3D getPos();
	void setPos(vector3D);

	vector3D getColor();
	void setColor(vector3D);

	float getIntensity();
	void setIntensity(float);

	//PointLight* m_pLights;
	//SpotLight* m_sLights;

	matrix3D m_matSpecularIntensityLocation;
	matrix3D m_matSpecularPowerLocation;
	int m_numPointLightsLocation;
	int m_numSpotLightsLocation;

	std::vector<DirectionalLight> m_dLights;
	std::vector<SpotLight> m_sLights;
	std::vector<PointLight> m_pLights;

	float specIntensity = 0.0f;
	float specPower = 0.0f;

private:

	vector3D color;
	vector3D pos;
	float intens;

};
