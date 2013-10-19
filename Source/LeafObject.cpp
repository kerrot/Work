#include "LeafObject.h"
#include "PMDefine.h"

LeafObject::LeafObject(PMVector a_position)
:
m_oriPosition(a_position)
{

}

LeafObject::LeafObject()
{

}

LeafObject::~LeafObject()
{

}

void LeafObject::AddAcceleration( PMVector a_acc )
{
    m_acceleration += a_acc;
}

void LeafObject::ResetToBegin()
{
    m_velocity.x = 0;
    m_velocity.y = 0;
    m_velocity.z = 0;

    m_acceleration.x = 0;
    m_acceleration.y = 0;
    m_acceleration.z = 0;

    SetPosition(m_oriPosition);
}

void LeafObject::UpdatePosition()
{
    if (!IsVisible())
    {
        return;
    }

    m_velocity += m_acceleration;
    m_acceleration.x = 0;
    m_acceleration.y = 0;
    m_acceleration.z = 0;

    PMVector position = GetPosition() + m_velocity;

    SetPosition(position);

    float v = m_velocity.x * m_velocity.x + m_velocity.z + m_velocity.z;
    float av = v * MAX_ANGULAR_VELOCITY / (MAX_VELOCITY * MAX_VELOCITY) + 0.5;
    PMVector r = GetRotation();
    r.y += av;
    SetRotation(r);
}

void LeafObject::SetOriposition( PMVector a_position )
{
    m_oriPosition = a_position;
    SetPosition(a_position);
}

const PMVector& LeafObject::GetVelocity()
{
    return m_velocity;
}
