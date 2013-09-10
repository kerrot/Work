#pragma once

#include "PMCommon.h"

namespace irr
{
    namespace scene
    {
        class ISceneManager;
    }
}

class GameObject;
class LeafObject;
class HandObject;
class CollidableObject;

class WindowInterface;

class GameObjectFactory
{
public:
    static GameObjectFactory& GetInstance();

    void FactoryInit(irr::scene::ISceneManager* a_mgr);

    LeafObject* CreateLeaf();
    CollidableObject* CreateWind();

    GameObject* GetCamera();
    std::vector<LeafObject*>& GetLeaves();
    std::vector<CollidableObject*>& GetCollidables();

    ~GameObjectFactory();

private:
    GameObjectFactory();

    void CreateCamera();
    void CreateGravity();
    void CreateResistance();
    CollidableObject* CreateBasket();

    std::vector<GameObject*> m_gameObjects;
    std::vector<LeafObject*> m_leaves;
    std::vector<HandObject*> m_hands;
    std::vector<CollidableObject*> m_collidables;

    std::map<UInt32, WindowInterface*> m_windows;

    irr::scene::ISceneManager* m_mgr;

    GameObject* m_camera;
};

#define sGameObjectFactory GameObjectFactory::GetInstance()