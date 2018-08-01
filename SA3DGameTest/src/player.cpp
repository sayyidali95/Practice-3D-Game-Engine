#include "player.h"


namespace sa3d {

	Player::Player()
	{
		this->obj = new graphics::Model("models/BossModel.obj");
	};

	void Player::update() {
		printf("player is alive");
	}

	void doMovement(graphics::Window *window, Entity *object)
	{
		if (window->m_Keys[GLFW_KEY_W])
			object->transform = glm::translate(object->transform, glm::vec3(0.0f, 0.0f, -1.0f));
		if (window->m_Keys[GLFW_KEY_S])
			object->transform = glm::translate(object->transform, glm::vec3(0.0f, 0.0f, 1.0f));
		if (window->m_Keys[GLFW_KEY_A])
			object->transform = glm::translate(object->transform, glm::vec3(-1.0f, 0.0f, 0.0f));
		if (window->m_Keys[GLFW_KEY_D])
			object->transform = glm::translate(object->transform, glm::vec3(1.0f, 0.0f, 0.0f));
		if (window->m_Keys[GLFW_KEY_Q])
			object->transform = glm::rotate(object->transform, -0.05f, glm::vec3(0.0f, 1.0f, 0.0f));
		if (window->m_Keys[GLFW_KEY_E])
			object->transform = glm::rotate(object->transform, 0.05f, glm::vec3(0.0f, 1.0f, 0.0f));
		if (window->m_Keys[GLFW_KEY_SPACE])
			object->transform = glm::translate(object->transform, glm::vec3(0.0f, 1.0f, 0.0f));
		if (window->m_Keys[GLFW_KEY_LEFT_SHIFT])
			object->transform = glm::translate(object->transform, glm::vec3(0.0f, -1.0f, 0.0f));

	}
}