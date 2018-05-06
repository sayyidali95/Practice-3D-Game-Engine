#include <glad/glad.h>
#include <iostream>
#include <stb_image.h>

#include <GLFW/glfw3.h>

#include "graphics.h"
#include "shader.h"
#include "model.h"
#include "camera.h";
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>


using namespace sa3d;
using namespace graphics;
using namespace glm;



GLFWwindow *w;
// Camera Properties
// Window dimensions
const GLuint screenWidth = 960, screenHeight = 540;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main()
{
	
	Window window(w, "SA Game Test", screenWidth, screenHeight);
	Window* pWindow = &window;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	

	glm::mat4 projection = glm::perspective(camera.Zoom, (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
	glm::mat4 view = camera.getViewMatrix();

	glEnable(GL_DEPTH_TEST);
	//Load shaders
	Shader playerShader("shaders/playerModel.vs", "shaders/playerModel.fs");
	Shader ourShader("shaders/coordinates.vs", "shaders/coordinates.fs");
	Shader lightingShader("shaders/basic_lighting.vs", "shaders/basic_lighting.fs");
	Shader lampShader("shaders/lamp.vs", "shaders/lamp.fs");
	//render model
	Model player("models/nanosuit.obj");
	
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};
	// first, configure the cube's VAO (and VBO)
	unsigned int VBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(cubeVAO);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	// second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// note that we update the lamp's position attribute's stride to reflect the updated buffer data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);



	while (!window.closed())
	{
		// input 
		// -----

		/**Set frame Time*/
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		window.clear();

		glm::mat4 projection = glm::perspective(camera.Zoom, (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
		glm::mat4 view = camera.getViewMatrix();
		glm::mat4 model;
		

		// be sure to activate shader when setting uniforms/drawing objects
		//lightingShader.enable();
		//lightingShader.setUniform3f("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
		//lightingShader.setUniform3f("lightColor", glm::vec3(1.0f, 0.3f, 0.0f));
		//lightingShader.setUniform3f("lightPos", lightPos);
		//lightingShader.setUniform3f("viewPos", camera.Position);


		//lightingShader.setUniformMat4("projection", projection);
		//lightingShader.setUniformMat4("view", view);

		//// world transformation
		//lightingShader.setUniformMat4("model", model);

		//// render the cube
		//glBindVertexArray(cubeVAO);
		//glDrawArrays(GL_TRIANGLES, 0, 36);


		//// also draw the lamp object
		//lampShader.enable();
		//lampShader.setUniformMat4("projection", projection);
		//lampShader.setUniformMat4("view", view);
		//model = glm::mat4();
		//model = glm::translate(model, lightPos);
		//model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
		//lampShader.setUniformMat4("model", model);

		//glBindVertexArray(lightVAO);
		//glDrawArrays(GL_TRIANGLES, 0, 36);


		//load player model
		playerShader.enable();
		playerShader.setUniformMat4("projection", projection);
		playerShader.setUniformMat4("view", view);

		
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
		playerShader.setUniformMat4("model", model);
		player.Draw(playerShader);
		//playerShader.disable();
		
		//camera movement
		// Camera controls
		if (pWindow->m_Keys[GLFW_KEY_UP])
			camera.ProcessKeyboard(FORWARD, deltaTime);
		if (pWindow->m_Keys[GLFW_KEY_DOWN])
			camera.ProcessKeyboard(BACKWARD, deltaTime);
		if (pWindow->m_Keys[GLFW_KEY_LEFT])
			camera.ProcessKeyboard(LEFT, deltaTime);
		if (pWindow->m_Keys[GLFW_KEY_RIGHT])
			camera.ProcessKeyboard(RIGHT, deltaTime);
		if (pWindow->m_Keys[GLFW_KEY_W])
			camera.ProcessKeyboard(UP, deltaTime);
		if (pWindow->m_Keys[GLFW_KEY_S])
			camera.ProcessKeyboard(DOWN, deltaTime);


		//cameraMovement(pWindow, camera, deltaTime);

		window.update();
	}
	glClear(GL_COLOR_BUFFER_BIT);
	glfwTerminate();
	return 0;
}



