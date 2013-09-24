#include "WindowInterface.h"
#include "GameObject.h"
#include "HandObject.h"
#include "GameObjectFactory.h"
#include "PMDefine.h"
#include "GameTime.h"
#include <assert.h>

UInt32 WindowInterface::m_idNow = 0;

WindowInterface::WindowInterface( GameObject* a_object, GameObject* a_ui)
:
m_object(a_object)
,m_ui(a_ui)
,m_attachedHand(0)
,m_normal(0, 1, 0)
,m_planeVectorX(1, 0, 0)
,m_planeVectorY(0, 0, 1)
,m_id(++m_idNow)
,m_width(MAX_VELOCITY)
,m_height(MAX_VELOCITY)
,m_state(WINDOW_STATE_NORMAL)
,m_resizeFingerID(0)
,m_resizeTime(0)
,m_moveTime(0)
{    
    assert(m_object);
    assert(m_ui);
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

void WindowInterface::UpdateFingers( std::map<UInt32, PMVector>& a_data )
{
    HideAllShadows();

    std::map<UInt32, WindowFingerData> data;

    for (std::map<UInt32, PMVector>::iterator iter = a_data.begin();
         iter != a_data.end();
         ++iter)
    {
        PMVector distance = TransformByCoordinateSqure(iter->second);
        if (distance.z < NEAR_DISTANCE_SQUARE)
        {
            WindowFingerData tmp;
            tmp.oriPos = iter->second;
            tmp.distance = distance;

            data[iter->first] = tmp;
        }
    }

    UpdateShadow(data);

//     if (data.empty())
//     {
//         m_lastFingerData.clear();
//         m_resizeFingerID = 0;
//         m_state = WINDOW_STATE_NORMAL;
//         return;
//     }
// 
//     unsigned int current = sGameTime.GetCurrentTimeInMS();
// 
//     switch (m_state)
//     {
//     case WINDOW_STATE_NORMAL:
//         {
//             if (m_resizeFingerID == 0)
//             {
//                 m_resizeFingerID = data.begin()->first;
//                 m_resizeTime = current;
//             }
//             else
//             {
//                 WindowFingerData::iterator iter = data.find(m_resizeFingerID);
//                 if (iter != data.end())
//                 {
//                     if (current - m_resizeTime > FINGER_CLICK_TIME)
//                     {
//                         m_state = WINDOW_STATE_RESIZING;
//                         m_resizeTime = current;
//                     }
//                 }
//                 else
//                 {
//                     m_resizeFingerID = 0;
//                 }
//             }
//         }
//         break;
//     case WINDOW_STATE_RESIZING:
//         {
//             WindowFingerData::iterator iter = data.find(m_resizeFingerID);
//             if (iter != data.end())
//             {
//                 WindowFingerData::iterator lastIter = m_lastFingerData.find(m_resizeFingerID);
//                 if (lastIter != m_lastFingerData.end())
//                 {
//                     float width = sqrt(iter->second.first) * 2;
//                     float height = sqrt(iter->second.second) * 2;
//                     Resize(width, height);
// 
//                     float xDif = iter->second.first - lastIter->second.first;
//                     float yDif = iter->second.second - lastIter->second.second;
// 
//                     if (abs(xDif) < FINGER_STOP_DISTANCE_SQUARE &&
//                         abs(yDif) < FINGER_STOP_DISTANCE_SQUARE)
//                     {
//                         if (current - m_resizeTime > FINGER_CLICK_TIME)
//                         {
//                             m_state = WINDOW_STATE_NORMAL;
//                             m_resizeFingerID = 0;
//                         }
//                     }
//                     else
//                     {
//                          m_resizeTime = current;
//                     }
//                 }
//             }
//             else
//             {
//                 m_resizeFingerID = 0;
//                 m_state = WINDOW_STATE_NORMAL;
//             }
//         }
//         break;
//     case WINDOW_STATE_SCALING:
//         break;
//     default:
//         break;
//     }

    m_lastFingerData.clear();
    m_lastFingerData = data;
}

void WindowInterface::UpdateShadow(std::map<UInt32, WindowFingerData>& a_data)
{
    UInt32 num = 0;

    for (std::map<UInt32, WindowFingerData>::iterator iter = a_data.begin();
        iter != a_data.end();
        ++iter)
    {
        WindowFingerData& tmp = iter->second;

        if (tmp.distance.x < m_width * m_width / 4 &&
            tmp.distance.y < m_height * m_height / 4)
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
                m_shadows.push_back(shadow);
            }

            shadow->SetVisible(true);
            shadow->SetRotation(m_object->GetAbsoluteRotation());
            shadow->SetScale((tmp.distance.z < ATTACH_DISTANCE_SQUARE) ? PMVector(3, 0, 3): PMVector(10, 0, 10));

            PMVector position = m_object->GetAbsolutePosition() + m_planeVectorX * x + m_planeVectorY * y + m_normal;

            shadow->SetPosition(position);

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

void WindowInterface::UpdateHands( std::map<UInt32, HandObject*>& a_data )
{
    UInt32 current = sGameTime.GetCurrentTimeInMS();

    if (m_attachedHand)
    {
        std::map<UInt32, HandObject*>::iterator iter = a_data.find(m_attachedHand->GetId());
        if (iter != a_data.end())
        {
            HandObject* hand = iter->second;
            PMVector scale = m_object->GetScale();
            
            PMVector posShift = m_lastHandPosition - hand->GetAbsolutePosition();
            PMVector roShift = m_lastHandRotation - hand->GetRotation();
            
            if (posShift.MagnitudeSquared() > FINGER_STOP_DISTANCE_SQUARE || roShift.MagnitudeSquared() > FINGER_STOP_DISTANCE_SQUARE)
            {
                m_moveTime = current;
                m_ui->SetPosition(PMVector(0, ATTACH_DISTANCE / scale.y, 0));
            }
            else
            {
                if (current - m_moveTime > FINGER_CLICK_TIME)
                {
                    m_attachedHand->AttachWindow(0);
                    m_attachedHand = 0;
                    m_ui->SetVisible(false);
                }
                else
                {
                    float shift = ATTACH_DISTANCE * (FINGER_CLICK_TIME - current + m_moveTime) / FINGER_CLICK_TIME;
                    m_ui->SetPosition(PMVector(0, shift / scale.y, 0));
                }
            }

            m_lastHandPosition = hand->GetAbsolutePosition();
            m_lastHandRotation = hand->GetRotation();
        }
        else
        {
            m_attachedHand->AttachWindow(0);
            m_attachedHand = 0;
            m_ui->SetVisible(false);
        }
    }
    else
    {
        for (std::map<UInt32, HandObject*>::iterator iter = a_data.begin();
            iter != a_data.end();
            ++iter)
        {
            HandObject* hand = iter->second;

            PMVector distance = TransformByCoordinateSqure(hand->GetAbsolutePosition());
            if (distance.z < ATTACH_DISTANCE_SQUARE)
            {
                m_attachedHand = hand;
                hand->AttachWindow(this);
                m_lastHandPosition = hand->GetAbsolutePosition();
                m_lastHandRotation = hand->GetRotation();
                m_moveTime = current;

                m_ui->SetVisible(true);
                m_ui->ChangeTexture(TEXTURE_MOVE);

                PMVector scale = m_object->GetScale();

                m_ui->SetPosition(PMVector(0, ATTACH_DISTANCE / scale.y, 0));

                return;
            }
        }
    }
}


