// WinSubst application.
// Copyright (c) 2004-2010 by Elijah Zarezky,
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

// WinSubstApp.cpp - implementation of the CWinSubstApp class

// initially generated by AfxScratch v1.0.2297 on 25.07.2004 at 13:26:11
// visit http://zarezky.spb.ru/projects/afx_scratch.html for more info

//////////////////////////////////////////////////////////////////////////////////////////////
// PCH includes

#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////////////////////////
// resource includes

#include "Resource.h"

//////////////////////////////////////////////////////////////////////////////////////////////
// other includes

#include "WinSubstApp.h"
#include "SubstsList.h"
#include "MainDialog.h"

//////////////////////////////////////////////////////////////////////////////////////////////
// misc defines

#define SZ_MUTEX_APP_INST_NAME _T("WinSubst.Instance.326F8F0D-E321-4832-B29F-08542F06BCB9")

//////////////////////////////////////////////////////////////////////////////////////////////
// debugging support

#if defined(_DEBUG)
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif   // _DEBUG

//////////////////////////////////////////////////////////////////////////////////////////////
// object model

IMPLEMENT_DYNAMIC(CWinSubstApp, CWinApp)

//////////////////////////////////////////////////////////////////////////////////////////////
// message map

BEGIN_MESSAGE_MAP(CWinSubstApp, CWinApp)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////////////////
// construction/destruction

CWinSubstApp::CWinSubstApp(void):
m_hMutexAppInst(NULL)
{
#if defined(WINSUBST_DETOURED)
	if (RegQueryCatchpit() > 0)
	{
		Detoured();

		(PVOID&)m_pfnLoadLibrary = ::DetourFindFunction("kernel32.dll", STRINGIZE(LoadLibrary));
		(PVOID&)m_pfnLoadLibraryEx = ::DetourFindFunction("kernel32.dll", STRINGIZE(LoadLibraryEx));
		
		DetourTransactionBegin();
		DetourUpdateThread(::GetCurrentThread());
		DetourAttach(reinterpret_cast<PVOID*>(&m_pfnLoadLibrary), &CWinSubstApp::LoadLibrary);
		DetourAttach(reinterpret_cast<PVOID*>(&m_pfnLoadLibraryEx), &CWinSubstApp::LoadLibraryEx);
		DetourTransactionCommit();
	}
#endif   // WINSUBST_DETOURED
}

CWinSubstApp::~CWinSubstApp(void)
{
#if defined(WINSUBST_DETOURED)
	if (!IsCatchpitEmpty())
	{
		DetourTransactionBegin();
		DetourUpdateThread(::GetCurrentThread());
		DetourDetach(reinterpret_cast<PVOID*>(&m_pfnLoadLibrary),  &CWinSubstApp::LoadLibrary);
		DetourDetach(reinterpret_cast<PVOID*>(&m_pfnLoadLibraryEx),  &CWinSubstApp::LoadLibraryEx);
		DetourTransactionCommit();
	}
#endif   // WINSUBST_DETOURED
}

//////////////////////////////////////////////////////////////////////////////////////////////
// operations

HICON CWinSubstApp::LoadSmIcon(LPCTSTR pszResName)
{
	HINSTANCE hInstRes = AfxGetResourceHandle();
	int cxSmIcon = ::GetSystemMetrics(SM_CXSMICON);
	int cySmIcon = ::GetSystemMetrics(SM_CYSMICON);
	HANDLE hSmIcon = ::LoadImage(hInstRes, pszResName, IMAGE_ICON, cxSmIcon, cySmIcon, 0);
	return (static_cast<HICON>(hSmIcon));
}

void CWinSubstApp::GetVersionString(CString& strDest)
{
	TCHAR szExeName[_MAX_PATH];
	DWORD dwHandle;
	CString strValueName;
	void* pvVerString;
	UINT cchFileVer;

	::GetModuleFileName(AfxGetInstanceHandle(), szExeName, _MAX_PATH);
	DWORD cbSize = ::GetFileVersionInfoSize(szExeName, &dwHandle);
	BYTE* pbVerInfo = new BYTE[cbSize];
	::GetFileVersionInfo(szExeName, dwHandle, cbSize, pbVerInfo);
	strValueName.LoadString(IDS_FILE_VERSION);
	::VerQueryValue(pbVerInfo, strValueName.GetBuffer(0), &pvVerString, &cchFileVer);
	strValueName.ReleaseBuffer();
	strDest = reinterpret_cast<LPCTSTR>(pvVerString);
	delete[] pbVerInfo;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// overridables

BOOL CWinSubstApp::InitInstance(void)
{
	m_hMutexAppInst = ::CreateMutex(NULL, TRUE, SZ_MUTEX_APP_INST_NAME);
	if (m_hMutexAppInst == NULL)
	{
		AfxMessageBox(IDS_APP_INIT_FAILED, MB_OK | MB_ICONSTOP);
		return (FALSE);
	}
	else if (::GetLastError() == ERROR_ALREADY_EXISTS)
	{
		HWND hMainDialog = ::FindWindow(CMainDialog::GetWindowClassName(), NULL);
		ASSERT(::IsWindow(hMainDialog));
		::SetForegroundWindow(hMainDialog);
		::CloseHandle(m_hMutexAppInst);
		return (FALSE);
	}

	CMainDialog dlgMain;
	m_pMainWnd = &dlgMain;
	dlgMain.DoModal();
	
	::CloseHandle(m_hMutexAppInst);
	m_hMutexAppInst = NULL;

	return (FALSE);
}

//////////////////////////////////////////////////////////////////////////////////////////////
// implementation helpers

#if defined(WINSUBST_DETOURED)

CWinSubstApp::PFN_LOAD_LIBRARY CWinSubstApp::m_pfnLoadLibrary(NULL);
CWinSubstApp::PFN_LOAD_LIBRARY_EX CWinSubstApp::m_pfnLoadLibraryEx(NULL);

HMODULE WINAPI CWinSubstApp::LoadLibrary(LPCTSTR pszFileName)
{
	TRACE(_T("*** CWinSubstApp::LoadLibrary(%s)\n"), pszFileName);

	CWinSubstApp* pApp = DYNAMIC_DOWNCAST(CWinSubstApp, AfxGetApp());
	ASSERT(pApp != NULL);

	CString strFileNameLower(::PathFindFileName(pszFileName));
	strFileNameLower.MakeLower();

	DWORD fCatch = false;
	if (pApp->m_mapCatchpit.Lookup(strFileNameLower, fCatch) && fCatch != 0)
	{
		::SetLastError(ERROR_FILE_NOT_FOUND);
		return (NULL);
	}
	else {
		return (m_pfnLoadLibrary(pszFileName));
	}
}

HMODULE WINAPI CWinSubstApp::LoadLibraryEx(LPCTSTR pszFileName, HANDLE hFile, DWORD fdwFlags)
{
	TRACE(_T("*** CWinSubstApp::LoadLibraryEx(%s, 0x%08X, 0x%08X)\n"), pszFileName, hFile, fdwFlags);

	CWinSubstApp* pApp = DYNAMIC_DOWNCAST(CWinSubstApp, AfxGetApp());
	ASSERT(pApp != NULL);

	CString strFileNameLower(::PathFindFileName(pszFileName));
	strFileNameLower.MakeLower();

	DWORD fCatch = FALSE;
	if (pApp->m_mapCatchpit.Lookup(strFileNameLower, fCatch) && fCatch != 0)
	{
		::SetLastError(ERROR_FILE_NOT_FOUND);
		return (NULL);
	}
	else {
		return (m_pfnLoadLibraryEx(pszFileName, hFile, fdwFlags));
	}
}

INT_PTR CWinSubstApp::RegQueryCatchpit(void)
{
	CString strKeyName;
	CRegKey regKey;

	m_mapCatchpit.RemoveAll();

	// build the name of the registry key...
	::LoadString(::GetModuleHandle(NULL), IDS_REGISTRY_KEY, strKeyName.GetBuffer(_MAX_PATH), _MAX_PATH);
	strKeyName.ReleaseBuffer();
	strKeyName.Insert(0, _T(".DEFAULT\\Software\\"));
	strKeyName += _T("\\WinSubst\\Catchpit");

	// ...and then open this key
	regKey.Create(HKEY_USERS, strKeyName);
	
	DWORD cNumValues = 0;
	if (::RegQueryInfoKey(regKey, 0, 0, 0, 0, 0, 0, &cNumValues, 0, 0, 0, 0) == ERROR_SUCCESS)
	{
		for (DWORD i = 0; i < cNumValues; ++i)
		{
			TCHAR szValueName[_MAX_PATH] = { 0 };
			DWORD cchNameLen = _countof(szValueName);
			DWORD fdwValueType = REG_NONE;
			if (::RegEnumValue(regKey, i, szValueName, &cchNameLen, 0, &fdwValueType, 0, 0) == ERROR_SUCCESS)
			{
				if (fdwValueType == REG_DWORD)
				{
					DWORD fCatch = FALSE;
					regKey.QueryDWORDValue(szValueName, fCatch);
					_tcslwr_s(szValueName, cchNameLen + 1);
					m_mapCatchpit.SetAt(szValueName, fCatch);
				}
			}
		}
	}

	return (m_mapCatchpit.GetCount());
}

#endif   // WINSUBST_DETOURED

//////////////////////////////////////////////////////////////////////////////////////////////
// diagnostic services

#if defined(_DEBUG)

void CWinSubstApp::AssertValid(void) const
{
	// first perform inherited validity check...
	CWinApp::AssertValid();

	// ...and then verify our own state as well
}

void CWinSubstApp::Dump(CDumpContext& dumpCtx) const
{
	try
	{
		// first invoke inherited dumper...
		CWinApp::Dump(dumpCtx);

		// ...and then dump own unique members
		dumpCtx << "m_hMutexAppInst = " << m_hMutexAppInst;
#if defined(WINSUBST_DETOURED)
		dumpCtx << "\nm_mapCatchpit = " << m_mapCatchpit;
#endif   // WINSUBST_DETOURED
	}
	catch (CFileException* pXcpt)
	{
		pXcpt->ReportError();
		pXcpt->Delete();
	}
}

#endif   // _DEBUG

//////////////////////////////////////////////////////////////////////////////////////////////
// the one and only application object

static CWinSubstApp g_appWinSubst;

// end of file
