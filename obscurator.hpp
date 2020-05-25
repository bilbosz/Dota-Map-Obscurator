#ifndef OBS_STUDIO_OBSCURATOR_HPP
#define OBS_STUDIO_OBSCURATOR_HPP

#include "utils/global.hpp"
#include <graphics/graphics.h>

class Obscurator
{
public:
    Obscurator();
    ~Obscurator();
    void Process(
        uint8_t* input,
        uint32_t width,
        uint32_t height,
        uint32_t lineSize,
        uint32_t mapBorder,
        float time,
        int32_t param1,
        int32_t param2,
        int32_t m_param3,
        int32_t m_param4,
        int32_t m_param5,
        int32_t m_param6 );
    uint8_t* Output() const;

private:
    void Draw();
    void UpdateSize( uint8_t* input, uint32_t width, uint32_t height, uint32_t lineSize, uint32_t mapBorder );

    uint8_t* m_input = nullptr;
    uint8_t* m_output = nullptr;
    uint8_t* m_masks = nullptr;
    uint32_t m_width = 0;
    uint32_t m_height = 0;
    uint32_t m_lineSize = 0;
    uint32_t m_mapBorder = 0;
    uint8_t* m_reference = nullptr;
    gs_texture_t* m_refTexture = nullptr;
    int32_t m_param1 = 0;
    int32_t m_param2 = 0;
    int32_t m_param3 = 0;
    int32_t m_param4 = 0;
    int32_t m_param5 = 0;
    int32_t m_param6 = 0;
    float m_time = 0.0f;
};

#endif //OBS_STUDIO_OBSCURATOR_HPP
