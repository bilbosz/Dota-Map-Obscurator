#include "graphics-scope.hpp"
#include <obs-module.h>

GraphicsScope::GraphicsScope()
{
    obs_enter_graphics();
}

GraphicsScope::~GraphicsScope()
{
    obs_leave_graphics();
}
