// SubstSvc NT service.
// Copyright (c) 2004 by Elijah Zarezky,
// All rights reserved.

// ExecLogging.h - interface of the execution logging
// initially generated by AfxScratch 1.0.2290 on 18.07.2004 at 11:12:43

#if !defined(__ExecLogging_h)
#define __ExecLogging_h

// logging level
enum LOG_LEVEL {
	LL_NONE		= 0,
	LL_MINIMAL	= 1,
	LL_NORMAL	= 2,
	LL_VERBOSE	= 3
};

void LogFile_Create(LPCTSTR pszFileName);
void LogFile_WriteEntry(LOG_LEVEL eLevel, LPCTSTR pszFormat, ...);
void LogFile_WriteEntry(LOG_LEVEL eLevel, UINT idsFormat, ...);
int LogFile_WriteDbgRpt(int fnType, char* pszMessage, int* pnRetVal);

#endif	// __ExecLogging_h

// end of file
