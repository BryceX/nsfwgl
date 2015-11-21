#pragma once

#include "ForwardPass.h"
#include "PostProcPass.h"
#include "ShadowPass.h"
#include "nsfw.h"
#include "Camera.h"
#include "GameObject.h"
#include "ParticleEmitter.h"


class TestApp : public nsfw::Application
{
	Camera camera;
	
	GameObject gameObject;
	GameObject gameObject1;
	GameObject floor;
	GameObject wall;
	ParticleEmitter PE;
	ForwardPass forwardPass;
	ShadowPass shadowPass;
	PostProcPass postPass;
	
	Light directionLight;
	
public:
	TestApp() {};
	virtual ~TestApp() override {};

	void onStep() override;
	void onPlay() override;
	void onInit() override;
	void onTerm() override;

};