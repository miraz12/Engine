#pragma once
#include "terrainGenerator.h"
#include "stb_image.h"
#include "PerlinNoise.h"
#include "image.h"
#include <cmath>;
#include "FastNoise.h"
#include "SimplexNoise.h"

TerrainGenerator::TerrainGenerator()
{
	mapFilename = nullptr;
	imageData = nullptr;
}

TerrainGenerator::~TerrainGenerator()
{
	if(mapFilename != nullptr)
	{
		if (remove(mapFilename) != 0)
			std::cerr << "Error deleting file: " << mapFilename << std::endl;
		else
			puts("Removed map file");

		delete mapFilename;
	}
	if (imageData != nullptr)
	{
		delete imageData;
	}
}

bool TerrainGenerator::Initialize(const char* filename)
{
	int error;
	long int index;
	imageData = stbi_load(filename, &this->width, &this->height, &error, 1);


	if (imageData == NULL) 
	{

		std::cerr << "Unable to load texture: " << filename << std::endl;
		return false;
	}

	mapFilename = new char;
	mapFilename = strdup(filename);


	//this->filename = filename;
	return true;
}

bool TerrainGenerator::GenerateHeigthMap(int widht, int height, float freq, int oct, int seed)
{
	Image image(widht, height);

	double frequency = freq;
	frequency = Clamp(frequency, 0.1, 64.0);

	int octaves = oct;
	octaves = int(Clamp(octaves, 1, 16));


	const siv::PerlinNoise perlin(seed);
	const double fx = image.width() / frequency;
	const double fy = image.height() / frequency;

	int index = 0;
	for (int i = 0; i < image.height(); i++)
	{
		for (int j = 0; j < image.width(); j++)
		{

			float e = 1 * perlin.octaveNoise0_1(j / fx, i / fy, octaves);
			const RGB color(e);

			image.set(j, i, color);
		}
	}

	std::stringstream mapPath;
	mapPath << "content/" << 'f' << frequency << 'o' << octaves << '_' << seed << ".bmp";

	if (image.saveBMP(mapPath.str()))
	{
		std::cout << "...saved \"" << mapPath.str() << "\"\n";
	}
	else
	{
		std::cout << "...failed\n";
		return false;
	}

	return Initialize(mapPath.str().c_str());
}

vector3D TerrainGenerator::GetNeighbourVertex(vector3D pos, int xOffset, int yOffset)
{
	if (pos[0] + xOffset < 0 || pos[0] + xOffset > width - 1)
		return pos;
	if (pos[2] + yOffset < 0 || pos[2] + yOffset > width - 1)
		return pos;

	int y = pos[2] + yOffset;
	int x = pos[0] + xOffset;
	return vector3D(x, imageData[y*width + x] * 0.25f, y);
}

double TerrainGenerator::Clamp(const double& x, const double& min, const double& max)
{
	return (x < min) ? min : ((max < x) ? max : x);
}
