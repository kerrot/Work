#pragma once
#include "GameObject.h"
#include "PMCommon.h"
#include "PMType.h"

enum HandSetting
{
    MAX_FINGERS                 = 5,
};

class WindowInterface;

class HandObject : public GameObject
{
public:
    void SetFingerPosition(int a_index, PMVector a_position);
    void HideFingler(int a_index);
    virtual void SetVisible(bool a_result);

    WindowInterface* GetAttachedWindow();
    PMVector GetFingerAbsolutePosition(int a_index);
    void AttachWindow(WindowInterface* a_window);

    virtual void SetPosition(PMVector a_position);
    virtual void SetRotation(PMVector a_rotation);

    //void SetNormalDirection(PMVector a_normal, PMVector a_direction);
    
private:
    friend class GameObjectFactory;
    HandObject(UInt32 a_id);
    virtual ~HandObject();

    UInt32 m_id;
    WindowInterface* m_window;
    PMVector m_down;
    GameObject m_fingles[MAX_FINGERS];
};