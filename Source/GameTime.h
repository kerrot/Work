#pragma once

#include "PMCommon.h"

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

    PMTypeDefine::UInt32 GetCurrentTimeInMS();

    ~GameTime();
private:
    GameTime();

    irr::ITimer* m_timer;
};

#define sGameTime GameTime::GetInstance()