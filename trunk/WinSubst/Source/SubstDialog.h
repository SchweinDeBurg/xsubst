// WinSubst application.
// Copyright (c) 2004-2010 by Elijah Zarezky,
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

// SubstDialog.h - interface of the CSubstDialog class

#if !defined(__SubstDialog_h)
#define __SubstDialog_h

#if defined(_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif   // _MSC_VER

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

#endif   // __SubstDialog_h

// end of file
