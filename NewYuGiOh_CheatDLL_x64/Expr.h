#ifndef __NEWYUGIOH_CHEATDLL_X64_EXPR_EXPR_H__
#define __NEWYUGIOH_CHEATDLL_X64_EXPR_EXPR_H__

#include <vector>
#include <MyTools/CLExpression.h>
#include <MyTools/ClassInstance.h>

class CExpr : public CExprFunBase, virtual public CClassInstance<CExpr>
{
public:
	CExpr();
	virtual ~CExpr();
public:
	virtual VOID Release();

	virtual std::vector<ExpressionFunPtr>& GetVec();

private:
	virtual VOID Help(_In_ CONST std::vector<std::wstring>&);

	// 
	VOID FindCard(_In_ CONST std::vector<std::wstring>&);

	// Set Next Card
	VOID SetDeskCard(_In_ CONST std::vector<std::wstring>&);

	// Set 5 Card when Start Game
	VOID SetInitialCard(_In_ CONST std::vector<std::wstring>&);
};



#endif // !__NEWYUGIOH_CHEATDLL_X64_EXPR_EXPR_H__
