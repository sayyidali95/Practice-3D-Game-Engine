#pragma once
#include "entity.h"

namespace sa3d{

class EntityManager
{
public:
	EntityManager();
	~EntityManager();

	void Add(std::string, Entity* entity);  /**Add Entity to Manager list */
	void Remove(std::string name);			/**Remove Entity from Manager list */
	int GetEntityCount() const;				/**Get Entity count in list*/
	Entity* Get(std::string name) const;	/**Get Entity reference*/

	void DrawAll(graphics::Shader shaderID);

private:
	std::map<std::string, Entity*> entityList;

	struct EntityDeacllocator				/**Purge Entity List*/
	{
		void operator()(const std::pair<std::string, Entity*> & p) const
		{
			delete p.second;
		}
	};


};


}