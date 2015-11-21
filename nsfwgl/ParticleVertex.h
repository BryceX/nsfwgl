#pragma once
#include "gl_core_4_4.h"
#include "nsfw.h"

struct GPUParticle 
{
	GPUParticle() :lifetime(1), lifespan(0) {} // <-need to ask about what this is doing
	
	glm::vec3 position;
	glm::vec3 velocity;

	float lifetime;
	float lifespan;
};