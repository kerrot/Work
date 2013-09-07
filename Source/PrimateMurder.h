#pragma once

#include <IEventReceiver.h>

namespace irr
{
    class IrrlichtDevice;
    struct SEvent;
    
    namespace video
    {
        class IVideoDriver;
    }

    namespace scene
    {
        class ISceneManager;
        class ILightSceneNode;
    }

    namespace gui
    {
        class IGUIEnvironment;
    }
}

class GameWorld;
class GamePhysics;
class LeapDevice;

class PrimateMurder : public irr::IEventReceiver
{
public:
    PrimateMurder();
    ~PrimateMurder();

    
    void Run();

    virtual bool OnEvent(const irr::SEvent& event);

private:
    void Init();
    void SetupWorld();

    GameWorld* m_world;
    GamePhysics* m_physics;
    LeapDevice* m_leap;

    irr::IrrlichtDevice* m_device;
    irr::video::IVideoDriver* m_driver;
    irr::scene::ISceneManager* m_smgr;
    irr::gui::IGUIEnvironment* m_env;

    irr::scene::ILightSceneNode* m_light;
};
