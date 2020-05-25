project(dota-map-obscurator)

if ($CMAKE_C_COMPILER_ID STREQUAL GNU)
    set(CMAKE_CXX_FLAGS -Wno-sign-compare)
endif (UNIX)

set(dota-map-obscurator_SOURCES
        loader.cpp
        dota-map-obscurator.cpp
        utils/graphics-scope.cpp obscurator.cpp utils/vec-4-u.cpp)

add_library(dota-map-obscurator MODULE
        ${dota-map-obscurator_SOURCES})
target_link_libraries(dota-map-obscurator
        libobs)

install_obs_plugin_with_data(dota-map-obscurator data)