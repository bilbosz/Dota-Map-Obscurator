project(dota-map-obscurator)

set(CMAKE_CXX_FLAGS -Wno-sign-compare)

set(dota-map-obscurator_SOURCES
        loader.cpp
        dota-map-obscurator.cpp
        utils/graphics-scope.cpp obscurator.cpp obscurator.hpp)

add_library(dota-map-obscurator MODULE
        ${dota-map-obscurator_SOURCES})
target_link_libraries(dota-map-obscurator
        libobs)

install_obs_plugin_with_data(dota-map-obscurator data)