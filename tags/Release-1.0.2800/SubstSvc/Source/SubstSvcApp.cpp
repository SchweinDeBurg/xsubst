// SubstSvc NT service.
// Copyright (c) 2004-2005 by Elijah Zarezky,
// All rights reserved.

// SubstSvcApp.cpp - implementation of the CSubstSvcApp class

#include "stdafx.h"
#include "SubstSvcApp.h"

#if defined(_DEBUG)
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif	// _DEBUG

// object model
IMPLEMENT_DYNAMIC(CSubstSvcApp, CWinApp)

CSubstSvcApp::CSubstSvcApp(void):
CWinApp(_T("SubstSvc"))
{
}

CSubstSvcApp::~CSubstSvcApp(void)
{
}

// end of file
