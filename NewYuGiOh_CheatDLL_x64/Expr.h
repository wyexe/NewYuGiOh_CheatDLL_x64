#ifndef __NEWYUGIOH_CHEATDLL_X64_EXPR_EXPR_H__
#define __NEWYUGIOH_CHEATDLL_X64_EXPR_EXPR_H__

#include <vector>
#include "FormLog.h"

typedef std::vector<std::wstring> ExprParamecter;
struct ExprMethodPtr
{
	std::function<VOID(CONST ExprParamecter&)> MethodPtr;
	std::wstring CmdText;
};

class CExpr
{
public:
	CExpr();
	~CExpr() = default;
public:
	struct ExprThreadContent
	{
		std::wstring CmdText;
		CONST ExprMethodPtr* pExprMethodPtr;
	};
public:
	static CExpr& GetInstance();

	static BOOL GetExprParmeter(_In_ CONST std::wstring& CmdText, _Out_ ExprParamecter& ExprParamecter_);
public:
	BOOL Run(_In_ CONST std::wstring& CmdText) CONST;
private:
	// 
	VOID FindCard(_In_ CONST ExprParamecter& Paramecter) CONST;

	// Set Next Card
	VOID SetNextCard(_In_ CONST ExprParamecter& Paramecter) CONST;
	
	// Set 5 Card when Start Game
	VOID SetHookInitialCard(_In_ CONST ExprParamecter& Paramecter) CONST;

private:
	static std::wstring GetCmd(_In_ CONST std::wstring& CmdText);

	static DWORD WINAPI _WorkThread(LPVOID lpParm);

	static std::wstring MakeTextToUpper(_In_ CONST std::wstring& Text);
private:
	std::vector<ExprMethodPtr> VecExprMethodPtr;
};



#endif // !__NEWYUGIOH_CHEATDLL_X64_EXPR_EXPR_H__
