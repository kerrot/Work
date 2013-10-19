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

class GameObject;
class AvatarObject;
class LeafObject;
class HandObject;
class CollidableObject;
class BasketObject;

class WindowInterface;
class MainWindowInterface;
class MenuUIObject;
class ComicUIObject;
class ButtonObject;

struct PMVector;

struct ShadowData
{
    float x;
    float y;
};

class GameObjectFactory
{
public:
    static GameObjectFactory& GetInstance();

    void FactoryInit(irr::scene::ISceneManager* a_mgr, irr::video::IVideoDriver* a_driver);

    LeafObject* CreateLeaf();
    CollidableObject* CreateWind();
    BasketObject* CreateBasket();
    GameObject* CreatePlaneShadow();

    HandObject* GetorCreateHand(UInt32 a_id);
    WindowInterface* GetWindowByID(UInt32 a_id);
    AvatarObject* GetAvatar();
    MenuUIObject* GetMenuUI();
    ComicUIObject* GetComicUI();
    std::vector<LeafObject*>& GetLeaves();
    std::vector<CollidableObject*>& GetCollidables();
    std::map<UInt32, WindowInterface*>& GetWindows();
    std::map<UInt32, MainWindowInterface*>& GetMainWindows();

    
    std::pair<UInt32, UInt32> LoadTexture(GameObject* a_object, std::string a_name);
    void DrawShadow(GameObject* a_object, std::vector<ShadowData>& a_shadows, PMVector a_shift);

    void HideAllHand();

    ~GameObjectFactory();

private:
    GameObjectFactory();

    void CreateAvatar();
    void CreateGravity();
    void CreateResistance();
    ButtonObject* CreateButton();

    std::vector<LeafObject*> m_leaves;
    std::vector<CollidableObject*> m_collidables;

    std::map<UInt32, WindowInterface*> m_windows;
    std::map<UInt32, MainWindowInterface*> m_mainWindows;
    std::map<UInt32, HandObject*> m_hands;
    std::list<GameObject*> m_display;

    irr::scene::ISceneManager* m_mgr;
    irr::video::IVideoDriver* m_driver;

    AvatarObject* m_avatar;

    MenuUIObject* m_menuUI;
    ComicUIObject* m_comicUI;
    GameObject* m_shadowDisplay;
};

#define sGameObjectFactory GameObjectFactory::GetInstance()