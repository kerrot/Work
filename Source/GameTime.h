#pragma once

namespace irr
{
    class ITimer;
}

using namespace irr;

class GameTime
{
public:
    static GameTime& GetInstance();

    void Init(irr::ITimer* a_timer);

    unsigned int GetCurrentTimeInMS();

    ~GameTime();
private:
    GameTime();

    irr::ITimer* m_timer;
};

#define sGameTime GameTime::GetInstance()