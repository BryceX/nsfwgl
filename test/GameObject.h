#pragma once
#include "nsfw.h"
#include <glm\ext.hpp>


struct GameObject
{
	glm::mat4 transform;
	nsfw::Asset<nsfw::ASSET::VAO> mesh;
	nsfw::Asset<nsfw::ASSET::SIZE> tris;
	nsfw::Asset<nsfw::ASSET::TEXTURE> diffuse;
};