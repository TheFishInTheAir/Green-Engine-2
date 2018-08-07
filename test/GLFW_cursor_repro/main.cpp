#include "../ExampleController.h"
#ifdef ENABLE_GLFW_CURSOR_REPRO

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>


GLFWwindow* window;


void threadRun()
{
	while (!glfwWindowShouldClose(window))
	{
		static bool toggleRecent = false;
		static bool toggleState = 0;
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			if (toggleState)
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			else
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

			if (!toggleRecent)
			{
				toggleRecent = true;
				printf("Test");


				toggleState = !toggleState;

			}
		}
		else
			toggleRecent = false;

	}
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(800, 600, "Test", NULL, NULL);

	glfwMakeContextCurrent(window); 

	glewExperimental = (GLboolean)true; 
	if (glewInit() != GLEW_OK) {
		printf("glew init err");
	}

	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	while (!glfwWindowShouldClose(window))
	{

		glfwPollEvents();
	}
}

#endif