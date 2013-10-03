#pragma once

#include "WindowInterface.h"

class HandObject;

class MainWindowInterface : public WindowInterface
{
public:

    virtual void Resize(float &a_width, float &a_height);
    virtual void ChangeRange(float &a_range);

    void UpdateHands(std::map<UInt32, HandObject*>& a_data);

    virtual void SetNormalDirection(PMVector a_normal, PMVector a_planeVectorX, PMVector a_planeVectorY);

protected:
    MainWindowInterface(GameObject* a_object, GameObject* a_ui);
    virtual ~MainWindowInterface();

    enum WindowState
    {
        WINDOW_STATE_NORMAL,
        WINDOW_STATE_MOVE,
        WINDOW_STATE_RESIZING,
        WINDOW_STATE_SCALING,
    };

    virtual void InterAction(CursorData &a_data);
    void ChangeState(WindowState a_state);
    void UpdateState();
    void UpdateNormalState(CursorData &a_data);
    void UpdateResizeState(CursorData &a_data);
    void UpdateScaleState(CursorData &a_data);

    bool m_canResize;
    bool m_canScale;
    UInt32 m_releaseTime;
    UInt32 m_resizeCursor;
    std::set<UInt32> m_scalePoints;
    WindowState m_state;

    float m_scaleDistance;
    float m_lastscaleDistance;
    float m_range;
    float m_initRange;

    GameObject* m_ui;
    HandObject* m_attachedHand;

    PMVector m_lastHandPosition;
    PMVector m_lastHandRotation;

    std::vector<WindowInterface*> m_subWindows;
};