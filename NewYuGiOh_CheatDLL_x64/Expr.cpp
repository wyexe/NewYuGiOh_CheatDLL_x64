#include "stdafx.h"
#include "Expr.h"
#include <algorithm>
#include <thread>
#include <sstream>
#include "CardExtend.h"

CExpr::CExpr()
{
	std::vector<ExprMethodPtr> Vec =
	{
		{ std::bind(&CExpr::FindCard,this, std::placeholders::_1),				L"FindCard" },
		{ std::bind(&CExpr::SetNextCard,this, std::placeholders::_1),			L"SetNextCard" },
		{ std::bind(&CExpr::SetHookInitialCard,this, std::placeholders::_1),	L"SetHookInitialCard" },
	};
	VecExprMethodPtr = std::move(Vec);
}

CExpr& CExpr::GetInstance()
{
	static CExpr Expr;
	return Expr;
}

DWORD WINAPI CExpr::_WorkThread(LPVOID lpParm)
{
	auto pExprThreadContent = reinterpret_cast<ExprThreadContent*>(lpParm);

	ExprParamecter ExprParamecter_;
	if (!CExpr::GetExprParmeter(pExprThreadContent->CmdText, ExprParamecter_))
	{
		CFormLog::GetInstance().Print(L"Decompose Text Faild!");
		delete pExprThreadContent;
		return 0;
	}

	pExprThreadContent->pExprMethodPtr->MethodPtr(ExprParamecter_);
	delete pExprThreadContent;
	return 0;
}

std::wstring CExpr::MakeTextToUpper(_In_ CONST std::wstring& Text)
{
	std::wstring UpperText;
	for (CONST auto& itm : Text)
		UpperText.push_back(static_cast<wchar_t>(toupper(itm)));

	return UpperText;
}

BOOL CExpr::Run(_In_ CONST std::wstring& CmdText) CONST
{
	std::wstring Cmd = GetCmd(CmdText);
	auto itr = std::find_if(VecExprMethodPtr.begin(), VecExprMethodPtr.end(), [Cmd](CONST ExprMethodPtr ExprPtr) { return ExprPtr.CmdText == Cmd; });
	if (itr == VecExprMethodPtr.end())
		return FALSE;

	auto pExprThreadContent = new ExprThreadContent;
	pExprThreadContent->CmdText = CmdText;
	pExprThreadContent->pExprMethodPtr = &*itr;
	::CloseHandle(::CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)_WorkThread, pExprThreadContent, NULL, NULL));
	return TRUE;
}

VOID CExpr::FindCard(_In_ CONST ExprParamecter& Paramecter) CONST
{
	if (Paramecter.size() == 0)
	{
		CFormLog::GetInstance().Print(L"Parameter.size=0!");
		return;
	}

	CONST std::wstring wsFindType = MakeTextToUpper(Paramecter.at(0));
	if (wsFindType == L"-ALL")
	{
		std::vector<CCard> VecCard;
		CCardExtend::GetInstance().GetALLCard(VecCard);
		for (CONST auto& itm : VecCard)
			CFormLog::GetInstance().Print(L"ID=%X,Name=%s,Detail=%s", static_cast<DWORD>(itm.GetCardID()), itm.GetCardName().c_str(), itm.GetCardDetail().c_str());
	}
	else if (wsFindType == L"-CARD")
	{
		if (Paramecter.size() == 1)
		{
			CFormLog::GetInstance().Print(L"Parameter.size=1!");
			return;
		}

		CONST std::wstring& wsFindText = Paramecter.at(1);


		std::vector<CCard> VecCard;
		CCardExtend::GetInstance().GetALLCard(VecCard);
		for (CONST auto& itm : VecCard)
		{
			if (itm.GetCardName().find(wsFindText) != -1 || itm.GetCardDetail().find(wsFindText) != -1)
			{
				CFormLog::GetInstance().Print(L"ID=%X,Name=%s,Detail=%s", static_cast<DWORD>(itm.GetCardID()), itm.GetCardName().c_str(), itm.GetCardDetail().c_str());
			}
		}
	}
}

VOID CExpr::SetNextCard(_In_ CONST ExprParamecter& Paramecter) CONST
{
	if (Paramecter.size() < 2)
	{
		CFormLog::GetInstance().Print(L"Parameter.size<2!");
		return;
	}

	CONST std::wstring wsFindType = MakeTextToUpper(Paramecter.at(0));
	if (wsFindType == L"-CARDID")
	{
		DWORD dwCardId = static_cast<DWORD>(wcstol(Paramecter.at(1).c_str(), NULL, 16));
		CCardExtend::GetInstance().SetNextCard(dwCardId);
	}
	else if (wsFindType == L"-CARDNAME")
	{
		CONST std::wstring& wsCardName = Paramecter.at(1);

		std::vector<CCard> VecCard;
		CCardExtend::GetInstance().GetALLCard(VecCard);
		
		auto itr = std::find_if(VecCard.begin(), VecCard.end(), [wsCardName](CONST CCard& Card) { return Card.GetCardName() == wsCardName; });
		if (itr == VecCard.end())
		{
			CFormLog::GetInstance().Print(L"UnExist Card:%s", wsCardName.c_str());
			return;
		}

		DWORD dwCardId = static_cast<DWORD>(itr->GetCardID());
		CCardExtend::GetInstance().SetNextCard(dwCardId);
	}
}

VOID CExpr::SetHookInitialCard(_In_ CONST ExprParamecter& Paramecter) CONST
{

}

std::wstring CExpr::GetCmd(_In_ CONST std::wstring& CmdText)
{
	auto Index = CmdText.find(L" ");
	if (Index == -1)
		return CmdText;

	return CmdText.substr(0, Index);
}

BOOL CExpr::GetExprParmeter(_In_ CONST std::wstring& CmdText, _Out_ ExprParamecter& ExprParamecter_)
{
	// FindCard -CardId 1A5C
	auto Index = CmdText.find(L" ");
	if (Index == -1) // Empty Parmacter
		return TRUE;

	// -CardId 1A5C
	std::wstring Text = CmdText.substr(Index + 1);

	auto fnTrimText = [&Text]
	{
		std::wstringstream Trimer;
		Trimer << Text;
		Trimer.clear();
		Trimer >> Text;
	};


	fnTrimText();
	while (!Text.empty())
	{
		// -CardId 1A5C
		Index = Text.find(L" ");
		if (Index == -1)
		{
			ExprParamecter_.push_back(Text);
			return TRUE;
		}

		ExprParamecter_.push_back(Text.substr(0, Index));
		Text = Text.substr(Index + 1);
		fnTrimText();
	}

	
	return TRUE;
}
