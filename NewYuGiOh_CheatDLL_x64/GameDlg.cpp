// GameDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NewYuGiOh_CheatDLL_x64.h"
#include "GameDlg.h"
#include "afxdialogex.h"
#include "CardExtend.h"
#include <algorithm>

// CGameDlg dialog

IMPLEMENT_DYNAMIC(CGameDlg, CDialogEx)

CGameDlg::CGameDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGameDlg::IDD, pParent)
{

}

CGameDlg::~CGameDlg()
{
}

void CGameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGameDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_GETALLCARD, &CGameDlg::OnBnClickedBtnGetallcard)
	ON_BN_CLICKED(IDC_SETNEXTCARD, &CGameDlg::OnBnClickedSetnextcard)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CGameDlg::OnNMDblclkList1)
	ON_BN_CLICKED(IDC_BUTTON1, &CGameDlg::OnBnClickedButton1)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CGameDlg::OnNMClickList1)
END_MESSAGE_MAP()

BOOL CGameDlg::OnInitDialog()
{
	CListCtrl * m_List = ((CListCtrl *)GetDlgItem(IDC_LIST1));
	if (m_List != NULL)
		m_List->SetExtendedStyle(m_List->GetExtendedStyle() | LVS_EX_FULLROWSELECT);

	CComboBox* pCCB_NextCard = static_cast<CComboBox *>(GetDlgItem(IDC_CCB_NextCard));
	pCCB_NextCard->InsertString(0, L"影身女妖的羽毛扫");
	pCCB_NextCard->InsertString(1, L"雷击");

	CComboBox* pCCB_DeskCard = static_cast<CComboBox *>(GetDlgItem(IDC_CCB_DeskCard));
	pCCB_DeskCard->InsertString(0, L"科技属 戟炮手(1回合破坏一次召唤)");
	pCCB_DeskCard->InsertString(1, L"圣珖神龙 星尘·零(1回合一次无视怪物效果)");
	pCCB_DeskCard->InsertString(2, L"红莲新星龙(魔法陷阱破坏无效)");
	pCCB_DeskCard->InsertString(3, L"究极幻神(给对方也召唤衍生物)");
	pCCB_DeskCard->InsertString(4, L"五神龙(攻击5000)");
	pCCB_DeskCard->InsertString(5, L"极战机王 战神机人");
	pCCB_DeskCard->InsertString(6, L"混沌幻魔(自己回合攻击+1W)");
	pCCB_DeskCard->InsertString(7, L"极度悲伤的魔龙(破坏怪物并且掉血)");
	pCCB_DeskCard->InsertString(8, L"星态龙");
	pCCB_DeskCard->InsertString(9, L"合神龙 迪迈奥斯");
	pCCB_DeskCard->InsertString(10, L"邪神 抹灭者");
	pCCB_DeskCard->InsertString(11, L"邪神 恐惧之源");
	pCCB_DeskCard->InsertString(12, L"至尊太阳");
	pCCB_DeskCard->InsertString(13, L"幻魔皇");
	pCCB_DeskCard->InsertString(14, L"隐藏的机壳杀手 物质主义");
	pCCB_DeskCard->InsertString(15, L"混沌帝龙 -终焉的使者-");
	pCCB_DeskCard->InsertString(16, L"暴君爆风龙");
	pCCB_DeskCard->InsertString(17, L"龙骑士 黑魔术少女");
	pCCB_DeskCard->InsertString(18, L"女神的圣弓");
	pCCB_DeskCard->InsertString(19, L"反射镜力龙");

	return TRUE;
}


// CGameDlg message handlers
LPCWSTR ConvertNumber(__in LPCWSTR pwszFormat, ...)
{
	va_list		args;
	static wchar_t		szBuff[1024] = { 0 };

	va_start(args, pwszFormat);
	_vsnwprintf_s(szBuff, _countof(szBuff) - 1, _TRUNCATE, pwszFormat, args);
	va_end(args);

	return szBuff;
}

void CGameDlg::OnBnClickedBtnGetallcard()
{
	CListCtrl * m_List = (CListCtrl *)GetDlgItem(IDC_LIST1);
	m_List->DeleteAllItems();
	int nColumnCount = m_List->GetHeaderCtrl()->GetItemCount();
	for (int i = 0; i < nColumnCount; i++)
	{
		m_List->DeleteColumn(0);
	}

	m_List->InsertColumn(0, L"ID", LVCFMT_LEFT, 80);
	m_List->InsertColumn(1, L"Name", LVCFMT_LEFT, 150);
	m_List->InsertColumn(2, L"Star", LVCFMT_LEFT, 30);
	m_List->InsertColumn(3, L"Power/Defence", LVCFMT_LEFT, 100);
	m_List->InsertColumn(4, L"Attribute", LVCFMT_LEFT, 50);
	m_List->InsertColumn(5, L"Detail", LVCFMT_LEFT, 400);
	
	std::vector<CCard> VecCard;
	CCardExtend::GetInstance().GetALLCard(VecCard);

	CEdit* pEdit = reinterpret_cast<CEdit*>(GetDlgItem(IDC_EDIT_CARDNAME));
	CString strText;
	pEdit->GetWindowTextW(strText);

	if (strText.Trim() != L"")
	{
		VecCard.erase(std::remove_if(VecCard.begin(), VecCard.end(), [&strText](CONST CCard& Card){ return Card.GetCardName().find(strText.GetBuffer()) == -1 && Card.GetCardAttributeText().find(strText.GetBuffer()) == -1; }), VecCard.end());
	}

	for (CONST auto& itm : VecCard)
	{
		int nRow = m_List->InsertItem(m_List->GetItemCount(), ConvertNumber(L"%X", itm.GetCardID()));

		m_List->SetItemText(nRow, 1, itm.GetCardName().c_str());
		m_List->SetItemText(nRow, 2, ConvertNumber(L"%d", itm.GetStarCount()));
		m_List->SetItemText(nRow, 3, ConvertNumber(L"%d/%d", itm.GetPower(), itm.GetDefence()));
		m_List->SetItemText(nRow, 4, itm.GetCardAttributeText().c_str());
		m_List->SetItemText(nRow, 5, itm.GetCardDetail().c_str());
	}
}

struct DeskCardContent
{
	int nIndex;
	DWORD dwCardId;
	std::wstring wsCardName;
};

void CGameDlg::OnBnClickedSetnextcard()
{
	/*CListCtrl * m_List = (CListCtrl *)GetDlgItem(IDC_LIST1);
	m_List->DeleteAllItems();
	int nColumnCount = m_List->GetHeaderCtrl()->GetItemCount();
	for (int i = 0; i < nColumnCount; i++)
	{
		m_List->DeleteColumn(0);
	}

	m_List->InsertColumn(0, L"ID", LVCFMT_LEFT, 80);
	m_List->InsertColumn(1, L"Name", LVCFMT_LEFT, 150);
	m_List->InsertColumn(2, L"Star", LVCFMT_LEFT, 30);
	m_List->InsertColumn(3, L"Power/Defence", LVCFMT_LEFT, 100);
	m_List->InsertColumn(4, L"Attribute", LVCFMT_LEFT, 50);
	m_List->InsertColumn(5, L"Detail", LVCFMT_LEFT, 400);

	std::vector<CCard> VecCard;
	CCardExtend::GetInstance().GetCurrentCardGroup(VecCard);*/

	/*CEdit* pEdit = reinterpret_cast<CEdit*>(GetDlgItem(IDC_EDIT_CARDNAME));
	CString strText;
	pEdit->GetWindowTextW(strText);

	if (strText.Trim() != L"")
	{
		VecCard.erase(std::remove_if(VecCard.begin(), VecCard.end(), [&strText](CONST CCard& Card){ return Card.GetCardName().find(strText.GetBuffer()) == -1; }), VecCard.end());
	}

	for (CONST auto& itm : VecCard)
	{
		int nRow = m_List->InsertItem(m_List->GetItemCount(), ConvertNumber(L"%X", itm.GetFullID()));

		m_List->SetItemText(nRow, 1, itm.GetCardName().c_str());
		m_List->SetItemText(nRow, 2, ConvertNumber(L"%d", itm.GetStarCount()));
		m_List->SetItemText(nRow, 3, ConvertNumber(L"%d/%d", itm.GetPower(), itm.GetDefence()));
		m_List->SetItemText(nRow, 4, itm.GetCardAttributeText().c_str());
		m_List->SetItemText(nRow, 5, itm.GetCardDetail().c_str());
	}*/

	CONST static std::vector<DeskCardContent> VecDeskCardContent =
	{
		{ 0, 0x1246, L"影身女妖的羽毛扫" },
		{ 1, 0x10F7, L"雷击" },
	};

	CComboBox* pCCB_NextCard = static_cast<CComboBox*>(GetDlgItem(IDC_CCB_NextCard));
	int nIndex = pCCB_NextCard->GetCurSel();
	if (nIndex == -1)
		return;

	if (nIndex >= VecDeskCardContent.size())
		return;

	CCardExtend::GetInstance().SetNextCard(VecDeskCardContent.at(nIndex).dwCardId);
}


void CGameDlg::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	*pResult = 0;
}


void CGameDlg::OnBnClickedButton1()
{
	CONST static std::vector<DeskCardContent> VecDeskCardContent = 
	{
		{ 0, 0x252A, L"科技属 戟炮手" },
		{ 1, 0x2E9B, L"圣珖神龙 星尘·零" },
		{ 2, 0x239F, L"红莲新星龙" },
		{ 3, 0x2FE2, L"究极幻神 奥特美特尔·比希巴尔金" },
		{ 4, 0x157E, L"五神龙" },
		{ 5, 0x24BA, L"极战机王 战神机人" },
		{ 6, 0x1CAE, L"混沌幻魔 阿米泰尔" },
		{ 7, 0x1CF3, L"于贝尔-极度悲伤的魔龙" },
		{ 8, 0x199A, L"星态龙" },
		{ 9, 0x2E68, L"合神龙 迪迈奥斯" },
		{ 10, 0x1BB4, L"邪神 抹灭者" },
		{ 11, 0x1905, L"邪神 恐惧之源" },
		{ 12, 0x255A, L"至尊太阳" },
		{ 13, 0x19A5, L"幻魔皇" },
		{ 14, 0x2C5E, L"隐藏的机壳杀手 物质主义" },
		{ 15, 0x16E4, L"混沌帝龙 -终焉的使者-" },
		{ 16, 0x2E6B, L"暴君爆风龙" },
		{ 17, 0x1B8F, L"龙骑士 黑魔术少女" },
		{ 18, 0x2E72, L"女神的圣弓" },
		{ 19, 0x2E6C, L"反射镜力龙" },
	};


	CComboBox* pCCB_DeskCard = static_cast<CComboBox*>(GetDlgItem(IDC_CCB_DeskCard));
	int nIndex = pCCB_DeskCard->GetCurSel();
	if (nIndex == -1)
		return;

	if (nIndex >= VecDeskCardContent.size())
		return;

	CCardExtend::GetInstance().SetCardToDesk(VecDeskCardContent.at(nIndex).dwCardId);
}


void CGameDlg::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (pNMListView->iItem != -1 && pNMListView->iSubItem != -1)
	{
		CListCtrl * m_list = (CListCtrl *)GetDlgItem(IDC_LIST1);
		CString str = m_list->GetItemText(pNMListView->iItem, pNMListView->iSubItem);
		MsgBoxLog(L"str=%s, Item=%d, iSub=%d", str.GetBuffer(), pNMListView->iItem, pNMListView->iSubItem);
		CEdit* pEdit = reinterpret_cast<CEdit*>(GetDlgItem(IDC_EDIT_CARDNAME));
		pEdit->SetWindowTextW(str.GetBuffer());
	}
	*pResult = 0;
}
