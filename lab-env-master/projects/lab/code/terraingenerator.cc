#pragma once
#include "terrainGenerator.h"
#include "stb_image.h"
#include <noise/noise.h>
#include "noiseutils.h"

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

	return true;
}

bool TerrainGenerator::GenerateHeigthMap(int widht, int height, float freq, int oct, float lacu, float persi, int seed)
{
	noise::module::Perlin myModule;
	/*Noise seed*/
	myModule.SetSeed(seed);

	/*The number of cycles per unit length that a specific coherent-noise function outputs.*/
	myModule.SetFrequency(freq);
	
	/*One of the coherent-noise functions in a series of coherent-noise functions that are added together to form Perlin noise.*/
	myModule.SetOctaveCount(oct);

	/*A multiplier that determines how quickly the frequency increases for each successive octave in a Perlin-noise function.*/
	myModule.SetLacunarity(lacu);

	/*A multiplier that determines how quickly the amplitudes diminish for each successive octave in a Perlin-noise function.*/
	myModule.SetPersistence(persi);

	//myModule.SetSourceModule(0, myModule);

	utils::NoiseMap heightMap;
	utils::NoiseMapBuilderPlane heightMapBuilder;
	heightMapBuilder.SetSourceModule(myModule);
	heightMapBuilder.SetDestNoiseMap(heightMap);
	heightMapBuilder.SetDestSize(widht, height);
	heightMapBuilder.SetBounds(2.0, 6.0, 1.0, 5.0);
	heightMapBuilder.Build();

	utils::RendererImage renderer;
	utils::Image image;
	renderer.SetSourceNoiseMap(heightMap);
	renderer.SetDestImage(image);
	renderer.Render();

	utils::WriterBMP writer;
	writer.SetSourceImage(image);

	std::stringstream mapPath;
	mapPath << "content/" << 'f' << freq << 'o' << oct << '_' << seed << ".bmp";

	writer.SetDestFilename(mapPath.str());
	writer.WriteDestFile();

	std::cout << "Loaded file " << mapPath.str() << "\n";

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
