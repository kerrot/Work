#include <irrlicht.h>
#include <OVR.h>
#include <stdio.h>

using namespace OVR;
using namespace OVR::Util::Render;

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


bool enable = false;

IrrlichtDevice *device = 0;

void matrix4f_print(Matrix4f m) {
    printf(
        "{\n"
        "  % 08f,% 08f,% 08f,% 08f\n"
        "  % 08f,% 08f,% 08f,% 08f\n"
        "  % 08f,% 08f,% 08f,% 08f\n"
        "  % 08f,% 08f,% 08f,% 08f\n"
        "}\n",
        m.M[0][0], m.M[0][1], m.M[0][2], m.M[0][3],
        m.M[1][0], m.M[1][1], m.M[1][2], m.M[1][3],
        m.M[2][0], m.M[2][1], m.M[2][2], m.M[2][3],
        m.M[3][0], m.M[3][1], m.M[3][2], m.M[3][3]
    );
    return;
}

void matrix4_print(matrix4 m) {
    printf(
        "{\n"
        "  % 08f,% 08f,% 08f,% 08f\n"
        "  % 08f,% 08f,% 08f,% 08f\n"
        "  % 08f,% 08f,% 08f,% 08f\n"
        "  % 08f,% 08f,% 08f,% 08f\n"
        "}\n",
        m[0], m[4], m[8], m[12],
        m[1], m[5], m[9], m[13],
        m[2], m[6], m[10], m[14],
        m[3], m[7], m[11], m[15]
    );
    return;
}

Matrix4f matrix4f_from_matrix4(matrix4 m) {
    return Matrix4f(
        m[0], m[4], m[8], m[12],
        m[1], m[5], m[9], m[13],
        m[2], m[6], m[10], m[14],
        m[3], m[7], m[11], m[15]
    );
}

matrix4 matrix4_from_matrix4f(Matrix4f m) {
    matrix4 ret;
    ret[0] = m.M[0][0];
    ret[1] = m.M[1][0];
    ret[2] = m.M[2][0];
    ret[3] = m.M[3][0];
    ret[4] = m.M[0][1];
    ret[5] = m.M[1][1];
    ret[6] = m.M[2][1];
    ret[7] = m.M[3][1];
    ret[8] = m.M[0][2];
    ret[9] = m.M[1][2];
    ret[10] = m.M[2][2];
    ret[11] = m.M[3][2];
    ret[12] = m.M[0][3];
    ret[13] = m.M[1][3];
    ret[14] = m.M[2][3];
    ret[15] = m.M[3][3];
    return ret;
}

void matrix4_another_handed(matrix4 &m) {
    m[8] = -m[8];
    m[9] = -m[9];
    m[10] = -m[10];
    m[11] = -m[11];
    return;
}

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

class RiftDistortionCallback: public irr::video::IShaderConstantSetCallBack {
public:
    irr::f32 m_fScale[2],
             m_fScaleIn[2],
             m_fScreenCenter[2],
             m_fLensCenter[2],
             m_fHmdWarpParam[4];

    virtual void OnSetConstants(irr::video::IMaterialRendererServices *a_pServices, irr::s32 a_iUserData)
    {
        a_pServices->setPixelShaderConstant("scale"       , m_fScale       , 2);
        a_pServices->setPixelShaderConstant("scaleIn"     , m_fScaleIn     , 2);
        a_pServices->setPixelShaderConstant("lensCenter"  , m_fLensCenter  , 2);
        a_pServices->setPixelShaderConstant("hmdWarpParam", m_fHmdWarpParam, 4);
    }
};

int main()
{
    dimension2d<u32> resolution(1280, 800);
    MyEventReceiver receiver;
    StereoConfig SConfig;

    device = createDevice(
        video::EDT_OPENGL,
        resolution, 16,
        true, false, false, &receiver
    );

    if (!device) return 1;

    IVideoDriver* driver = device->getVideoDriver();
    ISceneManager* smgr = device->getSceneManager();
    IGUIEnvironment* guienv = device->getGUIEnvironment();
    IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices();

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
    
    System::Init(Log::ConfigureDefaultLog(LogMask_All));
    Ptr<DeviceManager> pManager = *DeviceManager::Create();
    Ptr<HMDDevice> pHMD = 0;
    Ptr<SensorDevice > riftSensor = 0;
    SensorFusion riftFusion;

    bool riftAvailable = pManager->EnumerateDevices<OVR::HMDDevice>().IsAvailable();

    HMDInfo hmd;

    if (riftAvailable)
    {
        pHMD = *pManager->EnumerateDevices<HMDDevice>().CreateDevice();
        riftSensor = *pHMD->GetSensor();
        if (riftSensor) 
        {
            riftFusion.AttachToSensor(riftSensor);
        }

        pHMD->GetDeviceInfo(&hmd);
    }

    SConfig.SetHMDInfo(hmd); // there is a default hmd info in StereoConfig
    SConfig.SetFullViewport(Viewport(0, 0, resolution.Width, resolution.Height));

    RiftDistortionCallback* m_cDistortionCB = new RiftDistortionCallback();

    f32 l_fDistScale = SConfig.GetDistortionScale();
    f32 l_fAspect    = SConfig.GetAspect();

    m_cDistortionCB->m_fScale[0]        = 1.0f / l_fDistScale;
    m_cDistortionCB->m_fScale[1]        = 1.0f * l_fAspect / l_fDistScale;
    m_cDistortionCB->m_fScaleIn[0]      = 1.0f;
    m_cDistortionCB->m_fScaleIn[1]      = 1.0f / l_fAspect;
    m_cDistortionCB->m_fLensCenter[1]   = 0.0f;
    m_cDistortionCB->m_fHmdWarpParam[0] = hmd.DistortionK[0];
    m_cDistortionCB->m_fHmdWarpParam[1] = hmd.DistortionK[1];
    m_cDistortionCB->m_fHmdWarpParam[2] = hmd.DistortionK[2];
    m_cDistortionCB->m_fHmdWarpParam[3] = hmd.DistortionK[3];

    ISceneNode *m_pHeadX,
               *m_pHeadY,
               *m_pHeadZ,
               *m_pHead,
               *m_pYaw,
               *m_pLeftEye,
               *m_pRghtEye;

    m_pYaw   = smgr->addEmptySceneNode(0, 0);
    m_pHead  = smgr->addEmptySceneNode(m_pYaw, 0);
    /*
    m_pHeadX = smgr->addEmptySceneNode(m_pYaw  , 0);
    m_pHeadY = smgr->addEmptySceneNode(m_pHeadX, 0);
    m_pHeadZ = smgr->addEmptySceneNode(m_pHeadY, 0);
    */
    m_pLeftEye = smgr->addEmptySceneNode(m_pHead, 0);
    m_pRghtEye = smgr->addEmptySceneNode(m_pHead, 0); 

    f32 m_fEyeSeparation = hmd.InterpupillaryDistance/2.0f;
    m_pLeftEye->setPosition(irr::core::vector3df( m_fEyeSeparation, 0.0f, 0.0f));
    m_pRghtEye->setPosition(irr::core::vector3df(-m_fEyeSeparation, 0.0f, 0.0f));

    io::path vsFileName = "Resource/main.vert";
    io::path psFileName = "Resource/main.frag";

    SMaterial m_cRenderMaterial;
    m_cRenderMaterial.Wireframe = false;
    m_cRenderMaterial.Lighting = false;
    m_cRenderMaterial.TextureLayer[0].TextureWrapU = irr::video::ETC_CLAMP;
    m_cRenderMaterial.TextureLayer[0].TextureWrapV = irr::video::ETC_CLAMP;
    m_cRenderMaterial.MaterialType = (irr::video::E_MATERIAL_TYPE)gpu->addHighLevelShaderMaterialFromFiles(
        vsFileName, "main", irr::video::EVST_VS_3_0,
        psFileName, "main", irr::video::EPST_PS_3_0,
        m_cDistortionCB
    );
    ITexture      *m_pRenderTexture = 0;

    m_cDistortionCB->drop();
    if (driver->queryFeature(irr::video::EVDF_RENDER_TO_TARGET))
    {
        if (m_pRenderTexture) 
        {
            m_pRenderTexture->drop();
        }
        m_pRenderTexture = driver->addRenderTargetTexture(irr::core::dimension2d<irr::u32>((irr::u32)(hmd.HResolution * l_fDistScale / 2.0f), (irr::u32)(hmd.VResolution * l_fDistScale)));
        m_cRenderMaterial.setTexture(0, m_pRenderTexture);
    }

    ICameraSceneNode* lCamera = 0;
    ICameraSceneNode* rCamera = 0;
    if (riftAvailable)
    {
        lCamera = smgr->addCameraSceneNode();
        rCamera = smgr->addCameraSceneNode();
    }
    else
    {
        lCamera = smgr->addCameraSceneNodeFPS();    
        rCamera = smgr->addCameraSceneNodeFPS();    
    }
    
    device->getCursorControl()->setVisible(false);

    u32 then = device->getTimer()->getTime();
    const f32 MOVEMENT_SPEED = 10.f;


    irr::video::S3DVertex m_cPlaneVertices[4];
    irr::u16 m_iPlaneIndices[6];

    m_cPlaneVertices[0] = irr::video::S3DVertex(-1.0f, -1.0f, 1.0f,1,1,0, irr::video::SColor(255,255,255,255), 0.0f, 0.0f);
    m_cPlaneVertices[1] = irr::video::S3DVertex(-1.0f,  1.0f, 1.0f,1,1,0, irr::video::SColor(255,255,255,255), 0.0f, 1.0f);
    m_cPlaneVertices[2] = irr::video::S3DVertex( 1.0f,  1.0f, 1.0f,1,1,0, irr::video::SColor(255,255,255,255), 1.0f, 1.0f);
    m_cPlaneVertices[3] = irr::video::S3DVertex( 1.0f, -1.0f, 1.0f,1,1,0, irr::video::SColor(255,255,255,255), 1.0f, 0.0f);

    m_iPlaneIndices[0] = 0;
    m_iPlaneIndices[1] = 1;
    m_iPlaneIndices[2] = 2;
    m_iPlaneIndices[3] = 0;
    m_iPlaneIndices[4] = 2;
    m_iPlaneIndices[5] = 3;


    ICursorControl * m_pCursor = device->getCursorControl();

    position2di lastCursorPos(resolution.Width / 2, resolution.Height / 2);
    
    float pitch = 0, yaw = 0, roll = 0;

    char posText[100];

    while(device->run())
    {
        if(receiver.IsKeyDown(irr::KEY_ESCAPE))
        {
            device->closeDevice();
            break;
        }

        irr::s32 l_iMouseX     = lastCursorPos.X - m_pCursor->getPosition().X;
        irr::s32 l_iMouseY     = lastCursorPos.Y - m_pCursor->getPosition().Y;
 
        irr::core::vector3df l_vRot  = lCamera->getRotation();

        m_pCursor->setPosition(lastCursorPos.X, lastCursorPos.Y);

         if (riftAvailable) 
         {
             OVR::Quatf l_cHmdOrient = riftFusion.GetOrientation();
             l_cHmdOrient.GetEulerAngles<OVR::Axis_X, OVR::Axis_Y, OVR::Axis_Z>(&pitch, &yaw, &roll);
         }
        else
        {
            lCamera->setRotation(irr::core::vector3df(l_vRot.X - ((irr::f32)l_iMouseY) / 5.0f, l_vRot.Y - ((irr::f32)l_iMouseX) / 5.0f, l_vRot.Z));
        }
        matrix4 l_cMat;

        //printf("P: %f, Y: %f, R: %f\n", pitch, yaw, roll);

        pitch *=  irr::core::RADTODEG;
        yaw *= -irr::core::RADTODEG;
        roll *= -irr::core::RADTODEG;

        m_pYaw  ->setRotation(lCamera->getRotation());
        m_pHead->setRotation(irr::core::vector3df( pitch, yaw, roll));
        //m_pHeadX->setRotation(irr::core::vector3df( pitch,   0,    0));
        //m_pHeadZ->setRotation(irr::core::vector3df(     0,   0, roll));

        l_cMat.setRotationDegrees(m_pHead->getAbsoluteTransformation().getRotationDegrees());


        vector3df vFore = irr::core::vector3df(0, 0, -1),
                  vUp   = irr::core::vector3df(0, 1,  0);

        l_cMat.transformVect(vFore);
        l_cMat.transformVect(vUp  );

        lCamera->setTarget  (lCamera->getPosition() + vFore);
        lCamera->setUpVector(                         vUp  );
        
        driver->beginScene(true, true, SColor(0,100,100,100));
        
        
        smgr->setActiveCamera(rCamera);

        /* render what left eye sees to a plane */
        StereoEyeParams params_left = SConfig.GetEyeRenderParams(StereoEye_Left);
        matrix4 m_left = matrix4_from_matrix4f(params_left.Projection * params_left.ViewAdjust);
        matrix4_another_handed(m_left);

        rCamera->setPosition(lCamera->getPosition() + m_pLeftEye->getAbsolutePosition());
        rCamera->setTarget  (lCamera->getTarget  () + m_pLeftEye->getAbsolutePosition());
        rCamera->setUpVector(lCamera->getUpVector());
        rCamera->setProjectionMatrix(m_left);

        driver->setRenderTarget(m_pRenderTexture, true, true, irr::video::SColor(0,0,0,0));
        smgr->drawAll();
        driver->setRenderTarget(0, false, false, irr::video::SColor(0,100,100,100));
        
        /* render the plane to the screen with shader */
        /* no projection here */
        driver->setTransform(video::ETS_VIEW, core::matrix4());
        driver->setTransform(video::ETS_WORLD, core::matrix4());
        driver->setTransform(video::ETS_PROJECTION, core::matrix4());
        
        driver->setViewPort(rect<s32>(0, 0, resolution.Width / 2, resolution.Height));
        m_cDistortionCB->m_fLensCenter[0] = SConfig.GetProjectionCenterOffset();
        driver->setMaterial(m_cRenderMaterial);
        driver->drawIndexedTriangleList(m_cPlaneVertices, 4, m_iPlaneIndices, 2);

        /* now render what right eye sees */
        StereoEyeParams params_right = SConfig.GetEyeRenderParams(StereoEye_Right);
        matrix4 m_right = matrix4_from_matrix4f(params_right.Projection * params_left.ViewAdjust);
        matrix4_another_handed(m_right);

        rCamera->setPosition(lCamera->getPosition() + m_pRghtEye->getAbsolutePosition());
        rCamera->setTarget  (lCamera->getTarget  () + m_pRghtEye->getAbsolutePosition());
        rCamera->setUpVector(lCamera->getUpVector());
        rCamera->setProjectionMatrix(m_right);
        
        driver->setRenderTarget(m_pRenderTexture, true, true, irr::video::SColor(0,0,0,0));
        smgr->drawAll();
        driver->setRenderTarget(0, false, false, irr::video::SColor(0,100,100,100));
 
        /* render the plane to the screen */
        driver->setTransform(video::ETS_VIEW, core::matrix4());
        driver->setTransform(video::ETS_WORLD, core::matrix4());
        driver->setTransform(video::ETS_PROJECTION, core::matrix4());
 
        driver->setViewPort(rect<s32>(resolution.Width / 2, 0, resolution.Width, resolution.Height));
        m_cDistortionCB->m_fLensCenter[0] = -SConfig.GetProjectionCenterOffset();
        driver->setMaterial(m_cRenderMaterial);
        driver->drawIndexedTriangleList(m_cPlaneVertices, 4, m_iPlaneIndices, 2);


        driver->endScene();
    }

    device->drop();

    return 0;
}
