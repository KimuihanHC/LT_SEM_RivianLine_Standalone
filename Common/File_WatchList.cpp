//*****************************************************************************
// Filename	: 	File_WatchList.cpp
// Created	:	2016/1/29 - 16:23
// Modified	:	2016/1/29 - 16:23
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
#include "stdafx.h"
#include "File_WatchList.h"


CFile_WatchList::CFile_WatchList()
{
	m_hOwner			= NULL;
	m_hThrWatchDir		= NULL;
	m_hEndWatchEvent	= NULL;
	m_WM_Refresh		= NULL;

	m_hEndWatchEvent = CreateEvent( NULL, FALSE, FALSE,NULL);
}


CFile_WatchList::~CFile_WatchList()
{
	if (NULL != m_hThrWatchDir)
	{
		EndWatchThrFunc();
	}

	if (NULL != m_hEndWatchEvent)
	{
		CloseHandle(m_hEndWatchEvent);
		m_hEndWatchEvent = NULL;
	}
}

//=============================================================================
// Method		: SetOwner
// Access		: public  
// Returns		: void
// Parameter	: __in HWND hOwner
// Parameter	: __in UINT nWinMsg
// Qualifier	:
// Last Update	: 2016/3/15 - 13:28
// Desc.		:
//=============================================================================
void CFile_WatchList::SetOwner(__in HWND hOwner, __in UINT nWinMsg)
{
	if (NULL != hOwner)
		m_hOwner = hOwner;

	m_WM_Refresh = nWinMsg;
}

//=============================================================================
// Method		: SetWatchOption
// Access		: public  
// Returns		: void
// Parameter	: __in LPCTSTR lpszDir
// Parameter	: __in LPCTSTR lpszExt
// Qualifier	:
// Last Update	: 2016/3/15 - 13:28
// Desc.		:
//=============================================================================
void CFile_WatchList::SetWatchOption(__in LPCTSTR lpszDir, __in LPCTSTR lpszExt)
{
	if (NULL != lpszDir)
		m_strFilePath = lpszDir;

	if (NULL != lpszExt)
		m_strExt = lpszExt;
}

//=============================================================================
// Method		: BeginWatchThrFunc
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2016/2/1 - 11:43
// Desc.		:
//=============================================================================
void CFile_WatchList::BeginWatchThrFunc()
{
	if (IsThreadAlive() || m_strFilePath.IsEmpty())
		return;

	UINT nWatchDirID = 0;	
	m_hThrWatchDir = HANDLE(_beginthreadex(NULL, 0, ThrWatchDirectory, this, 0, NULL));
	
	if (NULL == m_hThrWatchDir)
		AfxMessageBox(_T("m_hThrWatchDir Handle is NULL!!"));
}

//=============================================================================
// Method		: EndWatchThrFunc
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2016/2/1 - 11:48
// Desc.		:
//=============================================================================
void CFile_WatchList::EndWatchThrFunc()
{
	DWORD dwExitCode = NULL;
	__try
	{
		if (FALSE == SetEvent(m_hEndWatchEvent))
		{
			TRACE(_T("Set m_hEndWatchEvent Event 실패!!\n"));
		}

		WaitForSingleObject(m_hThrWatchDir, STATUS_TIMEOUT);

		GetExitCodeThread(m_hThrWatchDir, &dwExitCode);
		if (STILL_ACTIVE == dwExitCode)
		{
			TerminateThread(m_hThrWatchDir, dwExitCode);
			WaitForSingleObject(m_hThrWatchDir, WAIT_ABANDONED);
		}

		if (NULL != m_hThrWatchDir)
		{
			CloseHandle(m_hThrWatchDir);
			m_hThrWatchDir = NULL;
		}
		TRACE(_T("Delete CFile_WatchList::EndWatchThrFunc Completed\n"));
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		TRACE(_T("*** Exception Error : CFile_WatchList::EndWatchThrFunc() \n"));
	}
}

//=============================================================================
// Method		: ThrWatchDirectory
// Access		: protected static  
// Returns		: UINT WINAPI
// Parameter	: LPVOID lpV
// Qualifier	:
// Last Update	: 2016/2/1 - 11:48
// Desc.		:
//=============================================================================
UINT WINAPI CFile_WatchList::ThrWatchDirectory(LPVOID lpV)
{
	DWORD	dwWaitStatus = 0;
	HANDLE	hEvents[2];
	BOOL	bExitLoop = TRUE;

	CFile_WatchList* pThis;
	pThis = (CFile_WatchList*)lpV;

	hEvents[0] = FindFirstChangeNotification(pThis->m_strFilePath, FALSE, FILE_NOTIFY_CHANGE_FILE_NAME);
	hEvents[1] = pThis->m_hEndWatchEvent;

	if (hEvents[0] == INVALID_HANDLE_VALUE)
		ExitProcess(GetLastError());

	while (bExitLoop)
	{
		dwWaitStatus = WaitForMultipleObjects(2, hEvents, FALSE, INFINITE);

		switch (dwWaitStatus)
		{
		case WAIT_OBJECT_0:
			pThis->RefreshList();
			if (FindNextChangeNotification(hEvents[0]) == FALSE)
			{
				ExitProcess(GetLastError());
			}
			break;

		case WAIT_OBJECT_0 + 1:
			TRACE(_T("Exit WatchDirectory Thread\n"));
			bExitLoop = FALSE;
			break;
				
		default:
			MSG msg;
			while (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			break;
		}
	}

	//strMDLPath.ReleaseBuffer();
	TRACE(_T("쓰레드 종료 : ThrWatchDirectory \n"));
	return TRUE;
}

//=============================================================================
// Method		: RefreshList
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2016/2/1 - 12:01
// Desc.		:
//=============================================================================
void CFile_WatchList::RefreshList()
{
	ResetList();

	SearchFileDir();

	if (NULL != m_hOwner)
	{
		SendNotifyMessage(m_hOwner, m_WM_Refresh, 0, 0);
		//AfxGetApp()->GetMainWnd()->SendNotifyMessage(m_hOwner, 0, 0);
	}
}

//=============================================================================
// Method		: ResetList
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2016/2/1 - 11:55
// Desc.		:
//=============================================================================
void CFile_WatchList::ResetList()
{
	m_lstFile.RemoveAll();
}

//=============================================================================
// Method		: SearchModuleDir
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2016/2/1 - 11:55
// Desc.		:
//=============================================================================
void CFile_WatchList::SearchFileDir()
{
	CFileFind	finder;
	CString		strName;
	CString		strPartID;
	UINT		nInch = 0;
	CString		strInchType;

	strName.Format(_T("%s\\*.%s"), m_strFilePath, m_strExt);

	BOOL bWorking = finder.FindFile(strName);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		strPartID = finder.GetFileTitle();

		if (!strPartID.IsEmpty())
		{
			// 파일 이름을 목록에 추가
			m_lstFile.AddTail(strPartID);
		}
	}
}

//=============================================================================
// Method		: SaveFile
// Access		: virtual public  
// Returns		: BOOL
// Parameter	: __in LPCTSTR lpszFilePath
// Parameter	: __in const VOID * pIn
// Qualifier	:
// Last Update	: 2016/3/15 - 13:25
// Desc.		:
//=============================================================================
BOOL CFile_WatchList::SaveFile(__in LPCTSTR lpszFilePath, __in const VOID* pIn)
{
	if (NULL == lpszFilePath)
		return FALSE;

	if (NULL == pIn)
		return FALSE;

	::DeleteFile(lpszFilePath);


	return TRUE;
}

//=============================================================================
// Method		: LoadFile
// Access		: virtual public  
// Returns		: BOOL
// Parameter	: __in LPCTSTR lpszFilePath
// Parameter	: __out VOID * pOut
// Qualifier	:
// Last Update	: 2016/3/15 - 13:24
// Desc.		:
//=============================================================================
BOOL CFile_WatchList::LoadFile(__in LPCTSTR lpszFilePath, __out VOID* pOut)
{
	if (NULL == lpszFilePath)
		return FALSE;




	return TRUE;
}

//=============================================================================
// Method		: IsThreadAlive
// Access		: public  
// Returns		: BOOL
// Qualifier	:
// Last Update	: 2016/3/16 - 18:07
// Desc.		:
//=============================================================================
BOOL CFile_WatchList::IsThreadAlive()
{
	DWORD dwExitCode = 0;
	GetExitCodeThread(m_hThrWatchDir, &dwExitCode);
	if (STILL_ACTIVE == dwExitCode)
	{
		return TRUE;
	}

	return FALSE;
}
