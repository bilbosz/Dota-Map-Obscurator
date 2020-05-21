#include "dota-map-obscurator.hpp"
#include "utils/global.hpp"

OBS_DECLARE_MODULE()

OBS_MODULE_USE_DEFAULT_LOCALE( "dota-map-obscurator", "en-US" )

bool obs_module_load( void )
{
    DotaMapObscurator::RegisterSourceInfo();
    return true;
}