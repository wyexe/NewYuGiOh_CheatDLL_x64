#include "stdafx.h"
#include "Expr.h"
#include <MyTools/Log.h>
#include <MyTools/Character.h>
#include "CardExtend.h"

#define _SELF L"Expr.cpp"
CExpr::CExpr()
{
	
}


CExpr::~CExpr()
{

}

VOID CExpr::Release()
{

}

std::vector<ExpressionFunPtr>& CExpr::GetVec()
{
	static std::vector<ExpressionFunPtr> Vec = 
	{
		{ std::bind(&CExpr::Help, this, std::placeholders::_1), L"Help" },
		{ std::bind(&CExpr::FindCard, this, std::placeholders::_1), L"FindCard" },
		{ std::bind(&CExpr::SetDeskCard, this, std::placeholders::_1), L"SetDeskCard" },
		{ std::bind(&CExpr::SetInitialCard, this, std::placeholders::_1), L"SetInitialCard" },
	};
	return Vec;
}

VOID CExpr::Help(_In_ CONST std::vector<std::wstring>&)
{
	auto& Vec = GetVec();
	for (CONST auto& itm : Vec)
		LOG_C(CLog::em_Log_Type::em_Log_Type_Custome, L"FunctionName=%s", itm.wsFunName.c_str());
}

VOID CExpr::FindCard(_In_ CONST std::vector<std::wstring>& VecParm)
{
	if (VecParm.size() == 0)
	{
		LOG_CF_E(L"Parameter.size=0!");
		return;
	}

	CONST std::wstring wsFindType = CCharacter::MakeTextToUpper(VecParm.at(0));
	if (wsFindType == L"ALL")
	{
		std::vector<CCard> VecCard;
		CCardExtend::GetInstance().GetALLCard(VecCard);
		for (CONST auto& itm : VecCard)
			LOG_CF_D(L"ID=%X,Name=%s,Detail=%s", static_cast<DWORD>(itm.GetCardID()), itm.GetCardName().c_str(), itm.GetCardDetail().c_str());
	}
	if (wsFindType == L"GROUP")
	{
		std::vector<CCard> VecCard;
		CCardExtend::GetInstance().GetCurrentCardGroup(VecCard);
		LOG_CF_D(L"GroupSize=%d", static_cast<DWORD>(VecCard.size()));
		for (CONST auto& itm : VecCard)
			LOG_CF_D(L"ID=%X,Name=%s,Detail=%s", static_cast<DWORD>(itm.GetCardID()), itm.GetCardName().c_str(), itm.GetCardDetail().c_str());
	}
	else if (wsFindType == L"CARDNAME")
	{
		if (VecParm.size() == 1)
		{
			LOG_CF_E(L"Parameter.size=1!");
			return;
		}

		CONST std::wstring& wsFindText = VecParm.at(1);


		std::vector<CCard> VecCard;
		CCardExtend::GetInstance().GetALLCard(VecCard);
		for (CONST auto& itm : VecCard)
		{
			if (itm.GetCardName().find(wsFindText) != -1)
			{
				LOG_CF_D(L"ID=%X,Name=%s,Detail=%s", static_cast<DWORD>(itm.GetCardID()), itm.GetCardName().c_str(), itm.GetCardDetail().c_str());
			}
		}
	}
	else if (wsFindType == L"CARDID")
	{
		if (VecParm.size() == 1)
		{
			LOG_CF_E(L"Parameter.size=1!");
			return;
		}

		DWORD dwCardId = wcstol(VecParm.at(1).c_str(), nullptr, 16);


		std::vector<CCard> VecCard;
		CCardExtend::GetInstance().GetALLCard(VecCard);
		for (CONST auto& itm : VecCard)
		{
			if (static_cast<DWORD>(itm.GetCardID()) == dwCardId)
			{
				LOG_CF_D(L"ID=%X,Name=%s,Detail=%s", static_cast<DWORD>(itm.GetCardID()), itm.GetCardName().c_str(), itm.GetCardDetail().c_str());
			}
		}
	}
}

VOID CExpr::SetDeskCard(_In_ CONST std::vector<std::wstring>& VecParm)
{
	if (VecParm.size() < 2)
	{
		LOG_CF_E(L"Parameter.size<2!");
		return;
	}

	CONST std::wstring wsFindType = CCharacter::MakeTextToUpper(VecParm.at(0));
	if (wsFindType == L"CARDID")
	{
		DWORD dwCardId = static_cast<DWORD>(wcstol(VecParm.at(1).c_str(), NULL, 16));
		CCardExtend::GetInstance().SetCardToDesk(dwCardId);
	}
}

VOID CExpr::SetInitialCard(_In_ CONST std::vector<std::wstring>& VecParm)
{
	std::vector<DWORD> VecCard;
	for (CONST auto& itm : VecParm)
		VecCard.push_back(wcstol(itm.c_str(), nullptr, 16));

	CCardExtend::GetInstance().SetIniazleCard(VecCard);
}