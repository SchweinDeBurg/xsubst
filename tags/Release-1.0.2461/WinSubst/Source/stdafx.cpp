// WinSubst application.
// Copyright (c) 2004 by Elijah Zarezky,
// All rights reserved.

// stdafx.cpp - source file that includes just the standard includes

// initially generated by AfxScratch v1.0.2297 on 25.07.2004 at 13:26:11
// visit http://home.peterstar.ru/zarezky/projects/afx_scratch.html for more info

#include "stdafx.h"

#if defined(_DEBUG)
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif	// _DEBUG

// import libraries
#pragma comment(lib, "version.lib")
#pragma comment(lib, "shlwapi.lib")

// linker options
#pragma comment(linker, "/ignore:4089")		// all references to DLL discarded by /opt:ref
#pragma comment(linker, "/opt:ref")
#pragma comment(linker, "/opt:nowin98")

// entry-point symbol for the Unicode builds
#if defined(UNICODE) || defined(_UNICODE)
#pragma comment(linker, "/entry:wWinMainCRTStartup")
#endif	// UNICODE

// end of file