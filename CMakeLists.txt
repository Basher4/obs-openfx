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
file(GLOB obs-openfx_HEADERS "src/ofx-host/*.h")
source_group("Source Files\\ofx-host" FILES ${obs-openfx_SOURCES})
source_group("Header Files\\ofx-host" FILES ${obs-openfx_HEADERS})

file(GLOB obs-openfx-suites_SOURCES "src/ofx-host/suites/*.cpp")
file(GLOB obs-openfx-suites_HEADERS "src/ofx-host/suites/*.h")
source_group("Source Files\\ofx-host\\suites" FILES ${obs-openfx-suites_SOURCES})
source_group("Header Files\\ofx-host\\suites" FILES ${obs-openfx-suites_HEADERS})

file(GLOB obs-openfx-image-effect_SOURCES "src/ofx-host/image-effect/*.cpp")
file(GLOB obs-openfx-image-effect_HEADERS "src/ofx-host/image-effect/*.h")
source_group("Source Files\\ofx-host\\image-effect" FILES ${obs-openfx-image-effect_SOURCES})
source_group("Header Files\\ofx-host\\image-effect" FILES ${obs-openfx-image-effect_HEADERS})

set(obs-openfx_PROJECT_FILES ${obs-openfx_SOURCES} ${obs-openfx_HEADERS}
                             ${obs-openfx-suites_SOURCES} ${obs-openfx-suites_HEADERS}
                             ${obs-openfx-image-effect_SOURCES} ${obs-openfx-image-effect_HEADERS})

target_include_directories(obs-openfx PRIVATE include src)
target_sources(obs-openfx PRIVATE
    ${obs-openfx_PLUGIN_SRC}
    ${obs-openfx_PROJECT_FILES}
    ${OFX_HEADER_FILES}
)

target_compile_definitions(
  obs-openfx PRIVATE UNICODE _UNICODE _CRT_SECURE_NO_WARNINGS
                   _CRT_NONSTDC_NO_WARNINGS)

setup_plugin_target(obs-openfx data)
