#include "WindowInterface.h"
#include "GameObject.h"
#include <assert.h>

UInt32 WindowInterface::m_idNow = 0;

WindowInterface::WindowInterface( GameObject* a_object )
:
m_object(a_object)
,m_normal(0, 1, 0)
,m_id(++m_idNow)
{    
    assert(m_object);
}

WindowInterface::~WindowInterface()
{

}

void WindowInterface::Resize( float a_width, float a_height )
{
    m_width = (a_width > 0) ? a_width : -a_width;
    m_height = (a_height > 0) ? a_height : -a_height;

    m_object->SetScale(PMVector(m_width, m_height, 0));
}

UInt32 WindowInterface::GetId()
{
    return m_id;
}


