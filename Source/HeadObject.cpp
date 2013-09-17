#include "HeadObject.h"
#include "PMType.h"
#include <irrlicht.h>

using namespace irr;
using namespace irr::scene;
using namespace irr::core;

HeadObject::HeadObject()
{

}

HeadObject::~HeadObject()
{

}

void HeadObject::SetTarget( PMVector a_target ) const
{
    ICameraSceneNode* cam = dynamic_cast<ICameraSceneNode*>(m_node);
    if (cam)
    {
        cam->setTarget(vector3df(a_target.x, a_target.y, a_target.z));
    }
}
