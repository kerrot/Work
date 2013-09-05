#pragma once

#include "irrlicht.h"

using namespace irr;

class GameWorld : public IEventReceiver
{
public:
    GameWorld();

    virtual bool OnEvent(const SEvent& event);

    void WorldInit();
    void WorldUpdate();
};