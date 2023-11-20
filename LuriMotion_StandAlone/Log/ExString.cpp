#include "../pch.h"
#include "ExString.h"

#include <malloc.h>
#include <stdio.h>

#include <oleauto.h>

typedef unsigned short UNICHAR;
#pragma warning(disable : 4996)
// Globals

// For an empty string, m_pchData will point here
// (note: avoids special case of checking for NULL m_pchData)
// empty string data (and locked)
extern _declspec(selectany) int rgInitData[] = { -1, 0, 0, 0 };
extern _declspec(selectany) CExStringData* _atltmpDataNil = (CExStringData*)&rgInitData;
_declspec(selectany) LPCTSTR _atltmpPchNil = (LPCTSTR)(((BYTE*)&rgInitData) + sizeof(CExStringData));

CExString::CExString(const CExString& stringSrc)
{
	ASSERT(stringSrc.GetData()->nRefs != 0);
	if (stringSrc.GetData()->nRefs >= 0)
	{
		ASSERT(stringSrc.GetData() != _atltmpDataNil);
		m_pchData = stringSrc.m_pchData;
		InterlockedIncrement(&GetData()->nRefs);
	}
	else
	{
		Init();
		*this = stringSrc.m_pchData;
	}
}

BOOL CExString::AllocBuffer(int nLen)
// always allocate one extra character for '\0' termination
// assumes [optimistically] that data length will equal allocation length
{
	ASSERT(nLen >= 0);
	ASSERT(nLen <= INT_MAX - 1);    // max size (enough room for 1 extra)

	if (nLen == 0)
	{
		Init();
	}
	else
	{
		CExStringData* pData = NULL;
		pData = (CExStringData*)new BYTE[sizeof(CExStringData) + (nLen + 1) * sizeof(TCHAR)];

		if (pData == NULL)
			return FALSE;

		pData->nRefs = 1;
		pData->data()[nLen] = '\0';
		pData->nDataLength = nLen;
		pData->nAllocLength = nLen;
		m_pchData = pData->data();
	}

	return TRUE;
}

void CExString::Release()
{
	if (GetData() != _atltmpDataNil)
	{
		ASSERT(GetData()->nRefs != 0);
		if (InterlockedDecrement(&GetData()->nRefs) <= 0)
			delete[] (BYTE*)GetData();
		Init();
	}
}

void PASCAL CExString::Release(CExStringData* pData)
{
	if (pData != _atltmpDataNil)
	{
		ASSERT(pData->nRefs != 0);
		if (InterlockedDecrement(&pData->nRefs) <= 0)
			delete[] (BYTE*)pData;
	}
}

void CExString::Empty()
{
	if (GetData()->nDataLength == 0)
		return;

	if (GetData()->nRefs >= 0)
		Release();
	else
		*this = _T("");

	ASSERT(GetData()->nDataLength == 0);
	ASSERT(GetData()->nRefs < 0 || GetData()->nAllocLength == 0);
}

void CExString::CopyBeforeWrite()
{
	if (GetData()->nRefs > 1)
	{
		CExStringData* pData = GetData();
		Release();
		if (AllocBuffer(pData->nDataLength))
			memcpy(m_pchData, pData->data(), (pData->nDataLength + 1) * sizeof(TCHAR));
	}
	ASSERT(GetData()->nRefs <= 1);
}

BOOL CExString::AllocBeforeWrite(int nLen)
{
	BOOL bRet = TRUE;
	if (GetData()->nRefs > 1 || nLen > GetData()->nAllocLength)
	{
		Release();
		bRet = AllocBuffer(nLen);
	}
	ASSERT(GetData()->nRefs <= 1);
	return bRet;
}

CExString::~CExString()
//  free any attached data
{
	if (GetData() != _atltmpDataNil)
	{
		if (InterlockedDecrement(&GetData()->nRefs) <= 0)
			delete[] (BYTE*)GetData();
	}
}

void CExString::AllocCopy(CExString& dest, int nCopyLen, int nCopyIndex,
						 int nExtraLen) const
{
	// will clone the data attached to this string
	// allocating 'nExtraLen' characters
	// Places results in uninitialized string 'dest'
	// Will copy the part or all of original data to start of new string

	int nNewLen = nCopyLen + nExtraLen;
	if (nNewLen == 0)
	{
		dest.Init();
	}
	else
	{
		if (dest.AllocBuffer(nNewLen))
			memcpy(dest.m_pchData, m_pchData + nCopyIndex, nCopyLen * sizeof(TCHAR));
	}
}

CExString::CExString(LPCTSTR lpsz)
{
	Init();
	int nLen = SafeStrlen(lpsz);
	if (nLen != 0)
	{
		if (AllocBuffer(nLen))
			memcpy(m_pchData, lpsz, nLen * sizeof(TCHAR));
	}
}

#ifdef _UNICODE
CExString::CExString(LPCSTR lpsz)
{
	Init();
	int nSrcLen = (lpsz != NULL) ? (int)strlen(lpsz) : 0;
	if (nSrcLen != 0)
	{
		if (AllocBuffer(nSrcLen))
		{
			_mbstowcsz(m_pchData, lpsz, nSrcLen + 1);
			ReleaseBuffer();
		}
	}
}
#else //_UNICODE
CExString::CExString(LPCWSTR lpsz)
{
	Init();
	int nSrcLen = (lpsz != NULL) ? (int)wcslen(lpsz) : 0;
	if (nSrcLen != 0)
	{
		if (AllocBuffer(nSrcLen * 2))
		{
			_wcstombsz(m_pchData, lpsz, (nSrcLen * 2) + 1);
			ReleaseBuffer();
		}
	}
}
#endif //!_UNICODE

// Assignment operators
//  All assign a new value to the string
//      (a) first see if the buffer is big enough
//      (b) if enough room, copy on top of old buffer, set size and type
//      (c) otherwise free old string data, and create a new one
//
//  All routines return the new string (but as a 'const CExString&' so that
//      assigning it again will cause a copy, eg: s1 = s2 = "hi there".
//

void CExString::AssignCopy(int nSrcLen, LPCTSTR lpszSrcData)
{
	if (AllocBeforeWrite(nSrcLen))
	{
		memcpy(m_pchData, lpszSrcData, nSrcLen * sizeof(TCHAR));
		GetData()->nDataLength = nSrcLen;
		m_pchData[nSrcLen] = '\0';
	}
}

const CExString& CExString::operator =(const CExString& stringSrc)
{
	if (m_pchData != stringSrc.m_pchData)
	{
		if ((GetData()->nRefs < 0 && GetData() != _atltmpDataNil) || stringSrc.GetData()->nRefs < 0)
		{
			// actual copy necessary since one of the strings is locked
			AssignCopy(stringSrc.GetData()->nDataLength, stringSrc.m_pchData);
		}
		else
		{
			// can just copy references around
			Release();
			ASSERT(stringSrc.GetData() != _atltmpDataNil);
			m_pchData = stringSrc.m_pchData;
			InterlockedIncrement(&GetData()->nRefs);
		}
	}
	return *this;
}

// Concatenation
// NOTE: "operator +" is done as friend functions for simplicity
//      There are three variants:
//          CExString + CExString
// and for ? = TCHAR, LPCTSTR
//          CExString + ?
//          ? + CExString

BOOL CExString::ConcatCopy(int nSrc1Len, LPCTSTR lpszSrc1Data,
						  int nSrc2Len, LPCTSTR lpszSrc2Data)
{
	// -- master concatenation routine
	// Concatenate two sources
	// -- assume that 'this' is a new CExString object

	BOOL bRet = TRUE;
	int nNewLen = nSrc1Len + nSrc2Len;
	if (nNewLen != 0)
	{
		bRet = AllocBuffer(nNewLen);
		if (bRet)
		{
			memcpy(m_pchData, lpszSrc1Data, nSrc1Len * sizeof(TCHAR));
			memcpy(m_pchData + nSrc1Len, lpszSrc2Data, nSrc2Len * sizeof(TCHAR));
		}
	}
	return bRet;
}

void CExString::ConcatInPlace(int nSrcLen, LPCTSTR lpszSrcData)
{
	//  -- the main routine for += operators

	// concatenating an empty string is a no-op!
	if (nSrcLen == 0)
		return;

	// if the buffer is too small, or we have a width mis-match, just
	//   allocate a new buffer (slow but sure)
	if (GetData()->nRefs > 1 || GetData()->nDataLength + nSrcLen > GetData()->nAllocLength)
	{
		// we have to grow the buffer, use the ConcatCopy routine
		CExStringData* pOldData = GetData();
		if (ConcatCopy(GetData()->nDataLength, m_pchData, nSrcLen, lpszSrcData))
		{
			ASSERT(pOldData != NULL);
			CExString::Release(pOldData);
		}
	}
	else
	{
		// fast concatenation when buffer big enough
		memcpy(m_pchData + GetData()->nDataLength, lpszSrcData, nSrcLen * sizeof(TCHAR));
		GetData()->nDataLength += nSrcLen;
		ASSERT(GetData()->nDataLength <= GetData()->nAllocLength);
		m_pchData[GetData()->nDataLength] = '\0';
	}
}

LPTSTR CExString::GetBuffer(int nMinBufLength)
{
	ASSERT(nMinBufLength >= 0);

	if (GetData()->nRefs > 1 || nMinBufLength > GetData()->nAllocLength)
	{
		// we have to grow the buffer
		CExStringData* pOldData = GetData();
		int nOldLen = GetData()->nDataLength;   // AllocBuffer will tromp it
		if (nMinBufLength < nOldLen)
			nMinBufLength = nOldLen;

		if (!AllocBuffer(nMinBufLength))
			return NULL;

		memcpy(m_pchData, pOldData->data(), (nOldLen + 1) * sizeof(TCHAR));
		GetData()->nDataLength = nOldLen;
		CExString::Release(pOldData);
	}
	ASSERT(GetData()->nRefs <= 1);

	// return a pointer to the character storage for this string
	ASSERT(m_pchData != NULL);
	return m_pchData;
}

void CExString::ReleaseBuffer(int nNewLength)
{
	CopyBeforeWrite();  // just in case GetBuffer was not called

	if (nNewLength == -1)
		nNewLength = lstrlen(m_pchData); // zero terminated

	ASSERT(nNewLength <= GetData()->nAllocLength);
	GetData()->nDataLength = nNewLength;
	m_pchData[nNewLength] = '\0';
}

LPTSTR CExString::GetBufferSetLength(int nNewLength)
{
	ASSERT(nNewLength >= 0);

	if (GetBuffer(nNewLength) == NULL)
		return NULL;

	GetData()->nDataLength = nNewLength;
	m_pchData[nNewLength] = '\0';
	return m_pchData;
}

void CExString::FreeExtra()
{
	ASSERT(GetData()->nDataLength <= GetData()->nAllocLength);
	if (GetData()->nDataLength != GetData()->nAllocLength)
	{
		CExStringData* pOldData = GetData();
		if (AllocBuffer(GetData()->nDataLength))
		{
			memcpy(m_pchData, pOldData->data(), pOldData->nDataLength * sizeof(TCHAR));
			ASSERT(m_pchData[GetData()->nDataLength] == '\0');
			CExString::Release(pOldData);
		}
	}
	ASSERT(GetData() != NULL);

}

LPTSTR CExString::LockBuffer()
{
	LPTSTR lpsz = GetBuffer(0);
	if (lpsz != NULL)
		GetData()->nRefs = -1;
	return lpsz;
}

void CExString::UnlockBuffer()
{
	ASSERT(GetData()->nRefs == -1);
	if (GetData() != _atltmpDataNil)
		GetData()->nRefs = 1;
}

int CExString::Find(TCHAR ch) const
{
	return Find(ch, 0);
}

int CExString::Find(TCHAR ch, int nStart) const
{
	int nLength = GetData()->nDataLength;
	if (nStart >= nLength)
		return -1;

	// find first single character
	LPTSTR lpsz = _cstrchr(m_pchData + nStart, (_TUCHAR)ch);

	// return -1 if not found and index otherwise
	return (lpsz == NULL) ? -1 : (int)(lpsz - m_pchData);
}

int CExString::FindOneOf(LPCTSTR lpszCharSet) const
{
	ASSERT(_IsValidString(lpszCharSet));
	LPTSTR lpsz = _cstrpbrk(m_pchData, lpszCharSet);
	return (lpsz == NULL) ? -1 : (int)(lpsz - m_pchData);
}

void CExString::MakeUpper()
{
	CopyBeforeWrite();
	CharUpper(m_pchData);
}

void CExString::MakeLower()
{
	CopyBeforeWrite();
	CharLower(m_pchData);
}

void CExString::MakeReverse()
{
	CopyBeforeWrite();
	_cstrrev(m_pchData);
}

void CExString::SetAt(int nIndex, TCHAR ch)
{
	ASSERT(nIndex >= 0);
	ASSERT(nIndex < GetData()->nDataLength);

	CopyBeforeWrite();
	m_pchData[nIndex] = ch;
}

#ifndef _UNICODE
void CExString::AnsiToOem()
{
	CopyBeforeWrite();
	::AnsiToOem(m_pchData, m_pchData);
}

void CExString::OemToAnsi()
{
	CopyBeforeWrite();
	::OemToAnsi(m_pchData, m_pchData);
}
#endif //_UNICODE

CExString::CExString(TCHAR ch, int nLength)
{
	ASSERT(!_istlead(ch));    // can't create a lead byte string
	Init();
	if (nLength >= 1)
	{
		if (AllocBuffer(nLength))
		{
#ifdef _UNICODE
			for (int i = 0; i < nLength; i++)
				m_pchData[i] = ch;
#else
			memset(m_pchData, ch, nLength);
#endif
		}
	}
}

CExString::CExString(LPCTSTR lpch, int nLength)
{
	Init();
	if (nLength != 0)
	{
		if (AllocBuffer(nLength))
			memcpy(m_pchData, lpch, nLength * sizeof(TCHAR));
	}
}

#ifdef _UNICODE
CExString::CExString(LPCSTR lpsz, int nLength)
{
	Init();
	if (nLength != 0)
	{
		if (AllocBuffer(nLength))
		{
			int n = ::MultiByteToWideChar(CP_ACP, 0, lpsz, nLength, m_pchData, nLength + 1);
			ReleaseBuffer((n >= 0) ? n : -1);
		}
	}
}
#else //_UNICODE
CExString::CExString(LPCWSTR lpsz, int nLength)
{
	Init();
	if (nLength != 0)
	{
		if (AllocBuffer(nLength * 2))
		{
			int n = ::WideCharToMultiByte(CP_ACP, 0, lpsz, nLength, m_pchData, (nLength * 2) + 1, NULL, NULL);
			ReleaseBuffer((n >= 0) ? n : -1);
		}
	}
}
#endif //!_UNICODE

CExString CExString::Mid(int nFirst) const
{
	return Mid(nFirst, GetData()->nDataLength - nFirst);
}

CExString CExString::Mid(int nFirst, int nCount) const
{
	// out-of-bounds requests return sensible things
	if (nFirst < 0)
		nFirst = 0;
	if (nCount < 0)
		nCount = 0;

	if (nFirst + nCount > GetData()->nDataLength)
		nCount = GetData()->nDataLength - nFirst;
	if (nFirst > GetData()->nDataLength)
		nCount = 0;

	CExString dest;
	AllocCopy(dest, nCount, nFirst, 0);
	return dest;
}

CExString CExString::Right(int nCount) const
{
	if (nCount < 0)
		nCount = 0;
	else if (nCount > GetData()->nDataLength)
		nCount = GetData()->nDataLength;

	CExString dest;
	AllocCopy(dest, nCount, GetData()->nDataLength-nCount, 0);
	return dest;
}

CExString CExString::Left(int nCount) const
{
	if (nCount < 0)
		nCount = 0;
	else if (nCount > GetData()->nDataLength)
		nCount = GetData()->nDataLength;

	CExString dest;
	AllocCopy(dest, nCount, 0, 0);
	return dest;
}

CExString CExString::Left(TCHAR ch) const
{
	if (IsEmpty()) return *this;

	CExString strMe = *this;

	strMe.TrimLeft();
	strMe.TrimRight();

	int nSpacePos=strMe.Find(ch);
	CExString strDest="";

	if (nSpacePos!=-1)
	{
		strDest=strMe.Left(nSpacePos);
	}

	return strDest;
}

// strspn equivalent
CExString CExString::SpanIncluding(LPCTSTR lpszCharSet) const
{
	ASSERT(_IsValidString(lpszCharSet));
	return Left(_cstrspn(m_pchData, lpszCharSet));
}

// strcspn equivalent
CExString CExString::SpanExcluding(LPCTSTR lpszCharSet) const
{
	ASSERT(_IsValidString(lpszCharSet));
	return Left(_cstrcspn(m_pchData, lpszCharSet));
}

int CExString::ReverseFind(TCHAR ch) const
{
	// find last single character
	LPTSTR lpsz = _cstrrchr(m_pchData, (_TUCHAR)ch);

	// return -1 if not found, distance from beginning otherwise
	return (lpsz == NULL) ? -1 : (int)(lpsz - m_pchData);
}

// find a sub-string (like strstr)
int CExString::Find(LPCTSTR lpszSub) const
{
	return Find(lpszSub, 0);
}

int CExString::Find(LPCTSTR lpszSub, int nStart) const
{
	ASSERT(_IsValidString(lpszSub));

	int nLength = GetData()->nDataLength;
	if (nStart > nLength)
		return -1;

	// find first matching substring
	LPTSTR lpsz = _cstrstr(m_pchData + nStart, lpszSub);

	// return -1 for not found, distance from beginning otherwise
	return (lpsz == NULL) ? -1 : (int)(lpsz - m_pchData);
}

BOOL CExString::FormatV(LPCTSTR lpszFormat, va_list argList)
{
	ASSERT(_IsValidString(lpszFormat));

	enum _FormatModifiers
	{
		FORCE_ANSI =	0x10000,
		FORCE_UNICODE =	0x20000,
		FORCE_INT64 =	0x40000
	};

	va_list argListSave = argList;

	// make a guess at the maximum length of the resulting string
	int nMaxLen = 0;
	for (LPCTSTR lpsz = lpszFormat; *lpsz != '\0'; lpsz = ::CharNext(lpsz))
	{
		// handle '%' character, but watch out for '%%'
		if (*lpsz != '%' || *(lpsz = ::CharNext(lpsz)) == '%')
		{
			// this is instead of _tclen()
#if !defined(_UNICODE) && defined(_MBCS)
			nMaxLen += (int)(::CharNext(lpsz) - lpsz);
#else
			nMaxLen++;
#endif
			continue;
		}

		int nItemLen = 0;

		// handle '%' character with format
		int nWidth = 0;
		for (; *lpsz != '\0'; lpsz = ::CharNext(lpsz))
		{
			// check for valid flags
			if (*lpsz == '#')
				nMaxLen += 2;   // for '0x'
			else if (*lpsz == '*')
				nWidth = va_arg(argList, int);
			else if (*lpsz == '-' || *lpsz == '+' || *lpsz == '0' || *lpsz == ' ')
				;
			else // hit non-flag character
				break;
		}
		// get width and skip it
		if (nWidth == 0)
		{
			// width indicated by
			nWidth = _cstrtoi(lpsz);
			for (; *lpsz != '\0' && _cstrisdigit(*lpsz); lpsz = ::CharNext(lpsz))
				;
		}
		ASSERT(nWidth >= 0);

		int nPrecision = 0;
		if (*lpsz == '.')
		{
			// skip past '.' separator (width.precision)
			lpsz = ::CharNext(lpsz);

			// get precision and skip it
			if (*lpsz == '*')
			{
				nPrecision = va_arg(argList, int);
				lpsz = ::CharNext(lpsz);
			}
			else
			{
				nPrecision = _cstrtoi(lpsz);
				for (; *lpsz != '\0' && _cstrisdigit(*lpsz); lpsz = ::CharNext(lpsz))
					;
			}
			ASSERT(nPrecision >= 0);
		}

		// should be on type modifier or specifier
		int nModifier = 0;
		if (lpsz[0] == _T('I') && lpsz[1] == _T('6') && lpsz[2] == _T('4'))
		{
			lpsz += 3;
			nModifier = FORCE_INT64;
		}
		else
		{
			switch (*lpsz)
			{
				// modifiers that affect size
			case 'h':
				nModifier = FORCE_ANSI;
				lpsz = ::CharNext(lpsz);
				break;
			case 'l':
				nModifier = FORCE_UNICODE;
				lpsz = ::CharNext(lpsz);
				break;

				// modifiers that do not affect size
			case 'F':
			case 'N':
			case 'L':
				lpsz = ::CharNext(lpsz);
				break;
			}
		}

		// now should be on specifier
		switch (*lpsz | nModifier)
		{
			// single characters
		case 'c':
		case 'C':
			nItemLen = 2;
			va_arg(argList, TCHAR);
			break;
		case 'c' | FORCE_ANSI:
		case 'C' | FORCE_ANSI:
			nItemLen = 2;
			va_arg(argList, char);
			break;
		case 'c' | FORCE_UNICODE:
		case 'C' | FORCE_UNICODE:
			nItemLen = 2;
			va_arg(argList, WCHAR);
			break;

			// strings
		case 's':
			{
				LPCTSTR pstrNextArg = va_arg(argList, LPCTSTR);
				if (pstrNextArg == NULL)
				{
					nItemLen = 6;  // "(null)"
				}
				else
				{
					nItemLen = lstrlen(pstrNextArg);
					nItemLen = max(1, nItemLen);
				}
				break;
			}

		case 'S':
			{
#ifndef _UNICODE
				LPWSTR pstrNextArg = va_arg(argList, LPWSTR);
				if (pstrNextArg == NULL)
				{
					nItemLen = 6;  // "(null)"
				}
				else
				{
					nItemLen = (int)wcslen(pstrNextArg);
					nItemLen = max(1, nItemLen);
				}
#else //_UNICODE
				LPCSTR pstrNextArg = va_arg(argList, LPCSTR);
				if (pstrNextArg == NULL)
				{
					nItemLen = 6; // "(null)"
				}
				else
				{
					nItemLen = (int)strlen(pstrNextArg);
					nItemLen = max(1, nItemLen);
				}
#endif //_UNICODE
				break;
			}

		case 's' | FORCE_ANSI:
		case 'S' | FORCE_ANSI:
			{
				LPCSTR pstrNextArg = va_arg(argList, LPCSTR);
				if (pstrNextArg == NULL)
				{
					nItemLen = 6; // "(null)"
				}
				else
				{
					nItemLen = (int)strlen(pstrNextArg);
					nItemLen = max(1, nItemLen);
				}
				break;
			}

		case 's' | FORCE_UNICODE:
		case 'S' | FORCE_UNICODE:
			{
				LPWSTR pstrNextArg = va_arg(argList, LPWSTR);
				if (pstrNextArg == NULL)
				{
					nItemLen = 6; // "(null)"
				}
				else
				{
					nItemLen = (int)wcslen(pstrNextArg);
					nItemLen = max(1, nItemLen);
				}
				break;
			}
		}

		// adjust nItemLen for strings
		if (nItemLen != 0)
		{
			nItemLen = max(nItemLen, nWidth);
			if (nPrecision != 0)
				nItemLen = min(nItemLen, nPrecision);
		}
		else
		{
			switch (*lpsz)
			{
				// integers
			case 'd':
			case 'i':
			case 'u':
			case 'x':
			case 'X':
			case 'o':
				if (nModifier & FORCE_INT64)
					va_arg(argList, __int64);
				else
					va_arg(argList, int);
				nItemLen = 32;
				nItemLen = max(nItemLen, nWidth + nPrecision);
				break;

#ifndef _ATL_USE_CSTRING_FLOAT
			case 'e':
			case 'E':
			case 'f':
			case 'g':
			case 'G':
				ASSERT(!"Floating point (%%e, %%E, %%f, %%g, and %%G) is not supported by the CExString class.");
#ifndef _DEBUG
				::OutputDebugString(_T("Floating point (%%e, %%f, %%g, and %%G) is not supported by the CExString class."));
#ifndef _WIN32_WCE
				::DebugBreak();
#else // CE specific
				DebugBreak();
#endif //_WIN32_WCE
#endif //!_DEBUG
				break;
#else //_ATL_USE_CSTRING_FLOAT
			case 'e':
			case 'E':
			case 'g':
			case 'G':
				va_arg(argList, double);
				nItemLen = 128;
				nItemLen = max(nItemLen, nWidth + nPrecision);
				break;
			case 'f':
				{
					double f;
					LPTSTR pszTemp;

					// 312 == strlen("-1+(309 zeroes).")
					// 309 zeroes == max precision of a double
					// 6 == adjustment in case precision is not specified,
					//   which means that the precision defaults to 6
					pszTemp = (LPTSTR)_alloca(max(nWidth, 312 + nPrecision + 6));

					f = va_arg(argList, double);
					_stprintf(pszTemp, _T( "%*.*f" ), nWidth, nPrecision + 6, f);
					nItemLen = (int)_tcslen(pszTemp);
				}
				break;
#endif //_ATL_USE_CSTRING_FLOAT

			case 'p':
				va_arg(argList, void*);
				nItemLen = 32;
				nItemLen = max(nItemLen, nWidth + nPrecision);
				break;

				// no output
			case 'n':
				va_arg(argList, int*);
				break;

			default:
				ASSERT(FALSE);  // unknown formatting option
				break;
			}
		}

		// adjust nMaxLen for output nItemLen
		nMaxLen += nItemLen;
	}

	if (GetBuffer(nMaxLen) == NULL)
		return FALSE;
#ifndef _ATL_USE_CSTRING_FLOAT
	int nRet = wvsprintf(m_pchData, lpszFormat, argListSave);
#else //_ATL_USE_CSTRING_FLOAT
	int nRet = _vstprintf(m_pchData, lpszFormat, argListSave);
#endif //_ATL_USE_CSTRING_FLOAT
	nRet;   // ref
	ASSERT(nRet <= GetAllocLength());
	ReleaseBuffer();

	va_end(argListSave);
	return TRUE;
}

// formatting (using wsprintf style formatting)
BOOL __cdecl CExString::Format(LPCTSTR lpszFormat, ...)
{
	ASSERT(_IsValidString(lpszFormat));

	va_list argList;
	va_start(argList, lpszFormat);
	BOOL bRet = FormatV(lpszFormat, argList);
	va_end(argList);
	return bRet;
}

// formatting (using FormatMessage style formatting)
BOOL __cdecl CExString::FormatMessage(LPCTSTR lpszFormat, ...)
{
	// format message into temporary buffer lpszTemp
	va_list argList;
	va_start(argList, lpszFormat);
	LPTSTR lpszTemp;
	BOOL bRet = TRUE;

	if (::FormatMessage(FORMAT_MESSAGE_FROM_STRING | FORMAT_MESSAGE_ALLOCATE_BUFFER,
		lpszFormat, 0, 0, (LPTSTR)&lpszTemp, 0, &argList) == 0 || lpszTemp == NULL)
		bRet = FALSE;

	// assign lpszTemp into the resulting string and free the temporary
	*this = lpszTemp;
	LocalFree(lpszTemp);
	va_end(argList);
	return bRet;
}

void CExString::TrimRight()
{
	CopyBeforeWrite();

	// find beginning of trailing spaces by starting at beginning (DBCS aware)
	LPTSTR lpsz = m_pchData;
	LPTSTR lpszLast = NULL;
	while (*lpsz != '\0')
	{
		if (_cstrisspace(*lpsz))
		{
			if (lpszLast == NULL)
				lpszLast = lpsz;
		}
		else
		{
			lpszLast = NULL;
		}
		lpsz = ::CharNext(lpsz);
	}

	if (lpszLast != NULL)
	{
		// truncate at trailing space start
		*lpszLast = '\0';
		GetData()->nDataLength = (int)(DWORD*)(lpszLast - m_pchData);
	}
}

void CExString::TrimLeft()
{
	CopyBeforeWrite();

	// find first non-space character
	LPCTSTR lpsz = m_pchData;
	while (_cstrisspace(*lpsz))
		lpsz = ::CharNext(lpsz);

	// fix up data and length
	int nDataLength = GetData()->nDataLength - (int)(DWORD*)(lpsz - m_pchData);
	memmove(m_pchData, lpsz, (nDataLength + 1) * sizeof(TCHAR));
	GetData()->nDataLength = nDataLength;
}

void CExString::TrimRight(LPCTSTR lpszTargetList)
{
	// find beginning of trailing matches
	// by starting at beginning (DBCS aware)

	CopyBeforeWrite();
	LPTSTR lpsz = m_pchData;
	LPTSTR lpszLast = NULL;

	while (*lpsz != '\0')
	{
		TCHAR* pNext = ::CharNext(lpsz);
		if (pNext > lpsz + 1)
		{
			if (_cstrchr_db(lpszTargetList, *lpsz, *(lpsz + 1)) != NULL)
			{
				if (lpszLast == NULL)
					lpszLast = lpsz;
			}
			else
			{
				lpszLast = NULL;
			}
		}
		else
		{
			if (_cstrchr(lpszTargetList, *lpsz) != NULL)
			{
				if (lpszLast == NULL)
					lpszLast = lpsz;
			}
			else
			{
				lpszLast = NULL;
			}
		}

		lpsz = pNext;
	}

	if (lpszLast != NULL)
	{
		// truncate at left-most matching character
		*lpszLast = '\0';
		GetData()->nDataLength = (int)(DWORD*)(lpszLast - m_pchData);
	}
}

void CExString::TrimRight(TCHAR chTarget)
{
	// find beginning of trailing matches
	// by starting at beginning (DBCS aware)

	CopyBeforeWrite();
	LPTSTR lpsz = m_pchData;
	LPTSTR lpszLast = NULL;

	while (*lpsz != '\0')
	{
		if (*lpsz == chTarget)
		{
			if (lpszLast == NULL)
				lpszLast = lpsz;
		}
		else
			lpszLast = NULL;
		lpsz = ::CharNext(lpsz);
	}

	if (lpszLast != NULL)
	{
		// truncate at left-most matching character
		*lpszLast = '\0';
		GetData()->nDataLength = (int)(DWORD*)(lpszLast - m_pchData);
	}
}

void CExString::TrimLeft(LPCTSTR lpszTargets)
{
	// if we're not trimming anything, we're not doing any work
	if (SafeStrlen(lpszTargets) == 0)
		return;

	CopyBeforeWrite();
	LPCTSTR lpsz = m_pchData;

	while (*lpsz != '\0')
	{
		TCHAR* pNext = ::CharNext(lpsz);
		if (pNext > lpsz + 1)
		{
			if (_cstrchr_db(lpszTargets, *lpsz, *(lpsz + 1)) == NULL)
				break;
		}
		else
		{
			if (_cstrchr(lpszTargets, *lpsz) == NULL)
				break;
		}
		lpsz = pNext;
	}

	if (lpsz != m_pchData)
	{
		// fix up data and length
		int nDataLength = GetData()->nDataLength - (int)(DWORD*)(lpsz - m_pchData);
		memmove(m_pchData, lpsz, (nDataLength + 1) * sizeof(TCHAR));
		GetData()->nDataLength = nDataLength;
	}
}

void CExString::TrimLeft(TCHAR chTarget)
{
	// find first non-matching character

	CopyBeforeWrite();
	LPCTSTR lpsz = m_pchData;

	while (chTarget == *lpsz)
		lpsz = ::CharNext(lpsz);

	if (lpsz != m_pchData)
	{
		// fix up data and length
		int nDataLength = GetData()->nDataLength - (int)(DWORD*)(lpsz - m_pchData);
		memmove(m_pchData, lpsz, (nDataLength + 1) * sizeof(TCHAR));
		GetData()->nDataLength = nDataLength;
	}
}

int CExString::Delete(int nIndex, int nCount /* = 1 */)
{
	if (nIndex < 0)
		nIndex = 0;
	int nLength = GetData()->nDataLength;
	if (nCount > 0 && nIndex < nLength)
	{
		if ((nIndex + nCount) > nLength)
			nCount = nLength - nIndex;
		CopyBeforeWrite();
		int nBytesToCopy = nLength - (nIndex + nCount) + 1;

		memmove(m_pchData + nIndex, m_pchData + nIndex + nCount, nBytesToCopy * sizeof(TCHAR));
		nLength -= nCount;
		GetData()->nDataLength = nLength;
	}

	return nLength;
}

int CExString::Insert(int nIndex, TCHAR ch)
{
	CopyBeforeWrite();

	if (nIndex < 0)
		nIndex = 0;

	int nNewLength = GetData()->nDataLength;
	if (nIndex > nNewLength)
		nIndex = nNewLength;
	nNewLength++;

	if (GetData()->nAllocLength < nNewLength)
	{
		CExStringData* pOldData = GetData();
		LPTSTR pstr = m_pchData;
		if (!AllocBuffer(nNewLength))
			return -1;
		memcpy(m_pchData, pstr, (pOldData->nDataLength + 1) * sizeof(TCHAR));
		CExString::Release(pOldData);
	}

	// move existing bytes down
	memmove(m_pchData + nIndex + 1, m_pchData + nIndex, (nNewLength - nIndex) * sizeof(TCHAR));
	m_pchData[nIndex] = ch;
	GetData()->nDataLength = nNewLength;

	return nNewLength;
}

int CExString::Insert(int nIndex, LPCTSTR pstr)
{
	if (nIndex < 0)
		nIndex = 0;

	int nInsertLength = SafeStrlen(pstr);
	int nNewLength = GetData()->nDataLength;
	if (nInsertLength > 0)
	{
		CopyBeforeWrite();
		if (nIndex > nNewLength)
			nIndex = nNewLength;
		nNewLength += nInsertLength;

		if (GetData()->nAllocLength < nNewLength)
		{
			CExStringData* pOldData = GetData();
			LPTSTR pstr = m_pchData;
			if (!AllocBuffer(nNewLength))
				return -1;
			memcpy(m_pchData, pstr, (pOldData->nDataLength + 1) * sizeof(TCHAR));
			CExString::Release(pOldData);
		}

		// move existing bytes down
		memmove(m_pchData + nIndex + nInsertLength, m_pchData + nIndex, (nNewLength - nIndex - nInsertLength + 1) * sizeof(TCHAR));
		memcpy(m_pchData + nIndex, pstr, nInsertLength * sizeof(TCHAR));
		GetData()->nDataLength = nNewLength;
	}

	return nNewLength;
}

int CExString::Replace(TCHAR chOld, TCHAR chNew)
{
	int nCount = 0;

	// short-circuit the nop case
	if (chOld != chNew)
	{
		// otherwise modify each character that matches in the string
		CopyBeforeWrite();
		LPTSTR psz = m_pchData;
		LPTSTR pszEnd = psz + GetData()->nDataLength;
		while (psz < pszEnd)
		{
			// replace instances of the specified character only
			if (*psz == chOld)
			{
				*psz = chNew;
				nCount++;
			}
			psz = ::CharNext(psz);
		}
	}
	return nCount;
}

int CExString::Replace(LPCTSTR lpszOld, LPCTSTR lpszNew)
{
	// can't have empty or NULL lpszOld

	int nSourceLen = SafeStrlen(lpszOld);
	if (nSourceLen == 0)
		return 0;
	int nReplacementLen = SafeStrlen(lpszNew);

	// loop once to figure out the size of the result string
	int nCount = 0;
	LPTSTR lpszStart = m_pchData;
	LPTSTR lpszEnd = m_pchData + GetData()->nDataLength;
	LPTSTR lpszTarget;
	while (lpszStart < lpszEnd)
	{
		while ((lpszTarget = _cstrstr(lpszStart, lpszOld)) != NULL)
		{
			nCount++;
			lpszStart = lpszTarget + nSourceLen;
		}
		lpszStart += lstrlen(lpszStart) + 1;
	}

	// if any changes were made, make them
	if (nCount > 0)
	{
		CopyBeforeWrite();

		// if the buffer is too small, just
		//   allocate a new buffer (slow but sure)
		int nOldLength = GetData()->nDataLength;
		int nNewLength =  nOldLength + (nReplacementLen - nSourceLen) * nCount;
		if (GetData()->nAllocLength < nNewLength || GetData()->nRefs > 1)
		{
			CExStringData* pOldData = GetData();
			LPTSTR pstr = m_pchData;
			if (!AllocBuffer(nNewLength))
				return -1;
			memcpy(m_pchData, pstr, pOldData->nDataLength * sizeof(TCHAR));
			CExString::Release(pOldData);
		}
		// else, we just do it in-place
		lpszStart = m_pchData;
		lpszEnd = m_pchData + GetData()->nDataLength;

		// loop again to actually do the work
		while (lpszStart < lpszEnd)
		{
			while ( (lpszTarget = _cstrstr(lpszStart, lpszOld)) != NULL)
			{
				int nBalance = nOldLength - ((int)(DWORD*)(lpszTarget - m_pchData) + nSourceLen);
				memmove(lpszTarget + nReplacementLen, lpszTarget + nSourceLen, nBalance * sizeof(TCHAR));
				memcpy(lpszTarget, lpszNew, nReplacementLen * sizeof(TCHAR));
				lpszStart = lpszTarget + nReplacementLen;
				lpszStart[nBalance] = '\0';
				nOldLength += (nReplacementLen - nSourceLen);
			}
			lpszStart += lstrlen(lpszStart) + 1;
		}
		ASSERT(m_pchData[nNewLength] == '\0');
		GetData()->nDataLength = nNewLength;
	}

	return nCount;
}

int CExString::Remove(TCHAR chRemove)
{
	CopyBeforeWrite();

	LPTSTR pstrSource = m_pchData;
	LPTSTR pstrDest = m_pchData;
	LPTSTR pstrEnd = m_pchData + GetData()->nDataLength;

	while (pstrSource < pstrEnd)
	{
		if (*pstrSource != chRemove)
		{
			*pstrDest = *pstrSource;
			pstrDest = ::CharNext(pstrDest);
		}
		pstrSource = ::CharNext(pstrSource);
	}
	*pstrDest = '\0';
	int nCount = (int)(DWORD*)(pstrSource - pstrDest);
	GetData()->nDataLength -= nCount;

	return nCount;
}

CExString CExString::ExtractLeft()
{
	Replace( _T('\t'), _T(' ') );
	TrimLeft();
	TrimRight();

	if ( IsEmpty() ) return *this;

	int nSpacePos=Find(_T(' '));
	CExString strDest;

	if (nSpacePos!=-1)
	{
		strDest=Left(nSpacePos);

		Delete(0, nSpacePos+1);
		TrimLeft();

		return strDest;
	}
	else
	{
		strDest=*this;
		Empty();
		return strDest;
	}

}

CExString CExString::ExtractLeft(TCHAR ch)
{
	TrimLeft();
	TrimRight();

	if (IsEmpty()) return *this;

	int nSpacePos=Find(ch);
	CExString strDest;

	if (nSpacePos!=-1)
	{
		strDest=Left(nSpacePos);

		Delete(0, nSpacePos+1);
		TrimLeft();

		return strDest;
	}
	else
	{
		strDest=*this;
		Empty();
		return strDest;
	}

}

BOOL CExString::LoadString( unsigned nID, HINSTANCE hResourceHandle )
{
	int nSize = 256;
	int nLen;
	do
	{
		nSize += 256;
		nLen = ::LoadString(hResourceHandle, nID, GetBuffer(nSize-1), nSize);
	} while ( nSize - nLen <= 1 );
	ReleaseBuffer();

	return nLen > 0;
}

int CExString::MbsToUcs( LPWSTR lpuszDst, LPCSTR lpcszSrc, int nLen )
{
	return _mbstowcsz(lpuszDst, lpcszSrc, nLen);
}

int CExString::UcsToMbs( LPSTR lpszDst, LPCWSTR lpcuszSrc, int nLen )
{
	return _wcstombsz(lpszDst, lpcuszSrc, nLen);
}

int CExString::AtoI()
{
	return _tcstol(*this, NULL, 0);
}

DWORD CExString::ToDword()
{
	return _tcstoul(*this, NULL, 0);
}

double CExString::AtoF()
{
	return _tcstod(*this, NULL);
}

BOOL CExString::ToDouble(double& dValue)
{
	dValue=AtoF();

	if (dValue==0.0)
	{
		Remove(_T('.'));
		TrimLeft();
		TrimLeft(_T('0'));

		if (IsEmpty()) return TRUE;
		else return FALSE;

	}
	else
	{
		return TRUE;
	}
}

#define E_MAX_DRIVE  3
#define E_MAX_DIR    256
#define E_MAX_FILE_NAME  E_MAX_DIR
#define E_MAX_EXT    E_MAX_DIR
#define E_MAX_PATH_NAME   (E_MAX_DIR+E_MAX_DRIVE+1)

BOOL CExString::SplitPath(CExString* pstrDrive, CExString* pstrDir, CExString* pstrName, CExString* pstrExt)
{
	_tsplitpath(
		operator LPCTSTR(),
		pstrDrive ? pstrDrive->GetBuffer(E_MAX_DRIVE) : NULL,
		pstrDir ? pstrDir->GetBuffer(E_MAX_DIR) : NULL,
		pstrName ? pstrName->GetBuffer(E_MAX_FILE_NAME) : NULL,
		pstrExt ? pstrExt->GetBuffer(E_MAX_EXT) : NULL);

	if (pstrDrive) pstrDrive->ReleaseBuffer();
	if (pstrDir) pstrDir->ReleaseBuffer();
	if (pstrName) pstrName->ReleaseBuffer();
	if (pstrExt) pstrExt->ReleaseBuffer();

	return TRUE;
}

BOOL CExString::SplitPath(LPTSTR pstrDrive, LPTSTR pstrDir, LPTSTR pstrName, LPTSTR pstrExt)
{
	_tsplitpath(operator LPCTSTR(), pstrDrive, pstrDir, pstrName, pstrExt);

	return TRUE;
}

BOOL CExString::ToCurrentDir()
{
	CopyBeforeWrite();

	LPTSTR pBuffer = GetBufferSetLength(MAX_PATH);
	DWORD dwLen = ::GetCurrentDirectory(MAX_PATH, pBuffer);
	if ( dwLen == 0 ) return FALSE;

	pBuffer[dwLen] = E_DIR_BACK_SLASH;
	pBuffer[dwLen+1] = 0;

	ReleaseBuffer( dwLen+1 );

	return TRUE;
}

BOOL CExString::ConvertToShortPathName()
{
	CExString strLongPath = *this;
	::GetShortPathName(strLongPath, GetBufferSetLength(MAX_PATH), MAX_PATH);
	ReleaseBuffer();
	return TRUE;
}

BOOL CExString::IsOnNetwork()
{
	LPCTSTR lpctstr = operator LPCTSTR();
	if ( GetLength() >= 2 && lpctstr[0] == E_DIR_BACK_SLASH && lpctstr[1] == E_DIR_BACK_SLASH )
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CExString::MakeModuleFileName( HANDLE hModule )
{
	CopyBeforeWrite();
	LPTSTR lpszThis = GetBuffer(MAX_PATH);
	DWORD dwLen = ::GetModuleFileName((HMODULE)hModule, lpszThis, MAX_PATH);
	ReleaseBuffer();
	return ( dwLen != 0 );
}

BOOL CExString::MakeModuleDirectory( HANDLE hModule )
{
	if ( !MakeModuleFileName(hModule) ) return FALSE;
	ASSERT( GetLength() < MAX_PATH );

	TrimToDir();
	return TRUE;
}

BOOL CExString::MakePath( LPCTSTR lpcszDrive, LPCTSTR lpcszDir, LPCTSTR lpcszName, LPCTSTR lpcszExt )
{
	TCHAR szPath[E_MAX_PATH_NAME];
	_tmakepath(szPath, lpcszDrive, lpcszDir, lpcszName, lpcszExt);
	*this = szPath;
	return TRUE;
}

void CExString::TrimToDir()
{
	if ( m_pchData == NULL ) return;

	CopyBeforeWrite();

	CExString strDrive;
	CExString strDir;

	SplitPath(&strDrive, &strDir, NULL, NULL);
	MakePath(strDrive, strDir, NULL, NULL);
}

void CExString::TrimToFileName()
{
	if ( m_pchData == NULL ) return;

	CopyBeforeWrite();

	CExString strName;
	CExString strExt;

	SplitPath(NULL, NULL, &strName, &strExt);
	MakePath(NULL, NULL, strName, strExt);

}

void CExString::TrimToFileTitle()
{
	if ( m_pchData == NULL ) return;

	CopyBeforeWrite();

	CExString strName;
	CExString strExt;

	SplitPath(NULL, NULL, &strName, &strExt);
	MakePath(NULL, NULL, strName, NULL);

}

void CExString::TrimToFileExt()
{
	if ( m_pchData == NULL ) return;

	CopyBeforeWrite();

	CExString strName;
	CExString strExt;

	SplitPath(NULL, NULL, &strName, &strExt);
	MakePath(NULL, NULL, NULL, strExt);

	TrimLeft(_T("."));
}

void CExString::ToPathName(LPCTSTR lpszFileName)
{
	if ( lpszFileName == NULL ) return;

	CopyBeforeWrite();
	int nLenThis = GetLength();
	int nLenSrc = SafeStrlen(lpszFileName);
	LPTSTR lpszDest = GetBuffer(nLenThis + nLenSrc + 1);
	lpszDest[nLenThis] = 0;
	int nDestTarget = nLenThis;
	LPCTSTR lpcszSrcTarget = lpszFileName;

	TCHAR szDrive[E_MAX_DRIVE];
	_tsplitpath(lpszFileName, szDrive, NULL, NULL, NULL);

	int nLength = SafeStrlen(szDrive);
	if ( nLength != 0 )
	{
		memcpy(lpszDest, szDrive, nLength);
		lpcszSrcTarget += nLength;
		lpszDest[nLength++] = E_DIR_BACK_SLASH;
		lpszDest[nLength] = 0;
		nDestTarget = nLength;
	}

	int i;

	while ( TRUE )
	{
		switch ( lpcszSrcTarget[0] )
		{
		case _T('.'):
			if ( lpcszSrcTarget[1] == _T('.') )
			{
				if ( lpcszSrcTarget[2] == _T('.') )
				{
					lpcszSrcTarget++;
					break;
				}

				if ( ( nDestTarget <= 0 || lpszDest[nDestTarget-1] != _T(':') ) &&
					( nDestTarget <= 1 || lpszDest[nDestTarget-2] != _T(':') ) )
				{
					for ( i = nDestTarget-2; i >= 0; i-- )
					{
						if ( lpszDest[i] == '\\' || lpszDest[i] == E_DIR_BACK_SLASH )
						{
							break;
						}
					}
					if ( nDestTarget <= 0 || (lpszDest[i+1]==_T('.') && lpszDest[i+2]==_T('.')) )
					{
						lpszDest[nDestTarget] = _T('.');
						lpszDest[nDestTarget+1] = _T('.');
						lpszDest[nDestTarget+2] = E_DIR_BACK_SLASH;
						nDestTarget += 3;
					}
					else
					{
						nDestTarget = i + 1;
					}
					lpszDest[nDestTarget] = 0;
				}
				lpcszSrcTarget += 2;
			}
			else
			{
				lpcszSrcTarget++;
			}
			break;

		case _T('\\'):
		case _T('/'):
			if ( lpcszSrcTarget[1] == _T('\\') )
			{
				_tsplitpath(lpszDest, szDrive, NULL, NULL, NULL);
				nDestTarget = SafeStrlen(szDrive);
				if ( nDestTarget )
				{
					lpszDest[nDestTarget] = 0;
				}
				else
				{
					lpszDest[0] = _T('\\');
					lpszDest[1] = _T('\\');
					lpszDest[2] = 0;
					nDestTarget = 2;
				}
				lpcszSrcTarget += 2;
			}
			else
			{
				lpcszSrcTarget++;
			}
			break;

		default:
			i = 0;
			while ( lpcszSrcTarget[i] != 0 &&
				lpcszSrcTarget[i] != _T('\\') && lpcszSrcTarget[i] != '/' )
			{
				i++;
			}

			if ( nDestTarget != 0 &&
				lpszDest[nDestTarget-1] != _T('\\') && lpszDest[nDestTarget-1] != '/' )
			{
				lpszDest[nDestTarget] = E_DIR_BACK_SLASH;
				nDestTarget++;
			}
			memcpy(lpszDest+nDestTarget, lpcszSrcTarget, i*sizeof(TCHAR));
			nDestTarget += i;
			lpszDest[nDestTarget] = 0;
			lpcszSrcTarget += i;
			break;
		}

		if ( lpcszSrcTarget[0] == 0 ) break;
	}

	ReleaseBuffer();
}

BOOL CExString::MakePathName(CExString& strDir, CExString& strFileName)
{
	CExString pathDir=strDir;

	if (pathDir.IsEmpty()) pathDir.ToCurrentDir();

	CExString strDrive;

	if (!strFileName.IsEmpty())
	{
		strFileName.SplitPath(&strDrive, NULL, NULL, NULL);
	}

	if (strDrive.IsEmpty())
	{
		operator=(pathDir);
		ToPathName(strFileName);
		return TRUE;
	}
	else
	{
		operator=(strFileName);
		return TRUE;
	}
}

/*
BOOL CExString::MakeRelativePathName(CExString& strFilePath, CExString& strDirCur, int nUpperDir)
{
	if (strDirCur.IsEmpty())
	{
		strDirCur.ToCurrentDir();
	}

	TCHAR szFileDrive[_MAX_DRIVE];
	TCHAR szFileDir[_MAX_DIR];

	strFilePath.SplitPath(szFileDrive, szFileDir, NULL, NULL);

	if ( szFileDrive[0] == 0 && ( szFileDir[0] != _T('\\') || szFileDir[1] != _T('\\') ) )
	{
ret_fullfilepath:
		operator=((LPCTSTR)strFilePath);
		return TRUE;
	}
	int ni = 0;
	int nj = SafeStrlen(szFileDrive);
	while ( strFilePath[nj] != 0 )
	{
		szFileDir[ni] = strFilePath[nj];
		ni++;
		nj++;
	}
	szFileDir[ni] = 0;

	TCHAR szDrive[_MAX_DRIVE];
	strDirCur.SplitPath(szDrive, NULL, NULL, NULL);

	if ( _tcsicmp(szFileDrive, szDrive) != 0 )
	{
		goto ret_fullfilepath;
	}
	ni = 0;
	nj = SafeStrlen(szDrive);
	TCHAR szDir[_MAX_DIR];
	while ( strDirCur[nj] != 0 )
	{
		szDir[ni] = strDirCur[nj];
		++ni;
		++nj;
	}
	szDir[ni] = 0;

	TCHAR* pszPointerFileDir = szFileDir;
	TCHAR* pszPointerFileDir2;
	TCHAR* pPdFile;

	pPdFile = pszPointerFileDir;
	TCHAR * pszPointerCurDir = szDir, *pszPointerCurDir2, *pPdCur;
	pPdCur = pszPointerCurDir;
	TCHAR pCmpFile[2];
	pCmpFile[1] = 0;
	TCHAR pCmpCur[2];
	pCmpCur[1] = 0;
	while ( TRUE )
	{
		pszPointerFileDir2 = pszPointerFileDir++;
		char cCharSizeFile = pszPointerFileDir2 - pszPointerFileDir;
		pszPointerCurDir2 = pszPointerCurDir++;
		char cCharSizeCur = pszPointerCurDir2 - pszPointerCurDir;

		if ( cCharSizeFile != cCharSizeCur ) break;
		if ( cCharSizeFile == 1 )
		{
			if ( ( pszPointerFileDir[0] == 0 && (pszPointerCurDir[0]==0 || pszPointerCurDir[0]==_T('\\')) ) ||
				( pszPointerCurDir[0] == 0 && (pszPointerFileDir[0]==0 || pszPointerFileDir[0]==_T('\\')) ) )
			{
				pPdFile = pszPointerFileDir;
				pPdCur = pszPointerCurDir;
				break;
			}

			pCmpFile[0] = pszPointerFileDir[0];
			pCmpCur[0] = pszPointerCurDir[0];
			if ( _tcsicmp(pCmpFile, pCmpCur) != 0 )
			{
				break;
			}

			if ( pszPointerFileDir[0] == _T('\\') )
			{
				pPdFile = pszPointerFileDir2;
				pPdCur = pszPointerCurDir2;
			}
		}
		else
		{
			if ( pszPointerFileDir[0] != pszPointerCurDir[0] ||
				pszPointerFileDir[1] != pszPointerCurDir[1] )
			{
				break;
			}
		}

		pszPointerFileDir = pszPointerFileDir2;
		pszPointerCurDir = pszPointerCurDir2;
	}

	int UpperDirCount = 0;
	if ( pPdCur[0] == _T('\\') ) pPdCur++;
	pszPointerCurDir = pPdCur;
	pszPointerCurDir2 = pszPointerCurDir;
	while ( TRUE )
	{
		if ( pszPointerCurDir[0] == 0 )
		{
			if ( pszPointerCurDir != pszPointerCurDir2 )
			{
				UpperDirCount++;
			}
			break;
		}
		if ( pszPointerCurDir[0] == _T('\\') )
		{
			UpperDirCount++;
			if ( pszPointerCurDir[1] == 0 ) break;
		}
		setntstrinc(pszPointerCurDir);
	}

	if ( UpperDirCount > nUpperDir )
	{
		operator=((LPCTSTR)strFilePath);
		return FALSE;
	}
	Empty();
	for ( ni = 0; ni < UpperDirCount; ni++ )
	{
		operator+=(_T("..\\"));
	}
	operator+=((LPCTSTR)pPdFile);

	return TRUE;
}
*/


BOOL CExString::MakeRelativePathName(LPCTSTR lpszFilePath, LPCTSTR lpszDirCur)
{
	//	TCHAR szRelativeFilename[MAX_PATH+MAX_PATH];
	CExString strDirCur = lpszDirCur;
	CExString strFilePath = lpszFilePath;

	if ( strDirCur.IsEmpty() )
	{
		strDirCur.ToCurrentDir();
	}

	//-- simple checks
	if (strFilePath.IsEmpty())
	{
		Empty();
		return FALSE;
	}

	int cdLen = strDirCur.GetLength();
	int afLen = strFilePath.GetLength();

	TCHAR* pszDirCur = (TCHAR*)(LPCTSTR)strDirCur;
	TCHAR* pszFilePath = (TCHAR*)(LPCTSTR)strFilePath;

	//-- handle DOS names that are on different drives:
	if ( _tcsnicmp( pszDirCur, pszFilePath, 1 ) != 0 )
	{
		//-- not on the same drive, so only absolute filename will do
		operator=((LPCTSTR)strFilePath);
		return FALSE;
	}

	//-- they are on the same drive, find out how much of the current directory
	//-- is in the absolute filename
	int i = 0;

	while (i < afLen && i < cdLen && _tcsnicmp( pszDirCur+i, pszFilePath+i, 1 ) == 0 )
	{
		i++;
	}

#define SLASH	_T('\\')

	if (i == cdLen && (*(pszDirCur+i) == SLASH ||
		*(pszFilePath+i-1) == SLASH))
	{
		//-- the whole current directory name is in the file name,
		//-- so we just trim off the current directory name to get the
		//-- current file name.
		if (*(pszFilePath+i) == SLASH)
		{
			//-- a directory name might have a trailing slash but a relative
			//-- file name should not have a leading one...
			i++;
		}

		operator=((LPCTSTR)(pszFilePath+i));
		return TRUE;
	}

	// The file is not in a child directory of the current directory, so we
	// need to step back the appropriate number of parent directories by
	// using "..\"s.  First find out how many levels deeper we are than	the
	// common directory
	int afMarker = i;
	int levels = 1;

	// count the number of directory levels we have to go up to get to the
	// common directory
	while (i < cdLen)
	{
		i++;
		if (*(pszDirCur+i) == SLASH)
		{
			//-- make sure it's not a trailing slash
			i++;
			if (*(pszDirCur+i) != _T('\0'))
			{
				levels++;
			}
		}
	}

	//-- move the absolute filename marker back to the start of the	directory name
	//-- that it has stopped in.
	while (afMarker > 0 && *(pszFilePath+afMarker-1) != SLASH)
	{
		afMarker--;
	}

	//-- add the appropriate number of "..\"s.
	Empty();
	for (i = 0; i < levels; i++)
	{
		operator+=((LPCTSTR)_T(".."));
		operator+=((TCHAR)SLASH);
	}

	//-- copy the rest of the filename into the result string
	operator+=((LPCTSTR) (pszFilePath+afMarker));
	return TRUE;
} 

#ifdef UNICODE
LPSTR CExString::GetParamStringA()
{
	if ( IsEmpty() ) return NULL;

	LPCWSTR lpwcszStr = m_pchData;

	LPSTR pszStrTemp = new CHAR[GetLength() + 1];
	_wcstombsz(pszStrTemp, lpwcszStr, GetLength() + 1);

	return pszStrTemp;
}

#else //UNICODE

LPWSTR CExString::GetParamStringW()
{
	if ( IsEmpty() ) return NULL;

	LPCSTR lpmbcszStr = m_pchData;

	LPWSTR pszStrTemp;
	pszStrTemp = new WCHAR[GetLength() + 1];
	_mbstowcsz(pszStrTemp, lpmbcszStr, GetLength() + 1);

	return pszStrTemp;
}
#endif //UNICODE

void CExString::ToNumberString(DWORD dwValue)
{
	CExString strTmp;
	strTmp.Format(_T("%d"), dwValue);
	
	CExString strRet;
	int nLen = strTmp.GetLength();
	if (nLen > 0)
	{
		strRet = strTmp[nLen-1];
		
		for (int ni = 1; ni < nLen; ni++)
		{
			if ( (ni%3) == 0 ) strRet = ',' + strRet;
			strRet = strTmp[nLen-ni-1] + strRet;
		}
	}

	(*this)=strRet;
}

BSTR CExString::AllocSysString() const
{
#if defined(_UNICODE) || defined(OLE2ANSI)
	BSTR bstr = ::SysAllocStringLen(m_pchData, GetData()->nDataLength);
	if (bstr == NULL)
	{
		VERIFY(FALSE);
	}
#else
	int nLen = MultiByteToWideChar(CP_ACP, 0, m_pchData,
		GetData()->nDataLength, NULL, NULL);
	BSTR bstr = ::SysAllocStringLen(NULL, nLen);
	if (bstr == NULL)
	{
		VERIFY(FALSE);
	}
	MultiByteToWideChar(CP_ACP, 0, m_pchData, GetData()->nDataLength,
		bstr, nLen);
#endif

	return bstr;
}

BSTR CExString::SetSysString(BSTR* pbstr) const
{
	//ASSERT(AfxIsValidAddress(pbstr, sizeof(BSTR)));

#if defined(_UNICODE) || defined(OLE2ANSI)
	if (!::SysReAllocStringLen(pbstr, m_pchData, GetData()->nDataLength))
	{
		VERIFY( FALSE );
	}
#else
	int nLen = MultiByteToWideChar(CP_ACP, 0, m_pchData,
		GetData()->nDataLength, NULL, NULL);
	if (!::SysReAllocStringLen(pbstr, NULL, nLen))
	{
		VERIFY( FALSE );
	}
	MultiByteToWideChar(CP_ACP, 0, m_pchData, GetData()->nDataLength,
		*pbstr, nLen);
#endif

	ASSERT(*pbstr != NULL);
	return *pbstr;
}