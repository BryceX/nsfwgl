#include <gl_core_4_4.h>
#include "glm/ext.hpp"
#include "GPUParticleEmitter.h"

void GPUParticleEmitter::Init
	(unsigned int a_maxParticles,
	 float a_lifespanMin, float a_lifespanMax,
	 float a_velocityMin, float a_velocityMax,
	 float a_startSize, float a_endSize,
	 const glm::vec4& a_startColour,
	 const glm::vec4& a_endColour) 
{
	// store all variables passed in
	m_startColour = a_startColour;
	m_endColour = a_endColour;

	m_startSize = a_startSize;
	m_endSize = a_endSize;

	m_velocityMin = a_velocityMin;
	m_velocityMax = a_velocityMax;

	m_lifespanMin = a_lifespanMin;
	m_lifespanMax = a_lifespanMax;

	m_maxParticles = a_maxParticles;
	mesh = "Cube";
	tris = "Cube";
	diffuse = "White";
	// create particle array
	particles = new nsfw::ParticleVertex[a_maxParticles];
	// set our starting ping-pong buffer
	m_activeBuffer = 0;
	createBuffers();
	createUpdateShader();
	createDrawShader();

}

void GPUParticleEmitter::Draw
	(float time,
	 const glm::mat4& a_cameraTransform,
	 const glm::mat4& a_projectionView) 
{
	// update the particles using transform feedback
	glUseProgram(m_updateShader);


	// bind time information
	int location = glGetUniformLocation(m_updateShader, "time");
	glUniform1f(location, time);
	float deltaTime = time - m_lastDrawTime; m_lastDrawTime = time;
	location = glGetUniformLocation(m_updateShader, "deltaTime");
	glUniform1f(location, deltaTime);
	location = glGetUniformLocation(m_updateShader,
		"emitterPosition");
	glUniform3fv(location, 1, &startPosition[0]);	glEnable(GL_RASTERIZER_DISCARD);

	glBindVertexArray(m_vao[m_activeBuffer]);


	// work out the "other" buffer
	unsigned int otherBuffer = (m_activeBuffer + 1) % 2;



	// bind the buffer we will update into as points
	// and begin transform feedback
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0,
		m_vbo[otherBuffer]);
	glBeginTransformFeedback(GL_POINTS);
	glDrawArrays(GL_POINTS, 0, m_maxParticles);

	glEndTransformFeedback();
	glDisable(GL_RASTERIZER_DISCARD);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, 0);

	// draw the particles using the Geometry SHader to billboard them
	glUseProgram(m_drawShader);
	unsigned location = glGetUniformLocation(m_drawShader, "projectionView");
	glUniformMatrix4fv(location, 1, false, &a_projectionView[0][0]);
	location = glGetUniformLocation(m_drawShader, "cameraTransform");
	glUniformMatrix4fv(location, 1, false, &a_cameraTransform[0][0]);


	// draw particles in the "other" buffer
	glBindVertexArray(m_vao[otherBuffer]);
	glDrawArrays(GL_POINTS, 0, m_maxParticles);


	// swap for next frame
	m_activeBuffer = otherBuffer;
}
void GPUParticleEmitter::createBuffers() 
{
	nsfw::Assets::instance().makeVAO("vao0", particles,m_maxParticles);
	nsfw::Assets::instance().makeVAO("vao1", particles,m_maxParticles);
	

	/*
	glGenVertexArrays(2, m_vao);
	glGenBuffers(2, m_vbo);
	// setup the first buffer
	glBindVertexArray(m_vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, m_maxParticles *
		sizeof(GPUParticle), m_particles, GL_STREAM_DRAW);
	glEnableVertexAttribArray(0); // position
	glEnableVertexAttribArray(1); // velocity
	glEnableVertexAttribArray(2); // lifetime
	glEnableVertexAttribArray(3); // lifespan
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
		sizeof(GPUParticle), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
		sizeof(GPUParticle), ((char*)0) + 12);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE,
		sizeof(GPUParticle), ((char*)0) + 24);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE,
		sizeof(GPUParticle), ((char*)0) + 28);
	// setup the second buffer
	glBindVertexArray(m_vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, m_maxParticles *
		sizeof(GPUParticle), 0, GL_STREAM_DRAW);
	glEnableVertexAttribArray(0); // position
	glEnableVertexAttribArray(1); // velocity
	glEnableVertexAttribArray(2); // lifetime
	glEnableVertexAttribArray(3); // lifespan
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
		sizeof(GPUParticle), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
		sizeof(GPUParticle), ((char*)0) + 12);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE,
		sizeof(GPUParticle), ((char*)0) + 24);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE,
		sizeof(GPUParticle), ((char*)0) + 28);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	*/
}
void GPUParticleEmitter::createUpdateShader()
{
	const char *vars[] = { "position","velocity","lifetime","lifespan" };
	nsfw::Assets::instance().loadUpdateShader("gpuParticleUpdateShader", "Shaders/gpuUpdateShader.txt", vars, 4);
}
void GPUParticleEmitter::createDrawShader() 
{
	nsfw::Assets::instance().loadShader("particleShader", "Shaders/gpuParticleVert.txt", "Shaders/gpuParticleGeom.txt", "Shaders/gpuParticleFrag.txt");
}