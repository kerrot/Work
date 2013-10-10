#include "GameObjectFactory.h"
#include <irrlicht.h>
#include "GameObject.h"
#include "LeafObject.h"
#include "GravityObject.h"
#include "BasketObject.h"
#include "WindObject.h"
#include "ResistanceObject.h"
#include "HandObject.h"
#include "AvatarObject.h"
#include "MenuUIObject.h"
#include "ButtonObject.h"
#include "PMDefine.h"

using namespace irr;
using namespace irr::scene;
using namespace irr::core;
using namespace irr::video;

GameObjectFactory::GameObjectFactory()
:
m_avatar(0)
,m_menuUI(0)
{

}

GameObjectFactory::~GameObjectFactory()
{
    if (m_avatar)
    {
        delete m_avatar;
        m_avatar = 0;
    }

    if (m_menuUI)
    {
        delete m_menuUI;
        m_menuUI = 0;
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
    m_mainWindows.clear();

    for (std::map<UInt32, HandObject*>::iterator iter = m_hands.begin();
        iter != m_hands.end();
        ++iter)
    {
        HandObject* hand = iter->second;
        delete hand;
    }
    m_hands.clear();

    for (std::list<GameObject*>::iterator iter = m_display.begin();
        iter != m_display.end();
        ++iter)
    {
        GameObject* object = *iter;
        delete object;
    }
    m_display.clear();
}

void GameObjectFactory::FactoryInit( ISceneManager* a_mgr, irr::video::IVideoDriver* a_driver)
{
    m_mgr = a_mgr;
    m_driver = a_driver;

    CreateAvatar();
    CreateGravity();
    CreateResistance();
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

AvatarObject* GameObjectFactory::GetAvatar()
{
    return m_avatar;
}

void GameObjectFactory::CreateAvatar()
{
    m_avatar = new AvatarObject();

    ISceneNode* node = m_mgr->addSphereSceneNode(15);
    node->setMaterialFlag(EMF_LIGHTING, false);
    m_avatar->SetNode(node);
    
    ICameraSceneNode* cam = m_mgr->addCameraSceneNode(node);
    m_avatar->m_head.SetNode(cam);
    m_avatar->m_target.SetNode(m_mgr->addSphereSceneNode(3, 16, node));

    m_avatar->m_target.SetPosition(0, 80, 1000);

    m_avatar->SetPosition(PMVector(-10, 00, 350));
    m_avatar->SetHeadPosition(PMVector(0, 80, -300));
}

LeafObject* GameObjectFactory::CreateLeaf()
{
    LeafObject* leaf = new LeafObject();

    IAnimatedMesh* mesh = m_mgr->getMesh("Resource/leaf.obj");
    IAnimatedMeshSceneNode* leafNode = m_mgr->addAnimatedMeshSceneNode(mesh);
    leafNode->setMaterialFlag(EMF_BACK_FACE_CULLING, false);

    leaf->SetNode(leafNode);
    
    UInt32 angleX = rand() % 30;
    UInt32 angleZ = rand() % 30;

    leaf->SetRotation(PMVector(angleX, 0, angleZ));

    m_leaves.push_back(leaf);

    return leaf;
}

void GameObjectFactory::CreateGravity()
{
    m_collidables.push_back(new GravityObject());
}

void GameObjectFactory::CreateResistance()
{
    m_collidables.push_back(new ResistanceObject());
}

BasketObject* GameObjectFactory::CreateBasket()
{
    IAnimatedMesh* mesh = m_mgr->getMesh("Resource/basket.obj");
    IAnimatedMeshSceneNode* node = m_mgr->addAnimatedMeshSceneNode(mesh);

    IAnimatedMesh* uiMesh = m_mgr->getMesh("Resource/Plane.3DS");
    IAnimatedMeshSceneNode* uiNode = m_mgr->addAnimatedMeshSceneNode(uiMesh, node);
    uiNode->setMaterialFlag(EMF_LIGHTING, false);
    uiNode->setMaterialFlag(EMF_BACK_FACE_CULLING, false);

    GameObject* ui = new GameObject();
    ui->SetNode(uiNode);
    m_display.push_back(ui);

    BasketObject* basket = new BasketObject(ui);
    basket->SetNode(node);

    ui->SetPosition(0, 50, 0);
    ui->ChangeTexture(TEXTURE_RESIZE);
    ui->SetScale(PMVector(100, 0, 60));

    m_collidables.push_back(basket);

    return basket;
}

CollidableObject* GameObjectFactory::CreateWind()
{ 
    IAnimatedMesh* mesh = m_mgr->getMesh("Resource/Plane.3DS");
    IAnimatedMeshSceneNode* windNode = m_mgr->addAnimatedMeshSceneNode(mesh);
    windNode->setMaterialFlag(EMF_LIGHTING, false);
    windNode->setMaterialFlag(EMF_BACK_FACE_CULLING, false);
    windNode->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);

    IAnimatedMesh* uiMesh = m_mgr->getMesh("Resource/Plane.3DS");
    IAnimatedMeshSceneNode* uiNode = m_mgr->addAnimatedMeshSceneNode(uiMesh, windNode);
    uiNode->setMaterialFlag(EMF_LIGHTING, false);
    uiNode->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);
    uiNode->setMaterialFlag(EMF_BACK_FACE_CULLING, false);

    ISceneNode* node = m_mgr->addCubeSceneNode(1, windNode);
    node->setPosition(vector3df(0, 0.5, 0));
    node->setMaterialFlag(EMF_WIREFRAME, true);
    
    GameObject* rangeUI = new GameObject();
    rangeUI->SetNode(node);
    m_display.push_back(rangeUI);

    GameObject* ui = new GameObject();
    ui->SetNode(uiNode);
    ui->SetVisible(false);
    m_display.push_back(ui);

    WindObject* wind = new WindObject(ui, rangeUI);
    wind->SetNode(windNode);
    wind->ChangeTexture(TEXTURE_WIND);

    float width = MAX_VELOCITY, height = MAX_VELOCITY, range = MAX_VELOCITY;
    wind->Resize(width, height);
    wind->ChangeRange(range);

    m_collidables.push_back(wind);
    m_windows[wind->GetId()] = wind;
    m_mainWindows[wind->GetId()] = wind;

    return wind;
}

WindowInterface* GameObjectFactory::GetWindowByID( UInt32 a_id )
{
    std::map<UInt32, WindowInterface*>::iterator iter = m_windows.find(a_id);
    if (iter != m_windows.end())
    {
        return iter->second;
    }

    return 0; 
}

HandObject* GameObjectFactory::GetorCreateHand( UInt32 a_id )
{
    std::map<UInt32, HandObject*>::iterator iter = m_hands.find(a_id);
    if (iter != m_hands.end())
    {
        return iter->second;
    }

    IAnimatedMesh* mesh = m_mgr->getMesh("Resource/Hand.3DS");
    IAnimatedMeshSceneNode* node = m_mgr->addAnimatedMeshSceneNode(mesh, m_avatar->m_node);
    node->setScale(vector3df(10, 5, 10));
    node->setMaterialFlag(EMF_WIREFRAME, true);
    

    HandObject* hand = new HandObject(a_id);
    hand->SetNode(node);

    for (int i = 0; i < MAX_FINGERS; ++i)
    {
        ISceneNode* finger = m_mgr->addSphereSceneNode(1, 3, m_avatar->m_node);
        ISceneNode* fingerFrame = m_mgr->addSphereSceneNode(5, 5, finger);
        fingerFrame->setMaterialFlag(EMF_WIREFRAME, true);
        hand->m_fingles[i].SetNode(finger);
    }

    m_hands[a_id] = hand;

    return hand;
}

void GameObjectFactory::HideAllHand()
{
    for (std::map<UInt32, HandObject*>::iterator iter = m_hands.begin();
        iter != m_hands.end();
        ++iter)
    {
        iter->second->SetVisible(false);
    }
}

std::map<UInt32, WindowInterface*>& GameObjectFactory::GetWindows()
{
    return m_windows;
}

GameObject* GameObjectFactory::CreatePlaneShadow()
{
    IAnimatedMesh* mesh = m_mgr->getMesh("Resource/Plane.3DS");
    IAnimatedMeshSceneNode* node = m_mgr->addAnimatedMeshSceneNode(mesh);
    node->setMaterialFlag(EMF_LIGHTING, false);
    node->setMaterialFlag(EMF_BACK_FACE_CULLING, false);
    node->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);
    node->setMaterialTexture( 0, m_driver->getTexture("Resource/shadow.png"));

    GameObject* shadow = new GameObject();
    shadow->SetNode(node);

    m_display.push_back(shadow);

    return shadow;
}

std::map<UInt32, MainWindowInterface*>& GameObjectFactory::GetMainWindows()
{
    return m_mainWindows;
}

MenuUIObject* GameObjectFactory::GetMenuUI()
{
    if (m_menuUI)
    {
        m_menuUI->SetVisible(true);
        m_menuUI->SetEnabled(true);
        PMVector avatarPos = m_avatar->GetAbsolutePosition();
        PMVector headPos = m_avatar->GetHeadPosition();
        avatarPos.y += headPos.y;
        m_menuUI->SetPosition(avatarPos);
        m_menuUI->SetRotation(-90, 0, 0);
        return m_menuUI;
    }
    
    IAnimatedMesh* mesh = m_mgr->getMesh("Resource/Plane.3DS");
    IAnimatedMeshSceneNode* menuNode = m_mgr->addAnimatedMeshSceneNode(mesh);
    menuNode->setMaterialFlag(EMF_LIGHTING, false);
    menuNode->setMaterialFlag(EMF_BACK_FACE_CULLING, false);

    IAnimatedMesh* uiMesh = m_mgr->getMesh("Resource/Plane.3DS");
    IAnimatedMeshSceneNode* uiNode = m_mgr->addAnimatedMeshSceneNode(uiMesh, menuNode);
    uiNode->setMaterialFlag(EMF_LIGHTING, false);
    uiNode->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);
    uiNode->setMaterialFlag(EMF_BACK_FACE_CULLING, false);

    GameObject* ui = new GameObject();
    ui->SetNode(uiNode);
    ui->SetVisible(false);
    m_display.push_back(ui);

    m_menuUI = new MenuUIObject(ui);
    m_menuUI->SetNode(menuNode);
    m_menuUI->ChangeTexture(TEXTURE_RESIZE);

    float width = MAX_VELOCITY, height = MAX_VELOCITY;
    m_menuUI->Resize(width, height);
    m_menuUI->SetPosition(m_avatar->m_target.GetAbsolutePosition());
    m_menuUI->SetRotation(-90, 0, 0);

    m_menuUI->m_closeButton = CreateButton();
    m_menuUI->m_closeButton->m_node->setParent(menuNode);
    m_menuUI->m_closeButton->ChangeTexture(TEXTURE_CLOSE_NORMAL);

    m_menuUI->m_startButton = CreateButton();
    m_menuUI->m_startButton->m_node->setParent(menuNode);
    m_menuUI->m_startButton->ChangeTexture(TEXTURE_CLOSE_NORMAL);

    m_menuUI->Init();

    m_windows[m_menuUI->GetId()] = m_menuUI;
    m_mainWindows[m_menuUI->GetId()] = m_menuUI;

    return m_menuUI;
}

ButtonObject* GameObjectFactory::CreateButton()
{
    IAnimatedMesh* mesh = m_mgr->getMesh("Resource/Plane.3DS");
    IAnimatedMeshSceneNode* buttonNode = m_mgr->addAnimatedMeshSceneNode(mesh);
    buttonNode->setMaterialFlag(EMF_LIGHTING, false);
    buttonNode->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);
    ButtonObject* btn = new ButtonObject();
    btn->SetNode(buttonNode);
    
    m_display.push_back(btn);
    m_windows[btn->GetId()] = btn;

    return btn;
}
