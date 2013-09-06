#include "PrimateMurder.h"
#include "irrlicht.h"
#include "GameWorld.h"
#include "GamePhysics.h"
#include "GameObjectFactory.h"
#include "LeapDevice.h"
#include "COVRSceneManagerDecorator.h"

#include <assert.h>

using namespace irr;

using namespace scene;
using namespace video;
using namespace gui;
using namespace core;

enum DEVICE_SETTING
{
    SCREEN_WIDTH                = 1280,
    SCREEN_HEIGHT               = 800,
};

PrimateMurder::PrimateMurder()
{
    Init();
}

PrimateMurder::~PrimateMurder()
{
    delete m_world;
    delete m_physics;
    delete m_leap;
    m_smgr->drop();
}

void PrimateMurder::Init()
{
    m_world = new GameWorld();
    m_physics = new GamePhysics();
    m_leap = new LeapDevice();

    SIrrlichtCreationParameters params;
    params.DriverType = EDT_OPENGL;
    params.WindowSize = dimension2d<u32>(SCREEN_WIDTH, SCREEN_HEIGHT);
    params.Fullscreen = true;
    params.EventReceiver = m_world;
    m_device = createDeviceEx(params);

    if (!m_device)
    {
        assert("Device Init Error.");
        exit(0);
    }

    m_driver = m_device->getVideoDriver();
    //m_smgr = new COVRSceneManagerDecorator(m_device->getSceneManager());
    m_smgr = m_device->getSceneManager();
    m_env = m_device->getGUIEnvironment();

    m_world->WorldInit();
    sGameObjectFactory->FactoryInit(m_smgr);

    m_driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);

    m_smgr->addSkyBoxSceneNode(
        m_driver->getTexture("Resource/irrlicht2_up.jpg"),
        m_driver->getTexture("Resource/irrlicht2_dn.jpg"),
        m_driver->getTexture("Resource/irrlicht2_lf.jpg"),
        m_driver->getTexture("Resource/irrlicht2_rt.jpg"),
        m_driver->getTexture("Resource/irrlicht2_ft.jpg"),
        m_driver->getTexture("Resource/irrlicht2_bk.jpg"));

    m_driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);
}

void PrimateMurder::Run()
{
    while (m_device->run())
    {
        m_leap->LeapUpdate();
        m_world->WorldUpdate();
        m_physics->PhysicsUpdate();

        m_driver->beginScene(true, true, SColor(255, 100, 100, 100));

        m_smgr->drawAll();
        m_env->drawAll();

        m_driver->endScene();
    }
}
