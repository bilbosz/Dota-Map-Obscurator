project(dota-map-obscurator)

set(dota-map-obscurator_SOURCES
        dota-map-obscurator.c)

add_library(dota-map-obscurator MODULE
        ${dota-map-obscurator_SOURCES})
target_link_libraries(dota-map-obscurator
        libobs)

install_obs_plugin_with_data(dota-map-obscurator data)