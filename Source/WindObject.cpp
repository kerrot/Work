#include "WindObject.h"
#include "LeafObject.h"
#include "PMDefine.h"

WindObject::WindObject()
:
WindowInterface(this)
,m_range(10)
,m_force(0.01)
{

}

WindObject::~WindObject()
{

}

void WindObject::HitObject( LeafObject* a_object )
{
    PMVector point = a_object->GetPosition();
    PMVector transformPoint = TransformByCoordinateSqure(point);

    float boundX = m_width * m_width / 4;
    float boundY = m_height * m_height / 4;
    float boundZ = m_range * m_range / 4;

    if (transformPoint.x <= boundX &&
        transformPoint.y <= boundY &&
        transformPoint.z <= boundZ)
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
    a_range = (a_range < MAX_VELOCITY) ? MAX_VELOCITY : a_range;

    m_force = m_force * m_range / a_range;
    m_range = a_range;

    m_object->SetScale(PMVector(0, 0, m_range));
}
