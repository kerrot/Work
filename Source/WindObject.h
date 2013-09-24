#pragma once
#include "CollidableObject.h"
#include "WindowInterface.h"

class WindObject : public CollidableObject, public WindowInterface
{
public:
    virtual void HitObject(LeafObject* a_object);
    virtual void Resize(float &a_width, float &a_height);
    virtual void ChangeRange(float &a_range);

private:
    friend class GameObjectFactory;
    WindObject(GameObject* a_ui);
    virtual ~WindObject();

    float m_force;
    float m_range;
};