//*****************************************************************************
// Filename	: 	LogFile_V2.h
// Created	:	2016/11/19 - 10:40
// Modified	:	2016/11/19 - 10:40
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
#ifndef LogFile_V2_h__
#define LogFile_V2_h__

#pragma once

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
class CLogFile_V2
{
public:
	CLogFile_V2();
	~CLogFile_V2();

private:
	long		m_nThreadId;

	CFile		m_fileLog;
	SYSTEMTIME	m_tmStamp;

	CString		m_szPath;
	CString		m_szSubPath;
	CString		m_szLogFilename_Prefix;
	CString		m_szFileExt;

	// 버퍼용 데이터
	CString		m_szLogPath;
	CString		m_szLogFullPath;
	CString		m_szLog;

	// 설정 플래그
	BOOL		m_bUseTimeStamp;
	BOOL		m_bUseCrLf;
	BOOL		m_bUseAutoFilename;
	BOOL		m_bUseAutoSubPath;

	inline BOOL WriteLog				();

protected:

	void		CreateLogFolder			(__in CString szPath);
	CString		GetTimeStamp			();
	void		MakeLogPath				();
	void		MakeLogFullPath			(__in LPCTSTR szFilename = NULL);

public:

	void		SetPath					(__in LPCTSTR szPath, __in LPCTSTR szSubPath = NULL)
	{
		m_szPath	= szPath;
		m_szSubPath = szSubPath;
	};

	// Log 파일명 접두사 설정
	void		SetLogFileName_Prefix	(__in LPCTSTR lpszLogName)
	{
		m_szLogFilename_Prefix = lpszLogName;
	};

	// 추가 파일 확장자 지정 (기본 .log)
	void		SetFileExt				(__in LPCTSTR lpszExt)
	{
		if (NULL != lpszExt)
			m_szFileExt = lpszExt;
	};

	void		SetUseTimeStamp			(__in BOOL bUseTimeStamp, __in BOOL bUseCrLf, __in BOOL bUseAutoFilename, __in BOOL bUseAutoSubPath)
	{
		m_bUseTimeStamp		= bUseTimeStamp;
		m_bUseCrLf			= bUseCrLf;
		m_bUseAutoFilename	= bUseAutoFilename;
		m_bUseAutoSubPath	= bUseAutoSubPath;
	};

	void		LogWrite				(__in LPCTSTR szLog);
	void		LogWrite				(__in LPCTSTR szLog, __in LPCTSTR szFilename);
	void		LogWrite				(__in LPCTSTR szLog, __in LPCTSTR szSubPath, __in LPCTSTR szFilename);


	// 현재 저장되는 로그 파일의 패스를 반환
	CString		GetLogPath()
	{
		return m_szLogPath;
	};

	CString		GetLogFullPath()
	{
		return m_szLogFullPath;
	};

};

#endif // LogFile_V2_h__

