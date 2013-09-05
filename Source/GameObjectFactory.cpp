#include "GameObjectFactory.h"
#include <irrlicht.h>

using namespace irr::scene;

void GameObjectFactory::FactoryInit( ISceneManager* a_mgr )
{

}

const std::vector<LeafObject*>& GameObjectFactory::GetLeaves()
{
    return m_leaves;
}

const std::vector<CollidableObject*>& GameObjectFactory::GetCollidables()
{
    return m_collidables;
}
