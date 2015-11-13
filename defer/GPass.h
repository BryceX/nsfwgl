#pragma once
#include "../nsfwgl/nsfw.h"

#include "Camera.h"
#include "Geometry.h"

class GPass : public nsfw::RenderPass
{
public:	
	void prep();
	void post();
	void draw(const Camera &c, const Geometry &g);
};