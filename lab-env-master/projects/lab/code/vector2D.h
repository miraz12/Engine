#pragma once
#include <iostream>
#include <cmath>

class vector2D
{
public:
float Varray[2];



vector2D(void)
{
	Varray[0] = 0;
	Varray[1] = 0;
}

vector2D(float x, float y)
{
	Varray[0] = x;
	Varray[1] = y;
}

~vector2D(void)
{
	
}
//�verlagrar brackets f�r att kunna skriva a[x] = 1;
float& operator[](int n)
{

	return this->Varray[n];

}

inline float x()
{
	return this->Varray[0];
}
inline float y()
{
	return this->Varray[1];
}

};

