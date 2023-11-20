// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

//Windows XP	NTDDI_WINXP		(0x05010000)
//Windows 7		NTDDI_WIN7		(0x06010000)
//Windows 8		NTDDI_WIN8		(0x06020000)
//Windows 8.1	NTDDI_WINBLUE	(0x06030000)

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0601	// Change this to the appropriate value to target other versions of Windows.
#endif						

//#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <afxwin.h>         // MFC core and standard components
#include <Windows.h>



// TODO: reference additional headers your program requires here
#define _CRT_SECURE_NO_DEPRECATE