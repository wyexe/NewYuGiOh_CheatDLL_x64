#ifndef __NEWYUGIOH_CHEATDLL_X64_CARD_CARD_H__
#define __NEWYUGIOH_CHEATDLL_X64_CARD_CARD_H__

#include "GameEnv.h"

class CCard
{
public:
	CCard();
	CCard(_In_ DWORD64 ulCardId);
	~CCard() = default;

	// Get Card Attribute
	em_CardAttribute	GetCardAttribute() CONST;
	CONST std::wstring& GetCardAttributeText() CONST;

	// Get Power
	DWORD				GetPower() CONST;

	// Get Defence
	DWORD				GetDefence() CONST;

	// Get Count of Star 
	DWORD				GetStarCount() CONST;

	// Card Name
	CONST std::wstring&	GetCardName() CONST;

	// Card Detail
	CONST std::wstring& GetCardDetail() CONST;

	DWORD64				GetCardID() CONST;
	DWORD64				GetFullID() CONST;

	bool				IsCard() CONST;

private:
	VOID				SetCardName();

	VOID				SetCardDetail();
private:
	DWORD64				m_dwCardID;
	std::wstring		m_wsCardName;
	std::wstring		m_wsCardDetail;
	mutable em_CardAttribute	m_emCardAttribute;
};



#endif // !__NEWYUGIOH_CHEATDLL_X64_CARD_CARD_H__
