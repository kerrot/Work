#include "MenuUIObject.h"
#include "ButtonObject.h"
#include "GameWorld.h"

MenuUIObject::MenuUIObject(GameObject* a_ui)
:
MainWindowInterface(this, a_ui)
{

}

MenuUIObject::~MenuUIObject()
{

}

void MenuUIObject::Init()
{
    m_closeButton->SetPosition(0.5, SUBWINDOW_DISTANCE, 0.5);
//     float w = 0.1, h = 0.1;
//     m_closeButton->Resize(w, h);
    m_closeButton->SetButtonDisplay(TEXTURE_CLOSE_NORMAL, TEXTURE_CLOSE_HOVER, TEXTURE_CLOSE_PRESS, TEXTURE_CLOSE_DISABLE);
    m_closeButton->ReleaseEvent.AddCallBack(this, &MenuUIObject::CloseButton);
    m_subWindows.push_back(m_closeButton);

    m_startButton->SetPosition(-0.5, SUBWINDOW_DISTANCE, 0.5);
    m_startButton->SetButtonDisplay(TEXTURE_CLOSE_NORMAL, TEXTURE_CLOSE_HOVER, TEXTURE_CLOSE_PRESS, TEXTURE_CLOSE_DISABLE);
    m_startButton->ReleaseEvent.AddCallBack(this, &MenuUIObject::Game);
    m_subWindows.push_back(m_startButton);
}

void MenuUIObject::CloseButton()
{
    SetVisible(false);
    SetEnabled(false);
}

void MenuUIObject::Game()
{
    SetVisible(false);
    SetEnabled(false);
    if (sGameWorld.GetWorldState() == WORLD_STATE_NORMAL)
    {
        sGameWorld.GameStart();
    }
    else
    {
        sGameWorld.BackToNormal();
    }
}
