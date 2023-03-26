project(obs-openfx)

add_library(obs-openfx MODULE)
add_library(OBS::openfx ALIAS obs-openfx)

target_link_libraries(obs-openfx PRIVATE OBS::libobs)

find_package(Qt6 REQUIRED COMPONENTS Core)
target_link_libraries(obs-openfx PRIVATE Qt6::Core)

set_target_properties(obs-openfx PROPERTIES FOLDER "plugins")

set(MODULE_DESCRIPTION "OBS OpenFX Host")

set(obs-openfx_PLUGIN_SRC
    src/logging.h
    src/names.h
    src/obs-openfx.cpp
)

file(GLOB OFX_HEADER_FILES "include/openfx/*.h")
source_group("Header Files\\openfx" FILES ${OFX_HEADER_FILES})

file(GLOB obs-openfx_SOURCES "src/ofx-host/*.cpp")
source_group("Source Files\\ofx-host" FILES ${obs-openfx_SOURCES})

file(GLOB obs-openfx_HEADERS "src/ofx-host/*.h")
source_group("Header Files\\ofx-host" FILES ${obs-openfx_HEADERS})

target_include_directories(obs-openfx PRIVATE include)
target_sources(obs-openfx PRIVATE
    ${obs-openfx_PLUGIN_SRC}
    ${obs-openfx_SOURCES}
    ${obs-openfx_HEADERS}
    ${OFX_HEADER_FILES}
)

target_compile_definitions(
  obs-openfx PRIVATE UNICODE _UNICODE _CRT_SECURE_NO_WARNINGS
                   _CRT_NONSTDC_NO_WARNINGS)

setup_plugin_target(obs-openfx data)
