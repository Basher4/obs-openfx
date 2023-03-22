project(obs-openfx)

add_library(obs-openfx MODULE)
add_library(OBS::openfx ALIAS obs-openfx)

target_link_libraries(obs-openfx PRIVATE OBS::libobs)

find_package(Qt6 REQUIRED COMPONENTS Core)
target_link_libraries(obs-openfx PRIVATE Qt6::Core)

set_target_properties(obs-openfx PROPERTIES FOLDER "plugins")

set(MODULE_DESCRIPTION "OBS OpenFX Host")

set(obs-openfx_SOURCES
    src/obs-openfx.c
    src/ofx-obs/ofx-source.cpp
    src/ofx-host/ofxBinary.cpp
    src/ofx-host/ofxBinaryDiscovery.cpp
)

set(obs-openfx_HEADERS
    src/ofx-obs/ofx-source.h
    src/ofx-host/ofxBinary.h
    src/ofx-host/ofxBinaryDiscovery.h
 )

set(obs-openfx_OFX_HEADERS
    include/openfx/ofxCore.h
    include/openfx/ofxDialog.h
    include/openfx/ofxImageEffect.h
    include/openfx/ofxInteract.h
    include/openfx/ofxKeySyms.h
    include/openfx/ofxMemory.h
    include/openfx/ofxMessage.h
    include/openfx/ofxMultiThread.h
    include/openfx/ofxOld.h
    include/openfx/ofxOpenGLRender.h
    include/openfx/ofxParam.h
    include/openfx/ofxParametricParam.h
    include/openfx/ofxPixels.h
    include/openfx/ofxProgress.h
    include/openfx/ofxProperty.h
    include/openfx/ofxTimeLine.h
)

target_include_directories(obs-openfx PRIVATE include)
target_sources(obs-openfx PRIVATE
    ${obs-openfx_SOURCES}
    ${obs-openfx_HEADERS}
    ${obs-openfx_OFX_HEADERS}
)

target_compile_definitions(
  obs-openfx PRIVATE UNICODE _UNICODE _CRT_SECURE_NO_WARNINGS
                   _CRT_NONSTDC_NO_WARNINGS)

setup_plugin_target(obs-openfx data)
