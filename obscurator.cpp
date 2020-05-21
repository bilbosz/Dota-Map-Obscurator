#include "obscurator.hpp"

Obscurator::Obscurator() {}

Obscurator::~Obscurator()
{
    if( m_output )
    {
        delete[] m_output;
    }
}

void Obscurator::Process( uint8_t* input, uint32_t width, uint32_t height, uint32_t lineSize )
{
    const ptrdiff_t r = 0;
    const ptrdiff_t g = 1;
    const ptrdiff_t b = 2;
    const ptrdiff_t a = 3;
    if( height != m_height || width != m_width || lineSize != m_lineSize || !m_output )
    {
        m_height = height;
        m_width = width;
        m_lineSize = lineSize;
        if( m_output )
        {
            delete[] m_output;
        }
        m_output = new uint8_t[m_height * m_lineSize];
    }
    for( uint32_t y = 0; y < m_height; ++y )
    {
        for( uint32_t x = 0; x < m_width; ++x )
        {
            ptrdiff_t offset = y * lineSize + x * 4;
            uint8_t* in = input + offset;
            uint8_t* out = m_output + offset;
            *( out + r ) = ( *( in + r ) + 255 ) / 2;
            *( out + g ) = *( in + g );
            *( out + b ) = *( in + b );
            *( out + a ) = *( in + a );
        }
    }
}

uint8_t* Obscurator::Output() const
{
    return m_output;
}
