
#include "testApp.h"

#define 	GLFW_KEY_UNKNOWN   -1

#define 	GLFW_KEY_SPACE   32

#define 	GLFW_KEY_APOSTROPHE   39 /* ' */

#define 	GLFW_KEY_COMMA   44 /* , */

#define 	GLFW_KEY_MINUS   45 /* - */

#define 	GLFW_KEY_PERIOD   46 /* . */

#define 	GLFW_KEY_SLASH   47 /* / */

#define 	GLFW_KEY_0   48

#define 	GLFW_KEY_1   49

#define 	GLFW_KEY_2   50

#define 	GLFW_KEY_3   51

#define 	GLFW_KEY_4   52

#define 	GLFW_KEY_5   53

#define 	GLFW_KEY_6   54

#define 	GLFW_KEY_7   55

#define 	GLFW_KEY_8   56

#define 	GLFW_KEY_9   57

#define 	GLFW_KEY_SEMICOLON   59 /* ; */

#define 	GLFW_KEY_EQUAL   61 /* = */

#define 	GLFW_KEY_A   65

#define 	GLFW_KEY_B   66

#define 	GLFW_KEY_C   67

#define 	GLFW_KEY_D   68

#define 	GLFW_KEY_E   69

#define 	GLFW_KEY_F   70

#define 	GLFW_KEY_G   71

#define 	GLFW_KEY_H   72

#define 	GLFW_KEY_I   73

#define 	GLFW_KEY_J   74

#define 	GLFW_KEY_K   75

#define 	GLFW_KEY_L   76

#define 	GLFW_KEY_M   77

#define 	GLFW_KEY_N   78

#define 	GLFW_KEY_O   79

#define 	GLFW_KEY_P   80

#define 	GLFW_KEY_Q   81

#define 	GLFW_KEY_R   82

#define 	GLFW_KEY_S   83

#define 	GLFW_KEY_T   84

#define 	GLFW_KEY_U   85

#define 	GLFW_KEY_V   86

#define 	GLFW_KEY_W   87

#define 	GLFW_KEY_X   88

#define 	GLFW_KEY_Y   89

#define 	GLFW_KEY_Z   90
void TestApp::onInit() 
{
	
	auto &window = nsfw::Window::instance();

	nsfw::Assets::instance().loadShader("Basic", "Shaders/fwdVert.txt","Shaders/fwdFrag.txt");
	nsfw::Assets::instance().loadShader("PostProc", "Shaders/postVert.txt","Shaders/postFrag.txt");
	nsfw::Assets::instance().loadShader("Shadows", "Shaders/shadowVert.txt", "Shaders/shadowFrag.txt");
	GPUPE.Init(100, .1f, 5.0f, 1,10,1,.1f,glm::vec4(1.f,0.f,0.f,1.f),vec4(1.f,1.f,0.f,1.f));
	const char * shadowTexture[] = { "ShadowMap" };
	const unsigned shadowDepth[] = { GL_DEPTH_COMPONENT };
	nsfw::Assets::instance().makeFBO("ShadowPass", 1024, 1024, 1, shadowTexture, shadowDepth);


	const char * RenderTextures[] = { "RenderColor", "RenderDepth" };
	const unsigned RenderDepths[] = { GL_RGBA, GL_DEPTH_COMPONENT };
	nsfw::Assets::instance().makeFBO("Render", window.getWidth(), window.getHeight(), 2, RenderTextures, RenderDepths);	

	
	nsfw::Assets::instance().loadFBX("Spear", "../FBX/soulspear/soulspear.fbx");
	nsfw::Assets::instance().loadTexture("SpearDiffuse", "../FBX/soulspear/soulspear_diffuse.tga");
	nsfw::Assets::instance().loadTexture("SpearNormal", "../FBX/soulSpear/soulspear_normal.tga");
	nsfw::Assets::instance().loadTexture("RockFloorDiffuse", "../textures/rock_diffuse.tga");
	nsfw::Assets::instance().loadTexture("RockFloorNormal", "../textures/rock_normal.tga");
	////nsfw::Assets::instance().loadOBJ("Bunny", "../OBJ/bunny.obj");



//	nsfw::Assets::instance().loadTexture("Purple", "../resources/textures/purple.png");
}

void TestApp::onPlay()
{
	// initialize my scene objects
	camera.maspect = nsfw::Window::instance().getWidth() / (float)nsfw::Window::instance().getHeight();
	camera.lookAt(glm::vec3(0, 0, 6), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	
	// initialize my gameObject
	gameObject.transform = glm::mat4(1);
	gameObject.diffuse   = "SpearDiffuse";
	gameObject.normalMap = "SpearNormal";
	gameObject.mesh		 = "Spear";
	gameObject.tris		 = "Spear";

	/*gameObject1.transform = glm::translate(2,0,1);
	gameObject1.diffuse  = "White";
	gameObject1.mesh	 = "Spear";
	gameObject1.tris	 = "Spear";*/

	floor.transform =  glm::translate(0.f,0.0f,0.f) * glm::rotate(-90.f,vec3(1.f,0.f,0.f))  * glm::scale(10.f, 10.f, 1.f);
	floor.diffuse = "RockFloorDiffuse";
	floor.normalMap = "RockFloorNormal";
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

	directionLight.color = glm::vec4(.5f,.5f,.5f,1.f);
	directionLight.direction = glm::normalize(glm::vec4(0.f,0.f,1.0f,0));


}

void TestApp::onStep()
{
	// recalc delta time
	
	nsfw::Window::instance().SetTime();

	float time = nsfw::Window::instance().timePassed;

	// - game logic


	gameObject.transform = glm::translate( 0, 0, 0 );

	directionLight.direction = glm::rotate(time * 100, glm::vec3(0.f, 0.f, 1.f)) * glm::vec4(0.f, 1.f, 0.f, 0.f);

	//PE.Update(nsfw::Window::instance().deltaTime);

	shadowPass.prep();
	shadowPass.draw(gameObject, directionLight);
//	shadowPass.draw(gameObject1, directionLight);
	shadowPass.draw(floor, directionLight);
//	shadowPass.draw(wall, directionLight);
	shadowPass.post();

	forwardPass.prep();
	forwardPass.draw(camera, gameObject, directionLight);
//	forwardPass.draw(camera, gameObject1, directionLight);
	forwardPass.draw(camera, floor, directionLight);
//	forwardPass.draw(camera, wall, directionLight);
//	forwardPass.draw(camera, PE, directionLight);
	forwardPass.draw(camera, GPUPE);
	forwardPass.post();
	camera.MovePan(GLFW_KEY_W, GLFW_KEY_S, GLFW_KEY_A, GLFW_KEY_D);

//	postPass.prep();
//	postPass.draw();
//	postPass.post();
}


void TestApp::onTerm()
{
}