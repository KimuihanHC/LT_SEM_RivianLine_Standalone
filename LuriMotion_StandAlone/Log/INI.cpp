// INI.cpp: implementation of the CINI class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "../pch.h"
#include "../LuriMotion_OQA_SFR_WideDlg.h"
#include "INI.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define MAX_SIZE 256


CINI::CINI() 
{	
}

CINI::~CINI() 
{
//	m_file.Close();
}


CINI::CConvert CINI::SetKey(CString nSession,CString nKey)
{
	m_convert.m_session = nSession;
	m_convert.m_key = nKey;	
	return m_convert;	
}

CINI::CConvert CINI::GetKey(CString nSession,CString nKey)
{
	return SetKey(nSession,nKey);
}


BOOL CINI::SetSession(CString nValue)
{
	return WritePrivateProfileSection(nValue,NULL,m_convert.m_FileName);
}

CINI::CConvert CINI::GetSession(CString nSession)
{
	m_convert.m_session = nSession;
	m_convert.m_key.Empty();	
	return m_convert;	
}

BOOL CINI::Open(LPCTSTR lpszFileName,BOOL m_create)
{	
	if(!m_file.Open( lpszFileName, CFile::modeNoTruncate , NULL ) )
	{		
		if (m_create)
		{			
			m_file.Open( lpszFileName, CFile::modeCreate,NULL );
		}
		else
		{			
			return FALSE;
		}
	}
	
	m_convert.m_FileName = lpszFileName;
	m_file.Close();
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//  Class Name : CConvert 
/////////////////////////////////////////////////////////////////////////////
int CINI::CConvert::toInterger()
{
	return GetPrivateProfileInt(m_session,m_key,-1,m_FileName);
}

CString CINI::CConvert::toString()
{	
	TCHAR Temp[MAX_SIZE]={0};
	CString rValue;
	int pos = 0 ;
	GetPrivateProfileString(m_session,m_key,_T(""),Temp,MAX_SIZE,m_FileName);		
	rValue = Temp;
	while(1)
	{
		pos = rValue.Find('_') ;
		if(pos != -1)
		{
			rValue.Replace('_',' ') ;
		}
		else
			break ;
	}
//	rValue.TrimLeft();
//	rValue.TrimRight();
	return rValue;
}

BOOL CINI::CConvert::toBoolean()
{
	if (toString() == _T("true"))	return TRUE;
	return FALSE;
}

BOOL CINI::CConvert::addString(CString nValue)
{	
	if (!m_FileName.IsEmpty())
	{		
		return  WritePrivateProfileString(m_session,m_key,nValue,m_FileName);
	}
	return FALSE;
}

BOOL CINI::CConvert::addInterger(int nValue)
{
	CString Temp;
	Temp.Format(_T("%d"),nValue);
	return addString(Temp);	
}

BOOL CINI::CConvert::addBoolean(BOOL nValue)
{
	CString Temp;
	Temp = "false";
	if (nValue) Temp = _T("true");
	return addString(Temp);	
}

BOOL CINI::CConvert::Delete()
{	
	if (m_key.IsEmpty()) return WritePrivateProfileString(m_session,NULL,NULL,m_FileName);	
	return WritePrivateProfileString(m_session,m_key,NULL,m_FileName);
}






