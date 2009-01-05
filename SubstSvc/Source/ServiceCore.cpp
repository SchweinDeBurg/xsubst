// SubstSvc NT service.
// Copyright (c) 2004-2009 by Elijah Zarezky,
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

// ServiceCore.cpp - core service routines

// initially generated by AfxScratch v1.0.2290 on 18.07.2004 at 11:12:43
// visit http://zarezky.spb.ru/projects/afx_scratch.html for more info

//////////////////////////////////////////////////////////////////////////////////////////////
// PCH includes

#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////////////////////////
// resource includes

#include "Resource.h"

//////////////////////////////////////////////////////////////////////////////////////////////
// other includes

#include "ExecLogging.h"
#include "ServiceInterop.h"
#include "SubstSvcApp.h"

//////////////////////////////////////////////////////////////////////////////////////////////
// unwanted warnings

#if defined(__INTEL_COMPILER)
// remark #171: invalid type conversion
#pragma warning(disable: 171)
// remark #177: variable was declared but never referenced
#pragma warning(disable: 177)
// remark #981: operands are evaluated in unspecified order
#pragma warning(disable: 981)
#endif	// __INTEL_COMPILER

//////////////////////////////////////////////////////////////////////////////////////////////
// debugging support

#if defined(_DEBUG)
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif	// _DEBUG

//////////////////////////////////////////////////////////////////////////////////////////////
// implementation helpers

static int EnumDrivesKey(BOOL (__cdecl* pfnCallback)(ATL::CRegKey&, LPCTSTR, LPCTSTR))
{
	CString strKeyName;
	CString strRegistryKey;   // for logging
	ATL::CRegKey regKey;
	TCHAR szDrive[_MAX_DRIVE];
	TCHAR szPath[_MAX_PATH];
	CMap<CString, LPCTSTR, CString, LPCTSTR> mapSubsts;
	CString strDrive, strPath;

	// build key name
	strKeyName.LoadString(IDS_REGISTRY_KEY);
	strKeyName.Insert(0, _T("Software\\"));
	strKeyName += _T('\\');
	strKeyName += AfxGetAppName();
	strKeyName += _T("\\Drives");
	strRegistryKey.Format(IDS_REGKEY_FORMAT, static_cast<LPCTSTR>(strKeyName));

	if (regKey.Create(HKEY_CURRENT_USER, strKeyName) == ERROR_SUCCESS)
	{
		LogFile_WriteEntry(LL_MINIMAL, IDS_REGKEY_OPENED, static_cast<LPCTSTR>(strRegistryKey));

		// pass #1 - collect substitutions
		BOOL fHasValue = TRUE;
		for (int i = 0; fHasValue; ++i)
		{
			DWORD cchName = _MAX_DRIVE;
			DWORD fdwType = REG_NONE;
			LONG nResult = ::RegEnumValue(regKey, i, szDrive, &cchName, NULL, &fdwType, NULL, NULL);
			if (nResult == ERROR_NO_MORE_ITEMS)
			{
				// no more values - so break the loop
				fHasValue = FALSE;
			}
			else if (nResult == ERROR_SUCCESS)
			{
				// only string values are interested
				if (fdwType == REG_SZ)
				{
					DWORD cchValue = _MAX_PATH;
					if (regKey.QueryValue(szPath, szDrive, &cchValue) == ERROR_SUCCESS)
					{
						mapSubsts.SetAt(szDrive, szPath);
					}
				}
			}
			else {
				// what the shit is that?!
			}
		}

		// pass #2 - invoke callback for each substitution
		int cNumDrives = 0;
		POSITION pos = mapSubsts.GetStartPosition();
		while (pos != NULL)
		{
			mapSubsts.GetNextAssoc(pos, strDrive, strPath);
			if ((*pfnCallback)(regKey, strDrive, strPath))
			{
				++cNumDrives;
			}
		}
		if (regKey.Close() == ERROR_SUCCESS)
		{
			LogFile_WriteEntry(LL_MINIMAL, IDS_REGKEY_CLOSED, static_cast<LPCTSTR>(strRegistryKey));
		}
		return (cNumDrives);
	}
	else {
		LogFile_WriteEntry(LL_MINIMAL, IDS_REGKEY_FAILED, static_cast<LPCTSTR>(strRegistryKey));
		return (0);
	}
}

static BOOL SubstCreate(ATL::CRegKey& /*regKey*/, LPCTSTR pszDrive, LPCTSTR pszPath)
{
	if (::DefineDosDevice(0, pszDrive, pszPath))
	{
		LogFile_WriteEntry(LL_MINIMAL, IDS_SUBST_CREATED, pszDrive, pszPath);
		return (TRUE);
	}
	else {
		LogFile_WriteEntry(LL_MINIMAL, IDS_SUBST_FAILED, pszDrive, pszPath);
		return (FALSE);
	}
}

static BOOL SubstDelete(ATL::CRegKey& /*regKey*/, LPCTSTR pszDrive, LPCTSTR pszPath)
{
	if (::DefineDosDevice(DDD_REMOVE_DEFINITION, pszDrive, pszPath))
	{
		LogFile_WriteEntry(LL_MINIMAL, IDS_SUBST_DELETED, pszDrive, pszPath);
		return (TRUE);
	}
	else {
		LogFile_WriteEntry(LL_MINIMAL, IDS_DELETE_FAILED, pszDrive, pszPath);
		return (FALSE);
	}
}

static BOOL SubstModify(ATL::CRegKey& regKey, LPCTSTR pszDrive, LPCTSTR pszPath)
{
	if (pszPath[0] == _T('+'))
	{
		// create new substitution
		if (::DefineDosDevice(0, pszDrive, &pszPath[1]))
		{
			LogFile_WriteEntry(LL_MINIMAL, IDS_SUBST_CREATED, pszDrive, &pszPath[1]);
			regKey.SetValue(&pszPath[1], pszDrive);
			return (TRUE);
		}
		else {
			LogFile_WriteEntry(LL_MINIMAL, IDS_SUBST_FAILED, pszDrive, &pszPath[1]);
			return (FALSE);
		}
	}
	else if (pszPath[0] == _T('*'))
	{
		// modify existing substitution
		LPTSTR pszTemp = new TCHAR[_MAX_PATH];
		if (::QueryDosDevice(pszDrive, pszTemp, _MAX_PATH) > 0)
		{
			// first remove current substitution...
			LPTSTR pszPrevPath = ::StrChr(pszTemp, _T(':')) - 1;
			if (::DefineDosDevice(DDD_REMOVE_DEFINITION, pszDrive, pszPrevPath))
			{
				LogFile_WriteEntry(LL_MINIMAL, IDS_SUBST_DELETED, pszDrive, pszPrevPath);
			}
			else {
				LogFile_WriteEntry(LL_MINIMAL, IDS_DELETE_FAILED, pszDrive, pszPrevPath);
			}
		}
		delete[] pszTemp;

		// ...and then create a new one
		if (::DefineDosDevice(0, pszDrive, &pszPath[1]))
		{
			LogFile_WriteEntry(LL_MINIMAL, IDS_SUBST_CREATED, pszDrive, &pszPath[1]);
			regKey.SetValue(&pszPath[1], pszDrive);
			return (TRUE);
		}
		else {
			LogFile_WriteEntry(LL_MINIMAL, IDS_SUBST_FAILED, pszDrive, &pszPath[1]);
			return (FALSE);
		}
	}
	else if (pszPath[0] == _T('-'))
	{
		// delete substitution
		if (::DefineDosDevice(DDD_REMOVE_DEFINITION, pszDrive, &pszPath[1]))
		{
			LogFile_WriteEntry(LL_MINIMAL, IDS_SUBST_DELETED, pszDrive, &pszPath[1]);
			regKey.DeleteValue(pszDrive);
			return (TRUE);
		}
		else {
			LogFile_WriteEntry(LL_MINIMAL, IDS_DELETE_FAILED, pszDrive, &pszPath[1]);
			return (FALSE);
		}
	}
	else {
		// nothing to do
		return (FALSE);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////
// startup thread procedure

static UINT StartupProc(void* /*pParam*/)
{
	LogFile_WriteEntry(LL_MINIMAL, IDS_STARTUP_STARTED);

	// create initial substitutions
	int cDrivesSubst = EnumDrivesKey(SubstCreate);
	LogFile_WriteEntry(LL_MINIMAL, IDS_SUBST_TOTAL, cDrivesSubst);

	// successfully finished
	LogFile_WriteEntry(LL_MINIMAL, IDS_STARTUP_FINISHED);
	AfxEndThread(0);
	return (0);
}

//////////////////////////////////////////////////////////////////////////////////////////////
// control handler function of a service

void WINAPI ServiceHandler(DWORD fdwControl)
{
	SERVICE_STATUS ss;
	HANDLE hEvent;
	int cDrivesSubst;

	ss.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	switch (fdwControl)
	{
	case SERVICE_CONTROL_SHUTDOWN:
		LogFile_WriteEntry(LL_MINIMAL, IDS_OS_SHUTDOWN);
		// fall through
	case SERVICE_CONTROL_STOP:
		LogFile_WriteEntry(LL_MINIMAL, IDS_SERVICE_STOP, g_szServiceName);
		cDrivesSubst = EnumDrivesKey(SubstDelete);
		LogFile_WriteEntry(LL_MINIMAL, IDS_DELETED_TOTAL, cDrivesSubst);
		ss.dwCurrentState = SERVICE_STOPPED;
		g_dwServiceState = ss.dwCurrentState;
		break;
	case SUBSTSVC_SERVICE_CONTROL_MODIFY_DRIVES:
		LogFile_WriteEntry(LL_MINIMAL, IDS_MODIFY_REQUEST);
		cDrivesSubst = EnumDrivesKey(SubstModify);
		LogFile_WriteEntry(LL_MINIMAL, IDS_MODIFIED_TOTAL, cDrivesSubst);
		hEvent = ::OpenEvent(EVENT_MODIFY_STATE, FALSE, SZ_SYNC_EVENT_NAME);
		::SetEvent(hEvent);
		::CloseHandle(hEvent);
		// fall through
	default:
		ss.dwCurrentState = g_dwServiceState;
		break;
	}
	ss.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
	ss.dwWin32ExitCode = NO_ERROR;
	ss.dwServiceSpecificExitCode = 0;   // will be ignored
	ss.dwCheckPoint = 0;
	ss.dwWaitHint = 0;
	::SetServiceStatus(g_hServiceStatus, &ss);
	if (g_dwServiceState == SERVICE_STOPPED)
	{
		LogFile_WriteEntry(LL_MINIMAL, IDS_SERVICE_STOPPED, g_szServiceName);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////
// service entry-point function

void WINAPI ServiceMain(DWORD /*dwArgc*/, LPTSTR /*apszArgv*/[])
{
	SERVICE_STATUS ss;

	g_hServiceStatus = ::RegisterServiceCtrlHandler(g_szServiceName, ServiceHandler);
	ss.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	ss.dwCurrentState = SERVICE_RUNNING;
	ss.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
	ss.dwWin32ExitCode = NO_ERROR;
	ss.dwServiceSpecificExitCode = 0;   // will be ignored
	ss.dwCheckPoint = 0;
	ss.dwWaitHint = 0;
	::SetServiceStatus(g_hServiceStatus, &ss);
	g_dwServiceState = ss.dwCurrentState;
	LogFile_WriteEntry(LL_MINIMAL, IDS_SERVICE_RUNNED, g_szServiceName);
	AfxBeginThread(StartupProc, NULL);
}

// end of file
