// WinSubst application.
// Copyright (c) 2004 by Elijah Zarezky,
// All rights reserved.

// SubstsList.cpp - implementation of the CSubstsList class

#include "stdafx.h"
#include "SubstsList.h"
#include "Resource.h"
#include "MainDialog.h"

#if defined(_DEBUG)
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif	// _DEBUG

// object model
IMPLEMENT_DYNAMIC(CSubstsList, CSortingListCtrl)

// message map
BEGIN_MESSAGE_MAP(CSubstsList, CSortingListCtrl)
END_MESSAGE_MAP()

CSubstsList::CSubstsList(void)
{
}

CSubstsList::~CSubstsList(void)
{
}

void CSubstsList::InsertColumns(void)
{
	CRect rectClient;
	CString strHeading;

	int cxVScroll = ::GetSystemMetrics(SM_CXVSCROLL);
	GetClientRect(rectClient);
	int cxWidth = ((rectClient.Width() - cxVScroll) / NUM_COLUMNS);

	strHeading.LoadString(IDS_SUBST_DRIVE);
	InsertColumn(I_DRIVE, strHeading, LVCFMT_LEFT, cxWidth, I_DRIVE);
	strHeading.LoadString(IDS_SUBST_PATH);
	InsertColumn(I_PATH, strHeading, LVCFMT_LEFT, cxWidth, I_PATH);

	SetColumnWidth(I_DRIVE, LVSCW_AUTOSIZE_USEHEADER);
	SetColumnWidth(I_PATH, cxWidth + (cxWidth - GetColumnWidth(I_DRIVE)));
}

void CSubstsList::ListSubstitutions(void)
{
	CString strKeyName;
	LONG nResult;
	CRegKey regKey;
	TCHAR szDrive[_MAX_DRIVE];
	TCHAR szPath[_MAX_PATH];
	int iImage;
	SHFILEINFO shfi;

	strKeyName.LoadString(IDS_REGISTRY_KEY);
	strKeyName.Insert(0, _T(".DEFAULT\\Software\\"));
	strKeyName += _T("\\SubstSvc\\Drives");

	if ((nResult = regKey.Create(HKEY_USERS, strKeyName)) != ERROR_SUCCESS) {
		// failed to open needed key
		CWin32Error* pXcpt = new CWin32Error(nResult);
		throw pXcpt;
	}

	BOOL fHasValue = TRUE;
	for (int i = 0; fHasValue; ++i) {
		DWORD cchName = _MAX_DRIVE;
		DWORD fdwType = REG_NONE;
		nResult = ::RegEnumValue(regKey, i, szDrive, &cchName, NULL, &fdwType, NULL, NULL);
		if (nResult == ERROR_NO_MORE_ITEMS) {
			// no more values - so break the loop
			fHasValue = FALSE;
		}
		else if (nResult == ERROR_SUCCESS) {
			// only string values are interested
			if (fdwType == REG_SZ) {
				DWORD cchValue = _MAX_PATH;
				if (regKey.QueryValue(szPath, szDrive, &cchValue) == ERROR_SUCCESS) {
					// try to obtain associated icon from the system image list
					CString strTemp(szDrive);
					strTemp += _T('\\');
					memset(&shfi, 0, sizeof(shfi));
					::SHGetFileInfo(strTemp, 0, &shfi, sizeof(shfi), SHGFI_ICON | SHGFI_SMALLICON);
					if (shfi.hIcon != NULL) {
						iImage = GetImageList(LVSIL_SMALL)->Add(shfi.hIcon);
						::DestroyIcon(shfi.hIcon);
					}
					else {
						// no system icon - so use default
						CMainDialog* pMainDlg = DYNAMIC_DOWNCAST(CMainDialog, GetParent());
						ASSERT_VALID(pMainDlg);
						iImage = pMainDlg->m_iDefIcon;
					}
					VERIFY(InsertItem(0, szDrive, iImage) == 0);
					SetItemText(0, I_PATH, szPath);
				}
			}
		}
		else {
			// what the shit is that?!
			regKey.Close();
			CWin32Error* pXcpt = new CWin32Error(nResult);
			throw pXcpt;
		}
	}

	if ((nResult = regKey.Close()) != ERROR_SUCCESS) {
		// failed to close key
		CWin32Error* pXcpt = new CWin32Error(nResult);
		throw pXcpt;
	}
}

int CSubstsList::CompareItems(int /*iItemLhs*/, int /*iItemRhs*/)
{
	switch (m_iSortColumn)
	{
	case I_DRIVE:
		return (0);
	case I_PATH:
		return (0);
	default:
		return (0);
	}
}

#if defined(_DEBUG)

void CSubstsList::AssertValid(void) const
{
	// first perform inherited validity check...
	CSortingListCtrl::AssertValid();
	// ...and then verify our own state as well
}

void CSubstsList::Dump(CDumpContext& dumpCtx) const
{
	try {
		// first invoke inherited dumper...
		CSortingListCtrl::Dump(dumpCtx);
		// ...and then dump own unique members
	}
	catch (CFileException* pXcpt) {
		pXcpt->ReportError();
		pXcpt->Delete();
	}
}

#endif	// _DEBUG

// end of file
