#pragma once
#include "render/base/framepass.h"
#include "render/resources/textureresource.h"
#include "render/camera.h"
#include <vector>

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

namespace Passes
{
	///Template for creating new passes
    class ParticlePass : public Base::FramePass
    {
    public:
		ParticlePass();
        ~ParticlePass();
        void Execute();
		void UpdateParticles();

    private:

		// Represents a single particle and its state
		struct Particle {
			vector2D Position, Velocity;
			vector4D Color;
			GLfloat Life;

			Particle() : Position(vector2D(0,0)), Velocity(vector2D(0, 0)), Color(vector4D(1, 1, 1, 1)), Life(0.0f) { }
		};


		// State
		std::vector<Particle> particles;
		GLuint amount = 10;
		// Render state
		std::shared_ptr<Resources::ShaderObject> shader;
		std::shared_ptr<Resources::TextureResource>  texture;
		GLuint VAO;
		// Initializes buffer and vertex attributes
		void init();
		// Returns the first Particle index that's currently unused e.g. Life <= 0.0f or 0 if no particle is currently inactive
		GLuint firstUnusedParticle();
		// Respawns particle
		void respawnParticle(Particle &particle, vector2D offset = vector2D(0.0f, 0.0f));

		GLuint lastUsedParticle = 0;

		Display::Camera* cam;
		Servers::RenderServer* srv;

    };

}
