#pragma once
#include "nsfw.h" 
//#include "Camera.h"
#include <vector>


class ParticleEmitter
{	
private:
	struct Particle
	{
		vec3 position;
		vec3 velocity;
		float lifetime;
	};

	std::vector<Particle> blocks;
	float emissionTime;

public:
	nsfw::Asset<nsfw::ASSET::VAO> mesh;
	nsfw::Asset<nsfw::ASSET::SIZE> tris;
	nsfw::Asset<nsfw::ASSET::TEXTURE> diffuse;
	
	vec3 startPosition;
	float speed;
	float lifeSpan;
	float emissionRate;

	void Init(int particleAmount);
	void CreateParticle();
	void KillParticle(unsigned in_particle);
	void Update(float in_deltaTime);
	
	unsigned GetArraySize() const;
	glm::mat4 GetParticleMatrix(unsigned in_particle) const;
};