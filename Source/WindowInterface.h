#pragma once

#include "PMCommon.h"
#include "PMType.h"

class GameObject;

class WindowInterface
{
public:
    virtual void Resize(float a_width, float a_height);
    UInt32 GetId();

protected:
    WindowInterface(GameObject* a_object);
    virtual ~WindowInterface();

    static UInt32 m_idNow;
    UInt32 m_id;

    float m_width;
    float m_height;

    GameObject* m_object;

    PMVector m_normal;
};