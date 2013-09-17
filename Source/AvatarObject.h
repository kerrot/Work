#pragma once
#include "GameObject.h"
#include "HeadObject.h"
#include "PMType.h"

class AvatarObject : public GameObject
{
public:
    void Forward();
    void Backward();
    void Leftward();
    void Rightward();
    void Upward();
    void Downward();

    void TurnLeft();
    void TurnRight();

    virtual void SetPosition(PMVector a_position);
    virtual void SetRotation(PMVector a_rotation);
    void SetHeadPosition(PMVector a_position);

private:
    friend class GameObjectFactory;
    AvatarObject();
    virtual ~AvatarObject();

    float m_speed;
    PMVector m_fornt;
    PMVector m_right;
    HeadObject m_head;
    GameObject m_target;
};