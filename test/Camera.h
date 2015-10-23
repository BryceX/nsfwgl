#pragma once
#include "nsfw.h"
#include <glm\glm.hpp>

struct Camera
{
	glm::mat4 transform;
	float mnear, mfar, maspect, mfov;
	Camera() :mnear(0.1), mfar(100), maspect(nsfw::Window::instance().getWidth() / (float)nsfw::Window::instance().getHeight()), mfov(90) {}
	void update() {}
	void lookAt(glm::vec3 pos, glm::vec3 target, glm::vec3 up) 
	{
		transform = glm::inverse(glm::lookAt(pos, target, up));
	}

	glm::mat4 getView() const
	{
		return glm::inverse(transform);
	}
	glm::mat4 getProjection() const 
	{ 
		return glm::perspective(mfov, maspect, mnear, mfar); 
	}


};