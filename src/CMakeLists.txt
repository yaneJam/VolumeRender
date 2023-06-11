#project name
project(Opengl)
#cmake version
cmake_minimum_required (VERSION 3.8)
#include(environment.cmake)

get_filename_component(GIT_ROOT "${CMAKE_CURRENT_LIST_DIR}/../../" ABSOLUTE)
set(REPOSITORY_PATH "D:/Disk/git/thirdparty/repo_vs2019")
get_filename_component(PROJECT_FILE_NAME "${CMAKE_CURRENT_LIST_DIR}/../" ABSOLUTE)


set(Qt_Root "C:/Qt6.3.1/6.3.1/mingw_64/")
set(Qt_Bin "${Qt_Root}bin")
list(APPEND CMAKE_PREFIX_PATH "${Qt_Root}lib/cmake")
list(APPEND CMAKE_PREFIX_PATH "${Qt_Root}lib/cmake/Qt6")
set(QT_VERSION_MAJOR 6)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui  Network Qml Quick Widgets Sql Core5Compat OpenGL QuickControls2 REQUIRED QUIET)


set (SLN_NAME "Opengl")
set (TARGET_MACHINE "x64")
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

INCLUDE(ySetupProject)
set(Y_BUILD_SLN CACHE STATIC INTERNAL FORCE)



AddProject("${GIT_ROOT}/Opengl" OpenglObjects "Objects")
if(EXISTS "${CMAKE_CURRENT_LIST_DIR}/test.cmake")
	include(test.cmake)
endif ()
MACRO_UPDATE_DENPENDENCY()


