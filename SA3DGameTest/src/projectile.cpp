#include "entity.h"


namespace sa3d {
	Projectile::Projectile()
	{
		this->obj = new graphics::Model("models/ball.obj");
	};

	void Projectile::update()
	{
		transform = glm::translate(transform, glm::vec3(0,0,-1) * DELTA_TIME);
	}

	
}