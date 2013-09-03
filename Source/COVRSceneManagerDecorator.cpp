#include <stdio.h>
#include <IVideoDriver.h>
#include <IGPUProgrammingServices.h>
#include <ICameraSceneNode.h>
#include <EShaderTypes.h>

#include "COVRSceneManagerDecorator.h"

namespace irr
{

/* ugly handmade private functions */
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

void matrix4_print(core::matrix4 m) {
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

Matrix4f matrix4f_from_matrix4(core::matrix4 m) {
    return Matrix4f(
        m[0], m[4], m[8], m[12],
        m[1], m[5], m[9], m[13],
        m[2], m[6], m[10], m[14],
        m[3], m[7], m[11], m[15]
    );
}

core::matrix4 matrix4_from_matrix4f(Matrix4f m) {
    core::matrix4 ret;
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

void matrix4_another_handed(core::matrix4 &m) {
    m[8] = -m[8];
    m[9] = -m[9];
    m[10] = -m[10];
    m[11] = -m[11];
    return;
}

namespace scene
{

COVRSceneManagerDecorator::COVRSceneManagerDecorator(ISceneManager *smgr)
    :CSceneManagerDecorator(smgr), ZNear(1.0f), ZFar(10000.0f),
    vTarget(core::vector3df(0.0f, 0.0f, 1.0f)), vUp(core::vector3df(0.0f, 1.0f, 0.0f))
{
    #ifdef _DEBUG
    setDebugName("COVRSceneManagerDecorator");
    #endif

    video::IVideoDriver* driver = smgr ? smgr->getVideoDriver() : 0;
    video::IGPUProgrammingServices* gpu = driver ? driver->getGPUProgrammingServices() : 0;
    f32 width = (f32)driver->getCurrentRenderTargetSize().Width;
    f32 height = (f32)driver->getCurrentRenderTargetSize().Height;

    System::Init(Log::ConfigureDefaultLog(LogMask_All));
    HMDManager = *DeviceManager::Create();

    if (HMDManager->EnumerateDevices<OVR::HMDDevice>().IsAvailable())
    {   
        HMD = *HMDManager->EnumerateDevices<HMDDevice>().CreateDevice();
        Sensor = *HMD->GetSensor();
        if (Sensor) Fusion.AttachToSensor(Sensor);
        HMD->GetDeviceInfo(&Info);
    }   

    SConfig.SetHMDInfo(Info);
    SConfig.SetFullViewport(Viewport(0, 0, width, height));

    f32 DistScale = SConfig.GetDistortionScale();
    f32 DistAspect = SConfig.GetAspect();

    pDistortionCallBack = new RiftDistortionCallBack();
    pDistortionCallBack->Scale[0]        = 1.0f              / DistScale;
    pDistortionCallBack->Scale[1]        = 1.0f * DistAspect / DistScale;
    pDistortionCallBack->ScaleIn[0]      = 1.0f;
    pDistortionCallBack->ScaleIn[1]      = 1.0f / DistAspect;
    pDistortionCallBack->LensCenter[1]   = 0.0f;
    pDistortionCallBack->HMDWarpParam[0] = SConfig.GetDistortionK(0);
    pDistortionCallBack->HMDWarpParam[1] = SConfig.GetDistortionK(1);
    pDistortionCallBack->HMDWarpParam[2] = SConfig.GetDistortionK(2);
    pDistortionCallBack->HMDWarpParam[3] = SConfig.GetDistortionK(3);

    io::path vsFileName = "Resource/distortion.vert";
    io::path psFileName = "Resource/distortion.frag";

    DistortionMaterial.Wireframe = false;
    DistortionMaterial.Lighting = false;
    DistortionMaterial.TextureLayer[0].TextureWrapU = video::ETC_CLAMP;
    DistortionMaterial.TextureLayer[0].TextureWrapV = video::ETC_CLAMP;
    DistortionMaterial.MaterialType =
        (video::E_MATERIAL_TYPE)gpu->addHighLevelShaderMaterialFromFiles(
            vsFileName, "main", video::EVST_VS_3_0,
            psFileName, "main", video::EPST_PS_3_0,
            pDistortionCallBack
        );

    if (driver->queryFeature(video::EVDF_RENDER_TO_TARGET))
    {
        pDistortionTexture =
            driver->addRenderTargetTexture(core::dimension2d<u32>((u32)(width * DistScale / 2.0f), (u32)(height * DistScale)));
        DistortionMaterial.setTexture(0, pDistortionTexture);
    }

    PlaneVertices[0] = video::S3DVertex(-1.0f, -1.0f, 1.0f, 1, 1, 0, video::SColor(255, 255, 255, 255), 0.0f, 0.0f);
    PlaneVertices[1] = video::S3DVertex(-1.0f,  1.0f, 1.0f, 1, 1, 0, video::SColor(255, 255, 255, 255), 0.0f, 1.0f);
    PlaneVertices[2] = video::S3DVertex( 1.0f,  1.0f, 1.0f, 1, 1, 0, video::SColor(255, 255, 255, 255), 1.0f, 1.0f);
    PlaneVertices[3] = video::S3DVertex( 1.0f, -1.0f, 1.0f, 1, 1, 0, video::SColor(255, 255, 255, 255), 1.0f, 0.0f);

    PlaneIndices[0] = 0;
    PlaneIndices[1] = 1;
    PlaneIndices[2] = 2;
    PlaneIndices[3] = 0;
    PlaneIndices[4] = 2;
    PlaneIndices[5] = 3;

    pHead = smgr->addEmptySceneNode(0, 0);
    pHeadX = smgr->addEmptySceneNode(pHead, 0);
    pHeadY = smgr->addEmptySceneNode(pHeadX, 0);
    pHeadZ = smgr->addEmptySceneNode(pHeadY, 0);
    pEyeLeft = smgr->addEmptySceneNode(pHeadZ, 0);
    pEyeRight = smgr->addEmptySceneNode(pHeadZ, 0);
    pCamera = smgr->addCameraSceneNode();
    smgr->setActiveCamera(0);

    f32 offset = SConfig.GetIPD() / 2.0f;
    pEyeLeft->setPosition (core::vector3df( offset, 0.0f, 0.0f));
    pEyeRight->setPosition (core::vector3df(-offset, 0.0f, 0.0f));
}

COVRSceneManagerDecorator::~COVRSceneManagerDecorator()
{
    pDistortionCallBack->drop();
    pDistortionTexture->drop();
    pRealCamera->drop();
    pCamera->drop();
    pEyeRight->drop();
    pEyeLeft->drop();
    pHeadZ->drop();
    pHeadY->drop();
    pHeadX->drop();
    pHead->drop();
}

void COVRSceneManagerDecorator::drawAll()
{
    video::IVideoDriver* driver = SceneManager->getVideoDriver();
    if (!driver) return;

    pRealCamera = SceneManager->getActiveCamera();
    if (!pRealCamera) return;

    core::matrix4 matrix;
    core::vector3df target = vTarget,
                    up     = vUp;
    f32 pitch = 0,
        yaw = 0,
        roll = 0;

    if (HMDManager->EnumerateDevices<OVR::HMDDevice>().IsAvailable()) {
        Fusion.GetOrientation().GetEulerAngles<OVR::Axis_X, OVR::Axis_Y, OVR::Axis_Z>(&pitch, &yaw, &roll);
    }

    pitch *= -core::RADTODEG;
    yaw *= -core::RADTODEG;
    roll *= core::RADTODEG;

    pHead->setRotation(pRealCamera->getRotation());
    pHeadX->setRotation(irr::core::vector3df( pitch,   0,    0));
    pHeadY->setRotation(irr::core::vector3df(     0, yaw,    0));
    pHeadZ->setRotation(irr::core::vector3df(     0,   0, roll));

    matrix.setRotationDegrees(pHeadZ->getAbsoluteTransformation().getRotationDegrees());
    //matrix.transformVect(target);
    matrix.transformVect(up);

    // update animators
    CSceneManagerDecorator::drawAll();

    // render
    CSceneManagerDecorator::setActiveCamera(pCamera);

    const StereoEyeParams *params;

    // leat eye
    params = &SConfig.GetEyeRenderParams(StereoEye_Left);
    matrix = matrix4_from_matrix4f(params->Projection * params->ViewAdjust);
    matrix[10] = ZFar / (ZNear - ZFar);
    matrix[14] = ZFar * ZNear / (ZNear - ZFar);
    matrix4_another_handed(matrix);

    pCamera->setPosition(         pRealCamera->getPosition() + pEyeLeft->getAbsolutePosition());
    pCamera->setTarget  (target + pRealCamera->getPosition() + pEyeLeft->getAbsolutePosition());
    //pCamera->setUpVector(up);
    pCamera->setProjectionMatrix(matrix);

    driver->setRenderTarget(pDistortionTexture, true, true, video::SColor(0, 0, 0, 0));
    CSceneManagerDecorator::drawAll();
    driver->setRenderTarget(0, false, false, video::SColor(0, 100, 100, 100));

    driver->setTransform(video::ETS_VIEW, core::matrix4());
    driver->setTransform(video::ETS_WORLD, core::matrix4());
    driver->setTransform(video::ETS_PROJECTION, core::matrix4());

    driver->setViewPort(
        core::rect<s32>(
            params->VP.x,
            params->VP.y,
            params->VP.x + params->VP.w,
            params->VP.y + params->VP.h
        )
    );
    pDistortionCallBack->LensCenter[0] = SConfig.GetProjectionCenterOffset();
    driver->setMaterial(DistortionMaterial);
    driver->drawIndexedTriangleList(PlaneVertices, 4, PlaneIndices, 2);

    // right eye
    params = &SConfig.GetEyeRenderParams(StereoEye_Right);
    matrix = matrix4_from_matrix4f(params->Projection * params->ViewAdjust);
    matrix[10] = ZFar / (ZNear - ZFar);
    matrix[14] = ZFar * ZNear / (ZNear - ZFar);
    matrix4_another_handed(matrix);

    pCamera->setPosition(         pRealCamera->getPosition() + pEyeRight->getAbsolutePosition());
    pCamera->setTarget  (target + pRealCamera->getPosition() + pEyeRight->getAbsolutePosition());
    //pCamera->setUpVector(up);
    pCamera->setProjectionMatrix(matrix);

    driver->setRenderTarget(pDistortionTexture, true, true, video::SColor(0, 0, 0, 0));
    CSceneManagerDecorator::drawAll();
    driver->setRenderTarget(0, false, false, video::SColor(0, 100, 100, 100));

    driver->setTransform(video::ETS_VIEW, core::matrix4());
    driver->setTransform(video::ETS_WORLD, core::matrix4());
    driver->setTransform(video::ETS_PROJECTION, core::matrix4());

    driver->setViewPort(
        core::rect<s32>(
            params->VP.x,
            params->VP.y,
            params->VP.x + params->VP.w,
            params->VP.y + params->VP.h
        )
    );
    pDistortionCallBack->LensCenter[0] = -SConfig.GetProjectionCenterOffset();
    driver->setMaterial(DistortionMaterial);
    driver->drawIndexedTriangleList(PlaneVertices, 4, PlaneIndices, 2);

    CSceneManagerDecorator::setActiveCamera(pRealCamera);
}

/*
ICameraSceneNode* COVRSceneManagerDecorator::addCameraSceneNode(ISceneNode* parent,
    const core::vector3df& position, const core::vector3df& lookat, s32 id,
    bool makeActive)
{
    // "this" makes all the difference
    ICameraSceneNode* node =
        CSceneManagerDecorator::addCameraSceneNode(parent, position, lookat, id, makeActive);

    node->setSceneManager(this);

    return node;
}
*/

void COVRSceneManagerDecorator::setActiveCamera(ICameraSceneNode *camera)
{
    if (camera) camera->grab();
    if (pRealCamera) pRealCamera->drop();

    vTarget = camera->getTarget();
    vUp = camera->getUpVector();

    pRealCamera = camera;

    CSceneManagerDecorator::setActiveCamera(camera);
}

ICameraSceneNode* COVRSceneManagerDecorator::getActiveCamera() const
{
    return pRealCamera;
}

/*
bool COVRSceneManagerDecorator::postEventFromUser(const SEvent& event)
{
    bool ret = CSceneManagerDecorator::postEventFromUser(event);

    printf("passing event");

    if (pRealCamera)
        ret = ret || pRealCamera->OnEvent(event);

    return ret;
}
*/

}
}
