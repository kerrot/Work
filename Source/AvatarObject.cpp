#include "AvatarObject.h"
#include <irrlicht.h>
#include "PMCommon.h"

using namespace irr;
using namespace scene;
using namespace core;

AvatarObject::AvatarObject()
:
m_speed(1)
,m_fornt(0, 0, 1)
,m_right(1, 0, 0)
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

    m_head.SetTarget(m_target.GetAbsolutePosition());
}

void AvatarObject::SetRotation( PMVector a_rotation )
{
    GameObject::SetRotation(a_rotation);

    m_fornt.x = sin(a_rotation.y / 180 * PM_PI);
    m_fornt.z = cos(a_rotation.y / 180 * PM_PI);

    m_right.x = cos(a_rotation.y / 180 * PM_PI);
    m_right.z = -sin(a_rotation.y / 180 * PM_PI);

    m_head.SetTarget(m_target.GetAbsolutePosition());
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
