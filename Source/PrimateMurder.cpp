#include "PrimateMurder.h"
#include "irrlicht.h"
#include "GameWorld.h"
#include "GamePhysics.h"
#include "GameObjectFactory.h"
#include "LeapDevice.h"
#include "COVRSceneManagerDecorator.h"
#include "AvatarObject.h"
#include "GameTime.h"

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
    CENTER_X                    = 640,
    CENTER_Y                    = 400,
};

PrimateMurder::PrimateMurder()
:
m_lastMouseX(CENTER_X)
,m_lastMouseY(CENTER_Y)
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
        switch (event.KeyInput.Key) 
        {
        case irr::KEY_ESCAPE:    
            m_device->closeDevice();
            result = true;
            break;
        case KEY_KEY_W:
            {
                AvatarObject* avatar = sGameObjectFactory.GetAvatar();
                avatar->Forward();
            }
            break;
        case KEY_KEY_S:
            {
                AvatarObject* avatar = sGameObjectFactory.GetAvatar();
                avatar->Backward();
            }
            break;
        case KEY_KEY_A:
            {
                AvatarObject* avatar = sGameObjectFactory.GetAvatar();
                avatar->Leftward();
            }
            break;
        case KEY_KEY_D:
            {
                AvatarObject* avatar = sGameObjectFactory.GetAvatar();
                avatar->Rightward();
            }
            break;
        case KEY_KEY_X:
            {
                AvatarObject* avatar = sGameObjectFactory.GetAvatar();
                avatar->Downward();
            }
            break;
        case KEY_SPACE:
            {
                AvatarObject* avatar = sGameObjectFactory.GetAvatar();
                avatar->Upward();
            }
            break;
        case KEY_KEY_Q:
            {
                AvatarObject* avatar = sGameObjectFactory.GetAvatar();
                avatar->TurnLeft();
            }
            break;
        case KEY_KEY_E:
            {
                AvatarObject* avatar = sGameObjectFactory.GetAvatar();
                avatar->TurnRight();
            }
            break;
        case KEY_KEY_T:
            m_leap->TrackHeadPosition();
            break;
        }
    }

    if (event.EventType == irr::EET_MOUSE_INPUT_EVENT && event.MouseInput.Event == EMIE_MOUSE_MOVED)
    {
        PMVector rotation;

        rotation.y = float(event.MouseInput.X - m_lastMouseX) / CENTER_X * 180;
        rotation.x = float(event.MouseInput.Y - m_lastMouseY) / CENTER_Y * 180;

        m_lastMouseX = event.MouseInput.X;
        m_lastMouseY = event.MouseInput.Y;

        AvatarObject* avatar = sGameObjectFactory.GetAvatar();
        avatar->LookRotate(rotation);
    }

    // passing down the event
    return result;
}

void PrimateMurder::Init()
{
    m_world = new GameWorld();
    m_physics = new GamePhysics();
    m_leap = new LeapDevice();
    m_leap->Init();

    SIrrlichtCreationParameters params;
    params.DriverType = EDT_OPENGL;
    params.WindowSize = dimension2d<u32>(SCREEN_WIDTH, SCREEN_HEIGHT);
    params.Fullscreen = false;
    params.EventReceiver = this;
    m_device = createDeviceEx(params);
    m_device->getCursorControl()->setVisible(false);

    if (!m_device)
    {
        assert("Device Init Error.");
        exit(0);
    }

    m_driver = m_device->getVideoDriver();
    m_smgr = m_device->getSceneManager();
    //m_smgr = new COVRSceneManagerDecorator(m_smgr);
    m_env = m_device->getGUIEnvironment();
    InitTexture();

    m_world->WorldInit();
    sGameObjectFactory.FactoryInit(m_smgr, m_driver);
    sGameTime.Init(m_device->getTimer());

    SetupWorld();
}

void PrimateMurder::Run()
{
    while (m_device->run())
    {
        m_device->getTimer();

        m_device->getCursorControl()->setPosition(CENTER_X, CENTER_Y);
        m_lastMouseX = CENTER_X;
        m_lastMouseY = CENTER_Y;

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
    tree->setMaterialFlag(EMF_BACK_FACE_CULLING, false);

    ISceneNode* ground = m_smgr->addSphereSceneNode(1000, 256);
    ground->setPosition(core::vector3df(0, -1000, 0));
    video::SMaterial &m = ground->getMaterial(0);
    m.EmissiveColor = video::SColor(255, 0, 255, 0);

    m_light = m_smgr->addLightSceneNode(0, 
                                        vector3df(0, 1100, 0), 
                                        SColorf(0.7f, 1.0f, 1.0f, 0.0f), 
                                        1000);
}

void PrimateMurder::InitTexture()
{
    GameObject::SetTexture(TEXTURE_NONE, 0);
    GameObject::SetTexture(TEXTURE_WIND, m_driver->getTexture("Resource/wind.jpg"));
    GameObject::SetTexture(TEXTURE_PLANE_SHADOW, m_driver->getTexture("Resource/s.png"));
    GameObject::SetTexture(TEXTURE_MOVE, m_driver->getTexture("Resource/Move.png"));
    GameObject::SetTexture(TEXTURE_RESIZE, m_driver->getTexture("Resource/Resize.png"));
}
