#include "vec-4-u.hpp"

#define DEFINE_ASSIGN_OPERATOR( symbol )                \
    Vec4u& Vec4u::operator symbol( const Vec4u& other ) \
    {                                                   \
        this->x symbol other.x;                         \
        this->y symbol other.y;                         \
        this->z symbol other.z;                         \
        this->w symbol other.w;                         \
        return *this;                                   \
    }

#define DEFINE_BINARY_OPERATOR( symbol )                                  \
    Vec4u operator symbol( const Vec4u& self, const Vec4u& other ) \
    {                                                                     \
        Vec4u ret;                                                        \
        ret.x = self.x symbol other.x;                                    \
        ret.y = self.y symbol other.y;                                    \
        ret.z = self.z symbol other.z;                                    \
        ret.w = self.w symbol other.w;                                    \
        return ret;                                                       \
    }

// clang-format off
DEFINE_ASSIGN_OPERATOR( = )
DEFINE_ASSIGN_OPERATOR( += )
DEFINE_ASSIGN_OPERATOR( -= )
DEFINE_ASSIGN_OPERATOR( *= )
DEFINE_ASSIGN_OPERATOR( /= )

DEFINE_BINARY_OPERATOR( + )
DEFINE_BINARY_OPERATOR( - )
DEFINE_BINARY_OPERATOR( * )
DEFINE_BINARY_OPERATOR( / )
// clang-format on

#undef DEFINE_ASSIGN_OPERATOR
#undef DEFINE_BINARY_OPERATOR