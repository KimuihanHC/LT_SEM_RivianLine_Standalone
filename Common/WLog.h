//*****************************************************************************
// Filename	: 	WLog.h
// Created	:	2016/2/17 - 11:23
// Modified	:	2016/2/17 - 11:23
//
// Author	:	PiRing
//	
//-----------------------------------------------------------------------------
// Log Windows Message 파라미터
// Parameter	: WPARAM wParam	-> 메세지 문자열
// Parameter	: LPARAM lParam	
//					-> HIWORD : 오류 메세지 구분
//					-> LOWORD : 로그 종류 (메세지 소스 구분용)
//*****************************************************************************
#ifndef WLog_h__
#define WLog_h__

#pragma once

class CWLog
{
public:
	CWLog();
	~CWLog();

protected:
	//-----------------------------------------------------
	// 오너 윈도우로 Log 메세지 보내기 위한 함수
	//-----------------------------------------------------
	HWND			m_hOwnerWnd;
	UINT			m_WM_Log;

	UINT			m_nLogType;

#define		MAX_LOG_CNT		5

	CString			m_strLog[MAX_LOG_CNT];
	BYTE			m_byIndex;

	void			IncLogIndex			();
	void			Send_LogMsg			(__in LPCTSTR lpszLog, __in BOOL bError);

public:
	void	 SetOwner(HWND hWnd, UINT nWinMessage)
	{
		m_hOwnerWnd = hWnd;
		m_WM_Log = nWinMessage;
	};

	void	SetLogType(UINT nType)
	{
		m_nLogType = nType;
	};

	void			LogMsg				(__in LPCTSTR lpszLog, ...);
	void			LogMsg_Err			(__in LPCTSTR lpszLog, ...);
	void			LogMsg_ConvNULL		(__in LPCSTR lpszLog, __in UINT_PTR nLogSize);

};
#endif // WLog_h__

