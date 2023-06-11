#pragma once

#if defined(OpenglObjects_EXPORTS)
#if defined(_MSC_VER)
#define OPENGLOBJECTS_API __declspec(dllexport)
#else
#define OPENGLOBJECTS_API
#endif
#else
#if defined(_MSC_VER)
#define OPENGLOBJECTS_API __declspec(dllimport)
#else
#define OPENGLOBJECTS_API
#endif
#endif


//HDTOOL::AddImportProject
#ifndef OpenglObjects_EXPORTS
#ifdef _DEBUG
#   ifdef _UNICODE
#		ifdef _WIN64 
//HDTOOL::AddImportLibUnicodeDebugx64
#pragma comment(lib,"OpenglObjects/x64/UnicodeDebug/OpenglObjects.lib")
#		else
//HDTOOL::AddImportLibUnicodeDebugWin32
#pragma comment(lib,"OpenglObjects/Win32/UnicodeDebug/OpenglObjects.lib")
#		endif		
#   else
#		ifdef _WIN64 
//HDTOOL::AddImportLibDebugx64
#pragma comment(lib,"OpenglObjects/x64/Debug/OpenglObjects.lib")
#		else
//HDTOOL::AddImportLibDebugWin32
#pragma comment(lib,"OpenglObjects/Win32/Debug/OpenglObjects.lib")
#		endif
#   endif
#else
#   ifdef _UNICODE
#		ifdef _WIN64 
//HDTOOL::AddImportLibUnicodeReleasex64
#pragma comment(lib,"OpenglObjects/x64/UnicodeRelease/OpenglObjects.lib")
#		else
//HDTOOL::AddImportLibUnicodeReleaseWin32
#pragma comment(lib,"OpenglObjects/Win32/UnicodeRelease/OpenglObjects.lib")
#		endif	
#   else
#		ifdef _WIN64 
//HDTOOL::AddImportLibReleasex64
#pragma comment(lib,"OpenglObjects/x64/Release/OpenglObjects.lib")
#		else
//HDTOOL::AddImportLibReleaseWin32
#pragma comment(lib,"OpenglObjects/Win32/Release/OpenglObjects.lib")
#		endif
#   endif
#endif
#endif
//HDTOOL::AddExportClass


#define OPENGLOBJECTS_API

#include <qqmlengine.h>

//#include "GL.h"
#include "yLib.h"
#include "OpenglData.h"
#include "ColorPalette.h"
#include "PiecewisePalette.h"
#include "Coordinate.h"

#include "OpenglMapper.h"
#include "RawReader.h"
#include "STLReader.h"
#include "OpenglImageMapper.h"
#include "OpenglPolyMapper.h"
#include "OpenglSliceMapper.h"
#include "OpenglVolumeMapper.h"
#include "OpenglAxeMapper.h"
#include "MapperItem.h"
#include "OSGItem.h"




