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

// stdafx.h - include file for standard system include files

// initially generated by AfxScratch v1.0.2297 on 25.07.2004 at 13:26:11
// visit http://zarezky.spb.ru/projects/afx_scratch.html for more info

#if !defined(__stdafx_h)
#define __stdafx_h

#if defined(_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif	// _MSC_VER

// unreferenced inline/local function has been removed
#pragma warning(disable: 4514)
// function not inlined
#pragma warning(disable: 4710)
// identifier was truncated in the debug information
#pragma warning(disable: 4786)

#define WINVER 0x0500
#define _WIN32_WINNT 0x0500

#if (_MSC_VER >= 1300)
#define _ATL_DISABLE_DEPRECATED
#endif	// _MSC_VER

// MFC headers
#include <afxwin.h>			// MFC core and standard components
#include <afxcmn.h>			// MFC common control classes

// ATL headers
#include <atlbase.h>

// PSDK headers
#include <shlobj.h>
#include <shlwapi.h>			// light-weight utility APIs

// additional headers
#include "../../SubstSvc/Source/ServiceInterop.h"
#include "../../../Repository/AfxGadgets/Source/SortingListCtrl.h"
#include "../../../Repository/AfxGadgets/Source/Win32Error.h"
#include "../../../Repository/CodeProject/Source/FolderDlg.h"

// force ISO/IEC 14882 conformance in for loop scope
#if (_MSC_VER < 1300)
#define for if (false); else for
#else
#pragma conform(forScope, on)
#endif	// _MSC_VER

#endif	// __stdafx_h

// end of file
