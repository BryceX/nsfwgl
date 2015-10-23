#pragma once


#include "nsfw.h"
#include "Camera.h"
#include "GameObject.h"
#include "ForwardPass.h"

class TestApp :public nsfw::Application
{
	Camera camera;
	GameObject gameObject;
	ForwardPass forwardPass;

public:
	void OnStep();
	void OnPlay();
	void OnInit();
	void OnTerm();

};