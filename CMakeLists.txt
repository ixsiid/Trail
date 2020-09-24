set(plug_sources
    include/plugcontroller.h
    include/plugids.h
    include/plugprocessor.h
    include/version.h
    source/plugfactory.cpp
    source/plugcontroller.cpp
    source/plugprocessor.cpp

    include/log.h
    source/log.cpp
    include/initialize_message.h

    include/gui.h
    source/gui.cpp
    include/waveview.h
    source/waveview.cpp
    include/projectionView.h
    source/projectionView.cpp
    include/numericChangeView.h
    source/numericChangeView.cpp

    include/dft.h
    source/dft.cpp
    include/projection.h
    source/projection.cpp
)

if(SMTG_ADD_VSTGUI)
    include_directories(${ROOT}/vstgui4)
endif()

#--- HERE change the target Name for your plug-in (for ex. set(target myDelay))-------
set(target ${PLUGIN_NAME})

smtg_add_vst3plugin(${target} ${plug_sources})
set_target_properties(${target} PROPERTIES ${SDK_IDE_MYPLUGINS_FOLDER})
target_link_libraries(${target} PRIVATE base sdk vstgui vstgui_support vstgui_uidescription)

if(SMTG_MAC)
    smtg_set_bundle(${target} INFOPLIST "${CMAKE_CURRENT_LIST_DIR}/resource/Info.plist" PREPROCESS)
elseif(SMTG_WIN)
    target_sources(${target} PRIVATE resource/plug.rc)
endif()

add_definitions(-DPLUGIN_NAME="${PLUGIN_NAME}")
add_definitions(-DVENDOR="${VENDOR}")
add_definitions(-DURL="${URL}")
add_definitions(-DMAIL="${MAIL}")
add_definitions(-DCOPYRIGHT="${COPYRIGHT}")
add_definitions(-DTRADEMARK="${TRADEMARK}")
