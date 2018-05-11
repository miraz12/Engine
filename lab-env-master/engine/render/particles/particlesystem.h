#pragma once
#include "core/math/vector3D.h"
#include "render/resources/shaderobject.h"
#include <memory>
#include "render/resources/textureresource.h"

// Maximum number of particles
#define MAX_PARTICLES  800

// Life span of a particle (in seconds)
#define LIFE_SPAN       3.f

// A new particle is born every [BIRTH_INTERVAL] second
#define BIRTH_INTERVAL (LIFE_SPAN/(float)MAX_PARTICLES)

// Minimum delta-time for particle phisics (s)
#define MIN_DELTA_T     (BIRTH_INTERVAL * 0.5f)

// Base initial velocity (m/s)
#define VELOCITY        8.f


class Particlesystem
{
public:
	Particlesystem();
	~Particlesystem();

	void Render(matrix4D view, matrix4D projection);
	void Update(float t, float dt, vector3D campos);


	void set_position(const vector3D& position)
	{
		this->position = position;
	}
	void set_direction(const vector3D& direction)
	{
		this->direction = direction;
	}
	void set_spread(float spread)
	{
		this->spread = spread;
	}
	void set_colors(const vector4D& colors)
	{
		this->colors = colors;
	}

private:

	struct Particle
	{
		vector3D pos, vel, rot;
		unsigned char r, g, b, a; // Color
		float size, angle, weight;
		float life; // Remaining life of the particle. if <0 : dead and unused.
		float cameradistance; // *Squared* distance to the camera. if dead : -1.0f

		bool operator<(const Particle& that) const {
			// Sort in reverse order : far particles drawn first.
			return this->cameradistance > that.cameradistance;
		}
	};

	Particle ParticlesContainer[MAX_PARTICLES];
	int LastUsedParticle = 0;
	int ParticlesCount = 0;

	//Emitter settings
	float min_age;
	vector3D position;
	vector3D direction;
	float spread;
	vector4D colors;
	//--------

	GLuint billboard_vertex_buffer;
	GLuint particles_position_buffer;
	GLuint particles_color_buffer;

	std::shared_ptr<ShaderObject> shaderRs;
	std::shared_ptr<TextureResource> textureRs;

	GLfloat* g_particule_position_size_data;
	GLubyte* g_particule_color_data;

	int FindUnusedParticle();
	void SortParticles();

	void InitParticle(Particle *p, float dt);
	void UpdateParticle(Particle *p, double t, float dt, vector3D campos);

};
