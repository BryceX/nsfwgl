#pragma once
#include "nsfw.h"
#include <glm\glm.hpp>
#include "glfw3.h"


#define 	GLFW_PRESS   1
struct Camera
{
	glm::mat4 transform;
	vec3 Pos;
	vec3 Target;
	vec3 Up;
	float mnear, mfar, maspect, mfov;
	Camera() :mnear(1), mfar(50), maspect(nsfw::Window::instance().getWidth() / (float)nsfw::Window::instance().getHeight()), mfov(90) {}
	void update() {}
	void lookAt(glm::vec3 pos, glm::vec3 target, glm::vec3 up)
	{
		Pos = pos;
		Target = target;
		Up = up;
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

	void MovePan(  unsigned int up,unsigned int down,unsigned int left,unsigned int right)
	{
		auto& window = nsfw::Window::instance();
		
		if (window.getKey(window.window, up))
		{
			transform[3].y += 1* window.deltaTime;
		}
		if (window.getKey(window.window, down))
		{
			transform[3].y -= 1* window.deltaTime;
		}
		if (window.getKey(window.window, left))
		{
			transform[3].x -= 1* window.deltaTime;
		}
		if (window.getKey(window.window, right))
		{
			transform[3].x += 1* window.deltaTime;
		}
	}
};