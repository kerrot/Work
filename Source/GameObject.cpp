#include "GameObject.h"
#include <irrlicht.h>

using namespace irr;
using namespace scene;
using namespace core;

GameObject::GameObject( ISceneNode* a_node )
{
    m_node = a_node;
}

GameObject::~GameObject()
{

}

void GameObject::SetVisible( bool a_result )
{
    if (m_node)
    {
        m_node->setVisible(a_result);
    }
}

void GameObject::SetPosition( float a_x, float a_y, float a_z )
{
    if (m_node)
    {
        m_node->setPosition(vector3df(a_x, a_y, a_z));
    }
}

void GameObject::SetRotation( float a_x, float a_y, float a_z )
{
    if (m_node)
    {
        m_node->setRotation(vector3df(a_x, a_y, a_z));
    }
}

