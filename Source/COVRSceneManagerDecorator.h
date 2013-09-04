#ifndef __C_OVR_SCENE_MANAGER_DECORATOR__
#define __C_OVR_SCENE_MANAGER_DECORATOR__

#include <IShaderConstantSetCallBack.h>
#include <IMaterialRendererServices.h>
#include <OVR.h>

#include "CSceneManagerDecorator.h"

using namespace OVR;
using namespace OVR::Util::Render;

namespace irr
{

    class RiftDistortionCallBack : public video::IShaderConstantSetCallBack
    {   
    public:
        f32 Scale[2],
            ScaleIn[2],
            ScreenCenter[2],
            LensCenter[2],
            HMDWarpParam[4];

        virtual void OnSetConstants(video::IMaterialRendererServices *a_pServices, s32 a_iUserData)
        {   
            a_pServices->setPixelShaderConstant("scale"       , Scale       , 2); 
            a_pServices->setPixelShaderConstant("scaleIn"     , ScaleIn     , 2); 
            a_pServices->setPixelShaderConstant("lensCenter"  , LensCenter  , 2); 
            a_pServices->setPixelShaderConstant("hmdWarpParam", HMDWarpParam, 4); 
        }   
    };

namespace scene
{

    class COVRSceneManagerDecorator : public CSceneManagerDecorator
    {
    public:

        COVRSceneManagerDecorator(ISceneManager* smgr);

        ~COVRSceneManagerDecorator();

        virtual void drawAll();

        /*
        virtual ICameraSceneNode* addCameraSceneNode(ISceneNode* parent = 0,
            const core::vector3df& position = core::vector3df(0,0,0),
            const core::vector3df& lookat = core::vector3df(0,0,100),
            s32 id=-1, bool makeActive=true);
        */

        virtual void setActiveCamera(ICameraSceneNode* camera);
        
        virtual ICameraSceneNode* getActiveCamera() const;

        //virtual bool postEventFromUser(const SEvent& event);

    protected:

        /*
        f32 ZNear;
        f32 ZFar;
        */

        Ptr<DeviceManager> HMDManager;
        Ptr<HMDDevice> HMD;
        Ptr<SensorDevice> Sensor;
        HMDInfo Info;
        SensorFusion Fusion;
        StereoConfig SConfig;
        RiftDistortionCallBack *pDistortionCallBack;

        video::SMaterial DistortionMaterial;
        video::ITexture *pDistortionTexture;
        video::S3DVertex PlaneVertices[4];
        u16 PlaneIndices[6];

        ISceneNode *pHead;
        ISceneNode *pHeadX;
        ISceneNode *pHeadY;
        ISceneNode *pHeadZ;
        ISceneNode *pEyeLeft;
        ISceneNode *pEyeRight;
        //core::vector3df vTarget;
        //core::vector3df vUp;
        ICameraSceneNode *pRealCamera;
        ICameraSceneNode *pCameraLeft;
        ICameraSceneNode *pCameraRight;

    private:

        void mimicCamera();

    };

} // end of namespace scene
} // end of namespace irr

#endif
