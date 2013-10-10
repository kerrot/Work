#include "LeapDevice.h"
#include "Leap.h"
#include "GameObjectFactory.h"
#include "HandObject.h"
#include "AvatarObject.h"
#include "MainWindowInterFace.h"
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

        UpdateGesture(frame);
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
        std::map<UInt32, HandObject*> handData;
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

            handObject->SetRotation(PMVector(rotationX / PI * 180, rotationY / PI * 180, rotationZ / PI * 180));
            
            handData[(UInt32)hand.id()] = handObject;

            const FingerList fingers = hand.fingers();
            
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

        std::map<UInt32, MainWindowInterface*>& mainWindows = sGameObjectFactory.GetMainWindows();
        for (std::map<UInt32, MainWindowInterface*>::iterator iter = mainWindows.begin();
            iter != mainWindows.end();
            ++iter)
        {
            MainWindowInterface* window = iter->second;
            window->UpdateHands(handData);
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

void LeapDevice::Init()
{
    m_controller->enableGesture(Gesture::TYPE_SWIPE);
}

void LeapDevice::UpdateGesture( const Leap::Frame &a_frame )
{
    const GestureList gestures = a_frame.gestures();
    for (int g = 0; g < gestures.count(); ++g) 
    {
        Gesture gesture = gestures[g];
        if (gesture.type() == Gesture::TYPE_SWIPE)
        {
            SwipeGesture swipe = gesture;
            
            UInt32 id = swipe.id();
            Vector d = swipe.direction();
            Vector s = swipe.startPosition();
            float v = swipe.speed();

            id = id;

            if (swipe.direction().angleTo(Vector(0, -1, 0)) < 10.0f / 180.0f * PI)
            {
                if (swipe.startPosition().x > 0)
                {
                    sGameObjectFactory.GetMenuUI();
                    return;
                }
            }
        }
    }
}

void LeapDevice::AdjustByHand()
{
    if (m_controller->isConnected())
    {
        const Frame frame = m_controller->frame();
        if (frame.hands().count() == 1)
        {
            const Hand hand = frame.hands()[0];
            AvatarObject* avatar = sGameObjectFactory.GetAvatar();
            PMVector headPos = avatar->GetHeadPosition();
            float diff = headPos.y - hand.palmPosition().y;
            headPos.y = hand.palmPosition().y;
            avatar->SetHeadPosition(headPos);
            PMVector pos = avatar->GetAbsolutePosition();
            pos.y += diff;
            avatar->SetPosition(pos);
        }
    }
    else
    {
        printf("No LeapDevice\n");
    }
}
