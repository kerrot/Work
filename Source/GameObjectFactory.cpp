#include "GameObjectFactory.h"
#include <irrlicht.h>

using namespace irr;
using namespace irr::scene;

GameObjectFactory* GameObjectFactory::m_instance = 0;

GameObjectFactory::GameObjectFactory()
{

}

GameObjectFactory::~GameObjectFactory()
{
    if (m_instance)
    {
        delete m_instance;
        m_instance = 0;
    }
}

void GameObjectFactory::FactoryInit( ISceneManager* a_mgr )
{
    m_mgr = a_mgr;

    ISceneNode* o = m_mgr->addSphereSceneNode(20);
    //o->setMaterialFlag(video::EMF_LIGHTING, false);
    o->setPosition(core::vector3df(0,0,0));
    video::SMaterial &m = o->getMaterial(0);
    m.EmissiveColor = video::SColor(255, 255, 0, 0);

    scene::ICameraSceneNode* cam = m_mgr->addCameraSceneNodeFPS();
    core::vector3df camPos(0,0,200);
    cam->setPosition(camPos);
    cam->setTarget(core::vector3df(0,0,0));
}

const std::vector<LeafObject*>& GameObjectFactory::GetLeaves()
{
    return m_leaves;
}

const std::vector<CollidableObject*>& GameObjectFactory::GetCollidables()
{
    return m_collidables;
}

GameObjectFactory* GameObjectFactory::GetInstance()
{
    if (m_instance == 0)
    {
        m_instance = new GameObjectFactory();
    }

    return m_instance;
}
