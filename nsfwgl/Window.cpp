#include "gl_core_4_4.h"
#include "nsfw.h"
#include <glfw3.h>


void nsfw::Window::init(unsigned width, unsigned height)
{
	this->width = width;
	this->height = height;

	glfwInit();
	window = glfwCreateWindow(width, height, "myOpenGL", nullptr,nullptr);
	glfwMakeContextCurrent(window);
	if (window == nullptr) 
	{
		glfwTerminate();
		std::cout << "notworking" << std::endl;
	}
	
	glfwSetTime(0.0);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(window);
		glfwTerminate();
		return;
	}
}

void nsfw::Window::step()
{
	deltaTime = glfwGetTime() - timePassed;
	timePassed = glfwGetTime();

	glfwSwapBuffers(window);
	glfwPollEvents();	
}

void nsfw::Window::term()
{
	
	glfwTerminate();
	glfwDestroyWindow(window);
	//TODO();
}

bool nsfw::Window::getKey(GLFWwindow* window, unsigned int k) 
{
	if (glfwGetKey(window, k) != GLFW_PRESS)
	{
		return false;
	}

	else
	return true;
	
}

bool nsfw::Window::getShouldClose() 
{
	if (getKey(window ,GLFW_KEY_ESCAPE))
	{
		return true;
	}
	else
	return false;
}

unsigned nsfw::Window::getWidth() const
{
	return width;
}

unsigned nsfw::Window::getHeight() const
{
	return height;
}

glm::mat4 nsfw::Window::getTexelAdjustmentMatrix() const
{
	TODO_D("Implemented, not tested.");

	glm::vec3 texelSize = 1.0f/glm::vec3(width,height,0);
	glm::vec3 halfTexel = texelSize * 0.5f; // bottom left

	// scale up to the appropriate size, then shift it to line up w/center of texels
	return glm::translate(halfTexel) * glm::scale(glm::vec3(1, 1, 1) - texelSize);	
}