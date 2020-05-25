#ifndef OBS_STUDIO_DOTA_MAP_OBSCURATOR_HPP
#define OBS_STUDIO_DOTA_MAP_OBSCURATOR_HPP

#include "utils/global.hpp"
#include <obs-module.h>

class Obscurator;

class DotaMapObscurator
{
public:
    static void RegisterSourceInfo();

    obs_properties_t* GetProperties() const;
    void Init();
    void Update( float dt );
    void Render();
    void Reconfigure( obs_data_t* settings );

private:
    DotaMapObscurator( obs_source_t* source );
    ~DotaMapObscurator();

    void ProcessMapTexture();

    obs_source_t* m_source;
    gs_texrender_t* m_render;
    bool m_inited = false;
    obs_source_t* m_target = nullptr;
    obs_source_t* m_parent = nullptr;
    int32_t m_width = 0;
    int32_t m_height = 0;
    int32_t m_mapSide = 0;
    uint32_t m_mapBorder = 0;
    gs_texture* m_mapTexture = nullptr;
    gs_stage_surface* m_stageSurface = nullptr;
    std::unique_ptr< Obscurator > m_obscurator;
    float m_time = 0.0f;
    int32_t m_param1 = 0;
    int32_t m_param2 = 0;
    int32_t m_param3 = 0;
    int32_t m_param4 = 0;
    int32_t m_param5 = 0;
    int32_t m_param6 = 0;
};

#endif //OBS_STUDIO_DOTA_MAP_OBSCURATOR_HPP
