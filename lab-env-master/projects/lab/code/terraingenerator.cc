#pragma once
#include "terrainGenerator.h"
#include "stb_image.h"
#include "noiseutils.h"

TerrainGenerator::TerrainGenerator()
{
	mapFilename = nullptr;
	imageData = nullptr;
	noiseModule = nullptr;
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

bool TerrainGenerator::LoadHeightMap(const char* filename)
{
	int error;
	long int index;
	imageData = stbi_load(filename, &this->width, &this->height, &error, 1);	//Load file	
	if (imageData == NULL) 
	{
		std::cerr << "Unable to load heightmap: " << filename << std::endl;
		return false;
	}
	if (mapFilename != nullptr)
	{
		delete mapFilename;
	}

	mapFilename = new char;	//Save file name for later use
	mapFilename = strdup(filename);

	return true;
}

bool TerrainGenerator::GenerateHeigthMap(int widht, int height, float freq, int oct, float lacu, float persi, int seed, int noiseType)
{
	if (noiseModule != nullptr)
	{
		delete noiseModule;
	}

	if (noiseType == 0)
	{
		noise::module::Perlin* newMod = new noise::module::Perlin();

		/*Noise seed*/
		newMod->SetSeed(seed);

		/*The number of cycles per unit length that a specific coherent-noise function outputs.*/
		newMod->SetFrequency(freq);

		/*One of the coherent-noise functions in a series of coherent-noise functions that are added together to form Perlin noise.*/
		newMod->SetOctaveCount(oct);

		/*A multiplier that determines how quickly the frequency increases for each successive octave in a Perlin-noise function.*/
		newMod->SetLacunarity(lacu);

		/*A multiplier that determines how quickly the amplitudes diminish for each successive octave in a Perlin-noise function.*/
		newMod->SetPersistence(persi);

		noiseModule = newMod;
	}
	else if (noiseType == 1)
	{
		noise::module::Voronoi* newMod = new noise::module::Voronoi();

		/*Noise seed*/
		newMod->SetSeed(seed);

		/*The number of cycles per unit length that a specific coherent-noise function outputs.*/
		newMod->SetFrequency(freq);

		/*Use oct to not have an even more insane amount of arguments*/
		newMod->SetDisplacement(oct);

		noiseModule = newMod;

	}
	else if (noiseType == 2)
	{
		noise::module::RidgedMulti* newMod = new noise::module::RidgedMulti();

		/*Noise seed*/
		newMod->SetSeed(seed);

		/*The number of cycles per unit length that a specific coherent-noise function outputs.*/
		newMod->SetFrequency(freq);

		/*One of the coherent-noise functions in a series of coherent-noise functions that are added together to form Perlin noise.*/
		newMod->SetOctaveCount(oct);

		/*A multiplier that determines how quickly the frequency increases for each successive octave in a Perlin-noise function.*/
		newMod->SetLacunarity(lacu);

		noiseModule = newMod;
	}
	else if (noiseType == 3)
	{
		noise::module::Billow* newMod = new noise::module::Billow();

		/*Noise seed*/
		newMod->SetSeed(seed);

		/*The number of cycles per unit length that a specific coherent-noise function outputs.*/
		newMod->SetFrequency(freq);

		/*One of the coherent-noise functions in a series of coherent-noise functions that are added together to form Perlin noise.*/
		newMod->SetOctaveCount(oct);

		/*A multiplier that determines how quickly the frequency increases for each successive octave in a Perlin-noise function.*/
		newMod->SetLacunarity(lacu);

		/*A multiplier that determines how quickly the amplitudes diminish for each successive octave in a Perlin-noise function.*/
		newMod->SetPersistence(persi);

		noiseModule = newMod;
	}
	else if (noiseType == 4)
	{
		noise::module::Checkerboard* newMod = new noise::module::Checkerboard();

		noiseModule = newMod;
	}
	else if (noiseType == 5)
	{
		noise::module::Const* newMod = new noise::module::Const();

		/*Set the constant value*/
		newMod->SetConstValue(freq);

		noiseModule = newMod;
	}
	else if (noiseType == 6)
	{
		noise::module::Cylinders* newMod = new noise::module::Cylinders();

		newMod->SetFrequency(freq);

		noiseModule = newMod;
	}
	else if (noiseType == 7)
	{
		noise::module::Spheres* newMod = new noise::module::Spheres();

		/*The number of cycles per unit length that a specific coherent-noise function outputs.*/
		newMod->SetFrequency(freq);

		noiseModule = newMod;
	}

	utils::NoiseMap heightMap;
	utils::NoiseMapBuilderPlane heightMapBuilder;
	heightMapBuilder.SetSourceModule(*noiseModule);
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

	return LoadHeightMap(mapPath.str().c_str());
}

vector3D TerrainGenerator::GetNeighbourVertex(vector3D pos, int xOffset, int yOffset)
{
	if (pos[0] + xOffset < 0 || pos[0] + xOffset > width - 1)
		return pos;
	if (pos[2] + yOffset < 0 || pos[2] + yOffset > width - 1)
		return pos;

	int y = pos[2] + yOffset;
	int x = pos[0] + xOffset;
	return vector3D(x, imageData[y*width + x], y);
}
