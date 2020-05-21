#ifndef OBS_STUDIO_OBSCURATOR_HPP
#define OBS_STUDIO_OBSCURATOR_HPP

#include "utils/global.hpp"

class Obscurator
{
public:
    Obscurator();
    ~Obscurator();
    void Process( uint8_t* input, uint32_t width, uint32_t height, uint32_t lineSize );
    uint8_t* Output() const;

private:
    uint32_t m_width = 0;
    uint32_t m_height = 0;
    uint32_t m_lineSize = 0;
    uint8_t* m_output = nullptr;
};

#endif //OBS_STUDIO_OBSCURATOR_HPP
