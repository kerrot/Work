#pragma once
#include "CollidableObject.h"

class ResistanceObject : public CollidableObject
{
public:
    virtual void HitObject(LeafObject* a_object);

private:
    friend class GameObjectFactory;
    ResistanceObject();
    virtual ~ResistanceObject();

    float m_arg;
};