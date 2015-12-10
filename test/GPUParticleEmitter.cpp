#include <gl_core_4_4.h>
#include "glm/ext.hpp"
#include "GPUParticleEmitter.h"
#include <fstream>

//unsigned int loadShader(unsigned int type, const char* path)
//{
//	// loading in the shader source
//	FILE* file = fopen(path, "rb");
//	
//	if (file == nullptr)	// did it work?
//		return 0; // nope, rage quit
//
//	// read through the entire file
//	fseek(file, 0, SEEK_END);
//	unsigned int length = ftell(file);
//	fseek(file, 0, SEEK_SET);
//
//	// copy the string into the char array
//	char* source = new char[length + 1];
//	memset(source, 0, length + 1);
//	fread(source, sizeof(char), length, file);
//	fclose(file);
//
//	// create a shader
//	unsigned int shader = glCreateShader(type);
//	glShaderSource(shader, 1, &source, 0);
//	glCompileShader(shader);
//	delete[] source;
//
//	// return the shader
//	return shader;
//}
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
	/*mesh = "Cube";
	tris = "Cube";
	diffuse = "White";*/
	// create particle array
	particles = new nsfw::ParticleVertex[a_maxParticles];
	// set our starting ping-pong buffer
	m_activeBuffer = 0;
	createBuffers();
	createUpdateShader("Shaders/gpuUpdateShader.txt");
	createDrawShader();

}

void GPUParticleEmitter::Draw
	(float time,
	 const glm::mat4& a_cameraTransform,
	 const glm::mat4& a_projectionView) 
{
	// update the partupdateicles using transform feedback
	glUseProgram(m_updateShader);


	// bind time information
	int location = glGetUniformLocation(m_updateShader, "time");
	glUniform1f(location, time);
	float deltaTime = time - m_lastDrawTime;
	m_lastDrawTime = time;
	location = glGetUniformLocation(m_updateShader, "deltaTime");
	glUniform1f(location, deltaTime);
	location = glGetUniformLocation(m_updateShader,	"emitterPosition");
	glUniform3fv(location, 1, &startPosition[0]);
	glEnable(GL_RASTERIZER_DISCARD);

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
	location = glGetUniformLocation(m_drawShader, "projectionView");
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

	glGenVertexArrays(2, m_vao);
	glGenBuffers(2, m_vbo);
	// setup the first buffer
	glBindVertexArray(m_vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, m_maxParticles * sizeof(nsfw::ParticleVertex), particles, GL_STREAM_DRAW);
	glEnableVertexAttribArray(0); // position
	glEnableVertexAttribArray(1); // velocity
	glEnableVertexAttribArray(2); // lifetime
	glEnableVertexAttribArray(3); // lifespan
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
		sizeof(nsfw::ParticleVertex), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
		sizeof(nsfw::ParticleVertex), ((char*)0) + 12);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE,
		sizeof(nsfw::ParticleVertex), ((char*)0) + 24);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE,
		sizeof(nsfw::ParticleVertex), ((char*)0) + 28);
	// setup the second buffer
	glBindVertexArray(m_vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, m_maxParticles *
		sizeof(nsfw::ParticleVertex), 0, GL_STREAM_DRAW);
	glEnableVertexAttribArray(0); // position
	glEnableVertexAttribArray(1); // velocity
	glEnableVertexAttribArray(2); // lifetime
	glEnableVertexAttribArray(3); // lifespan
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
		sizeof(nsfw::ParticleVertex), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
		sizeof(nsfw::ParticleVertex), ((char*)0) + 12);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE,
		sizeof(nsfw::ParticleVertex), ((char*)0) + 24);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE,
		sizeof(nsfw::ParticleVertex), ((char*)0) + 28);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
//void GPUParticleEmitter::createUpdateShader()
//{
//	const char *vars[] = { "position","velocity","lifetime","lifespan" };
//	nsfw::Assets::instance().loadUpdateShader("gpuParticleUpdateShader", "Shaders/gpuUpdateShader.txt", vars, 4);
//}
//void GPUParticleEmitter::createDrawShader() 
//{
//	nsfw::Assets::instance().loadShader("particleShader", "Shaders/gpuParticleVert.txt", "Shaders/gpuParticleGeom.txt", "Shaders/gpuParticleFrag.txt");
//}

void GPUParticleEmitter::createDrawShader()
{
	unsigned int vs = nsfw::Assets::loadSubShader(GL_VERTEX_SHADER,"Shaders/gpuParticleVert.txt");
	unsigned int gs = nsfw::Assets::loadSubShader(GL_GEOMETRY_SHADER,"Shaders/gpuParticleGeom.txt");
	unsigned int fs = nsfw::Assets::loadSubShader(GL_FRAGMENT_SHADER,"Shaders/gpuParticleFrag.txt");
	m_drawShader = glCreateProgram();
	glAttachShader(m_drawShader, vs);
	glAttachShader(m_drawShader, fs);
	glAttachShader(m_drawShader, gs);
	glLinkProgram(m_drawShader);
	// remove unneeded handles
	glDeleteShader(vs);
	glDeleteShader(gs);
	glDeleteShader(fs);
	// bind the shader so that we can set
	// some uniforms that don't change per-frame
	glUseProgram(m_drawShader);
	// bind size information for interpolation that won’t change
	int location = glGetUniformLocation(m_drawShader, "sizeStart");
	glUniform1f(location, m_startSize);
	location = glGetUniformLocation(m_drawShader, "sizeEnd");
	glUniform1f(location, m_endSize);
	// bind colour information for interpolation that wont change
	location = glGetUniformLocation(m_drawShader, "colourStart");
	glUniform4fv(location, 1, &m_startColour[0]);
	location = glGetUniformLocation(m_drawShader, "colourEnd");
	glUniform4fv(location, 1, &m_endColour[0]);






	//std::ifstream vin(gpuParticleVert);
	//std::string vcontents((std::istreambuf_iterator<char>(vin)), std::istreambuf_iterator<char>());
	//std::ifstream gin(gpuParticleGeom);
	//std::string gcontents((std::istreambuf_iterator<char>(gin)), std::istreambuf_iterator<char>());
	//std::ifstream fin(gpuParticleFrag);
	//std::string fcontents((std::istreambuf_iterator<char>(fin)), std::istreambuf_iterator<char>());


	//GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	//GLuint gShader = glCreateShader(GL_GEOMETRY_SHADER);
	//GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

	//const char *vs = vcontents.c_str();
	//const char *gs = gcontents.c_str();
	//const char *fs = fcontents.c_str();

	//m_drawShader = glCreateProgram();

	//glShaderSource(vShader, 1, &vs, 0);
	//glShaderSource(gShader, 1, &gs, 0);
	//glShaderSource(fShader, 1, &fs, 0);
	//glCompileShader(vShader);
	//glCompileShader(gShader);
	//glCompileShader(fShader);
	//glAttachShader(m_drawShader, vShader);
	//glAttachShader(m_drawShader, gShader);
	//glAttachShader(m_drawShader, fShader);
	//glLinkProgram(m_drawShader);
	//glDeleteShader(vShader);
	//glDeleteShader(gShader);
	//glDeleteShader(fShader);

	int success;

	glGetProgramiv(m_drawShader, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		int infoLogLength = 0;
		glGetProgramiv(m_drawShader, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];
		glGetProgramInfoLog(m_drawShader, infoLogLength, 0, infoLog);
		printf("Error: Failed to link shader program!\n");
		printf("%s\n", infoLog);
		delete[] infoLog;

	}

}

void GPUParticleEmitter::createUpdateShader(const char * vpath) {

	//File Loading
	std::ifstream vin(vpath);
	std::string vcontents((std::istreambuf_iterator<char>(vin)), std::istreambuf_iterator<char>());

	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	const char *vs = vcontents.c_str();
	glShaderSource(vShader, 1, &vs, 0);
	glCompileShader(vShader);
	int success;
	glGetProgramiv(m_updateShader, GL_LINK_STATUS, &success);

	if (success == GL_FALSE)
	{
		int infoLogLength = 0;
		glGetProgramiv(m_updateShader, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];
		glGetProgramInfoLog(m_updateShader, infoLogLength, 0, infoLog);
		printf("Error: Failed to link shader program!\n");
		printf("%s\n", infoLog);
		delete[] infoLog;

	}

	unsigned int uvs = vShader;

	m_updateShader = glCreateProgram();
	glAttachShader(m_updateShader, vShader);

	//data that will stream back
	const char* varyings[] = { "position", "velocity","lifetime", "lifespan" };
	glTransformFeedbackVaryings(m_updateShader, 4, varyings, GL_INTERLEAVED_ATTRIBS);
	glLinkProgram(m_updateShader);

	glDeleteShader(uvs);
	glDeleteShader(vShader);

	glUseProgram(m_updateShader);

	//bind lifetime
	int location = glGetUniformLocation(m_updateShader, "lifeMin");
	glUniform1f(location, m_lifespanMin);
	location = glGetUniformLocation(m_updateShader, "lifeMax");
	glUniform1f(location, m_lifespanMax);

}
