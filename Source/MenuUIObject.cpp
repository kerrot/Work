#include "MenuUIObject.h"
#include "ButtonObject.h"
#include "GameWorld.h"
#include "GameObjectFactory.h"

MenuUIObject::MenuUIObject(GameObject* a_ui, GameObject* a_shadow)
:
MainWindowInterface(this, a_ui, a_shadow)
{

}

MenuUIObject::~MenuUIObject()
{

}

void MenuUIObject::Init()
{
    m_closeButton->SetPosition(0.5, SUBWINDOW_DISTANCE, 0.5);
    float w = 10, h = 10;
    m_closeButton->Resize(w, h);
    m_closeButton->SetScale(PMVector(0.1, 1, 0.1));
    m_closeButton->SetButtonDisplay(TEXTURE_CLOSE_NORMAL, TEXTURE_CLOSE_HOVER, TEXTURE_CLOSE_PRESS, TEXTURE_CLOSE_DISABLE);
    m_closeButton->ReleaseEvent.AddCallBack(this, &MenuUIObject::CloseButton);
    m_subWindows.push_back(m_closeButton);

    m_startButton->SetPosition(0, SUBWINDOW_DISTANCE, 0.2);
    w = 80; h = 20;
    m_startButton->Resize(w, h);
    m_startButton->SetScale(PMVector(0.6, 1, 0.15));
    m_startButton->SetButtonDisplay(TEXTURE_START_NORMAL, TEXTURE_START_HOVER, TEXTURE_START_PRESS, TEXTURE_CLOSE_DISABLE);
    m_startButton->ReleaseEvent.AddCallBack(this, &MenuUIObject::Game);
    m_subWindows.push_back(m_startButton);

    m_comicButton->SetPosition(0, SUBWINDOW_DISTANCE, -0.2);
    w = 80; h = 20;
    m_comicButton->Resize(w, h);
    m_comicButton->SetScale(PMVector(0.6, 1, 0.15));
    m_comicButton->SetButtonDisplay(TEXTURE_COMIC_NORMAL, TEXTURE_COMIC_HOVER, TEXTURE_COMIC_PRESS, TEXTURE_CLOSE_DISABLE);
    m_comicButton->ReleaseEvent.AddCallBack(this, &MenuUIObject::Comic);
    m_subWindows.push_back(m_comicButton);
}

void MenuUIObject::CloseButton()
{
    SetVisible(false);
    SetEnabled(false);
}

void MenuUIObject::Game()
{
    CloseButton();
    if (sGameWorld.GetWorldState() == WORLD_STATE_NORMAL)
    {
        sGameWorld.GameStart();
        m_startButton->SetButtonDisplay(TEXTURE_END_NORMAL, TEXTURE_END_HOVER, TEXTURE_END_PRESS, TEXTURE_CLOSE_DISABLE);
    }
    else
    {
        sGameWorld.BackToNormal();
        m_startButton->SetButtonDisplay(TEXTURE_START_NORMAL, TEXTURE_START_HOVER, TEXTURE_START_PRESS, TEXTURE_CLOSE_DISABLE);
    }
}

void MenuUIObject::Comic()
{
    CloseButton();
    sGameObjectFactory.GetComicUI();
}
