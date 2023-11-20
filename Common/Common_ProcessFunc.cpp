#include "stdafx.h"
#include <windows.h>
#include <process.h>
#include <Tlhelp32.h>
#include <stdio.h>
#include <conio.h>
#include <Psapi.h>
#include "Common_ProcessFunc.h"

struct CallbackInfo
{
	DWORD m_dwPID;
	HWND m_hWnd;
};

//=============================================================================
// Method		: EnumProc
// Access		: public 
// Returns		: BOOL CALLBACK
// Parameter	: HWND hWnd
// Parameter	: LPARAM lParam
// Qualifier	:
// Last Update	: 2011/1/10 - 11:49
// Desc.		:
//=============================================================================
BOOL CALLBACK EnumProc(HWND hWnd, LPARAM lParam)
{
	DWORD dwPID = 0;
	CallbackInfo *pst_Callback = (CallbackInfo *)lParam;

	GetWindowThreadProcessId(hWnd, &dwPID);

	if (pst_Callback->m_dwPID != dwPID)
		return TRUE;

	pst_Callback->m_hWnd = hWnd;
	return FALSE;
}

//=============================================================================
// Method		: GetWindowFromPID
// Access		: public 
// Returns		: HWND
// Parameter	: DWORD dwPID
// Qualifier	:
// Last Update	: 2011/1/10 - 11:49
// Desc.		: 프로세스 ID로 HWND 구하기
//=============================================================================
HWND GetWindowFromPID(DWORD dwPID)
{
	CallbackInfo st_Callback;

	if (dwPID == 0)
		return NULL;

	st_Callback.m_dwPID = dwPID;
	st_Callback.m_hWnd = NULL;

	EnumWindows(EnumProc, (LPARAM)&st_Callback);

	return st_Callback.m_hWnd;
}

//=============================================================================
// Method		: GetProcessModule
// Access		: public 
// Returns		: BOOL
// Parameter	: DWORD dwPID
// Parameter	: LPCTSTR pszProcessName
// Qualifier	:
// Last Update	: 2011/1/10 - 11:49
// Desc.		: 해당 프로세스의 모듈리스트를 루프로 돌려서 프로세스이름과 동일하면true를 리턴.
//=============================================================================
BOOL GetProcessModule(DWORD dwPID, LPCTSTR pszProcessName)
{
	HANDLE        hModuleSnap = NULL;
	MODULEENTRY32 me32        = {0};

	hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID);

	if (hModuleSnap == (HANDLE)-1)
		return FALSE;

	me32.dwSize = sizeof(MODULEENTRY32);
	//해당 프로세스의 모듈리스트를 루프로 돌려서 프로세스이름과 동일하면
	//true를 리턴한다.
	if(Module32First(hModuleSnap, &me32))
	{
		do
		{
			if(_tcscmp(me32.szModule, pszProcessName) == 0)
			{
				CloseHandle (hModuleSnap);
				return TRUE;
			}
		}while(Module32Next(hModuleSnap, &me32));
	}

	CloseHandle (hModuleSnap);

	return FALSE;
}

//=============================================================================
// Method		: GetProcessID
// Access		: public 
// Returns		: DWORD
// Parameter	: LPCTSTR pszProcessName
// Qualifier	:
// Last Update	: 2011/1/10 - 11:49
// Desc.		: 프로세스 이름으로 PID 구하기
//=============================================================================
DWORD GetProcessID(LPCTSTR pszProcessName)
{

	HANDLE         hProcessSnap = NULL;
	BOOL           bRet      = FALSE;
	PROCESSENTRY32 pe32      = {0};

	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == (HANDLE)-1)
		return false;

	pe32.dwSize = sizeof(PROCESSENTRY32);
	DWORD dwProcessID = 0;

	//프로세스가 메모리상에 있으면 첫번째 프로세스를 얻는다
	if (Process32First(hProcessSnap, &pe32))
	{
		BOOL          bCurrent = FALSE;
		MODULEENTRY32 me32       = {0};
		do
		{
			bCurrent = GetProcessModule(pe32.th32ProcessID, pszProcessName);
			if(bCurrent)
			{
				dwProcessID = pe32.th32ProcessID;
				break;
			}

		} while (Process32Next(hProcessSnap, &pe32)); //다음 프로세스의 정보를 구하여 있으면 루프
	}

	CloseHandle (hProcessSnap);
	return dwProcessID;
}

//=============================================================================
// Method		: ExitProcess
// Access		: public 
// Returns		: void
// Parameter	: CString strTargetProcName
// Parameter	: DWORD dwExceptProcId
// Qualifier	:
// Last Update	: 2011/1/10 - 11:50
// Desc.		: 프로세스 이름과 PID로 강제 종료 시키기
//=============================================================================
void ExitProcess(CString strTargetProcName, DWORD dwExceptProcId)
{
	DWORD aProcesses[1024]={0,}, cbNeeded, cProcesses;
	unsigned int i = 0;

	//실행중인 프로세스를 모두 구한다
	if (!EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded))
		return;

	//실행중인 프로세스의 개수를 구한다
	cProcesses = cbNeeded / sizeof(DWORD);
	//각 프로세스에 대한 이름 및 프로세스 아이디를 구하고 타겟 프로세스를 강제로 종료시킨다
	for ( i = 0; i < cProcesses; i++ )
	{
		TCHAR szProcessName[MAX_PATH] = _T("unknown");
		HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, aProcesses[i] );

		if (NULL != hProcess )
		{
			HMODULE hMod;
			DWORD cbNeeded;

			if (EnumProcessModules( hProcess, &hMod, sizeof(hMod), &cbNeeded))
				GetModuleBaseName( hProcess, hMod, szProcessName, sizeof(szProcessName)); //프로세스 이름을 구한다(szProcessName)
		}

		BOOL bRet;
		CString strComp = szProcessName;
		//실행중인 프로세스(strComp)와 죽이려는 프로세스(strTargetProcName) 이름을 비교하여 같으면 해당 프로세스 종료
		//단 실행중인 나 자신은 죽이려는 프로세스에서 제외시킨다
		if((strComp.MakeLower() == strTargetProcName.MakeLower()) && (aProcesses[i] != dwExceptProcId))
		{
			HANDLE hKillProc = OpenProcess(PROCESS_TERMINATE, FALSE, aProcesses[i]);

			if(hKillProc != NULL)
			{
				DWORD ExitCode = 0;

				GetExitCodeProcess( hKillProc, &ExitCode );
				bRet = TerminateProcess(hKillProc, ExitCode );
				if( bRet )
					WaitForSingleObject(hKillProc, INFINITE);

				CloseHandle(hKillProc);
			}
		}
		CloseHandle( hProcess );
	}
}

//=============================================================================
// Method		: FocredCloseProcess
// Access		: public 
// Returns		: BOOL
// Parameter	: LPCTSTR lpszProcessName
// Qualifier	:
// Last Update	: 2011/1/10 - 11:51
// Desc.		: 프로세스 이름으로 강제 종료
//=============================================================================
BOOL FocredCloseProcess(LPCTSTR lpszProcessName)
{
	DWORD dwSize = 250;
	HANDLE hSnapShot;
	PROCESSENTRY32 pEntry;
	BOOL bCrrent=FALSE;


	hSnapShot=CreateToolhelp32Snapshot(TH32CS_SNAPALL,NULL);

	pEntry.dwSize =sizeof(pEntry);
	// 실행중인 프로세스들의 첫번재 정보를 가져온다.
	Process32First (hSnapShot,&pEntry);

	// Tool이 실행중인지 확인
	while(1)
	{
		// 다음번 프로세스의 정보를 가져온다.
		if (FALSE == Process32Next (hSnapShot,&pEntry))
			break;

		if(!strncmp(pEntry.szExeFile, lpszProcessName, 15))
		{
			bCrrent = TRUE;
		}

		if(bCrrent)
		{
			HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pEntry.th32ProcessID);
			if(hProcess)
			{
				if(TerminateProcess(hProcess, 0))
				{
					unsigned long nCode; //프로세스 종료 상태
					GetExitCodeProcess(hProcess, &nCode);
				}
				CloseHandle(hProcess);
			}
			break;
		} 

	}

	return FALSE;
}


//=============================================================================
// Method		: GetProcessList
// Access		: public 
// Returns		: HANDLE
// Parameter	: LPCTSTR szFilename
// Qualifier	:
// Last Update	: 2011/1/10 - 11:52
// Desc.		: 프로세스 이름으로 핸들 구하기
//=============================================================================
HANDLE GetProcessList(LPCTSTR szFilename)
{
	HANDLE hProcessSnapshot;
	HANDLE hProcess;
	PROCESSENTRY32 pe32;

	hProcessSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPALL,0);

	if (hProcessSnapshot == INVALID_HANDLE_VALUE) 
		return INVALID_HANDLE_VALUE;

	pe32.dwSize = sizeof(PROCESSENTRY32);

	Process32First(hProcessSnapshot, &pe32);

	do
	{
		hProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE,pe32.th32ProcessID);
		if (hProcess != NULL) 
		{
			if (_tcscmp(pe32.szExeFile, szFilename) ==0)
			{
				wprintf(L"Process ID of %s : %d\n", pe32.szExeFile, pe32.th32ProcessID);
				return hProcess;
			}
		}

	} while(Process32Next(hProcessSnapshot, &pe32));

	return INVALID_HANDLE_VALUE;
}

//=============================================================================
// Method		: SafeTerminateProcess
// Access		: public 
// Returns		: BOOL
// Parameter	: HANDLE hProcess
// Parameter	: UINT uExitCode
// Qualifier	:
// Last Update	: 2011/1/10 - 11:52
// Desc.		: 프로세스 안전하게 종료시키기
//=============================================================================
BOOL SafeTerminateProcess(HANDLE hProcess, UINT uExitCode)
{
	DWORD dwTID, dwCode, dwErr = 0;
	HANDLE hProcessDup = INVALID_HANDLE_VALUE;
	HANDLE hRT = NULL;
	HINSTANCE hKernel = GetModuleHandle(_T("Kernel32"));

	BOOL bSuccess = FALSE;
	BOOL bDup = DuplicateHandle(GetCurrentProcess(), 
								hProcess, 
								GetCurrentProcess(), 
								&hProcessDup, 
								PROCESS_ALL_ACCESS, 
								FALSE, 
								0);
	if ( GetExitCodeProcess((bDup) ? hProcessDup : hProcess, &dwCode) && (dwCode == STILL_ACTIVE) )
	{
		FARPROC pfnExitProc;
		pfnExitProc = GetProcAddress(hKernel, "ExitProcess");
		hRT = CreateRemoteThread(	(bDup) ? hProcessDup : hProcess, 
									NULL, 
									0, 
									(LPTHREAD_START_ROUTINE)pfnExitProc,
									(PVOID)uExitCode, 0, &dwTID);
		if ( hRT == NULL ) dwErr = GetLastError();
	}
	else 
	{
		dwErr = ERROR_PROCESS_ABORTED;
	}
	if ( hRT )
	{
		WaitForSingleObject((bDup) ? hProcessDup : hProcess, INFINITE);
		CloseHandle(hRT);
		bSuccess = TRUE;
	}
	if ( bDup ) 
		CloseHandle(hProcessDup);
	if ( !bSuccess )
		SetLastError(dwErr);

	return bSuccess;
}



//int _tmain(int argc, _TCHAR* argv[])
//{
//
//	HANDLE hProcess;
//	WCHAR szExplorer[MAX_PATH];
//	STARTUPINFO si;   
//	PROCESS_INFORMATION pi;   
//	ZeroMemory(szExplorer, sizeof(szExplorer));
//	ZeroMemory( &si, sizeof( si ) );   
//	ZeroMemory( &pi, sizeof( pi ) );   
//	si.cb = sizeof( si );  
//
//
//	hProcess = GetProcessList(L"explorer.exe");
//	if (hProcess != INVALID_HANDLE_VALUE)
//	{
//		GetModuleFileNameEx(hProcess, NULL, szExplorer, MAX_PATH);  
//	}
//	else
//	{
//		_tcscpy(szExplorer, L"c:\\windows\\explorer.exe");
//	}
//
//
//	wprintf(L"Terminating %s...\n", szExplorer);
//
//	if (hProcess != INVALID_HANDLE_VALUE)
//	{ 
//		if (SafeTerminateProcess(hProcess, 1))
//			wprintf(L"Terminated %s.\n", szExplorer);
//	}
//
//	if (CreateProcessW( szExplorer, szExplorer, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi ))
//		wprintf(L"Running %s...\n", szExplorer);
//
//	//getchar();
//	return 0;
//}



HWND FindProcessHWND( ULONG pid )
{
	HWND hFindWnd = ::FindWindow(NULL,NULL); // 최상위 윈도우 핸들 찾기   

	while( hFindWnd != NULL )   
	{   
		if( ::GetParent(hFindWnd) == NULL ) // 최상위 핸들인지 체크, 버튼 등도 핸들을 가질 수 있으므로 무시하기 위해   
		{
			ULONG idProc;  
			GetWindowThreadProcessId( hFindWnd, &idProc );

			if( pid == idProc )   
				return hFindWnd;   
		}
		hFindWnd = ::GetWindow(hFindWnd, GW_HWNDNEXT); // 다음 윈도우 핸들 찾기   
	}

	return NULL;
}

HWND FindProcessHandle(CString strProcessName)
{
	DWORD aProcesses[1024], cbNeeded, cProcesses;
	unsigned int i;

	if( !EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded ))
		return NULL;

	cProcesses = cbNeeded / sizeof(DWORD);

	for ( i = 0; i < cProcesses; i++ )
	{
		char szProcessName[MAX_PATH] = "unknown";

		HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, aProcesses[i] );

		if (NULL != hProcess )
		{
			HMODULE hMod;
			DWORD cbNeeded;

			if ( EnumProcessModules( hProcess, &hMod, sizeof(hMod), &cbNeeded) )
			{
				GetModuleBaseName( hProcess, hMod, szProcessName, sizeof(szProcessName) );
			}
		}

		if(strcmp(szProcessName,(LPSTR)(LPCTSTR)strProcessName) == 0)
		{
			HANDLE hProcess = NULL;

			hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, aProcesses[i]);
			if(hProcess != NULL)
			{ 
				return FindProcessHWND (aProcesses[i]);
				//return hProcess;
			}
		}
		CloseHandle( hProcess );
	}

	return NULL;
}

void ExitProcessName(CString strProcessName)
{
	DWORD aProcesses[1024], cbNeeded, cProcesses;
	unsigned int iCnt = 0;

	if( !EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded ))
		return;

	cProcesses = cbNeeded / sizeof(DWORD);

	for ( iCnt = 0; iCnt < cProcesses; iCnt++ )
	{
		char szProcessName[MAX_PATH] = "unknown";

		HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, aProcesses[iCnt] );

		if (NULL != hProcess )
		{
			HMODULE hMod;
			DWORD	cbNeeded;

			if ( EnumProcessModules( hProcess, &hMod, sizeof(hMod), &cbNeeded) )
			{
				GetModuleBaseName( hProcess, hMod, szProcessName, sizeof(szProcessName) );
			}
		}

		BOOL bRet = FALSE;
		if (0 == strProcessName.Compare(szProcessName))		
		{
			HANDLE hProcess = NULL;

			hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, aProcesses[iCnt]);
			if(hProcess != NULL)
			{ 
				DWORD ExitCode = 0;
				GetExitCodeProcess( hProcess, &ExitCode );
				bRet = ::TerminateProcess(hProcess, ExitCode ); 
				if( bRet )
				{
					::WaitForSingleObject(hProcess, INFINITE);
				} 

				::CloseHandle(hProcess);
			}
		}
		CloseHandle( hProcess );
	}
}