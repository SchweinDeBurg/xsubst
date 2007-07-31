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

// SubstsList.h - interface of the CSubstsList class

#if !defined(__SubstsList_h)
#define __SubstsList_h

#if defined(_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif	// _MSC_VER

class CSubstsList: public CSortingListCtrl
{
	DECLARE_DYNAMIC(CSubstsList)
	DECLARE_MESSAGE_MAP()

// construction/destruction
public:
	CSubstsList(void);
	virtual ~CSubstsList(void);

// operations
public:
	void InsertColumns(void);
	void InsertItem(int iItem, LPCTSTR pszDrive, LPCTSTR pszPath);
	void InsertRegItems(void);

// overridables
protected:
	virtual int CompareItems(int iItemLhs, int iItemRhs);

// attributes
public:
	enum
	{
		// column indices
		I_DRIVE,
		I_PATH,
		NUM_COLUMNS		// should be the LAST enumerator!
	};

// diagnostic services
#if defined(_DEBUG)
public:
	virtual void AssertValid(void) const;
	virtual void Dump(CDumpContext& dumpCtx) const;
#endif
};

#endif	// __SubstsList_h

// end of file
