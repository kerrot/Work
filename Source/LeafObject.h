#pragma once
#include "GameObject.h"
#include "PMType.h"

class LeafObject : public GameObject
{
public:
    void AddAcceleration(PMVector a_acc);
    void ResetToBegin();
    void UpdatePosition();
    void SetOriposition(PMVector a_position);

private:
    friend class GameObjectFactory;
    LeafObject();
    LeafObject(PMVector a_position);
    virtual ~LeafObject();

    PMVector m_oriPosition;
    PMVector m_velocity;
    PMVector m_acceleration;
};