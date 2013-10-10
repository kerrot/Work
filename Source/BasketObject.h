#pragma once
#include "CollidableObject.h"

class BasketObject : public CollidableObject
{
public:
    virtual void HitObject(LeafObject* a_object);

    void SetFail();

private:
    friend class GameObjectFactory;
    BasketObject(GameObject* a_ui);
    virtual ~BasketObject();

    bool m_fail;
    GameObject* m_ui;
};