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
#include "PMDefine.h"

using namespace irr;
using namespace irr::scene;
using namespace irr::core;
using namespace irr::video;



GameObjectFactory::GameObjectFactory()
:
m_avatar(0)
{

}

GameObjectFactory::~GameObjectFactory()
{
    if (m_avatar)
    {
        delete m_avatar;
        m_avatar = 0;
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

    LeafObject* leaf = CreateLeaf();
    leaf->SetOriposition(PMVector(0, 200, -100));

    CreateGravity();
    CreateResistance();

    CollidableObject* basket = CreateBasket();
    basket->SetPosition(30, 0, -100);

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

    m_avatar->m_target.SetPosition(0, 100, 50);

    m_avatar->SetPosition(PMVector(0, 0, -50));
    m_avatar->SetHeadPosition(PMVector(0, 100, -200));
}

LeafObject* GameObjectFactory::CreateLeaf()
{
    LeafObject* leaf = new LeafObject();

    IAnimatedMesh* mesh = m_mgr->getMesh("Resource/leaf.obj");
    IAnimatedMeshSceneNode* leafNode = m_mgr->addAnimatedMeshSceneNode(mesh);
    leafNode->setMaterialFlag(EMF_BACK_FACE_CULLING, false);

    leaf->SetNode(leafNode);

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

CollidableObject* GameObjectFactory::CreateBasket()
{
    IAnimatedMesh* mesh = m_mgr->getMesh("Resource/basket.obj");
    IAnimatedMeshSceneNode* node = m_mgr->addAnimatedMeshSceneNode(mesh);

    BasketObject* basket = new BasketObject();
    basket->SetNode(node);

    m_collidables.push_back(basket);

    return basket;
}

CollidableObject* GameObjectFactory::CreateWind()
{ 
    IAnimatedMesh* mesh = m_mgr->getMesh("Resource/Plane.3DS");
    IAnimatedMeshSceneNode* windNode = m_mgr->addAnimatedMeshSceneNode(mesh);
    windNode->setMaterialFlag(EMF_LIGHTING, false);
    windNode->setMaterialFlag(EMF_BACK_FACE_CULLING, false);

    IAnimatedMesh* uiMesh = m_mgr->getMesh("Resource/Plane.3DS");
    IAnimatedMeshSceneNode* uiNode = m_mgr->addAnimatedMeshSceneNode(uiMesh, windNode);
    uiNode->setMaterialFlag(EMF_LIGHTING, false);
    uiNode->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);

    ISceneNode* node = m_mgr->addCubeSceneNode(1, windNode);
    node->setPosition(vector3df(0, 0.5, 0));
    node->setMaterialFlag(EMF_WIREFRAME, true);

    GameObject* ui = new GameObject();
    ui->SetNode(uiNode);
    ui->SetVisible(false);

    WindObject* wind = new WindObject(ui);
    wind->SetNode(windNode);
    wind->ChangeTexture(TEXTURE_WIND);

    float width = MAX_VELOCITY, height = MAX_VELOCITY, range = MAX_VELOCITY;
    wind->Resize(width, height);
    wind->ChangeRange(range);

    m_collidables.push_back(wind);
    m_windows[wind->GetId()] = wind;

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
        ISceneNode* finger = m_mgr->addSphereSceneNode(3, 5, m_avatar->m_node);
        finger->setMaterialFlag(EMF_WIREFRAME, true);
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
