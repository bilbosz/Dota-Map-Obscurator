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
    gs_texture* m_mapTexture = nullptr;
    gs_texture* m_textureBuffer = nullptr;
    gs_stage_surface* m_stageSurface = nullptr;
    bool m_ready = false;
    std::unique_ptr< Obscurator > m_obscurator;
};

#endif //OBS_STUDIO_DOTA_MAP_OBSCURATOR_HPP
