#pragma once
#include "gl_core_4_4.h"
#include "nsfw.h"
#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include "GameObject.h"
#include "Camera.h"
#include "Light.h"
#include "ParticleEmitter.h"
#include "GPUParticleEmitter.h"

class ForwardPass : public nsfw::RenderPass
{
public:
	void prep()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, *fbo);
		glViewport(0, 0, 1280, 720);

		glEnable(GL_DEPTH_TEST);
		//glEnable(GL_CULL_FACE);

		glClearColor(0.25f, 0.25f, 0.25f, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		
	}

	void post()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		//glDisable(GL_CULL_FACE);
		glUseProgram(0);
	}

	void draw(const Camera &c, const GameObject &go, const Light &dl)
	{
		glUseProgram(*shader);
		glm::mat4 clipToUV = glm::scale(.5f, .5f, .5f);
		//// first, scale it down by half in all components (xyz)
		clipToUV = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, -1.0f, -1.0f));
		//// then, translate it back by 1 in all components (xyz)

		//Camera
		setUniform("Projection", nsfw::UNIFORM::MAT4, glm::value_ptr(c.getProjection()));
		setUniform("View", nsfw::UNIFORM::MAT4, glm::value_ptr(c.getView()));
		////GameObject
		setUniform("Model", nsfw::UNIFORM::MAT4, glm::value_ptr(go.transform));

		setUniform("DiffuseMap",	nsfw::UNIFORM::TEX2, go.diffuse, 0);
		setUniform("NormalMap",		nsfw::UNIFORM::TEX2, go.normalMap, 1);

		// HACK: hard coding value for ShadowMap texture
		nsfw::Asset<nsfw::ASSET::TEXTURE> shadowmap = "ShadowMap";
		setUniform("ShadowMap",		nsfw::UNIFORM::TEX2, shadowmap, 2);

		//Light
		//setUniform("LightColor", nsfw::UNIFORM::FLO4, glm::value_ptr(dl.color) );
		setUniform("LightDirection",nsfw::UNIFORM::FLO4, glm::value_ptr(dl.direction));

		/*
		* DON'T TOUCH THIS
		* \/\/\/\/\/
		*/
		glm::mat4 texSpaceOffset(0.5f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.5f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.5f, 0.0f,
			0.5f, 0.5f, 0.5f, 1.0f);
		/*
		* /\/\/\/\/\/\
		* DON'T TOUCH THAT
		*/

		setUniform("LightMatrix", nsfw::UNIFORM::MAT4, glm::value_ptr(texSpaceOffset * dl.getProjection() * dl.getView()));



		glBindVertexArray(*go.mesh);
		glDrawElements(GL_TRIANGLES, *go.tris, GL_UNSIGNED_INT, 0);
	}

	void draw(const Camera &c, const ParticleEmitter &pe, const Light &dl)
	{
		glm::mat4 clipToUV = glm::scale(.5f, .5f, .5f);
		//// first, scale it down by half in all components (xyz)
		clipToUV = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, -1.0f, -1.0f));
		//// then, translate it back by 1 in all components (xyz)

		//Camera
		setUniform("Projection", nsfw::UNIFORM::MAT4, glm::value_ptr(c.getProjection()));
		setUniform("View", nsfw::UNIFORM::MAT4, glm::value_ptr(c.getView()));
		////GameObject

		setUniform("Diffuse", nsfw::UNIFORM::TEX2, pe.diffuse, 0);
		//Light
		//setUniform("LightColor", nsfw::UNIFORM::FLO4, glm::value_ptr(dl.color) );
		setUniform("LightDirection", nsfw::UNIFORM::FLO4, glm::value_ptr(dl.direction));

		/*
		* DON'T TOUCH THIS
		* \/\/\/\/\/
		*/
		glm::mat4 texSpaceOffset(0.5f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.5f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.5f, 0.0f,
			0.5f, 0.5f, 0.5f, 1.0f);
		/*
		* /\/\/\/\/\/\
		* DON'T TOUCH THAT
		*/

		setUniform("LightMatrix", nsfw::UNIFORM::MAT4, glm::value_ptr(texSpaceOffset * dl.getProjection() * dl.getView()));

		// HACK: hard coding value for ShadowMap texture
		nsfw::Asset<nsfw::ASSET::TEXTURE> shadowmap = "ShadowMap";
		setUniform("ShadowMap", nsfw::UNIFORM::TEX2, shadowmap, 1);

		for (int i = 0; i < pe.GetArraySize(); i++) {
			setUniform("Model", nsfw::UNIFORM::MAT4, glm::value_ptr(pe.GetParticleMatrix(i)));

			glBindVertexArray(*pe.mesh);
			glDrawElements(GL_TRIANGLES, *pe.tris, GL_UNSIGNED_INT, 0);

		}

		

	}
	void draw(const Camera &c, GPUParticleEmitter &pe)
	{
		float time = nsfw::Window::instance().timePassed;

		glm::mat4 projectionView = c.getProjection() * c.getView();
		pe.Draw(time, c.transform, projectionView);
	}
};