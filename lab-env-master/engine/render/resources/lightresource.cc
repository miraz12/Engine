#include "config.h"
#include "lightresource.h"


LightResource::LightResource()
{
	color = vector3D(0.1f, 0.1f, 0.1f);
	pos = vector3D(1.0, 1.0, 1.0);
	intens = 1.0f;


	m_dLights = std::vector<DirectionalLight>();

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

