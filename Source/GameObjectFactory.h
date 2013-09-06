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
    static GameObjectFactory* GetInstance();

    void FactoryInit(irr::scene::ISceneManager* a_mgr);

    const std::vector<LeafObject*>& GetLeaves();
    const std::vector<CollidableObject*>& GetCollidables();

    ~GameObjectFactory();

private:
    static GameObjectFactory* m_instance;

    GameObjectFactory();

    std::vector<GameObject*> m_gameObjects;
    std::vector<LeafObject*> m_leaves;
    std::vector<HandObject*> m_hands;
    std::vector<CollidableObject*> m_collidables;

    irr::scene::ISceneManager* m_mgr;
};

#define sGameObjectFactory GameObjectFactory::GetInstance()