#include "../pch.h"

#include "LogBarcode.h"

#include "../CommonFunction.h"

#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>

#pragma warning(disable : 4996)

//////////////////////////////////////////////////////////////////////
// CLogSeq
//////////////////////////////////////////////////////////////////////

CLogBarcode::CLogBarcode( LPCTSTR lpszServiceName ) : CLogBase(lpszServiceName)
{
}

CLogBarcode::~CLogBarcode()
{
	if (m_bStart)
		Stop();
}

BOOL CLogBarcode::OpenLogFile( CExString& strLogPathName )
{
	if ( !m_bUseTxtFile )
		return TRUE;

	USES_CONVERSION;

	if ( !m_File.Open(strLogPathName, CFile::modeRead|CFile::shareDenyWrite) )
	{
		if ( !m_File.Open(strLogPathName, CFile::modeCreate|CFile::modeWrite) )
			return FALSE;

		CTime CurTime = CTime::GetCurrentTime();
		CExString strTime;
		strTime.Format( _T("%04d/%02d/%02d"), CurTime.GetYear(), CurTime.GetMonth(), CurTime.GetDay() );
		CExString strComment;
		strComment.Format( _T("#################### Luritech Barcode Log ####################\r\n")
						   _T("- Created by Barcode\r\n")
						   _T("- Service Name: %s\r\n")
						   _T("- Date: %s\r\n")
						   _T("- SW Ver : %s.%s \r\n")
						   _T("#########################################################\r\n")
			, m_strServiceName, strTime, GetVersionInfo(_T("ProductVersion")), GetVersionInfo(_T("FileVersion")));
		m_File.Write( T2A( (LPTSTR)(LPCTSTR)strComment ), strComment.GetLength() );
	}
	m_File.Close();
	
	if ( !m_File.Open(strLogPathName, CFile::modeWrite|CFile::shareDenyWrite) )
		return FALSE;

	m_File.SeekToEnd();

	return TRUE;
}

void  CLogBarcode::Init()
{
	CString strTime;

	SYSTEMTIME tm;
	GetLocalTime(&tm);
	strTime.Format(_T("[%02d:%02d:%02d.%03d]"), tm.wHour, tm.wMinute, tm.wSecond, tm.wMilliseconds);

	CString str;
	str += _T("****************************** Start Program ******************************************\r\n");

	str.Format(_T("\r\n**************************************** Start Program *****************************\r\n")
		_T("- S/W Ver : %s.%s \r\n")
		_T("- Equipment : %s \r\n")
		_T("**************************************************************************************")
		, GetVersionInfo(_T("ProductVersion")), GetVersionInfo(_T("FileVersion")), g_szInsptrSysType[GET_INSPECTOR]);
	LogOut(strTime, str);
}
