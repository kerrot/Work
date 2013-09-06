#include "LeapDevice.h"
#include "Leap.h"

using namespace Leap;

LeapDevice::LeapDevice()
{
    m_controller = new Controller();
}

LeapDevice::~LeapDevice()
{
    m_controller = 0;
}

void LeapDevice::LeapUpdate()
{
    if (m_controller->isConnected())
    {
        const Frame frame = m_controller->frame();
        if (!frame.hands().isEmpty()) 
        {
           
            for (int i = 0; i < frame.hands().count(); ++i)
            {
                const Hand hand = frame.hands()[i];
                const FingerList fingers = hand.fingers();
                for (int j = 0; j < fingers.count(); ++j)
                {
                    
                }

                Leap::Vector position = hand.palmPosition();
            }
        }
    }
}