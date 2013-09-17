#pragma once
#include "GameObject.h"
#include "PMCommon.h"

struct PMVector;

enum HandSetting
{
    MAX_FINGERS                 = 5,
};

class HandObject : public GameObject
{
public:
    void SetFingerPosition(int a_index, PMVector a_position);
    void HideFingler(int a_index);
    virtual void SetVisible(bool a_result);

    //void SetNormalDirection(PMVector a_normal, PMVector a_direction);
    
private:
    friend class GameObjectFactory;
    HandObject(UInt32 a_id);
    virtual ~HandObject();

    UInt32 m_id;
    GameObject m_fingles[MAX_FINGERS];
};