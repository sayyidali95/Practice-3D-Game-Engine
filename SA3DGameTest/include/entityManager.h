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
	
	void ThinkAll();							/** updates all inputs from each entities*/
	void TouchAll();							/** Updates all interactions between entities*/
	void UpdateAll();							/** updates all entities in manager*/
	void DrawAll(graphics::Shader shaderID);	/** Draw all entities in manager*/
	void ClearAll();							/** clear list of all entities*/
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