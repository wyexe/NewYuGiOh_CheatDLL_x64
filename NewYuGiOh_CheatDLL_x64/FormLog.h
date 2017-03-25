#ifndef __NEWYUGIOH_CHEATDLL_X64_LOG_FORMLOG_H__
#define __NEWYUGIOH_CHEATDLL_X64_LOG_FORMLOG_H__
#include <string>
#include <functional>

class CFormLog
{
public:
	CFormLog() = default;
	~CFormLog() = default;

public:
	static CFormLog& GetInstance();
public:
	VOID SetLoger(_In_ std::function<VOID(CONST std::wstring&)> Loger_);

	VOID Print(_In_ LPCWSTR pwszFormat, ...);
private:
	std::function<VOID(CONST std::wstring&)> Loger;
};



#endif // !__NEWYUGIOH_CHEATDLL_X64_LOG_FORMLOG_H__
