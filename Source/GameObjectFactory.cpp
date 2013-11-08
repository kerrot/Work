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
#include "ComicUIObject.h"
#include "PMDefine.h"

#include "OVRMachine.h"

using namespace irr;
using namespace irr::scene;
using namespace irr::core;
using namespace irr::video;

#define UI_DEFAULT_Y  150
#define UI_DEFAULT_Z  200

GameObjectFactory::GameObjectFactory()
:
m_avatar(0)
,m_menuUI(0)
,m_comicUI(0)
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

    if (m_comicUI)
    {
        delete m_comicUI;
        m_comicUI = 0;
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

void GameObjectFactory::FactoryInit(IrrlichtDevice* a_device)
{
    m_device = a_device;
    m_driver = a_device->getVideoDriver();
    m_mgr = a_device->getSceneManager();

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

    ISceneNode* node = m_mgr->addEmptySceneNode();
    node->setMaterialFlag(EMF_LIGHTING, false);
    m_avatar->SetNode(node);
    
    ICameraSceneNode* cam = m_mgr->addCameraSceneNode(node);

    if (sOVRMachine.IsRiftSceneMode())
    {
        sOVRMachine.SetMainCamera(cam);
    }   

    m_avatar->m_head.SetNode(cam);
    m_avatar->m_target.SetNode(m_mgr->addEmptySceneNode(node));

    m_avatar->m_target.SetPosition(0, 80, 1000);

    m_avatar->SetPosition(PMVector(-10, 0, 50));
    m_avatar->SetHeadPosition(PMVector(0, 80, 0));
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

    IBillboardTextSceneNode* text = m_mgr->addBillboardTextSceneNode(m_device->getGUIEnvironment()->getBuiltInFont(), L"Clear", node, dimension2df(100, 100), vector3df(0, 100, 0));
    GameObject* textNode = new GameObject();
    textNode->SetNode(text);
    textNode->SetVisible(false);
    m_display.push_back(textNode);

    BasketObject* basket = new BasketObject(ui, textNode);
    basket->SetNode(node);

    ui->SetPosition(0, 50, 0);
    ui->ChangeTexture(TEXTURE_CLOSE_PRESS);
    ui->SetScale(PMVector(100, 0, 60));
    ui->SetVisible(false);

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

    IAnimatedMesh* shadowMesh = m_mgr->getMesh("Resource/Plane.3DS");
    IAnimatedMeshSceneNode* shadowNode = m_mgr->addAnimatedMeshSceneNode(shadowMesh);
    shadowNode->setMaterialFlag(EMF_LIGHTING, false);
    shadowNode->setMaterialFlag(EMF_BACK_FACE_CULLING, false);
    shadowNode->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);
    GameObject* shadow = new GameObject();
    shadow->SetNode(shadowNode);
    shadow->SetScale(PMVector(256, 1, 256));
    shadow->m_renderTarget = m_driver->addRenderTargetTexture(dimension2d<u32>(2048, 2048));
    m_display.push_back(shadow);

    WindObject* wind = new WindObject(ui, rangeUI, shadow);
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
        avatarPos.y += UI_DEFAULT_Y;
        avatarPos.z += UI_DEFAULT_Z;
        m_menuUI->SetPosition(avatarPos);
        m_menuUI->SetRotation(-90, 0, 0);
        return m_menuUI;
    }
    
    IAnimatedMesh* mesh = m_mgr->getMesh("Resource/Plane.3DS");
    IAnimatedMeshSceneNode* menuNode = m_mgr->addAnimatedMeshSceneNode(mesh);
    menuNode->setMaterialFlag(EMF_LIGHTING, false);
    menuNode->setMaterialFlag(EMF_BACK_FACE_CULLING, false);
    menuNode->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);

    IAnimatedMesh* uiMesh = m_mgr->getMesh("Resource/Plane.3DS");
    IAnimatedMeshSceneNode* uiNode = m_mgr->addAnimatedMeshSceneNode(uiMesh, menuNode);
    uiNode->setMaterialFlag(EMF_LIGHTING, false);
    uiNode->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);
    uiNode->setMaterialFlag(EMF_BACK_FACE_CULLING, false);

    GameObject* ui = new GameObject();
    ui->SetNode(uiNode);
    ui->SetVisible(false);
    m_display.push_back(ui);

    IAnimatedMesh* shadowMesh = m_mgr->getMesh("Resource/Plane.3DS");
    IAnimatedMeshSceneNode* shadowNode = m_mgr->addAnimatedMeshSceneNode(shadowMesh);
    shadowNode->setMaterialFlag(EMF_LIGHTING, false);
    shadowNode->setMaterialFlag(EMF_BACK_FACE_CULLING, false);
    shadowNode->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);
    GameObject* shadow = new GameObject();
    shadow->SetNode(shadowNode);
    shadow->SetScale(PMVector(256, 1, 256));
    shadow->m_renderTarget = m_driver->addRenderTargetTexture(dimension2d<u32>(2048, 2048));
    m_display.push_back(shadow);

    m_menuUI = new MenuUIObject(ui, shadow);
    m_menuUI->SetNode(menuNode);
    m_menuUI->ChangeTexture(TEXTURE_MENU_BG);

    float width = MAX_VELOCITY, height = MAX_VELOCITY;
    m_menuUI->Resize(width, height);
    m_menuUI->SetPosition(m_avatar->GetAbsolutePosition() + PMVector(0, UI_DEFAULT_Y, UI_DEFAULT_Z));
    m_menuUI->SetRotation(-90, 0, 0);

    m_menuUI->m_closeButton = CreateButton();
    m_menuUI->m_closeButton->m_node->setParent(menuNode);
    m_menuUI->m_closeButton->ChangeTexture(TEXTURE_CLOSE_NORMAL);

    m_menuUI->m_startButton = CreateButton();
    m_menuUI->m_startButton->m_node->setParent(menuNode);
    m_menuUI->m_startButton->ChangeTexture(TEXTURE_START_NORMAL);

    m_menuUI->m_comicButton = CreateButton();
    m_menuUI->m_comicButton->m_node->setParent(menuNode);
    m_menuUI->m_comicButton->ChangeTexture(TEXTURE_COMIC_NORMAL);

    m_menuUI->Init();

    m_windows[m_menuUI->GetId()] = m_menuUI;
    m_mainWindows[m_menuUI->GetId()] = m_menuUI;

    return m_menuUI;
}

ButtonObject* GameObjectFactory::CreateButton()
{
    IAnimatedMesh* shadowMesh = m_mgr->getMesh("Resource/Plane.3DS");
    IAnimatedMeshSceneNode* shadowNode = m_mgr->addAnimatedMeshSceneNode(shadowMesh);
    shadowNode->setMaterialFlag(EMF_LIGHTING, false);
    shadowNode->setMaterialFlag(EMF_BACK_FACE_CULLING, false);
    shadowNode->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);
    GameObject* shadow = new GameObject();
    shadow->SetNode(shadowNode);
    shadow->SetScale(PMVector(256, 1, 256));
    shadow->m_renderTarget = m_driver->addRenderTargetTexture(dimension2d<u32>(2048, 2048));
    m_display.push_back(shadow);

    IAnimatedMesh* mesh = m_mgr->getMesh("Resource/Plane.3DS");
    IAnimatedMeshSceneNode* buttonNode = m_mgr->addAnimatedMeshSceneNode(mesh);
    buttonNode->setMaterialFlag(EMF_LIGHTING, false);
    buttonNode->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);
    ButtonObject* btn = new ButtonObject(shadow);
    btn->SetNode(buttonNode);
    
    m_display.push_back(btn);
    m_windows[btn->GetId()] = btn;

    return btn;
}

position2d<s32> ComputeShadow(ShadowData& a_pos, dimension2d<u32>& a_shadowSize)
{    
    return position2d<s32>( a_pos.x * 8 + 1024 - a_shadowSize.Width / 2, 
                            (-a_pos.y) * 8 + 1024 - a_shadowSize.Height / 2);
}

void GameObjectFactory::DrawShadow(GameObject* a_object, std::vector<ShadowData>& a_shadows)
{
    if (!a_object || a_shadows.empty())
    {
        return;
    }

    m_driver->setRenderTarget(a_object->m_renderTarget, true, true, SColor(0, 255, 255, 255));
    
    std::vector<ShadowData>::iterator iter = a_shadows.begin();
    ITexture* cursorT = GameObject::GetTexture(TEXTURE_PLANE_CURSOR);
    dimension2d<u32> cursorSize(cursorT->getOriginalSize());
    position2d<s32> cursorPos = ComputeShadow(*iter, cursorSize);
    m_driver->draw2DImage(cursorT, cursorPos, rect<s32>(0 , 0, 64, 64), 0, video::SColor(255, 255, 255, 255), true);
    ++iter;
    ITexture* shadowT = GameObject::GetTexture(TEXTURE_PLANE_SHADOW);
    for (; iter != a_shadows.end(); ++iter)
    {
        dimension2d<u32> shadowSize(shadowT->getOriginalSize());
        position2d<s32> shadowPos = ComputeShadow(*iter, shadowSize);
        m_driver->draw2DImage(shadowT, shadowPos, rect<s32>(0 , 0, shadowSize.Width, shadowSize.Height), 0, video::SColor(255, 255, 255, 255), true);
    }

    m_driver->setRenderTarget(0);
    a_object->m_node->setMaterialTexture(0, a_object->m_renderTarget);
}

ComicUIObject* GameObjectFactory::GetComicUI()
{
    if (m_comicUI)
    {
        m_comicUI->SetVisible(true);
        m_comicUI->SetEnabled(true);
        PMVector avatarPos = m_avatar->GetAbsolutePosition();
        PMVector headPos = m_avatar->GetHeadPosition();
        avatarPos.y += UI_DEFAULT_Y;
        avatarPos.z += UI_DEFAULT_Z;
        m_comicUI->SetPosition(avatarPos);
        m_comicUI->SetRotation(-90, 0, 0);
        return m_comicUI;
    }

    IAnimatedMesh* mesh = m_mgr->getMesh("Resource/Plane.3DS");
    IAnimatedMeshSceneNode* comicNode = m_mgr->addAnimatedMeshSceneNode(mesh);
    comicNode->setMaterialFlag(EMF_LIGHTING, false);
    comicNode->setMaterialFlag(EMF_BACK_FACE_CULLING, false);
    comicNode->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);

    IAnimatedMesh* uiMesh = m_mgr->getMesh("Resource/Plane.3DS");
    IAnimatedMeshSceneNode* uiNode = m_mgr->addAnimatedMeshSceneNode(uiMesh, comicNode);
    uiNode->setMaterialFlag(EMF_LIGHTING, false);
    uiNode->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);
    uiNode->setMaterialFlag(EMF_BACK_FACE_CULLING, false);

    GameObject* ui = new GameObject();
    ui->SetNode(uiNode);
    ui->SetVisible(false);
    m_display.push_back(ui);

    IAnimatedMesh* shadowMesh = m_mgr->getMesh("Resource/Plane.3DS");
    IAnimatedMeshSceneNode* shadowNode = m_mgr->addAnimatedMeshSceneNode(shadowMesh);
    shadowNode->setMaterialFlag(EMF_LIGHTING, false);
    shadowNode->setMaterialFlag(EMF_BACK_FACE_CULLING, false);
    shadowNode->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);
    GameObject* shadow = new GameObject();
    shadow->SetNode(shadowNode);
    shadow->SetScale(PMVector(256, 1, 256));
    shadow->m_renderTarget = m_driver->addRenderTargetTexture(dimension2d<u32>(2048, 2048));
    m_display.push_back(shadow);

    m_comicUI = new ComicUIObject(ui, shadow);
    m_comicUI->SetNode(comicNode);
    m_comicUI->ChangeTexture(TEXTURE_CLOSE_DISABLE);

    PMVector avatarPos = m_avatar->GetAbsolutePosition();
    PMVector headPos = m_avatar->GetHeadPosition();
    avatarPos.y += UI_DEFAULT_Y;
    avatarPos.z += UI_DEFAULT_Z;
    m_comicUI->SetPosition(avatarPos);
    m_comicUI->SetRotation(-90, 0, 0);

    m_comicUI->m_closeButton = CreateButton();
    m_comicUI->m_closeButton->m_node->setParent(comicNode);
    m_comicUI->m_closeButton->ChangeTexture(TEXTURE_CLOSE_NORMAL);

    m_comicUI->m_upButton = CreateButton();
    m_comicUI->m_upButton->m_node->setParent(comicNode);
    m_comicUI->m_upButton->ChangeTexture(TEXTURE_LAST_NORMAL);

    m_comicUI->m_downButton = CreateButton();
    m_comicUI->m_downButton->m_node->setParent(comicNode);
    m_comicUI->m_downButton->ChangeTexture(TEXTURE_NEXT_NORMAL);

    m_comicUI->Init();

    m_windows[m_comicUI->GetId()] = m_comicUI;
    m_mainWindows[m_comicUI->GetId()] = m_comicUI;

    return m_comicUI;
}

std::pair<UInt32, UInt32> GameObjectFactory::LoadTexture( GameObject* a_object, std::string a_name )
{
    std::pair<UInt32, UInt32> textureSize(0, 0);

    if (a_object && a_object->m_node)
    {
        ITexture* t = m_driver->getTexture(a_name.c_str());
        if (t)
        {
            a_object->m_node->setMaterialTexture(0, t);
            const core::dimension2d<u32>& tmp = t->getOriginalSize();
            textureSize.first = tmp.Width;
            textureSize.second = tmp.Height;
        }
    }

    return textureSize;
}
