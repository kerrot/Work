#pragma once
#include "GameObject.h"

class LeafObject;

class CollidableObject : public GameObject
{
public:
    virtual void HitObject(LeafObject* a_Object) = 0;

private:
    friend class GameObjectFactory;
    CollidableObject(irr::scene::ISceneNode* a_node);
    virtual ~CollidableObject();
};