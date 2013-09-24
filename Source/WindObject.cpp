#include "WindObject.h"
#include "LeafObject.h"
#include "PMDefine.h"

WindObject::WindObject(GameObject* a_ui)
:
WindowInterface(this, a_ui)
,m_range(MAX_VELOCITY)
,m_force(GRAVITY_ACCELERATION * 2)
{

}

WindObject::~WindObject()
{

}

void WindObject::HitObject( LeafObject* a_object )
{
    PMVector point = a_object->GetAbsolutePosition();
    PMVector transformPoint = TransformByCoordinateSqure(point);

    PMVector direction = point - GetAbsolutePosition();

    float boundX = m_width * m_width / 4;
    float boundY = m_height * m_height / 4;
    float boundZ = m_range * m_range;

    if (transformPoint.x <= boundX &&
        transformPoint.y <= boundY &&
        transformPoint.z <= boundZ &&
        m_normal.Dot(direction) >= 0)
    {
        a_object->AddAcceleration(m_normal * m_force);
    }
}

void WindObject::Resize( float &a_width, float &a_height )
{
    WindowInterface::Resize(a_width, a_height);

    m_force = m_width * m_height * m_force / (a_width * a_height);
}

void WindObject::ChangeRange( float &a_range )
{
    float tmpRange = m_range + a_range;

    tmpRange = (tmpRange < MAX_VELOCITY) ? MAX_VELOCITY : tmpRange;

    m_force = m_force * m_range / tmpRange;
    m_range = tmpRange;

    m_object->SetScale(PMVector(0, m_range, 0));
}
