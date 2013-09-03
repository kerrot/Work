#include <irrlicht.h>
#include <stdio.h>

#include "COVRSceneManagerDecorator.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


IrrlichtDevice *device = 0;

class MyEventReceiver : public IEventReceiver
{
public:
    // This is the one method that we have to implement
    virtual bool OnEvent(const SEvent& event)
    {
        // Remember whether each key is down or up
        if (event.EventType == irr::EET_KEY_INPUT_EVENT)
            KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

        if (event.EventType == irr::EET_MOUSE_INPUT_EVENT)
        {
            switch(event.MouseInput.Event)
            {
            case EMIE_MOUSE_MOVED:
                //cursorPos.X = event.MouseInput.X;
                //cursorPos.Y = event.MouseInput.Y;
                break;

            default:
                // We won't use the wheel
                break;
            }
        }

        return false;
    }

    // This is used to check whether a key is being held down
    virtual bool IsKeyDown(EKEY_CODE keyCode) const
    {
        return KeyIsDown[keyCode];
    }

    MyEventReceiver()
    {
        for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
            KeyIsDown[i] = false;
    }

private:
    // We use this array to store the current state of each key
    bool KeyIsDown[KEY_KEY_CODES_COUNT];
};

int main()
{
    dimension2d<u32> resolution(1280, 800);
    MyEventReceiver receiver;

    device = createDevice(
        video::EDT_OPENGL,
        resolution, 16,
        true, false, false, &receiver
    );

    if (!device) return 1;

    IVideoDriver* driver = device->getVideoDriver();
    ISceneManager* smgr = device->getSceneManager();
    //smgr = new CSceneManagerDecorator(smgr);
    smgr = new COVRSceneManagerDecorator(smgr);
    IGUIEnvironment* guienv = device->getGUIEnvironment();

    device->getFileSystem()->addFileArchive("Resource/map-20kdm2.pk3");
    scene::IAnimatedMesh* mesh = smgr->getMesh("20kdm2.bsp");
    scene::ISceneNode* node = 0;

    if (mesh)
    {
        node = smgr->addOctreeSceneNode(mesh->getMesh(0), 0, -1, 1024);
        if (node)
        {
            node->setPosition(core::vector3df(-1300,-144,-1249));
        }
    }
    
    SKeyMap keyMap[9];
    keyMap[0].Action = EKA_MOVE_FORWARD;
    keyMap[0].KeyCode = KEY_UP;
    keyMap[1].Action = EKA_MOVE_FORWARD;
    keyMap[1].KeyCode = KEY_KEY_W;

    keyMap[2].Action = EKA_MOVE_BACKWARD;
    keyMap[2].KeyCode = KEY_DOWN;
    keyMap[3].Action = EKA_MOVE_BACKWARD;
    keyMap[3].KeyCode = KEY_KEY_S;

    keyMap[4].Action = EKA_STRAFE_LEFT;
    keyMap[4].KeyCode = KEY_LEFT;
    keyMap[5].Action = EKA_STRAFE_LEFT;
    keyMap[5].KeyCode = KEY_KEY_A;

    keyMap[6].Action = EKA_STRAFE_RIGHT;
    keyMap[6].KeyCode = KEY_RIGHT;
    keyMap[7].Action = EKA_STRAFE_RIGHT;
    keyMap[7].KeyCode = KEY_KEY_D;

    keyMap[8].Action = EKA_JUMP_UP;
    keyMap[8].KeyCode = KEY_KEY_J;

    ICameraSceneNode* Camera = smgr->addCameraSceneNodeFPS(0, 100.0f, .4f, -1, keyMap, 9, false, 3.f);
    ISceneNode* Head = smgr->addSphereSceneNode(2.0f, 16, Camera, -1, vector3df(0, 0, 20.0f));

    while(device->run())
    {
        if(receiver.IsKeyDown(irr::KEY_ESCAPE))
        {
            device->closeDevice();
            break;
        }

        driver->beginScene(true, true, SColor(0,100,100,100));
        smgr->drawAll();
        driver->endScene();
    }

    device->drop();

    return 0;
}
