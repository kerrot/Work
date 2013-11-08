#pragma once
#include "CollidableObject.h"

class BasketObject : public CollidableObject
{
public:
    virtual void HitObject(LeafObject* a_object);

    void SetFail();

private:
    friend class GameObjectFactory;
    BasketObject(GameObject* a_ui, GameObject* a_clear);
    virtual ~BasketObject();

    bool m_fail;
    GameObject* m_ui;
    GameObject* m_clear;
};