
#include "testApp.h"
void TestApp::onInit() 
{
	
	auto &window = nsfw::Window::instance();

	nsfw::Assets::instance().loadShader("Basic", "Shaders/fwdVert.txt","Shaders/fwdFrag.txt");
	nsfw::Assets::instance().loadShader("PostProc", "Shaders/postVert.txt","Shaders/postFrag.txt");
	nsfw::Assets::instance().loadShader("Shadows", "Shaders/shadowVert.txt", "Shaders/shadowFrag.txt");
	GPUPE.Init(20,5,20,5,20,1,9,glm::vec4(1.f,1.f,1.f,1.f),vec4(1.f,0.f,0.f,1.f));
	const char * shadowTexture[] = { "ShadowMap" };
	const unsigned shadowDepth[] = { GL_DEPTH_COMPONENT };
	nsfw::Assets::instance().makeFBO("ShadowPass", 1024, 1024, 1, shadowTexture, shadowDepth);


	const char * RenderTextures[] = { "RenderColor", "RenderDepth" };
	const unsigned RenderDepths[] = { GL_RGBA, GL_DEPTH_COMPONENT };
	nsfw::Assets::instance().makeFBO("Render", window.getWidth(), window.getHeight(), 2, RenderTextures, RenderDepths);	

	
	//nsfw::Assets::instance().loadFBX("Spear", "../FBX/soulspear/soulspear.fbx");
	//nsfw::Assets::instance().loadTexture("Spear", "../FBX/soulspear/soulspear_diffuse.tga");
	////nsfw::Assets::instance().loadOBJ("Bunny", "../OBJ/bunny.obj");

//	nsfw::Assets::instance().loadTexture("Purple", "../resources/textures/purple.png");
}

void TestApp::onPlay()
{
	// initialize my scene objects
	camera.maspect = nsfw::Window::instance().getWidth() / (float)nsfw::Window::instance().getHeight();
	camera.lookAt(glm::vec3(10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	
	// initialize my gameObject
	gameObject.transform = glm::mat4(1);
	gameObject.diffuse   = "Spear";
	gameObject.mesh		 = "Spear";
	gameObject.tris		 = "Spear";

	/*gameObject1.transform = glm::translate(2,0,1);
	gameObject1.diffuse  = "White";
	gameObject1.mesh	 = "Spear";
	gameObject1.tris	 = "Spear";*/

	floor.transform =  glm::translate(0.f,0.0f,0.f) * glm::rotate(-90.f,vec3(1.f,0.f,0.f))  * glm::scale(10.f, 10.f, 1.f);
	floor.diffuse = "White";
	floor.mesh = "Quad";
	floor.tris = "Quad";

	wall.transform = glm::translate(-10.f, 0.0f, 0.f) * glm::rotate(90.f, vec3(0.f, 1.f, 0.f)) *  glm::scale(10.f, 10.f, 1.f);
	wall.diffuse = "White";
	wall.mesh = "Quad";
	wall.tris = "Quad";

	
	PE.Init(30);
	PE.diffuse = "White";
	PE.mesh = "Cube";
	PE.tris = "Cube";
	PE.startPosition = glm::vec3(0, 0, 0);
	PE.speed = 1;
	PE.lifeSpan = 3;
	PE.emissionRate = .5;

	// initialize my forward pass
	forwardPass.shader = "Basic";
	forwardPass.fbo = "Screen";
	
	shadowPass.shader = "Shadows";
	shadowPass.fbo = "ShadowPass";

	postPass.shader = "PostProc";
	postPass.fbo = "Screen";
	postPass.color = "RenderColor";

	//directionLight.color = glm::vec4(.5f,.5f,.5f,1.f);
	directionLight.direction = glm::normalize(glm::vec4(0.f,0.f,1.0f,0));


}

void TestApp::onStep()
{

	

	nsfw::Window::instance().SetTime();
	float time = nsfw::Window::instance().timePassed;
	gameObject.transform = glm::rotate(time * 100, glm::vec3(0, 1, 0));

	directionLight.direction = glm::rotate(time*10, glm::vec3(0.f, 0.f, 1.f)) * glm::vec4(0.f,1.f,0.f,0.f);

	PE.Update(nsfw::Window::instance().deltaTime);

	shadowPass.prep();
	shadowPass.draw(gameObject, directionLight);
	shadowPass.draw(gameObject1, directionLight);
	shadowPass.draw(floor, directionLight);
	shadowPass.draw(wall, directionLight);
	shadowPass.post();

	forwardPass.prep();
	//forwardPass.draw(camera, gameObject, directionLight);
	//forwardPass.draw(camera, gameObject1, directionLight);
	//forwardPass.draw(camera, floor, directionLight);
	//forwardPass.draw(camera, wall, directionLight);
	//forwardPass.draw(camera, PE, directionLight);
	//forwardPass.post();
	GPUPE.Draw(time,camera.transform,camera.getProjection());
	

	//postPass.prep();
	//postPass.draw();
	//postPass.post();
}


void TestApp::onTerm()
{
}