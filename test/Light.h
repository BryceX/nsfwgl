#pragma once
#include "nsfw.h"
#include <glm\ext.hpp>

struct Light
{
	glm::vec3 direction;
	glm::vec4 color;

	glm::mat4 getView() const //formerly known as lightview
	{
		return glm::lookAt(glm::vec3(direction), glm::vec3(0), glm::vec3(0, 1, 0));
	}
	glm::mat4 getProjection() const // formerly known as lightprojection
	{
		return glm::ortho<float>(-20, 20, -20, 20, -20, 20);
	}
};