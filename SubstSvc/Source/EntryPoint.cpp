// SubstSvc NT service.
// Copyright (c) 2004 by Elijah Zarezky,
// All rights reserved.

// EntryPoint.cpp - application entry point
// initially generated by AfxScratch 1.0.2290 on 18.07.2004 at 11:12:43

#include "stdafx.h"
#include "SubstSvcApp.h"
#include "Resource.h"
#include "ExecLogging.h"

#if defined(_DEBUG)
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif	// _DEBUG

// the one and only application object
static CSubstSvcApp g_appSubstSvc;

// entry point
int _tmain(int argc, TCHAR* argv[], TCHAR* /*envp*/[])
{
	// try to initialize MFC
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), SW_HIDE)) {
		return (-1);
	}

#if defined(_DEBUG)
	// install a client-defined reporting function
	_CrtSetReportHook(WriteLogFileDbgRpt);
#endif	// _DEBUG

#if defined(_DEBUG)
	CMemoryState memStart, memStop, memDiff;
	// get the initial snapshot of memory
	memStart.Checkpoint();
#endif	// _DEBUG

	// assign registry key
	g_appSubstSvc.SetRegistryKey(IDS_REGISTRY_KEY);

	// start logging
	CString strLogName(argv[0]);
	::PathRenameExtension(strLogName.GetBuffer(0), _T(".log"));
	strLogName.ReleaseBuffer();
	CreateLogFile(strLogName);
	WriteLogFileEntry(LL_MINIMAL, _T("Started %s executable."), argv[0]);

	// parse command-line arguments
	if (argc > 1) {
		SC_HANDLE schManager = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
		if (::lstrcmpi(argv[1], _T("/Register")) == 0) {
			// insert service into the SCM database
			SC_HANDLE schService = ::CreateService(schManager, g_szServiceName, g_szServiceName,
				SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS, SERVICE_DEMAND_START,
				SERVICE_ERROR_NORMAL, argv[0], NULL, NULL, NULL, NULL, NULL);
			::CloseServiceHandle(schService);
			WriteLogFileEntry(LL_MINIMAL, _T("Registered %s service."), g_szServiceName);
		}
		else if (::lstrcmpi(argv[1], _T("/UnRegister")) == 0) {
			// remove service from the SCM database
			SC_HANDLE schService = ::OpenService(schManager, g_szServiceName, SERVICE_ALL_ACCESS);
			::DeleteService(schService);
			::CloseServiceHandle(schService);
			WriteLogFileEntry(LL_MINIMAL, _T("Unregistered %s service."), g_szServiceName);
		}
		else {
			// unrecognized command-line argument
			WriteLogFileEntry(LL_MINIMAL, _T("Command-line argument %s unrecognized."), argv[1]);
		}
		::CloseServiceHandle(schManager);
	}
	else {
		// just start the service
		static SERVICE_TABLE_ENTRY aste[] = {
			{ g_szServiceName, ServiceMain },
			{ NULL, NULL }		// end of table - required
		};
		WriteLogFileEntry(LL_MINIMAL, _T("Attempted to connect to the SCM."));
		::StartServiceCtrlDispatcher(aste);
		WriteLogFileEntry(LL_MINIMAL, _T("Disconnected from the SCM."));
	}

#if defined(_DEBUG)
	// get the final memory state...
	memStop.Checkpoint();
	// ...and then check the leaks
	if (memDiff.Difference(memStart, memStop)) {
		_CrtMemDumpStatistics(&memDiff.m_memState);
		_CrtDumpMemoryLeaks();
	}
#endif	// _DEBUG
	WriteLogFileEntry(LL_MINIMAL, _T("Finished %s executable.\n<<< end of log"), argv[0]);

	// terminate MFC
#if defined(_DEBUG)
	AfxLockTempMaps();
	AfxUnlockTempMaps(-1);
#endif	// _DEBUG
	AfxWinTerm();

	// return to Windoze
	return (0);
}

// end of file