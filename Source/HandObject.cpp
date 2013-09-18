#include "HandObject.h"
#include "WindowInterface.h"

#define WINDOW_HAND_DISTANCE 20

HandObject::HandObject(UInt32 a_id)
:
m_id(a_id)
,m_window(0)
,m_down(0, -1, 0)
{

}

HandObject::~HandObject()
{

}

void HandObject::SetFingerPosition( int a_index, PMVector a_position )
{
    if (a_index >= 0 && a_index < MAX_FINGERS)
    {
        m_fingles[a_index].SetPosition(a_position);
    }
}

void HandObject::SetVisible( bool a_result )
{
    GameObject::SetVisible(a_result);

    for (int i = 0; i < MAX_FINGERS; ++i)
    {
        m_fingles[i].SetVisible(a_result);
    }
}

void HandObject::HideFingler( int a_index )
{
    if (a_index >= 0 && a_index < MAX_FINGERS)
    {
        m_fingles[a_index].SetVisible(false);
    }
}

WindowInterface* HandObject::GetAttachedWindow()
{
    return m_window;
}

void HandObject::AttachWindow( WindowInterface* a_window )
{
    m_window = a_window;

    SetPosition(GetPosition());
    SetRotation(GetRotation());
}

void HandObject::SetPosition( PMVector a_position )
{
    GameObject::SetPosition(a_position);
    if (m_window)
    {
        PMVector position = GetAbsolutePosition() + m_down * WINDOW_HAND_DISTANCE;

        m_window->GetObject()->SetPosition(position);
    }
}

void HandObject::SetRotation( PMVector a_rotation )
{
    GameObject::SetRotation(a_rotation);

    m_down.x = 0;
    m_down.y = -1;
    m_down.z = 0;

    PMVector absoluteRotation = GetAbsoluteRotation();

    GameObject::RotationToDirection(m_down, absoluteRotation);

    if (m_window)
    {
        m_window->GetObject()->SetRotation(absoluteRotation);

        PMVector normal = -1 * m_down;

        PMVector planeVectorX(1, 0, 0);
        GameObject::RotationToDirection(planeVectorX, absoluteRotation);

        PMVector planeVectorY(0, 0, 1);
        GameObject::RotationToDirection(planeVectorY, absoluteRotation);

        m_window->SetNormalDirection(normal, planeVectorX, planeVectorY);
    }
}

// void HandObject::SetNormalDirection( PMVector a_normal, PMVector a_direction )
// {
//     PMVector tmp = a_normal.CrossProduct(a_direction);
//     
//     float tanX = a_normal.z / a_direction.z;
// 
//     float degreeX = atan(tanX);
//     float degreeY = asin(-tmp.z);
//     float degreeZ = asin(tmp.y / cos(degreeY));
// 
//     SetRotation(PMVector(degreeX / PM_PI * 180, degreeY / PM_PI * 180, degreeZ / PM_PI * 180));
// }