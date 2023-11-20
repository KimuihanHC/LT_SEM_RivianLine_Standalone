//*****************************************************************************
// Filename	: 	LogFile_V2.cpp
// Created	:	2016/11/19 - 10:41
// Modified	:	2016/11/19 - 10:41
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
#include "stdafx.h"
#include "LogFile_V2.h"
#include <io.h>

static const USHORT	usUnicode = 0xfeff;

//=============================================================================
// 생성자
//=============================================================================
CLogFile_V2::CLogFile_V2()
{
	m_nThreadId			= 0;
	m_szFileExt			= _T("log");
	m_bUseTimeStamp		= FALSE;
	m_bUseCrLf			= FALSE;
	m_bUseAutoFilename	= TRUE;
	m_bUseAutoSubPath	= TRUE;
	
	GetCurrentDirectory(_MAX_PATH, m_szPath.GetBuffer(_MAX_PATH));
}

//=============================================================================
// 소멸자
//=============================================================================
CLogFile_V2::~CLogFile_V2()
{

}

//=============================================================================
// Method		: WriteLog
// Access		: private  
// Returns		: BOOL
// Qualifier	:
// Last Update	: 2016/11/19 - 12:22
// Desc.		:
//=============================================================================
BOOL CLogFile_V2::WriteLog()
{
	BOOL bOpen = FALSE;
	if (!PathFileExists(m_szLogFullPath))
	{
		// 폴더 생성
		CreateLogFolder(m_szLogPath);

		bOpen = m_fileLog.Open(m_szLogFullPath, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyNone | CFile::typeBinary);
		
#ifdef UNICODE
		m_fileLog.Write(&usUnicode, 2);
#endif
	}
	else
	{
		if (bOpen = m_fileLog.Open(m_szLogFullPath, CFile::modeRead | CFile::modeWrite | CFile::shareDenyNone | CFile::typeBinary))
		{
			m_fileLog.SeekToEnd();
		}
	}

	if (bOpen)
	{
		if (m_bUseTimeStamp)
		{
			m_szLog.Insert(0, GetTimeStamp() + _T(" -> "));
		}

		if (m_bUseCrLf)
		{
			m_szLog.Insert(m_szLog.GetLength(), _T("\r\n"));
		}

		m_fileLog.Write(m_szLog, m_szLog.GetLength() * sizeof(TCHAR));
		m_fileLog.Flush();
		m_fileLog.Close();
	}

	return bOpen;
}

//=============================================================================
// Method		: CreateLogFolder
// Access		: protected  
// Returns		: void
// Parameter	: __in CString szPath
// Qualifier	:
// Last Update	: 2016/11/19 - 12:23
// Desc.		:
//=============================================================================
void CLogFile_V2::CreateLogFolder(__in CString szPath)
{
	if (_taccess(szPath, 0) != 0)
	{
		BOOL	l_bExtractSucc = TRUE;
		int		iCnt = 0;
		CString l_strDirPath = _T("");
		CString l_strNowPath = _T("");

		while (l_bExtractSucc)
		{
			l_bExtractSucc = AfxExtractSubString(l_strNowPath, szPath, iCnt, '\\');
			l_strDirPath += l_strNowPath + _T("\\");

			if (l_bExtractSucc && _taccess(l_strDirPath, 0) != 0)
			{
				CreateDirectory(l_strDirPath, NULL);
			}
			iCnt++;
		}
	}
}

//=============================================================================
// Method		: GetTimeStamp
// Access		: protected  
// Returns		: CString
// Qualifier	:
// Last Update	: 2016/11/19 - 12:23
// Desc.		:
//=============================================================================
CString CLogFile_V2::GetTimeStamp()
{
	CString		strTime;

	strTime.Format(_T("[%02d:%02d:%02d.%03d]"), m_tmStamp.wHour, m_tmStamp.wMinute, m_tmStamp.wSecond, m_tmStamp.wMilliseconds);

	return strTime;
}

//=============================================================================
// Method		: MakeLogPath
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2016/11/19 - 12:23
// Desc.		:
//=============================================================================
void CLogFile_V2::MakeLogPath()
{
	if (m_bUseAutoSubPath)
	{
		if (m_szSubPath.IsEmpty())
			m_szLogPath.Format(_T("%s\\%04d\\%02d"), m_szPath, m_tmStamp.wYear, m_tmStamp.wMonth, m_tmStamp.wDay);
		else
			m_szLogPath.Format(_T("%s\\%s\\%04d\\%02d"), m_szPath, m_szSubPath, m_tmStamp.wYear, m_tmStamp.wMonth, m_tmStamp.wDay);
	}
	else
	{
		m_szLogPath.Format(_T("%s\\%s"), m_szPath, m_szSubPath);
	}
}

//=============================================================================
// Method		: MakeLogFullPath
// Access		: protected  
// Returns		: void
// Parameter	: __in LPCTSTR szFilename
// Qualifier	:
// Last Update	: 2016/11/19 - 12:23
// Desc.		:
//=============================================================================
void CLogFile_V2::MakeLogFullPath(__in LPCTSTR szFilename /*= NULL*/)
{
	MakeLogPath();

	if ((m_bUseAutoFilename) || (NULL == szFilename))
		m_szLogFullPath.Format(_T("%s\\%s_%04d_%02d_%02d.%s"), m_szLogPath, m_szLogFilename_Prefix, m_tmStamp.wYear, m_tmStamp.wMonth, m_tmStamp.wDay, m_szFileExt);
	else
		m_szLogFullPath.Format(_T("%s\\%s.%s"), m_szLogPath, szFilename, m_szFileExt);
}

//=============================================================================
// Method		: LogWrite
// Access		: public  
// Returns		: void
// Parameter	: __in LPCTSTR szLog
// Qualifier	:
// Last Update	: 2016/11/19 - 12:23
// Desc.		:
//=============================================================================
void CLogFile_V2::LogWrite(__in LPCTSTR szLog)
{
	long nThreadId = ::GetCurrentThreadId();

	while (m_nThreadId != nThreadId)
	{
		::InterlockedCompareExchange(&m_nThreadId, nThreadId, 0);
		if (m_nThreadId == nThreadId)
			break;

		::Sleep(25);
	}

	// 현재 시간
	GetLocalTime(&m_tmStamp);

	m_szLog = szLog;

	// 로그 경로 + 로그 파일 이름
	MakeLogFullPath();

	if (PathFileExists(m_szLogFullPath))
	{
		BOOL bOpen = false;

		if (bOpen = m_fileLog.Open(m_szLogFullPath, CFile::modeRead | CFile::modeWrite | CFile::shareDenyNone | CFile::typeBinary))
		{
			ULONGLONG fileSize = m_fileLog.SeekToEnd();

			if (fileSize > 1000000)
			{
				CString szNewName;

				szNewName.Format(_T("%s\\%s_%04d_%02d_%02d_%02d%02d.%s"), m_szLogPath, m_szLogFilename_Prefix, m_tmStamp.wYear, m_tmStamp.wMonth, m_tmStamp.wDay, m_tmStamp.wHour, m_tmStamp.wMinute,m_szFileExt);
			//	m_fileLog.Rename(m_szLogFullPath, szNewName);
				m_fileLog.Close();

				USES_CONVERSION;
				rename(T2A(m_szLogFullPath), T2A(szNewName));
			}
			else
			{
				m_fileLog.Close();
			}
		}
	}

	WriteLog();

	::InterlockedCompareExchange(&m_nThreadId, 0, ::GetCurrentThreadId());
}

//=============================================================================
// Method		: LogWrite
// Access		: public  
// Returns		: void
// Parameter	: __in LPCTSTR szLog
// Parameter	: __in LPCTSTR szFilename
// Qualifier	:
// Last Update	: 2016/11/19 - 12:23
// Desc.		:
//=============================================================================
void CLogFile_V2::LogWrite(__in LPCTSTR szLog, __in LPCTSTR szFilename)
{
	long nThreadId = ::GetCurrentThreadId();

	while (m_nThreadId != nThreadId)
	{
		::InterlockedCompareExchange(&m_nThreadId, nThreadId, 0);
		if (m_nThreadId == nThreadId)
			break;

		::Sleep(25);
	}

	// 현재 시간
	GetLocalTime(&m_tmStamp);

	m_szLog = szLog;

	// 로그 경로 + 로그 파일 이름
	MakeLogFullPath(szFilename);

	if (PathFileExists(m_szLogFullPath))
	{
		BOOL bOpen = false;

		if (bOpen = m_fileLog.Open(m_szLogFullPath, CFile::modeRead | CFile::modeWrite | CFile::shareDenyNone | CFile::typeBinary))
		{
			ULONGLONG fileSize = m_fileLog.SeekToEnd();

			if (fileSize > 10240)
			{
				CString szNewName;

				SYSTEMTIME tm;
				GetLocalTime(&tm);

				szNewName.Format(_T("%s\\%s_%02d.%s"), m_szLogPath, szFilename, tm.wHour, m_szFileExt);
				m_fileLog.Rename(m_szLogFullPath, szNewName);
			}
		}

		m_fileLog.Close();
	}

	WriteLog();

	::InterlockedCompareExchange(&m_nThreadId, 0, ::GetCurrentThreadId());
}

//=============================================================================
// Method		: LogWrite
// Access		: public  
// Returns		: void
// Parameter	: __in LPCTSTR szLog
// Parameter	: __in LPCTSTR szSubPath
// Parameter	: __in LPCTSTR szFilename
// Qualifier	:
// Last Update	: 2016/11/19 - 12:23
// Desc.		:
//=============================================================================
void CLogFile_V2::LogWrite(__in LPCTSTR szLog, __in LPCTSTR szSubPath, __in LPCTSTR szFilename)
{
	long nThreadId = ::GetCurrentThreadId();

	while (m_nThreadId != nThreadId)
	{
		::InterlockedCompareExchange(&m_nThreadId, nThreadId, 0);
		if (m_nThreadId == nThreadId)
			break;

		::Sleep(25);
	}

	// 현재 시간
	GetLocalTime(&m_tmStamp);

	m_szLog = szLog;

	m_szLogPath.Format(_T("%s\\%s"), m_szPath, szSubPath);
	m_szLogFullPath.Format(_T("%s\\%s.%s"), m_szLogPath, szFilename, m_szFileExt);

	if (PathFileExists(m_szLogFullPath))
	{
		BOOL bOpen = false;

		if (bOpen = m_fileLog.Open(m_szLogFullPath, CFile::modeRead | CFile::modeWrite | CFile::shareDenyNone | CFile::typeBinary))
		{
			ULONGLONG fileSize = m_fileLog.SeekToEnd();

			if (fileSize > 10240)
			{
				CString szNewName;

				SYSTEMTIME tm;
				GetLocalTime(&tm);

				szNewName.Format(_T("%s\\%s_%02d.%s"), m_szLogPath, szFilename, tm.wHour, m_szFileExt);
				m_fileLog.Rename(m_szLogFullPath, szNewName);
			}
		}

		m_fileLog.Close();
	}

	WriteLog();

	::InterlockedCompareExchange(&m_nThreadId, 0, ::GetCurrentThreadId());
}
