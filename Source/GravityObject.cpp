#include "GravityObject.h"
#include "LeafObject.h"

GravityObject::GravityObject()
:
m_gravity(0, -0.001f, 0)
{

}

GravityObject::~GravityObject()
{

}

void GravityObject::HitObject(LeafObject* a_object )
{
    a_object->AddAcceleration(m_gravity);
}
