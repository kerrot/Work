#pragma once

namespace Leap
{
    class Controller;
}

class LeapDevice
{
public:
    LeapDevice();
    ~LeapDevice();

    void LeapUpdate();

private:
    Leap::Controller* m_controller;
};