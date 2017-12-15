#pragma once
#include "vector3D.h"
#include <sstream>
#include <vector>
#include <string>

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
	bool GenerateHeigthMap(int widht, int height, float freq, int oct, float lacu, float persi, int seed);
	

	int width, height;
	unsigned char* imageData;
	std::vector<MapData> mapData;
	vector3D GetNeighbourVertex(vector3D pos, int xOffset, int yOffset);
	char* mapFilename;


private:
	
	double Clamp(const double& x, const double& min, const double& max);



};
