#ifndef OBS_STUDIO_VEC_4_U_HPP
#define OBS_STUDIO_VEC_4_U_HPP

#include <cstdint>

union Vec3u;

#define DECLARE_ASSIGN_OPERATOR( symbol ) Vec4u& operator symbol( const Vec4u& other )
#define DECLARE_BINARY_OPERATOR( symbol ) Vec4u operator symbol( const Vec4u& self, const Vec4u& other )

union Vec4u
{
    struct
    {
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;
    };
    struct
    {
        uint8_t x;
        uint8_t y;
        uint8_t z;
        uint8_t w;
    };
    uint32_t word;

    // clang-format off
    DECLARE_ASSIGN_OPERATOR( = );
    DECLARE_ASSIGN_OPERATOR( += );
    DECLARE_ASSIGN_OPERATOR( -= );
    DECLARE_ASSIGN_OPERATOR( *= );
    DECLARE_ASSIGN_OPERATOR( /= );
    // clang-format on
};

DECLARE_BINARY_OPERATOR( + );
DECLARE_BINARY_OPERATOR( - );
DECLARE_BINARY_OPERATOR( * );
DECLARE_BINARY_OPERATOR( / );

#undef DECLARE_ASSIGN_OPERATOR
#undef DECLARE_BINARY_OPERATOR

#endif //OBS_STUDIO_VEC_4_U_HPP
