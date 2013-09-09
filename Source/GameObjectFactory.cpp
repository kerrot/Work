#include "GameObjectFactory.h"
#include <irrlicht.h>
#include "GameObject.h"
#include "LeafObject.h"
#include "GravityObject.h"
#include "BasketObject.h"
#include "WindObject.h"

using namespace irr;
using namespace irr::scene;
using namespace irr::core;
using namespace irr::video;

GameObjectFactory::GameObjectFactory()
:
m_camera(0)
{

}

GameObjectFactory::~GameObjectFactory()
{
    if (m_camera)
    {
        delete m_camera;
        m_camera = 0;
    }

    for (std::vector<LeafObject*>::iterator iter = m_leaves.begin();
        iter != m_leaves.end();
        ++iter)
    {
        LeafObject* object = *iter;
        delete object;
    }
    m_leaves.clear();

    for (std::vector<CollidableObject*>::iterator iter = m_collidables.begin();
        iter != m_collidables.end();
        ++iter)
    {
        CollidableObject* object = *iter;
        delete object;
    }
    m_collidables.clear();

    m_windows.clear();
}

void GameObjectFactory::FactoryInit( ISceneManager* a_mgr )
{
    m_mgr = a_mgr;

    CreateCamera();

    LeafObject* leaf = CreateLeaf();
    leaf->SetOriposition(PMVector(0, 100, -100));

    CreateGravity();
    CollidableObject* basket = CreateBasket();
    basket->SetPosition(0, 0, -100);

    CollidableObject* wind = CreateWind();
    wind->SetPosition(0, 50, -100);
}

std::vector<LeafObject*>& GameObjectFactory::GetLeaves()
{
    return m_leaves;
}

std::vector<CollidableObject*>& GameObjectFactory::GetCollidables()
{
    return m_collidables;
}

GameObjectFactory& GameObjectFactory::GetInstance()
{
    static GameObjectFactory m_instance;

    return m_instance;
}

GameObject* GameObjectFactory::GetCamera()
{
    return m_camera;
}

void GameObjectFactory::CreateCamera()
{
    SKeyMap keyMap[9];
    keyMap[0].Action = EKA_MOVE_FORWARD;
    keyMap[0].KeyCode = KEY_UP;
    keyMap[1].Action = EKA_MOVE_FORWARD;
    keyMap[1].KeyCode = KEY_KEY_W;

    keyMap[2].Action = EKA_MOVE_BACKWARD;
    keyMap[2].KeyCode = KEY_DOWN;
    keyMap[3].Action = EKA_MOVE_BACKWARD;
    keyMap[3].KeyCode = KEY_KEY_S;

    keyMap[4].Action = EKA_STRAFE_LEFT;
    keyMap[4].KeyCode = KEY_LEFT;
    keyMap[5].Action = EKA_STRAFE_LEFT;
    keyMap[5].KeyCode = KEY_KEY_A;

    keyMap[6].Action = EKA_STRAFE_RIGHT;
    keyMap[6].KeyCode = KEY_RIGHT;
    keyMap[7].Action = EKA_STRAFE_RIGHT;
    keyMap[7].KeyCode = KEY_KEY_D;

    keyMap[8].Action = EKA_JUMP_UP;
    keyMap[8].KeyCode = KEY_KEY_J;

    ICameraSceneNode* cam = m_mgr->addCameraSceneNodeFPS(0, 25.0f, .1f, -1, keyMap, 9, false, 3.f);
    
    m_camera = new GameObject();
    m_camera->SetNode(cam);

    m_camera->SetPosition(0, 0, -50);
}

LeafObject* GameObjectFactory::CreateLeaf()
{
    LeafObject* leaf = new LeafObject();

    ISceneNode* leafNode = m_mgr->addSphereSceneNode(3);

    leaf->SetNode(leafNode);

    m_leaves.push_back(leaf);

    return leaf;
}

void GameObjectFactory::CreateGravity()
{
    m_collidables.push_back(new GravityObject());
}

CollidableObject* GameObjectFactory::CreateBasket()
{
    ISceneNode* node = m_mgr->addCubeSceneNode(30);

    BasketObject* basket = new BasketObject();
    basket->SetNode(node);

    m_collidables.push_back(basket);

    return basket;
}

CollidableObject* GameObjectFactory::CreateWind()
{
    ISceneNode* node = m_mgr->addCubeSceneNode(1);
    node->setScale(vector3df(25, 20, 15));
    SMaterial &m = node->getMaterial(0);
    m.Wireframe = true;

    WindObject* wind = new WindObject();
    wind->SetNode(node);

    m_collidables.push_back(wind);
    m_windows[wind->GetId()] = wind;

    return wind;
}
