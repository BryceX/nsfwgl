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

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glClearColor(0, 0, 0, 0);
		glClear(GL_DEPTH_BUFFER_BIT);
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

	void draw(const GameObject &go, const Light &dl)
	{
		glm::mat4 lightProjection = glm::ortho<float>(-10, 10,-10, 10, -10, 10);
		glm::mat4 lightView = glm::lookAt(dl.direction,glm::vec3(0), glm::vec3(0, 1, 0));
		glm::mat4 lightMatrix = lightProjection* lightView;

		setUniform("LightMatrix", nsfw::UNIFORM::MAT4, glm::value_ptr(lightMatrix));

		glBindVertexArray(*go.mesh);
		glDrawElements(GL_TRIANGLES, *go.tris, GL_UNSIGNED_INT, 0);
	}


};