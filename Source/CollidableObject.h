#pragma once
#include "GameObject.h"

class LeafObject;

class CollidableObject : public GameObject
{
public:
    virtual void HitObject(LeafObject* a_Object) = 0;

protected:
    friend class GameObjectFactory;
    CollidableObject();
    virtual ~CollidableObject();
};