//*****************************************************************************
// Filename	: LogFile.cpp
// Created	: 2010/10/14
// Modified	: 2010/10/14 - 18:04
//
// Author	: G.H Lee (수정 : PiRing)
//	
// Purpose	: 
//*****************************************************************************
#include "stdafx.h"
#include "LogFile.h"
#include <io.h>


//=============================================================================
// 생성자
//=============================================================================
CLogFile::CLogFile( LPCTSTR lpszLogName /*= NULL*/, LPCTSTR lpszSubDirName /*= NULL*/ )
{
	m_nThreadId		= 0;
	m_byFileDivType	= 0;
	m_bUseWriteTime = FALSE;
	
	GetCurrentDirectory(_MAX_PATH, m_strPath.GetBuffer(_MAX_PATH));
	
	if (NULL != lpszLogName)
		m_strLogName	= lpszLogName;

	if (NULL != lpszSubDirName)
		m_strSubDirName	= lpszSubDirName;

	m_strFileExt = _T("log");
}

//=============================================================================
// 소멸자
//=============================================================================
CLogFile::~CLogFile()
{
	
}

//=============================================================================
// Method		: CLogFile::FindLogFile
// Access		: public 
// Returns		: BOOL
// Parameter	: 
// Qualifier	:
// Last Update	: 2010/10/14 - 18:08
// Desc.		:
//=============================================================================
BOOL CLogFile::FindLogFile()
{
    CFileFind	filefind;
    CString		strFullPath;

	GetLogFilePath(strFullPath);
   // strFullPath = GetLogFilePath() + Logfilename();
	//strFullPath = strFullPath + Logfilename();
	strFullPath.Append(Logfilename());

    if(filefind.FindFile(strFullPath))
		return TRUE;
    else
		return FALSE;
}

//=============================================================================
// Method		: CLogFile::GetLogFilePath
// Access		: private 
// Returns		: void
// Parameter	: CString & strPath
// Qualifier	:
// Last Update	: 2013/2/4 - 17:38
// Desc.		:
//=============================================================================
void CLogFile::GetLogFilePath( CString& strPath )
{
	SYSTEMTIME	timeLocal;
	GetLocalTime(&timeLocal);

	if (m_strSubDirName.IsEmpty())
		strPath.Format(_T("%s\\LOG\\%04d\\%02d"), m_strPath, timeLocal.wYear, timeLocal.wMonth, timeLocal.wDay);
	else
		strPath.Format(_T("%s\\LOG\\%s\\%04d\\%02d"), m_strPath, m_strSubDirName, timeLocal.wYear, timeLocal.wMonth, timeLocal.wDay);	
}

//=============================================================================
// Method		: CLogFile::Logfilename
// Access		: public 
// Returns		: CString
// Qualifier	:
// Last Update	: 2010/10/14 - 18:08
// Desc.		:
//=============================================================================
CString CLogFile::Logfilename()
{
    SYSTEMTIME	timeLocal;
    CString		strFilename;

    GetLocalTime(&timeLocal);


	switch (m_byFileDivType)
	{
	case 0:
		strFilename.Format(_T("\\%s%04d_%02d_%02d.%s"),m_strLogName, timeLocal.wYear, timeLocal.wMonth, timeLocal.wDay, m_strFileExt);
		break;

	case 1: // AM/PM
		break;

	case 2: // 시간별
		break;

	default:
		strFilename.Format(_T("\\%s%04d_%02d_%02d.%s"),m_strLogName, timeLocal.wYear, timeLocal.wMonth, timeLocal.wDay, m_strFileExt);
		break;
	}    

    return strFilename;
}

//=============================================================================
// Method		: CLogFile::CreateLogFolder
// Access		: public 
// Returns		: void
// Parameter	: CString strPath
// Qualifier	:
// Last Update	: 2010/10/14 - 18:08
// Desc.		:
//=============================================================================
void CLogFile::CreateLogFolder( CString strPath )
{
    if (_taccess(strPath, 0) != 0) 
    {
		BOOL	l_bExtractSucc = TRUE;
		int		iCnt = 0;
		CString l_strDirPath = _T("");
		CString l_strNowPath = _T("");

		while(l_bExtractSucc)
		{
			l_bExtractSucc   =  AfxExtractSubString(l_strNowPath, strPath, iCnt, '\\');
			l_strDirPath    +=	l_strNowPath + _T("\\");

			if(l_bExtractSucc && _taccess(l_strDirPath,0) != 0)
			{
				CreateDirectory(l_strDirPath,NULL);
			}
			iCnt++;
		}
    }
}

//=============================================================================
// Method		: CLogFile::GetTime
// Access		: public 
// Returns		: CString
// Qualifier	:
// Last Update	: 2010/10/14 - 18:08
// Desc.		:
//=============================================================================
CString CLogFile::GetTime()
{
    SYSTEMTIME	timeLocal;
    CString		strTime;

    GetLocalTime(&timeLocal);

    strTime.Format(_T("[%02d:%02d:%02d]"), timeLocal.wHour, timeLocal.wMinute, timeLocal.wSecond);

    return strTime;
}

//=============================================================================
// Method		: CLogFile::LogWrite
// Access		: public 
// Returns		: void
// Parameter	: LPCTSTR lpszMsg
// Parameter	: LPCTSTR lpszSubDir
// Qualifier	:
// Last Update	: 2010/10/14 - 18:33
// Desc.		:
//=============================================================================
void CLogFile::LogWrite( LPCTSTR lpszMsg )
{
	long nThreadId = ::GetCurrentThreadId();

	while(m_nThreadId != nThreadId)
	{		
		::InterlockedCompareExchange(&m_nThreadId, nThreadId, 0);
		if(m_nThreadId == nThreadId) 
			break;

		::Sleep(25);
	}

	CString strFolder;
	CString strFullPath;
	CString	strMsg = lpszMsg;

	GetLogFilePath(strFolder);
	strFullPath = strFolder + Logfilename();

	BOOL bOpen = FALSE;
	if(!FindLogFile())
	{
		CreateLogFolder(strFolder);
		
		bOpen = m_fileLog.Open(strFullPath, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyNone | CFile::typeBinary);
#ifdef UNICODE
		USHORT sUnicode = 0xfeff;
		m_fileLog.Write(&sUnicode, 2);
#endif
	}
	else
	{
		if (bOpen = m_fileLog.Open(strFullPath, CFile::modeRead | CFile::modeWrite | CFile::shareDenyNone | CFile::typeBinary))
			m_fileLog.SeekToEnd();
	}

	if (bOpen)
	{
		if (m_bUseWriteTime)
		{
			strMsg.Insert(0, GetTime() + _T(" -> "));			
		}
		//strMsg.Insert(strMsg.GetLength(), _T("\r\n"));

		m_fileLog.Write(strMsg, strMsg.GetLength() * sizeof(TCHAR));
		m_fileLog.Flush();
		m_fileLog.Close();
	}

	::InterlockedCompareExchange(&m_nThreadId, 0, ::GetCurrentThreadId());
}

//=============================================================================
// Method		: CLogFile::LogWriteWithHeader
// Access		: public 
// Returns		: void
// Parameter	: LPCTSTR lpszMsg
// Parameter	: LPCTSTR lpszHeader
// Qualifier	:
// Last Update	: 2010/12/21 - 17:03
// Desc.		: 파일이 없다면 헤더를 추가한다.
//=============================================================================
void CLogFile::LogWriteWithHeader( LPCTSTR lpszMsg, LPCTSTR lpszHeader /*= NULL*/ )
{
	long nThreadId = ::GetCurrentThreadId();

	while(m_nThreadId != nThreadId)
	{		
		::InterlockedCompareExchange(&m_nThreadId, nThreadId, 0);
		if(m_nThreadId == nThreadId) 
			break;

		::Sleep(25);
	}

	CString strFolder;
	CString strFullPath;
	CString	strMsg = lpszMsg;

	GetLogFilePath(strFolder);
	strFullPath = strFolder + Logfilename();

	BOOL bOpen = FALSE;
	if(!FindLogFile())
	{
		CreateLogFolder(strFolder);

		if (bOpen = m_fileLog.Open(strFullPath, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyNone | CFile::typeBinary))
		{
			// 헤더 기록
			if (NULL != lpszHeader)
			{
				CString strHeader;
				strHeader.Format(_T("%s\r\n"), lpszHeader);
				m_fileLog.Write(strHeader, strHeader.GetLength() * sizeof(TCHAR));
			}
		}
	}
	else
	{
		if (bOpen = m_fileLog.Open(strFullPath, CFile::modeRead | CFile::modeWrite | CFile::shareDenyNone | CFile::typeBinary))
		{
			m_fileLog.SeekToEnd();
		}
	}

	if (bOpen)
	{
		if (m_bUseWriteTime)
		{
			strMsg.Insert(0, GetTime() + _T(" "));			
		}
		strMsg.Insert(strMsg.GetLength(), _T("\r\n"));

		m_fileLog.Write(strMsg, strMsg.GetLength() * sizeof(TCHAR));
		m_fileLog.Flush();
		m_fileLog.Close();
	}

	::InterlockedCompareExchange(&m_nThreadId, 0, ::GetCurrentThreadId());
}

//=============================================================================
// Method		: CLogFile::SetLogName
// Access		: public 
// Returns		: void
// Parameter	: LPCTSTR lpszLogName
// Qualifier	:
// Last Update	: 2010/10/22 - 11:12
// Desc.		:
//=============================================================================
void CLogFile::SetLogName( LPCTSTR lpszLogName )
{
	m_strLogName = lpszLogName;
}

//=============================================================================
// Method		: CLogFile::SetSubDirName
// Access		: public 
// Returns		: void
// Parameter	: LPCTSTR lpszSubDirName
// Qualifier	:
// Last Update	: 2010/10/22 - 11:57
// Desc.		:
//=============================================================================
void CLogFile::SetSubDirName( LPCTSTR lpszSubDirName )
{
	m_strSubDirName = lpszSubDirName;
}

//=============================================================================
// Method		: CLogFile::SetFileDivType
// Access		: public 
// Returns		: void
// Parameter	: BYTE byDivType
// Qualifier	:
// Last Update	: 2010/10/22 - 11:23
// Desc.		:
//=============================================================================
void CLogFile::SetFileDivType( BYTE byDivType )
{
	m_byFileDivType = byDivType;
}

//=============================================================================
// Method		: CLogFile::GetFilePath
// Access		: public 
// Returns		: CString
// Qualifier	:
// Last Update	: 2010/12/21 - 17:00
// Desc.		:
//=============================================================================
CString CLogFile::GetFilePath()
{
	CString strFolder;
	GetLogFilePath(strFolder);

	CString strFullPath = strFolder + Logfilename();

	return strFullPath;
}

//=============================================================================
// Method		: CLogFile::SetFileExt
// Access		: public 
// Returns		: void
// Parameter	: LPCTSTR lpszExt
// Qualifier	:
// Last Update	: 2012/1/19 - 11:21
// Desc.		:
//=============================================================================
void CLogFile::SetFileExt( LPCTSTR lpszExt )
{
	if (NULL != lpszExt)
		m_strFileExt = lpszExt;
}