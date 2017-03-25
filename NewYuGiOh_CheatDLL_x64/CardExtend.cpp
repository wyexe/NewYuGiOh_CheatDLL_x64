#include "stdafx.h"
#include "CardExtend.h"
#include <algorithm>

CCardExtend& CCardExtend::GetInstance()
{
	static CCardExtend CCardExtend_;
	return CCardExtend_;
}

UINT64 CCardExtend::GetALLCard(_Out_opt_ std::vector<CCard>& VecCard) CONST
{
	for (DWORD dwCardId = 0xF3C; dwCardId < 0x3091; ++dwCardId)
	{
		CCard Card(dwCardId);
		if (Card.GetCardAttribute() != em_CardAttribute::em_CardAttribute_Unknow)
			VecCard.push_back(std::move(Card));
	}
	return VecCard.size();
}

UINT64 CCardExtend::GetCurrentCardGroup(_Out_opt_ std::vector<CCard>& VecCard) CONST
{
	auto ulAddr = CURRENT_CARD_GROUP_BASE - 0x438;
	for (int i = 0; i < 60; ++i, ulAddr += 4)
	{
		if (ReadDWORD64(ulAddr) == NULL)
			continue;

		CCard Card(static_cast<DWORD>(ReadDWORD64(ulAddr) & 0xFFFFFFFF));
		if (Card.GetCardAttribute() != em_CardAttribute::em_CardAttribute_Unknow)
			VecCard.push_back(std::move(Card));
	}
	return VecCard.size();
}

BOOL CCardExtend::SetNextCard(_In_ DWORD dwCardId) CONST
{
	/*if (!ExistCardInCardGroup(dwCardId))
	{
		//::MessageBoxW(NULL, L"该ID不存在卡组里面!", L"", NULL);
		return FALSE;
	}*/

	auto ulAddr = CURRENT_CARD_GROUP_BASE - 0x438;
	auto dwValue = ReadDWORD64(ulAddr);
	MsgBoxLog(L"dwValue=%lX", dwValue);
	dwValue >>= 0x10;
	dwValue <<= 0x10;
	dwValue += dwCardId;
	MsgBoxLog(L"dwValue=%lX", dwValue);
	*reinterpret_cast<DWORD64*>(ulAddr) = dwValue;

	/*for (int i = 0; i < 60 && ReadDWORD64(ulAddr) != NULL; ++i, ulAddr += 4)
	{
		if (static_cast<DWORD>(ReadDWORD64(ulAddr) & 0xFFFFFFFF) == dwCardId)
		{
			DWORD64* pulAddr = reinterpret_cast<DWORD64*>(CURRENT_CARD_GROUP_BASE - 0x438);
			auto ulCardId = ReadDWORD64(ulAddr);

			// swap CardId
			*reinterpret_cast<DWORD64*>(ulAddr) = ReadDWORD64(CURRENT_CARD_GROUP_BASE - 0x438);
			*reinterpret_cast<DWORD64*>(CURRENT_CARD_GROUP_BASE - 0x438) = ulCardId;
			break;
		}
	}*/
	return FALSE;
}

BOOL CCardExtend::ExistCardInCardGroup(_In_ DWORD dwCardId) CONST
{
	std::vector<CCard> VecCard;
	GetCurrentCardGroup(VecCard);

	auto itr = std::find_if(VecCard.begin(), VecCard.end(), [dwCardId](CONST CCard& Card){ return static_cast<DWORD>(Card.GetFullID()) == dwCardId; });
	return itr != VecCard.end();
}

BOOL CCardExtend::SetCardToDesk(_In_ DWORD dwCardId) CONST
{
	auto ulAddr = CURRENT_CARD_DESK_BASE + 0x30 + 0x48;
	for (DWORD64 i = 0; i < 5; ++i)
	{
		DWORD64 ulCardAddr = ulAddr + i * 6 * 4;
		MsgBoxLog(L"ulAddr=%llX, Value=%llX",ulCardAddr, (ReadDWORD64(ulCardAddr) & 0xFFFF));
		if ((ReadDWORD64(ulCardAddr) & 0xFFFF) == NULL)
			continue;

		auto dwCardValue = ReadDWORD64(ulCardAddr);

		// remove low Value
		dwCardValue >>= 0x10;
		dwCardValue <<= 0x10;
		dwCardValue += dwCardId;
		*reinterpret_cast<DWORD64*>(ulCardAddr) = dwCardValue;
		MsgBoxLog(L"Write Value=%llX", (ReadDWORD64(ulCardAddr) & 0xFFFF));
		//return TRUE;
	}
	return FALSE;
}
