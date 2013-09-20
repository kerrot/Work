#include "AvatarObject.h"
#include <irrlicht.h>

#define _USE_MATH_DEFINES 
#include <math.h>

using namespace irr;
using namespace scene;
using namespace core;

AvatarObject::AvatarObject()
:
m_speed(1)
,m_fornt(0, 0, 1)
,m_right(1, 0, 0)
,m_lookRotation(0, 0, 0)
{

}

AvatarObject::~AvatarObject()
{

}

void AvatarObject::Forward()
{
    SetPosition(GetPosition() + m_fornt * m_speed);
}

void AvatarObject::Backward()
{
    SetPosition(GetPosition() - m_fornt * m_speed);
}

void AvatarObject::Leftward()
{
    SetPosition(GetPosition() - m_right * m_speed);
}

void AvatarObject::Rightward()
{
    SetPosition(GetPosition() + m_right * m_speed);
}

void AvatarObject::Upward()
{
    SetPosition(GetPosition() + PMVector(0, m_speed, 0));
}

void AvatarObject::Downward()
{
    SetPosition(GetPosition() - PMVector(0, m_speed, 0));
}

void AvatarObject::SetPosition( PMVector a_position )
{
    GameObject::SetPosition(a_position);

    UpdateHeadLookAt();
}

void AvatarObject::LookRotate(PMVector a_rotation)
{
    m_lookRotation += a_rotation;

    if (m_lookRotation.x > 0)
    {
        if (m_lookRotation.x > 85)
        {
            m_lookRotation.x = 85;
        }
    }
    else
    {
        if (m_lookRotation.x < -85)
        {
            m_lookRotation.x = -85;
        }
    }

    UpdateHeadLookAt();
}

void AvatarObject::SetRotation( PMVector a_rotation )
{
    GameObject::SetRotation(a_rotation);

    m_fornt.x = sin(a_rotation.y / 180 * M_PI);
    m_fornt.z = cos(a_rotation.y / 180 * M_PI);

    m_right.x = cos(a_rotation.y / 180 * M_PI);
    m_right.z = -sin(a_rotation.y / 180 * M_PI);

   UpdateHeadLookAt();
}

void AvatarObject::SetHeadPosition( PMVector a_position )
{
    m_head.SetPosition(a_position);
}

void AvatarObject::TurnLeft()
{
    SetRotation(GetRotation() - PMVector(0, m_speed, 0));
}

void AvatarObject::TurnRight()
{
    SetRotation(GetRotation() + PMVector(0, m_speed, 0));
}

void AvatarObject::UpdateHeadLookAt()
{
    PMVector lookVector = m_target.GetAbsolutePosition() - m_head.GetAbsolutePosition();
    GameObject::RotationToDirection(lookVector, m_lookRotation);

    PMVector v = m_head.GetAbsolutePosition();
    PMVector s = m_target.GetAbsolutePosition();

    m_head.SetTarget(m_head.GetAbsolutePosition() + lookVector);
}
