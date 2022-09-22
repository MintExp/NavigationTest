#ifndef ____SMART_PATH_EXPORT_H____
#define ____SMART_PATH_EXPORT_H____

#include "SmartDefine.h"

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the GAMEUTIL_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// GAMEEUTIL_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef SMART_PATH_EXPORT
#define SMART_PATH_API __declspec(dllexport)
#else
#define SMART_PATH_API __declspec(dllimport)
#endif

// This is main function exported from PathImpl.dll
extern "C"
{
	SMART_PATH_API SMART_HANDLE Smart_Create(LPCSTR lpszPrivateCode);
    SMART_PATH_API BOOL Smart_LoadNavMesh(SMART_HANDLE hHandle, LPCSTR lpszNavMeshFile);
    SMART_PATH_API BOOL Smart_GetMeshRegion(SMART_HANDLE hHandle, REGION2D& stRegion);
	SMART_PATH_API BOOL Smart_GetTriangles(SMART_HANDLE hHandle, TRIANGLE3D_LIST& lstTriangles);
    SMART_PATH_API BOOL Smart_IsPointOnNavMesh(SMART_HANDLE hHandle, const POINT3D& point);
	SMART_PATH_API BOOL Smart_SearchPath(SMART_HANDLE hHandle, const POINT3D& ptStart, const POINT3D& ptEnd, POINT3D_LIST& lstPathPoints);
    SMART_PATH_API BOOL Smart_SearchPathEx(SMART_HANDLE hHandle, const POINT3D& ptStart, const POINT3D& ptEnd,
        POINT3D_LIST& lstOrgPathPoints, POINT3D_LIST& lstOptPathPoints1, POINT3D_LIST& lstOptPathPoints2, POINT3D_LIST& lstOptPathPoints3);
    SMART_PATH_API void Smart_Destroy(SMART_HANDLE hHandle);
}


#endif
