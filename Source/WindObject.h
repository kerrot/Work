#pragma once
#include "CollidableObject.h"
#include "MainWindowInterface.h"

class WindObject : public CollidableObject, public MainWindowInterface
{
public:
    virtual void HitObject(LeafObject* a_object);
    virtual void Resize(float &a_width, float &a_height);
    virtual void ChangeRange(float &a_range);

private:
    friend class GameObjectFactory;
    WindObject(GameObject* a_ui, GameObject* a_rangeUI);
    virtual ~WindObject();

    float m_force;

    GameObject* m_rangeUI;
};