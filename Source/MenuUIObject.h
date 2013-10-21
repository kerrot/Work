#pragma once

#include "GameObject.h"
#include "MainWindowInterFace.h"

class ButtonObject;

class MenuUIObject : public GameObject, public MainWindowInterface
{
public:
    
    
private:
    friend class GameObjectFactory;
    MenuUIObject(GameObject* a_ui, GameObject* a_shadow);
    virtual ~MenuUIObject();

    void Init();
    void CloseButton();
    void Game();
    void Comic();

    ButtonObject* m_closeButton;
    ButtonObject* m_startButton;
    ButtonObject* m_comicButton;
};