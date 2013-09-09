#include "GameObject.h"
#include "PMType.h"
#include <irrlicht.h>
#include <assert.h>


using namespace irr;
using namespace scene;
using namespace core;

GameObject::GameObject()
:
m_node(0)
{
    
}

GameObject::~GameObject()
{

}

void GameObject::SetVisible( bool a_result ) const
{
    if (m_node)
    {
        m_node->setVisible(a_result);
    }
}

void GameObject::SetPosition( float a_x, float a_y, float a_z ) const
{
    if (m_node)
    {
        m_node->setPosition(vector3df(a_x, a_y, a_z));
    }
}

void GameObject::SetPosition( PMVector a_position ) const
{
    if (m_node)
    {
        m_node->setPosition(vector3df(a_position.x, a_position.y, a_position.z));
    }
}

void GameObject::SetRotation( float a_x, float a_y, float a_z ) const
{
    if (m_node)
    {
        m_node->setRotation(vector3df(a_x, a_y, a_z));
    }
}

void GameObject::SetRotation( PMVector a_rotation ) const
{
    if (m_node)
    {
        m_node->setRotation(vector3df(a_rotation.x, a_rotation.y, a_rotation.z));
    }
}

void GameObject::SetNode( irr::scene::ISceneNode* a_node )
{
    m_node = a_node;
    assert(m_node);
}

PMVector GameObject::GetPosition() const
{
    vector3df position;

    if (m_node)
    {
        position = m_node->getPosition();
    }

    return PMVector(position.X, position.Y, position.Z);
}

void GameObject::SetScale( PMVector a_scale ) const
{
    if (m_node)
    {
        const vector3df &scale = m_node->getScale();

        if (a_scale.x == 0)
        {
            a_scale.x = scale.X;
        }

        if (a_scale.y == 0)
        {
            a_scale.y = scale.Y;
        }

        if (a_scale.z == 0)
        {
            a_scale.z = scale.Z;
        }

        m_node->setScale(vector3df(a_scale.x, a_scale.y, a_scale.z));
    }
}

