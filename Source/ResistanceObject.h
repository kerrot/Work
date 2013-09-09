#pragma once
#include "CollidableObject.h"
#include "PMType.h"

class ResistanceObject : public CollidableObject
{
public:
    virtual void HitObject(LeafObject* a_object);

private:
    friend class GameObjectFactory;
    ResistanceObject();
    virtual ~ResistanceObject();
};