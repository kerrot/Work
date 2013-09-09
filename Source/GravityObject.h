#pragma once
#include "CollidableObject.h"
#include "PMType.h"

class GravityObject : public CollidableObject
{
public:
    virtual void HitObject(LeafObject* a_object);

private:
    friend class GameObjectFactory;
    GravityObject();
    virtual ~GravityObject();

    PMVector m_gravity;
};