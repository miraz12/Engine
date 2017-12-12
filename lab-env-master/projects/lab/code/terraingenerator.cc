#pragma once
#include "terrainGenerator.h"
#include "stb_image.h"
#include <iostream>


TerrainGenerator::TerrainGenerator()
{
}

TerrainGenerator::~TerrainGenerator()
{
}

bool TerrainGenerator::Initialize(const char* filename)
{

	int error;
	long int k = 0, index;
	unsigned char* imageData = stbi_load(filename, &this->width, &this->height, &error, 1);


	if (imageData == NULL) 
	{

		std::cerr << "Unable to load texture: " << filename << std::endl;
		return false;
	}

	hData = new MapData[width * height];

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			index = (width * i) + j;

			hData[index].xyz[0] = float(j * 4);	//Set width 
			hData[index].xyz[1] = float(imageData[k]);	//Set 
			hData[index].xyz[2] = float(i * 4.0);

			hData[index].width = width;
			hData[index].height = height;

			//TODO: Set colors
		}
	}
	return true;
}
