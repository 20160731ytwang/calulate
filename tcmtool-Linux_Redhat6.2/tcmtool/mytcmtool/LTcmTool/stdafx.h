
#ifdef WIN32

#if !defined(AFX_STDAFX_H__246FA801_921E_448D_B016_8396250257C6__INCLUDED_)
#define AFX_STDAFX_H__246FA801_921E_448D_B016_8396250257C6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__246FA801_921E_448D_B016_8396250257C6__INCLUDED_)

#else //WIN32

#include   <sys/types.h> 
#include   <sys/stat.h> 
#include   <fcntl.h> 
#include   <stdio.h> 
#include   <unistd.h> 

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "fcntl.h"
#include "SMS4.h"
/*
typedef unsigned long UINT32;
typedef unsigned char       BYTE;


typedef int		INT32;
*/

typedef int		BOOL;
typedef unsigned long       DWORD;

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif


#endif //WIN32

