#include "WindowInterface.h"
#include "GameObject.h"
#include "PMDefine.h"
#include <assert.h>

UInt32 WindowInterface::m_idNow = 0;

WindowInterface::WindowInterface( GameObject* a_object )
:
m_object(a_object)
,m_normal(0, 0, 1)
,m_planeVectorX(1, 0, 0)
,m_planeVectorY(0, 1, 0)
,m_id(++m_idNow)
,m_width(10)
,m_height(10)
{    
    assert(m_object);
}

WindowInterface::~WindowInterface()
{

}

void WindowInterface::Resize( float &a_width, float &a_height )
{
    a_width = (a_width < MAX_VELOCITY) ? MAX_VELOCITY : a_width;
    a_height = (a_height < MAX_VELOCITY) ? MAX_VELOCITY : a_height;

    m_object->SetScale(PMVector(m_width, m_height, 0));
}

UInt32 WindowInterface::GetId()
{
    return m_id;
}

PMVector WindowInterface::GetProjectionPoint( PMVector a_point )
{
    PMVector position = m_object->GetPosition();

    float k = (m_normal.x * (position.x - a_point.x) + m_normal.y * (position.y - a_point.y) + m_normal.z * (position.z - a_point.z)) / (m_normal.x * m_normal.x + m_normal.y * m_normal.y + m_normal.z * m_normal.z);

    return PMVector(a_point.x + m_normal.x * k, a_point.y + m_normal.y * k, a_point.z + m_normal.z * k);
}

PMVector WindowInterface::TransformByCoordinateSqure( PMVector a_point )
{
    PMVector position = m_object->GetPosition();
    float normalRate = (m_normal.x * (position.x - a_point.x) + m_normal.y * (position.y - a_point.y) + m_normal.z * (position.z - a_point.z)) / (m_normal.x * m_normal.x + m_normal.y * m_normal.y + m_normal.z * m_normal.z);
    float vectorXRate = (m_planeVectorX.x * (position.x - a_point.x) + m_planeVectorX.y * (position.y - a_point.y) + m_planeVectorX.z * (position.z - a_point.z)) / (m_planeVectorX.x * m_planeVectorX.x + m_planeVectorX.y * m_planeVectorX.y + m_planeVectorX.z * m_planeVectorX.z);
    float vectorYRate = (m_planeVectorY.x * (position.x - a_point.x) + m_planeVectorY.y * (position.y - a_point.y) + m_planeVectorY.z * (position.z - a_point.z)) / (m_planeVectorY.x * m_planeVectorY.x + m_planeVectorY.y * m_planeVectorY.y + m_planeVectorY.z * m_planeVectorY.z);

    float distanceSqureX = (m_planeVectorX.x * m_planeVectorX.x + m_planeVectorX.y * m_planeVectorX.y + m_planeVectorX.z * m_planeVectorX.z) * vectorXRate * vectorXRate;
    float distanceSqureY = (m_planeVectorY.x * m_planeVectorY.x + m_planeVectorY.y * m_planeVectorY.y + m_planeVectorY.z * m_planeVectorY.z) * vectorYRate * vectorYRate;
    float distanceSqureZ = (m_normal.x * m_normal.x + m_normal.y * m_normal.y + m_normal.z * m_normal.z) * normalRate * normalRate;

    return PMVector(distanceSqureX, distanceSqureY, distanceSqureZ);
}

void WindowInterface::ChangeRange( float &a_range )
{

}


