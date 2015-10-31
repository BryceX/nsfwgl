#pragma once
#include "gl_core_4_4.h"
#include "../nsfwgl/nsfw.h"

#include "Camera.h"
#include "Geometry.h"

class GPass : public nsfw::RenderPass
{


public:	
	void prep() 
	{ 
		glUseProgram(*shader);
		glClearColor(0.25f, 0.25f, 0.25f, 1);
		glBindBuffer(GL_ARRAY_BUFFER, *shader);
		glViewport(0, 0, 1280, 720);
		glEnable(GL_DEPTH_TEST);
		//TODO_D("glUseProgram, glClear, glBindFrameBuffer, glViewPort, glEnable etc...");
	}
	void post() 
	{


		TODO_D("Unset any gl settings"); 
	}

	void draw(const Camera &c, const Geometry &g)	
	{
		setUniform("Projection",	nsfw::UNIFORM::TYPE::MAT4, glm::value_ptr(c.getProjection()));
		setUniform("View",			nsfw::UNIFORM::TYPE::MAT4, glm::value_ptr(c.getView()));
		setUniform("Model",			nsfw::UNIFORM::TYPE::MAT4, glm::value_ptr(g.transform));

		setUniform("Diffuse",		nsfw::UNIFORM::TEX2,  g.diffuse,  0);
		setUniform("Normal",		nsfw::UNIFORM::TEX2,  g.normal,   1);
		setUniform("Specular",		nsfw::UNIFORM::TEX2,  g.specular, 2);

		setUniform("SpecularPower", nsfw::UNIFORM::FLO1, (void*)&g.specPower);

		
        nsfw::Assets::instance().get(g.mesh);
        nsfw::Assets::instance().get(g.tris);

		glBindVertexArray(*g.mesh);
		glDrawElements(GL_TRIANGLES,*g.tris, GL_UNSIGNED_INT,0);
        

		//TODO_D("bindVAO and Draw Elements!");
	}
};