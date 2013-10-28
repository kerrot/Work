#pragma once

#include "PMCommon.h"
#include "irrlicht.h"

using namespace irr;
using namespace scene;
using namespace video;
using namespace gui;
using namespace core;

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

    GamePhysics* m_physics;
    LeapDevice* m_leap;

    IrrlichtDevice* m_device;
    IVideoDriver* m_driver;
    ISceneManager* m_smgr;
    gui::IGUIEnvironment* m_env;

    ILightSceneNode* m_light;

    Int32 m_lastMouseX;
    Int32 m_lastMouseY;

    bool KeyIsDown[KEY_KEY_CODES_COUNT];
};
