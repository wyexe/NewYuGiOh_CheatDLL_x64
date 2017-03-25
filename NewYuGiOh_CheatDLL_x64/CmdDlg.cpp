// CmdDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NewYuGiOh_CheatDLL_x64.h"
#include "CmdDlg.h"
#include "afxdialogex.h"
#include <vector>
#include <algorithm>
#include "FormLog.h"
#include "Expr.h"
// CCmdDlg dialog

IMPLEMENT_DYNAMIC(CCmdDlg, CDialogEx)

CCmdDlg::CCmdDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG2, pParent)
{

}

CCmdDlg::~CCmdDlg()
{
}

BOOL CCmdDlg::OnInitDialog()
{
	auto Loger = std::bind(&CCmdDlg::AddTextToEdit, this, std::placeholders::_1);
	CFormLog::GetInstance().SetLoger(Loger);
	return TRUE;
}

void CCmdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCmdDlg, CDialogEx)
END_MESSAGE_MAP()


// CCmdDlg message handlers


void CCmdDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	//CDialogEx::OnOK();
}


BOOL CCmdDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		CWnd* pWndComboBox = this->GetDlgItem(IDC_COMBO_CMD);
		CWnd* pWndFocus = this->GetFocus();

		if (pWndFocus != nullptr && pWndFocus->GetParent() != nullptr && pWndComboBox != nullptr && pWndFocus->GetParent()->GetSafeHwnd() == pWndComboBox->GetSafeHwnd())
		{
			CComboBox* pComboBox = static_cast<CComboBox *>(pWndComboBox);
			CStringW CmdText;
			pComboBox->GetWindowTextW(CmdText);
			if (CmdText.Trim().IsEmpty())
				return TRUE;

			if (!CExpr::GetInstance().Run(CmdText.GetBuffer()))
			{
				CFormLog::GetInstance().Print(L"UnExist Cmd '%s'", CmdText.GetBuffer());
				return TRUE;
			}

			// Save Old Data in ComboBox
			std::vector<std::wstring> VecText;
			for (INT i = 0; i < pComboBox->GetCount(); ++i)
			{
				CStringW ComboBoxText;
				pComboBox->GetLBText(i, ComboBoxText);
				VecText.push_back(ComboBoxText.GetBuffer());
			}

			// Clear ComboBox
			while (pComboBox->DeleteString(0) >= 0);

			// Add New Data
			auto itr = std::find_if(VecText.begin(), VecText.end(), [&CmdText](CONST std::wstring& wsText) { return wsText == std::wstring(CmdText.GetBuffer()); });
			if (itr == VecText.end())
				VecText.insert(VecText.begin(), CmdText.GetBuffer());


			// Put the Data to ComboBox
			for (CONST auto& itm : VecText)
			{
				pComboBox->InsertString(pComboBox->GetCount(), itm.c_str());
			}

			pComboBox->SetCurSel(-1);
			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

VOID CCmdDlg::AddTextToEdit(_In_ CONST std::wstring& Text) CONST
{
	CEdit* pEdit = static_cast<CEdit*>(this->GetDlgItem(IDC_EDIT_LOG));

	CStringW EditText;
	pEdit->GetWindowTextW(EditText);
	EditText += Text.c_str();
	EditText += "\r\n";
	pEdit->SetWindowTextW(EditText);
	pEdit->LineScroll(pEdit->GetLineCount());
}
