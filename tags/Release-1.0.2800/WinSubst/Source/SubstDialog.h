// WinSubst application.
// Copyright (c) 2004-2005 by Elijah Zarezky,
// All rights reserved.

// SubstDialog.h - interface of the CSubstDialog class

#if !defined(__SubstDialog_h)
#define __SubstDialog_h

#if defined(_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif	// _MSC_VER

class CSubstDialog: public CDialog
{
	DECLARE_DYNAMIC(CSubstDialog)
	DECLARE_MESSAGE_MAP()

// construction/destruction
public:
	CSubstDialog(CWnd* pParentWnd = NULL);
	virtual ~CSubstDialog(void);

// overridables
public:
	virtual BOOL OnInitDialog(void);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnOK(void);

// message map functions
protected:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT uCtlColor);
	afx_msg void OnEditPathChange(void);
	afx_msg void OnButtonBrowse(void);

// attributes
public:
	TCHAR m_szDrive[_MAX_DRIVE];
	CComboBox m_comboDrive;
	TCHAR m_szPath[_MAX_PATH];
	CEdit m_editPath;

// diagnostic services
#if defined(_DEBUG)
public:
	virtual void AssertValid(void) const;
	virtual void Dump(CDumpContext& dumpCtx) const;
#endif
};

#endif	// __SubstDialog_h

// end of file
