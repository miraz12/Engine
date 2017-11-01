#include "LightNode.h"


LightNode::LightNode()
{
	color = vector3D(0.1f, 0.1f, 0.1f);
	pos = vector3D(1.0, 1.0, 1.0);
	intens = 1.0f;


	m_dLights = std::vector<DirectionalLight>();

}


LightNode::~LightNode()
{
}

vector3D LightNode::getPos()
{
	return pos;
}

void LightNode::setPos(vector3D p)
{
	pos = p;
}

vector3D LightNode::getColor()
{
	return color;
}

void LightNode::setColor(vector3D c)
{
	color = c;
}

float LightNode::getIntensity()
{
	return intens;
}

void LightNode::setIntensity(float i)
{
	intens = i;
}

