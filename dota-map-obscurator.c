#include <obs-module.h>

#define min( X, Y ) (((X) < (Y)) ? (X) : (Y))
#define max( X, Y ) (((X) < (Y)) ? (Y) : (X))

struct dota_map_obscurator_data
{
    obs_source_t* source;
    gs_texrender_t* render;
    gs_stagesurf_t* stagesurf;
    gs_texture_t* out_texture;
    uint8_t* in_data;
    uint8_t* out_data;
    uint32_t w;
    uint32_t h;
    uint32_t texture_size;
    bool ready;
};

static const char* dota_map_obscurator_get_name( void* unused )
{
    UNUSED_PARAMETER( unused );
    return obs_module_text( "DotaMapObscurator" );
}

static void dota_map_obscurator_update( void* data, obs_data_t* s )
{
    UNUSED_PARAMETER( data );
    UNUSED_PARAMETER( s );
}

static obs_properties_t* dota_map_obscurator_properties( void* data )
{
    UNUSED_PARAMETER( data );
    obs_properties_t* properties = obs_properties_create();
    return properties;
}

static void* dota_map_obscurator_create( obs_data_t* settings, obs_source_t* source )
{
    struct dota_map_obscurator_data* f = bzalloc( sizeof( *f ) );
    f->source = source;
    obs_source_update( source, settings );
    return f;
}

static void dota_map_obscurator_destroy( void* data )
{
    struct dota_map_obscurator_data* f = data;
    {
        obs_enter_graphics();
        gs_stagesurface_destroy( f->stagesurf );
        gs_texrender_destroy( f->render );
        gs_texture_destroy( f->out_texture );
        obs_leave_graphics();
    }
    bfree( f->out_data );
    bfree( f );
}

static void process_data( struct dota_map_obscurator_data* f, uint32_t* data )
{
    int32_t side = 325 * f->h / 1440.0f;
    for( uint32_t y = max( 0, f->h - side ); y < f->h; ++y )
    {
        for( uint32_t x = 0; x < min( f->w, side ); ++x )
        {
            uint32_t* pixel = data + y * f->w + x - 3;
            uint8_t* r = pixel + 3;
            uint8_t* g = pixel + 2;
            uint8_t* b = pixel + 1;
            uint8_t* a = pixel;
            *r = ( 255 + *r ) / 2;
        }
    }
}

static void dota_map_obscurator_tick( void* data, float t )
{
    struct dota_map_obscurator_data* f = data;

    struct obs_source_t* target = obs_filter_get_target( f->source );
    f->w = obs_source_get_base_width( target );
    f->h = obs_source_get_base_height( target );
    f->texture_size = f->w * f->h * 4;
    if( !f->out_data )
    {
        f->out_data = bzalloc( f->texture_size );
    }

    UNUSED_PARAMETER( t );
}

static void dota_map_obscurator_render( void* data, gs_effect_t* effect )
{
    struct dota_map_obscurator_data* f = data;
    obs_source_t* target = obs_filter_get_target( f->source );
    obs_source_t* parent = obs_filter_get_parent( f->source );

    if( !target || !parent || f->w == 0 || f->h == 0 )
    {
        obs_source_skip_video_filter( f->source );
        return;
    }

    if( !f->render )
    {
        f->render = gs_texrender_create( GS_RGBA, GS_ZS_NONE );
    }

    if( !f->stagesurf )
    {
        f->stagesurf = gs_stagesurface_create( f->w, f->h, GS_RGBA );
    }

    if( !f->out_texture )
    {
        f->out_texture = gs_texture_create( f->w, f->h, GS_RGBA, 1, NULL, GS_DYNAMIC );
    }

    gs_texrender_reset( f->render );
    if( gs_texrender_begin( f->render, f->w, f->h ) )
    {
        uint32_t flags = obs_source_get_output_flags( target );
        bool custom_draw = ( flags & OBS_SOURCE_CUSTOM_DRAW ) != 0;
        bool async = ( flags & OBS_SOURCE_ASYNC ) != 0;

        struct vec4 clear_color;
        vec4_zero( &clear_color );
        gs_clear( GS_CLEAR_COLOR, &clear_color, 0.0f, 0 );
        gs_ortho( 0.0f, ( float )f->w, 0.0f, ( float )f->h, -100.0f, 100.0f );

        if( target == parent && !custom_draw && !async )
        {
            obs_source_default_render( target );
        }
        else
        {
            obs_source_video_render( target );
        }
        gs_texrender_end( f->render );
    }

    gs_texture_t* texture = gs_texrender_get_texture( f->render );

    uint8_t* mem_in;
    uint32_t line_size;
    size_t mem_size = f->w * f->h * sizeof( *mem_in ) * 4;

    gs_stage_texture( f->stagesurf, texture );
    gs_stagesurface_map( f->stagesurf, &( f->in_data ), &line_size );
    memcpy( f->out_data, f->in_data, mem_size );
    gs_stagesurface_unmap( f->stagesurf );

    process_data( f, f->out_data );

    gs_texture_set_image( f->out_texture, f->out_data, line_size, false );

    gs_effect_t* image_effect = obs_get_base_effect( OBS_EFFECT_DEFAULT );
    if( f->out_texture )
    {
        gs_eparam_t* image = gs_effect_get_param_by_name( image_effect, "image" );
        gs_effect_set_texture( image, f->out_texture );

        while( gs_effect_loop( image_effect, "Draw" ) )
        {
            gs_draw_sprite( f->out_texture, 0, f->w, f->h );
        }
    }

    UNUSED_PARAMETER( effect );
}

struct obs_source_info dota_map_obscurator = {
    .id = "dota_map_obscurator", .type = OBS_SOURCE_TYPE_FILTER, .output_flags = OBS_SOURCE_VIDEO, .get_name = dota_map_obscurator_get_name, .create = dota_map_obscurator_create, .destroy = dota_map_obscurator_destroy, .update = dota_map_obscurator_update, .get_properties = dota_map_obscurator_properties, .video_tick = dota_map_obscurator_tick, .video_render = dota_map_obscurator_render,
};

OBS_DECLARE_MODULE()

OBS_MODULE_USE_DEFAULT_LOCALE( "dota-map-obscurator", "en-US" )

bool obs_module_load( void )
{
    obs_register_source( &dota_map_obscurator );
    return true;
}
