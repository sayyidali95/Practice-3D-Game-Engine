#include "entity.h"
#include "entityManager.h"

namespace sa3d {


	enum {
		PLAYER,
		ENEMY,
		ALLY,
	};

	//typedef struct {
	//	int maxEntities;
	//	std::vector<Entity> entityList;
	//	int incr;
	//}EntityManager;


	static EntityManager* entityManager = new EntityManager();
	
	char snum[5];

	Entity::Entity()
	{
		
		inuse = true;
		
		scale = glm::vec3(1, 1, 1);
		color = glm::vec4(255, 255, 255, 255);
		transform = glm::mat4();
		refID = entityManager->entityID++;
		entityManager->Add(this);
		
	}


	Entity::~Entity()
	{

	}

	//Entity *entity_new()
	//{
	//	int i;
	//	for (i = 0; i < entityManager.maxEntities; i++)
	//	{  
	//		if (!entityManager.entityList[i].inuse)
	//		{
	//			
	//			memset(&entityManager.entityList[i], 0, sizeof(Entity));
	//			entityManager.entityList[i].refID = entityManager.incr++;
	//			entityManager.entityList[i].inuse = 1;
	//			//entityManager.entityList[i].normalGravity = 0.5f;

	//			//itoa(entityManager.entityList[i].refID, snum, 10);
	//			//printf("%s\n", snum);
	//			entityManager.entityList[i].scale = glm::vec3(1, 1, 1);
	//			entityManager.entityList[i].color = glm::vec4(255, 255, 255, 255);
	//			entityManager.entityList[i].transform = glm::mat4();
	//			return &entityManager.entityList[i];

	//		}
	//	}
	//	return NULL;
	//}
	

	//void entity_manager_close()
	//{
	//	int i;
	//	entityManager.entityList.clear();

	//	memset(&entityManager, 0, sizeof(EntityManager));
	//	printf("Closed Entity Manager");
	//}

	//void entityManagerInit(int maxEntities)
	//{
	//	if (!maxEntities)
	//	{
	//		printf("already initialized");
	//		return;
	//	}
	//	entityManager.entityList.resize(maxEntities);
	//	if (entityManager.entityList.empty())
	//	{
	//		printf("failed to allocate entity list");
	//		entity_manager_close();
	//		return;
	//	}
	//}

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


	}




	void entityThinkAll()
	{
		entityManager->ThinkAll();

	}

	void entityUpdateAll()
	{
		entityManager->UpdateAll();
	}
	
	void entityDrawAll(graphics::Shader shaderID)
	{
		entityManager->DrawAll(shaderID);
	}

	void entityGetCount() 
	{
		cout << entityManager->GetEntityCount() << endl;
	}

}