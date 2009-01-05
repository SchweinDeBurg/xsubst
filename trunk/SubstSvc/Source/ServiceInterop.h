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

// ServiceInterop.h - interoperability declarations

#if !defined(__ServiceInterop_h)
#define __ServiceInterop_h

#if defined(_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif   // _MSC_VER

// set up member alignment to be 4 bytes for all shared structures
#pragma pack(push, ServiceInterop, 4)

//////////////////////////////////////////////////////////////////////////////////////////////
// service name

#define SZ_SERVICE_NAME       _T("Substituted Drives Manager")

//////////////////////////////////////////////////////////////////////////////////////////////
// synchronization event name

#define SZ_SYNC_EVENT_NAME    _T("xsubst_completion_event")

//////////////////////////////////////////////////////////////////////////////////////////////
// custom control codes

#define SUBSTSVC_SERVICE_CONTROL_MODIFY_DRIVES	128

// return alignment to whatever it was before we entered this file
#pragma pack(pop, ServiceInterop)

#endif   // __ServiceInterop_h

// end of file
