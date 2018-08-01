#pragma once
#include "entity.h"


namespace sa3d {


	/**
	@brief Player Class to handle player interaction*/
	class Player : public Entity {

	public:
		Player();

		void update();

	};

	void doMovement(graphics::Window *window, Entity *object);
}