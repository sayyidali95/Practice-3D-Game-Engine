#include "entityManager.h"

namespace sa3d {
	EntityManager::EntityManager()
	{

	}

	EntityManager::~EntityManager()
	{
		entityList.clear();
	}

	void EntityManager::Add(Entity* entity)
	{
		//unsigned int  i;
		//for (i = 0; i < entityList.size(); i++)
		//{
		//	if (entityList[i]->inuse == false)
		//	{
	
				entityList.push_back(entity);
				
		//	}
		//}
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

	void EntityManager::DrawAll(graphics::Shader shaderID)
	{
		for(unsigned int i=0; i<entityList.size();i++)
			entityList[i]->draw(shaderID);
	}
}