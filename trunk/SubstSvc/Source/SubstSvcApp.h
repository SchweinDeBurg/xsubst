// SubstSvc NT service.
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

// SubstSvcApp.h - interface of the CSubstSvcApp class

#if !defined(__SubstSvcApp_h)
#define __SubstSvcApp_h

#if defined(_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif   // _MSC_VER

class CSubstSvcApp: public CWinApp
{
	friend int _tmain(int argc, TCHAR* argv[], TCHAR* envp[]);

	DECLARE_DYNAMIC(CSubstSvcApp)

// construction/destruction
public:
	CSubstSvcApp(void);
	virtual ~CSubstSvcApp(void);
};

#endif   // __SubstSvcApp_h

// end of file
