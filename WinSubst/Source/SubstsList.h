// WinSubst application.
// Copyright (c) 2004 by Elijah Zarezky,
// All rights reserved.

// SubstsList.h - interface of the CSubstsList class

#if !defined(__SubstsList_h)
#define __SubstsList_h

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
	enum {
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
