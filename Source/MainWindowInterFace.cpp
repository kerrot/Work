#include "MainWindowInterFace.h"
#include "PMDefine.h"
#include "HandObject.h"
#include <assert.h>
#include "GameTime.h"

MainWindowInterface::MainWindowInterface( GameObject* a_object, GameObject* a_ui )
:
WindowInterface(a_object)
,m_ui(a_ui)
,m_attachedHand(0)
,m_range(MAX_VELOCITY)
,m_state(WINDOW_STATE_NORMAL)
,m_releaseTime(0)
,m_resizeCursor(0)
,m_canResize(true)
,m_canScale(true)
{
    assert(m_ui);
}

MainWindowInterface::~MainWindowInterface()
{

}

void MainWindowInterface::Resize( float &a_width, float &a_height )
{
    m_width = (a_width < MAX_VELOCITY) ? MAX_VELOCITY : a_width;
    m_height = (a_height < MAX_VELOCITY) ? MAX_VELOCITY : a_height;

    m_object->SetScale(PMVector(m_width, 0, m_height));
}

void MainWindowInterface::ChangeRange( float &a_range )
{

}

void MainWindowInterface::UpdateHands( std::map<UInt32, HandObject*>& a_data )
{
    UInt32 current = sGameTime.GetCurrentTimeInMS();

    if (m_attachedHand)
    {
        std::map<UInt32, HandObject*>::iterator iter = a_data.find(m_attachedHand->GetId());
        if (iter != a_data.end())
        {
            HandObject* hand = iter->second;

            PMVector posShift = m_lastHandPosition - hand->GetAbsolutePosition();
            PMVector roShift = m_lastHandRotation - hand->GetRotation();

            if (posShift.MagnitudeSquared() > STOP_DISTANCE_SQUARE || roShift.MagnitudeSquared() > STOP_DISTANCE_SQUARE)
            {
                m_releaseTime = current;
            }
            else
            {
                if (current - m_releaseTime > FINGER_CLICK_TIME)
                {
                    m_attachedHand->AttachWindow(0);
                    m_attachedHand = 0;
                    ChangeState(WINDOW_STATE_NORMAL);
                }
            }

            m_lastHandPosition = hand->GetAbsolutePosition();
            m_lastHandRotation = hand->GetRotation();
        }
        else
        {
            m_attachedHand->AttachWindow(0);
            m_attachedHand = 0;
            ChangeState(WINDOW_STATE_NORMAL);
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
            if (distance.z < ATTACH_DISTANCE_SQUARE &&
                distance.x < m_width * m_width / 4 &&
                distance.y < m_height * m_height / 4)
            {
                m_attachedHand = hand;
                hand->AttachWindow(this);
                m_lastHandPosition = hand->GetAbsolutePosition();
                m_lastHandRotation = hand->GetRotation();
                m_releaseTime = current;

                ChangeState(WINDOW_STATE_MOVE);
                return;
            }
        }
    }
}

void MainWindowInterface::InterAction( CursorData &a_data )
{
    switch (m_state)
    {
    case MainWindowInterface::WINDOW_STATE_NORMAL:
        UpdateNormalState(a_data);
        break;
    case MainWindowInterface::WINDOW_STATE_MOVE:
        break;
    case MainWindowInterface::WINDOW_STATE_RESIZING:
        UpdateResizeState(a_data);
        break;
    case MainWindowInterface::WINDOW_STATE_SCALING:
        UpdateScaleState(a_data);
        break;
    default:
        break;
    }

    UpdateState();
}

void MainWindowInterface::ChangeState( WindowState a_state )
{
    GameObjectTexture t = TEXTURE_NONE;

    switch (a_state)
    {
    case MainWindowInterface::WINDOW_STATE_NORMAL:
        break;
    case MainWindowInterface::WINDOW_STATE_MOVE:
        t = TEXTURE_MOVE;
        break;
    case MainWindowInterface::WINDOW_STATE_RESIZING:
        t = TEXTURE_RESIZE;
        break;
    case MainWindowInterface::WINDOW_STATE_SCALING:
        t = TEXTURE_SCALE;
        break;
    default:
        break;
    }

    m_ui->SetVisible(t != TEXTURE_NONE);
    m_ui->ChangeTexture(t);

    m_state = a_state;
}

void MainWindowInterface::UpdateState()
{
    if (m_state != WINDOW_STATE_NORMAL)
    {
        float shift = ATTACH_DISTANCE * (FINGER_CLICK_TIME - sGameTime.GetCurrentTimeInMS() + m_releaseTime) / FINGER_CLICK_TIME;

        PMVector scale = m_object->GetScale();
        m_ui->SetPosition(PMVector(0, shift / scale.y, 0));
    }
}

void MainWindowInterface::UpdateNormalState( CursorData &a_data )
{
    if (a_data.cursorID == 0)
    {
        return;
    }

    WindowFingerData& cursor = a_data.fingerData[a_data.cursorID];
    if (cursor.distance.z > CORNER_ATTACH_DISTANCE_SQUARE)
    {
        return;
    }
    float tmpWidth = m_width * m_width / 4;
    float tmpHeight = m_height * m_height / 4;
    float diffx = abs(cursor.distance.x - tmpWidth);
    float diffy = abs(cursor.distance.y - tmpHeight);

    if (diffx < CORNER_ATTACH_DISTANCE_SQUARE &&
        diffy < CORNER_ATTACH_DISTANCE_SQUARE)
    {
        if (m_canResize)
        {
            ChangeState(WINDOW_STATE_RESIZING);
            m_resizeCursor = a_data.cursorID;
            m_releaseTime = sGameTime.GetCurrentTimeInMS();
            return;
        }
    }
    else
    {
        m_canResize = true;
        m_scalePoints.clear();

        for (std::map<UInt32, WindowFingerData>::iterator iter = a_data.fingerData.begin();
            iter != a_data.fingerData.end();
            ++iter)
        {
            WindowFingerData& finger = iter->second;

            float tmpDiffx = abs(finger.distance.x - tmpWidth);
            float tmpDiffy = abs(finger.distance.y - tmpHeight);

            if (finger.distance.z < ATTACH_DISTANCE_SQUARE &&
                finger.distance.x < tmpWidth &&
                finger.distance.y < tmpHeight)
            {
                m_scalePoints.insert(iter->first);
            }
        }

        if (cursor.distance.x < tmpWidth &&
            cursor.distance.y < tmpHeight &&
            m_scalePoints.size() > 1)
        {
            if (m_canScale)
            {
                ChangeState(WINDOW_STATE_SCALING);
                m_releaseTime = sGameTime.GetCurrentTimeInMS();

                for (std::set<UInt32>::iterator iter = m_scalePoints.begin();
                    iter != m_scalePoints.end();
                    ++iter)
                {
                    PMVector dis = a_data.fingerData[*iter].distance - cursor.distance;
                    m_scaleDistance += dis.MagnitudeSquared();
                }
                m_scaleDistance /= (m_scalePoints.size() - 1);
                m_lastscaleDistance = m_scaleDistance;
                m_initRange = m_range;
            }
        }
        else
        {
            m_scaleDistance = 0;
            m_canScale = true;
        }
    }
}

void MainWindowInterface::UpdateResizeState( CursorData &a_data )
{
    m_canResize = false;
    if (a_data.cursorID != m_resizeCursor)
    {
        ChangeState(WINDOW_STATE_NORMAL);
        m_resizeCursor = 0;
    }
    else
    {
        WindowFingerData& Cursor = a_data.fingerData[m_resizeCursor];

        float width = sqrt(Cursor.distance.x) * 2;
        float height = sqrt(Cursor.distance.y) * 2;
        Resize(width, height);

        UInt32 current = sGameTime.GetCurrentTimeInMS();

        PMVector diff = m_lastData.fingerData[m_resizeCursor].oriPos - Cursor.oriPos;        
        if (diff.MagnitudeSquared() > STOP_DISTANCE_SQUARE)
        {
            m_releaseTime = current;
        }
        else
        {
            if (current - m_releaseTime > FINGER_CLICK_TIME)
            {
                ChangeState(WINDOW_STATE_NORMAL);
                m_resizeCursor = 0;
            }
        }
    }
}

void MainWindowInterface::UpdateScaleState( CursorData &a_data )
{
    m_canScale = false;
    if (a_data.cursorID != m_lastData.cursorID)
    {
        ChangeState(WINDOW_STATE_NORMAL);
        return;
    }

    UInt32 count = 0;
    float tmpScaleDistance = 0;
    WindowFingerData& cursor = a_data.fingerData[a_data.cursorID];
    for (std::set<UInt32>::iterator iter = m_scalePoints.begin();
        iter != m_scalePoints.end();
        ++iter)
    {
        std::map<UInt32, WindowFingerData>::iterator fingerIter = a_data.fingerData.find(*iter);
        if (fingerIter != a_data.fingerData.end())
        {
            PMVector dis = fingerIter->second.distance - cursor.distance;
            tmpScaleDistance += dis.MagnitudeSquared();
            ++count;
        }
    }

    if (count <= 1)
    {
        ChangeState(WINDOW_STATE_NORMAL);
        return;
    }

    tmpScaleDistance /= (count - 1);

    float diffScale = tmpScaleDistance / m_scaleDistance;
    float range = m_initRange * diffScale;
    ChangeRange(range);

    UInt32 current = sGameTime.GetCurrentTimeInMS();

    float change = abs(diffScale - m_lastscaleDistance / m_scaleDistance) * 10;
    if (change > STOP_DISTANCE_SQUARE)
    {
        m_releaseTime = current;
    }
    else
    {
        if (current - m_releaseTime > FINGER_CLICK_TIME)
        {
            ChangeState(WINDOW_STATE_NORMAL);
        }
    }

    m_lastscaleDistance = tmpScaleDistance;
}

void MainWindowInterface::SetNormalDirection( PMVector a_normal, PMVector a_planeVectorX, PMVector a_planeVectorY )
{
    WindowInterface::SetNormalDirection(a_normal, a_planeVectorX, a_planeVectorY);

    for (std::vector<WindowInterface*>::iterator iter = m_subWindows.begin();
        iter != m_subWindows.end();
        ++iter)
    {
        WindowInterface* window = *iter;
        window->SetNormalDirection(a_normal, a_planeVectorX, a_planeVectorY);
    }
}


