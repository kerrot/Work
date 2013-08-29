#include "opencv/cv.h"
#include "opencv/highgui.h"

#include <irrlicht.h>
#include <map>



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
    //sss->setMaterialTexture(0, driver->getTexture("wall.bmp"));
    sss->setPosition(sssPos);
    SMaterial &sssm = sss->getMaterial(0);
    sssm.EmissiveColor = SColor(100, 255, 255, 255);


    scene::ISceneNode*  hp = smgr->addSphereSceneNode(10);
    SMaterial &hpm = hp->getMaterial(0);
    hpm.EmissiveColor = SColor(255, 0, 0, 255);
    scene::ISceneNode*  hd = smgr->addSphereSceneNode(10);
    SMaterial &hdm = hd->getMaterial(0);
    hdm.EmissiveColor = SColor(255, 0, 0, 100);


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
    m_cRenderMaterial.Lighting = false;
    m_cRenderMaterial.TextureLayer[0].TextureWrapU = irr::video::ETC_CLAMP;
    m_cRenderMaterial.TextureLayer[0].TextureWrapV = irr::video::ETC_CLAMP;
    
    ITexture      *m_pRenderTexture = 0;

    CvCapture *capture;
    IplImage *frame;
    char AviFileName[]="Accel World.mp4";
    capture = cvCaptureFromAVI(AviFileName);

    if (driver->queryFeature(irr::video::EVDF_RENDER_TO_TARGET))
    {
        m_pRenderTexture = driver->addRenderTargetTexture(irr::core::dimension2d<irr::u32>((irr::u32)(720), (irr::u32)(480)));
        m_cRenderMaterial.setTexture(0, m_pRenderTexture);
    }

//     m_pRenderTexture = driver->getTexture("wall.bmp");
//     m_cRenderMaterial.setTexture(0, m_pRenderTexture);

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




        driver->setRenderTarget(m_pRenderTexture, true, true, irr::video::SColor(0,0,0,0));
        
        if(cvGrabFrame(capture))
        {
            frame=cvRetrieveFrame(capture);

            void* pBits = m_pRenderTexture->lock();
            u32  Pitch = m_pRenderTexture->getPitch();  

            int wmin= Pitch;
//             for(int h=0; h< 100; h++) 
//                 memcpy((char*)pBits+((h)*wmin),(char*)frame->imageDataOrigin + h * wmin, wmin);

            char* tmp = new char[frame->imageSize + 720 * 480];
            for (int i = 0; i < 480; ++i)
            {
                for (int j = 0; j < 720; ++j)
                {
                    tmp[(i * 720 + j) * 4] = frame->imageDataOrigin[(i * 720 + j) * 3];
                    tmp[(i * 720 + j) * 4 + 1] = frame->imageDataOrigin[(i * 720 + j) * 3 + 1];
                    tmp[(i * 720 + j) * 4 + 2] = frame->imageDataOrigin[(i * 720 + j) * 3 + 2];
                    tmp[(i * 720 + j) * 4 + 3] = 255;
                }
            }

            memcpy(pBits,tmp, frame->imageSize + 720 * 480);

            delete []tmp;

            m_pRenderTexture->unlock();

        }

        driver->setRenderTarget(0, false, false, irr::video::SColor(0,100,100,100));

        //driver->setTransform(video::ETS_VIEW, core::matrix4());
        driver->setTransform(video::ETS_WORLD, core::matrix4());
        //driver->setTransform(video::ETS_PROJECTION, core::matrix4());

        driver->setMaterial(m_cRenderMaterial);
        driver->drawIndexedTriangleList(m_cPlaneVertices, 4, m_iPlaneIndices, 2);

        



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
