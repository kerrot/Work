#pragma once

namespace irr
{
    class IrrlichtDevice;
    
    namespace video
    {
        class IVideoDriver;
    }

    namespace scene
    {
        class ISceneManager;
    }

    namespace gui
    {
        class IGUIEnvironment;
    }
}

class GameWorld;
class GamePhysics;
class LeapDevice;

class PrimateMurder
{
public:
    PrimateMurder();
    ~PrimateMurder();

    
    void Run();

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
};