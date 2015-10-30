#pragma once

#include "ForwardPass.h"
#include "PostProcPass.h"
#include "nsfw.h"
#include "Camera.h"
#include "GameObject.h"


class TestApp : public nsfw::Application
{
	Camera camera;
	GameObject gameObject;
	ForwardPass forwardPass;
	PostProcPass postPass;

public:
	TestApp() {};
	virtual ~TestApp() override {};

	void onStep() override;
	void onPlay() override;
	void onInit() override;
	void onTerm() override;

};