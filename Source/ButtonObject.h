#pragma once

#include "GameObject.h"
#include "WindowInterface.h"
#include "Delegate.h"

class ButtonObject : public WindowInterface, public GameObject
{
public:
    void SetButtonDisplay(GameObjectTexture a_n, GameObjectTexture a_r, GameObjectTexture a_p, GameObjectTexture a_d);

    virtual void Resize(float &a_width, float &a_height);

    Delegate ReleaseEvent;
protected:
    friend class GameObjectFactory;
    ButtonObject(GameObject* a_shadow);
    virtual ~ButtonObject();

    enum ButtonDisplay
    {
        BUTTON_DISPLAY_NORMAL,
        BUTTON_DISPLAY_ROLLOVER,
        BUTTON_DISPLAY_PRESS,
        BUTTON_DISPLAY_DISABLE,

        MAX_BUTTON_DISPLAY,
    };

    enum ButtonState
    {
        BUTTON_STATE_NORMAL,
        BUTTON_STATE_HOVOR,
        BUTTON_STATE_PRESS,
    };

    virtual void InterAction(CursorData &a_data);

    void OnRollOver();
    void OnRollOut();
    void OnPress();
    void OnRelease();
    void OnReleaseOutside();

    ButtonState m_state;

    GameObjectTexture m_buttonDisplay[MAX_BUTTON_DISPLAY];
};