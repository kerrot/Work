#include "HandObject.h"
#include "PMType.h"

HandObject::HandObject(UInt32 a_id)
:
m_id(a_id)
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