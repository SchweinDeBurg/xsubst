// SubstSvc NT service.
// Copyright (c) 2004 by Elijah Zarezky,
// All rights reserved.

// ServiceInterop.h - interoperability declarations

#if !defined(__ServiceInterop_h)
#define __ServiceInterop_h

// set up member alignment to be 4 bytes for all shared structures
#pragma pack(push, ServiceInterop, 4)

// service name
#define SZ_SERVICE_NAME			_T("Substituted Drives Manager")

// synchronization event name
#define SZ_SYNC_EVENT_NAME		_T("xsubst_completion_event")

// custom control codes
#define SUBSTSVC_SERVICE_CONTROL_MODIFY_DRIVES	128

// return alignment to whatever it was before we entered this file
#pragma pack(pop, ServiceInterop)

#endif	// __ServiceInterop_h

// end of file
