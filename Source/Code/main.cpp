#include <irrlicht.h>
#include <map>
#include "Leap.h"


using namespace Leap;

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


class MyEventReceiver : public IEventReceiver
{
public:
    // This is the one method that we have to implement
    virtual bool OnEvent(const SEvent& event)
    {
        // Remember whether each key is down or up
        if (event.EventType == irr::EET_KEY_INPUT_EVENT)
            KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

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

    Controller controller;

    MyEventReceiver receiver;

    IrrlichtDevice *device =
        createDevice( video::EDT_OPENGL, dimension2d<u32>(1280, 800), 16,
        false, false, false, &receiver);

    if (!device)
        return 1;

    IVideoDriver* driver = device->getVideoDriver();
    ISceneManager* smgr = device->getSceneManager();
    IGUIEnvironment* guienv = device->getGUIEnvironment();

    scene::ISceneNode* o = smgr->addSphereSceneNode(20);
    //o->setMaterialFlag(video::EMF_LIGHTING, false);
    o->setPosition(core::vector3df(0,0,0));
    SMaterial &m = o->getMaterial(0);
    m.EmissiveColor = SColor(255, 255, 0, 0);


    vector3df sssPos(0, 60, 0);

    scene::ISceneNode* sss = smgr->addCubeSceneNode(30);
    sss->setMaterialFlag(video::EMF_LIGHTING, true);
    sss->setMaterialTexture(0, driver->getTexture("Resource/wall.bmp"));
    sss->setPosition(sssPos);
    SMaterial &sssm = sss->getMaterial(0);
    sssm.EmissiveColor = SColor(100, 255, 255, 255);


    scene::ISceneNode*  hp = smgr->addSphereSceneNode(10);
    SMaterial &hpm = hp->getMaterial(0);
    hpm.EmissiveColor = SColor(255, 0, 0, 255);
    scene::ISceneNode*  hd = smgr->addSphereSceneNode(10);
    SMaterial &hdm = hd->getMaterial(0);
    hdm.EmissiveColor = SColor(255, 0, 0, 100);

    std::map<int32_t, Vector> touchs;

    scene::ISceneNode* node[10];
    for (int i = 0; i < 10; ++i)
    {
        node[i] = smgr->addSphereSceneNode(5);
        //node[i]->setMaterialFlag(video::EMF_LIGHTING, false);
        node[i]->setVisible(false);
        SMaterial &m = node[i]->getMaterial(0);
        m.EmissiveColor = SColor(255, 255, 0, 0);
    }

    scene::ICameraSceneNode* cam = smgr->addCameraSceneNodeFPS();

    vector3df camPos(0,0,200);
    vector3df camshift(0,0,0);

    cam->setPosition(camPos);
    cam->setTarget(core::vector3df(0,0,0));

    u32 then = device->getTimer()->getTime();
    const f32 MOVEMENT_SPEED = 50.f;

    int32_t handID = 0;

    irr::video::S3DVertex m_cPlaneVertices[4];
    irr::u16 m_iPlaneIndices[6];

    m_cPlaneVertices[0] = irr::video::S3DVertex(-100.0f, -100.0f, -100.0f,1,1,0, irr::video::SColor(255,255,255,255), 0.0f, 0.0f);
    m_cPlaneVertices[1] = irr::video::S3DVertex(-100.0f,  100.0f, -100.0f,1,1,0, irr::video::SColor(255,255,255,255), 0.0f, 1.0f);
    m_cPlaneVertices[2] = irr::video::S3DVertex( 100.0f,  100.0f, -100.0f,1,1,0, irr::video::SColor(255,255,255,255), 1.0f, 1.0f);
    m_cPlaneVertices[3] = irr::video::S3DVertex( 100.0f, -100.0f, -100.0f,1,1,0, irr::video::SColor(255,255,255,255), 1.0f, 0.0f);

    m_iPlaneIndices[0] = 0;
    m_iPlaneIndices[1] = 2;
    m_iPlaneIndices[2] = 1;
    m_iPlaneIndices[3] = 0;
    m_iPlaneIndices[4] = 3;
    m_iPlaneIndices[5] = 2;


    SMaterial m_cRenderMaterial;
    m_cRenderMaterial.Wireframe = false;
    m_cRenderMaterial.Lighting = true;
    m_cRenderMaterial.TextureLayer[0].TextureWrapU = irr::video::ETC_CLAMP;
    m_cRenderMaterial.TextureLayer[0].TextureWrapV = irr::video::ETC_CLAMP;
    m_cRenderMaterial.EmissiveColor = SColor(255, 100, 100, 100);

    while(device->run())
    {
        const u32 now = device->getTimer()->getTime();
        const f32 frameDeltaTime = (f32)(now - then) / 1000.f; // Time in seconds
        then = now;

        if(receiver.IsKeyDown(irr::KEY_KEY_W))
            camshift.Z -= MOVEMENT_SPEED * frameDeltaTime;
        else if(receiver.IsKeyDown(irr::KEY_KEY_S))
            camshift.Z += MOVEMENT_SPEED * frameDeltaTime;

        if(receiver.IsKeyDown(irr::KEY_KEY_A))
            camshift.X -= MOVEMENT_SPEED * frameDeltaTime;
        else if(receiver.IsKeyDown(irr::KEY_KEY_D))
            camshift.X += MOVEMENT_SPEED * frameDeltaTime;

        if(receiver.IsKeyDown(irr::KEY_KEY_X))
            camshift.Y -= MOVEMENT_SPEED * frameDeltaTime;
        else if(receiver.IsKeyDown(irr::KEY_SPACE))
            camshift.Y += MOVEMENT_SPEED * frameDeltaTime;

        cam->setPosition(camPos + camshift);


        driver->beginScene(true, true, SColor(255,0,0,0));

        //driver->setTransform(video::ETS_VIEW, core::matrix4());
        driver->setTransform(video::ETS_WORLD, core::matrix4());
        //driver->setTransform(video::ETS_PROJECTION, core::matrix4());

        driver->setMaterial(m_cRenderMaterial);
        driver->drawIndexedTriangleList(m_cPlaneVertices, 4, m_iPlaneIndices, 2);

        for (int i = 0; i < 10; ++i)
        {
            node[i]->setVisible(false);
            //node[i]->setPosition(core::vector3df(0,-10,i * 10));
        }

        //         listener.lock = true;
        // 
        //         std::list<Vector> tmp = listener.tips;
        // 
        //         int fingers = 0;
        //         for (std::list<Vector>::iterator iter = tmp.begin();
        //             iter != tmp.end() && fingers < 10;
        //             ++iter, ++fingers)
        //         {
        //             Vector& v = *iter;
        // 
        //             node[fingers]->setVisible(true);
        //             node[fingers]->setPosition(vector3df(v.x, v.y, v.z));
        //         }
        // 
        //         listener.lock = false;

        if (controller.isConnected())
        {
            const Frame frame = controller.frame();
            if (!frame.hands().isEmpty()) {
                // Get the first hand

                int count = 0;

                std::map<int32_t, Vector> tmptouchs;

                for (int i = 0; i < frame.hands().count(); ++i)
                {
                    const Hand hand = frame.hands()[i];
                    const FingerList fingers = hand.fingers();
                    for (int j = 0; j < fingers.count(); ++j)
                    {
                        if (count < 10)
                        {
                            Vector v = fingers[j].tipPosition();
                            node[count]->setVisible(true);
                            node[count]->setPosition(vector3df(-v.x, v.y, v.z));

                            float d = fingers[j].touchDistance();

                            if (count == 0)
                            {
                                core::stringw str = L"D ";
                                str += d;

                                device->setWindowCaption(str.c_str());
                            }

                            vector3df tmpPos = node[count]->getPosition();

                            f32 dis = tmpPos.getDistanceFrom(sssPos);


                            SMaterial &m = node[count]->getMaterial(0);
                            if (dis > 40)
                            {
                                m.EmissiveColor = SColor(255, 255, 255, 255);
                            }
                            else if (dis > 20)
                            {
                                m.EmissiveColor = SColor(255, 0, 255, 0);
                            }
                            else
                            {
                                m.EmissiveColor = SColor(255, 255, 0, 0);

                                tmptouchs[fingers[j].id()] = fingers[j].tipPosition();
                            }

                            ++count;

                        }
                    }

                    //                     if (touchs.size() > 1)
                    //                     {
                    //                         std::map<int32_t, Vector>::iterator iter = touchs.begin();
                    // 
                    //                         int32_t id1 = iter->first;
                    //                         ++iter;
                    //                         int32_t id2 = iter->first;
                    // 
                    // 
                    //                         if (tmptouchs.find(id1) != tmptouchs.end() && tmptouchs.find(id2) != tmptouchs.end())
                    //                         {
                    //                             Vector v1 = tmptouchs[id1];
                    //                             Vector v2 = tmptouchs[id2];
                    // 
                    //                             float vdis = v1.distanceTo(v2);
                    // 
                    //                             if (vdis < 40)
                    //                             {
                    //                                 sss->setPosition(vector3df((-v1.x -v2.x) / 2, (v1.y + v2.y) / 2, (v1.z + v1.z) / 2));
                    //                                 sssPos = sss->getPosition();
                    //                             }
                    //                         }
                    // 
                    //                     }
                    // 
                    //                     touchs.clear();
                    //                     touchs = tmptouchs;



                    Leap::Vector position = hand.palmPosition();

                    hp->setPosition(vector3df(-position.x, position.y, position.z));
                    Leap::Vector direction = hand.direction();
                    hd->setPosition(vector3df(-position.x - direction.x, position.y + direction.y, position.z + direction.z));


                    if (handID == 0)
                    {
                        f32 hdis = sssPos.getDistanceFrom(hp->getPosition());
                        if (hdis < 20)
                        {
                            handID = hand.id();
                        }
                    }
                    else
                    {
                        if (hand.fingers().count() > 0)
                        {
                            sss->setPosition(hp->getPosition());
                            sssPos = sss->getPosition();
                        }
                        else
                        {
                            handID = 0;
                        }
                    }

                    if (handID != 0)
                    {
                        Leap::Vector ro = direction.normalized();

                        ro.x *= RADTODEG;
                        ro.y *= -RADTODEG;
                        ro.z *= -RADTODEG;

                        sss->setRotation(vector3df(ro.x, ro.y, ro.z));
                    }
                }
            }
        }



        smgr->drawAll();

        driver->endScene();
    }

    //controller.removeListener(listener);
    device->drop();

    return 0;
}

/*
That's it. Compile and run.
**/
