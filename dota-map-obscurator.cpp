#include "dota-map-obscurator.hpp"
#include "obscurator.hpp"
#include "utils/graphics-scope.hpp"

void DotaMapObscurator::RegisterSourceInfo()
{
    obs_source_info sourceInfo = {};
    sourceInfo.id = "dota_map_obscurator";
    sourceInfo.type = OBS_SOURCE_TYPE_FILTER;
    sourceInfo.output_flags = OBS_SOURCE_VIDEO;
    sourceInfo.get_name = []( void* ) -> const char* { return obs_module_text( "DotaMapObscurator" ); };
    sourceInfo.create = []( [[maybe_unused]] obs_data_t* settings, obs_source_t* source ) -> void* {
        return static_cast< void* >( new DotaMapObscurator( source ) );
    };
    sourceInfo.destroy = []( void* data ) -> void { delete static_cast< DotaMapObscurator* >( data ); };
    sourceInfo.update = []( [[maybe_unused]] void* data, [[maybe_unused]] obs_data_t* settings ) -> void {

    };
    sourceInfo.get_properties = []( void* data ) -> obs_properties_t* {
        return static_cast< DotaMapObscurator* >( data )->GetProperties();
    };
    sourceInfo.video_tick = []( void* data, float seconds ) -> void {
        static_cast< DotaMapObscurator* >( data )->Update( seconds );
    };
    sourceInfo.video_render = []( void* data, [[maybe_unused]] gs_effect_t* effect ) -> void {
        static_cast< DotaMapObscurator* >( data )->Render();
    };
    obs_register_source( &sourceInfo );
}

obs_properties_t* DotaMapObscurator::GetProperties() const
{
    return obs_properties_create();
}

DotaMapObscurator::DotaMapObscurator( obs_source_t* source )
    : m_source( source ), m_obscurator( std::make_unique< Obscurator >() )
{
    GraphicsScope scope;
    m_render = gs_texrender_create( GS_RGBA, GS_ZS_NONE );
}

DotaMapObscurator::~DotaMapObscurator()
{
    GraphicsScope scope;
    if( m_stageSurface )
    {
        gs_stagesurface_destroy( m_stageSurface );
    }
    if( m_textureBuffer )
    {
        gs_texture_destroy( m_textureBuffer );
    }
    if( m_mapTexture )
    {
        gs_texture_destroy( m_mapTexture );
    }
    gs_texrender_destroy( m_render );
}

void DotaMapObscurator::Init()
{
    if( m_inited )
    {
        return;
    }
    m_target = obs_filter_get_target( m_source );
    m_parent = obs_filter_get_parent( m_source );
    if( !m_target || !m_parent )
    {
        return;
    }
    m_width = obs_source_get_base_width( m_target );
    m_height = obs_source_get_base_height( m_target );
    if( m_width <= 0 || m_height <= 0 )
    {
        return;
    }
    m_inited = true;
}

void DotaMapObscurator::Update( [[maybe_unused]] float dt )
{
    Init();
    if( !m_inited )
    {
        return;
    }

    //TODO Add change target size handling
    if( m_mapTexture )
    {
        ProcessMapTexture();
    }

    auto newWidth = obs_source_get_base_width( m_target );
    auto newHeight = obs_source_get_base_height( m_target );
    assert( newWidth > 0 && newHeight > 0 );
    if( newWidth != m_width || newHeight != m_height || !m_mapTexture )
    {
        GraphicsScope scope;
        m_width = newWidth;
        m_height = newHeight;
        m_mapSide = 325.0f * m_height / 1440.0f;
        if( m_stageSurface )
        {
            gs_stagesurface_destroy( m_stageSurface );
        }
        if( m_textureBuffer )
        {
            gs_texture_destroy( m_textureBuffer );
        }
        if( m_mapTexture )
        {
            gs_texture_destroy( m_mapTexture );
        }
        m_mapTexture = gs_texture_create( m_mapSide, m_mapSide, GS_RGBA, 1, nullptr, GS_DYNAMIC );
        m_textureBuffer = gs_texture_create( m_mapSide, m_mapSide, GS_RGBA, 1, nullptr, GS_DYNAMIC );
        m_stageSurface = gs_stagesurface_create( m_mapSide, m_mapSide, GS_RGBA );
    }
}

void DotaMapObscurator::Render()
{
    if( !m_inited )
    {
        obs_source_skip_video_filter( m_source );
        return;
    }

    gs_texrender_reset( m_render );
    if( gs_texrender_begin( m_render, m_width, m_height ) )
    {
        uint32_t flags = obs_source_get_output_flags( m_target );
        bool isCustomDraw = ( flags & OBS_SOURCE_CUSTOM_DRAW ) != 0;
        bool isAsync = ( flags & OBS_SOURCE_ASYNC ) != 0;

        vec4 clearColor;
        vec4_zero( &clearColor );
        gs_clear( GS_CLEAR_COLOR, &clearColor, 0.0f, 0 );
        gs_ortho( 0.0f, static_cast< float >( m_width ), 0.0f, static_cast< float >( m_height ), -100.0f, 100.0f );

        if( m_target == m_parent && !isCustomDraw && !isAsync )
        {
            obs_source_default_render( m_target );
        }
        else
        {
            obs_source_video_render( m_target );
        }
        gs_texrender_end( m_render );
    }

    gs_texture_t* texture = gs_texrender_get_texture( m_render );
    if( m_ready )
    {
        gs_copy_texture_region( m_textureBuffer, 0, 0, texture, 0, m_height - m_mapSide, m_mapSide, m_mapSide );
        gs_copy_texture_region( texture, 0, m_height - m_mapSide, m_mapTexture, 0, 0, m_mapSide, m_mapSide );
        gs_copy_texture( m_mapTexture, m_textureBuffer );
        m_ready = false;
    }

    gs_effect_t* effect = obs_get_base_effect( OBS_EFFECT_DEFAULT );
    if( texture )
    {
        gs_eparam_t* image = gs_effect_get_param_by_name( effect, "image" );
        gs_effect_set_texture( image, texture );

        while( gs_effect_loop( effect, "Draw" ) )
        {
            gs_draw_sprite( texture, 0, m_width, m_height );
        }
    }
}

void DotaMapObscurator::ProcessMapTexture()
{
    GraphicsScope scope;

    uint8_t* inData;
    uint32_t lineSize;

    gs_stage_texture( m_stageSurface, m_mapTexture );
    gs_stagesurface_map( m_stageSurface, &inData, &lineSize );
    m_obscurator->Process( inData, m_mapSide, m_mapSide, lineSize );
    gs_stagesurface_unmap( m_stageSurface );
    gs_texture_set_image( m_mapTexture, m_obscurator->Output(), lineSize, false );
    m_ready = true;
}
