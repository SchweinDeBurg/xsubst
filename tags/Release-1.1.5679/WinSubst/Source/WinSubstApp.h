// WinSubst application.
// Copyright (c) 2004-2011 by Elijah Zarezky,
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

// WinSubstApp.h - interface of the CWinSubstApp class

// initially generated by AfxScratch v1.0.2297 on 25.07.2004 at 13:26:11
// visit http://zarezky.spb.ru/projects/afx_scratch.html for more info

#if !defined(__WinSubstApp_h)
#define __WinSubstApp_h

#if defined(_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif   // _MSC_VER

class CWinSubstApp: public CWinApp
{
	DECLARE_DYNAMIC(CWinSubstApp)
	DECLARE_MESSAGE_MAP()

// construction/destruction
public:
	CWinSubstApp(void);
	virtual ~CWinSubstApp(void);

// operations
public:
	HICON LoadSmIcon(LPCTSTR pszResName);
	void GetVersionString(CString& strDest);

// overridables
public:
	virtual BOOL InitInstance(void);

// attributes
public:
	HANDLE m_hMutexAppInst;

// implementation helpers
private:
#if defined(WINSUBST_DETOURED)
	// LoadLibrary[A/W] catcher
	typedef HMODULE(WINAPI* PFN_LOAD_LIBRARY)(LPCTSTR);
	static PFN_LOAD_LIBRARY m_pfnLoadLibrary;
	static HMODULE WINAPI LoadLibrary(LPCTSTR pszFileName);
	// LoadLibraryEx[A/W] catcher
	typedef HMODULE(WINAPI* PFN_LOAD_LIBRARY_EX)(LPCTSTR, HANDLE, DWORD);
	static PFN_LOAD_LIBRARY_EX m_pfnLoadLibraryEx;
	static HMODULE WINAPI LoadLibraryEx(LPCTSTR pszFileName, HANDLE hFile, DWORD fdwFlags);
	// catched DLLs
	CMap<CString, LPCTSTR, DWORD, DWORD> m_mapCatchpit;
	INT_PTR RegQueryCatchpit(void);
	bool IsCatchpitEmpty(void) const;
#endif   // WINSUBST_DETOURED

// diagnostic services
#if defined(_DEBUG)
public:
	virtual void AssertValid(void) const;
	virtual void Dump(CDumpContext& dumpCtx) const;
#endif   // _DEBUG
};

//////////////////////////////////////////////////////////////////////////////////////////////
// inlines

inline bool CWinSubstApp::IsCatchpitEmpty(void) const
{
	return (m_mapCatchpit.GetCount() == 0);
}

#endif   // __WinSubstApp_h

// end of file
