#include "ParticleEmitter.h"
#include "glm/ext.hpp"
#include <random>

void ParticleEmitter::Init(int particleAmount) 
{
	blocks = std::vector<Particle>();
	blocks.reserve(particleAmount);

	mesh = "Cube";
	tris = "Cube";
	diffuse = "White";

	startPosition = vec3(0, 0, 0);
	speed = 1;
	lifeSpan = 10;
	emissionRate = 2;
	emissionTime = 0;
}
void ParticleEmitter::CreateParticle() 
{
	Particle temp = Particle();

	temp.position = startPosition;
	temp.velocity = glm::normalize(glm::vec3(std::rand() % 20 - 10, std::rand() % 20 - 10, std::rand() % 20 - 10)) * speed;
	temp.lifetime = lifeSpan;

	blocks.push_back(temp);
}

void ParticleEmitter::KillParticle(unsigned in_particle)
{

	blocks.erase(blocks.begin() + in_particle);
}

void ParticleEmitter::Update(float in_deltaTime) 
{
	for (int i = 0; i < blocks.size(); i++)
	{
		blocks[i].lifetime -= in_deltaTime;

		blocks[i].position += blocks[i].velocity * in_deltaTime;
	}

	for (int i = 0; i < blocks.size(); i++)
	{
		if (blocks[i].lifetime < 0) 
		{
			KillParticle(i);
			i--;
		}
	}

	emissionTime -= in_deltaTime;

	if (emissionTime < 0) {
		CreateParticle();
		emissionTime = emissionRate;
	}
}

glm::mat4 ParticleEmitter::GetParticleMatrix(unsigned in_particle) const
{
	return glm::translate(blocks[in_particle].position);
}

unsigned ParticleEmitter::GetArraySize() const 
{
	return blocks.size();
}