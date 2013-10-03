#pragma once

#include "GameObject.h"
#include "WindowInterface.h"

class ButtonObject : public WindowInterface, public GameObject
{
public:
    void SetButtonDisplay(GameObjectTexture a_n, GameObjectTexture a_r, GameObjectTexture a_p, GameObjectTexture a_d);

protected:
    friend class GameObjectFactory;
    ButtonObject();
    virtual ~ButtonObject();

    enum ButtonDisplay
    {
        BUTTON_DISPLAY_NORMAL,
        BUTTON_DISPLAY_ROLLOVER,
        BUTTON_DISPLAY_PRESS,
        BUTTON_DISPLAY_DISABLE,

        MAX_BUTTON_DISPLAY,
    };

    virtual void InterAction(CursorData &a_data);

    void OnRollOver();
    void OnRollOut();
    void OnPress();
    void OnRelease();
    void OnReleaseOutside();

    GameObjectTexture m_buttonDisplay[MAX_BUTTON_DISPLAY];
};