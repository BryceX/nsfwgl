#include <gl_core_4_4.h>
#include <glfw3.h>
#include <fstream>
#include <stb\stb_image.h>
#include "nsfw.h"
#include <FBX\FBXFile.h>
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
	unsigned int vao, vbo, ibo;

	glGenBuffers(1, &vao);	// generate the VAO
	glBindVertexArray(vao);

	glGenBuffers(1,&vbo);	// generate the VBO
	glBindBuffer(GL_ARRAY_BUFFER,vbo);	// bind the VBO

	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vsize, verts, GL_STATIC_DRAW); // buffer the VBO
	glEnableVertexAttribArray(0);  // specify the attributes for the VBO
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE,	sizeof(Vertex), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 4));

	glGenBuffers(1, &ibo);	// generate the IBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);	// bind the IBO
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

bool nsfw::Assets::makeFBO(const char * name, unsigned w, unsigned h, unsigned nTextures, const char * names[], const unsigned depths[])
{

#if _DEBUG
	// check if the data is valid
	if (depths == nullptr)
	{
		std::cout << "ERROR: Texture data is null!" << std::endl;
		return false;
	}
#endif
	ASSET_LOG(GL_HANDLE_TYPE::FBO);
	unsigned int fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	for (int i = 0; i < nTextures; i++)
	{
		unsigned depth = depths[i];
		const char* pixels = names[i];
		makeTexture(name, w, h, depth, pixels);
	}
	
	setINTERNAL(GL_HANDLE_TYPE::FBO, name, nTextures);
	//TODO_D("Create an FBO! Array parameters are for the render targets, which this function should also generate!\nuse makeTexture.\nNOTE THAT THERE IS NO FUNCTION SETUP FOR MAKING RENDER BUFFER OBJECTS.");
	return true;
}

bool nsfw::Assets::makeTexture(const char * name, unsigned w, unsigned h, unsigned depth, const char *pixels)
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
	setINTERNAL(GL_HANDLE_TYPE::TEXTURE, name, depth);	// deliver this to the assman

	//TODO_D("Allocate a texture using the given space/dimensions. Should work if 'pixels' is null, so that you can use this same function with makeFBO\n note that Dept will use a GL value.");
	return true;
}

bool nsfw::Assets::loadTexture(unsigned char * name, const char * path)
{
#if _DEBUG
	// check if the data is valid
	if (path == nullptr)
	{
		std::cout << "ERROR: Texture data is null!" << std::endl;
		return false;
	}
#endif
	int width=0, height=0, format=0;
	name = stbi_load(path,&width,&height,&format,STBI_default);


	unsigned int m_texture;
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, name);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(name);


	//TODO_D("This should load a texture from a file, using makeTexture to perform the allocation.\nUse STBI, and make sure you switch the format STBI provides to match what openGL needs!");
	return true;
}
unsigned int nsfw::Assets::loadSubShader(unsigned int type, const char* path)
{
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

	// generate a blank shader and hold a handle to it so we can access it in the future
	unsigned int shader = glCreateShader(type);
	glShaderSource(shader, 1, &src, 0);	// supply the source for the shader to the shader
	glCompileShader(shader);	// attempt to compile the shader
	delete[] src;	// delete the dynamic char array from the heap. if we don't do this, we'll have a memory leak because we
					// won't be able to delete this anymore because we won't know where it is

					// return a handle to the shader
	return shader;
}

bool nsfw::Assets::loadShader(const char * name, const char * vpath, const char * fpath)
{
#if _DEBUG
	// check if the data is valid
	if (fpath == nullptr)
	{
		std::cout << "ERROR: Texture data is null!" << std::endl;
		return false;
	}
#endif
	
	ASSET_LOG(GL_HANDLE_TYPE::SHADER);
	
	unsigned programID = glCreateProgram();	// store handle to generated shader program
	unsigned vshader = loadSubShader(GL_VERTEX_SHADER, vpath);
	unsigned fshader = loadSubShader(GL_FRAGMENT_SHADER, fpath);
	glAttachShader(programID, vshader);
	glAttachShader(programID, fshader);
	glLinkProgram(programID);
	int success;

	glDeleteShader(vshader);
	glDeleteShader(fshader);
	glGetProgramiv(programID, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		int infoLogLength = 0;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];
		glGetProgramInfoLog(programID, infoLogLength, 0, infoLog);
		printf("Error: Failed to link shader program!\n");
		printf("%s\n", infoLog);
		delete[] infoLog;
	}
	setINTERNAL(GL_HANDLE_TYPE::TEXTURE, name, programID);


	//TODO_D("Load shader from a file.");
	return true;
}

bool nsfw::Assets::loadFBX(const char * name, const char * path)
{
	Vertex temp;
	makeVAO(name,&temp,4, /*??????*/);
	//myFBX.getMeshByName(name);
	//myFBX.getTextureByName(path);

	myFBX.initialiseOpenGLTextures();
	myFBX = temp;

	FBXMeshNode * mesh = temp->getMeshByIndex(0);
	//FBXTexture* texture = temp.getTextureByName(fileName);

	glGenBuffers(1, &myFBX);
	glGenBuffers(1, &rObject.IBO);
	glGenVertexArrays(1, &rObject.VAO);
	glBindVertexArray(rObject.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, rObject.VBO);
	glBufferData(GL_ARRAY_BUFFER, mesh->m_vertices.size()*sizeof(FBXVertex), &mesh->m_vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (void*)(FBXVertex::Offsets::PositionOffset));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (void*)(FBXVertex::Offsets::TexCoord1Offset));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rObject.IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->m_indices.size()*sizeof(unsigned int), &mesh->m_indices[0], GL_STATIC_DRAW);

	rObject.indexCount = mesh->m_indices.size();
	glBindVertexArray(0);	// unbind the VAO by setting to zero, which is NULL
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // unbind the IBO
	glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind the VBO

	//name/meshName
	//name/textureName
	TODO_D("FBX file-loading support needed.\nThis function should call loadTexture and makeVAO internally.\nFBX meshes each have their own name, you may use this to name the meshes as they come in.\nMAKE SURE YOU SUPPORT THE DIFFERENCE BETWEEN FBXVERTEX AND YOUR VERTEX STRUCT!\n");
	return false;
}

bool nsfw::Assets::loadOBJ(const char * name, const char * path)
{
	TODO_D("OBJ file-loading support needed.\nThis function should call makeVAO and loadTexture (if necessary), MAKE SURE TO TAKE THE OBJ DATA AND PROPERLY LINE IT UP WITH YOUR VERTEX ATTRIBUTES (or interleave the data into your vertex struct).\n");
	return false;
}

void nsfw::Assets::init()
{
	TODO_D("Load up some default assets here if you want.");
	
	unsigned int vbo;
	unsigned int ibo;
	unsigned int vao;
	



	setINTERNAL(FBO,"Screen",0);
	
	makeVAO("Cube",CubeVerts,24,CubeTris,36);

	makeVAO("Quad",QuadVerts, 4, QuadTris,6);
	
	char w[] = { 255,255,255,255 };
	makeTexture("White", 1, 1, GL_RGB8, w);
	

}

void nsfw::Assets::term()
{
	TODO();
	for each(std::pair<AssetKey,unsigned> k in handles)
	{
		switch (k.first.first)
		{
		case VBO:		TODO_D("VBO deletion");		break;
		case IBO:		TODO_D("IBO deletion");		break;
		case VAO:		TODO_D("VAO deletion");		break;
		case SHADER:	TODO_D("Shader deletion");	break;
		case TEXTURE:	TODO_D("Texture deletion"); break;
		case RBO:		TODO_D("RBO deletion");		break;
		case FBO:		TODO_D("FBO deletion");		break;
		}
	}
}


