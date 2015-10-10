#pragma once
/****************
GLFW Wrapper with BASIC interface.

Extend functionality as you see fit.

This class should ONLY use GLFW!

No OpenGL!!!
*****************/

#include <glm/glm.hpp>

struct GLFWwindow;

namespace nsfw
{
	class Window
	{
		GLFWwindow *window;
		
		unsigned width, height;		// keep track of width height- needed for glViewport FBO integration

		Window():window(nullptr),width(0),height(0) {}
	public:
		static Window &instance() { static Window w; return w; }
		
		//create the context!
		void init(unsigned width, unsigned height);
		
		// swapBuffers and pollEvents!
		void step();

		// kill the context!
		void term();
		float deltaTime = 0;
		float timePassed = 0;
		// just wrap what GLFW already does
		void	  SetTime();
		bool	  getKey(GLFWwindow* window, unsigned int k);
		bool	  getShouldClose()           ;
		unsigned  getWidth()                 const;
		unsigned  getHeight()                const;
		int key = 0;
		int action = 0;
		glm::mat4 getTexelAdjustmentMatrix() const;
	};
}