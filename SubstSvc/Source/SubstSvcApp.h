// SubstSvc NT service.
// Copyright (c) 2004 by Elijah Zarezky,
// All rights reserved.

// SubstSvcApp.h - interface of the CSubstSvcApp class

#if !defined(__SubstSvcApp_h)
#define __SubstSvcApp_h

class CSubstSvcApp: public CWinApp
{
	friend int _tmain(int argc, TCHAR* argv[], TCHAR* envp[]);
	DECLARE_DYNAMIC(CSubstSvcApp)

// construction/destruction
public:
	CSubstSvcApp(void);
	virtual ~CSubstSvcApp(void);
};

#endif	// __SubstSvcApp_h

// end of file
