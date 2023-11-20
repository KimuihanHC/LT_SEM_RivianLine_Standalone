// INI.h: interface for the CINI class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INI_H__588C64E3_D253_4FAF_8791_A0B4AF87F797__INCLUDED_)
#define AFX_INI_H__588C64E3_D253_4FAF_8791_A0B4AF87F797__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CINI 
{
public:
	class CConvert 
	{
	public :
		BOOL addBoolean(BOOL nValue);
		BOOL addInterger(int nValue);
		BOOL addString(CString nValue);
		CString toString(void);
		int toInterger(void);
		BOOL toBoolean(void);
		BOOL Delete(void);
		friend class CINI;
	private :
		CString m_value;
		CString m_key;
		CString m_session;
		CString m_FileName;
	};

	CINI();
	virtual ~CINI();
	BOOL SetSession(CString nValue);
	BOOL Open(LPCTSTR lpszFileName,BOOL m_create = NULL);
	CConvert SetKey(CString nSession,CString nKey);	
	CConvert GetKey(CString nSession,CString nKey);
	CConvert GetSession(CString nSession);

private :
	CConvert m_convert;
	CFile m_file;

};

#endif // !defined(AFX_INI_H__588C64E3_D253_4FAF_8791_A0B4AF87F797__INCLUDED_)
