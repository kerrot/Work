#pragma once

#include "GameObject.h"
#include "MainWindowInterFace.h"

class ButtonObject;

class ComicUIObject : public GameObject, public MainWindowInterface
{
public:
    virtual void Resize(float &a_width, float &a_height);

private:
    friend class GameObjectFactory;
    ComicUIObject(GameObject* a_ui, GameObject* a_shadow);
    virtual ~ComicUIObject();

    void Init();
    void Close();
    void PageUp();
    void PageDown();

    void UpdatePage();
    void InitSize(UInt32 a_width, UInt32 a_height);

    ButtonObject* m_closeButton;
    ButtonObject* m_upButton;
    ButtonObject* m_downButton;
    
    UInt32 m_index;
    UInt32 m_oriWidth;
    UInt32 m_oriHeight;
    float m_scale;
    std::vector<std::string> m_comicList;
};