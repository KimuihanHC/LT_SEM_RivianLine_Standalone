#ifndef _LOGCOMM_H__
#define _LOGCOMM_H__

#if _MSC_VER > 1000
#pragma once
#endif

#include <vector>
#include "ExString.h"

#define LOG_BUFFER_SIZE		10240

class CLogCommunication
{
private:
	CFile		m_File;
	CFile		m_CSVFile;
	CFile		m_InspectionDataFile;
	CExString	m_strLogRootPath;
	CExString	m_strServiceName;
	int			m_nExpirePeriod;
	CExString	m_strCurrentLog;
	CTime		m_Time;

	CExString	m_strCSVHeader;

	//CRITICAL_SECTION   m_csFile;

	BOOL		m_bUseCSVFile;
	BOOL		m_bUseTxtFile;
	BOOL		m_bUseInspectionDataFile;

	BOOL		m_bMonthlySplitFolder;
	
	BOOL		m_bStart;

protected:
	BOOL OpenLogFile( CExString& strLogPathName );
	void DeleteExpiredLogFile();
	void DeleteExpiredCSVFile();

	BOOL OpenCSVFile( CExString& strCSVPathName );
	void GetCSVPathName( CTime& Time, CExString& strCSVPathName );
	void GetLogPathName( CTime& Time, CExString& strLogPathName );

public:
	explicit CLogCommunication( LPCTSTR lpszServiceName );
	virtual ~CLogCommunication();

	void UseTextFile( BOOL bUse );
	void UseCSVFile( BOOL bUse, LPCTSTR lpszCSVHeader );
	void UseMonthlySplitFolder( BOOL bUse );
		
	void SetLogDirectory( LPCTSTR lpszPath ) { m_strLogRootPath = lpszPath; }
	void SetExpirePeriod( int nDay ) { m_nExpirePeriod = nDay; }
	
	void LogOut( LPCTSTR lpszId, LPCTSTR lpszFormat, ... );

	BOOL Start();
	BOOL Stop();

	void CreateFolder(LPCTSTR lpszPath);

	void Init();
	void Exit();
};

#endif // __X_LOGSYSTEM_H__