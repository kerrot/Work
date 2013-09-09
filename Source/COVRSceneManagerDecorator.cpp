#include <stdio.h>
#include <IVideoDriver.h>
#include <IGPUProgrammingServices.h>
#include <ICameraSceneNode.h>
#include <EShaderTypes.h>

#include "COVRSceneManagerDecorator.h"
#include "GameMatrix4.h"

namespace irr
{
/* ugly handmade private functions */
void vector3df_print(core::vector3df v) {
    printf(
        "{ X: %f, Y: %f, Z: %f }\n",
        v.X, v.Y, v.Z
    );
    return;
}

void camera_print(irr::scene::ICameraSceneNode* cam) {
    printf("  position: ");
    vector3df_print(cam->getPosition());
    printf("  rotation: ");
    vector3df_print(cam->getRotation());
    printf("  target: ");
    vector3df_print(cam->getTarget());
    printf("  up: ");
    vector3df_print(cam->getUpVector());
    return;
}

namespace scene
{

COVRSceneManagerDecorator::COVRSceneManagerDecorator(ISceneManager *smgr)
    :CSceneManagerDecorator(smgr)
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
        SConfig.SetHMDInfo(Info);
    }   

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

    // strategy: if we keep it, we grab it
    pHead = smgr->addEmptySceneNode();
    pHead->grab();

    pHeadX = smgr->addEmptySceneNode(pHead);
    pHeadX->grab();

    pHeadY = smgr->addEmptySceneNode(pHeadX);
    pHeadY->grab();

    pHeadZ = smgr->addEmptySceneNode(pHeadY);
    pHeadZ->grab();

    pTarget = smgr->addEmptySceneNode(pHeadZ);
    pTarget->grab();

    pUp = smgr->addEmptySceneNode(pHeadZ);
    pUp->grab();

    f32 offset = SConfig.GetIPD() / 2.0f;

    pCameraLeft = smgr->addCameraSceneNode(
        pHeadZ,
        core::vector3df( offset, 0,   0),
        core::vector3df( 0, 0, 100),
        -1,
        false // not active yet
    );
    pCameraLeft->grab();

    pCameraRight = smgr->addCameraSceneNode(
        pHeadZ,
        core::vector3df(-offset, 0,   0),
        core::vector3df( 0, 0, 100),
        -1,
        false // not active yet
    );
    pCameraRight->grab();
}

COVRSceneManagerDecorator::~COVRSceneManagerDecorator()
{
    // we grabbed callback and texture when we new them
    pDistortionCallBack->drop();
    pDistortionTexture->drop();
    // we grabbed pRealCamera when we get it, so we drop it now
    pRealCamera->drop();
    // if we grabbed them, we drop them
    pCameraLeft->drop();
    pCameraRight->drop();
    pUp->drop();
    pTarget->drop();
    pHeadZ->drop();
    pHeadY->drop();
    pHeadX->drop();
    pHead->drop();
    // then let scene manager drops all its children

    /*
    printf(
        "ref counts:\n"
        "  distortion callback: %d\n"
        "  distortion texture: %d\n"
        "  real camera: %d\n"
        "  left camera: %d\n"
        "  right camera: %d\n"
        "  head x: %d\n"
        "  head y: %d\n"
        "  head z: %d\n"
        "  head: %d\n",
        pDistortionCallBack->getReferenceCount(),
        pDistortionTexture->getReferenceCount(),
        pRealCamera->getReferenceCount(),
        pCameraLeft->getReferenceCount(),
        pCameraRight->getReferenceCount(),
        pHeadX->getReferenceCount(),
        pHeadY->getReferenceCount(),
        pHeadZ->getReferenceCount(),
        pHead->getReferenceCount()
    );
    */
}

void COVRSceneManagerDecorator::drawAll()
{
    video::IVideoDriver* driver = SceneManager->getVideoDriver();
    if (!driver) return;

    pRealCamera = SceneManager->getActiveCamera();
    if (!pRealCamera) return;

    // draw nothing, only update animators
    f32 zNear = pRealCamera->getNearValue(),
        zFar  = pRealCamera->getFarValue();

    pRealCamera->setFarValue(pRealCamera->getNearValue() + .1f);
    CSceneManagerDecorator::drawAll();
    pRealCamera->setFarValue(zFar);

    // if setActiveCamera is invoked from the inside of the decorator
    if (pRealCamera != pHead->getParent()) mimicCamera();

    // update rotation for HMD
    core::vector3df offset;
    f32 pitch = 0,
        yaw = 0,
        roll = 0;

    if (HMDManager->EnumerateDevices<OVR::HMDDevice>().IsAvailable()) {
        Fusion.GetOrientation().GetEulerAngles<OVR::Axis_X, OVR::Axis_Y, OVR::Axis_Z>(&pitch, &yaw, &roll);
    }

    pitch *= -core::RADTODEG;
    yaw   *= -core::RADTODEG;
    roll  *=  core::RADTODEG;

    pHeadX->setRotation(irr::core::vector3df( pitch,   0,    0));
    pHeadY->setRotation(irr::core::vector3df(     0, yaw,    0));
    pHeadZ->setRotation(irr::core::vector3df(     0,   0, roll));

    //bind camera target to HMD
    //pRealCamera->setRotation(pRealCamera->getRotation() + target);

    // render
    GameMatrix4 matrix;
    const StereoEyeParams *params;

    // left eye
    params = &SConfig.GetEyeRenderParams(StereoEye_Left);
    matrix = params->Projection * params->ViewAdjust;
    matrix[10] = zFar / (zNear - zFar);
    matrix[14] = zFar * zNear / (zNear - zFar);
    matrix.switchHanded();

    offset = pCameraLeft->getAbsolutePosition() - pRealCamera->getAbsolutePosition();

    CSceneManagerDecorator::setActiveCamera(pCameraLeft);
    pCameraLeft->bindTargetAndRotation(pRealCamera->getTargetAndRotationBinding());
    pCameraLeft->setTarget(pTarget->getAbsolutePosition() + offset);
    pCameraLeft->setUpVector(pUp->getAbsolutePosition() - pHead->getAbsolutePosition());
    pCameraLeft->setProjectionMatrix(matrix);

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
    matrix = params->Projection * params->ViewAdjust;
    matrix[10] = zFar / (zNear - zFar);
    matrix[14] = zFar * zNear / (zNear - zFar);
    matrix.switchHanded();

    offset = pCameraRight->getAbsolutePosition() - pRealCamera->getAbsolutePosition();

    CSceneManagerDecorator::setActiveCamera(pCameraRight);
    pCameraRight->bindTargetAndRotation(pRealCamera->getTargetAndRotationBinding());
    pCameraRight->setTarget(pTarget->getAbsolutePosition() + offset);
    pCameraRight->setUpVector(pUp->getAbsolutePosition() - pHead->getAbsolutePosition());
    pCameraRight->setProjectionMatrix(matrix);

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

    pRealCamera = camera;

    mimicCamera();

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

void COVRSceneManagerDecorator::mimicCamera()
{
    pRealCamera->addChild(pHead);
    pTarget->setPosition(pRealCamera->getTarget() - pRealCamera->getAbsolutePosition());
    pUp->setPosition(pRealCamera->getUpVector());
}

}
}
