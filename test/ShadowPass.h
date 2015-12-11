#pragma once
#include "gl_core_4_4.h"
#include "nsfw.h"
#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include "GameObject.h"
#include "Camera.h"
#include "Light.h"

class ShadowPass : public nsfw::RenderPass
{
public:
	void prep()
	{
		// RENDERING WINDOW
		glBindFramebuffer(GL_FRAMEBUFFER, *fbo);
		glViewport(0, 0, 1024, 1024);

		// GL FLAGS
		glEnable(GL_DEPTH_TEST);
		//glEnable(GL_CULL_FACE);

		// glClear
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_DEPTH_BUFFER_BIT);
		
		// pipeline
		glUseProgram(*shader);
	}

	void post()
	{
		auto &window = nsfw::Window::instance();

		// RENDERING WINDOW
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, window.getWidth(),window.getHeight());
		
		// GL FLAGS
		glDisable(GL_DEPTH_TEST);
		//glDisable(GL_CULL_FACE);

		// pipeline
		glUseProgram(0);
	}

	void draw(const GameObject &go, Light &dl)
	{
		setUniform("LightMatrix", nsfw::UNIFORM::MAT4, glm::value_ptr(dl.getProjection()*dl.getView()));
		setUniform("Model", nsfw::UNIFORM::MAT4, glm::value_ptr(go.transform));

		glBindVertexArray(*go.mesh);
		glDrawElements(GL_TRIANGLES, *go.tris, GL_UNSIGNED_INT, 0);
	}
};