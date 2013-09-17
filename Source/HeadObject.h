#pragma once
#include "GameObject.h"

struct PMVector;

class HeadObject : public GameObject
{
public:
    void SetTarget(PMVector a_target) const;

private:
    friend class AvatarObject;
    HeadObject();
    virtual ~HeadObject();
};