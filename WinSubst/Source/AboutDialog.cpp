// WinSubst application.
// Copyright (c) 2004 by Elijah Zarezky,
// All rights reserved.

// AboutDialog.cpp - implementation of the CAboutDialog class

#include "stdafx.h"
#include "AboutDialog.h"
#include "Resource.h"
#include "WinSubstApp.h"

#if defined(_DEBUG)
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif	// _DEBUG

// object model
IMPLEMENT_DYNAMIC(CAboutDialog, CDialog)

// message map
BEGIN_MESSAGE_MAP(CAboutDialog, CDialog)
END_MESSAGE_MAP()

CAboutDialog::CAboutDialog(CWnd* pParentWnd):
CDialog(IDD_ABOUT, pParentWnd)
{
}

CAboutDialog::~CAboutDialog(void)
{
}

BOOL CAboutDialog::OnInitDialog(void)
{
	CString strAppVer;
	CString strTitle;

	CDialog::OnInitDialog();

	// obtain and show version info
	CWinSubstApp* pApp = DYNAMIC_DOWNCAST(CWinSubstApp, AfxGetApp());
	ASSERT_VALID(pApp);
	pApp->GetVersionString(strAppVer);
	strTitle.Format(IDS_TITLE_FORMAT, static_cast<LPCTSTR>(strAppVer));
	SetDlgItemText(IDC_STATIC_TITLE, strTitle);

	// initialized
	return (TRUE);
}

#if defined(_DEBUG)

void CAboutDialog::AssertValid(void) const
{
	// first perform inherited validity check...
	CDialog::AssertValid();
	// ...and then verify our own state as well
}

void CAboutDialog::Dump(CDumpContext& dumpCtx) const
{
	try {
		// first invoke inherited dumper...
		CDialog::Dump(dumpCtx);
		// ...and then dump own unique members
	}
	catch (CFileException* pXcpt) {
		pXcpt->ReportError();
		pXcpt->Delete();
	}
}

#endif	// _DEBUG

// end of file
