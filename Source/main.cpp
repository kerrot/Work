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

static const char* vertexShader =
    "void main() {"
    "  gl_Position = vec4(gl_Vertex.xy, 0.0, 1.0);"
    "  gl_TexCoord[0].st = gl_MultiTexCoord0;"
    "}";

static const char *fragShader =
    "uniform vec2 scale;"
    "uniform vec2 scaleIn;"
    "uniform vec2 lensCenter;"
    "uniform vec4 hmdWarpParam;"
    "uniform sampler2D texid;"
    "void main()"
    "{"
    "  vec2 uv = (gl_TexCoord[0].st*2.0)-1.0;" // range from [0,1] to [-1,1]
    "  vec2 theta = (uv-lensCenter)*scaleIn;"
    "  float rSq = theta.x*theta.x + theta.y*theta.y;"
    "  vec2 rvector = theta*(hmdWarpParam.x + hmdWarpParam.y*rSq + hmdWarpParam.z*rSq*rSq + hmdWarpParam.w*rSq*rSq*rSq);"
    "  vec2 tc = (lensCenter + scale * rvector);"
    "  tc = (tc+1.0)/2.0;" // range from [-1,1] to [0,1]
    "  if (any(bvec2(clamp(tc, vec2(0.0,0.0), vec2(1.0,1.0))-tc)))"
    "    gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);"
    "  else"
    "    gl_FragColor = texture2D(texid, tc);"
    "}";

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
    MyEventReceiver receiver;

    device = createDevice(
        video::EDT_OPENGL,
        dimension2d<u32>(1280, 800), 16,
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
    else
    {
        hmd.HResolution = 1280;
        hmd.VResolution = 800;
        hmd.HScreenSize = 0.14976f;
        hmd.VScreenSize= 0.0936f;
        hmd.InterpupillaryDistance = 0.064f;
        hmd.LensSeparationDistance = 0.064f;
        hmd.EyeToScreenDistance = 0.041f;
        hmd.DistortionK[0] = 1.0f;
        hmd.DistortionK[1] = 0.22f;
        hmd.DistortionK[2] = 0.24f;
        hmd.DistortionK[3] = 0.0f;
    }

    RiftDistortionCallback* m_cDistortionCB = new RiftDistortionCallback();

    f32 m_fLensShift = 4.0f * (hmd.HScreenSize / 4.0f - hmd.LensSeparationDistance/2.0f) / hmd.HScreenSize;
    f32 l_fR         = -1.0f - (4.0f * (hmd.HScreenSize / 4.0f - hmd.LensSeparationDistance / 2.0f) / hmd.HScreenSize);
    f32 l_fDistScale = (hmd.DistortionK[0] + hmd.DistortionK[1] * pow(l_fR,2) + hmd.DistortionK[2] * pow(l_fR,4) + hmd.DistortionK[3] * pow(l_fR,6));
    f32 l_fAspect    = hmd.HResolution / (2.0f * hmd.VResolution);
    f32 l_fFov       = 2.0f * atan2(hmd.VScreenSize * l_fDistScale, 2.0f * hmd.EyeToScreenDistance);
    f32    l_fH         = 4 * (hmd.HScreenSize / 4 - hmd.InterpupillaryDistance/2) / hmd.HScreenSize;

    matrix4 l_cCenterProjection = irr::core::matrix4().buildProjectionMatrixPerspectiveFovLH (l_fFov, l_fAspect, 1, 10000);

    matrix4 m_cProjectionLeft = irr::core::matrix4().setTranslation(irr::core::vector3df( l_fH, 0.0, 0.0)) * l_cCenterProjection;
    matrix4 m_cProjectionRght = irr::core::matrix4().setTranslation(irr::core::vector3df(-l_fH, 0.0, 0.0)) * l_cCenterProjection;

    m_cDistortionCB->m_fScale[0] = 1.0f            / l_fDistScale;
    m_cDistortionCB->m_fScale[1] = 1.0f * l_fAspect/ l_fDistScale;
    m_cDistortionCB->m_fScaleIn[0] = 1.0f;
    m_cDistortionCB->m_fScaleIn[1] = 1.0f / l_fAspect;
    m_cDistortionCB->m_fLensCenter[1] = 0.0f;
    m_cDistortionCB->m_fHmdWarpParam[0] = hmd.DistortionK[0];
    m_cDistortionCB->m_fHmdWarpParam[1] = hmd.DistortionK[1];
    m_cDistortionCB->m_fHmdWarpParam[2] = hmd.DistortionK[2];
    m_cDistortionCB->m_fHmdWarpParam[3] = hmd.DistortionK[3];

    ISceneNode *m_pHeadX,
               *m_pHeadY,
               *m_pHeadZ,
               *m_pYaw,
               *m_pLeftEye,
               *m_pRghtEye;

    m_pYaw   = smgr->addEmptySceneNode(0, 0);
    m_pHeadX = smgr->addEmptySceneNode(m_pYaw  , 0);
    m_pHeadY = smgr->addEmptySceneNode(m_pHeadX, 0);
    m_pHeadZ = smgr->addEmptySceneNode(m_pHeadY, 0);
    m_pLeftEye = smgr->addEmptySceneNode(m_pHeadZ, 0);
    m_pRghtEye = smgr->addEmptySceneNode(m_pHeadZ, 0); 

    f32 m_fEyeSeparation = hmd.InterpupillaryDistance/2.0f;
    m_pLeftEye->setPosition(irr::core::vector3df( m_fEyeSeparation, 0.0f, 0.0f));
    m_pRghtEye->setPosition(irr::core::vector3df(-m_fEyeSeparation, 0.0f, 0.0f));


    SMaterial m_cRenderMaterial;
    m_cRenderMaterial.Wireframe = false;
    m_cRenderMaterial.Lighting = false;
    m_cRenderMaterial.TextureLayer[0].TextureWrapU = irr::video::ETC_CLAMP;
    m_cRenderMaterial.TextureLayer[0].TextureWrapV = irr::video::ETC_CLAMP;
    m_cRenderMaterial.MaterialType = (irr::video::E_MATERIAL_TYPE)gpu->addHighLevelShaderMaterial(vertexShader, "main", irr::video::EVST_VS_3_0, fragShader, "main", irr::video::EPST_PS_3_0, m_cDistortionCB);
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

    position2di lastCursorPos(640, 400);
    
    float pitch = 0, yaw = 0, roll = 0;

    char posText[100];

    while(device->run())
    {
        if(receiver.IsKeyDown(irr::KEY_ESCAPE))
        {
            device->closeDevice();
        }

        irr::s32 l_iMouseX     = 640 - m_pCursor->getPosition().X;
        irr::s32 l_iMouseY     = 400 - m_pCursor->getPosition().Y;
 
        irr::core::vector3df l_vRot  = lCamera->getRotation();

        m_pCursor->setPosition(640, 400);

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

        printf("P: %f, Y: %f, R: %f\n", pitch, yaw, roll);

        pitch *=  irr::core::RADTODEG;
        yaw *= -irr::core::RADTODEG;
        roll *= -irr::core::RADTODEG;

        m_pYaw  ->setRotation(lCamera->getRotation()); // irr::core::vector3df(        0, l_pCamera->getRotation().Y,   0));
        m_pHeadY->setRotation(irr::core::vector3df(        0, yaw                       ,   0));
        m_pHeadX->setRotation(irr::core::vector3df(      pitch,   0                       ,   0));
        m_pHeadZ->setRotation(irr::core::vector3df(        0,   0                       , roll));

        l_cMat.setRotationDegrees(m_pHeadZ->getAbsoluteTransformation().getRotationDegrees());


        vector3df vFore = irr::core::vector3df(0, 0, -1),
                  vUp   = irr::core::vector3df(0, 1,  0);

        l_cMat.transformVect(vFore);
        l_cMat.transformVect(vUp  );

        lCamera->setTarget  (lCamera->getPosition() + vFore);
        lCamera->setUpVector(                           vUp  );
        
//      vector3df position = camera->getPosition();
//      vector3df target = camera->getTarget();

        //camera->get

        driver->beginScene(true, true, SColor(0,100,100,100));
        
        smgr->setActiveCamera(rCamera);
        driver->setRenderTarget(m_pRenderTexture, true, true, irr::video::SColor(0,0,0,0));
        
        

        rCamera->setPosition(lCamera->getPosition() + m_pLeftEye->getAbsolutePosition());
        rCamera->setTarget  (lCamera->getTarget  () + m_pLeftEye->getAbsolutePosition());//getTarget  () + l_vTx);
        rCamera->setUpVector(lCamera->getUpVector());
        
        rCamera->setProjectionMatrix(m_cProjectionLeft);
//      camera->setPosition(position + m_pLeftEye->getAbsolutePosition());
//      camera->setTarget  (target + m_pLeftEye->getAbsolutePosition());

        smgr->drawAll();
        driver->setRenderTarget(0, false, false, irr::video::SColor(0,100,100,100));
        
        driver->setViewPort(rect<s32>(0,0,640,800));
        driver->setTransform(video::ETS_VIEW, core::matrix4());
        driver->setTransform(video::ETS_WORLD, core::matrix4());
        driver->setTransform(video::ETS_PROJECTION, core::matrix4());
        m_cDistortionCB->m_fLensCenter[0] = m_fLensShift;
        driver->setMaterial(m_cRenderMaterial);
        driver->drawIndexedTriangleList(m_cPlaneVertices, 4, m_iPlaneIndices, 2);

        driver->setRenderTarget(m_pRenderTexture, true, true, irr::video::SColor(0,0,0,0));

        //smgr->setActiveCamera(rCamera);
        
        rCamera->setPosition(lCamera->getPosition() + m_pRghtEye->getAbsolutePosition());
        rCamera->setTarget  (lCamera->getTarget  () + m_pRghtEye->getAbsolutePosition());//getTarget  () + l_vTx);
        rCamera->setUpVector(lCamera->getUpVector());
        
        rCamera->setProjectionMatrix(m_cProjectionRght);
//      camera->setPosition(position + m_pRghtEye->getAbsolutePosition());
//      camera->setTarget  (target + m_pRghtEye->getAbsolutePosition());
        
        smgr->drawAll();
        driver->setRenderTarget(0, false, false, irr::video::SColor(0,100,100,100));
 
        driver->setTransform(video::ETS_VIEW, core::matrix4());
        driver->setTransform(video::ETS_WORLD, core::matrix4());
        driver->setTransform(video::ETS_PROJECTION, core::matrix4());
 
        driver->setViewPort(rect<s32>(640,0,1280,800));
        m_cDistortionCB->m_fLensCenter[0] = -m_fLensShift;
        driver->setMaterial(m_cRenderMaterial);
        driver->drawIndexedTriangleList(m_cPlaneVertices, 4, m_iPlaneIndices, 2);

//      camera->setPosition(position);
//      camera->setTarget  (target);

        driver->endScene();
    }

    device->drop();

//      if (pHMD)
//      {
//          pHMD->Release();
//      }
//      if (pManager)
//      {
//          pManager->Release();
//          pManager = 0;
//      }
// 
//      OVR::System::Destroy();

    return 0;
}
