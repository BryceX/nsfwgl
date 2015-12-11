#include <gl_core_4_4.h>
#include <glfw3.h>
#include <fstream>
#include <stb\stb_image.h>
#include "nsfw.h"
#include <FBX\FBXFile.h>
#include <TinyOBJ\tiny_obj_loader.h>
#include <vector>

using std::vector;
using namespace nsfw::ASSET;

const char *nsfw::TYPE_NAMES[eSIZE + 1] = { "NONE","vao","ibo","vbo","tri-size","fbo","rbo","texture","shader","SIZE" };

#ifdef _DEBUG
#define ASSET_LOG(type) do { std::cerr << "Trying to create key: <" << TYPE_NAMES[type] << ">" << name << std::endl; } while(0)
#else 
#define ASSET_LOG(type) do {} while (0)
#endif


nsfw::GL_HANDLE nsfw::Assets::getVERIFIED(const AssetKey &key) const
{
#ifdef _DEBUG
			if (!handles.count(key))
			{
				std::cerr << "Asset Key not found: <" << TYPE_NAMES[key.first] << ">" << key.second << std::endl;
				return 0;
			}
#endif
			return handles.at(key);
}

bool nsfw::Assets::setINTERNAL(ASSET::GL_HANDLE_TYPE t, const char *name, GL_HANDLE handle)
{
	AssetKey key(t, name);
#ifdef _DEBUG
	if (handles.count(key))
	{
		std::cerr << "Asset Key already exists: <" << TYPE_NAMES[key.first] << ">" << key.second << " ignoring." << std::endl;
		return false;
	}
	else std::cerr << "Asset Key successfully created: <" << TYPE_NAMES[key.first] << ">" << key.second << std::endl;
#endif
	handles[key] = handle;
	return true;
}


bool nsfw::Assets::makeVAO(const char * name, const struct Vertex *verts, unsigned vsize,  const unsigned * tris, unsigned tsize)
{

#if _DEBUG
	// check if the data is valid
	if (verts == nullptr)
	{
		std::cout << "ERROR: Vertex data is null!" << std::endl;
		return false;
	}
#endif

	// logs for the asset manager
	ASSET_LOG(GL_HANDLE_TYPE::VBO);	// Trying to create key VBO...
	ASSET_LOG(GL_HANDLE_TYPE::IBO);
	ASSET_LOG(GL_HANDLE_TYPE::VAO);
	ASSET_LOG(GL_HANDLE_TYPE::SIZE);
	GLuint vao, vbo, ibo;

	glGenVertexArrays (1, &vao);	// generate the VAO
	glGenBuffers(1,&vbo);	// generate the VBO
	glGenBuffers(1, &ibo);	// generate the IBO

	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);	// bind the IBO
	glBindBuffer(GL_ARRAY_BUFFER,vbo);	// bind the VBO


	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vsize, verts, GL_STATIC_DRAW); // buffer the VBO

	// specify the attributes for the VBO
	glEnableVertexAttribArray(0);	// pos
	glEnableVertexAttribArray(1);	// normal
	glEnableVertexAttribArray(2);	// tangent
	glEnableVertexAttribArray(3);	// texcoord

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(Vertex::POSITION_OFFSET));
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)(Vertex::NORMAL_OFFSET));
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)(Vertex::TANGENT_OFFSET));
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(Vertex::TEXCOORD_OFFSET));
	
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * tsize, tris, GL_STATIC_DRAW); // buffer the IBO

	glBindVertexArray(0); // unbind the VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind the VBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // unbind the IBO

	setINTERNAL(VAO, name, vao);						// save the VAO to the assman
	setINTERNAL(VBO, name, vbo);						// save the VBO to the assman
	setINTERNAL(IBO, name, ibo);						// save the IBO to the assman
	setINTERNAL(GL_HANDLE_TYPE::SIZE, name, tsize);		// save the SIZE to the assman


	//TODO_D("Should generate VBO, IBO, VAO, and SIZE using the parameters, storing them in the 'handles' map.\nThis is where vertex attributes are set!");
	return true;
}
bool nsfw::Assets::makeVAO(const char * name, const struct ParticleVertex *verts, unsigned vsize)
{
#if _DEBUG
	// check if the data is valid
	if (verts == nullptr)
	{
		std::cout << "ERROR: Vertex data is null!" << std::endl;
		return false;
	}
#endif

	// logs for the asset manager
	ASSET_LOG(GL_HANDLE_TYPE::VBO);	// Trying to create key VBO...
	ASSET_LOG(GL_HANDLE_TYPE::VAO);
	ASSET_LOG(GL_HANDLE_TYPE::SIZE);
	GLuint vao, vbo,size=vsize;

	glGenVertexArrays(1, &vao);	// generate the VAO
	glGenBuffers(1, &vbo);	// generate the VBO
		

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);	// bind the VBO


	glBufferData(GL_ARRAY_BUFFER, sizeof(ParticleVertex) * vsize, verts, GL_STATIC_DRAW); // buffer the VBO

																				  // specify the attributes for the VBO
	glEnableVertexAttribArray(0);	// pos
	glEnableVertexAttribArray(1);	// vel
	glEnableVertexAttribArray(2);	// time
	glEnableVertexAttribArray(3);	// span

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ParticleVertex), (void*)(ParticleVertex::POSITION_OFFSET));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ParticleVertex), (void*)(ParticleVertex::VELOCITY_OFFSET));
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(ParticleVertex), (void*)(ParticleVertex::LIFETIME_OFFSET));
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(ParticleVertex), (void*)(ParticleVertex::LIFESPAN_OFFSET));

	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * tsize, tris, GL_STATIC_DRAW); // buffer the IBO

	glBindVertexArray(0); // unbind the VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind the VBO
	

	setINTERNAL(VAO, name, vao);						// save the VAO to the assman
	setINTERNAL(VBO, name, vbo);						// save the VBO to the assman
	setINTERNAL(GL_HANDLE_TYPE::SIZE, name, size);		// save the SIZE to the assman


														//TODO_D("Should generate VBO, IBO, VAO, and SIZE using the parameters, storing them in the 'handles' map.\nThis is where vertex attributes are set!");
	return true;
}

bool nsfw::Assets::makeFBO(const char * name, unsigned w, unsigned h, unsigned nTextures, const char * names[], const unsigned depths[])
{
	// 1. Generate and bind the FBO
	// 2. Iterate over the textures for the FBO
	//		a. generate a texture for it
	//		b. determine what kind of attachment it is
	//		c. attach it to the framebuffer
	// 3. Provide a listing of the color attachments we're using
	// 4. Add the newly created FBO to the asset manager
	//		a. Unbind the FBO
	// 5. Return a bool, indicating whether or not we successfully created an FBO object

#if _DEBUG
	// check if the data is valid
	if (depths == nullptr)
	{
		std::cout << "ERROR: Texture data is null!" << std::endl;
		return false;
	}
#endif
	ASSET_LOG(GL_HANDLE_TYPE::FBO);

	GLuint fbo;	// stores a handle (what OpenGL refers to as a "name") to our FBO
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	int nColors = 0;
	for (int i = 0; i < nTextures; ++i)
	{
		makeTexture(names[i], w, h, depths[i]);
		unsigned Attachment;
		if (depths[i] == GL_DEPTH_COMPONENT)		// GL_DEPTH_COMPONENT	-> GL_DEPTH_ATTACHMENT
		{
			Attachment = GL_DEPTH_ATTACHMENT;
		}
		else if (depths[i] == GL_DEPTH_STENCIL)
		{
			Attachment = GL_STENCIL_ATTACHMENT;		// GL_DEPTH_STENCIL		->	GL_STENCIL_ATTACHMENT
		}
		else
		{
			Attachment = GL_COLOR_ATTACHMENT0 + nColors++;	// 
		}

		glFramebufferTexture(GL_FRAMEBUFFER, Attachment, get<TEXTURE>(names[i]), 0);
	}

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("Framebuffer Error!\n");

		// ABORT ABORT ABORT
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDeleteFramebuffers(1, &fbo);			// clean up
		return false;
	}
	
	GLenum *colorAttachments = new GLenum[nColors];
	for (int i = 0; i < nColors; ++i)
	{
		colorAttachments[i] = GL_COLOR_ATTACHMENT0 + i;
	}
	glDrawBuffers(nColors, colorAttachments);
	delete[] colorAttachments;
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	setINTERNAL(FBO, name, fbo);
	return true;
}

bool nsfw::Assets::makeTexture(const char * name, unsigned w, unsigned h, unsigned depth, const unsigned char *pixels)
{
#if _DEBUG
	// check if the data is valid
	if (name == nullptr)
	{
		std::cout << "ERROR: Texture data is null!" << std::endl;
		return false;
	}
#endif
	

	ASSET_LOG(GL_HANDLE_TYPE::TEXTURE);
	unsigned int texHandle;
	glGenTextures(1, &texHandle);	// texHandle will be assigned a value that corresponds to the texture within OpenGL
	glBindTexture(GL_TEXTURE_2D, texHandle); // bind the texture
	glTexImage2D(GL_TEXTURE_2D, 0, depth, w, h, 0, depth, GL_UNSIGNED_BYTE, pixels); // send the pixel data
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);// specify min filter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);// specify max filter
	glBindTexture(GL_TEXTURE_2D, 0);		// unbind the texture
	setINTERNAL(GL_HANDLE_TYPE::TEXTURE, name, texHandle);	// deliver this to the assman
															// TODO: find out if this works
															// HACK: this probably doesn't work
															// CONSIDER: does this actually work?

	//TODO_D("Allocate a texture using the given space/dimensions. Should work if 'pixels' is null, so that you can use this same function with makeFBO\n note that Dept will use a GL value.");
	return true;
}

bool nsfw::Assets::loadTexture(const char * name, const char * path)
{
	// 1. Load a texture from file
	// 2. Feed texture data into makeTexture
	// 3. Returning whether or not we successfully loaded the texture into OpenGL

#if _DEBUG
	// check if the data is valid
	if (name == nullptr)
	{
		std::cout << "ERROR: Texture data is null!" << std::endl;
		return false;
	}
#endif
	int width = 0, height = 0, format = 0;
	unsigned char* data = stbi_load(path, &width, &height, &format, STBI_default);

	switch (format)
	{
	case STBI_grey:			format = GL_RED;	break;
	case STBI_grey_alpha:	format = GL_RG;		break;
	case STBI_rgb:			format = GL_RGB;	break;
	case STBI_rgb_alpha:	format = GL_RGBA;	break;
	}

	makeTexture(name, width, height, format, data);
	stbi_image_free(data);


	//TODO_D("This should load a texture from a file, using makeTexture to perform the allocation.\nUse STBI, and make sure you switch the format STBI provides to match what openGL needs!");
	return true;
}
unsigned int nsfw::Assets::loadSubShader(unsigned int type, const char* path)
{
	// loading in the shader source
	std::ifstream in(path);	// opens a inbound-file stream to the file containing the shader source code

	if (!in.is_open())
	{
		std::cerr << "Failed to open shader file!" << std::endl;
		return -1;
	}

	std::string contents((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());	// load the source code into a std::string object
	char *src = new char[contents.length() + 1];

	// copy the string into the char array
	strncpy_s(src, sizeof(char)*contents.length() + 1, contents.c_str(), sizeof(char)*contents.length() + 1);

	// create a shader
	unsigned int shader = glCreateShader(type);
	glShaderSource(shader, 1, &src, 0);	// supply the source for the shader to the shader
	glCompileShader(shader);	// attempt to compile the shader
	delete[] src;	// delete the dynamic char array from the heap. if we don't do this, we'll have a memory leak because we
					// won't be able to delete this anymore because we won't know where it is

	// return a handle to the shader
	return shader;
}

bool nsfw::Assets::loadShader(const char * name, const char * vpath, const char *fpath)
{

	
	ASSET_LOG(GL_HANDLE_TYPE::SHADER);
	GLuint shader = glCreateProgram();

	std::ifstream vin(vpath);
	std::string vcontents((std::istreambuf_iterator<char>(vin)), std::istreambuf_iterator<char>());
	std::ifstream fin(fpath);
	std::string fcontents((std::istreambuf_iterator<char>(fin)), std::istreambuf_iterator<char>());

	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

	const char *vs = vcontents.c_str();
	const char *fs = fcontents.c_str();

	glShaderSource(vShader, 1, &vs, 0);
	glShaderSource(fShader, 1, &fs, 0);
	glCompileShader(vShader);
	glCompileShader(fShader);
	glAttachShader(shader, vShader);
	glAttachShader(shader, fShader);
	glLinkProgram(shader);
	glDeleteShader(vShader);
	glDeleteShader(fShader);


	int success;

	glGetProgramiv(shader, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		int infoLogLength = 0;
		glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];
		glGetProgramInfoLog(shader, infoLogLength, 0, infoLog);
		printf("Error: Failed to link shader program!\n");
		printf("%s\n", infoLog);
		delete[] infoLog;
		return false;
	}
	setINTERNAL(GL_HANDLE_TYPE::SHADER, name, shader);


	//TODO_D("Load shader from a file.");
	return true;
}
bool nsfw::Assets::loadShader(const char * name, const char *vpath, const char *gpath , const char *fpath)
{

	ASSET_LOG(GL_HANDLE_TYPE::SHADER);
	GLuint shader = glCreateProgram();

	std::ifstream vin(vpath);
	std::string vcontents((std::istreambuf_iterator<char>(vin)), std::istreambuf_iterator<char>());
	std::ifstream gin(gpath);
	std::string gcontents((std::istreambuf_iterator<char>(gin)), std::istreambuf_iterator<char>());
	std::ifstream fin(fpath);
	std::string fcontents((std::istreambuf_iterator<char>(fin)), std::istreambuf_iterator<char>());


	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint gShader = glCreateShader(GL_GEOMETRY_SHADER);
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

	const char *vs = vcontents.c_str();
	const char *gs = gcontents.c_str();
	const char *fs = fcontents.c_str();

	glShaderSource(vShader, 1, &vs, 0);
	glShaderSource(gShader, 1, &gs, 0);
	glShaderSource(fShader, 1, &fs, 0);
	glCompileShader(vShader);
	glCompileShader(gShader);
	glCompileShader(fShader);
	glAttachShader(shader, vShader);
	glAttachShader(shader, gShader);
	glAttachShader(shader, fShader);
	glLinkProgram(shader);
	glDeleteShader(vShader);
	glDeleteShader(gShader);
	glDeleteShader(fShader);


	int success;

	glGetProgramiv(shader, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		int infoLogLength = 0;
		glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];
		glGetProgramInfoLog(shader, infoLogLength, 0, infoLog);
		printf("Error: Failed to link shader program!\n");
		printf("%s\n", infoLog);
		delete[] infoLog;
		return false;
	}
	setINTERNAL(GL_HANDLE_TYPE::SHADER, name, shader);


	//TODO_D("Load shader from a file.");
	return true;
}
//unsigned int nsfw::Assets::loadSingleShader(unsigned int type, const char* path) 
//{
//	FILE* file = fopen(path, "rb");
//	if (file == nullptr)
//		return 0;
//	// read the shader source
//	fseek(file, 0, SEEK_END);
//	unsigned int length = ftell(file);
//	fseek(file, 0, SEEK_SET);
//	char* source = new char[length + 1];
//	memset(source, 0, length + 1);
//	fread(source, sizeof(char), length, file);
//	fclose(file);
//	unsigned int shader = glCreateShader(type);
//	glShaderSource(shader, 1, &source, 0);
//	glCompileShader(shader);
//	delete[] source;
//	return shader;
//
//}

bool nsfw::Assets::loadUpdateShader(const char * name, const char * vpath, const char* varyings[], int noutputvars)
{


	ASSET_LOG(GL_HANDLE_TYPE::SHADER);
	GLuint shader = glCreateProgram();

	std::ifstream vin(vpath);
	std::string vcontents((std::istreambuf_iterator<char>(vin)), std::istreambuf_iterator<char>());

	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);

	const char *vs = vcontents.c_str();

	glShaderSource(vShader, 1, &vs, 0);
	glCompileShader(vShader);
	glAttachShader(shader, vShader);
	// const char* varyings[] = { "position", "velocity","lifetime", "lifespan" };

	glTransformFeedbackVaryings(shader, noutputvars, varyings, GL_INTERLEAVED_ATTRIBS);

	glLinkProgram(shader);
	glDeleteShader(vShader);


	int success;

	glGetProgramiv(shader, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		int infoLogLength = 0;
		glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];
		glGetProgramInfoLog(shader, infoLogLength, 0, infoLog);
		printf("Error: Failed to link shader program!\n");
		printf("%s\n", infoLog);
		delete[] infoLog;
		return false;
	}
	setINTERNAL(GL_HANDLE_TYPE::SHADER, name, shader);


	//TODO_D("Load shader from a file.");
	return true;
}


bool nsfw::Assets::loadFBX(const char * name, const char * path)
{
	// TODO List for loadFBX
	//	OK	1. Load in the FBX file at the specified path
	//		2. Create an array of verticies using our Vertex data structure
	//		3. Create an array of indicies using unsigned integers
	//		4. Pass those arrays into the makeVAO function
	//		5. Return a boolean stating whether or not we were able to successfully load the FBX files into OpenGL

#if _DEBUG
	// check if the data is valid
	if (name == nullptr)
	{
		std::cout << "ERROR: FBX data is null!" << std::endl;
		return false;
	}
#endif
	FBXFile myFBX;
	bool status = myFBX.load(path, FBXFile::UNITS_CENTIMETER, true, false, true);

	assert(status == true);	// I AM ASSERTING THAT THIS EXPRESSION IS TRUE
							// 
							// IF THIS EXPRESSION IS FALSE, STOP WHAT YOU ARE DOING
	if (status == false)
	{
		// there are multiple streams
		//
		// std::cout -- goes to stdout, just general console output
		// std::cerr -- goes to stderr, reserved for reporting errors, still goes to console
		// std::cin  -- goes to stdin, for accepting user input
		std::cerr << "FBX file failed to load." << std::endl;

		return false;	// exit early
	}
	myFBX.initialiseOpenGLTextures();
	FBXMeshNode * mesh = myFBX.getMeshByIndex(0);
	vector<Vertex> FBXVertices;
	FBXVertices.reserve(mesh->m_vertices.size());
	for (int currentVertex = 0; currentVertex < mesh->m_vertices.size();++currentVertex)
	{
		Vertex tempVert;
		tempVert.position = mesh->m_vertices[currentVertex].position; // makes our vposition in the vertex structure = to the position in the fbx mesh
		tempVert.tangent = mesh->m_vertices[currentVertex].tangent;
		tempVert.normal = mesh->m_vertices[currentVertex].normal;
		tempVert.texCoord = mesh->m_vertices[currentVertex].texCoord1;
		FBXVertices.push_back(tempVert);
	}

	makeVAO(name, FBXVertices.data(), FBXVertices.size(), mesh->m_indices.data(), mesh->m_indices.size());
	//TODO_D("FBX file-loading support needed.\nThis function should call loadTexture and makeVAO internally.\nFBX meshes each have their own name, you may use this to name the meshes as they come in.\nMAKE SURE YOU SUPPORT THE DIFFERENCE BETWEEN FBXVERTEX AND YOUR VERTEX STRUCT!\n");
	return true;
}

bool nsfw::Assets::loadOBJ(const char * name, const char * path)//fucked up
{
	// 1. Load the obj file
	// 2. Load the tinyObj data into our Vertex data
	// 3. Pass that information into makeVAO
	// 4. Return true if we successfully loaded the file, otherwise return false
	assert(false);
#if _DEBUG
	// check if the data is valid
	if (name == nullptr)
	{
		std::cout << "ERROR: FBX data is null!" << std::endl;
		return false;
	}
#endif

	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string object = tinyobj::LoadObj(shapes, materials,path);

	std::vector<Vertex> vertices;

	for (unsigned int mesh_index = 0;mesh_index < shapes.size();++mesh_index)
	{
		unsigned int float_count = shapes[mesh_index].mesh.positions.size();
		float_count += shapes[mesh_index].mesh.normals.size();
		float_count += shapes[mesh_index].mesh.texcoords.size();
		std::vector<float> vertex_data;
		vertex_data.reserve(float_count);
		vertex_data.insert(vertex_data.end(),shapes[mesh_index].mesh.positions.begin(),	shapes[mesh_index].mesh.positions.end());
		vertex_data.insert(vertex_data.end(),shapes[mesh_index].mesh.normals.begin(), shapes[mesh_index].mesh.normals.end());
	}

	//makevao
	makeVAO(name, vertices.data(), vertices.size(), shapes[0].mesh.indices.data(), shapes[0].mesh.indices.size());

	//TODO_D("OBJ file-loading support needed.\nThis function should call makeVAO and loadTexture (if necessary), MAKE SURE TO TAKE THE OBJ DATA AND PROPERLY LINE IT UP WITH YOUR VERTEX ATTRIBUTES (or interleave the data into your vertex struct).\n");
	return true;
}

void nsfw::Assets::init()
{
	//TODO_D("Load up some default assets here if you want.");
	
	unsigned int vbo;
	unsigned int ibo;
	unsigned int vao;
	



	setINTERNAL(FBO,"Screen",0);
	
	makeVAO("Cube",CubeVerts,24,CubeTris,36);

	makeVAO("Quad",QuadVerts, 4, QuadTris,6);
	
	unsigned char w[] = { 255,255,255,255 };
	makeTexture("White", 1, 1, GL_RGBA, w);
	

}

void nsfw::Assets::term()
{
	//TODO();
	for each(std::pair<AssetKey,unsigned> k in handles)
	{
		GLuint handle = k.second;
		switch (k.first.first)
		{
		case VBO:		glDeleteBuffers(1,&handle);			break;
		case IBO:		glDeleteBuffers(1,&handle);			break;
		case VAO:		glDeleteVertexArrays(1,&handle);	break;
		case SHADER:	glDeleteProgram(handle);			break;
		case TEXTURE:	glDeleteTextures(1,&handle);		break;
		case FBO:		glDeleteFramebuffers(1,&handle);	break;
		}
	}
}


