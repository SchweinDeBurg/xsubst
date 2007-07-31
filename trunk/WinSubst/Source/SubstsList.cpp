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

void CSubstsList::InsertItem(int iItem, LPCTSTR pszDrive, LPCTSTR pszPath)
{
	int iImage;
	SHFILEINFO shfi;

	ASSERT(0 <= iItem && iItem <= GetItemCount());
	ASSERT(AfxIsValidString(pszDrive));
	ASSERT(AfxIsValidString(pszPath));

	CString strTemp(pszDrive);
	strTemp += _T('\\');
	memset(&shfi, 0, sizeof(shfi));
	::SHGetFileInfo(strTemp, 0, &shfi, sizeof(shfi), SHGFI_ICON | SHGFI_SMALLICON);
	if (shfi.hIcon != NULL)
	{
		iImage = GetImageList(LVSIL_SMALL)->Add(shfi.hIcon);
		::DestroyIcon(shfi.hIcon);
	}
	else {
		// no system icon - so use default
		CMainDialog* pMainDlg = DYNAMIC_DOWNCAST(CMainDialog, GetParent());
		ASSERT_VALID(pMainDlg);
		iImage = pMainDlg->m_iDefIcon;
	}
	VERIFY(CSortingListCtrl::InsertItem(iItem, pszDrive, iImage) == iItem);
	SetItemText(iItem, I_PATH, pszPath);
}

void CSubstsList::InsertRegItems(void)
{
	CString strKeyName;
	LONG nResult;
	CRegKey regKey;
	TCHAR szDrive[_MAX_DRIVE];
	TCHAR szPath[_MAX_PATH];

	// build the name of the registry key
	strKeyName.LoadString(IDS_REGISTRY_KEY);
	strKeyName.Insert(0, _T(".DEFAULT\\Software\\"));
	strKeyName += _T("\\SubstSvc\\Drives");

	if ((nResult = regKey.Create(HKEY_USERS, strKeyName)) != ERROR_SUCCESS)
	{
		// failed to open needed key
		CWin32Error* pXcpt = new CWin32Error(nResult);
		throw pXcpt;
	}

	BOOL fHasValue = TRUE;
	for (int i = 0; fHasValue; ++i)
	{
		DWORD cchName = _MAX_DRIVE;
		DWORD fdwType = REG_NONE;
		nResult = ::RegEnumValue(regKey, i, szDrive, &cchName, NULL, &fdwType, NULL, NULL);
		if (nResult == ERROR_NO_MORE_ITEMS)
		{
			// no more values - so break the loop
			fHasValue = FALSE;
		}
		else if (nResult == ERROR_SUCCESS)
		{
			// only string values are interested
			if (fdwType == REG_SZ)
			{
				DWORD cchValue = _MAX_PATH;
				if (regKey.QueryValue(szPath, szDrive, &cchValue) == ERROR_SUCCESS)
				{
					InsertItem(0, szDrive, szPath);
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

	if ((nResult = regKey.Close()) != ERROR_SUCCESS)
	{
		// failed to close key
		CWin32Error* pXcpt = new CWin32Error(nResult);
		throw pXcpt;
	}
}

int CSubstsList::CompareItems(int iItemLhs, int iItemRhs)
{
	CString strTextLhs;
	CString strTextRhs;

	switch (m_iSortColumn)
	{
	case I_DRIVE:
	case I_PATH:
		strTextLhs = GetItemText(iItemLhs, m_iSortColumn);
		strTextRhs = GetItemText(iItemRhs, m_iSortColumn);
		return (::lstrcmp(strTextLhs, strTextRhs) * m_nSortOrder);
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
	try
	{
		// first invoke inherited dumper...
		CSortingListCtrl::Dump(dumpCtx);
		// ...and then dump own unique members
	}
	catch (CFileException* pXcpt)
	{
		pXcpt->ReportError();
		pXcpt->Delete();
	}
}

#endif	// _DEBUG

// end of file
