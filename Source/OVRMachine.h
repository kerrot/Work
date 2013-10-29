#pragma once

#include "irrlicht.h"

using namespace irr;
using namespace scene;
using namespace video;
using namespace gui;
using namespace core;

class OVRMachine
{
public:
    static OVRMachine& GetInstance();

    void Init(IrrlichtDevice* a_device);
    void SetMainCamera(ICameraSceneNode* a_camera);
    void Draw();
    bool IsRiftAvailable();
    bool IsRiftSceneMode();

    void Test(float a_x, float a_y, float a_z);

    ~OVRMachine();
private:
    OVRMachine();

    bool m_riftAvailable;
    bool m_riftSceneMode;

    IVideoDriver* m_driver;
    ISceneManager* m_smgr;
    IGUIEnvironment* m_env;

    ISceneNode *m_pHeadX;
    ISceneNode *m_pHeadY;
    ISceneNode *m_pHeadZ;
    ISceneNode *m_pYaw;
    ISceneNode *m_pLeftEye;
    ISceneNode *m_pRghtEye;
    ISceneNode *m_pTarget;
    ISceneNode *m_pUp;

    ICameraSceneNode* m_mainCamera;
    ICameraSceneNode* m_riftCamera;

    ITexture *m_pRenderTexture;
    f32 m_fLensShift;

    S3DVertex m_cPlaneVertices[4];
    u16 m_iPlaneIndices[6];

    SMaterial m_cRenderMaterial;

    matrix4 m_cProjectionLeft;
    matrix4 m_cProjectionRght;

    float m_x;
    float m_y;
    float m_z;
};

#define sOVRMachine OVRMachine::GetInstance()