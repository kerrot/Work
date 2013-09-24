#pragma once

#include "PMCommon.h"
#include <IEventReceiver.h>



namespace irr
{
    class IrrlichtDevice;
    struct SEvent;
    enum EKEY_CODE;

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

using namespace irr;

class GameWorld;
class GamePhysics;
class LeapDevice;

class PrimateMurder : public IEventReceiver
{
public:
    PrimateMurder();
    ~PrimateMurder();

    
    void Run();

    virtual bool OnEvent(const irr::SEvent& event);

private:
    void Init();
    void SetupWorld();
    void InitTexture();

    void OnKeyDown(EKEY_CODE a_key);
    void OnKeyUp(EKEY_CODE a_key);

    GameWorld* m_world;
    GamePhysics* m_physics;
    LeapDevice* m_leap;

    IrrlichtDevice* m_device;
    video::IVideoDriver* m_driver;
    scene::ISceneManager* m_smgr;
    gui::IGUIEnvironment* m_env;

    scene::ILightSceneNode* m_light;

    Int32 m_lastMouseX;
    Int32 m_lastMouseY;

    bool KeyIsDown[KEY_KEY_CODES_COUNT];
};
