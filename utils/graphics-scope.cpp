#include "graphics-scope.hpp"
#include <obs-module.h>

GraphicsScope::GraphicsScope( bool fake ) : m_fake( fake )
{
    assert( m_count >= 0 );
    if( m_count == 0 && !m_fake )
    {
        obs_enter_graphics();
    }
    ++m_count;
}

GraphicsScope::~GraphicsScope()
{
    assert( m_count > 0 );
    --m_count;
    if( m_count == 0 && !m_fake )
    {
        obs_leave_graphics();
    }
}

int GraphicsScope::m_count = 0;