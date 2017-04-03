// NewYuGiOh_CheatDLL_x64.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "NewYuGiOh_CheatDLL_x64.h"
#include <thread>
#include <MyTools/Log.h>
#include "Expr.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: If this DLL is dynamically linked against the MFC DLLs,
//		any functions exported from this DLL which call into
//		MFC must have the AFX_MANAGE_STATE macro added at the
//		very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

// CNewYuGiOh_CheatDLL_x64App

BEGIN_MESSAGE_MAP(CNewYuGiOh_CheatDLL_x64App, CWinApp)
END_MESSAGE_MAP()


// CNewYuGiOh_CheatDLL_x64App construction

CNewYuGiOh_CheatDLL_x64App::CNewYuGiOh_CheatDLL_x64App()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CNewYuGiOh_CheatDLL_x64App object

CNewYuGiOh_CheatDLL_x64App theApp;

/*
DWORD WINAPI _ShowThread(LPVOID)
{
	//AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	return 0;
}*/
// CNewYuGiOh_CheatDLL_x64App initialization

BOOL CNewYuGiOh_CheatDLL_x64App::InitInstance()
{
	CWinApp::InitInstance();
	/*static HANDLE hThread = NULL;
	if (hThread == NULL)
	{
		hThread = ::CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)_ShowThread, NULL, NULL, NULL);
	}*/
	CLog::GetInstance().GetLogExpr().SetVecExprFunPtr(CExpr::GetInstance().GetVec());
	CLog::GetInstance().SetClientName(L"YuGiOh", L"C:\\", TRUE, 20 * 1024 * 1024);
	return TRUE;
}
