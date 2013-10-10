#pragma once
#include "PMCommon.h"

class LeafObject;
class WindObject;
class BasketObject;

enum GameWorldState
{
    WORLD_STATE_NORMAL,
    WORLD_STATE_GAME,
};

class GameWorld
{
public:
    ~GameWorld();

    static GameWorld& GetInstance();

    void WorldInit();
    void WorldUpdate();

    GameWorldState GetWorldState();
    void GameStart();
    void BackToNormal();

    BasketObject* GetBasket();

private:
    GameWorld();

    void RandomLeaves();

    enum WorldSetting
    {
        LEAVES_POSITION_RADIUS              = 500,
        LEAVES_MAX                          = 100,
        LEAVES_PERIOD_AMOUNT                = 5,
        LEAVES_PERIOD_TIME                  = 1000,
        LEAVES_START_HEIGHT                 = 800,
    };  

    UInt32 m_leafIndex;
    UInt32 m_leafTime;
    GameWorldState m_state;
    BasketObject* m_basket;
    WindObject* m_gameWind;
    LeafObject* m_gameLeaf;
    LeafObject* m_regularLeavs[LEAVES_MAX];
};

#define sGameWorld GameWorld::GetInstance()
