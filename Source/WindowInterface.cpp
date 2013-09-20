#include "WindowInterface.h"
#include "GameObject.h"
#include "PMDefine.h"
#include <assert.h>

UInt32 WindowInterface::m_idNow = 0;

WindowInterface::WindowInterface( GameObject* a_object )
:
m_object(a_object)
,m_normal(0, 1, 0)
,m_planeVectorX(1, 0, 0)
,m_planeVectorY(0, 0, 1)
,m_id(++m_idNow)
,m_width(MAX_VELOCITY)
,m_height(MAX_VELOCITY)
,m_state(WINDOW_STATE_NORMAL)
,m_resizeFingerID(0)
{    
    assert(m_object);
}

WindowInterface::~WindowInterface()
{

}

void WindowInterface::Resize( float &a_width, float &a_height )
{
    m_width = (a_width < MAX_VELOCITY) ? MAX_VELOCITY : a_width;
    m_height = (a_height < MAX_VELOCITY) ? MAX_VELOCITY : a_height;

    m_object->SetScale(PMVector(m_width, 0, m_height));
}

UInt32 WindowInterface::GetId()
{
    return m_id;
}

PMVector WindowInterface::GetProjectionPoint( PMVector a_point )
{
    PMVector position = m_object->GetAbsolutePosition();

    float k = (m_normal.x * (position.x - a_point.x) + m_normal.y * (position.y - a_point.y) + m_normal.z * (position.z - a_point.z)) / (m_normal.MagnitudeSquared());

    return PMVector(a_point.x + m_normal.x * k, a_point.y + m_normal.y * k, a_point.z + m_normal.z * k);
}

PMVector WindowInterface::TransformByCoordinateSqure( PMVector a_point )
{
    PMVector position = m_object->GetAbsolutePosition();
    float normalRate = (m_normal.x * (position.x - a_point.x) + m_normal.y * (position.y - a_point.y) + m_normal.z * (position.z - a_point.z)) / (m_normal.MagnitudeSquared());
    float vectorXRate = (m_planeVectorX.x * (position.x - a_point.x) + m_planeVectorX.y * (position.y - a_point.y) + m_planeVectorX.z * (position.z - a_point.z)) / (m_planeVectorX.MagnitudeSquared());
    float vectorYRate = (m_planeVectorY.x * (position.x - a_point.x) + m_planeVectorY.y * (position.y - a_point.y) + m_planeVectorY.z * (position.z - a_point.z)) / (m_planeVectorY.MagnitudeSquared());

    float distanceSqureX = (m_planeVectorX.MagnitudeSquared()) * vectorXRate * vectorXRate;
    float distanceSqureY = (m_planeVectorY.MagnitudeSquared()) * vectorYRate * vectorYRate;
    float distanceSqureZ = (m_normal.MagnitudeSquared()) * normalRate * normalRate;

    return PMVector(distanceSqureX, distanceSqureY, distanceSqureZ);
}

void WindowInterface::ChangeRange( float &a_range )
{

}

void WindowInterface::SetNormalDirection( PMVector a_normal, PMVector a_planeVectorX, PMVector a_planeVectorY )
{
    m_normal = a_normal;
    m_planeVectorX = a_planeVectorX;
    m_planeVectorY = a_planeVectorY;
}

GameObject* WindowInterface::GetObject()
{
    return m_object;
}

WindowDistanceType WindowInterface::PointToWindowDistanceType( PMVector a_point )
{
    PMVector dis = TransformByCoordinateSqure(a_point);

    if (dis.x <= (m_width * m_width / 4) && dis.y <= (m_height * m_height / 4))
    {
        if (dis.z == 0)
        {
            return WINDOW_DISTANCE_ON_PLANE;
        }
        else if (dis.z < ATTACH_DISTANCE_SQUARE)
        {
            return WINDOW_DISTANCE_ATTACH;
        }
        else if (dis.z < NEAR_DISTANCE_SQUARE)
        {
            return WINDOW_DISTANCE_NEAR;
        }
        else
        {
            return WINDOW_DISTANCE_FAR;
        }
    }
    else
    {
        return WINDOW_DISTANCE_FAR;
    }
}

void WindowInterface::UpdateFingers( std::map<UInt32, PMVector>& a_data )
{
    WindowFingerData data;
    for (std::map<UInt32, PMVector>::iterator iter = a_data.begin();
        iter != a_data.end();
        ++iter)
    {
        PMVector distance = TransformByCoordinateSqure(iter->second);
        if (distance.z < WINDOW_DISTANCE_ATTACH)
        {
            data[iter->first] = std::pair<float, float>(distance.x, distance.y);
        }
        else if (iter->first == m_resizeFingerID && distance.z < WINDOW_DISTANCE_NEAR)
        {
            data[iter->first] = std::pair<float, float>(distance.x, distance.y);
        }
    }

    switch (m_state)
    {
    case WINDOW_STATE_NORMAL:
        {
            for (WindowFingerData::iterator iter = data.begin();
                iter != data.end();
                ++iter)
            {
                if (abs(iter->second.first - m_width * m_width / 4) < FINGER_CLICK_DISTANCE_SQUARE &&
                    abs(iter->second.second - m_height * m_height / 4) < FINGER_CLICK_DISTANCE_SQUARE)
                {
                    m_state = WINDOW_STATE_RESIZING;
                    m_resizeFingerID = iter->first;
                    break;
                }
            }
        }
        break;
    case WINDOW_STATE_RESIZING:
        {
            WindowFingerData::iterator iter = data.find(m_resizeFingerID);
            if (iter != data.end())
            {
                float width = sqrt(iter->second.first) * 2;
                float height = sqrt(iter->second.second) * 2;
                Resize(width, height);
            }
            else
            {
                m_resizeFingerID = 0;
                m_state = WINDOW_STATE_NORMAL;
            }
        }
        break;
    case WINDOW_STATE_SCALING:
        break;
    default:
        break;
    }

    m_lastFingerData.clear();
    m_lastFingerData = data;
}


