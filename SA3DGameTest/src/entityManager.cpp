#include "entityManager.h"

namespace sa3d {
	EntityManager::EntityManager()
	{
		//entityList.resize(maxEntities);
	}

	EntityManager::~EntityManager()
	{
		entityList.clear();
	}

	void EntityManager::Add(Entity* entity)
	{
		
		if (entityList.empty())
			entity->refID = 0;
			
			entityList.push_back(entity);

	}

	void EntityManager::Remove(Entity* entity)
	{

		entityList.erase(entityList.begin() +entity->refID);
		
	}

	Entity* EntityManager::Get(int refId) const
	{
		for (unsigned i = 0; i < entityList.size();i++)
			if (entityList[i]->refID == refId)
			{
				return entityList[i];
			}
		
	}

	int EntityManager::GetEntityCount() const
	{
		return entityList.size();
	}

	void EntityManager::ThinkAll()
	{
		for (unsigned int i = 0; i < entityList.size(); i++)
			entityList[i]->think();
	}
	
	void EntityManager::UpdateAll()
	{
		for (unsigned int i = 0; i < entityList.size(); i++)
		{
			if (entityList[i]->inuse == 0)
				continue;
			entityList[i]->update();
		}
	}

	void EntityManager::TouchAll()
	{
		for (unsigned int i = 0; i < entityList.size(); i++)
		{

		}
	}

	void EntityManager::DrawAll(graphics::Shader shaderID)
	{
		for(unsigned int i=0; i < entityList.size(); i++)
			entityList[i]->draw(shaderID);
	}
}