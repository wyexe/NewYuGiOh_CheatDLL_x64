#include "stdafx.h"
#include "CardExtend.h"
#include <MyTools/Character.h>
#include <MyTools/CLPublic.h>
#include <MyTools/Log.h>

#define _SELF L"CardExtend.cpp"
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

		CCard Card(CCharacter::ReadDWORD(ulAddr) & 0xFFFFFFFF);
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
	dwValue >>= 0x10;
	dwValue <<= 0x10;
	dwValue += dwCardId;
	*reinterpret_cast<DWORD64*>(ulAddr) = dwValue;
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
	for (DWORD64 i = 0; i <= 20; ++i)
	{
		DWORD64 ulCardAddr = ulAddr + i * 4 * 6;
		LOG_CF_D(L"ulCardAddr=%lX, CardId=%lX", ulCardAddr, (ReadDWORD64(ulCardAddr) & 0xFFFF));
		if ((ReadDWORD64(ulCardAddr) & 0xFFFF) == NULL)
			continue;


		dwCardId += static_cast<DWORD>(ReadDWORD64(ulCardAddr) & 0xFFFF0000);
		LOG_CF_D(L"Set Value ulCardAddr=%lX, CardId=%X", ulCardAddr, dwCardId);
		WriteValue(ulCardAddr, dwCardId);
		return TRUE;
	}
	return FALSE;
}

BOOL CCardExtend::SetIniazleCard(_In_ CONST std::vector<DWORD>& VecCard) CONST
{
	if (VecCard.size() > 5)
	{
		MsgBoxLog(L"VecCard.size=%d  > 5!", VecCard.size());
		return FALSE;
	}

	std::vector<CCard> VecGroupCard;
	GetCurrentCardGroup(VecGroupCard);

	DWORD64 ulGroupAddr = CURRENT_CARD_GROUP_BASE - 0x438;
	for (size_t i = 0;i < VecCard.size(); ++i)
	{
		DWORD dwCardId = VecCard.at(i);
		auto p = CLPublic::Vec_find_if(VecGroupCard, [dwCardId](CONST CCard& Card) { return static_cast<DWORD>(Card.GetCardID() & 0xFFFF) == (dwCardId & 0xFFFF); });
		if (p == nullptr)
			continue;

		// swap
		for (size_t j = 0; j < 60; ++j)
		{
			DWORD dwGroupCardId = static_cast<DWORD>(CCharacter::ReadDWORD(ulGroupAddr + j * 4));
			if ((dwGroupCardId & 0xFFFF) == dwCardId)
			{
				DWORD dwHandCardId = static_cast<DWORD>(CCharacter::ReadDWORD(CURRENT_HAND_CARD_BASE + i * 4));
				LOG_CF_D(L"OldHandID=%X, OldGroupID=%X", dwHandCardId, dwGroupCardId);

				// Set Select Card Id to Hand Card
				LOG_CF_D(L"SetHandCardId=%X", dwGroupCardId);
				WriteValue(CURRENT_HAND_CARD_BASE + i * 4, dwGroupCardId);

				// Set Old HandCard to Group
				LOG_CF_D(L"SetGroupCardId=%X", dwHandCardId);
				WriteValue(ulGroupAddr + j * 4, dwHandCardId);

				LOG_CF_D(L"NewHandID=%X, NewGroupID=%X", static_cast<DWORD>(CCharacter::ReadDWORD(CURRENT_HAND_CARD_BASE + i * 4)), static_cast<DWORD>(CCharacter::ReadDWORD(ulGroupAddr + j * 4)));
				break;
			}
		}
	}

	return TRUE;
}
