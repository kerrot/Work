#pragma once
#include "PMCommon.h"

namespace Leap
{
    class Controller;
    class Frame;
}

class LeapDevice
{
public:
    LeapDevice();
    ~LeapDevice();

    void Init();
    void LeapUpdate();
    void TrackHeadPosition();
    void AdjustByHand();

private:
    void UpdateHead(const Leap::Frame &a_frame);
    void UpdateHands(const Leap::Frame &a_frame);
    void UpdateGesture(const Leap::Frame &a_frame);

    Leap::Controller* m_controller;

    UInt32 m_headID;
};