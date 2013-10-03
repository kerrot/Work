#pragma once

#include "GameObject.h"
#include "MainWindowInterFace.h"

class ButtonObject;

class MenuUIObject : public GameObject, public MainWindowInterface
{
public:
    
    
private:
    friend class GameObjectFactory;
    MenuUIObject(GameObject* a_ui);
    virtual ~MenuUIObject();

    void Init();

    ButtonObject* m_closeButton;
};