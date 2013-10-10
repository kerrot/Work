#include "GameWorld.h"
#include "GameObjectFactory.h"
#include "WindowInterface.h"
#include "GameObjectFactory.h"
#include "BasketObject.h"
#include "LeafObject.h"
#include "WindObject.h"
#include "BasketObject.h"
#include "GameTime.h"

#define _USE_MATH_DEFINES 
#include <math.h>

GameWorld::GameWorld()
:
m_leafIndex(0)
,m_leafTime(0)
,m_state(WORLD_STATE_NORMAL)
{

}

GameWorld::~GameWorld()
{

}

void GameWorld::WorldUpdate()
{
    switch (m_state)
    {
    case WORLD_STATE_NORMAL:
        RandomLeaves();
        break;
    case WORLD_STATE_GAME:
        break;
    default:
        break;
    }
}

void GameWorld::WorldInit()
{
    m_basket = sGameObjectFactory.CreateBasket();
    m_basket->SetPosition(0, 0, 500);

    for (int i = 0; i < LEAVES_MAX; ++i)
    {
        m_regularLeavs[i] = sGameObjectFactory.CreateLeaf();
        m_regularLeavs[i]->SetVisible(false);
    }

    WindObject* natrualWind = dynamic_cast<WindObject*>(sGameObjectFactory.CreateWind());
    natrualWind->SetPosition(PMVector(0, LEAVES_START_HEIGHT, - LEAVES_POSITION_RADIUS));
    natrualWind->SetRotation(PMVector(90, 0, 0));
    float w = 1000, h = 500, r = 1000;
    natrualWind->Resize(w, h);
    natrualWind->ChangeRange(r);
    natrualWind->SetNormalDirection(PMVector(0, 0, 1), PMVector(-1, 0, 0), PMVector(0, 1, 0));
    natrualWind->SetEnabled(false);

    m_gameWind = dynamic_cast<WindObject*>(sGameObjectFactory.CreateWind());
    m_gameWind->SetPosition(0, 100, 300);
    m_gameWind->SetVisible(false);
    m_gameWind->SetEnabled(false);

    m_gameLeaf = sGameObjectFactory.CreateLeaf();
    m_gameLeaf->SetPosition(0, 250, 300);
    m_gameLeaf->SetOriposition(PMVector(0, 250, 300));
    m_gameLeaf->SetVisible(false);
}

GameWorld& GameWorld::GetInstance()
{
    static GameWorld m_instance;

    return m_instance;
}

void GameWorld::RandomLeaves()
{
    UInt32 current = sGameTime.GetCurrentTimeInMS();

    if (current - m_leafTime > LEAVES_PERIOD_TIME)
    {
        m_leafTime = current;
    }
    else
    {
        return;
    }

    UInt32 maxIndex = m_leafIndex + LEAVES_PERIOD_AMOUNT;

    for (int i = m_leafIndex; i < maxIndex; ++i)
    {
        m_regularLeavs[i]->SetVisible(true);
        m_regularLeavs[i]->ResetToBegin();

        UInt32 angle = rand() % 360;
        UInt32 radious = rand() % LEAVES_POSITION_RADIUS;
        PMVector pos(float(radious) * sin(float(angle) / 180.0f * M_PI), LEAVES_START_HEIGHT, float(radious) * cos(float(angle) / 180.0f * M_PI));
        m_regularLeavs[i]->SetPosition(pos);
    }

    m_leafIndex += LEAVES_PERIOD_AMOUNT;
    if (maxIndex == LEAVES_MAX)
    {
        m_leafIndex = 0;
    }
}

void GameWorld::GameStart()
{
    for (int i = 0; i < LEAVES_MAX; ++i)
    {
        m_regularLeavs[i]->SetVisible(false);
    }

    m_state = WORLD_STATE_GAME;
    m_gameLeaf->SetVisible(true);
    m_gameWind->SetVisible(true);
    m_gameWind->SetEnabled(true);
    m_gameWind->SetRotation(0, 0, 0);
    m_gameWind->SetPosition(0, 100, 300);
    m_gameWind->SetNormalDirection(PMVector(0, 1, 0), PMVector(1, 0, 0), PMVector(0, 0, 1));
}

GameWorldState GameWorld::GetWorldState()
{
    return m_state;
}

void GameWorld::BackToNormal()
{
    m_state = WORLD_STATE_NORMAL;

    m_gameWind->SetVisible(false);
    m_gameWind->SetEnabled(false);
    m_gameLeaf->SetVisible(false);
}

BasketObject* GameWorld::GetBasket()
{
    return m_basket;
}
