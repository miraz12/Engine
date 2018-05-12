#include "config.h"
#include "lightresource.h"


LightResource::LightResource(float specularintensity, float speculatpower)
{
	color = vector3D(0.1f, 0.1f, 0.1f);
	pos = vector3D(1.0, 1.0, 1.0);
	intens = 1.0f;

    specIntensity = specularintensity;
    specPower = speculatpower;

	m_dLights = std::vector<DirectionalLight>();

}

void LightResource::AddDirectionalLight(vector3D color, float ambientintensity, float diffuseintensity,
    vector3D direction)
{
    DirectionalLight dirLight = DirectionalLight();
    dirLight.Color = color;
    dirLight.AmbientIntensity = ambientintensity;
    dirLight.DiffuseIntensity = diffuseintensity;
    dirLight.Direction = direction;
    this->m_dLights.push_back(dirLight);
}

void LightResource::AddPointLight(vector3D color, float diffuseintensity, vector3D position, float attenuationConst,
    float attenuationLin, float attenuationExp)
{
    PointLight pLight = PointLight();
    pLight.DiffuseIntensity = diffuseintensity; // 0.25f;
    pLight.Color = color;
    pLight.Position = position;
    pLight.Attenuation.Constant = attenuationConst;
    pLight.Attenuation.Linear = attenuationLin;
    pLight.Attenuation.Exp = attenuationExp;
    this->m_pLights.push_back(pLight);
}

void LightResource::AddSpotLight(vector3D color, float diffuseintensity, vector3D position, vector3D direction, float attenuationConst,
    float attenuationLin, float attenuationExp, float cutoff)
{
    SpotLight sLight = SpotLight();
    sLight.DiffuseIntensity = diffuseintensity;
    sLight.Color = color;
    sLight.Position = position;
    sLight.Direction = direction;
    sLight.Attenuation.Constant = attenuationConst;
    sLight.Attenuation.Linear = attenuationLin;
    sLight.Attenuation.Exp = attenuationExp;
    sLight.Cutoff = cutoff;
}


LightResource::~LightResource()
{
}

vector3D LightResource::getPos()
{
	return pos;
}

void LightResource::setPos(vector3D p)
{
	pos = p;
}

vector3D LightResource::getColor()
{
	return color;
}

void LightResource::setColor(vector3D c)
{
	color = c;
}

float LightResource::getIntensity()
{
	return intens;
}

void LightResource::setIntensity(float i)
{
	intens = i;
}

