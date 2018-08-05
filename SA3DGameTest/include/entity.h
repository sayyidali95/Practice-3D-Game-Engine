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


#define MAX_ENTITIES 2048

namespace sa3d {
	/**EntitySystem variables*/

	/**
	@brief Entity Class to Manage Game Objects*/
	class Entity
	{
	public:
	
		bool inuse = false;
		int refID;    /**<unique id of this entity*/
		char name[128];

		/** Transform variables */
		glm::mat4 transform = glm::mat4();
		glm::vec3 position;
		glm::vec3 acceleration;
		glm::vec3 rotation;
		glm::vec3 scale;
		glm::vec4 color;
		GLchar* objPath;
		graphics::Model *obj;

		/**States and other Info */
		int state;
		float frame;
		bool dead;
		graphics::Texture *texture;

		/**Entity Constructor*/
		Entity();
		virtual ~Entity();


		virtual void draw(graphics::Shader shaderID);	/** Draws Entity with the given Shader*/
		void free();							/**Free Entity Memory*/
		
		/**Entity Interactions and Behaviors */
		virtual void think();							/**Updates Entity Behavior*/
		virtual void touch(Entity* other);				/**Updates interaction between two Entities*/
		virtual void update();							/** updates Entity State*/


	};
	Entity *entity_new();
	//entity Manager
	void entityManagerInit(int maxEntities);
	void entityManagerClose();

	/** global entity functions that update the static entity manager*/
	void entityDrawAll(graphics::Shader shaderID);
	void entityThinkAll();
	void entityTouchAll();
	void entityUpdateAll();
	void entityClearAll();
	void entityGetCount();								/**Get Count of all entities in the entityManager*/
}

#endif