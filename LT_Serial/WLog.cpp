//*****************************************************************************
// Filename	: 	WLog.cpp
// Created	:	2016/2/17 - 11:39
// Modified	:	2016/2/17 - 11:39
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
#include "stdafx.h"
#include "WLog.h"
#include <strsafe.h>

#define MAX_LOG_BUFFER		2048

CWLog::CWLog()
{
}


CWLog::~CWLog()
{
	for (int iCnt = 0; iCnt < MAX_LOG_CNT; iCnt++)
	{
		m_strLog[iCnt].ReleaseBuffer();
	}
}

//=============================================================================
// Method		: IncLogIndex
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2016/2/17 - 11:39
// Desc.		:
//=============================================================================
void CWLog::IncLogIndex()
{
	if (m_byIndex < (MAX_LOG_CNT - 1))
		++m_byIndex;
	else
		m_byIndex = 0;
}

//=============================================================================
// Method		: Send_LogMsg
// Access		: protected  
// Returns		: void
// Parameter	: __in LPCTSTR lpszLog
// Parameter	: __in BOOL bError
// Qualifier	:
// Last Update	: 2016/2/17 - 13:03
// Desc.		:
//=============================================================================
void CWLog::Send_LogMsg(__in LPCTSTR lpszLog, __in BOOL bError)
{
	if (NULL == m_hOwnerWnd)
	{
		ASSERT(_T("NULL == m_hOwnerWnd"));
		return;
	}

	if (NULL == m_WM_Log)
	{
		ASSERT(_T("NULL == m_WM_ID_LOG"));
		return;
	}

	IncLogIndex();
	m_strLog[m_byIndex].ReleaseBuffer();
	m_strLog[m_byIndex] = lpszLog;

	// Parameter	: WPARAM wParam	-> 메세지 문자열
	// Parameter	: LPARAM lParam	
	//					-> HIWORD : 오류 메세지 인가?
	//					-> LOWORD : 로그 종류 (메세지 소스 구분용)
	::SendNotifyMessage(m_hOwnerWnd, m_WM_Log, (WPARAM)m_strLog[m_byIndex].GetBuffer(), (LPARAM)MAKELONG((BYTE)m_nLogType, (BYTE)bError));
}

//=============================================================================
// Method		: LogMsg
// Access		: public  
// Returns		: void
// Parameter	: __in LPCTSTR lpszLog
// Parameter	: ...
// Qualifier	:
// Last Update	: 2016/2/17 - 11:55
// Desc.		:
//=============================================================================
void CWLog::LogMsg(__in LPCTSTR lpszLog, ...)
{
	__try
	{
		TCHAR *lpszBuffer = new TCHAR[MAX_LOG_BUFFER];

		size_t cb = 0;
		va_list args;
		va_start(args, lpszLog);
		::StringCchVPrintfEx(lpszBuffer, MAX_LOG_BUFFER, NULL, &cb, 0, lpszLog, args);
		va_end(args);

		Send_LogMsg(lpszBuffer, FALSE);

		delete[] lpszBuffer;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		TRACE(_T("*** Exception Error : CWLog::LogMsg()\n"));
	}
}

//=============================================================================
// Method		: LogMsg_Err
// Access		: public  
// Returns		: void
// Parameter	: __in LPCTSTR lpszLog
// Parameter	: ...
// Qualifier	:
// Last Update	: 2016/2/17 - 11:55
// Desc.		:
//=============================================================================
void CWLog::LogMsg_Err(__in LPCTSTR lpszLog, ...)
{
	__try
	{
		TCHAR *lpszBuffer = new TCHAR[MAX_LOG_BUFFER];

		size_t cb = 0;
		va_list args;
		va_start(args, lpszLog);
		::StringCchVPrintfEx(lpszBuffer, MAX_LOG_BUFFER, NULL, &cb, 0, lpszLog, args);
		va_end(args);

		Send_LogMsg(lpszBuffer, TRUE);

		delete[] lpszBuffer;

	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		TRACE(_T("*** Exception Error : CWLog::LogMsg_Err()\n"));
	}
}


//=============================================================================
// Method		: LogMsg_ConvNULL
// Access		: public  
// Returns		: void
// Parameter	: __in LPCSTR lpszLog
// Parameter	: __in UINT_PTR nLogSize
// Qualifier	:
// Last Update	: 2016/2/17 - 11:55
// Desc.		:
//=============================================================================
void CWLog::LogMsg_ConvNULL(__in LPCSTR lpszLog, __in UINT_PTR nLogSize)
{
// 	TCHAR* lpszLogString = new TCHAR[nLogSize + 2];
// 	ZeroMemory(lpszLogString, sizeof(TCHAR) * nLogSize + 2);

	CString szLog;

	for (UINT iIndex = 0; iIndex < nLogSize; iIndex++)
	{
		if (NULL == lpszLog[iIndex])
		{
			szLog.AppendChar(_T(' '));
			//lpszLogString[iIndex] = ' ';
		}
		else
		{
			szLog.AppendChar(lpszLog[iIndex]);
			//lpszLogString[iIndex] = lpszLog[iIndex];
		}
	}

	TRACE(_T("Send : %s\n"), szLog);
	Send_LogMsg(szLog, FALSE);

// 	TRACE(_T("Send : %s\n"), lpszLogString);
// 	Send_LogMsg(lpszLogString, FALSE);
// 
// 	delete[] lpszLogString;
}
