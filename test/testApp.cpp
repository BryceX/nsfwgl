#include "testApp.h"

// - loads up external assets for the project to use
//   - shaders
//	 - models
//   - textures
// - initializes certain low-level GL assets required
//   - FBOs
void TestApp::onInit() 
{
	// instantiating a reference to the current window
	auto &window = nsfw::Window::instance();	

	// loading the shaders
	nsfw::Assets::instance().loadShader("Basic", "Shaders/fwdVert.txt","Shaders/fwdFrag.txt");
	nsfw::Assets::instance().loadShader("PostProc", "Shaders/postVert.txt","Shaders/postFrag.txt");
	nsfw::Assets::instance().loadShader("Shadows", "Shaders/shadowVert.txt", "Shaders/shadowFrag.txt");

	// creating FBOs
	const char * shadowTexture[] = { "ShadowMap" };
	const unsigned shadowDepth[] = { GL_DEPTH_COMPONENT };
	nsfw::Assets::instance().makeFBO("ShadowPass", 1024, 1024, 1, shadowTexture, shadowDepth);

	const char * RenderTextures[] = { "RenderColor", "RenderDepth" };
	const unsigned RenderDepths[] = { GL_RGBA, GL_DEPTH_COMPONENT };
	nsfw::Assets::instance().makeFBO("Render", window.getWidth(), window.getHeight(), 2, RenderTextures, RenderDepths);	

	// load up models and textures
	nsfw::Assets::instance().loadFBX("Spear", "../FBX/soulspear/soulspear.fbx");
	nsfw::Assets::instance().loadTexture("SpearDiffuse", "../FBX/soulspear/soulspear_diffuse.tga");
	nsfw::Assets::instance().loadTexture("SpearNormal", "../FBX/soulSpear/soulspear_normal.tga");
	nsfw::Assets::instance().loadTexture("RockFloorDiffuse", "../textures/rock_diffuse.tga");
	nsfw::Assets::instance().loadTexture("RockFloorNormal", "../textures/rock_normal.tga");
	//nsfw::Assets::instance().loadOBJ("Bunny", "../OBJ/bunny.obj");
}

// - initializing gameobjects
// - initializing certain game related entities
//   - ParticleSystem
//	 - Light
void TestApp::onPlay()
{
	// setting camera direction it looks at
	camera.maspect = nsfw::Window::instance().getWidth() / (float)nsfw::Window::instance().getHeight();
	camera.lookAt(glm::vec3(0, 0, 6), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	// initialize my gameObject
	gameObject.transform = glm::mat4(1);		// changes where the object is draw at (trans/pos, rot, scale)
	gameObject.diffuse   = "SpearDiffuse";		// putting texture on the object
	gameObject.normalMap = "SpearNormal";  //putting shadow and depth on a spear
	gameObject.mesh		 = "Spear"; // has the location of allt he vertices in the model
	gameObject.tris		 = "Spear"; // what the fuck is this shit

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

	// initialize the gpu particle emitter and seting values for the particles it spits out
	GPUPE.Init(100,			// max particles
			  .1f, 5.0f,	// min and max lifespan
			   1, 10,		// start and end velocity
			   1, .1f,		// starting and end size
			   glm::vec4(1.f, 0.f, 0.f, 1.f), vec4(1.f, 1.f, 0.f, 1.f)); // start and end color
	
	/*PE.Init(30);
	PE.diffuse = "White";
	PE.mesh = "Cube";
	PE.tris = "Cube";
	PE.startPosition = glm::vec3(0, 0, 0);
	PE.speed = 1;
	PE.lifeSpan = 3;
	PE.emissionRate = .5;*/

	// initialize my passes
	forwardPass.shader = "Basic";	// shader program used for drawing the meshes
	forwardPass.fbo = "Screen";		// the frame buffer object that we draw to
	
	shadowPass.shader = "Shadows";
	shadowPass.fbo = "ShadowPass";

	postPass.shader = "PostProc";
	postPass.fbo = "Screen";
	postPass.color = "RenderColor";

	//initializing rgb color and direction of light
	directionLight.color = glm::vec4(.5f,.5f,.5f,1.f);
	directionLight.direction = glm::normalize(glm::vec4(0.f,0.f,1.0f,0));
}

// - calculations we perform each frame
// - input handling
// - drawing
void TestApp::onStep()
{
	float time = nsfw::Window::instance().timePassed;

	// - game logic
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

	// HACK: this is jank (not dank) as fuck
	// dank memes cant melt steel beans
	camera.MovePan('W', 'S', 'A', 'D');

//	postPass.prep();
//	postPass.draw();
//	postPass.post();
}


void TestApp::onTerm()
{
}