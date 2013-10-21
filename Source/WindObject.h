#pragma once
#include "CollidableObject.h"
#include "MainWindowInterface.h"

class WindObject : public CollidableObject, public MainWindowInterface
{
public:
    virtual void HitObject(LeafObject* a_object);
    virtual void Resize(float &a_width, float &a_height);
    virtual void ChangeRange(float &a_range);
    virtual PMVector GetPlaneScale();
private:
    friend class GameObjectFactory;
    WindObject(GameObject* a_ui, GameObject* a_rangeUI, GameObject* a_shadow);
    virtual ~WindObject();

    virtual void ChangeState(WindowState a_state);
    virtual void UpdateState();

    bool m_working;
    float m_force;

    GameObject* m_rangeUI;
};