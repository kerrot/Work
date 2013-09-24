#pragma once

#include "PMCommon.h"
#include "PMType.h"



class GameObject;
class HandObject;

class WindowInterface
{
public:
    PMVector GetProjectionPoint(PMVector a_point);
    PMVector TransformByCoordinateSqure(PMVector a_point);

    virtual void Resize(float &a_width, float &a_height);
    virtual void ChangeRange(float &a_range);
    UInt32 GetId();

    GameObject* GetObject();

    void SetNormalDirection(PMVector a_normal, PMVector a_planeVectorX, PMVector a_planeVectorY);

    void UpdateFingers(std::map<UInt32, PMVector>& a_data);
    void UpdateHands(std::map<UInt32, HandObject*>& a_data);
protected:
    WindowInterface(GameObject* a_object, GameObject* a_ui);
    virtual ~WindowInterface();

    enum WindowSetting
    {
        ATTACH_DISTANCE                     = 10,
        ATTACH_DISTANCE_SQUARE              = 100,
        NEAR_DISTANCE_SQUARE                = 500,
        FINGER_CLICK_DISTANCE_SQUARE        = 300,
        FINGER_CLICK_TIME                   = 1000,
        FINGER_STOP_DISTANCE_SQUARE         = 1,
    };

    enum WindowState
    {
        WINDOW_STATE_NORMAL,
        WINDOW_STATE_RESIZING,
        WINDOW_STATE_SCALING,
    };

    struct WindowFingerData
    {
        PMVector oriPos;
        PMVector distance;
    };

    void HideAllShadows();
    void UpdateShadow(std::map<UInt32, WindowFingerData>& a_data);

    static UInt32 m_idNow;
    UInt32 m_id;
    UInt32 m_resizeFingerID;
    UInt32 m_resizeTime;
    WindowState m_state;

    float m_width;
    float m_height;

    GameObject* m_object;
    GameObject* m_ui;
    HandObject* m_attachedHand;

    PMVector m_normal;
    PMVector m_planeVectorX;
    PMVector m_planeVectorY;

    UInt32 m_moveTime;
    PMVector m_lastHandPosition;
    PMVector m_lastHandRotation;

    std::map<UInt32, WindowFingerData> m_lastFingerData;

    std::vector<GameObject*> m_shadows;
};