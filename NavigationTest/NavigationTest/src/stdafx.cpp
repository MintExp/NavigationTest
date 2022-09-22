// stdafx.cpp : source file that includes just the standard includes
// NavigationTest.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"


// Set process working directory
BOOL SetProcessWorkingDir()
{
    // Path name buffer
    TCHAR szPathName[MAX_PATH];

    // Get current executable file GameX.exe path name
    DWORD dwLen = GetModuleFileName(NULL, szPathName, MAX_PATH - 1);
    if (!dwLen)
    {
        return FALSE;
    }
    szPathName[dwLen] = _T('\0');

    // Get current executable file path
    while (dwLen > 0 && szPathName[--dwLen] != _T('\\'));
    if (!dwLen)
    {
        return FALSE;
    }

    // Cut file name off
    szPathName[dwLen + 1] = _T('\0');

    return SetCurrentDirectory(szPathName);
}

float Ranged_Random(float fMin, float fMax)
{
    const float fRange = fMax - fMin;

    const float fRand = (float)rand()/(float)RAND_MAX; // 0.0 - 1.0

    return fRand * fRange + fMin;
}