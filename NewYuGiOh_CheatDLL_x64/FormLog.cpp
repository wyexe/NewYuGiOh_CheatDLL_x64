#include "stdafx.h"
#include "FormLog.h"
#include <mutex>

CFormLog& CFormLog::GetInstance()
{
	static CFormLog FormLog;
	return FormLog;
}

VOID CFormLog::SetLoger(_In_ std::function<VOID(CONST std::wstring&)> Loger_)
{
	Loger = Loger_;
}

VOID CFormLog::Print(_In_ LPCWSTR pwszFormat, ...)
{
	static std::mutex MutexLog;
	std::lock_guard<std::mutex> LockGuard(MutexLog);

	va_list		args;
	static wchar_t		szBuff[1024] = { 0 };

	va_start(args, pwszFormat);
	_vsnwprintf_s(szBuff, _countof(szBuff) - 1, _TRUNCATE, pwszFormat, args);
	va_end(args);

	Loger(szBuff);
}
