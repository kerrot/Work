#include "WindowInterface.h"
#include "GameObject.h"
#include "HandObject.h"
#include "AvatarObject.h"
#include "GameObjectFactory.h"
#include "PMDefine.h"
#include "GameTime.h"
#include <assert.h>

UInt32 WindowInterface::m_idNow = 0;

WindowInterface::WindowInterface( GameObject* a_object)
:
m_object(a_object)
,m_normal(0, 1, 0)
,m_planeVectorX(1, 0, 0)
,m_planeVectorY(0, 0, 1)
,m_id(++m_idNow)
,m_width(MAX_VELOCITY)
,m_height(MAX_VELOCITY)
,m_enabled(true)
{    
    assert(m_object);
}

WindowInterface::~WindowInterface()
{

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

void WindowInterface::UpdateFingers( std::map<UInt32, PMVector>& a_data )
{
    HideAllShadows();

    CursorData data;
    UInt32 minDis = -1;

    for (std::map<UInt32, PMVector>::iterator iter = a_data.begin();
         iter != a_data.end();
         ++iter)
    {
        WindowFingerData tmp;
        tmp.oriPos = iter->second;
        tmp.distance = TransformByCoordinateSqure(iter->second);
        data.fingerData[iter->first] = tmp;

        if (tmp.distance.z < minDis)
        {
            minDis = tmp.distance.z;
            data.cursorID = iter->first;
        }
    }

    if (m_lastData.cursorID != 0)
    {
        std::map<UInt32, WindowFingerData>::iterator iter = data.fingerData.find(m_lastData.cursorID);
        if (iter != data.fingerData.end() && iter->second.distance.z < NEAR_DISTANCE_SQUARE)
        {
            data.cursorID = m_lastData.cursorID;
        }
    }

    UpdateShadow(data);
    if (m_enabled)
    {
        InterAction(data);
    }

    m_lastData.cursorID = data.cursorID;
    m_lastData.fingerData.clear();
    m_lastData.fingerData = data.fingerData;
    m_lastData.coordinate = data.coordinate;
}

void WindowInterface::UpdateShadow(CursorData& a_data)
{
    UInt32 num = 0;

    for (std::map<UInt32, WindowFingerData>::iterator iter = a_data.fingerData.begin();
        iter != a_data.fingerData.end();
        ++iter)
    {
        WindowFingerData& tmp = iter->second;

        if (tmp.distance.x < m_width * m_width / 4 &&
            tmp.distance.y < m_height * m_height / 4 &&
            tmp.distance.z < NEAR_DISTANCE_SQUARE)
        {
            PMVector direction = tmp.oriPos - m_object->GetAbsolutePosition();
            float x = (m_planeVectorX.Dot(direction) >= 0) ? sqrt(tmp.distance.x) : - sqrt(tmp.distance.x);
            float y = (m_planeVectorY.Dot(direction) >= 0) ? sqrt(tmp.distance.y) : - sqrt(tmp.distance.y);
            
            GameObject* shadow;

            if (num + 1 < m_shadows.size())
            {
                shadow = m_shadows[num];
            }
            else
            {
                shadow = sGameObjectFactory.CreatePlaneShadow();
                shadow->SetParent(m_object);
                m_shadows.push_back(shadow);
            }

            shadow->SetVisible(true);
            
            PMVector scale = m_object->GetAbsoluteScale();
            float shadowScale = (scale.x > scale.z) ? scale.x : scale.z; 
            shadow->SetScale((tmp.distance.z < ATTACH_DISTANCE_SQUARE) ? PMVector(5 / shadowScale, 0, 5 / shadowScale): PMVector(10 / shadowScale, 0, 10 / shadowScale));

            AvatarObject* avatar = sGameObjectFactory.GetAvatar();
            PMVector view = avatar->GetHeadAbsolutePosition() - m_object->GetAbsolutePosition();

            float shadowY = (view.Dot(m_normal) > 0) ? 1.0f / scale.y : -1.0f / scale.y;
            
            if (iter->first == a_data.cursorID)
            {
                a_data.coordinate.x = x;
                a_data.coordinate.y = y;
                a_data.coordinate.z = (m_normal.Dot(direction) >= 0) ? sqrt(tmp.distance.z) : - sqrt(tmp.distance.z);
                shadow->ChangeTexture(TEXTURE_PLANE_CURSOR);
            }
            else
            {
                shadow->ChangeTexture(TEXTURE_PLANE_SHADOW);
            }

            shadow->SetPosition(PMVector(x / scale.x, shadowY, y / scale.z));

            ++num;
        }
    }
}

void WindowInterface::HideAllShadows()
{
    for (std::vector<GameObject*>::iterator iter = m_shadows.begin();
        iter != m_shadows.end();
        ++iter)
    {
        GameObject* shadow = *iter;
        shadow->SetVisible(false);
    }
}

void WindowInterface::InterAction( CursorData &a_data )
{
    
}

void WindowInterface::Resize( float &a_width, float &a_height )
{
    m_object->SetScale(PMVector(a_width, 0, a_height));

    PMVector scale = m_object->GetAbsoluteScale();

    m_width = scale.x;
    m_height = scale.y;
}

void WindowInterface::SetEnabled( bool a_result )
{
    m_enabled = a_result;
}
