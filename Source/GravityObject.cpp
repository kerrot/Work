#include "GravityObject.h"
#include "LeafObject.h"
#include "PMDefine.h"

GravityObject::GravityObject()
:
m_gravity(0, - GRAVITY_ACCELERATION, 0)
{

}

GravityObject::~GravityObject()
{

}

void GravityObject::HitObject(LeafObject* a_object )
{
    a_object->AddAcceleration(m_gravity);
}
