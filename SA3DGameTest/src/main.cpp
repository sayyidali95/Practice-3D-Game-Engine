#include <GL/glew.h>
#include <iostream>


#include <GLFW/glfw3.h>

#include "graphics.h"
#include "camera.h";
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

using namespace sa3d;
using namespace graphics;
using namespace glm;


GLFWwindow *w;
// Camera Properties
// Window dimensions
const GLuint screenWidth = 960, screenHeight = 540;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

int main()
{
	
	Window window(w, "SA Game Test", screenWidth, screenHeight);
	Window* pWindow = &window;
	glClearColor(1.0f, 0.3f, 0.0f, 1.0f);
	

	glm::mat4 projection = glm::perspective(camera.Zoom, (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
	glm::mat4 view = camera.getViewMatrix();


	while (!window.closed())
	{
		// input 
		// -----
		window.clear();
		window.update();
	}
	glClear(GL_COLOR_BUFFER_BIT);
	glfwTerminate();
	return 0;
}



