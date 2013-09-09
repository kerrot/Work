#include "BasketObject.h"
#include "LeafObject.h"
#include "PMType.h"

BasketObject::BasketObject()
{

}

BasketObject::~BasketObject()
{

}

void BasketObject::HitObject(LeafObject* a_object )
{
    PMVector position = a_object->GetPosition();

    if (position.y < 0)
    {
        a_object->ResetToBegin();
    }
}
