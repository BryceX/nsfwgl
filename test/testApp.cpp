#include "testApp.h"

void TestApp::OnStep() {}
void TestApp::OnPlay() 
{
	camera.lookAt(glm::vec3(10, 10, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	gameObject.transform = glm::mat4(1);
	gameObject.diffuse = "White";
	gameObject.mesh = "Quad";
	gameObject.tris = "Quad";
	forwardPass.shader = "Basic";
	forwardPass.fbo = "Screen";
}
void TestApp::OnInit() {}
void TestApp::OnTerm() {}