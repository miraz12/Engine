#pragma once
#include "terrainGenerator.h"
#include "stb_image.h"
#include "PerlinNoise.h"
#include "image.h"


TerrainGenerator::TerrainGenerator()
{
}

TerrainGenerator::~TerrainGenerator()
{
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

	this->filename = filename;	
	return true;
}

bool TerrainGenerator::GenerateHeigthMap()
{
	Image image(512, 512);

	double frequency = 8.0;
	frequency = Clamp(frequency, 0.1, 64.0);

	int octaves = 8;
	octaves = int(Clamp(octaves, 1, 16));

	int seed = 12345;

	const siv::PerlinNoise perlin(seed);
	const double fx = image.width() / frequency;
	const double fy = image.height() / frequency;

	for (int i = 0; i < image.height(); i++)
	{
		for (int j = 0; j < image.width(); j++)
		{
			const RGB color(perlin.octaveNoise0_1(j / fx, i / fy, octaves));

			image.set(j, i, color);
		}
	}

	mapPath << "content/" << 'f' << frequency << 'o' << octaves << '_' << seed << ".bmp";

	if (image.saveBMP(mapPath.str()))
	{
		std::cout << "...saved \"" << mapPath.str() << "\"\n";
	}
	else
	{
		std::cout << "...failed\n";
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
