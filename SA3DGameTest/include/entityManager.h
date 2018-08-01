#pragma once
#include <algorithm>
#include "entity.h"

namespace sa3d{

class EntityManager
{
public:
	EntityManager();
	~EntityManager();

	int maxEntities = MAX_ENTITIES;

	void Add(Entity* entity);  /**Add Entity to Manager list */
	void Remove(Entity* entity);			/**Remove Entity from Manager list */
	int GetEntityCount() const;				/**Get Entity count in list*/
	Entity* Get(int refId) const;	/**Get Entity reference*/
	
	void DrawAll(graphics::Shader shaderID);
	int entityID;

private:
	std::vector<Entity*> entityList;

	struct EntityDeallocator				/**Purge Entity List*/
	{
		void operator()(const std::pair<std::string, Entity*> & p) const
		{
			delete p.second;
		}
	};


};


}