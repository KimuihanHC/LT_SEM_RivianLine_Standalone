//*****************************************************************************
// Filename	: LogFile.h
// Created	: 2010/10/14
// Modified	: 2012/1/19 - 11:15
//
// Author	: G.H Lee (수정 : PiRing)
//
// Purpose	: 
//*****************************************************************************
#ifndef LogFile_h__
#define LogFile_h__

#pragma once

//=============================================================================
// CLogFile
//=============================================================================
class CLogFile
{
public:    
	CLogFile(LPCTSTR lpszLogName = NULL, LPCTSTR lpszSubDirName = NULL);
    ~CLogFile();

private:
	long		m_nThreadId;

	CFile		m_fileLog;

	//로그폴더에 현재 작성될 로그폴더이름이 존재하는지 검사
	BOOL		FindLogFile			();
	void		GetLogFilePath		(CString& strPath);
	CString		Logfilename			();
	void		CreateLogFolder		(CString strPath );	
	CString		GetTime				();

	// 기본 로그 출력
	void		AddLog				(LPCTSTR lpszLog, 
									 LPCTSTR lpszSubDir = NULL,
									 LPCTSTR lpszLogName = NULL,
									 UINT	nFileDivType = 0	// 로그 파일 분할 종류 (none, am/pm, filesize, hour)
									);

	CString		m_strPath;
	CString		m_strLogName;
	CString		m_strSubDirName;
	BYTE		m_byFileDivType;

	BOOL		m_bUseWriteTime;

	CString		m_strFileExt;

public:

	void		SetDefPath			(LPCTSTR lpszPath)
	{
		m_strPath = lpszPath;
	};

	void		SetUseWriteTime		(BOOL bUse = TRUE)
	{
		m_bUseWriteTime = bUse;
	};

	void		LogWrite			(LPCTSTR lpszMsg);
	void		LogWriteWithHeader	(LPCTSTR lpszMsg, LPCTSTR lpszHeader = NULL);

	// Log 기본 파일명 설정 (파일명 + 날짜 + 시간 or 카운트)
	void		SetLogName			(LPCTSTR lpszLogName);

	void		SetSubDirName		(LPCTSTR lpszSubDirName);

	// 로그 파일 분할 종류 (none, am/pm, filesize, hour)
	void		SetFileDivType		(BYTE byDivType);


	//  날짜 폴더 생성여부 (년, 월, 일)
	// all, 월까지, 년까지, 생성안함
	//void		SetUseDateSubDir	(BYTE byDateDirLimit = 0);

	// 현재 저장되는 로그 파일의 패스를 반환
	CString		GetFilePath			();

	// 추가 파일 확장자 지정 (기본 .log)
	void		SetFileExt			(LPCTSTR lpszExt);
};

#endif // LogFile_h__
