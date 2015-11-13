#pragma once
#include "nsfw.h"
#include <glm\ext.hpp>

struct Light
{
	glm::vec3 direction;
	glm::vec4 color;

	glm::mat4 getView() const
	{
		
	}
	glm::mat4 getProjection() const
	{
		
	}
};