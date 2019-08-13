CMAKE_MINIMUM_REQUIRED( VERSION 3.8 )
PROJECT(TestAnimTimeline)

################################################################################
# Compiler and linker options
SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTORCC ON)
SET(CMAKE_AUTOUIC ON)
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

################################################################################
# Sources and headers
#FIND_PACKAGE(Qt5 COMPONENTS Core Widgets REQUIRED)
find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

FILE(GLOB headers *.h)
FILE(GLOB sources *.cpp)

file(GLOB forms *.ui)
#qt5_wrap_ui(app_uis_moc ${app_uis})

#add_definitions(${Qt5Core_DEFINITIONS})
#add_definitions(${Qt5Widges_DEFINITIONS})

#INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}
include_directories(
#    src/
    ${M_ANIM_TIMELINE_DIR}
#    src/AnimTimeline/ #for ui include "ui_animtimeline.h", moc not visible
#    ${CMAKE_CURRENT_BINARY_DIR}
#    ${CMAKE_CURRENT_SOURCE_DIR}
    )

#add_definitions( ${Qt5Core_DEFINITIONS} )
#add_definitions( ${Qt5Widges_DEFINITIONS} )

add_executable(${PROJECT_NAME}
    ${sources}
    ${forms}
    ${headers}
    )

target_link_libraries(${PROJECT_NAME} ${M_Qt5_LIBRARIES} AnimTimeline)
################################################################################
# Compile target
#ADD_EXECUTABLE(${PROJECT_NAME} ${V1_SOURCE_FILES} ${V1_HEADER_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/resource/timeline.qrc)
#ADD_EXECUTABLE(${PROJECT_NAME} ${sources} ${headers} ${app_uis} src/AnimTimeline/timeline.qrc)
# https://www.kdab.com/using-cmake-with-qt-5/
#qt5_use_modules(${PROJECT_NAME} Core Widgets)

#add_definitions(-DQT_NO_DEBUG_OUTPUT)
#if(CMAKE_BUILD_TYPE MATCHES "Release")
#    add_definitions(-DQT_NO_DEBUG_OUTPUT)
#endif()

add_subdirectory(src)
