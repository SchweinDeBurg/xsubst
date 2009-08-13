// WinSubst application.
// Copyright (c) 2004-2005 by Elijah Zarezky,
// All rights reserved.

// AboutDialog.h - interface of the CAboutDialog class

#if !defined(__AboutDialog_h)
#define __AboutDialog_h

#if defined(_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif	// _MSC_VER

class CAboutDialog: public CDialog
{
	DECLARE_DYNAMIC(CAboutDialog)
	DECLARE_MESSAGE_MAP()

// construction/destruction
public:
	CAboutDialog(CWnd* pParentWnd = NULL);
	virtual ~CAboutDialog(void);

// overridables
public:
	virtual BOOL OnInitDialog(void);

// diagnostic services
#if defined(_DEBUG)
public:
	virtual void AssertValid(void) const;
	virtual void Dump(CDumpContext& dumpCtx) const;
#endif
};

#endif	// __AboutDialog_h

// end of file