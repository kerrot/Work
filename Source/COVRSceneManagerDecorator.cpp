#include <stdio.h>

#include "COVRSceneManagerDecorator.h"

namespace irr
{
namespace scene
{

void COVRSceneManagerDecorator::drawAll()
{
    printf("do OVR rendering!\n");

    CSceneManagerDecorator::drawAll();
}

}
}
