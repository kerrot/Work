#include "GameTime.h"
#include <irrlicht.h>

GameTime::GameTime()
:
m_timer(0)
{

}

GameTime::~GameTime()
{

}

GameTime& GameTime::GetInstance()
{
    static GameTime m_instance;

    return m_instance;
}

unsigned int GameTime::GetCurrentTimeInMS()
{
    if (m_timer)
    {
        return m_timer->getTime();
    }

    return 0;
}

void GameTime::Init( irr::ITimer* a_timer )
{
    m_timer = a_timer;
}
