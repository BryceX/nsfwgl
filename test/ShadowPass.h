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
		glBindFramebuffer(GL_FRAMEBUFFER, *fbo);
		glViewport(0, 0, 1024, 1024);
		glUseProgram(*shader);
		glClear(GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glClearColor(0.25f, 0.25f, 0.25f, 1);
		
	
	}

	void post()
	{
		auto &window = nsfw::Window::instance();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, window.getWidth(),window.getHeight());
		glUseProgram(0);

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
	}

	void draw(const GameObject &go, Light &dl)
	{
		
			setUniform("LightMatrix", nsfw::UNIFORM::MAT4, glm::value_ptr(dl.getProjection()*dl.getView()));
			setUniform("Model", nsfw::UNIFORM::MAT4, glm::value_ptr(go.transform));
			glBindVertexArray(*go.mesh);
			glDrawElements(GL_TRIANGLES, *go.tris, GL_UNSIGNED_INT, 0);
		
	}


};