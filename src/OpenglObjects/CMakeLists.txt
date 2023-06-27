#This CMakeLists.txt is created by HDTool V1
#project name
project(OpenglObjects)

#cmake version
cmake_minimum_required (VERSION 3.8)
#system environment
set(QTENHANCED TRUE)#HDTOOL::AddQtEnhance


MACRO_PROJECT_ENVIRONMENT()

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(QRC_SOURCE_FILES "resources/resources.qrc")
qt6_add_resources(QRC_FILES ${QRC_SOURCE_FILES})
file(GLOB sources "./*.cpp" "./*.h")
file(GLOB headers "${PROJECT_SOURCE_DIR}/../../inc/${PROJECT_NAME}/*.h")
file(GLOB src_dir  "*.h" )
#add_executable(${PROJECT_NAME} ${sources} ${headers} ${src_dir} ${QRC_FILES})
add_library(${PROJECT_NAME} SHARED ${sources} ${headers} ${src_dir} ${QRC_FILES})
qt_disable_unicode_defines(${PROJECT_NAME})

#target_link_libraries(${PROJECT_NAME} Qt${QT_VERSION_MAJOR}::Widgets)	
#target_link_libraries(${PROJECT_NAME} Qt${QT_VERSION_MAJOR}::Sql)		
#target_link_libraries(${PROJECT_NAME} Qt${QT_VERSION_MAJOR}::Network)
#target_link_libraries(${PROJECT_NAME} Qt${QT_VERSION_MAJOR}::Core5Compat)	
target_link_libraries(${PROJECT_NAME} Qt${QT_VERSION_MAJOR}::Quick)	
target_link_libraries(${PROJECT_NAME} Qt${QT_VERSION_MAJOR}::QuickControls2)
target_link_libraries(${PROJECT_NAME} Qt${QT_VERSION_MAJOR}::OpenGL)		

#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${GIT_ROOT}/../src")#FindyLib.cmake
find_package(yLib)
if(yLib_FOUND)
    target_include_directories(${PROJECT_NAME} PRIVATE ${yLib_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} ${yLib_LIBRARY})
else(yLib_FOUND)
    message(FATAL_ERROR "yLib library not found")
endif(yLib_FOUND)

target_precompile_headers(${PROJECT_NAME} PRIVATE stdafx.h)

#MACRO_PROJECT_DENPENDENCY(${PROJECT_NAME} yLib)

#set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_COMMAND "$(OutDir)../../../../x64/Release/yane.exe" )
