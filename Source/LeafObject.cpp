#include "LeafObject.h"

LeafObject::LeafObject( irr::scene::ISceneNode* a_node )
:
GameObject(a_node)
,m_velocity(0)
,m_acceleration(0)
{

}

LeafObject::~LeafObject()
{

}
