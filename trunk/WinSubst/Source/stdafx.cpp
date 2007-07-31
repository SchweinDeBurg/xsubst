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

// stdafx.cpp - source file that includes just the standard includes

// initially generated by AfxScratch v1.0.2297 on 25.07.2004 at 13:26:11
// visit http://zarezky.spb.ru/projects/afx_scratch.html for more info

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
#if (_MSC_VER < 1300)
#pragma comment(linker, "/ignore:4089")		// all references to DLL discarded by /opt:ref
#pragma comment(linker, "/opt:ref")
#pragma comment(linker, "/opt:nowin98")
#endif	// _MSC_VER

// entry-point symbol for the Unicode builds
#if defined(UNICODE) || defined(_UNICODE)
#pragma comment(linker, "/entry:wWinMainCRTStartup")
#endif	// UNICODE

// end of file
