#include "GameObject.h"
#include "PMType.h"
#include <irrlicht.h>
#include <assert.h>

using namespace irr;
using namespace irr::scene;
using namespace irr::core;
using namespace irr::video;

namespace
{
    ITexture* s_textures[MAX_TEXTURE];
}

GameObject::GameObject()
:
m_node(0)
,m_textureID(TEXTURE_NONE)
,m_renderTarget(0)
{
    
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

void GameObject::SetPosition( PMVector a_position )
{
    if (m_node)
    {
        m_node->setPosition(vector3df(a_position.x, a_position.y, a_position.z));
    }
}

void GameObject::SetRotation( float a_x, float a_y, float a_z )
{
    if (m_node)
    {
        m_node->setRotation(vector3df(a_x, a_y, a_z));
    }
}

void GameObject::SetRotation( PMVector a_rotation )
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

PMVector GameObject::GetPosition()
{
    vector3df position;

    if (m_node)
    {
        position = m_node->getPosition();
    }

    return PMVector(position.X, position.Y, position.Z);
}

void GameObject::SetScale( PMVector a_scale )
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

PMVector GameObject::GetRotation()
{
    vector3df rotation;

    if (m_node)
    {
        rotation = m_node->getRotation();
    }

    return PMVector(rotation.X, rotation.Y, rotation.Z);
}

PMVector GameObject::GetAbsolutePosition()
{
    vector3df position;

    if (m_node)
    {
        position = m_node->getAbsolutePosition();
    }

    return PMVector(position.X, position.Y, position.Z);
}

void GameObject::RotationToDirection( PMVector &a_vector, PMVector a_rotation)
{
    vector3df rotation(a_rotation.x, a_rotation.y, a_rotation.z);
    vector3df v = rotation.rotationToDirection(vector3df(a_vector.x, a_vector.y, a_vector.z));

    a_vector.x = v.X;
    a_vector.y = v.Y;
    a_vector.z = v.Z;
}

PMVector GameObject::GetAbsoluteRotation()
{
    vector3df rotation;

    if (m_node)
    {
        const core::matrix4& matrix = m_node->getAbsoluteTransformation();

        rotation = matrix.getRotationDegrees();
    }

    return PMVector(rotation.X, rotation.Y, rotation.Z);
}

void GameObject::SetColor( char a_r, char a_g, char a_b, char a_a )
{
    if (m_node)
    {
        SMaterial &m = m_node->getMaterial(0);
        m.EmissiveColor = SColor(a_a, a_r, a_g, a_b);
    }
}

void GameObject::SetTexture(GameObjectTexture a_index, irr::video::ITexture* a_texture)
{
    s_textures[a_index] = a_texture;
}

irr::video::ITexture* GameObject::GetTexture( GameObjectTexture a_index )
{
    return s_textures[a_index];
}

void GameObject::ChangeTexture( GameObjectTexture a_index )
{
    if (m_node)
    {
        m_textureID = a_index;
        m_node->setMaterialTexture(0, s_textures[a_index]);
    }
}

void GameObject::RecoverTexture()
{
    if (m_node)
    {
        m_node->setMaterialTexture(0, s_textures[m_textureID]);
    }
}

PMVector GameObject::GetScale()
{
    vector3df scale(1, 1, 1);

    if (m_node)
    {
        scale = m_node->getScale();
    }

    return PMVector(scale.X, scale.Y, scale.Z);
}

bool GameObject::IsVisible()
{
    if (m_node)
    {
        return m_node->isTrulyVisible();
    }

    return false;
}

void GameObject::SetParent( GameObject* a_object )
{
    if (a_object && a_object->m_node && m_node)
    {
        return m_node->setParent(a_object->m_node);
    }
}