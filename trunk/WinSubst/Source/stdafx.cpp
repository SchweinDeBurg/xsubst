// WinSubst application.
// Copyright (c) 2004 by Elijah Zarezky,
// All rights reserved.

// stdafx.cpp - source file that includes just the standard includes
// initially generated by AfxScratch 1.0.2297 on 25.07.2004 at 13:26:11

#include "stdafx.h"

#if defined(_DEBUG)
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif	// _DEBUG

// linker options
#pragma comment(linker, "/ignore:4089")		// all references to DLL discarded by /opt:ref

// entry-point symbol for the Unicode builds
#if defined(UNICODE) || defined(_UNICODE)
#pragma comment(linker, "/entry:wWinMainCRTStartup")
#endif	// UNICODE

// end of file
