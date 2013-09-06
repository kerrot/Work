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

bool PrimateMurder::OnEvent(const SEvent& event)
{
    bool result = false;

    if (event.EventType == irr::EET_KEY_INPUT_EVENT && event.KeyInput.PressedDown)
    {
        switch (event.KeyInput.Key) {
            case irr::KEY_ESCAPE:
                m_device->closeDevice();
                result = true;
                break;
        }
    }

    // passing down the event
    return result || m_world->OnEvent(event);
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
    params.EventReceiver = this;
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

    SetupWorld();
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

void PrimateMurder::SetupWorld()
{
    m_driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);

    m_smgr->addSkyBoxSceneNode(
        m_driver->getTexture("Resource/irrlicht2_up.jpg"),
        m_driver->getTexture("Resource/irrlicht2_dn.jpg"),
        m_driver->getTexture("Resource/irrlicht2_lf.jpg"),
        m_driver->getTexture("Resource/irrlicht2_rt.jpg"),
        m_driver->getTexture("Resource/irrlicht2_ft.jpg"),
        m_driver->getTexture("Resource/irrlicht2_bk.jpg"));

    m_driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);

    IAnimatedMesh* mesh = m_smgr->getMesh("Resource/maple.obj");
    IAnimatedMeshSceneNode* tree = m_smgr->addAnimatedMeshSceneNode(mesh);
    if (tree)
    {
        SMaterial &treem = tree->getMaterial(6);
        treem.BackfaceCulling = false;
    }

    ISceneNode* ground = m_smgr->addSphereSceneNode(1000, 256);
    ground->setPosition(core::vector3df(0, -1000, 0));
    video::SMaterial &m = ground->getMaterial(0);
    m.EmissiveColor = video::SColor(255, 0, 255, 0);

    scene::ILightSceneNode* light = m_smgr->addLightSceneNode(0, 
                                                    vector3df(0, 1100, 0), 
                                                    SColorf(0.3f, 1.0f, 1.0f, 0.0f), 
                                                    1000);

}
