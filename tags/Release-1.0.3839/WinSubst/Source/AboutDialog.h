// WinSubst application.
// Copyright (c) 2004-2007 by Elijah Zarezky,
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
