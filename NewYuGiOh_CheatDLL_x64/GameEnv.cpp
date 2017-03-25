#include "stdafx.h"
#include "GameEnv.h"

DWORD g_dwNextCardId = 0;

extern DWORD64 ReadDWORD64(_In_ DWORD64 dwAddr)
{
	__try
	{
		if (!::IsBadCodePtr(FARPROC(dwAddr)))
			return *reinterpret_cast<DWORD64*>(dwAddr);
	}
	__except (1)
	{

	}
	return NULL;
}

VOID MsgBoxLog(_In_ LPCWSTR pwszFormat, ...)
{
	va_list		args;
	WCHAR		szBuff[1024];
	va_start(args, pwszFormat);
	_vsnwprintf_s(szBuff, _countof(szBuff) - 1, _TRUNCATE, pwszFormat, args);
	va_end(args);

	//::MessageBoxW(NULL, szBuff, L"Prompt", NULL);
	std::wstring wsText = L"YuGiOh:";
	wsText += szBuff;
	OutputDebugStringW(wsText.c_str());
}
