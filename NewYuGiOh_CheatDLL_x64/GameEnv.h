#ifndef __NEWYUGIOH_CHEATDLL_X64_CARD_GAMEENV_H__
#define __NEWYUGIOH_CHEATDLL_X64_CARD_GAMEENV_H__

#include <Windows.h>
#include <string>
#include <stdint.h>

enum em_CardAttribute
{
	em_CardAttribute_Magic = 0x8,
	em_CardAttribute_Trap = 0x9,
	em_CardAttribute_Land = 0x5,
	em_CardAttribute_Dark = 0x2,
	em_CardAttribute_Light = 0x1,
	em_CardAttribute_Water = 0x3,
	em_CardAttribute_Wind = 0x6,
	em_CardAttribute_Fire = 0x4,
	em_CardAttribute_God = 0x7,
	em_CardAttribute_Dark2 = 0x0,
	em_CardAttribute_Unknow = 0x10,
};

#define CARD_ATTRIBUTE_BASE			(DWORD64)0x0000000141019880
#define CARD_NAME_BASE				(DWORD64)0x140DA1240
#define CARD_NAME_INDEX_BASE		(DWORD64)0x140F7F220
#define CURRENT_CARD_GROUP_BASE		(DWORD64)0x14117DD30
#define CURRENT_CARD_DESK_BASE		(DWORD64)0x14117D580
#define CURRENT_HAND_CARD_BASE		(DWORD64)0x14117D718

extern inline DWORD64 ReadDWORD64(_In_ DWORD64 dwAddr);
extern VOID WriteValue(_In_ DWORD64 dwAddr, _In_ DWORD dwValue);
extern VOID MsgBoxLog(_In_ LPCWSTR pwszFormat,...);


extern DWORD g_dwNextCardId;

#endif // !__NEWYUGIOH_CHEATDLL_X64_CARD_GAMEENV_H__