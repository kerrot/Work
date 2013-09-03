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


int main()
{
  
    Controller controller;


	IrrlichtDevice *device =
		createDevice( video::EDT_OPENGL, dimension2d<u32>(1280, 800), 16,
			false, false, false, 0);

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
    sss->setMaterialTexture(0, driver->getTexture("wall.bmp"));
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
    cam->setPosition(core::vector3df(0,200,200));
    cam->setTarget(core::vector3df(0,20,0));
    

    int32_t handID = 0;
    
	while(device->run())
	{
		driver->beginScene(true, true, SColor(255,0,0,0));

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
