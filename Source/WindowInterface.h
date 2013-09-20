#pragma once

#include "PMCommon.h"
#include "PMType.h"

class GameObject;

enum WindowDistanceType
{
    WINDOW_DISTANCE_ON_PLANE,
    WINDOW_DISTANCE_ATTACH,
    WINDOW_DISTANCE_NEAR,
    WINDOW_DISTANCE_FAR,
};

class WindowInterface
{
public:
    PMVector GetProjectionPoint(PMVector a_point);
    PMVector TransformByCoordinateSqure(PMVector a_point);

    WindowDistanceType PointToWindowDistanceType(PMVector a_point);

    virtual void Resize(float &a_width, float &a_height);
    virtual void ChangeRange(float &a_range);
    UInt32 GetId();

    GameObject* GetObject();

    void SetNormalDirection(PMVector a_normal, PMVector a_planeVectorX, PMVector a_planeVectorY);

    void UpdateFingers(std::map<UInt32, PMVector>& a_data);

protected:
    WindowInterface(GameObject* a_object);
    virtual ~WindowInterface();

    enum WindowSetting
    {
        ATTACH_DISTANCE_SQUARE              = 100,
        NEAR_DISTANCE_SQUARE                = 200,
        FINGER_CLICK_DISTANCE_SQUARE        = 300,
    };

    enum WindowState
    {
        WINDOW_STATE_NORMAL,
        WINDOW_STATE_RESIZING,
        WINDOW_STATE_SCALING,
    };

    static UInt32 m_idNow;
    UInt32 m_id;
    UInt32 m_resizeFingerID;
    WindowState m_state;

    float m_width;
    float m_height;

    GameObject* m_object;

    PMVector m_normal;
    PMVector m_planeVectorX;
    PMVector m_planeVectorY;

    typedef std::map<UInt32, std::pair<float, float>> WindowFingerData;
    WindowFingerData m_lastFingerData;
};