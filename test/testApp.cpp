
#include "testApp.h"
void TestApp::onInit() 
{
	
	auto			&window	 = nsfw::Window::instance();

	nsfw::Assets::instance().loadShader("Basic", "Shaders/fwdVert.txt","Shaders/fwdFrag.txt");
	nsfw::Assets::instance().loadShader("PostProc", "Shaders/postVert.txt","Shaders/postFrag.txt");
	nsfw::Assets::instance().loadShader("Shadows", "Shaders/shadowVert.txt", "Shaders/shadowFrag.txt");

	const char * shadowTexture[] = { "ShadowMap" };
	const unsigned shadowDepth[] = { GL_DEPTH_COMPONENT };
	nsfw::Assets::instance().makeFBO("ShadowPass", 1024, 1024, 1, shadowTexture, shadowDepth);


	const char * RenderTextures[] = { "RenderColor", "RenderDepth" };
	const unsigned RenderDepths[] = { GL_RGBA, GL_DEPTH_COMPONENT };
	nsfw::Assets::instance().makeFBO("Render", window.getWidth(), window.getHeight(), 2, RenderTextures, RenderDepths);	


	nsfw::Assets::instance().loadFBX("Spear", "../FBX/soulspear/soulspear.fbx");
	nsfw::Assets::instance().loadTexture("Spear", "../FBX/soulspear/soulspear_diffuse.tga");
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
	gameObject.diffuse   = "Spear";
	gameObject.mesh		 = "Spear";
	gameObject.tris		 = "Spear";

	gameObject1.transform = glm::translate(1,0,0);
	gameObject1.diffuse  = "White";
	gameObject1.mesh	 = "Spear";
	gameObject1.tris	 = "Spear";

	floor.transform =  glm::rotate(90.f,vec3(-1.f,0.f,0.f))  * glm::scale(10.f, 10.f, 1.f);
	floor.diffuse = "White";
	floor.mesh = "Quad";
	floor.tris = "Quad";
	// initialize my forward pass
	forwardPass.shader = "Basic";
	forwardPass.fbo = "Render";
	
	shadowPass.shader = "Shadows";
	shadowPass.fbo = "ShadowPass";

	

	postPass.shader = "PostProc";
	postPass.fbo = "Screen";
	postPass.color = "RenderColor";

	directionLight.color = glm::vec4(1.f,.5f,.5f,1.f);
	directionLight.direction = glm::normalize(glm::vec3(1.f,2.5f,1.f));


}

void TestApp::onStep()
{
	nsfw::Window::instance().SetTime();
	float time = nsfw::Window::instance().timePassed;
	gameObject.transform = glm::rotate(time * 100, glm::vec3(0, 1, 0));

	shadowPass.prep();
	shadowPass.draw(gameObject, directionLight);
	shadowPass.draw(gameObject1, directionLight);
	shadowPass.draw(floor, directionLight);
	shadowPass.post();

	forwardPass.prep();
	forwardPass.draw(camera, gameObject, directionLight);
	forwardPass.draw(camera, gameObject1, directionLight);
	forwardPass.draw(camera, floor, directionLight);
	forwardPass.post();



	postPass.prep();
	postPass.draw();
	postPass.post();
}


void TestApp::onTerm()
{
}