#pragma once
#include "CollidableObject.h"

class BasketObject : public CollidableObject
{
public:
    virtual void HitObject(LeafObject* a_object);

private:
    friend class GameObjectFactory;
    BasketObject();
    virtual ~BasketObject();
};