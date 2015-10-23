#pragma once

#include "nsfw.h"
#include <glm\glm.hpp>

#include "GameObject.h"
#include "Camera.h"

class ForwardPass :public nsfw::RenderPass
{
public:
	void post();

	void prep();


	void draw(const Camera&c, const GameObject&go);
	

};