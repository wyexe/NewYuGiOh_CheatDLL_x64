#include "stdafx.h"
#include "Card.h"
#include <vector>
#include <algorithm>

CCard::CCard() : m_dwCardID(NULL), m_emCardAttribute(em_CardAttribute::em_CardAttribute_Unknow)
{
	
}

CCard::CCard(_In_ DWORD64 dwCardId) : m_dwCardID(dwCardId), m_emCardAttribute(em_CardAttribute::em_CardAttribute_Unknow)
{
	if (GetCardAttribute() != em_CardAttribute::em_CardAttribute_Unknow)
	{
		SetCardName();
		SetCardDetail();
	}
}

DWORD64 CCard::GetCardID() CONST
{
	return m_dwCardID & 0xFFFF;
}

CONST std::wstring& CCard::GetCardDetail() CONST
{
	return m_wsCardDetail;
}

CONST std::wstring& CCard::GetCardName() CONST
{
	return m_wsCardName;
}

em_CardAttribute CCard::GetCardAttribute() CONST
{
	if (m_emCardAttribute != em_CardAttribute::em_CardAttribute_Unknow)
		return m_emCardAttribute;

	if (!IsCard())
		return em_CardAttribute::em_CardAttribute_Unknow;

	DWORD64 ulValue = GetCardID() * 3;
	ulValue *= 2;

	m_emCardAttribute = static_cast<em_CardAttribute>(ReadDWORD64(CARD_ATTRIBUTE_BASE + ulValue * 8 + 0x14));
	return m_emCardAttribute;
}

VOID CCard::SetCardName()
{
	DWORD64 r8	= ReadDWORD64(ReadDWORD64(CARD_NAME_BASE) + 0x18);
	DWORD64 rdx = ReadDWORD64(ReadDWORD64(CARD_NAME_BASE) + 0x28);
	if (r8 == NULL || rdx == NULL)
		return;

	if (!IsCard())
		return;

	auto ulCardID = GetCardID() - 0xF3C;
	DWORD64 ulNamePtr = ReadDWORD64((ReadDWORD64(CARD_NAME_INDEX_BASE + ulCardID * 2) & 0xFFFF) * 8 + rdx) + r8;
	if (ulNamePtr == NULL)
		return;

	WCHAR wszCardName[512] = { 0 };
	memcpy(wszCardName, reinterpret_cast<WCHAR*>(ulNamePtr & 0xFFFFFFFF), _countof(wszCardName) - 1);
	m_wsCardName = wszCardName;
}

bool CCard::IsCard() CONST
{
	return GetCardID() <= 0x3091 && GetCardID() >= 0xF3C;
}

VOID CCard::SetCardDetail()
{
	DWORD64 r8 = ReadDWORD64(ReadDWORD64(CARD_NAME_BASE) + 0x38);
	DWORD64 rdx = ReadDWORD64(ReadDWORD64(CARD_NAME_BASE) + 0x28);
	if (r8 == NULL || rdx == NULL)
		return;

	if (!IsCard())
		return;

	auto ulCardID = GetCardID() - 0xF3C;
	DWORD64 ulDetailPtr = ReadDWORD64((ReadDWORD64(CARD_NAME_INDEX_BASE + ulCardID * 2) & 0xFFFF) * 8 + 0x4 + rdx) + r8;
	if (ulDetailPtr == NULL)
		return;

	WCHAR wszDetailText[512] = { 0 };
	memcpy(wszDetailText, reinterpret_cast<WCHAR*>(ulDetailPtr & 0xFFFFFFFF), _countof(wszDetailText) - 1);
	m_wsCardDetail = wszDetailText;
}

DWORD CCard::GetPower() CONST
{
	if (!IsCard())
		return 0;

	auto RCX = GetCardID() * 3;
	RCX <<= 0x4;

	auto EAX = ReadDWORD64(CARD_ATTRIBUTE_BASE + RCX + 0x4);
	if (EAX == 0x1FF)
		return 0;

	EAX *= 5;
	EAX *= 2;
	return static_cast<DWORD>(EAX);
}

DWORD CCard::GetDefence() CONST
{
	if (!IsCard())
	return 0;

	auto RCX = GetCardID() * 3;
	RCX <<= 0x4;

	auto EAX = ReadDWORD64(CARD_ATTRIBUTE_BASE + RCX + 0x8);
	if (EAX == 0x1FF)
		return 0;

	EAX *= 5;
	EAX *= 2;
	return static_cast<DWORD>(EAX);
}

DWORD CCard::GetStarCount() CONST
{
	if (!IsCard())
	return 0;

	auto RCX = GetCardID() * 3;
	RCX <<= 0x4;

	auto EAX = ReadDWORD64(CARD_ATTRIBUTE_BASE + RCX + 0xC) & 0xFFFFFFFF;
	return static_cast<DWORD>(ReadDWORD64(CARD_ATTRIBUTE_BASE + RCX + 0x18) & 0xFFFFFFFF);
}

CONST std::wstring& CCard::GetCardAttributeText() CONST
{
	struct AttributeText
	{
		em_CardAttribute	emCardAttribute;
		std::wstring		wsText;
	};

	CONST static std::vector<AttributeText> VecAttributeText = 
	{
		{ em_CardAttribute::em_CardAttribute_Magic, L"魔法卡" },
		{ em_CardAttribute::em_CardAttribute_Trap,	L"陷阱卡" },
		{ em_CardAttribute::em_CardAttribute_Land,	L"地" },
		{ em_CardAttribute::em_CardAttribute_Dark,	L"暗" },
		{ em_CardAttribute::em_CardAttribute_Light, L"光" },
		{ em_CardAttribute::em_CardAttribute_Water, L"水" },
		{ em_CardAttribute::em_CardAttribute_Wind,	L"风" },
		{ em_CardAttribute::em_CardAttribute_Fire,	L"炎" },
		{ em_CardAttribute::em_CardAttribute_God,	L"神" },
		{ em_CardAttribute::em_CardAttribute_Dark2, L"暗" },
	};
	CONST static std::wstring wsUnknow = L"未知";

	auto itr = std::find_if(VecAttributeText.begin(), VecAttributeText.end(), [this](CONST AttributeText& AttributeText_){ return AttributeText_.emCardAttribute == m_emCardAttribute; });
	return itr == VecAttributeText.end() ? wsUnknow : itr->wsText;
}

DWORD64 CCard::GetFullID() CONST
{
	return m_dwCardID;
}
