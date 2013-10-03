#include "MenuUIObject.h"
#include "ButtonObject.h"

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
    //float w = 0.1, h = 0.1;
    //m_closeButton->Resize(w, h);
    m_closeButton->SetButtonDisplay(TEXTURE_CLOSE_NORMAL, TEXTURE_CLOSE_HOVER, TEXTURE_CLOSE_PRESS, TEXTURE_CLOSE_DISABLE);
    m_subWindows.push_back(m_closeButton);
}
