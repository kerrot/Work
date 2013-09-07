#pragma once
#include "GameObject.h"

class LeafObject : public GameObject
{
private:
    friend class GameObjectFactory;
    LeafObject(irr::scene::ISceneNode* a_node);
    virtual ~LeafObject();

    float m_velocity;
    float m_acceleration;
};