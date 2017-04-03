#ifndef __NEWYUGIOH_CHEATDLL_X64_CARD_CARDEXTEND_H__
#define __NEWYUGIOH_CHEATDLL_X64_CARD_CARDEXTEND_H__

#include "Card.h"
#include <vector>

class CCardExtend
{
public:
	CCardExtend() = default;
	~CCardExtend() = default;

	static CCardExtend& GetInstance();

	// Get ALL Card
	UINT64 GetALLCard(_Out_opt_ std::vector<CCard>& VecCard) CONST;

	// Get Count of CurrentCardGroup
	UINT64 GetCurrentCardGroup(_Out_opt_ std::vector<CCard>& VecCard) CONST;

	// Set Next Card by CardID
	BOOL   SetNextCard(_In_ DWORD dwCardId) CONST;

	// Set Card To Desk
	BOOL   SetCardToDesk(_In_ DWORD dwCardId) CONST;

	BOOL   SetIniazleCard(_In_ CONST std::vector<DWORD>& VecCard) CONST;
private:
	BOOL   ExistCardInCardGroup(_In_ DWORD dwCardId) CONST;
};



#endif // !__NEWYUGIOH_CHEATDLL_X64_CARD_CARDEXTEND_H__