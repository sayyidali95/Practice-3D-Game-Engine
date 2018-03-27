#pragma once
#pragma once
#include <vector>


#include <GL/glew.h>
#include "graphics.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace sa3d {

	enum Camera_Movement {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};

	// Default camera values
	const GLfloat YAW = -90.0f;
	const GLfloat PITCH = 0.0f;
	const GLfloat SPEED = 3.0f;
	const GLfloat SENSITIVTY = 0.25f;
	const GLfloat ZOOM = 45.0f;

	class Camera {

	public:
		/**Camera Atributes*/
		glm::vec3 Position;
		glm::vec3 Right = { 1.0f, 0.0f, 0.0f };
		glm::vec3 Up = { 0.0f, 1.0f, 0.0f };
		glm::vec3 Front = { 0.0f, 0.0f, 1.0f };
		glm::vec3 WorldUp = { 0.0f, 1.0f, 0.0f };
		// Eular Angles
		GLfloat Yaw;
		GLfloat Pitch;
		// Camera options
		GLfloat MovementSpeed;
		GLfloat MouseSensitivity;
		GLfloat Zoom;

		/**Constructor */
		Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw = YAW, GLfloat pitch = PITCH);
		Camera(glm::vec3 position);
		/**Returns View Matrix of camera*/
		glm::mat4 getViewMatrix();

		void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime);

		/** Processes input received from a mouse input system. Expects the offset value in both the x and y direction.*/
		void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch);

		/** Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis*/
		void ProcessMouseScroll(GLfloat yoffset);

	private:

		void updateCameraVectors();

	};

	void cameraMovement(graphics::Window *window, Camera cam, GLfloat dt);
}