//*****************************************************************************
// Filename	: Common_ProcessFunc.h
// Created	: 2011/1/6
// Modified	: 2011/1/6 - 18:09
//
// Author	: PiRing
//	
// Purpose	: 
//*****************************************************************************
#ifndef Common_ProcessFunc_h__
#define Common_ProcessFunc_h__


// 프로세스명으로 HWND 구하기
BOOL CALLBACK EnumProc				(HWND hWnd, LPARAM lParam);
HWND		GetWindowFromPID		(DWORD dwPID);
BOOL		GetProcessModule		(DWORD dwPID, LPCTSTR pszProcessName);
DWORD		GetProcessID			(LPCTSTR pszProcessName);

// 프로세스 강제 종료
void		ExitProcess				(CString strTargetProcName, DWORD dwExceptProcId);
// 프로세스 명으로 강제 종료
BOOL		FocredCloseProcess		(LPCTSTR lpszProcessName);

// 프로세스 안전하게 종료 시키기
HANDLE		GetProcessList			(LPCTSTR szFilename);
BOOL		SafeTerminateProcess	(HANDLE hProcess, UINT uExitCode);

// 프로세스명으로 HWND 구하기?
HWND FindProcessHWND( ULONG pid );
HWND FindProcessHandle(CString strProcessName);

void ExitProcessName(CString strProcessName);


#endif // Common_ProcessFunc_h__