// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#ifndef  STDAFX_H 
#define  STDAFX_H 

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#pragma once

//Windows XP	NTDDI_WINXP		(0x05010000)
//Windows 7		NTDDI_WIN7		(0x06010000)
//Windows 8		NTDDI_WIN8		(0x06020000)
//Windows 8.1	NTDDI_WINBLUE	(0x06030000)

#ifndef WINVER                          // 필요한 최소 플랫폼을 Windows Vista로 지정합니다.
#define WINVER 0x0601           // 다른 버전의 Windows에 맞도록 적합한 값으로 변경해 주십시오.
#endif

#ifndef _WIN32_WINNT            // 필요한 최소 플랫폼을 Windows Vista로 지정합니다.
#define _WIN32_WINNT 0x0601     // 다른 버전의 Windows에 맞도록 적합한 값으로 변경해 주십시오.
#endif

#ifndef _WIN32_WINDOWS          // 필요한 최소 플랫폼을 Windows 98로 지정합니다.
#define _WIN32_WINDOWS 0x0601 // Windows Me 이상에 맞도록 적합한 값으로 변경해 주십시오.
#endif

#ifndef _WIN32_IE                       // 필요한 최소 플랫폼을 Internet Explorer 7.0으로 지정합니다.
#define _WIN32_IE _WIN32_IE_IE70        // 다른 버전의 IE에 맞도록 적합한 값으로 변경해 주십시오.
#endif


#include <afxwin.h>
#include <afxcontrolbars.h>

#define _CRT_SECURE_NO_DEPRECATE

 #pragma warning(disable: 4996)

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif 