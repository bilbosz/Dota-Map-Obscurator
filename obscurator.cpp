#include "obscurator.hpp"
#include "utils/graphics-scope.hpp"
#include "utils/vec-4-u.hpp"
#include <obs-module.h>
#include <valarray>

Obscurator::Obscurator() {}

Obscurator::~Obscurator()
{
    if( m_output )
    {
        gs_texture_destroy( m_refTexture );
        bfree( m_reference );
        delete[] m_masks;
        delete[] m_output;
    }
}

void Obscurator::Process(
    uint8_t* input,
    uint32_t width,
    uint32_t height,
    uint32_t lineSize,
    uint32_t mapBorder,
    float time,
    int32_t param1,
    int32_t param2,
    int32_t param3,
    int32_t param4,
    int32_t param5,
    int32_t param6 )
{
    m_param1 = param1;
    m_param2 = param2;
    m_param3 = param3;
    m_param4 = param4;
    m_param5 = param5;
    m_param6 = param6;
    m_time = time;
    UpdateSize( input, width, height, lineSize, mapBorder );
    Draw();
}

uint8_t* Obscurator::Output() const
{
    return m_output;
}

// promising:
//    1   -60
//    2   4
//    3   -129
//    4   -9
//    5   -99
//    6   63

void Obscurator::Draw()
{
    for( uint32_t y = 0; y < m_height; ++y )
    {
        for( uint32_t x = 0; x < m_width; ++x )
        {
            ptrdiff_t offset = y * m_lineSize + x * 4;
            Vec4u& in = *reinterpret_cast< Vec4u* >( m_input + offset );
            Vec4u& out = *reinterpret_cast< Vec4u* >( m_output + offset );
            Vec4u& ref = *reinterpret_cast< Vec4u* >( m_reference + offset );
            uint8_t inAvg = ( (int32_t)in.r + in.g + in.b ) / 3;
            uint8_t refAvg = ( (int32_t)ref.r + ref.g + ref.b ) / 3;

            if( x >= m_mapBorder && x < m_width - m_mapBorder && y >= m_mapBorder && y < m_width - m_mapBorder )
            {
                if( static_cast< int32_t >( in.r )  >= m_param1 &&
                    static_cast< int32_t >( in.r )  <= m_param2 &&
                    static_cast< int32_t >( in.g )  >= m_param3 &&
                    static_cast< int32_t >( in.g )  <= m_param4 &&
                    static_cast< int32_t >( in.b )  >= m_param5 &&
                    static_cast< int32_t >( in.b )  <= m_param6 )
                {
                    out = { 255, 255, 255, 255 };
                }
                else
                {
                    out = in;
                }
                out.a = 255;
            }
            else
            {
                out = in;
            }
        }
    }
}

void Obscurator::UpdateSize( uint8_t* input, uint32_t width, uint32_t height, uint32_t lineSize, uint32_t mapBorder )
{
    if( height != m_height || width != m_width || lineSize != m_lineSize || !m_output )
    {
        GraphicsScope scope;
        m_input = input;
        m_height = height;
        m_width = width;
        m_lineSize = lineSize;
        m_mapBorder = mapBorder;
        if( m_output )
        {
            gs_texture_destroy( m_refTexture );
            bfree( m_reference );
            delete[] m_masks;
            delete[] m_output;
        }
        m_output = new uint8_t[m_height * m_lineSize];
        m_masks = new uint8_t[m_height * m_width];

        m_refTexture =
            gs_texture_create_from_file( "C:\\repo\\obs-dev\\obs-studio\\plugins\\dota-map-obscurator\\data\\32.png" );
        //            "/media/525A2AB45A2A94AF/repo/obs-dev/obs-studio/plugins/dota-map-obscurator/data/32.png" );
        auto texrender = gs_texrender_create( GS_RGBA, GS_ZS_NONE );
        if( gs_texrender_begin( texrender, m_width, m_height ) )
        {
            vec4 clearColor;
            vec4_zero( &clearColor );
            gs_clear( GS_CLEAR_COLOR, &clearColor, 0.0f, 0 );
            gs_ortho( 0.0f, static_cast< float >( m_width ), 0.0f, static_cast< float >( m_height ), -100.0f, 100.0f );
            gs_effect_t* effect = obs_get_base_effect( OBS_EFFECT_DEFAULT );
            if( m_refTexture )
            {
                gs_eparam_t* image = gs_effect_get_param_by_name( effect, "image" );
                gs_effect_set_texture( image, m_refTexture );

                while( gs_effect_loop( effect, "Draw" ) )
                {
                    gs_draw_sprite( m_refTexture, 0, m_width, m_height );
                }
            }
            gs_texrender_end( texrender );
        }

        gs_texture_t* tex = gs_texrender_get_texture( texrender );
        auto stagesurf = gs_stagesurface_create( m_width, m_height, GS_RGBA );
        uint8_t* data;
        uint32_t linesize;
        gs_stage_texture( stagesurf, tex );
        gs_stagesurface_map( stagesurf, &data, &linesize );
        m_reference = static_cast< uint8_t* >( bmemdup( data, linesize * m_height ) );
        gs_stagesurface_unmap( stagesurf );
        gs_stagesurface_destroy( stagesurf );
        gs_texrender_destroy( texrender );
    }
}
