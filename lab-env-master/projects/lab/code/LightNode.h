#pragma once
#include "vector3D.h"

class LightNode
{
public:
	LightNode();
	~LightNode();

	vector3D getPos();
	void setPos(vector3D);

	vector3D getColor();
	void setColor(vector3D);

	float getIntensity();
	void setIntensity(float);

private:


	vector3D color;
	vector3D pos;
	float intens;



};

