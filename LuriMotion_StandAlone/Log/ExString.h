#ifndef __X_STRING_H__
#define __X_STRING_H__

#if _MSC_VER > 1000
#pragma once
#endif

#include <wtypes.h>
#include <tchar.h>
#include <stdlib.h>

#define _ATL_USE_CSTRING_FLOAT
//#define _CV_MIN_CRT

#define E_DIR_BACK_SLASH (_T('\\'))

struct CExStringData
{
	long nRefs;     // reference count
	int nDataLength;
	int nAllocLength;
	// TCHAR data[nAllocLength]

	TCHAR* data()
	{ return (TCHAR*)(this + 1); }
};


class CExString
{
public:
	// Constructors
	CExString();
	CExString(const CExString& stringSrc);
	CExString(TCHAR ch, int nRepeat = 1);
	CExString(LPCSTR lpsz);
	CExString(LPCWSTR lpsz);
	CExString(LPCSTR lpch, int nLength);
	CExString(LPCWSTR lpch, int nLength);
	CExString(const unsigned char* psz);

	// Attributes & Operations
	// as an array of characters
	int GetLength() const;
	BOOL IsEmpty() const;
	void Empty();                       // free up the data

	TCHAR GetAt(int nIndex) const;      // 0 based
	TCHAR operator [](int nIndex) const; // same as GetAt
	void SetAt(int nIndex, TCHAR ch);
	operator LPCTSTR() const;           // as a C string

	// overloaded assignment
	const CExString& operator =(const CExString& stringSrc);
	const CExString& operator =(TCHAR ch);
#ifdef _UNICODE
	const CExString& operator =(char ch);
#else
	const CExString& operator =(WCHAR ch);
#endif
	const CExString& operator =(LPCSTR lpsz);
	const CExString& operator =(LPCWSTR lpsz);
	const CExString& operator =(const unsigned char* psz);

	// string concatenation
	const CExString& operator +=(const CExString& string);
	const CExString& operator +=(TCHAR ch);
#ifdef _UNICODE
	const CExString& operator +=(char ch);
#else
	const CExString& operator +=(WCHAR ch);
#endif
	const CExString& operator +=(LPCTSTR lpsz);

	friend CExString __stdcall operator +(const CExString& string1, const CExString& string2);
	friend CExString __stdcall operator +(const CExString& string, TCHAR ch);
	friend CExString __stdcall operator +(TCHAR ch, const CExString& string);
#ifdef _UNICODE
	friend CExString __stdcall operator +(const CExString& string, char ch);
	friend CExString __stdcall operator +(char ch, const CExString& string);
#else
	friend CExString __stdcall operator +(const CExString& string, WCHAR ch);
	friend CExString __stdcall operator +(WCHAR ch, const CExString& string);
#endif
	friend CExString __stdcall operator +(const CExString& string, LPCTSTR lpsz);
	friend CExString __stdcall operator +(LPCTSTR lpsz, const CExString& string);

	// string comparison
	int Compare(LPCTSTR lpsz) const;         // straight character
	int CompareNoCase(LPCTSTR lpsz) const;   // ignore case
	int Collate(LPCTSTR lpsz) const;         // NLS aware
	int CollateNoCase(LPCTSTR lpsz) const;   // ignore case

	// simple sub-string extraction
	CExString Mid(int nFirst, int nCount) const;
	CExString Mid(int nFirst) const;
	CExString Left(int nCount) const;
	CExString Left(TCHAR ch) const;
	CExString Right(int nCount) const;

	CExString SpanIncluding(LPCTSTR lpszCharSet) const;
	CExString SpanExcluding(LPCTSTR lpszCharSet) const;

	// upper/lower/reverse conversion
	void MakeUpper();
	void MakeLower();
	void MakeReverse();

	// trimming whitespace (either side)
	void TrimRight();
	void TrimLeft();

	// remove continuous occurrences of chTarget starting from right
	void TrimRight(TCHAR chTarget);
	// remove continuous occcurrences of characters in passed string,
	// starting from right
	void TrimRight(LPCTSTR lpszTargets);
	// remove continuous occurrences of chTarget starting from left
	void TrimLeft(TCHAR chTarget);
	// remove continuous occcurrences of characters in
	// passed string, starting from left
	void TrimLeft(LPCTSTR lpszTargets);

	// advanced manipulation
	// replace occurrences of chOld with chNew
	int Replace(TCHAR chOld, TCHAR chNew);
	// replace occurrences of substring lpszOld with lpszNew;
	// empty lpszNew removes instances of lpszOld
	int Replace(LPCTSTR lpszOld, LPCTSTR lpszNew);
	// remove occurrences of chRemove
	int Remove(TCHAR chRemove);
	// insert character at zero-based index; concatenates
	// if index is past end of string
	int Insert(int nIndex, TCHAR ch);
	// insert substring at zero-based index; concatenates
	// if index is past end of string
	int Insert(int nIndex, LPCTSTR pstr);
	// delete nCount characters starting at zero-based index
	int Delete(int nIndex, int nCount = 1);

	// searching (return starting index, or -1 if not found)
	// look for a single character match
	int Find(TCHAR ch) const;                     // like "C" strchr
	int ReverseFind(TCHAR ch) const;
	int Find(TCHAR ch, int nStart) const;         // starting at index
	int FindOneOf(LPCTSTR lpszCharSet) const;

	// look for a specific sub-string
	int Find(LPCTSTR lpszSub) const;        // like "C" strstr
	int Find(LPCTSTR lpszSub, int nStart) const;  // starting at index

	// Concatentation for non strings
	const CExString& Append(int n)
	{
		const int cchBuff = 12;
		TCHAR szBuffer[cchBuff];
		wsprintf(szBuffer,_T("%d"),n);
		ConcatInPlace(SafeStrlen(szBuffer), szBuffer);
		return *this;
	}

	// simple formatting
	BOOL __cdecl Format(LPCTSTR lpszFormat, ...);
	BOOL FormatV(LPCTSTR lpszFormat, va_list argList);

	// formatting for localization (uses FormatMessage API)
	BOOL __cdecl FormatMessage(LPCTSTR lpszFormat, ...);

#ifndef _UNICODE
	// ANSI <-> OEM support (convert string in place)
	void AnsiToOem();
	void OemToAnsi();
#endif

	// return a BSTR initialized with this CString's data
	BSTR AllocSysString() const;
	// reallocates the passed BSTR, copies content of this CString to it
	BSTR SetSysString(BSTR* pbstr) const;

	// Access to string implementation buffer as "C" character array
	LPTSTR GetBuffer(int nMinBufLength);
	void ReleaseBuffer(int nNewLength = -1);
	LPTSTR GetBufferSetLength(int nNewLength);
	void FreeExtra();

	// Use LockBuffer/UnlockBuffer to turn refcounting off
	LPTSTR LockBuffer();
	void UnlockBuffer();

	CExString ExtractLeft();
	CExString ExtractLeft(TCHAR ch);	

	BOOL LoadString( unsigned nID, HINSTANCE hResourceHandle );

	static int MbsToUcs( LPWSTR lpuszDst, LPCSTR lpcszSrc, int nLen );
	static int UcsToMbs( LPSTR lpszDst, LPCWSTR lpcuszSrc, int nLen );

	int AtoI();
	DWORD ToDword();
	double AtoF();
	BOOL ToDouble(double& dValue);

	BOOL SplitPath(CExString* pstrDrive=NULL, CExString* pstrDir=NULL, CExString* pstrName=NULL, CExString* pstrExt= NULL);
	BOOL SplitPath(LPTSTR pstrDrive, LPTSTR pstrDir, LPTSTR pstrName, LPTSTR pstrExt);

	BOOL ToCurrentDir();

	BOOL ConvertToShortPathName();
	BOOL IsOnNetwork();
	BOOL MakeModuleFileName( HANDLE hModule = NULL );
	BOOL MakeModuleDirectory( HANDLE hModule = NULL );
	BOOL MakePath( LPCTSTR lpcszDrive, LPCTSTR lpcszDir, LPCTSTR lpcszName, LPCTSTR lpcszExt );
	void TrimToDir();
	void TrimToFileName();
	void TrimToFileExt();
	void TrimToFileTitle();
	void ToPathName(LPCTSTR lpszFileName);
	void ToNumberString(DWORD dwValue);
	BOOL MakePathName(CExString& strDir, CExString& strFileName);
	BOOL MakeRelativePathName(LPCTSTR lpszFilePath, LPCTSTR lpszDirCur);

#ifdef UNICODE
	LPSTR GetParamStringA();
#else
	LPWSTR GetParamStringW();
#endif

	static int PASCAL SafeStrlen(LPCSTR lpsz);
	static int PASCAL SafeStrlen(LPCWSTR lpsz);

	// Implementation
public:
	~CExString();
	int GetAllocLength() const;

	static BOOL __stdcall _IsValidString(LPCWSTR lpsz, int nLength = -1)
	{
		if (lpsz == NULL)
			return FALSE;
#ifndef _WIN32_WCE
		return !::IsBadStringPtrW(lpsz, nLength);
#else // CE specific
		nLength;
		return TRUE;
#endif //_WIN32_WCE
	}

	static BOOL __stdcall _IsValidString(LPCSTR lpsz, int nLength = -1)
	{
		if (lpsz == NULL)
			return FALSE;
#ifndef _WIN32_WCE
		return !::IsBadStringPtrA(lpsz, nLength);
#else // CE specific
		nLength;
		return TRUE;
#endif //_WIN32_WCE
	}

protected:

	LPTSTR m_pchData;   // pointer to ref counted string data

	// implementation helpers
	CExStringData* GetData() const;
	void Init();
	void AllocCopy(CExString& dest, int nCopyLen, int nCopyIndex, int nExtraLen) const;
	BOOL AllocBuffer(int nLen);
	void AssignCopy(int nSrcLen, LPCTSTR lpszSrcData);
	BOOL ConcatCopy(int nSrc1Len, LPCTSTR lpszSrc1Data, int nSrc2Len, LPCTSTR lpszSrc2Data);
	void ConcatInPlace(int nSrcLen, LPCTSTR lpszSrcData);
	void CopyBeforeWrite();
	BOOL AllocBeforeWrite(int nLen);
	void Release();
	static void PASCAL Release(CExStringData* pData);

	// CExString conversion helpers
	static int __cdecl _wcstombsz(char* mbstr, const wchar_t* wcstr, size_t count)
	{
		if (count == 0 && mbstr != NULL)
			return 0;

		int result = ::WideCharToMultiByte(CP_ACP, 0, wcstr, -1, mbstr, (int)count, NULL, NULL);

		ASSERT(mbstr == NULL || result <= (int)count);
		if (result > 0)
			mbstr[result - 1] = 0;
		return result;
	}

	static int __cdecl _mbstowcsz(wchar_t* wcstr, const char* mbstr, size_t count)
	{
		if (count == 0 && wcstr != NULL)
			return 0;

		int result = ::MultiByteToWideChar(CP_ACP, 0, mbstr, -1, wcstr, (int)count);
		ASSERT(wcstr == NULL || result <= (int)count);
		if (result > 0)
			wcstr[result - 1] = 0;
		return result;
	}

	// Helpers to avoid CRT startup code
#ifdef _CV_MIN_CRT
	static TCHAR* _cstrchr(const TCHAR* p, TCHAR ch)
	{
		//strchr for '\0' should succeed
		while (*p != 0)
		{
			if (*p == ch)
				break;
			p = ::CharNext(p);
		}
		return (TCHAR*)((*p == ch) ? p : NULL);
	}

	static TCHAR* _cstrrchr(const TCHAR* p, TCHAR ch)
	{
		const TCHAR* lpsz = NULL;
		while (*p != 0)
		{
			if (*p == ch)
				lpsz = p;
			p = ::CharNext(p);
		}
		return (TCHAR*)lpsz;
	}

	static TCHAR* _cstrrev(TCHAR* pStr)
	{
		// Optimize NULL, zero-length, and single-char case.
		if ((pStr == NULL) || (pStr[0] == '\0') || (pStr[1] == '\0'))
			return pStr;

		TCHAR* p = pStr;

		while (*p != 0) 
		{
			TCHAR* pNext = ::CharNext(p);
			if (pNext > p + 1)
			{
				char p1 = *(char*)p;
				*(char*)p = *(char*)(p + 1);
				*(char*)(p + 1) = p1;
			}
			p = pNext;
		}

		p--;
		TCHAR* q = pStr;

		while (q < p)
		{
			TCHAR t = *q;
			*q = *p;
			*p = t;
			q++;
			p--;
		}
		return (TCHAR*)pStr;
	}

	static TCHAR* _cstrstr(const TCHAR* pStr, const TCHAR* pCharSet)
	{
		int nLen = lstrlen(pCharSet);
		if (nLen == 0)
			return (TCHAR*)pStr;

		const TCHAR* pRet = NULL;
		const TCHAR* pCur = pStr;
		while ((pCur = _cstrchr(pCur, *pCharSet)) != NULL)
		{
			if (memcmp(pCur, pCharSet, nLen * sizeof(TCHAR)) == 0)
			{
				pRet = pCur;
				break;
			}
			pCur = ::CharNext(pCur);
		}
		return (TCHAR*) pRet;
	}

	static int _cstrspn(const TCHAR* pStr, const TCHAR* pCharSet)
	{
		int nRet = 0;
		TCHAR* p = (TCHAR*)pStr;
		while (*p != 0)
		{
			TCHAR* pNext = ::CharNext(p);
			if (pNext > p + 1)
			{
				if (_cstrchr_db(pCharSet, *p, *(p + 1)) == NULL)
					break;
				nRet += 2;
			}
			else
			{
				if (_cstrchr(pCharSet, *p) == NULL)
					break;
				nRet++;
			}
			p = pNext;
		}
		return nRet;
	}

	static int _cstrcspn(const TCHAR* pStr, const TCHAR* pCharSet)
	{
		int nRet = 0;
		TCHAR* p = (TCHAR*)pStr;
		while (*p != 0)
		{
			TCHAR* pNext = ::CharNext(p);
			if (pNext > p + 1)
			{
				if (_cstrchr_db(pCharSet, *p, *(p + 1)) != NULL)
					break;
				nRet += 2;
			}
			else
			{
				if (_cstrchr(pCharSet, *p) != NULL)
					break;
				nRet++;
			}
			p = pNext;
		}
		return nRet;
	}

	static TCHAR* _cstrpbrk(const TCHAR* p, const TCHAR* lpszCharSet)
	{
		int n = _cstrcspn(p, lpszCharSet);
		return (p[n] != 0) ? (TCHAR*)&p[n] : NULL;
	}

	static int _cstrisdigit(TCHAR ch)
	{
		WORD type;
		GetStringTypeEx(GetSystemDefaultLCID(), CT_CTYPE1, &ch, 1, &type);
		return (type & C1_DIGIT) == C1_DIGIT;
	}

	static int _cstrisspace(TCHAR ch)
	{
		WORD type;
		GetStringTypeEx(GetSystemDefaultLCID(), CT_CTYPE1, &ch, 1, &type);
		return (type & C1_SPACE) == C1_SPACE;
	}

	static int _cstrcmp(const TCHAR* pstrOne, const TCHAR* pstrOther)
	{
		return lstrcmp(pstrOne, pstrOther);
	}

	static int _cstrcmpi(const TCHAR* pstrOne, const TCHAR* pstrOther)
	{
		return lstrcmpi(pstrOne, pstrOther);
	}

	static int _cstrcoll(const TCHAR* pstrOne, const TCHAR* pstrOther)
	{
		int nRet = CompareString(GetSystemDefaultLCID(), 0, pstrOne, -1, pstrOther, -1);
		ASSERT(nRet != 0);
		return nRet - 2;  // Convert to strcmp convention.  This really is documented.
	}

	static int _cstrcolli(const TCHAR* pstrOne, const TCHAR* pstrOther)
	{
		int nRet = CompareString(GetSystemDefaultLCID(), NORM_IGNORECASE, pstrOne, -1, pstrOther, -1);
		ASSERT(nRet != 0);
		return nRet - 2;  // Convert to strcmp convention.  This really is documented.
	}

	static int _cstrtoi(const TCHAR* nptr)
	{
		int c;              /* current char */
		int total;          /* current total */
		int sign;           /* if '-', then negative, otherwise positive */

		while ( _cstrisspace(*nptr) )
			++nptr;

		c = (int)(_TUCHAR)*nptr++;
		sign = c;           /* save sign indication */
		if (c == _T('-') || c == _T('+'))
			c = (int)(_TUCHAR)*nptr++;    /* skip sign */

		total = 0;

		while (_cstrisdigit((TCHAR)c)) {
			total = 10 * total + (c - '0');     /* accumulate digit */
			c = (int)(_TUCHAR)*nptr++;    /* get next char */
		}

		if (sign == '-')
			return -total;
		else
			return total;   /* return result, negated if necessary */
	}
#else //!_CV_MIN_CRT
	static TCHAR* _cstrchr(const TCHAR* p, TCHAR ch)
	{
		return (TCHAR*)_tcschr(p, ch);
	}

	static TCHAR* _cstrrchr(const TCHAR* p, TCHAR ch)
	{
		return (TCHAR*)_tcsrchr(p, ch);
	}

	static TCHAR* _cstrrev(TCHAR* pStr)
	{
		return (TCHAR*)_tcsrev(pStr);
	}

	static TCHAR* _cstrstr(const TCHAR* pStr, const TCHAR* pCharSet)
	{
		return (TCHAR*)_tcsstr(pStr, pCharSet);
	}

	static int _cstrspn(const TCHAR* pStr, const TCHAR* pCharSet)
	{
		return (int)_tcsspn(pStr, pCharSet);
	}

	static int _cstrcspn(const TCHAR* pStr, const TCHAR* pCharSet)
	{
		return (int)_tcscspn(pStr, pCharSet);
	}

	static TCHAR* _cstrpbrk(const TCHAR* p, const TCHAR* lpszCharSet)
	{
		return (TCHAR*)_tcspbrk(p, lpszCharSet);
	}

	static int _cstrisdigit(TCHAR ch)
	{
		return _istdigit(ch);
	}

	static int _cstrisspace(TCHAR ch)
	{
		return _istspace((_TUCHAR)ch);
	}

	static int _cstrcmp(const TCHAR* pstrOne, const TCHAR* pstrOther)
	{
		return _tcscmp(pstrOne, pstrOther);
	}

	static int _cstrcmpi(const TCHAR* pstrOne, const TCHAR* pstrOther)
	{
		return _tcsicmp(pstrOne, pstrOther);
	}

#ifndef _WIN32_WCE
	static int _cstrcoll(const TCHAR* pstrOne, const TCHAR* pstrOther)
	{
		return _tcscoll(pstrOne, pstrOther);
	}

	static int _cstrcolli(const TCHAR* pstrOne, const TCHAR* pstrOther)
	{
		return _tcsicoll(pstrOne, pstrOther);
	}
#endif //!_WIN32_WCE

	static int _cstrtoi(const TCHAR* nptr)
	{
		return _ttoi(nptr);
	}
#endif //!_CV_MIN_CRT

	static TCHAR* _cstrchr_db(const TCHAR* p, TCHAR ch1, TCHAR ch2)
	{
		const TCHAR* lpsz = NULL;
		while (*p != 0)
		{
			if (*p == ch1 && *(p + 1) == ch2)
			{
				lpsz = p;
				break;
			}
			p = ::CharNext(p);
		}
		return (TCHAR*)lpsz;
	}
};


// Compare helpers
bool __stdcall operator ==(const CExString& s1, const CExString& s2);
bool __stdcall operator ==(const CExString& s1, LPCTSTR s2);
bool __stdcall operator ==(LPCTSTR s1, const CExString& s2);
bool __stdcall operator !=(const CExString& s1, const CExString& s2);
bool __stdcall operator !=(const CExString& s1, LPCTSTR s2);
bool __stdcall operator !=(LPCTSTR s1, const CExString& s2);
bool __stdcall operator <(const CExString& s1, const CExString& s2);
bool __stdcall operator <(const CExString& s1, LPCTSTR s2);
bool __stdcall operator <(LPCTSTR s1, const CExString& s2);
bool __stdcall operator >(const CExString& s1, const CExString& s2);
bool __stdcall operator >(const CExString& s1, LPCTSTR s2);
bool __stdcall operator >(LPCTSTR s1, const CExString& s2);
bool __stdcall operator <=(const CExString& s1, const CExString& s2);
bool __stdcall operator <=(const CExString& s1, LPCTSTR s2);
bool __stdcall operator <=(LPCTSTR s1, const CExString& s2);
bool __stdcall operator >=(const CExString& s1, const CExString& s2);
bool __stdcall operator >=(const CExString& s1, LPCTSTR s2);
bool __stdcall operator >=(LPCTSTR s1, const CExString& s2);


///////////////////////////////////////////////////////////////////////////////
// CExString Implementation

inline CExStringData* CExString::GetData() const
{
	ASSERT(m_pchData != NULL);
	return ((CExStringData*)m_pchData) - 1;
}

extern LPCTSTR _atltmpPchNil;
#define efxEmptyString ((CExString&)*(CExString*)&_atltmpPchNil)

inline void CExString::Init()
{ m_pchData = efxEmptyString.m_pchData; }

inline CExString::CExString()
{ m_pchData = efxEmptyString.m_pchData; }

inline CExString::CExString(const unsigned char* lpsz)
{
	Init();
	*this = (LPCSTR)lpsz;
}

inline const CExString& CExString::operator =(const unsigned char* lpsz)
{
	*this = (LPCSTR)lpsz;
	return *this;
}

#ifdef _UNICODE
inline const CExString& CExString::operator +=(char ch)
{
	TCHAR tch;
	MultiByteToWideChar(CP_ACP, 0, &ch, 1, &tch, 2);
	*this += (TCHAR)tch;
	return *this;
}

inline const CExString& CExString::operator =(char ch)
{
	TCHAR tch;
	MultiByteToWideChar(CP_ACP, 0, &ch, 1, &tch, 2);
	*this = (TCHAR)tch;
	return *this;
}

inline CExString __stdcall operator +(const CExString& string, char ch)
{ 
	TCHAR tch;
	MultiByteToWideChar(CP_ACP, 0, &ch, 1, &tch, 2);
	return string + (TCHAR)tch; 
}

inline CExString __stdcall operator +(char ch, const CExString& string)
{ 
	TCHAR tch;
	MultiByteToWideChar(CP_ACP, 0, &ch, 1, &tch, 2);
	return (TCHAR)tch + string; 
}
#else //_UNICODE		// 오류날 가능성있는 코드!!!
inline const CExString& CExString::operator +=(WCHAR ch)
{
	TCHAR tch;
	WideCharToMultiByte(CP_ACP, 0, &ch, 2, &tch, 1, NULL, NULL)
	*this += (TCHAR)tch;
	return *this;
}

inline const CExString& CExString::operator =(WCHAR ch)
{
	TCHAR tch;
	WideCharToMultiByte(CP_ACP, 0, &ch, 2, &tch, 1, NULL, NULL)
	*this = (TCHAR)tch;
	return *this;
}

inline CExString __stdcall operator +(const CExString& string, WCHAR ch)
{ 
	TCHAR tch;
	WideCharToMultiByte(CP_ACP, 0, &ch, 2, &tch, 1, NULL, NULL)
	return string + (TCHAR)tch; 
}

inline CExString __stdcall operator +(WCHAR ch, const CExString& string)
{ 
	TCHAR tch;
	WideCharToMultiByte(CP_ACP, 0, &ch, 2, &tch, 1, NULL, NULL)
	return (TCHAR)tch + string; 
}
#endif	//!_UNICODE

inline int CExString::GetLength() const
{ return GetData()->nDataLength; }

inline int CExString::GetAllocLength() const
{ return GetData()->nAllocLength; }

inline BOOL CExString::IsEmpty() const
{ return GetData()->nDataLength == 0; }

inline CExString::operator LPCTSTR() const
{ return m_pchData; }

inline int PASCAL CExString::SafeStrlen( LPCSTR lpsz )
{ return (lpsz == NULL) ? 0 : (int)strlen(lpsz); }

inline int PASCAL CExString::SafeStrlen( LPCWSTR lpusz )
{ return (lpusz == NULL) ? 0 : (int)wcslen(lpusz); }

// CExString support (windows specific)
inline int CExString::Compare(LPCTSTR lpsz) const
{ return _cstrcmp(m_pchData, lpsz); }    // MBCS/Unicode aware

inline int CExString::CompareNoCase(LPCTSTR lpsz) const
{ return _cstrcmpi(m_pchData, lpsz); }   // MBCS/Unicode aware

// CExString::Collate is often slower than Compare but is MBSC/Unicode
//  aware as well as locale-sensitive with respect to sort order.
#ifndef _WIN32_WCE
inline int CExString::Collate(LPCTSTR lpsz) const
{ return _cstrcoll(m_pchData, lpsz); }   // locale sensitive

inline int CExString::CollateNoCase(LPCTSTR lpsz) const
{ return _cstrcolli(m_pchData, lpsz); }   // locale sensitive
#endif //!_WIN32_WCE

inline TCHAR CExString::GetAt(int nIndex) const
{
	ASSERT(nIndex >= 0);
	ASSERT(nIndex < GetData()->nDataLength);
	return m_pchData[nIndex];
}

inline TCHAR CExString::operator [](int nIndex) const
{
	// same as GetAt
	ASSERT(nIndex >= 0);
	ASSERT(nIndex < GetData()->nDataLength);
	return m_pchData[nIndex];
}

inline bool __stdcall operator ==(const CExString& s1, const CExString& s2)
{ return s1.Compare(s2) == 0; }

inline bool __stdcall operator ==(const CExString& s1, LPCTSTR s2)
{ return s1.Compare(s2) == 0; }

inline bool __stdcall operator ==(LPCTSTR s1, const CExString& s2)
{ return s2.Compare(s1) == 0; }

inline bool __stdcall operator !=(const CExString& s1, const CExString& s2)
{ return s1.Compare(s2) != 0; }

inline bool __stdcall operator !=(const CExString& s1, LPCTSTR s2)
{ return s1.Compare(s2) != 0; }

inline bool __stdcall operator !=(LPCTSTR s1, const CExString& s2)
{ return s2.Compare(s1) != 0; }

inline bool __stdcall operator <(const CExString& s1, const CExString& s2)
{ return s1.Compare(s2) < 0; }

inline bool __stdcall operator <(const CExString& s1, LPCTSTR s2)
{ return s1.Compare(s2) < 0; }

inline bool __stdcall operator <(LPCTSTR s1, const CExString& s2)
{ return s2.Compare(s1) > 0; }

inline bool __stdcall operator >(const CExString& s1, const CExString& s2)
{ return s1.Compare(s2) > 0; }

inline bool __stdcall operator >(const CExString& s1, LPCTSTR s2)
{ return s1.Compare(s2) > 0; }

inline bool __stdcall operator >(LPCTSTR s1, const CExString& s2)
{ return s2.Compare(s1) < 0; }

inline bool __stdcall operator <=(const CExString& s1, const CExString& s2)
{ return s1.Compare(s2) <= 0; }

inline bool __stdcall operator <=(const CExString& s1, LPCTSTR s2)
{ return s1.Compare(s2) <= 0; }

inline bool __stdcall operator <=(LPCTSTR s1, const CExString& s2)
{ return s2.Compare(s1) >= 0; }

inline bool __stdcall operator >=(const CExString& s1, const CExString& s2)
{ return s1.Compare(s2) >= 0; }

inline bool __stdcall operator >=(const CExString& s1, LPCTSTR s2)
{ return s1.Compare(s2) >= 0; }

inline bool __stdcall operator >=(LPCTSTR s1, const CExString& s2)
{ return s2.Compare(s1) <= 0; }

inline const CExString& CExString::operator =(LPCTSTR lpsz)
{
	ASSERT(lpsz == NULL || _IsValidString(lpsz));
	AssignCopy(SafeStrlen(lpsz), lpsz);
	return *this;
}

#ifdef _UNICODE
inline const CExString& CExString::operator =(LPCSTR lpsz)
{
	int nSrcLen = (lpsz != NULL) ? (int)strlen(lpsz) : 0;
	if (AllocBeforeWrite(nSrcLen))
	{
		_mbstowcsz(m_pchData, lpsz, nSrcLen + 1);
		ReleaseBuffer();
	}
	return *this;
}
#else //!_UNICODE
inline const CExString& CExString::operator =(LPCWSTR lpsz)
{
	int nSrcLen = (lpsz != NULL) ? (int)wcslen(lpsz) : 0;
	if (AllocBeforeWrite(nSrcLen * 2))
	{
		_wcstombsz(m_pchData, lpsz, (nSrcLen * 2) + 1);
		ReleaseBuffer();
	}
	return *this;
}
#endif  //!_UNICODE

inline CExString __stdcall operator +(const CExString& string1, const CExString& string2)
{
	CExString s;
	s.ConcatCopy(string1.GetData()->nDataLength, string1.m_pchData, string2.GetData()->nDataLength, string2.m_pchData);
	return s;
}

inline CExString __stdcall operator +(const CExString& string, LPCTSTR lpsz)
{
	ASSERT(lpsz == NULL || CExString::_IsValidString(lpsz));
	CExString s;
	s.ConcatCopy(string.GetData()->nDataLength, string.m_pchData, CExString::SafeStrlen(lpsz), lpsz);
	return s;
}

inline CExString __stdcall operator +(LPCTSTR lpsz, const CExString& string)
{
	ASSERT(lpsz == NULL || CExString::_IsValidString(lpsz));
	CExString s;
	s.ConcatCopy(CExString::SafeStrlen(lpsz), lpsz, string.GetData()->nDataLength, string.m_pchData);
	return s;
}

inline const CExString& CExString::operator +=(LPCTSTR lpsz)
{
	ASSERT(lpsz == NULL || _IsValidString(lpsz));
	ConcatInPlace(SafeStrlen(lpsz), lpsz);
	return *this;
}

inline const CExString& CExString::operator +=(TCHAR ch)
{
	ConcatInPlace(1, &ch);
	return *this;
}

inline const CExString& CExString::operator +=(const CExString& string)
{
	ConcatInPlace(string.GetData()->nDataLength, string.m_pchData);
	return *this;
}

inline const CExString& CExString::operator =(TCHAR ch)
{
	ASSERT(!_istlead(ch));    // can't set single lead byte
	AssignCopy(1, &ch);
	return *this;
}

inline CExString __stdcall operator +(const CExString& string1, TCHAR ch)
{
	CExString s;
	s.ConcatCopy(string1.GetData()->nDataLength, string1.m_pchData, 1, &ch);
	return s;
}

inline CExString __stdcall operator +(TCHAR ch, const CExString& string)
{
	CExString s;
	s.ConcatCopy(1, &ch, string.GetData()->nDataLength, string.m_pchData);
	return s;
}

#endif // __X_STRING_H__
