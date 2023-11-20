// Registry.cpp: implementation of the CRegistry class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Registry.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRegistry::CRegistry(HKEY hKey)
{
	ASSERT(hKey);

	m_hKey = hKey;

  pKeyName = NULL;
  pValueName = NULL;
  nKeyIndex = 0;
  nValIndex = 0;

}

CRegistry::~CRegistry()
{
	Close();

	if (NULL != pKeyName)
		delete [] pKeyName;

	if (NULL != pValueName)
		delete [] pValueName;
}

BOOL CRegistry::Open(HKEY hKey, LPCTSTR lpszPath)
{
	ASSERT(hKey);
	ASSERT(lpszPath);

	m_sPath = lpszPath;

	LONG lResult = RegOpenKeyEx(hKey, lpszPath, NULL, KEY_ALL_ACCESS, &m_hKey);

	if(lResult == ERROR_SUCCESS)
		return TRUE;

	return FALSE;
}

BOOL CRegistry::Open(LPCTSTR lpszPath)
{
	ASSERT(lpszPath);

	m_sPath = lpszPath;

	LONG lResult = RegOpenKeyEx(m_hKey, lpszPath, NULL, KEY_ALL_ACCESS, &m_hKey);

	if(lResult == ERROR_SUCCESS)
		return TRUE;

	return FALSE;
}

void CRegistry::Close()
{
	if (m_hKey)
	{
		RegCloseKey(m_hKey);
		m_hKey = NULL;
	}
}

BOOL CRegistry::VerifyKey(HKEY hKey, LPCTSTR lpszPath)
{
	ASSERT(hKey);
	ASSERT(lpszPath);

	LONG lResult = RegOpenKeyEx(hKey, lpszPath, NULL, KEY_ALL_ACCESS, &m_hKey);

	if(lResult == ERROR_SUCCESS)
		return TRUE;
	
	return FALSE;
}

BOOL CRegistry::VerifyKey(LPCTSTR lpszPath)
{
	ASSERT(lpszPath);

	LONG lResult = RegOpenKeyEx(m_hKey, lpszPath, NULL, KEY_ALL_ACCESS, &m_hKey);
	
	if(lResult == ERROR_SUCCESS)
		return TRUE;
	
	return FALSE;
}

BOOL CRegistry::VerifyValue(LPCTSTR lpszValue)
{
	ASSERT(lpszValue);

	LONG lResult = RegQueryValueEx(m_hKey, lpszValue, NULL, NULL, NULL, NULL);

	if(lResult == ERROR_SUCCESS)
		return TRUE;

	return FALSE;
}

BOOL CRegistry::CreateKey(HKEY hKey, LPCTSTR lpszPath, DWORD dwOptions)
{
	ASSERT(hKey);
	ASSERT(lpszPath);

	DWORD dwDisp = -1;

	LONG lResult = RegCreateKeyEx(hKey, lpszPath, NULL, NULL, dwOptions, KEY_ALL_ACCESS, NULL, &m_hKey, &dwDisp);

	if(lResult == ERROR_SUCCESS)
		return TRUE;

	return FALSE;
}

BOOL CRegistry::DeleteKey(HKEY hKey, LPCTSTR lpszPath)
{
	ASSERT(hKey);
	ASSERT(lpszPath);

	LONG lResult = RegDeleteKey(hKey, lpszPath);

	if(lResult == ERROR_SUCCESS)
		return TRUE;

	return FALSE;
}

BOOL CRegistry::DeleteValue(LPCTSTR lpszValue)
{
	ASSERT(lpszValue);

	LONG lResult = RegDeleteValue(m_hKey, lpszValue);

	if(lResult == ERROR_SUCCESS)
		return TRUE;

	return FALSE;
}

BOOL CRegistry::WriteString(LPCTSTR lpszKey, LPCTSTR lpszData)
{
	ASSERT(lpszKey);
	ASSERT(lpszData);

	USES_CONVERSION;
	CStringA strData;
	strData = CT2A(lpszData);

	//LONG_PTR lResult = RegSetValueEx(m_hKey, lpszKey, NULL, REG_SZ, (const BYTE*) lpszData, (DWORD)_tcslen(lpszData) + 1);
	LONG_PTR lResult = RegSetValueEx(m_hKey, lpszKey, NULL, REG_SZ, (const BYTE*)strData.GetBuffer(), (DWORD)strData.GetLength() + 1);
	strData.ReleaseBuffer();

	if(lResult == ERROR_SUCCESS)
		return TRUE;
	
	return FALSE;
}

BOOL CRegistry::ReadString(LPCTSTR lpszKey, CString &sData)
{
	ASSERT(lpszKey);

	DWORD dwType = -1;
	DWORD dwSize = -1;

	LONG lResult = RegQueryValueEx(m_hKey, (LPTSTR) lpszKey, NULL, &dwType, NULL, &dwSize);

	if(lResult == ERROR_SUCCESS && dwType == REG_SZ)
	{
		UCHAR* pszData = new UCHAR[dwSize];
		lResult = RegQueryValueEx(m_hKey, (LPTSTR) lpszKey, NULL, &dwType, pszData, &dwSize);
		if (lResult == ERROR_SUCCESS && dwType == REG_SZ)
		{
			sData = pszData;
			delete []pszData;
			return TRUE;
		}
		delete []pszData;
		return TRUE;
	}
	else
		return FALSE;
}

BOOL CRegistry::WriteDWORD(LPCTSTR lpszKey, const DWORD &dwData)
{
	ASSERT(lpszKey);
	
	LONG lResult = RegSetValueEx(m_hKey, lpszKey, NULL, REG_DWORD, (const BYTE*) &dwData, sizeof(DWORD));

	if(lResult == ERROR_SUCCESS)
		return TRUE;

	return FALSE;
}

BOOL CRegistry::ReadDWORD(LPCTSTR lpszKey, DWORD &dwData)
{
	ASSERT(lpszKey);

	DWORD dwType = -1;
	DWORD dwSize = sizeof(DWORD);

	LONG lResult = RegQueryValueEx(m_hKey, (LPTSTR) lpszKey, NULL, &dwType, (BYTE*) &dwData, &dwSize);

	if(lResult == ERROR_SUCCESS && dwType == REG_DWORD)
		return TRUE;

	return FALSE;
}

BOOL CRegistry::Write(LPCTSTR lpszKey, CWnd *pWnd)
{
	ASSERT(lpszKey);
	ASSERT(pWnd);

	WINDOWPLACEMENT wp;
	wp.length = sizeof(WINDOWPLACEMENT);
	pWnd->GetWindowPlacement(&wp);

	CMemFile file;
	CArchive ar(&file, CArchive::store);

	ar << wp.flags
		 << wp.showCmd
		 << wp.ptMinPosition
		 << wp.ptMaxPosition
		 << wp.rcNormalPosition;
	ar.Close();

	DWORD dwSize = (DWORD)file.GetLength();
	BYTE* pByte = file.Detach();

	LONG lResult = RegSetValueEx(m_hKey, lpszKey, NULL, REG_BINARY, pByte, dwSize);

	if (pByte)
		free(pByte);

	if(lResult == ERROR_SUCCESS)
		return TRUE;
	
	return FALSE;
}

BOOL CRegistry::Read(LPCTSTR lpszKey, CWnd* pWnd)
{
	ASSERT(lpszKey);
	ASSERT(pWnd);

	DWORD dwType;
	DWORD dwSize = sizeof(WINDOWPLACEMENT);
	UCHAR* pszData = new UCHAR[dwSize];

	WINDOWPLACEMENT wp;
	wp.length = dwSize;

	LONG lResult = RegQueryValueEx(m_hKey, lpszKey, NULL, &dwType, (BYTE*) pszData, &dwSize);

	if(lResult == ERROR_SUCCESS && dwType == REG_BINARY)
	{
		CMemFile file(pszData, dwSize);
		CArchive ar(&file, CArchive::load);

		ar >> wp.flags
			 >> wp.showCmd
			 >> wp.ptMinPosition
			 >> wp.ptMaxPosition
			 >> wp.rcNormalPosition;
		ar.Close();

		file.Close();
		
		pWnd->SetWindowPlacement(&wp);

		delete []pszData;
		return TRUE;
	}

	delete []pszData;
	return FALSE;
}

BOOL CRegistry::Write(LPCTSTR lpszKey, CObject& object)
{
	ASSERT(lpszKey);
	
	CMemFile file;
	CArchive ar(&file, CArchive::store);
	
	object.Serialize(ar);
	ar.Close();
	
	DWORD dwSize = (DWORD)file.GetLength();
	#ifdef _DEBUG
		if (dwSize > 2048)
			TRACE1("CRegistry::Write - Object size = %d (> 2048) better use a file !\n", dwSize);
	#endif
	BYTE* pByte = file.Detach();
	
	LONG lResult = RegSetValueEx(m_hKey, lpszKey, NULL, REG_BINARY, pByte, dwSize);
	
	if (pByte)
		free(pByte);
	
	if(lResult == ERROR_SUCCESS)
		return TRUE;
	
	return FALSE;
}

BOOL CRegistry::Read(LPCTSTR lpszKey, CObject& object)
{
	ASSERT(lpszKey);
	
	DWORD dwType = -1;
	DWORD dwSize = -1;
	UCHAR *pszData = NULL;
	
	LONG lResult = RegQueryValueEx(m_hKey, lpszKey, NULL, &dwType, (BYTE*) pszData, &dwSize);
	
	if (lResult == ERROR_SUCCESS && dwType == REG_BINARY)
		pszData = new UCHAR[dwSize];

	lResult = RegQueryValueEx(m_hKey, lpszKey, NULL, &dwType, (BYTE*) pszData, &dwSize);
	if (lResult == ERROR_SUCCESS && dwType == REG_BINARY)
	{
		CMemFile file(pszData, dwSize);
		CArchive ar(&file, CArchive::load);
		
		object.Serialize(ar);
		ar.Close();
			
		file.Close();
			
		delete []pszData;
		return TRUE;
	}

	delete []pszData;
	return FALSE;
}


LPCTSTR CRegistry::enumKeys(int nIndex)
{
  // Enumerate all the subkeys for the current key, returning a
  // pointer to the name string. Return null when no more.

//  if (bReset)
	if(nIndex<0)
		nIndex=0;

    nKeyIndex = nIndex;

  // Query the current key to find the maximum subkey size
  queryInfo();

  // Allocate a buffer to hold largest subkey name
	if (NULL != pKeyName)
	{
		delete [] pKeyName;
		pKeyName = NULL;
	}

  pKeyName = new TCHAR[maxSubkeyLength+1];
  DWORD bufsize = maxSubkeyLength+1;
  FILETIME lastWriteTime;

  // get the subkey info... don't worry about class name or file time
  LONG err = RegEnumKeyEx(m_hKey, nKeyIndex,
    pKeyName, &bufsize, NULL, NULL, NULL, &lastWriteTime);
  SetLastError(err);

  if (err == ERROR_SUCCESS)
    return pKeyName;
  else
    return NULL;
}

LPCTSTR CRegistry::enumValues(LPDWORD pdwType, LPBYTE pData, 
                             LPDWORD pSize, bool bReset)
{
  // Enumerate all the values for the current key, returning a
  // pointer to the name string. Return null when no more.

  if (bReset)
    nValIndex = 0;

  // Query the current key to find the maximum value size
  queryInfo();

  // Allocate a buffer to hold largest value name
	if (NULL != pValueName)
	{
		delete [] pValueName;
		pValueName = NULL;
	}

  pValueName = new TCHAR[maxValueNameLength+1];

  // get the value info... 
  DWORD bufsize = maxValueNameLength+1;
  LONG err = RegEnumValue(m_hKey, nValIndex++,
    pValueName, &bufsize, NULL, pdwType, pData, pSize);
  SetLastError(err);

  if (err == ERROR_SUCCESS)
    return pValueName;
  else
    return NULL;
}


bool CRegistry::queryInfo()
{
  TCHAR tc[REG_BUFF_SIZE+1];
  DWORD bufsize = sizeof tc;
   
  LONG err = RegQueryInfoKey(m_hKey, tc, &bufsize,
                  static_cast<LPDWORD>(NULL), &numSubkeys, &maxSubkeyLength,
                  &maxClassLength, &numValues,
                  &maxValueNameLength, &maxValueDataLength,
                  &SDLength, &lastWriteTime);

  if (err == ERROR_SUCCESS)
  {
    _tcscpy(className, tc);   // save class name away
    SetLastError(ERROR_SUCCESS);
    return true;
  }
  else
  {
    SetLastError(err);
    return false;
  }
}
