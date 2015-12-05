#pragma once
#include "nsfw.h"





class GPUParticleEmitter {
public:
	GPUParticleEmitter::GPUParticleEmitter() :
		particles(nullptr),
		m_maxParticles(0),
		startPosition(0, 0, 0),
		m_drawShader(0),
		m_updateShader(0),
		m_lastDrawTime(0)
	{
		m_vao[0] = 0;
		m_vao[1] = 0;
		m_vbo[0] = 0;
		m_vbo[1] = 0;
	}
	GPUParticleEmitter::~GPUParticleEmitter()
	{

		//delete[] m_particles;
		//glDeleteVertexArrays(2, m_vao);
		//glDeleteBuffers(2, m_vbo);
		//// delete the shaders
		//glDeleteProgram(m_drawShader);
		//glDeleteProgram(m_updateShader);
	}
	/*GPUParticleEmitter();
	virtual ~GPUParticleEmitter();*/

	void Init(unsigned int a_maxParticles,
		float a_lifespanMin, float a_lifespanMax,
		float a_velocityMin, float a_velocityMax,
		float a_startSize, float a_endSize,
		const glm::vec4& a_startColour,
		const glm::vec4& a_endColour);

	void Draw(float time, const glm::mat4& a_cameraTransform,
		const glm::mat4& a_projectionView);

	nsfw::Asset<nsfw::ASSET::VAO> mesh;
	nsfw::Asset<nsfw::ASSET::SIZE> tris;
	nsfw::Asset<nsfw::ASSET::TEXTURE> diffuse;
	
protected:

	
	void createBuffers();
	void createUpdateShader();
	void createDrawShader();

	nsfw::ParticleVertex* particles;

	glm::vec3 startPosition;
	glm::vec4 m_startColour;
	glm::vec4 m_endColour;

	unsigned int m_maxParticles;

	float m_lifespanMin;
	float m_lifespanMax;

	float m_velocityMin;
	float m_velocityMax;

	float m_startSize;
	float m_endSize;
	
	unsigned int m_activeBuffer;
	unsigned int m_vao[2];
	unsigned int m_vbo[2];
	unsigned int m_drawShader;
	unsigned int m_updateShader;
	float m_lastDrawTime;

	

};