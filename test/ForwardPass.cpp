#include <gl_core_4_4.h>
#include "ForwardPass.h"

void ForwardPass::post()
{
	glUseProgram(0);
}
void ForwardPass::prep() 
{
	glClearColor(0.25f, 0.25f, 0.25f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(*shader);
}


void ForwardPass::draw(const Camera&c, const GameObject&go)
{
	//camerastuff
	setUniform("Projection", nsfw::UNIFORM::MAT4, glm::value_ptr(c.transform));
	setUniform("View", nsfw::UNIFORM::MAT4, glm::value_ptr(c.getView()));

	//gameobjectstuff
	setUniform("Model", nsfw::UNIFORM::MAT4, glm::value_ptr(go.transform));
	setUniform("Diffuse", nsfw::UNIFORM::TEX2, &(go.diffuse));
}