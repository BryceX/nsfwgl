#pragma once
#include "gl_core_4_4.h"
#include "nsfw.h"
#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include "GameObject.h"
#include "Camera.h"

class PostProcPass : public nsfw::RenderPass
{
public:

	nsfw::Asset<nsfw::ASSET::TEXTURE> color;

	void prep()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, *fbo);
		glUseProgram(*shader);
		glClearColor(0.25f, 0.25f, 0.25f, 1);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void post()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glUseProgram(0);
	}

	void draw()
	{
		// send in the image of the screen
		setUniform("Diffuse", nsfw::UNIFORM::TEX2, color, 0);

		auto &assets = nsfw::Assets::instance();

		// draw the full screen quad
		glBindVertexArray(assets.get<nsfw::ASSET::VAO>("Quad"));
		glDrawElements(GL_TRIANGLES, assets.get<nsfw::ASSET::SIZE>("Quad"), GL_UNSIGNED_INT, 0);
	}
};