#pragma once
#ifndef __ENTITY_H__
#define __ENTITY_H__

#include <iostream>
#include <vector>
#include <list>

#include "camera.h"
#include "model.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#define MAX_ENTITIES 1024

namespace sa3d {
	/**EntitySystem variables*/

	/**
	@brief Entity Class to Manage Game Objects*/
	class Entity
	{
	public:
		bool inuse = false;
		int uid;    /**<unique id of this entity*/
		char name[128];
		glm::mat4 transform = glm::mat4();

		glm::vec3 position;
		glm::vec3 acceleration;
		glm::vec3 rotation;
		glm::vec3 scale;
		glm::vec4 color;
		GLchar* objPath;
		graphics::Model *obj;
		int state;
		float frame;
		graphics::Texture *texture;

		/**Entity Constructor*/
		Entity();

		virtual void draw(graphics::Shader shaderID);
		//virtual void update();


	};

	//global entity functions
	void entity_draw_all(Camera camera);
	void entity_think_all();
	void entity_clear_all();
}

#endif