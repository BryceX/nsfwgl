
#include "testApp.h"
void TestApp::onInit() 
{
	
	auto			&window	 = nsfw::Window::instance();

	nsfw::Assets::instance().loadShader("Basic", "testVert.txt",
		"testFrag.txt");

	nsfw::Assets::instance().loadShader("PostProc", "postVert.txt","postFrag.txt");

	const char * RenderTextures[] = { "RenderColor", "RenderDepth" };
	const unsigned RenderDepths[] = { GL_RGBA, GL_DEPTH_COMPONENT };
	//int 
	nsfw::Assets::instance().makeFBO("Render", window.getWidth(), window.getHeight(), 2, RenderTextures, RenderDepths);
	nsfw::Assets::instance().loadFBX("Spear", "../FBX/soulspear/soulspear.fbx");
	//nsfw::Assets::instance().loadTexture("Spear", "../FBX/soulspear/soulspear_diffuse.tga");
	////nsfw::Assets::instance().loadOBJ("Bunny", "../OBJ/bunny.obj");

//	nsfw::Assets::instance().loadTexture("Purple", "../resources/textures/purple.png");
}

void TestApp::onPlay()
{
	// initialize my scene objects
	camera.maspect = nsfw::Window::instance().getWidth() / (float)nsfw::Window::instance().getHeight();
	camera.lookAt(glm::vec3(3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	
	// initialize my gameObject
	gameObject.transform = glm::mat4(1);
	gameObject.diffuse = "White";
	gameObject.mesh = "Spear";
	gameObject.tris = "Spear";
	// initialize my forward pass
	forwardPass.shader = "Basic";
	forwardPass.fbo = "Render";

	postPass.shader = "PostProc";
	postPass.fbo = "Screen";
	postPass.color = "RenderColor";
}

void TestApp::onStep()
{
	float time = nsfw::Window::instance().timePassed;
	//gameObject.transform = glm::rotate(time * 10, glm::vec3(0, 1, 0));

	forwardPass.prep();
	forwardPass.draw(camera, gameObject);
	forwardPass.post();

	postPass.prep();
	postPass.draw();
	postPass.post();
}


void TestApp::onTerm()
{
}