#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include <iostream>



#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

namespace sa3d {


	/**
	* @brief initialize the 3d graphics system
	* @param sw the screen width you want
	* @param sh the screen height you want
	* @param fullscreen set true to initialize to fullscreen
	* @param project the name to appear on the game window
	* @param frameDelay the desired delay (in ms) between frames
	* @return -1 on error, 0 otherwise
	*/


namespace graphics {

#define MAX_KEYS		1024
#define MAX_BUTTONS		32





			class Window
			{

			private:
				friend struct GLFWwindow;
				const char *m_Title;
				int m_Width, m_Height;
				bool m_Closed;




				/*Define Keys and buttons*/
			public:
				GLFWwindow *m_Window;
				bool m_Keys[MAX_KEYS];
				bool m_MouseButtons[MAX_BUTTONS];
				double mx, my;





				Window(GLFWwindow *window, const char *name, int width, int height);
				~Window();
				void clear() const;
				void update();
				bool closed() const;

				/*Screen Width*/
				int getWidth() const { return m_Width; }
				int getHeight() const { return m_Height; }

				/*Key is Pressed*/
				bool isKeyPressed(unsigned int keycode) const;
				bool isMouseButtonPressed(unsigned int button) const;
				void getMousePosition(double& x, double& y) const;
				void doMovement();

			private:
				bool init();

				/*Input Callbacks*/
				friend static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
				friend static void mouse_button_callback(GLFWwindow* window, int button, int action, int mode);
				friend static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);



			};


		}
}