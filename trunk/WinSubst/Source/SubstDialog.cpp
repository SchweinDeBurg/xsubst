// WinSubst application.
// Copyright (c) 2004-2005 by Elijah Zarezky,
// All rights reserved.

// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// SubstDialog.cpp - implementation of the CSubstDialog class

#include "stdafx.h"
#include "SubstDialog.h"
#include "Resource.h"

#if defined(_DEBUG)
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif	// _DEBUG

// object model
IMPLEMENT_DYNAMIC(CSubstDialog, CDialog)

// message map
BEGIN_MESSAGE_MAP(CSubstDialog, CDialog)
	ON_WM_CTLCOLOR()
	ON_EN_CHANGE(IDC_EDIT_PATH, OnEditPathChange)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
END_MESSAGE_MAP()

CSubstDialog::CSubstDialog(CWnd* pParentWnd):
CDialog(IDD_SUBST, pParentWnd)
{
	memset(m_szDrive, 0, sizeof(m_szDrive));
	memset(m_szPath, 0, sizeof(m_szPath));
}

CSubstDialog::~CSubstDialog(void)
{
}

BOOL CSubstDialog::OnInitDialog(void)
{
	CString strCaption;

	CDialog::OnInitDialog();

	if (m_szDrive[0] != 0)
	{
		// change selected substitution
		strCaption.LoadString(IDS_CHANGE_SUBST);
		SetWindowText(strCaption);
		m_comboDrive.AddString(m_szDrive);
		m_comboDrive.SetCurSel(0);
		m_comboDrive.EnableWindow(FALSE);
		m_editPath.SetWindowText(m_szPath);
	}
	else {
		// create new substitution
		strCaption.LoadString(IDS_NEW_SUBST);
		SetWindowText(strCaption);
		TCHAR szTemp[_MAX_DRIVE] = _T("#:");
		DWORD dwDrives = ::GetLogicalDrives();
		for (int i = 0; i <= _T('Z') - _T('A'); i++)
		{
			if ((dwDrives & (1 << i)) == 0)
			{
				szTemp[0] = static_cast<TCHAR>(_T('A') + i);
				m_comboDrive.AddString(szTemp);
			}
		}
		if (m_comboDrive.GetCount() > 0)
		{
			// found free drive letter(s)
			m_comboDrive.SetCurSel(0);
			GetDlgItem(IDOK)->EnableWindow(FALSE);
		}
		else {
			// no free drive letters found
			AfxMessageBox(IDS_NO_FREE_DRIVES, MB_ICONWARNING | MB_OK);
			EndDialog(IDABORT);
		}
	}

	// initialized
	return (TRUE);
}

void CSubstDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_COMBO_DRIVE, m_comboDrive);
	DDX_Control(pDX, IDC_EDIT_PATH, m_editPath);
}

void CSubstDialog::OnOK(void)
{
	m_comboDrive.GetLBText(m_comboDrive.GetCurSel(), m_szDrive);
	m_editPath.GetWindowText(m_szPath, _MAX_PATH);
	CDialog::OnOK();
}

HBRUSH CSubstDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT uCtlColor)
{
	HBRUSH hbr;

	switch (pWnd->GetDlgCtrlID())
	{
	case IDC_EDIT_PATH:
		if (uCtlColor == CTLCOLOR_STATIC)
		{
			pDC->SetBkColor(::GetSysColor(COLOR_WINDOW));
			pDC->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
			hbr = ::GetSysColorBrush(COLOR_WINDOW);
			break;
		}
	default:
		hbr = CDialog::OnCtlColor(pDC, pWnd, uCtlColor);
	}
	return (hbr);
}

void CSubstDialog::OnEditPathChange(void)
{
	GetDlgItem(IDOK)->EnableWindow(m_editPath.GetWindowTextLength() > 0);
}

void CSubstDialog::OnButtonBrowse(void)
{
	CString strPrompt;
	CString strPath;

	strPrompt.LoadString(IDS_CHOOSE_PATH);
	m_editPath.GetWindowText(strPath);
	CFolderDialog dlgFolder(strPrompt, strPath, this);
	if (dlgFolder.DoModal() == IDOK)
	{
		m_editPath.SetWindowText(dlgFolder.GetFolderPath());
	}
}

#if defined(_DEBUG)

void CSubstDialog::AssertValid(void) const
{
	// first perform inherited validity check...
	CDialog::AssertValid();
	// ...and then verify our own state as well
	ASSERT_VALID(&m_comboDrive);
	ASSERT_VALID(&m_editPath);
}

void CSubstDialog::Dump(CDumpContext& dumpCtx) const
{
	try
	{
		// first invoke inherited dumper...
		CDialog::Dump(dumpCtx);
		// ...and then dump own unique members
		dumpCtx << "m_szDrive = " << m_szDrive;
		dumpCtx << "\nm_comboDrive = " << m_comboDrive;
		dumpCtx << "\nm_szPath = " << m_szPath;
		dumpCtx << "\nm_editPath = " << m_editPath;
	}
	catch (CFileException* pXcpt)
	{
		pXcpt->ReportError();
		pXcpt->Delete();
	}
}

#endif	// _DEBUG

// end of file
