#ifndef __C_OVR_SCENE_MANAGER_DECORATOR__
#define __C_OVR_SCENE_MANAGER_DECORATOR__

#include "CSceneManagerDecorator.h"

namespace irr
{
namespace scene
{

    class COVRSceneManagerDecorator : public CSceneManagerDecorator
    {
    public:

        COVRSceneManagerDecorator(ISceneManager* smgr) : CSceneManagerDecorator(smgr) {}

        virtual void drawAll();
    };

}
}

#endif
