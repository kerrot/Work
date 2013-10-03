#pragma once

#include "PMCommon.h"
#include "PMType.h"

#define STOP_DISTANCE_SQUARE 0.5

class GameObject;

class WindowInterface
{
public:
    PMVector GetProjectionPoint(PMVector a_point);
    PMVector TransformByCoordinateSqure(PMVector a_point);

    virtual void Resize(float &a_width, float &a_height);

    UInt32 GetId();

    GameObject* GetObject();

    virtual void SetNormalDirection(PMVector a_normal, PMVector a_planeVectorX, PMVector a_planeVectorY);

    void UpdateFingers(std::map<UInt32, PMVector>& a_data);
protected:
    WindowInterface(GameObject* a_object);
    virtual ~WindowInterface();

    enum WindowSetting
    {
        ATTACH_DISTANCE                     = 10,
        ATTACH_DISTANCE_SQUARE              = 100,
        NEAR_DISTANCE                       = 20,
        NEAR_DISTANCE_SQUARE                = 400,
        FINGER_CLICK_DISTANCE_SQUARE        = 300,
        FINGER_CLICK_TIME                   = 1500,
        CORNER_ATTACH_DISTANCE_SQUARE       = 100,
        SUBWINDOW_DISTANCE                  = 15,
    };

    struct WindowFingerData
    {
        PMVector oriPos;
        PMVector distance;
    };

    struct CursorData
    {
        CursorData()
        :
        cursorID(0)
        {

        }

        UInt32 cursorID;
        PMVector coordinate;

        std::map<UInt32, WindowFingerData> fingerData;
    };

    void HideAllShadows();
    void UpdateShadow(CursorData& a_data);
    virtual void InterAction(CursorData &a_data);

    static UInt32 m_idNow;
    UInt32 m_id;

    float m_width;
    float m_height;

    GameObject* m_object;

    PMVector m_normal;
    PMVector m_planeVectorX;
    PMVector m_planeVectorY;

    CursorData m_lastData;

    std::vector<GameObject*> m_shadows;
};