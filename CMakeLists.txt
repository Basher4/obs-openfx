project(obs-openfx)

set(MODULE_DESCRIPTION "OBS OpenFX Host")

set(obs-openfx_SOURCES
    obs-openfx.c
    "ofx-obs/ofx-source.cpp"
    "ofx-host/ofxBinary.cpp"
    "ofx-host/ofxBinaryDiscovery.cpp"
)

set(obs-openfx_HEADERS
    "ofx-obs/ofx-source.h"
)

set(obs-openfx_OFX_HEADERS
    "openfx/ofxCore.h"
    "openfx/ofxDialog.h"
    "openfx/ofxImageEffect.h"
    "openfx/ofxInteract.h"
    "openfx/ofxKeySyms.h"
    "openfx/ofxMemory.h"
    "openfx/ofxMessage.h"
    "openfx/ofxMultiThread.h"
    "openfx/ofxOld.h"
    "openfx/ofxOpenGLRender.h"
    "openfx/ofxParam.h"
    "openfx/ofxParametricParam.h"
    "openfx/ofxPixels.h"
    "openfx/ofxProgress.h"
    "openfx/ofxProperty.h"
    "openfx/ofxTimeLine.h"
)

add_library(obs-openfx MODULE
    ${obs-openfx_SOURCES}
    ${obs-openfx_HEADERS}
    ${obs-openfx_OFX_HEADERS}
)
target_link_libraries(obs-openfx
    libobs
    Qt5::Core
)

set_target_properties(obs-openfx PROPERTIES FOLDER "plugins")

install_obs_plugin_with_data(obs-openfx data)
