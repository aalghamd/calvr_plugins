SET(LIB_NAME OsgMovie)

FIND_PACKAGE(Fmod)

if(FMOD_FOUND)
    SET(FMOD_HEADER FmodAudioSink.h)
    SET(FMOD_SOURCE FmodAudioSink.cpp)
ENDIF(FMOD_FOUND)

SET(PLUGIN_HEADERS
    OsgMovie.h
    ${FMOD_HEADER}
)

ADD_LIBRARY(${LIB_NAME}
    "SHARED"
    ${PLUGIN_HEADERS}
    OsgMovie.cpp
    ${FMOD_SOURCE}
)

FIND_PACKAGE(Fmod)

CONFIGURE_FILE(config.h.in config.h)

INCLUDE_DIRECTORIES(${OSG_INCLUDE_DIR})
TARGET_LINK_LIBRARIES(${LIB_NAME} ${OSG_LIBRARIES})

if(FMOD_FOUND)
    INCLUDE_DIRECTORIES(${FMOD_INCLUDE_DIR})
    TARGET_LINK_LIBRARIES(${LIB_NAME} ${FMOD_LIBRARY})
ENDIF(FMOD_FOUND)

INSTALL(TARGETS ${LIB_NAME} DESTINATION lib/plugins)

