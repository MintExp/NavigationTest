// NavigationTest.h : main header file for the NavigationTest application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CNavigationTestApp:
// See NavigationTest.cpp for the implementation of this class
//

class CNavigationTestApp : public CWinApp
{

public:
	CNavigationTestApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
    // map to the following for file new/open
    afx_msg void OnFileNew();
    afx_msg void OnFileOpen();

	// about
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CNavigationTestApp theApp;