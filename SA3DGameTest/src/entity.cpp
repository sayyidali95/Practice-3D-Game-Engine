#include "entity.h"

namespace sa3d {


	enum {
		PLAYER,
		ENEMY,
		ALLY,
	};

	typedef struct {
		int maxEntities;
		Entity * entityList;
		int incr;
	}EntityManager;


	static EntityManager entityManager = { 0 };

	char snum[5];

	Entity::Entity()
	{
		int i;
		for (i = 0; i < entityManager.maxEntities; i++)
		{
			if (!entityManager.entityList[i].inuse)
			{
				

				refID = entityManager.incr++;
				inuse = true;
				//entityManager.entityList[i].normalGravity = 0.5f;

				/*itoa(entityManager.entityList[i].refID, snum, 10);
				printf("%s\n", snum);*/
				scale = glm::vec3(1, 1, 1);
				color = glm::vec4(255, 255, 255, 255);
				

			}
		}
		
	}

	

	void entity_manager_close()
	{
		int i;

		if (entityManager.entityList != 0)
		{
			for (i = 0; i < entityManager.maxEntities; i++)
			{
				entityManager.entityList[i].free();
			}
			free(entityManager.entityList);
		}
		memset(&entityManager, 0, sizeof(EntityManager));
		printf("Closed Entity Manager");
	}

	void entityManagerInit(int maxEntities)
	{
		if (!maxEntities)
		{
			printf("already initialized");
			return;
		}
		entityManager.entityList = new Entity[maxEntities];
		if (!entityManager.entityList)
		{
			printf("failed to allocate entity list");
			entity_manager_close();
			return;
		}
	}

	void Entity::draw(graphics::Shader shaderID)
	{

		shaderID.setUniformMat4("model", this->transform);
		(*this->obj).Draw(shaderID);
	}

	void Entity::free()
	{
		if (!this)
		{
			printf("passed a null entity");
			return;
		}
	}

	void Entity::think()
	{
		if (!this)
		{
			printf("Entity Think Failed: Wrong Entity");
			return;
		}
		if (!inuse)
		{
			printf("Entity Think Failed: Entity not in use");
			return;
		}


		if (&Entity::think != NULL)
		{
			think();
		}
	}

	void Entity::touch(Entity * other)
	{
		if (!this || !other->inuse)
		{
			printf("Entity Touch Failed: Wrong Entity");
			return;
		}

		if (!inuse || !other->inuse)
		{
			printf("Entity Touch Failed: Entity not in use");
			return;
		}

		if (&Entity::touch != NULL)
		{
			touch(other);
		}
	}

	void Entity::update()
	{
		if (!this)
		{
			printf("Entity Update Failed: Wrong Entity");
			return;
		}
		if (!inuse)
		{
			printf("Entity Update Failed: Entity not in use");
			return;
		}
		if (dead)
		{
			free();
			return;
		}

		if (&Entity::update != NULL)
		{
			update();
		}
	}




	void entityThinkAll()
	{
		int i;
		
		for (i = 0; i < entityManager.maxEntities; i++)
		{
				if ((entityManager.entityList[i].inuse))
				{
					entityManager.entityList[i].think();
				}

		}
	}
	
	void entityDrawAll(graphics::Shader shaderID)
	{
		int i;
		for (i = 0; i < entityManager.maxEntities; i++)
		{
			if (entityManager.entityList[i].inuse)
			{
				entityManager.entityList[i].draw(shaderID);
			}
		}
	}

	

}