#pragma once

#include <vector>

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

class GameObjectFactory
{
public:
    static GameObjectFactory& GetInstance();

    void FactoryInit(irr::scene::ISceneManager* a_mgr);

    const GameObject* GetCamera();
    const std::vector<LeafObject*>& GetLeaves();
    const std::vector<CollidableObject*>& GetCollidables();

    ~GameObjectFactory();

private:
    GameObjectFactory();

    void CreateCamera();

    std::vector<GameObject*> m_gameObjects;
    std::vector<LeafObject*> m_leaves;
    std::vector<HandObject*> m_hands;
    std::vector<CollidableObject*> m_collidables;

    irr::scene::ISceneManager* m_mgr;

    GameObject* m_camera;
};

#define sGameObjectFactory GameObjectFactory::GetInstance()