#include "StdAfx.h"
#include "atoken.h"

CAToken::CAToken(void)
: m_szData(NULL)
, m_szDelimiter(NULL)
{
	m_arToken.RemoveAll();
}

CAToken::~CAToken(void)
{
	if (m_szData != NULL)
	{
		delete [] m_szData;
		m_szData = NULL;
	}

	if (m_szDelimiter != NULL)
	{
		delete [] m_szDelimiter;
		m_szDelimiter = NULL;
	}

	m_arToken.RemoveAll();
}

bool CAToken::Init()
{
	return true;
}

bool CAToken::InitToken()
{
	m_bTokenized = false;
	m_ItemCount  = 0;
	m_arToken.RemoveAll();

	return true;
}

bool CAToken::SetData(CString strData)
{
	if (m_szData != NULL)
	{
		delete [] m_szData;
		m_szData = NULL;
	}

	int BufferLength = strData.GetLength() + 1;
	m_szData = new TCHAR[ BufferLength ];
	memset(m_szData, 0, sizeof(TCHAR) * BufferLength );

	_tcscpy_s( m_szData, BufferLength, strData.GetBuffer( strData.GetLength() ) );
	strData.ReleaseBuffer();

	InitToken();

	return true;
}

bool CAToken::SetData(const TCHAR* szData)
{
	if (m_szData != NULL)
	{
		delete [] m_szData;
		m_szData = NULL;
	}

	int BufferLength = static_cast<int>( _tcslen( szData ) + 1 );
	m_szData = new TCHAR[ BufferLength ];
	memset( m_szData, 0, sizeof(TCHAR) * BufferLength );

	_tcscpy_s( m_szData, BufferLength, szData );

	InitToken();

	return true;
}

bool CAToken::SetDelimiter(const TCHAR* szDelimiter)
{
	if (m_szDelimiter != NULL)
	{
		delete [] m_szDelimiter;
		m_szDelimiter = NULL;
	}

	int BufferLength = static_cast<int>( _tcslen( szDelimiter ) + 1 );
	m_szDelimiter = new TCHAR[ BufferLength ];
	memset(m_szDelimiter, 0, sizeof(TCHAR) * BufferLength );

	_tcscpy_s( m_szDelimiter, BufferLength, szDelimiter );

	return true;
}

int CAToken::DoTokenize()
{
	if (m_szData == NULL)
		return -1;

	if (m_szDelimiter == NULL)
		return -2;

	TCHAR *next_token;
	TCHAR *token = _tcstok_s( m_szData, m_szDelimiter, &next_token );

	m_arToken.RemoveAll();
	m_ItemCount = 0;

	while(token != NULL)
	{
		m_ItemCount++;
		m_arToken.Add(token);
		token = _tcstok_s( NULL, m_szDelimiter, &next_token );
	}

	return m_ItemCount;
}

int CAToken::DoTokenizeEx()
{
	if (m_szData == NULL)
		return -1;

	if (m_szDelimiter == NULL)
		return -2;

	CString	strNextToken;
	CString	strToken;
	
	strNextToken = m_szData;

	m_arToken.RemoveAll();
	m_ItemCount = 0;

	BOOL bReturn = FALSE;

	bReturn = TokenizeEx (strNextToken, strToken);
	
	while(bReturn != FALSE)
	{
		m_ItemCount++;
		m_arToken.Add(strToken);		

		bReturn = TokenizeEx (strNextToken, strToken);
	}	

	return m_ItemCount;
}

void CAToken::GetItem(int index, CString& strItem)
{
	if (index < 0 || index > m_ItemCount)
		return;

	strItem = m_arToken.GetAt(index);
}

CString CAToken::GetItem(int index)
{
	if (index < 0 || index > m_ItemCount)
		return _T("");

	return m_arToken.GetAt(index);
}