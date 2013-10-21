#include "ButtonObject.h"

ButtonObject::ButtonObject(GameObject* a_shadow)
:
WindowInterface(this, a_shadow)
,m_state(BUTTON_STATE_NORMAL)
{

}

ButtonObject::~ButtonObject()
{

}

void ButtonObject::InterAction( CursorData &a_data )
{
    if (a_data.cursorID == 0 || !m_enabled)
    {
        return;
    }

    WindowFingerData& cursor = a_data.fingerData[a_data.cursorID];
    float tmpWidth = m_width * m_width / 4;
    float tmpHeight = m_height * m_height / 4;

    if (cursor.distance.x < tmpWidth && cursor.distance.y < tmpHeight && cursor.distance.z < NEAR_DISTANCE_SQUARE)
    {
        if (cursor.distance.z < ATTACH_DISTANCE)
        {
            if (m_state != BUTTON_STATE_PRESS)
            {
                OnPress();
            }
        }
        else
        {
            if (m_state == BUTTON_STATE_PRESS)
            {
                OnRelease();
            }

            if (m_state != BUTTON_STATE_HOVOR)
            {
                OnRollOver();
            }
        }
    }
    else
    {
        if (m_state == BUTTON_STATE_HOVOR)
        {
            OnRollOut();
        }

        if (m_state == BUTTON_STATE_PRESS)
        {
            OnReleaseOutside();
        }
    }

//     if ((abs(a_data.coordinate.x) <= tmpWidth && abs(a_data.coordinate.y) <= tmpHeight && a_data.coordinate.z <= NEAR_DISTANCE && a_data.coordinate.z > ATTACH_DISTANCE) &&
//         (abs(m_lastData.coordinate.x) > tmpWidth || abs(m_lastData.coordinate.y) > tmpHeight || m_lastData.coordinate.z > NEAR_DISTANCE || m_lastData.coordinate.z < ATTACH_DISTANCE))
//     {
//         OnRollOver();
//     }
// 
//     if ((abs(m_lastData.coordinate.x) <= tmpWidth && abs(m_lastData.coordinate.y) <= tmpHeight && m_lastData.coordinate.z <= NEAR_DISTANCE && m_lastData.coordinate.z > ATTACH_DISTANCE) &&
//         (abs(a_data.coordinate.x) > tmpWidth || abs(a_data.coordinate.y) > tmpHeight || a_data.coordinate.z > NEAR_DISTANCE))
//     {
//         OnRollOut();
//     }
// 
//     if ((abs(a_data.coordinate.x) <= tmpWidth && abs(a_data.coordinate.y) <= tmpHeight && a_data.coordinate.z <= ATTACH_DISTANCE && a_data.coordinate.z > 0) &&
//         (abs(m_lastData.coordinate.x) > tmpWidth || abs(m_lastData.coordinate.y) > tmpHeight || m_lastData.coordinate.z > ATTACH_DISTANCE || m_lastData.coordinate.z < 0))
//     {
//         OnPress();
//     }
// 
//     if ((abs(m_lastData.coordinate.x) <= tmpWidth && abs(m_lastData.coordinate.y) <= tmpHeight && m_lastData.coordinate.z <= ATTACH_DISTANCE && m_lastData.coordinate.z > 0) &&
//         (abs(a_data.coordinate.x) > tmpWidth || abs(a_data.coordinate.y) > tmpHeight || a_data.coordinate.z > ATTACH_DISTANCE || a_data.coordinate.z < 0))
//     {
//         OnRelease();
//     }

//     if (cursor.distance.z < NEAR_DISTANCE_SQUARE &&
//         cursor.distance.x <= tmpWidth &&
//         cursor.distance.y <= tmpHeight)
//     {
//         OnRollOver();
//         if (a_data.cursorID != m_lastData.cursorID)
//         {
//             return;
//         }
//         WindowFingerData& lastCursor = m_lastData.fingerData[m_lastData.cursorID];
//         if (cursor.distance.z < ATTACH_DISTANCE_SQUARE &&
//             (lastCursor.distance.z > ATTACH_DISTANCE_SQUARE || lastCursor.distance.x > tmpWidth || lastCursor.distance.y > tmpHeight)
//             )
//         {
//             OnPress();
//         }
//     }
//     else
//     {
//         OnRollOut();
//         WindowFingerData& lastCursor = m_lastData.fingerData[m_lastData.cursorID];
//         if ((cursor.distance.z > ATTACH_DISTANCE_SQUARE) &&
//             (lastCursor.distance.z < ATTACH_DISTANCE_SQUARE && lastCursor.distance.x <= tmpWidth && lastCursor.distance.y <= tmpHeight)
//             )
//         {
//             if (cursor.distance.x > tmpWidth || cursor.distance.y > tmpHeight)
//             {
//                 OnReleaseOutside();
//             }
//             else
//             {
//                 OnRelease();
//             }
//         }
//     }
}

void ButtonObject::OnRollOver()
{
    ChangeTexture(m_buttonDisplay[BUTTON_DISPLAY_ROLLOVER]);
    m_state = BUTTON_STATE_HOVOR;
}

void ButtonObject::OnRollOut()
{
    ChangeTexture(m_buttonDisplay[BUTTON_DISPLAY_NORMAL]);
    m_state = BUTTON_STATE_NORMAL;
}

void ButtonObject::OnPress()
{
    ChangeTexture(m_buttonDisplay[BUTTON_DISPLAY_PRESS]);
    m_state = BUTTON_STATE_PRESS;
}

void ButtonObject::OnRelease()
{
    ChangeTexture(m_buttonDisplay[BUTTON_DISPLAY_NORMAL]);
    m_state = BUTTON_STATE_NORMAL;
    ReleaseEvent();
}

void ButtonObject::OnReleaseOutside()
{
    ChangeTexture(m_buttonDisplay[BUTTON_DISPLAY_NORMAL]);
    m_state = BUTTON_STATE_NORMAL;
}

void ButtonObject::SetButtonDisplay( GameObjectTexture a_n, GameObjectTexture a_r, GameObjectTexture a_p, GameObjectTexture a_d )
{
    m_buttonDisplay[BUTTON_DISPLAY_NORMAL] = a_n;
    m_buttonDisplay[BUTTON_DISPLAY_ROLLOVER] = a_r;
    m_buttonDisplay[BUTTON_DISPLAY_PRESS] = a_p;
    m_buttonDisplay[BUTTON_DISPLAY_DISABLE] = a_d;
}

void ButtonObject::Resize( float &a_width, float &a_height )
{
    m_width = a_width;
    m_height = a_height;
}
