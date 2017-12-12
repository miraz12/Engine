#pragma once
#include "GL/glew.h"
#include <string>
#include "vector3D.h"
#include "vector4D.h"

class TerrainGenerator
{
public:
	struct MapData
	{
		vector3D xyz;
		//vector4D color; // Color
		int width = 0;
		int height = 0;
	};

	/// constructor
	TerrainGenerator();
	/// destructor
	~TerrainGenerator();

	bool Initialize(const char* filename);

	int width, height;
	MapData* hData;

private:
	



};
