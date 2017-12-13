#pragma once
#include "vector3D.h"
#include <sstream>
#include <vector>

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
	bool GenerateHeigthMap();

	int width, height;
	MapData* hData;
	unsigned char* imageData;
	vector3D GetNeighbourVertex(vector3D pos, int xOffset, int yOffset);


private:
	
	double Clamp(const double& x, const double& min, const double& max);
	std::stringstream mapPath;
	vector3D* nmap;
	const char* filename;



};
