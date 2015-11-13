#pragma once
#include "../nsfwgl/nsfw.h"
#include "Light.h"
#include "Camera.h"

class LPassD : public nsfw::RenderPass
{
public:
	void prep() 
	{
		glUseProgram(*shader);
		glClear(GL_COLOR_BUFFER_BIT);
		glBindBuffer(GL_FRAMEBUFFER,*shader);
		glViewport(0,0,1280,720);
		glEnable(GL_BLEND);
		TODO_D("glUseProgram, glClear, glBindFrameBuffer, glViewPort, glEnable etc..."); 
	}
	void post() 
	{
		glUseProgram(0);
		glDisable(GL_BLEND);
		glBindBuffer(GL_FRAMEBUFFER, 0);
		TODO_D("Unset any gl settings"); 
	}


	void draw(const Camera &c, const LightD &l)
	{
		setUniform("Projection", nsfw::UNIFORM::TYPE::MAT4, glm::value_ptr(c.getProjection()));
		setUniform("View",       nsfw::UNIFORM::TYPE::MAT4, glm::value_ptr(c.getView()));

		setUniform("LightDirection", nsfw::UNIFORM::TYPE::FLO3, glm::value_ptr(l.direction));
		setUniform("LightColor",     nsfw::UNIFORM::TYPE::FLO3, glm::value_ptr(l.color));

		setUniform("TexelScalar",    nsfw::UNIFORM::MAT4, glm::value_ptr(nsfw::Window::instance().getTexelAdjustmentMatrix()));

		unsigned quadVAOHandle  = nsfw::Assets::instance().get<nsfw::ASSET::VAO>("Quad");
		unsigned quadNumtris    = nsfw::Assets::instance().get<nsfw::ASSET::SIZE>("Quad");

		glBindVertexArray(quadVAOHandle);
		glDrawElements(GL_TRIANGLES, quadNumtris, GL_UNSIGNED_INT, 0);
		//TODO_D("GL BindVAO/DrawElements with quad size and vao");
	}
};