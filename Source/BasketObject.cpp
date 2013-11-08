#include "BasketObject.h"
#include "LeafObject.h"
#include "PMType.h"
#include "GameWorld.h"

BasketObject::BasketObject(GameObject* a_ui, GameObject* a_clear)
:
m_ui(a_ui)
,m_clear(a_clear)
,m_fail(false)
{

}

BasketObject::~BasketObject()
{

}

void BasketObject::HitObject(LeafObject* a_object )
{
    if (!a_object->IsVisible())
    {
        return;
    }

    PMVector leafPos = a_object->GetPosition();
    if (leafPos.y < 0)
    {
        if (sGameWorld.GetWorldState() == WORLD_STATE_GAME)
        {
            a_object->ResetToBegin();            
        }
        else
        {
            a_object->SetVisible(false);
        }
        m_ui->SetVisible(false);
        m_fail = false;
        return;
    }

    if (sGameWorld.GetWorldState() == WORLD_STATE_GAME && !m_fail)
    {
        PMVector v = a_object->GetVelocity();

        PMVector pos = m_ui->GetAbsolutePosition();
        if (leafPos.y >= pos.y && leafPos.y + v.y <= pos.y && abs(v.y) > 0)
        {
            float rate = (leafPos.y - pos.y) / abs(v.y);
            float x = leafPos.x + rate * v.x;
            float z = leafPos.z + rate * v.z;

            if (abs(x - pos.x) <= 50, abs(z - pos.z) <= 30)
            {
                sGameWorld.BackToNormal();
                m_clear->SetVisible(true);
            }
        }
    }
}

void BasketObject::SetFail()
{
    if (sGameWorld.GetWorldState() == WORLD_STATE_GAME)
    {
        m_ui->SetVisible(true);
        m_fail = true;
    }
}
