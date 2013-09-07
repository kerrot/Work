#include "GameObjectFactory.h"
#include <irrlicht.h>
#include "GameObject.h"

using namespace irr;
using namespace irr::scene;

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
}

void GameObjectFactory::FactoryInit( ISceneManager* a_mgr )
{
    m_mgr = a_mgr;

    CreateCamera();
}

const std::vector<LeafObject*>& GameObjectFactory::GetLeaves()
{
    return m_leaves;
}

const std::vector<CollidableObject*>& GameObjectFactory::GetCollidables()
{
    return m_collidables;
}

GameObjectFactory& GameObjectFactory::GetInstance()
{
    static GameObjectFactory m_instance;

    return m_instance;
}

const GameObject* GameObjectFactory::GetCamera()
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
    
    m_camera = new GameObject(cam);
    m_camera->SetPosition(0, 0, -50);
}
