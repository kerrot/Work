#include "LeapDevice.h"
#include "Leap.h"
#include "GameObjectFactory.h"
#include "HandObject.h"
#include "AvatarObject.h"
#include "WindowInterface.h"
#include "PMType.h"
#include <stdio.h>

using namespace Leap;

LeapDevice::LeapDevice()
:
m_headID(0)
{
    m_controller = new Controller();
}

LeapDevice::~LeapDevice()
{
    m_controller = 0;
}

void LeapDevice::LeapUpdate()
{
    sGameObjectFactory.HideAllHand();

    if (m_controller->isConnected())
    {
        const Frame frame = m_controller->frame();

        //UpdateHead(frame);

        UpdateHands(frame);
    }
}

void LeapDevice::TrackHeadPosition()
{
    if (m_controller->isConnected())
    {
        const Frame frame = m_controller->frame();

        PointableList pL = frame.pointables();
        if (pL.count() == 1)
        {
            Pointable p = pL[0];
            if (!p.isFinger())
            {
                //m_headID = (UInt32)p.id();
                float length = p.length();
                Vector direction = p.direction();
                float k = sqrt(length * length / (direction.magnitudeSquared()));
                PMVector position(p.tipPosition().x + direction.x * k, p.tipPosition().y + direction.y * k, p.tipPosition().z + direction.z * k);

                AvatarObject* avatar = sGameObjectFactory.GetAvatar();
                avatar->SetHeadPosition(position);
            }
            else
            {
                printf("Not finger.\n");
            }
        }
        else
        {
            printf("pL Count: %d\n", pL.count());
        }
    }
    else
    {
        printf("No LeapDevice\n");
    }
}

void LeapDevice::UpdateHead( const Frame &a_frame )
{
    if (m_headID != 0)
    {
        PointableList pL = a_frame.pointables();
        for (int i = 0; i < pL.count(); ++i)
        {
            Pointable p = pL[i];
            if (p.id() == m_headID)
            {
                float length = p.length();
                Vector direction = p.direction();
                float k = sqrt(length * length / (direction.magnitudeSquared()));
                PMVector position(p.tipPosition().x + direction.x * k, p.tipPosition().y + direction.y * k, p.tipPosition().z + direction.z * k);

                AvatarObject* avatar = sGameObjectFactory.GetAvatar();
                avatar->SetHeadPosition(position);
                return;
            }
        }
        m_headID = 0;
    }
    else
    {
        printf("head lost\n");
    }
}

void LeapDevice::UpdateHands( const Leap::Frame &a_frame )
{
    if (!a_frame.hands().isEmpty())
    {
        std::map<UInt32, PMVector> fingerData;

        for (int i = 0; i < a_frame.hands().count(); ++i)
        {
            const Hand hand = a_frame.hands()[i];
            
            HandObject* handObject = sGameObjectFactory.GetorCreateHand((UInt32)hand.id());
            handObject->SetVisible(true);
            Vector palmPos = hand.palmPosition();
            handObject->SetPosition(PMVector(palmPos.x, palmPos.y, -palmPos.z));

            float rotationX = - hand.palmNormal().pitch() - PI / 2;
            float rotationY = hand.direction().yaw();
            float rotationZ = hand.palmNormal().roll();

            //handObject->SetNormalDirection(PMVector(-palmNormal.x, -palmNormal.y, palmNormal.z), PMVector(palmDirection.x, palmDirection.y, -palmDirection.z));
            handObject->SetRotation(PMVector(rotationX / PI * 180, rotationY / PI * 180, rotationZ / PI * 180));
            
            const FingerList fingers = hand.fingers();

            InterAction(hand, handObject);
            
            for (int j = 0; j < MAX_FINGERS; ++j)
            {
                if (j < fingers.count())
                {
                    const Finger finger = fingers[j];
                    Vector tipPos = finger.tipPosition();
                    handObject->SetFingerPosition(j, PMVector(tipPos.x, tipPos.y, -tipPos.z));

                    fingerData[finger.id()] = handObject->GetFingerAbsolutePosition(j);
                }
                else
                {
                    handObject->HideFingler(j);
                }
            }
        }

        std::map<UInt32, WindowInterface*>& windows = sGameObjectFactory.GetWindows();
        for (std::map<UInt32, WindowInterface*>::iterator iter = windows.begin();
            iter != windows.end();
            ++iter)
        {
            WindowInterface* window = iter->second;
            window->UpdateFingers(fingerData);
        }
    }
}

void LeapDevice::InterAction(const Leap::Hand &a_hand, HandObject *a_handObject)
{
    if (a_handObject->GetAttachedWindow())
    {
        if (a_hand.fingers().count() == 0)
        {
            a_handObject->AttachWindow(0);
        }
    }
    else if (a_hand.fingers().count() > 0)
    {
        std::map<UInt32, WindowInterface*>& windows = sGameObjectFactory.GetWindows();
        for (std::map<UInt32, WindowInterface*>::iterator iter = windows.begin();
            iter != windows.end();
            ++iter)
        {
            WindowInterface* window = iter->second;
            if (window->PointToWindowDistanceType(a_handObject->GetAbsolutePosition()) == WINDOW_DISTANCE_ATTACH)
            {
                a_handObject->AttachWindow(window);
                return;
            }
        }
    }
}
