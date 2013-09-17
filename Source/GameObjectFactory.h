#pragma once

#include "PMCommon.h"

namespace irr
{
    namespace scene
    {
        class ISceneManager;
    }

    namespace video
    {
        class IVideoDriver;
    }
}

class AvatarObject;
class LeafObject;
class HandObject;
class CollidableObject;

class WindowInterface;

class GameObjectFactory
{
public:
    static GameObjectFactory& GetInstance();

    void FactoryInit(irr::scene::ISceneManager* a_mgr, irr::video::IVideoDriver* a_driver);

    LeafObject* CreateLeaf();
    CollidableObject* CreateWind();

    HandObject* GetorCreateHand(UInt32 a_id);
    WindowInterface* GetWindowByID(UInt32 a_id);
    AvatarObject* GetAvatar();
    std::vector<LeafObject*>& GetLeaves();
    std::vector<CollidableObject*>& GetCollidables();

    void HideAllHand();

    ~GameObjectFactory();

private:
    GameObjectFactory();

    void CreateAvatar();
    void CreateGravity();
    void CreateResistance();
    CollidableObject* CreateBasket();

    //std::vector<GameObject*> m_gameObjects;
    std::vector<LeafObject*> m_leaves;
    std::vector<CollidableObject*> m_collidables;

    std::map<UInt32, WindowInterface*> m_windows;
    std::map<UInt32, HandObject*> m_hands;

    irr::scene::ISceneManager* m_mgr;
    irr::video::IVideoDriver* m_driver;

    AvatarObject* m_avatar;
};

#define sGameObjectFactory GameObjectFactory::GetInstance()