#pragma once
#include "GameObject.h"
#include "HeadObject.h"
#include "PMType.h"
#include "PMCommon.h"

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

    void LookRotate(PMVector a_rotation);

    virtual void SetPosition(PMVector a_position);
    virtual void SetRotation(PMVector a_rotation);
    void SetHeadPosition(PMVector a_position);
    PMVector GetHeadPosition();
    PMVector GetHeadAbsolutePosition();

private:
    friend class GameObjectFactory;
    AvatarObject();
    virtual ~AvatarObject();

    void UpdateHeadLookAt();

    float m_speed;
    PMVector m_fornt;
    PMVector m_right;
    PMVector m_lookRotation;
    HeadObject m_head;
    GameObject m_target;
};