#pragma once
#include "vector3D.h"
#include <sstream>
#include <vector>
#include <string>
#include <noise/noise.h>


class TerrainGenerator
{
public:
	/// constructor
	TerrainGenerator();
	/// destructor
	~TerrainGenerator();

	bool LoadHeightMap(const char* filename);
	bool GenerateHeigthMap(int widht, int height, float freq, int oct, float lacu, float persi, int seed, int noiseType);
	void Generate(noise::module::Module* m);
	
	int width, height;
	unsigned char* imageData;
	vector3D GetNeighbourVertex(vector3D pos, int xOffset, int yOffset);
	char* mapFilename;
	noise::module::Module* noiseModule;



private:
	



};
