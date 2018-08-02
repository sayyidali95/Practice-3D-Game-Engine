#include <glad/glad.h>
#include <iostream>
#include <stb_image.h>

#include <GLFW/glfw3.h>

#include "graphics.h"
#include "shader.h"
#include "model.h"
#include "camera.h"
#include "entityManager.h"
#include "player.h"
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
	glClearColor(.8f, 0.8f, 0.8f, 1.0f);
	

	//Initialize resource managers



	//initialize player
	Entity* player = new Player();
	Entity* player2 = new Entity();
	
	player->transform = glm::translate(player->transform, glm::vec3(0.0f, -3.0f, -3.0f)); // Translate it down a bit so it's at the center of the scene
	player->transform = glm::scale(player->transform, glm::vec3(0.4f, 0.4f, 0.4f));	// It's a bit too big for our scene, so scale it down
	player2->transform = glm::scale(player->transform, glm::vec3(0.4f, 0.4f, 0.4f));	// It's a bit too big for our scene, so scale it down
	glm::mat4 projection = glm::perspective(camera.Zoom, (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
	glm::mat4 view = camera.getViewMatrix();

	glEnable(GL_DEPTH_TEST);
	//Load shaders
	Shader playerShader("shaders/playerModel.vs", "shaders/playerModel.fs");
	Shader ourShader("shaders/coordinates.vs", "shaders/coordinates.fs");
	Shader lightingShader("shaders/basic_lighting.vs", "shaders/basic_lighting.fs");
	Shader lampShader("shaders/lamp.vs", "shaders/lamp.fs");
	//render model
	player2->obj = new Model("models/nanosuit.obj");
	
	


	/**@brief Game Loop **/
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
		playerShader.enable();
		playerShader.setUniform3f("objectColor", glm::vec3(1.0f, 1.0f, 1.0f));
		playerShader.setUniform3f("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
		playerShader.setUniform3f("lightPos", lightPos);
		
		playerShader.setUniformMat4("projection", projection);
		playerShader.setUniformMat4("view", view);

		// world transformation
		playerShader.setUniformMat4("model", model);


		//load player model
		//playerShader.enable();
		playerShader.setUniformMat4("projection", projection);
		playerShader.setUniformMat4("view", view);

		
		model = glm::translate(model , glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
		playerShader.setUniformMat4("model", model);

		doMovement(pWindow, player);
		lightPos = camera.Position;
		entityThinkAll();
		entityTouchAll();
		entityUpdateAll();
		entityDrawAll(playerShader);
		
		entityGetCount();
		//cout << player->refID << endl;
		//cout << player2->refID << endl;
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



