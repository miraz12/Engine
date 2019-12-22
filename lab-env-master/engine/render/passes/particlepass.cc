#include "config.h"
#include "ParticlePass.h"
#include <algorithm>
#include "render/servers/renderserver.h"



namespace Passes
{

	ParticlePass::ParticlePass()
    {
		cam = Display::Camera::GetInstance();
		srv = Servers::RenderServer::GetInstance();

		shader = std::make_shared<Resources::ShaderObject>("content/Shader/particles.vs", "content/Shader/particles.fs");
		texture = std::make_shared<Resources::TextureResource>("content/bricks.jpg");
		texture->Load();

		// Set up mesh and attribute properties
		GLuint VBO;
		GLfloat particle_quad[] = {
			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,

			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 0.0f
		};
		glGenVertexArrays(1, &this->VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(this->VAO);
		// Fill mesh buffer
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
		// Set mesh attributes
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
		glBindVertexArray(0);

		// Create this->amount default particle instances
		for (GLuint i = 0; i < this->amount; ++i)
			this->particles.push_back(Particle());

	}

	ParticlePass::~ParticlePass()
    {
		//Emtpy
    }

	void ParticlePass::Execute()
	{
		UpdateParticles();
		// Use additive blending to give it a 'glow' effect
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		this->shader->bind();
		for (Particle particle : this->particles)
		{
			if (particle.Life > 0.0f)
			{
				this->shader->modVector2f("offset", particle.Position);
				this->shader->modVector4f("color", particle.Color);
				this->shader->modMatrix4fv("projection", cam->projection);
				glActiveTexture(GL_TEXTURE0);
				texture->bind();
				glBindVertexArray(this->VAO);
				glDrawArrays(GL_TRIANGLES, 0, 6);
				glBindVertexArray(0);
			}
		}
		// Don't forget to reset to default blending mode
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void ParticlePass::UpdateParticles()
	{
		uint16 newParticles = 1000; 

		// Add new particles 
		for (size_t i = 0; i < newParticles; ++i)
		{
			int unusedParticle = this->firstUnusedParticle();
			this->respawnParticle(this->particles[unusedParticle]);
		}
		// Update all particles
		for (GLuint i = 0; i < this->amount; ++i)
		{
			Particle &p = this->particles[i];
			p.Life -= (GLfloat)srv->dt; // reduce life
			if (p.Life > 0.0f)
			{	// particle is alive, thus update
				p.Position = p.Position - p.Velocity * (float)srv->dt;
				p.Color[3] -= (float)srv->dt * 2.5f;
			}
		}
	}

	GLuint ParticlePass::firstUnusedParticle()
	{
		// First search from last used particle, this will usually return almost instantly
		for (GLuint i = lastUsedParticle; i < this->amount; ++i) {
			if (this->particles[i].Life <= 0.0f) {
				lastUsedParticle = i;
				return i;
			}
		}
		// Otherwise, do a linear search
		for (GLuint i = 0; i < lastUsedParticle; ++i) {
			if (this->particles[i].Life <= 0.0f) {
				lastUsedParticle = i;
				return i;
			}
		}
		// All particles are taken, override the first one (note that if it repeatedly hits this case, more particles should be reserved)
		lastUsedParticle = 0;
		return 0;
	}

	void ParticlePass::respawnParticle(Particle& particle, vector2D offset)
	{
		float random = ((rand() % 100) - 50) / 10.0f;
		float rColor = 1;// 0.5 + ((rand() % 100) / 100.0f);
		particle.Position = vector2D(0, 0) + random + offset;
		particle.Color = vector4D(rColor, rColor, rColor, 1.0f);
		particle.Life = 1.0f;
		particle.Velocity = vector2D(0, 0) * 0.1f;
	}
}
