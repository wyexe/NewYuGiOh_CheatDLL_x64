// NewYuGiOh_CheatDLL_x64.h : main header file for the NewYuGiOh_CheatDLL_x64 DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CNewYuGiOh_CheatDLL_x64App
// See NewYuGiOh_CheatDLL_x64.cpp for the implementation of this class
//

class CNewYuGiOh_CheatDLL_x64App : public CWinApp
{
public:
	CNewYuGiOh_CheatDLL_x64App();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
