#include "ResistanceObject.h"
#include "LeafObject.h"
#include "PMDefine.h"

ResistanceObject::ResistanceObject()
:
m_arg(GRAVITY_ACCELERATION / (MAX_VELOCITY * MAX_VELOCITY))
{

}

ResistanceObject::~ResistanceObject()
{

}

void ResistanceObject::HitObject(LeafObject* a_object )
{
    const PMVector& velocity = a_object->GetVelocity();

    float ax = (velocity.x > 0) ? -1 : 1;
    float ay = (velocity.y > 0) ? -1 : 1;
    float az = (velocity.z > 0) ? -1 : 1;

    ax *= m_arg * velocity.x * velocity.x;
    ay *= m_arg * velocity.y * velocity.y;
    az *= m_arg * velocity.z * velocity.z;

    a_object->AddAcceleration(PMVector(ax, ay, az));
}
