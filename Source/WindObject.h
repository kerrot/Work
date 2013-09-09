#pragma once
#include "CollidableObject.h"
#include "WindowInterface.h"
#include "PMType.h"

class WindObject : public CollidableObject, public WindowInterface
{
public:
    virtual void HitObject(LeafObject* a_object);

private:
    friend class GameObjectFactory;
    WindObject();
    virtual ~WindObject();
};