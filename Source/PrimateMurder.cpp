#include "PrimateMurder.h"

#include "GameWorld.h"
#include "GamePhysics.h"
#include "GameObjectFactory.h"
#include "LeapDevice.h"
//#include "COVRSceneManagerDecorator.h"
#include "AvatarObject.h"
#include "GameTime.h"

#include "OVRMachine.h"

#include <assert.h>

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
    delete m_physics;
    delete m_leap;
    m_smgr->drop();
}

bool PrimateMurder::OnEvent(const SEvent& event)
{
    bool result = false;

    if (event.EventType == irr::EET_KEY_INPUT_EVENT && event.KeyInput.PressedDown)
    {
        AvatarObject* avatar = sGameObjectFactory.GetAvatar();

        switch (event.KeyInput.Key) 
        {
        case irr::KEY_ESCAPE:    
            m_device->closeDevice();
            result = true;
            break;
        case KEY_KEY_W:
            {
                avatar->Forward();
            }
            break;
        case KEY_KEY_S:
            {
                avatar->Backward();
            }
            break;
        case KEY_KEY_A:
            {
                avatar->Leftward();
            }
            break;
        case KEY_KEY_D:
            {
                avatar->Rightward();
            }
            break;
        case KEY_KEY_X:
            {
                avatar->Downward();
            }
            break;
        case KEY_SPACE:
            {
                avatar->Upward();
            }
            break;
        case KEY_KEY_Q:
            {
                avatar->TurnLeft();
            }
            break;
        case KEY_KEY_E:
            {
                avatar->TurnRight();
            }
            break;
        case KEY_KEY_T:
            m_leap->TrackHeadPosition();
            break;
        case KEY_KEY_H:
            m_leap->AdjustByHand();
            break;
        case KEY_KEY_1:
            sOVRMachine.Test(0.1, 0, 0);
            break;
        case KEY_KEY_2:
            sOVRMachine.Test(-0.1, 0, 0);
            break;
        case KEY_KEY_3:
            sOVRMachine.Test(0, 0.1, 0);
            break;
        case KEY_KEY_4:
            sOVRMachine.Test(0, -0.1, 0);
            break;
        case KEY_KEY_5:
            sOVRMachine.Test(0, 0, 0.1);
            break;
        case KEY_KEY_6:
            sOVRMachine.Test(0, 0, -0.1);
            break;
        }
    }

    if (event.EventType == irr::EET_MOUSE_INPUT_EVENT && event.MouseInput.Event == EMIE_MOUSE_MOVED)
    {
        if (!sOVRMachine.IsRiftSceneMode())
        {
            PMVector rotation;

            rotation.y = float(event.MouseInput.X - m_lastMouseX) / CENTER_X * 180;
            rotation.x = float(event.MouseInput.Y - m_lastMouseY) / CENTER_Y * 180;

            m_lastMouseX = event.MouseInput.X;
            m_lastMouseY = event.MouseInput.Y;

            AvatarObject* avatar = sGameObjectFactory.GetAvatar();
            avatar->AppendLookRotate(rotation);
        }
    }

    // passing down the event
    return result;
}

void PrimateMurder::Init()
{
    m_physics = new GamePhysics();
    m_leap = new LeapDevice();
    m_leap->Init();

    SIrrlichtCreationParameters params;
    params.DriverType = EDT_OPENGL;
    params.WindowSize = dimension2d<u32>(SCREEN_WIDTH, SCREEN_HEIGHT);
    params.Fullscreen = true;
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

    if (sOVRMachine.IsRiftSceneMode())
    {
        sOVRMachine.Init(m_device);
    }

    InitTexture();

    sGameObjectFactory.FactoryInit(m_smgr, m_driver);
    sGameTime.Init(m_device->getTimer());
    sGameWorld.WorldInit();

    SetupWorld();
}

void PrimateMurder::Run()
{
    int lastFPS = -1;
    int needUpdatePhysics = 1000 / 60;
    PMTypeDefine::UInt32 lastUpdateTime = 0;

    while (m_device->run())
    {
        if (m_device->isWindowActive())
        {
            PMTypeDefine::UInt32 current = m_device->getTimer()->getTime();

            m_device->getCursorControl()->setPosition(CENTER_X, CENTER_Y);
            m_lastMouseX = CENTER_X;
            m_lastMouseY = CENTER_Y;

            m_leap->LeapUpdate();
            sGameWorld.WorldUpdate();

            if (current - lastUpdateTime > needUpdatePhysics)
            {
                m_physics->PhysicsUpdate();
                lastUpdateTime = current;
            }

            if (sOVRMachine.IsRiftSceneMode())
            {
                sOVRMachine.Draw();
            }
            else
            {
                m_driver->beginScene(true, true, SColor(255, 100, 100, 100));
                m_smgr->drawAll();
                m_env->drawAll();
                m_driver->endScene();
            }

            int fps = m_driver->getFPS();

            if (lastFPS != fps)
            {
                core::stringw str = L"FPS: ";
                str += fps;

                m_device->setWindowCaption(str.c_str());
                lastFPS = fps;
            }
        }
        else
        {
            m_device->yield();
        }
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
    tree->setRotation(vector3df(0, 120, 0));

    ISceneNode* ground = m_smgr->addSphereSceneNode(10000, 256);
    ground->setPosition(core::vector3df(0, -10000, 0));
    ground->setRotation(vector3df(90, 0, 0));
    video::SMaterial &m = ground->getMaterial(0);
    m.EmissiveColor = video::SColor(255, 0, 255, 0);

    m_light = m_smgr->addLightSceneNode(0, 
                                        vector3df(-450, 1000, -750), 
                                        SColorf(0.3f, 1.0f, 1.0f, 1.0f), 
                                        2500);

    //m_smgr->addBillboardTextSceneNode(m_device->getGUIEnvironment()->getBuiltInFont(), L"Clear", 0, dimension2df(100, 100), vector3df(0, 100, 500));
//     ITextSceneNode* text = m_smgr->addTextSceneNode(m_device->getGUIEnvironment()->getBuiltInFont(), L"Clear", SColor(255, 0, 0, 0), 0, vector3df(0, 100, 500));
//     text->setScale(vector3df(5, 5, 5));
}

void PrimateMurder::InitTexture()
{
    GameObject::SetTexture(TEXTURE_NONE, 0);
    GameObject::SetTexture(TEXTURE_WIND, m_driver->getTexture("Resource/wind.png"));
    GameObject::SetTexture(TEXTURE_PLANE_SHADOW, m_driver->getTexture("Resource/shadow.png"));
    GameObject::SetTexture(TEXTURE_PLANE_CURSOR, m_driver->getTexture("Resource/cursor.png"));
    GameObject::SetTexture(TEXTURE_MOVE, m_driver->getTexture("Resource/Move.png"));
    GameObject::SetTexture(TEXTURE_RESIZE, m_driver->getTexture("Resource/Resize.png"));
    GameObject::SetTexture(TEXTURE_SCALE, m_driver->getTexture("Resource/Scale.png"));
    GameObject::SetTexture(TEXTURE_CLOSE_NORMAL, m_driver->getTexture("Resource/CloseNormal.png"));
    GameObject::SetTexture(TEXTURE_CLOSE_HOVER, m_driver->getTexture("Resource/CloseHover.png"));
    GameObject::SetTexture(TEXTURE_CLOSE_PRESS, m_driver->getTexture("Resource/ClosePress.png"));
    GameObject::SetTexture(TEXTURE_CLOSE_DISABLE, m_driver->getTexture("Resource/CloseDisable.png"));
    GameObject::SetTexture(TEXTURE_START_NORMAL, m_driver->getTexture("Resource/StartNormal.png"));
    GameObject::SetTexture(TEXTURE_START_HOVER, m_driver->getTexture("Resource/StartHover.png"));
    GameObject::SetTexture(TEXTURE_START_PRESS, m_driver->getTexture("Resource/StartPress.png"));
    GameObject::SetTexture(TEXTURE_END_NORMAL, m_driver->getTexture("Resource/EndNormal.png"));
    GameObject::SetTexture(TEXTURE_END_HOVER, m_driver->getTexture("Resource/EndHover.png"));
    GameObject::SetTexture(TEXTURE_END_PRESS, m_driver->getTexture("Resource/EndPress.png"));
    GameObject::SetTexture(TEXTURE_COMIC_NORMAL, m_driver->getTexture("Resource/ComicNormal.png"));
    GameObject::SetTexture(TEXTURE_COMIC_HOVER, m_driver->getTexture("Resource/ComicHover.png"));
    GameObject::SetTexture(TEXTURE_COMIC_PRESS, m_driver->getTexture("Resource/ComicPress.png"));
    GameObject::SetTexture(TEXTURE_MENU_BG, m_driver->getTexture("Resource/MenuBG.png"));
    GameObject::SetTexture(TEXTURE_NEXT_NORMAL, m_driver->getTexture("Resource/NextNormal.png"));
    GameObject::SetTexture(TEXTURE_NEXT_HOVER, m_driver->getTexture("Resource/NextHover.png"));
    GameObject::SetTexture(TEXTURE_NEXT_PRESS, m_driver->getTexture("Resource/NextPress.png"));
    GameObject::SetTexture(TEXTURE_NEXT_DISABLE, m_driver->getTexture("Resource/NextDisable.png"));
    GameObject::SetTexture(TEXTURE_LAST_NORMAL, m_driver->getTexture("Resource/LastNormal.png"));
    GameObject::SetTexture(TEXTURE_LAST_HOVER, m_driver->getTexture("Resource/LastHover.png"));
    GameObject::SetTexture(TEXTURE_LAST_PRESS, m_driver->getTexture("Resource/LastPress.png"));
    GameObject::SetTexture(TEXTURE_LAST_DISABLE, m_driver->getTexture("Resource/LastDisable.png"));
}
