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

// MainDialog.cpp - implementation of the CMainDialog class

// initially generated by AfxScratch v1.0.2297 on 25.07.2004 at 13:26:11
// visit http://zarezky.spb.ru/projects/afx_scratch.html for more info

#include "stdafx.h"
#include "SubstsList.h"
#include "MainDialog.h"
#include "Resource.h"
#include "WinSubstApp.h"
#include "AboutDialog.h"
#include "SubstDialog.h"

#if defined(_DEBUG)
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif	// _DEBUG

// object model
IMPLEMENT_DYNAMIC(CMainDialog, CDialog)

// message map
BEGIN_MESSAGE_MAP(CMainDialog, CDialog)
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR()
	ON_WM_SYSCOMMAND()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_SUBSTS, OnSubstsListItemChanged)
	ON_BN_CLICKED(IDC_BUTTON_NEW_SUBST, OnButtonNewSubst)
	ON_BN_CLICKED(IDC_BUTTON_CHANGE_SUBST, OnButtonChangeSubst)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_SUBST, OnButtonDeleteSubst)
	ON_BN_CLICKED(IDC_BUTTON_EXIT, OnButtonExit)
END_MESSAGE_MAP()

CMainDialog::CMainDialog(CWnd* pParentWnd):
CDialog(IDD_MAIN, pParentWnd),
m_hIcon(NULL),
m_hSmIcon(NULL),
m_iDefIcon(-1),
m_dwSvcState(0)
{
	CWinSubstApp* pApp = DYNAMIC_DOWNCAST(CWinSubstApp, AfxGetApp());
	ASSERT_VALID(pApp);

	// load dialog's icons
	m_hIcon = pApp->LoadIcon(IDI_APP_ICON);
	m_hSmIcon = pApp->LoadSmIcon(MAKEINTRESOURCE(IDI_APP_ICON));

	// prepare image list
	int cxSmIcon = ::GetSystemMetrics(SM_CXSMICON);
	int cySmIcon = ::GetSystemMetrics(SM_CYSMICON);
	m_imageList.Create(cxSmIcon, cySmIcon, ILC_COLOR16 | ILC_MASK, 8, 8);
	HICON hIcon = pApp->LoadSmIcon(MAKEINTRESOURCE(IDI_DRIVE_ICON));
	m_iDefIcon = m_imageList.Add(hIcon);
	::DestroyIcon(hIcon);
}

CMainDialog::~CMainDialog(void)
{
	m_imageList.DeleteImageList();
	::DestroyIcon(m_hSmIcon);
	::DestroyIcon(m_hIcon);
}

BOOL CMainDialog::OnInitDialog(void)
{
	CString strAbout;
	SERVICE_STATUS ss;
	CString strMessage;
	CString strState;

	CDialog::OnInitDialog();

	// set dialog's icons
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hSmIcon, FALSE);

	// adjust system menu
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	ASSERT_VALID(pSysMenu);
	pSysMenu->AppendMenu(MF_BYPOSITION | MF_SEPARATOR);
	strAbout.LoadString(IDS_ABOUT);
	pSysMenu->AppendMenu(MF_BYPOSITION, IDM_SC_ABOUT, strAbout);

	// prepare current substitutions
	DWORD fdwExStyle = m_listSubsts.GetExtendedStyle();
	m_listSubsts.SetExtendedStyle(fdwExStyle | LVS_EX_FULLROWSELECT);
	m_listSubsts.SetImageList(&m_imageList, LVSIL_SMALL);
	m_listSubsts.InsertColumns();
	try
	{
		m_listSubsts.InsertRegItems();
	}
	catch (CWin32Error* pXcpt)
	{
		pXcpt->ReportError(MB_ICONSTOP | MB_OK);
		delete pXcpt;
	}

	// adjust controls state
	if (m_listSubsts.GetItemCount() > 0)
	{
		m_listSubsts.SortItems(CSubstsList::I_DRIVE, CSubstsList::SORT_ASCENDING);
		m_listSubsts.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, 0xFFFFFFFF);
	}
	else {
		GetDlgItem(IDC_BUTTON_CHANGE_SUBST)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_DELETE_SUBST)->EnableWindow(FALSE);
	}

	// verify service presence and state
	SC_HANDLE schManager = ::OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
	if (schManager != NULL)
	{
		SC_HANDLE schService = ::OpenService(schManager, SZ_SERVICE_NAME, SERVICE_QUERY_STATUS);
		if (schService != NULL)
		{
			memset(&ss, 0, sizeof(ss));
			::QueryServiceStatus(schService, &ss);
			m_dwSvcState = ss.dwCurrentState;
			GetSvcStateText(strState);
			GetDlgItem(IDC_TEXT_STATUS)->SetWindowText(strState);
			::CloseServiceHandle(schService);
		}
		else {
			DWORD dwErrCode = ::GetLastError();
			if (dwErrCode == ERROR_SERVICE_DOES_NOT_EXIST)
			{
				// probably service doesn't installed
				strMessage.Format(IDS_SERVICE_DOES_NOT_EXIST, SZ_SERVICE_NAME);
				AfxMessageBox(strMessage, MB_ICONSTOP | MB_OK);
				EndDialog(IDABORT);
			}
			else {
				CWin32Error xcpt(dwErrCode);
				xcpt.ReportError(MB_ICONWARNING | MB_OK);
			}
		}
		::CloseServiceHandle(schManager);
	}

	// initialized
	return (TRUE);
}

void CMainDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST_SUBSTS, m_listSubsts);
}

void CMainDialog::OnOK(void)
{
	// don't invoke inherited handler to avoid dialog closing
}

void CMainDialog::OnCancel(void)
{
	// don't invoke inherited handler to avoid dialog closing
}

void CMainDialog::OnClose(void)
{
	EndDialog(IDCANCEL);
}

HBRUSH CMainDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT uCtlColor)
{
	COLORREF crText;

	if (pWnd->GetDlgCtrlID() == IDC_TEXT_STATUS)
	{
		pDC->SetBkColor(::GetSysColor(COLOR_3DFACE));
		switch (m_dwSvcState)
		{
		case SERVICE_RUNNING:
			crText = RGB(0, 128, 0);
			break;
		case SERVICE_STOPPED:
		case SERVICE_START_PENDING:
		case SERVICE_STOP_PENDING:
		case SERVICE_CONTINUE_PENDING:
		case SERVICE_PAUSE_PENDING:
		case SERVICE_PAUSED:
			crText = RGB(128, 0, 0);
			break;
		default:
			crText = ::GetSysColor(COLOR_WINDOWTEXT);
		}
		pDC->SetTextColor(crText);
		return (::GetSysColorBrush(COLOR_3DFACE));
	}
	else {
		return (CDialog::OnCtlColor(pDC, pWnd, uCtlColor));
	}
}

void CMainDialog::OnSysCommand(UINT uID, LPARAM lParam)
{
	if ((uID & 0xFFF0) == IDM_SC_ABOUT)
	{
		CAboutDialog dlgAbout;
		dlgAbout.DoModal();
	}
	else {
		CDialog::OnSysCommand(uID, lParam);
	}
}

void CMainDialog::OnSubstsListItemChanged(NMHDR* /*pHdr*/, LRESULT* pnResult)
{
	BOOL fEnable = m_listSubsts.GetSelectedCount() > 0;
	GetDlgItem(IDC_BUTTON_CHANGE_SUBST)->EnableWindow(fEnable);
	GetDlgItem(IDC_BUTTON_DELETE_SUBST)->EnableWindow(fEnable);
	*pnResult = 0;
}

void CMainDialog::OnButtonNewSubst(void)
{
	CString strKeyName;
	LONG nResult;
	CRegKey regKey;

	CSubstDialog dlgSubst(this);
	if (dlgSubst.DoModal() == IDOK)
	{
		ASSERT(::lstrlen(dlgSubst.m_szDrive) > 0);
		ASSERT(::lstrlen(dlgSubst.m_szPath) > 0);
		// build the name of the registry key...
		strKeyName.LoadString(IDS_REGISTRY_KEY);
		strKeyName.Insert(0, _T(".DEFAULT\\Software\\"));
		strKeyName += _T("\\SubstSvc\\Drives");
		// ...and then open this key
		regKey.Create(HKEY_USERS, strKeyName);
		CString strPlusPath(dlgSubst.m_szPath);
		strPlusPath.Insert(0, _T('+'));
		if ((nResult = regKey.SetValue(strPlusPath, dlgSubst.m_szDrive)) == ERROR_SUCCESS)
		{
			// kick the service...
			ControlService(SUBSTSVC_SERVICE_CONTROL_MODIFY_DRIVES);
			// ...and then update the list
			int iItem = m_listSubsts.GetItemCount();
			m_listSubsts.InsertItem(iItem, dlgSubst.m_szDrive, dlgSubst.m_szPath);
			m_listSubsts.EnsureVisible(iItem, FALSE);
			m_listSubsts.SetItemState(iItem, LVIS_SELECTED | LVIS_FOCUSED, 0xFFFFFFFF);
			m_listSubsts.SetFocus();
		}
		else {
			// unable to update system registry
			CWin32Error* pXcpt = new CWin32Error(nResult);
			throw pXcpt;
		}
		regKey.Close();
	}
}

void CMainDialog::OnButtonChangeSubst(void)
{
	CString strKeyName;
	LONG nResult;
	CRegKey regKey;

	CSubstDialog dlgSubst(this);
	POSITION pos = m_listSubsts.GetFirstSelectedItemPosition();
	ASSERT(pos != NULL);
	int iItem = m_listSubsts.GetNextSelectedItem(pos);
	m_listSubsts.GetItemText(iItem, CSubstsList::I_DRIVE, dlgSubst.m_szDrive, _MAX_DRIVE);
	m_listSubsts.GetItemText(iItem, CSubstsList::I_PATH, dlgSubst.m_szPath, _MAX_PATH);
	if (dlgSubst.DoModal() == IDOK)
	{
		ASSERT(::lstrlen(dlgSubst.m_szPath) > 0);
		if (m_listSubsts.GetItemText(iItem, CSubstsList::I_PATH) != dlgSubst.m_szPath)
		{
			// build the name of the registry key...
			strKeyName.LoadString(IDS_REGISTRY_KEY);
			strKeyName.Insert(0, _T(".DEFAULT\\Software\\"));
			strKeyName += _T("\\SubstSvc\\Drives");
			// ...and then open this key
			regKey.Create(HKEY_USERS, strKeyName);
			CString strAsteriskPath(dlgSubst.m_szPath);
			strAsteriskPath.Insert(0, _T('*'));
			if ((nResult = regKey.SetValue(strAsteriskPath, dlgSubst.m_szDrive)) == ERROR_SUCCESS)
			{
				// kick the service...
				ControlService(SUBSTSVC_SERVICE_CONTROL_MODIFY_DRIVES);
				// ...and then update the list
				m_listSubsts.SetItemText(iItem, CSubstsList::I_PATH, dlgSubst.m_szPath);
				m_listSubsts.EnsureVisible(iItem, FALSE);
				m_listSubsts.SetItemState(iItem, LVIS_SELECTED | LVIS_FOCUSED, 0xFFFFFFFF);
				m_listSubsts.SetFocus();
				if (m_listSubsts.GetItemCount() == 1)
				{
					// first item inserted
					GetDlgItem(IDC_BUTTON_CHANGE_SUBST)->EnableWindow();
					GetDlgItem(IDC_BUTTON_DELETE_SUBST)->EnableWindow();
				}
			}
			else {
				// unable to update system registry
				CWin32Error* pXcpt = new CWin32Error(nResult);
				throw pXcpt;
			}
			regKey.Close();
		}
	}
}

void CMainDialog::OnButtonDeleteSubst(void)
{
	CString strKeyName;
	LONG nResult;
	CRegKey regKey;

	if (AfxMessageBox(IDS_DELETE_CONFIRM, MB_ICONQUESTION | MB_YESNO) == IDYES)
	{
		// obtain selected substitution
		POSITION pos = m_listSubsts.GetFirstSelectedItemPosition();
		ASSERT(pos != NULL);
		int iItem = m_listSubsts.GetNextSelectedItem(pos);
		// build the name of the registry key...
		strKeyName.LoadString(IDS_REGISTRY_KEY);
		strKeyName.Insert(0, _T(".DEFAULT\\Software\\"));
		strKeyName += _T("\\SubstSvc\\Drives");
		// ...and then open this key
		regKey.Create(HKEY_USERS, strKeyName);
		CString strDrive = m_listSubsts.GetItemText(iItem, CSubstsList::I_DRIVE);
		CString strMinusPath = m_listSubsts.GetItemText(iItem, CSubstsList::I_PATH);
		strMinusPath.Insert(0, _T('-'));
		if ((nResult = regKey.SetValue(strMinusPath, strDrive)) == ERROR_SUCCESS) {
			// kick the service...
			ControlService(SUBSTSVC_SERVICE_CONTROL_MODIFY_DRIVES);
			// ...and then update the list
			m_listSubsts.DeleteItem(iItem);
			int cItems = m_listSubsts.GetItemCount();
			if (cItems > 0)
			{
				// adjust selected item
				if (iItem == cItems)
				{
					--iItem;
				}
				m_listSubsts.SetItemState(iItem, LVIS_SELECTED | LVIS_FOCUSED, 0xFFFFFFFF);
			}
			else {
				// no more substitutions
				SetDefID(IDC_BUTTON_NEW_SUBST);
				GetDlgItem(IDC_BUTTON_CHANGE_SUBST)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_DELETE_SUBST)->EnableWindow(FALSE);
			}
			m_listSubsts.SetFocus();
		}
		else {
			// unable to update system registry
			CWin32Error* pXcpt = new CWin32Error(nResult);
			throw pXcpt;
		}
		regKey.Close();
	}
}

void CMainDialog::OnButtonExit(void)
{
	EndDialog(IDCANCEL);
}

void CMainDialog::GetSvcStateText(CString& strDest)
{
	UINT idsFormat;

	switch (m_dwSvcState)
	{
	case SERVICE_STOPPED:
		idsFormat = IDS_SERVICE_STOPPED;
		break;
	case SERVICE_START_PENDING:
		idsFormat = IDS_SERVICE_START_PENDING;
		break;
	case SERVICE_STOP_PENDING:
		idsFormat = IDS_SERVICE_STOP_PENDING;
		break;
	case SERVICE_RUNNING:
		idsFormat = IDS_SERVICE_RUNNING;
		break;
	case SERVICE_CONTINUE_PENDING:
		idsFormat = IDS_SERVICE_CONTINUE_PENDING;
		break;
	case SERVICE_PAUSE_PENDING:
		idsFormat = IDS_SERVICE_PAUSE_PENDING;
		break;
	case SERVICE_PAUSED:
		idsFormat = IDS_SERVICE_PAUSED;
		break;
	default:
		idsFormat = IDS_SERVICE_UNKNOWN_STATE;
	}
	strDest.Format(idsFormat, SZ_SERVICE_NAME);
}

void CMainDialog::ControlService(DWORD dwCode)
{
	SERVICE_STATUS ss;
	CString strState;

	SC_HANDLE schManager = ::OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
	enum { fdwAccess = SERVICE_QUERY_STATUS | SERVICE_USER_DEFINED_CONTROL };
	SC_HANDLE schService = ::OpenService(schManager, SZ_SERVICE_NAME, fdwAccess);
	memset(&ss, 0, sizeof(ss));
	::QueryServiceStatus(schService, &ss);
	m_dwSvcState = ss.dwCurrentState;
	GetSvcStateText(strState);
	GetDlgItem(IDC_TEXT_STATUS)->SetWindowText(strState);
	if (m_dwSvcState == SERVICE_RUNNING)
	{
		BeginWaitCursor();
		HANDLE hEvent = ::CreateEvent(NULL, TRUE, FALSE, SZ_SYNC_EVENT_NAME);
		::ControlService(schService, dwCode, &ss);
		::WaitForSingleObject(hEvent, INFINITE);
		::ResetEvent(hEvent);
		::CloseHandle(hEvent);
		EndWaitCursor();
	}
	::CloseServiceHandle(schService);
	::CloseServiceHandle(schManager);
}

#if defined(_DEBUG)

void CMainDialog::AssertValid(void) const
{
	// first perform inherited validity check...
	CDialog::AssertValid();
	// ...and then verify our own state as well
	ASSERT_VALID(&m_imageList);
	ASSERT_VALID(&m_listSubsts);
}

void CMainDialog::Dump(CDumpContext& dumpCtx) const
{
	try
	{
		// first invoke inherited dumper...
		CDialog::Dump(dumpCtx);
		// ...and then dump own unique members
		dumpCtx << "m_hIcon = " << m_hIcon;
		dumpCtx << "\nm_hSmIcon = " << m_hSmIcon;
		dumpCtx << "\nm_imageList = " << m_imageList;
		dumpCtx << "\nm_iDefIcon = " << m_iDefIcon;
		dumpCtx << "\nm_listSubsts = " << m_listSubsts;
		dumpCtx << "\nm_dwSvcState = " << m_dwSvcState;
	}
	catch (CFileException* pXcpt)
	{
		pXcpt->ReportError();
		pXcpt->Delete();
	}
}

#endif	// _DEBUG

// end of file
