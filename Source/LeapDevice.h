#pragma once
#include "PMCommon.h"

namespace Leap
{
    class Controller;
    class Frame;
    class Hand;
}

class LeapDevice
{
public:
    LeapDevice();
    ~LeapDevice();

    void LeapUpdate();
    void TrackHeadPosition();

private:
    void UpdateHead(const Leap::Frame &a_frame);
    void UpdateHands(const Leap::Frame &a_frame);

    void InterAction(const Leap::Hand &a_hand);

    Leap::Controller* m_controller;

    UInt32 m_headID;
};