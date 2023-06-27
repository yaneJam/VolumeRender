#project name
project(Opengl)
#cmake version
cmake_minimum_required (VERSION 3.8)
#include(environment.cmake)

get_filename_component(GIT_ROOT "${CMAKE_CURRENT_LIST_DIR}/../../" ABSOLUTE)
message(STATUS "GIT_ROOT:"${GIT_ROOT})
set(REPOSITORY_PATH ${GIT_ROOT})

get_filename_component(PROJECT_FILE_NAME "${CMAKE_CURRENT_LIST_DIR}/../" ABSOLUTE)


#set(Qt_Root "C:/Qt6.3.1/6.3.1/mingw_64/")
get_filename_component(Qt_Root "${CMAKE_CURRENT_LIST_DIR}/../../../tools/Qt6.3.1/mingw_64/" ABSOLUTE)
#get_filename_component(Qt_Root "${CMAKE_CURRENT_LIST_DIR}/../../../tools/qt-mingw-static/" ABSOLUTE)
message(STATUS "Qt_Root:"${Qt_Root})
set(Qt_Bin "${Qt_Root}/bin")
list(APPEND CMAKE_MODULE_PATH "${GIT_ROOT}/cmake")
list(APPEND CMAKE_PREFIX_PATH "${GIT_ROOT}/cmake")
list(APPEND CMAKE_PREFIX_PATH "${Qt_Root}/lib/cmake")
list(APPEND CMAKE_PREFIX_PATH "${Qt_Root}/lib/cmake/Qt6")
set(QT_VERSION_MAJOR 6)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui  Network Qml Quick Widgets Sql Core5Compat OpenGL QuickControls2 REQUIRED QUIET)


set (SLN_NAME "VolumeRender")
set (TARGET_MACHINE "x64")
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

INCLUDE(ySetupProject)
set(Y_BUILD_SLN CACHE STATIC INTERNAL FORCE)



AddProject("${GIT_ROOT}/VolumeRender" OpenglObjects "Objects")
if(EXISTS "${CMAKE_CURRENT_LIST_DIR}/test.cmake")
	include(test.cmake)
endif ()
MACRO_UPDATE_DENPENDENCY()


