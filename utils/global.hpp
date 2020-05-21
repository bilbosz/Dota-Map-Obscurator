#ifndef OBS_STUDIO_GLOBAL_HPP
#define OBS_STUDIO_GLOBAL_HPP

#include <algorithm>
#include <cstdint>
#include <map>
#include <memory>
#include <vector>

#ifdef _DEBUG
#define verify( expr )  \
    {                   \
        assert( expr ); \
    }
#else
#define verify( expr ) \
    {                  \
        expr;          \
    }
#endif

#endif //OBS_STUDIO_GLOBAL_HPP
