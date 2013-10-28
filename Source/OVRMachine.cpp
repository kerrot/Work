#include "OVRMachine.h"
#include "GameObjectFactory.h"
#include "AvatarObject.h"
#include <OVR.h>

using namespace OVR;
using namespace OVR::Util::Render;

using namespace irr;
using namespace core;

class RiftDistortionCallback: public irr::video::IShaderConstantSetCallBack {
public:
    irr::f32	m_fScale[2],
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

namespace
{
    Ptr<OVR::DeviceManager> g_pRiftManager = 0;
    Ptr<OVR::HMDDevice    > g_pRiftHMD     = 0;
    Ptr<OVR::SensorDevice > g_pRiftSensor  = 0;
    SensorFusion            g_cRiftFusion;
    HMDInfo hmd;
    RiftDistortionCallback* g_cDistortionCB = 0;

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
}

OVRMachine::OVRMachine()
:
m_riftAvailable(false)
,m_riftSceneMode(true)
,m_fLensShift(0)
,m_x(0)
,m_y(0)
,m_z(0)
{
}

OVRMachine::~OVRMachine()
{
    if (g_cDistortionCB)
    {
        g_cDistortionCB->drop();
        g_cDistortionCB = 0;
    }
}

OVRMachine& OVRMachine::GetInstance()
{
    static OVRMachine m_instance;

    return m_instance;
}

bool OVRMachine::IsRiftAvailable()
{
    return m_riftAvailable;
}

void OVRMachine::Init(IrrlichtDevice* a_device)
{
    if (!a_device)
    {
        return;
    }

    m_driver = a_device->getVideoDriver();
    m_smgr = a_device->getSceneManager();
    m_env = a_device->getGUIEnvironment();

    System::Init(Log::ConfigureDefaultLog(LogMask_All));
    g_pRiftManager = *DeviceManager::Create();

    m_riftAvailable = g_pRiftManager->EnumerateDevices<HMDDevice>().IsAvailable();

    if (m_riftAvailable)
    {
        g_pRiftHMD = *g_pRiftManager->EnumerateDevices<HMDDevice>().CreateDevice();
        g_pRiftSensor = *g_pRiftHMD->GetSensor();
        if (g_pRiftSensor) 
        {
            g_cRiftFusion.AttachToSensor(g_pRiftSensor);
        }

        g_pRiftHMD->GetDeviceInfo(&hmd);
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

    //m_riftAvailable = true;

    g_cDistortionCB = new RiftDistortionCallback();

    m_fLensShift = 4.0f * (hmd.HScreenSize / 4.0f - hmd.LensSeparationDistance/2.0f) / hmd.HScreenSize;
    f32 l_fR         = -1.0f - (4.0f * (hmd.HScreenSize / 4.0f - hmd.LensSeparationDistance / 2.0f) / hmd.HScreenSize);
    f32 l_fDistScale = (hmd.DistortionK[0] + hmd.DistortionK[1] * pow(l_fR,2) + hmd.DistortionK[2] * pow(l_fR,4) + hmd.DistortionK[3] * pow(l_fR,6));
    f32 l_fAspect    = hmd.HResolution / (2.0f * hmd.VResolution);
    f32 l_fFov       = 2.0f * atan2(hmd.VScreenSize * l_fDistScale, 2.0f * hmd.EyeToScreenDistance);
    f32	l_fH         = 4 * (hmd.HScreenSize / 4 - hmd.InterpupillaryDistance/2) / hmd.HScreenSize;

    matrix4 l_cCenterProjection = matrix4().buildProjectionMatrixPerspectiveFovLH (l_fFov, l_fAspect, 1, 10000);

    m_cProjectionLeft = matrix4().setTranslation(vector3df( l_fH, 0.0, 0.0)) * l_cCenterProjection;
    m_cProjectionRght = matrix4().setTranslation(vector3df(-l_fH, 0.0, 0.0)) * l_cCenterProjection;

    g_cDistortionCB->m_fScale[0] = 1.0f            / l_fDistScale;
    g_cDistortionCB->m_fScale[1] = 1.0f * l_fAspect/ l_fDistScale;
    g_cDistortionCB->m_fScaleIn[0] = 1.0f;
    g_cDistortionCB->m_fScaleIn[1] = 1.0f / l_fAspect;
    g_cDistortionCB->m_fLensCenter[1] = 0.0f;
    g_cDistortionCB->m_fHmdWarpParam[0] = hmd.DistortionK[0];
    g_cDistortionCB->m_fHmdWarpParam[1] = hmd.DistortionK[1];
    g_cDistortionCB->m_fHmdWarpParam[2] = hmd.DistortionK[2];
    g_cDistortionCB->m_fHmdWarpParam[3] = hmd.DistortionK[3];

    m_cRenderMaterial.Wireframe = false;
    m_cRenderMaterial.Lighting = false;
    m_cRenderMaterial.TextureLayer[0].TextureWrapU = ETC_CLAMP;
    m_cRenderMaterial.TextureLayer[0].TextureWrapV = ETC_CLAMP;
    IGPUProgrammingServices* gpu = m_driver->getGPUProgrammingServices();
    m_cRenderMaterial.MaterialType = (E_MATERIAL_TYPE)gpu->addHighLevelShaderMaterial(vertexShader, "main", EVST_VS_3_0, fragShader, "main", EPST_PS_3_0, g_cDistortionCB);

    if (m_driver->queryFeature(irr::video::EVDF_RENDER_TO_TARGET))
    {
        if (m_pRenderTexture) 
        {
            m_pRenderTexture->drop();
        }
        m_pRenderTexture = m_driver->addRenderTargetTexture(dimension2d<u32>((u32)(hmd.HResolution * l_fDistScale / 2.0f), (u32)(hmd.VResolution * l_fDistScale)));
        m_cRenderMaterial.setTexture(0, m_pRenderTexture);
    }

    m_cPlaneVertices[0] = S3DVertex(-1.0f, -1.0f, 1.0f,1,1,0, SColor(255,255,255,255), 0.0f, 0.0f);
    m_cPlaneVertices[1] = S3DVertex(-1.0f,  1.0f, 1.0f,1,1,0, SColor(255,255,255,255), 0.0f, 1.0f);
    m_cPlaneVertices[2] = S3DVertex( 1.0f,  1.0f, 1.0f,1,1,0, SColor(255,255,255,255), 1.0f, 1.0f);
    m_cPlaneVertices[3] = S3DVertex( 1.0f, -1.0f, 1.0f,1,1,0, SColor(255,255,255,255), 1.0f, 0.0f);

    m_iPlaneIndices[0] = 0;
    m_iPlaneIndices[1] = 1;
    m_iPlaneIndices[2] = 2;
    m_iPlaneIndices[3] = 0;
    m_iPlaneIndices[4] = 2;
    m_iPlaneIndices[5] = 3;
}

void OVRMachine::SetMainCamera( ICameraSceneNode* a_camera )
{
    m_mainCamera = a_camera;
    m_riftCamera = m_smgr->addCameraSceneNode();

    m_pYaw   = m_smgr->addEmptySceneNode(0, 0);
    m_pHeadX = m_smgr->addEmptySceneNode(m_pYaw  , 0);
    m_pHeadY = m_smgr->addEmptySceneNode(m_pHeadX, 0);
    m_pHeadZ = m_smgr->addEmptySceneNode(m_pHeadY, 0);
    m_pLeftEye = m_smgr->addEmptySceneNode(m_pHeadZ, 0);
    m_pRghtEye = m_smgr->addEmptySceneNode(m_pHeadZ, 0); 

    f32 m_fEyeSeparation = hmd.InterpupillaryDistance/2.0f;
    m_pLeftEye->setPosition(vector3df( m_fEyeSeparation, 0.0f, 0.0f));
    m_pRghtEye->setPosition(vector3df(-m_fEyeSeparation, 0.0f, 0.0f));
}

void OVRMachine::Draw()
{
    float pitch = 0, yaw = 0, roll = 0;

    if (m_riftAvailable)
    {
        Quatf l_cHmdOrient = g_cRiftFusion.GetOrientation();
        l_cHmdOrient.GetEulerAngles<OVR::Axis_X, OVR::Axis_Y, OVR::Axis_Z>(&pitch, &yaw, &roll);
    }
    else
    {
        if (m_riftSceneMode)
        {
            pitch = m_x;
            yaw = m_y;
            roll = m_z;
        }
    }

    matrix4 l_cMat;
    pitch *=  irr::core::RADTODEG;
    yaw *= -irr::core::RADTODEG;
    roll *= -irr::core::RADTODEG;
    m_pYaw->setRotation(m_mainCamera->getAbsoluteTransformation().getRotationDegrees());
    m_pHeadY->setRotation(vector3df(0, yaw, 0));
    m_pHeadX->setRotation(vector3df(pitch, 0, 0));
    m_pHeadZ->setRotation(vector3df(0, 0, roll));
    l_cMat.setRotationDegrees(m_pHeadZ->getAbsoluteTransformation().getRotationDegrees());
    vector3df vFore(0, 0, 1);
    vector3df vUp(0, 1, 0);
    l_cMat.transformVect(vFore);
    l_cMat.transformVect(vUp);
    m_mainCamera->setTarget(m_mainCamera->getAbsolutePosition() + vFore);
    m_mainCamera->setUpVector(vUp);


    m_driver->beginScene(true, true, SColor(0,100,100,100));

    m_smgr->setActiveCamera(m_riftCamera);

    m_driver->setRenderTarget(m_pRenderTexture, true, true, SColor(0,0,0,0));

    m_riftCamera->setPosition(m_mainCamera->getAbsolutePosition() + m_pLeftEye->getAbsolutePosition());
    m_riftCamera->setTarget  (m_mainCamera->getTarget() + m_pLeftEye->getAbsolutePosition());
    m_riftCamera->setUpVector(m_mainCamera->getUpVector());

    m_riftCamera->setProjectionMatrix(m_cProjectionLeft);

    m_smgr->drawAll();
    m_env->drawAll();
    m_driver->setRenderTarget(0, false, false, SColor(0,100,100,100));

    m_driver->setViewPort(rect<s32>(0,0,640,800));
    m_driver->setTransform(video::ETS_VIEW, core::matrix4());
    m_driver->setTransform(video::ETS_WORLD, core::matrix4());
    m_driver->setTransform(video::ETS_PROJECTION, core::matrix4());
    g_cDistortionCB->m_fLensCenter[0] = m_fLensShift;
    m_driver->setMaterial(m_cRenderMaterial);
    m_driver->drawIndexedTriangleList(m_cPlaneVertices, 4, m_iPlaneIndices, 2);

    m_driver->setRenderTarget(m_pRenderTexture, true, true, SColor(0,0,0,0));

    m_riftCamera->setPosition(m_mainCamera->getAbsolutePosition() + m_pRghtEye->getAbsolutePosition());
    m_riftCamera->setTarget  (m_mainCamera->getTarget  () + m_pRghtEye->getAbsolutePosition());
    m_riftCamera->setUpVector(m_mainCamera->getUpVector());

    m_riftCamera->setProjectionMatrix(m_cProjectionRght);

    m_smgr->drawAll();
    m_env->drawAll();
    m_driver->setRenderTarget(0, false, false, SColor(0,100,100,100));

    m_driver->setTransform(video::ETS_VIEW, core::matrix4());
    m_driver->setTransform(video::ETS_WORLD, core::matrix4());
    m_driver->setTransform(video::ETS_PROJECTION, core::matrix4());

    m_driver->setViewPort(rect<s32>(640,0,1280,800));
    g_cDistortionCB->m_fLensCenter[0] = -m_fLensShift;
    m_driver->setMaterial(m_cRenderMaterial);
    m_driver->drawIndexedTriangleList(m_cPlaneVertices, 4, m_iPlaneIndices, 2);

    m_driver->endScene();
}

void OVRMachine::Test( float a_x, float a_y, float a_z )
{
    m_x += a_x;
    m_y += a_y;
    m_z += a_z;
}

bool OVRMachine::IsRiftSceneMode()
{
    return m_riftSceneMode;
}
