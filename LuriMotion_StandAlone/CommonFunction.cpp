//*****************************************************************************
// Filename	: 	CommonFunction.cpp
//
// Created	:	2010/01/15
// Modified	:	2010/02/24 - 10:29
//
// Author	:	PiRing
//	
// Purpose	:	여러가지 기능이 있는 함수들 모음
//*****************************************************************************
#include "pch.h"
#include "strsafe.h"
#include "CommonFunction.h"
#include <io.h>
#include <Winspool.h>
#include <TimeAPI.h>

#pragma warning (disable:4996)
#pragma warning (disable: 4995)
#pragma comment( lib, "Version" )

//=============================================================================
// Method		: HexStringToDecimal
// Access		: public 
// Returns		: int
// Parameter	: const char * s
// Parameter	: int count
// Qualifier	:
// Last Update	: 2010/02/09 - 9:36
// Desc.		: 16진수 문자열을 10진수 숫자로 변환
//=============================================================================
int HexStringToDecimal(const char *s , int count)
{
	int result		= 0;
	int hexValue	= 0;
	char ch			= 0;

	while ( *s != '\0' )
	{
		ch = *s++;

		const char *digits = "0123456789ABCDEF";

		int i = 0;
		for ( i = 0; digits[i] != '\0'; i++ ) 
		{
			if ( toupper ( (unsigned char)ch ) == digits[i] )
				break;
		}
		hexValue =  digits[i] != '\0' ? i : -1;

		result = 16 * result + hexValue;
	}

	return result;
}

int HexStringToDecimal(LPCTSTR szHex)
{
	int result = 0;
	int hexValue = 0;
	TCHAR ch = 0;

	while (*szHex != '\0')
	{
		ch = *szHex++;

		const TCHAR *digits = _T("0123456789ABCDEF");

		int i = 0;
		for (i = 0; digits[i] != '\0'; i++)
		{
			if (toupper((TCHAR)ch) == digits[i])
				break;
		}
		hexValue = digits[i] != '\0' ? i : -1;

		result = 16 * result + hexValue;
	}

	return result;
}

//=============================================================================
// Method		: HexToCString
// Access		: public 
// Returns		: CString
// Parameter	: LPVOID lpBuff
// Parameter	: UINT nBuffSize
// Qualifier	:
// Last Update	: 2010/01/15 - 17:03
// Desc.		: 헥스 데이타를 아스키 형태의 스트링으로 바꾼다.
//=============================================================================
CString HexToCString(LPVOID lpBuff, UINT nBuffSize)
{
	ASSERT(NULL != lpBuff);

	CString strTemp;
	CString strResult;
	unsigned char* szHexString;
	szHexString = new unsigned char[nBuffSize];
	memset (szHexString, 0, nBuffSize);
	memcpy_s (szHexString, nBuffSize, (unsigned char*)lpBuff, nBuffSize);

	for (UINT nCnt = 0; nCnt < nBuffSize; nCnt++)
	{
		if (szHexString[nCnt] < 0x0F)
			strTemp.Format (_T(" %02X"), szHexString[nCnt]);
		else
			strTemp.Format (_T(" %02X"), szHexString[nCnt]);
		strResult = strResult + strTemp;
	}

	delete [] szHexString;

	return strResult;
}

BOOL HexToString( LPCSTR lpInBuff, UINT nInSize, LPSTR lpOutBuff, UINT& nOutSize )
{
	ASSERT(NULL != lpInBuff);

	CStringA strTemp;
	CStringA strResult;
	CStringA sz;

	for (UINT nCnt = 0; nCnt < nInSize; nCnt++)
	{
		if (NULL == &(lpInBuff[nCnt]))
		{
			TRACE (_T("*** Error HexToString \n"));
			strResult = strResult + "Error";			
			break;
		}

		if (lpInBuff[nCnt] < 0x0F)
			strTemp.Format (" %02X", lpInBuff[nCnt]);
		else
			strTemp.Format (" %02X", lpInBuff[nCnt]);
		strResult = strResult + strTemp;
	}
	
	nOutSize = strResult.GetLength();
	memcpy (lpOutBuff, strResult.GetBuffer(), nOutSize);

	return TRUE;
}

//=============================================================================
// Method		: NullCharToSpaceChar
// Access		: public 
// Returns		: CString
// Parameter	: LPVOID lpBuff
// Parameter	: DWORD dwBuffSize
// Qualifier	:
// Last Update	: 2010/01/15 - 17:03
// Desc.		: Null 문자('\0')가 포함된 데이타를 스페이스 문자(0x20)로 변경한다.
//=============================================================================
CString NullCharToSpaceChar(LPVOID lpBuff, DWORD dwBuffSize)
{
	CString strResult;

	for (UINT nCntSrc = 0; nCntSrc < dwBuffSize; nCntSrc++)
		if (NULL != *((char*)lpBuff + nCntSrc))
			strResult.AppendChar (*((char*)lpBuff + nCntSrc));
		else
			strResult.AppendChar (_T(' '));

	return strResult;
}

//=============================================================================
// Method		: HexCharzToByte
// Access		: public 
// Returns		: char
// Parameter	: char cFirstChar
// Parameter	: char cSecondChar
// Qualifier	:
// Last Update	: 2010/01/15 - 17:03
// Desc.		: Hex 문자 2개를 1개의 숫자형으로 바꾼다.
//=============================================================================
char HexCharzToByte (char cFirstChar, char cSecondChar)
{
	char cResult = 0;
	char cAsciiValue[2] = {0, 0};

	cAsciiValue[0] = HexCharToByte (cFirstChar);
	cAsciiValue[1] = HexCharToByte (cSecondChar);

	cResult = cResult | (cAsciiValue[0] << 4);
	cResult = cResult | (cAsciiValue[1] & 0x0F);

	return cResult;
}

//=============================================================================
// Method		: HexCharToByte
// Access		: public 
// Returns		: BYTE
// Parameter	: char cHexChar
// Qualifier	:
// Last Update	: 2010/01/15 - 17:02
// Desc.		: Hex 문자 1개를 1개의 숫자형으로 바꾼다.
//=============================================================================
BYTE HexCharToByte (char cHexChar)
{
	char cValue = (char) toupper (cHexChar);
	BYTE cAsciiValue;

	if( ( '0' <= cValue ) && ( cValue <='9' ) )
	{
		cAsciiValue = cValue - '0';
	}
	else if( ( 'A' <= cValue ) && ( cValue <= 'F' ) )
	{
		cAsciiValue = (cValue - 'A') + 10;
	}	
	else
	{
		cAsciiValue = 0xFF;
	}

	return cAsciiValue;
}

//=============================================================================
// Method		: SecondToTimeString
// Access		: public 
// Returns		: CString
// Parameter	: DWORD dwSecond
// Qualifier	:
// Last Update	: 2010/01/15 - 17:02
// Desc.		: 초 단위의 수치를 시간, 분, 초 단위의 스트링으로 변환한다.
//=============================================================================
CString SecondToTimeString (DWORD dwSecond)
{
	CString strTime = _T("");
	CString strTemp = _T("");

	UINT nSecond	= dwSecond % 60;
	UINT nMinute	= (dwSecond / 60) % 60;
	UINT nHour 		= dwSecond / 3600;
	
	if (0 < nHour)
	{
		strTemp.Format (_T("%d 시간 "), nHour);
		strTime += strTemp;
	}
	
	if (0 < nMinute)
	{
		strTemp.Format (_T("%d 분 "), nMinute);
		strTime += strTemp;
	}

	strTemp.Format (_T("%d 초"), nSecond);
	strTime += strTemp;

	return strTime;
}

//=============================================================================
// Method		: MilisecondToTimeString
// Access		: public 
// Returns		: CString
// Parameter	: DWORD dwMilisecond
// Qualifier	:
// Last Update	: 2010/01/15 - 17:02
// Desc.		: 밀리초 단위의 수치를 시간, 분, 초 단위의 스트링으로 변환한다.
//=============================================================================
CString MilisecondToTimeString (DWORD dwMilisecond)
{
	CString strTime = _T("");
	CString strTemp = _T("");

	//UINT nMilisecond= dwMilisecond % 1000;
	UINT nMilisecond= (dwMilisecond % 1000) / 100;
	UINT nSecond	= (dwMilisecond / 1000) % 60;
	UINT nMinute	= (dwMilisecond / 60000) % 60;
	UINT nHour 		= dwMilisecond / 3600000;
	
// 	if (0 < nHour)
// 	{
// 		strTemp.Format (_T("%d:"), nHour);
// 		strTime += strTemp;
// 	}
	
// 	if (0 < nMinute)
// 	{
// 		strTemp.Format (_T("%02d:"), nMinute);
// 		strTime += strTemp;
// 	}

	strTemp.Format (_T("%d:%02d:%02d.%01d"), nHour, nMinute, nSecond, nMilisecond);
	strTime += strTemp;

	return strTime;
}

//=============================================================================
// Method		: DigitToCommaDigitStr
// Access		: public 
// Returns		: CString
// Parameter	: long long llDigit
// Qualifier	:
// Last Update	: 2010/01/15 - 17:02
// Desc.		: 일반 숫자 스트링을 콤마가 포함된 스트링으로 변환
//=============================================================================
CString DigitToCommaDigitStr (long long llDigit)
{
	CString strDigit = _T("");
	strDigit.Format (_T("%d"), llDigit);
    int nLength = strDigit.GetLength();

	if (strDigit.GetAt(0) == _T('-'))
		--nLength;

	for ( int i = 1; i * 3 < nLength; i++ )
	{		
        strDigit.Insert(nLength - (i * 3), _T(","));
	}

	return strDigit;
}

//=============================================================================
// Method		: GetLastWriteTime
// Access		: public 
// Returns		: BOOL
// Parameter	: HANDLE hFile
// Parameter	: LPTSTR lpszString
// Parameter	: DWORD dwSize
// Qualifier	:
// Last Update	: 2010/01/15 - 17:02
// Desc.		: 파일의 마지막 변경 시간을 얻어온다.
//=============================================================================
BOOL GetLastWriteTime(HANDLE hFile, LPTSTR lpszString, DWORD dwSize)
{
    FILETIME ftCreate, ftAccess, ftWrite;
    SYSTEMTIME stUTC, stLocal;
    DWORD dwRet = 0;

    // Retrieve the file times for the file.
    if (!GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite))
        return FALSE;

    // Convert the last-write time to local time.
    FileTimeToSystemTime(&ftWrite, &stUTC);
    SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);

    // Build a string showing the date and time.
    //dwRet = StringCchPrintf(lpszString, dwSize, TEXT("%02d/%02d/%d  %02d:%02d"), stLocal.wMonth, stLocal.wDay, stLocal.wYear, stLocal.wHour, stLocal.wMinute);

    if( S_OK == dwRet )
        return TRUE;
    else return FALSE;
}

//=============================================================================
// Method		: GetLastWriteTime
// Access		: public  
// Returns		: BOOL
// Parameter	: __in LPCTSTR lpszFile
// Parameter	: __out SYSTEMTIME & tmFile
// Qualifier	:
// Last Update	: 2016/12/15 - 11:54
// Desc.		:
//=============================================================================
BOOL GetLastWriteTime(__in LPCTSTR lpszFile, __out SYSTEMTIME& tmFile)
{
	HANDLE hFile = NULL;

	hFile = CreateFile(lpszFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	FILETIME ftCreate, ftAccess, ftWrite;
	SYSTEMTIME stUTC;

	// Retrieve the file times for the file.
	if (!GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite))
	{
		CloseHandle(hFile);
		return FALSE;
	}

	// Convert the last-write time to local time.
	FileTimeToSystemTime(&ftWrite, &stUTC);
	SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &tmFile);

	CloseHandle(hFile);

	return TRUE;
}

//=============================================================================
// Method		: GetLastWriteTime
// Access		: public  
// Returns		: BOOL
// Parameter	: __in LPCTSTR lpszFile
// Parameter	: __out FILETIME & tmFile
// Qualifier	:
// Last Update	: 2016/12/15 - 12:21
// Desc.		:
//=============================================================================
BOOL GetLastWriteTime(__in LPCTSTR lpszFile, __out FILETIME& tmFile)
{
	HANDLE hFile = NULL;

	hFile = CreateFile(lpszFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	FILETIME ftCreate, ftAccess;

	// Retrieve the file times for the file.
	if (!GetFileTime(hFile, &ftCreate, &ftAccess, &tmFile))
	{
		CloseHandle(hFile);
		return FALSE;
	}

	CloseHandle(hFile);

	return TRUE;
}

//=============================================================================
// Method		: GetAppDir
// Access		: public 
// Returns		: void
// Parameter	: CString & strAppDir
// Qualifier	:
// Last Update	: 2010/01/15 - 17:01
// Desc.		: 애플캐이션 디렉토리를 얻어온다.
//=============================================================================
void GetAppDir(CString& strAppDir)
{
	TCHAR szFullPath[MAX_PATH];
	TCHAR szDir[_MAX_DIR];
	TCHAR szDrive[_MAX_DRIVE];

	// Get application's full path.
	::GetModuleFileName(NULL, szFullPath, MAX_PATH);

	// Break full path into seperate components.	
	_tsplitpath(szFullPath, szDrive, szDir, NULL, NULL);

	// Store application's drive and path
	strAppDir.Format(_T("%s%s"), szDrive, szDir);
}

//=============================================================================
// Method		: DoEvents
// Access		: public 
// Returns		: void
// Qualifier	:
// Last Update	: 2010/01/15 - 17:01
// Desc.		: Wait 계열 함수로 대기하는 동안에도 윈도우 메세지 처리가 가능하게 하는 루틴
//=============================================================================
void DoEvents()
{
	MSG msg;

	// window message         
	while (PeekMessage(&msg,NULL,NULL,NULL,PM_REMOVE))         
	{            
		TranslateMessage(&msg);            
		DispatchMessage(&msg);         
	}      
}

//=============================================================================
// Method		: DoEvents
// Access		: public  
// Returns		: void
// Parameter	: DWORD dwMiliSeconds
// Qualifier	:
// Last Update	: 2017/10/14 - 20:01
// Desc.		:
//=============================================================================
void DoEvents(DWORD dwMiliSeconds)
{
	clock_t start_tm = clock();
	do
	{
		DoEvents();
		::Sleep(1);
	} while ((DWORD)(clock() - start_tm)<dwMiliSeconds);
}

//=============================================================================
// Method		: WaitWithMessageLoop
// Access		: public 
// Returns		: BOOL
// Parameter	: HANDLE hEvent
// Parameter	: int nTimeout
// Qualifier	:
// Last Update	: 2010/01/15 - 17:00
// Desc.		: Pump messages while waiting for event
//=============================================================================
BOOL WaitWithMessageLoop(HANDLE hEvent, int nTimeout)
{   
	DWORD dwRet;
	DWORD dwMaxTick = GetTickCount() + nTimeout;

	while (1)
	{
		// wait for event or message, if it's a message, process it and return to waiting state
		dwRet = MsgWaitForMultipleObjects(1, &hEvent, FALSE, dwMaxTick - GetTickCount(), QS_ALLINPUT);
		if (dwRet == WAIT_OBJECT_0)
		{
			TRACE0("WaitWithMessageLoop() event triggered.\n");
			return TRUE;      
		}   
		else
		{
			if (dwRet == WAIT_OBJECT_0 + 1)
			{
				// process window messages
				DoEvents();
			}  
			else
			{
				// timed out !
				return FALSE;
			}
		}
	}
}

//=============================================================================
// Method		: Delay
// Access		: public 
// Returns		: void
// Parameter	: DWORD nTimeMs
// Qualifier	:
// Last Update	: 2010/01/15 - 17:00
// Desc.		: 해당 시간만큼 대기하는 함수 
//					(Sleep과 다르게 대기하는 동안 윈도우 메세지 처리가능)
//=============================================================================
void Delay(DWORD nTimeMs)
{
	MSG		msg;
	DWORD	endTick;

	endTick = GetTickCount() + nTimeMs;

	while(GetTickCount() < endTick)
	{
		if(PeekMessage(&msg, NULL, 0, 0, TRUE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}


//#include <imm.h>
////=============================================================================
//// Method		: GetHanMode
//// Access		: public 
//// Returns		: bool __fastcall
//// Parameter	: void
//// Qualifier	:
//// Last Update	: 2010/01/15 - 16:59
//// Desc.		: 현재 활성화된 윈도우가 한글 입력 상태인지 판별한다.
////=============================================================================
//BOOL __fastcall GetHanMode(void)
//{
//	/*unsigned long Conversion, Sentence;
//
//	HIMC Mode = ImmGetContext((HWND) GetForegroundWindow());
//	ImmGetConversionStatus(Mode, &Conversion, &Sentence);
//	return (Conversion == IME_CMODE_HANGEUL);*/
//
//	BOOL	bReturn = FALSE;
//	HIMC	hIMC;
//	DWORD	dwConversion;
//	DWORD	dwSentence;
//	HWND	hWnd = (HWND) GetForegroundWindow();
//
//	hIMC=::ImmGetContext(hWnd);
//	ImmGetConversionStatus(hIMC,&dwConversion,&dwSentence);
//	if (dwConversion & IME_CMODE_NATIVE)     // 한글 mode 
//	{
//		//if (dwConversion & IME_CMODE_FULLSHAPE)	// 전자 mode 
//		//	MessageBox (hWnd, "한글 전자", "IME Status", MB_OK);
//		//else
//		//	MessageBox (hWnd, "한글 반자", "IME Status", MB_OK);
//		bReturn = TRUE;
//	}
//	else		// 영문 mode
//	{
//		//if (dwConversion & IME_CMODE_FULLSHAPE)	// 전자 mode
//		//	MessageBox (hWnd, "영문 전자", "IME Status", MB_OK);
//		//else
//		//	MessageBox (hWnd, "영문 반자", "IME Status", MB_OK);
//		bReturn = FALSE;
//	}
//	ImmReleaseContext(hWnd,hIMC);
//
//	return bReturn;
//}

////=============================================================================
//// Method		: ToggleHanMode
//// Access		: public 
//// Returns		: void __fastcall
//// Parameter	: void
//// Qualifier	:
//// Last Update	: 2010/01/15 - 16:59
//// Desc.		: 현재 활성화된 윈도우의 한글/영문 입력 상태를 전환한다.
////=============================================================================
//void __fastcall ToggleHanMode(void)
//{
//	unsigned long Conversion, Sentence;
//	HWND hWnd = (HWND) GetForegroundWindow();
//
//	HIMC hIMC = ImmGetContext(hWnd);
//	ImmGetConversionStatus(hIMC, &Conversion, &Sentence);
//	if (Conversion == IME_CMODE_HANGEUL)
//		ImmSetConversionStatus(hIMC, IME_CMODE_ALPHANUMERIC, IME_SMODE_NONE);
//	else
//		ImmSetConversionStatus(hIMC, IME_CMODE_HANGEUL, IME_SMODE_NONE);
//
//	ImmReleaseContext( hWnd, hIMC );
//} 


//=============================================================================
// Method		: UtilIsDirectoryExists
// Access		: public 
// Returns		: BOOL
// Parameter	: LPCTSTR directory
// Qualifier	:
// Last Update	: 2010/02/10 - 10:26
// Desc.		: 디렉토리 존재여부 판별
//=============================================================================
BOOL UtilIsDirectoryExists(LPCTSTR directory)
{
	BOOL isExists=FALSE;
	DWORD attributes=GetFileAttributes(directory);
	if ((attributes!=0xFFFFFFFFUL)&&(attributes&FILE_ATTRIBUTE_DIRECTORY))
		isExists=TRUE;

	return isExists;
}

//=============================================================================
// Method		: UtilCreateDirectory
// Access		: public 
// Returns		: int
// Parameter	: LPCTSTR directory
// Qualifier	:
// Last Update	: 2010/02/10 - 10:26
// Desc.		: 디렉토리 생성 (하위 디렉토리 포함)
//=============================================================================
int UtilCreateDirectory(LPCTSTR directory)
{
	int createdItemCount=0;
	TCHAR buffer[MAX_PATH];

	if (lstrcpy(buffer,directory)!=NULL)
	{
		LPTSTR pBuffer=buffer;
		LPTSTR slash;

		do
		{
			BOOL isRoot=FALSE;
			LPTSTR colon;

			slash=_tcschr(pBuffer,'\\');

			if (slash!=NULL)
				*slash='\0';

			colon=_tcschr(pBuffer,':');
			if (colon!=NULL)
				isRoot=TRUE;

			if ((!isRoot)&&(!UtilIsDirectoryExists(buffer)))
			{
				if (CreateDirectory(buffer,NULL))
					createdItemCount++;
			}

			if (slash!=NULL)
				*slash='\\'; // Restore

			pBuffer=slash+1;
		}
		while (slash!=NULL);
	}

	return createdItemCount;
}

//=============================================================================
// Method		: DeleteAllFiles
// Access		: public  
// Returns		: int
// Parameter	: LPCTSTR szDir
// Parameter	: int recur
// Qualifier	:
// Last Update	: 2016/3/31 - 17:15
// Desc.		:
//=============================================================================
int DeleteAllFiles(LPCTSTR szDir, int recur)
{
	HANDLE hSrch;
	WIN32_FIND_DATA wfd;
	int res = 1;

	TCHAR DelPath[MAX_PATH];
	TCHAR FullPath[MAX_PATH];
	TCHAR TempPath[MAX_PATH];

	lstrcpy(DelPath, szDir);
	lstrcpy(TempPath, szDir);
	if (lstrcmp(DelPath + lstrlen(DelPath) - 4, _T("\\*.*")) != 0)
		lstrcat(DelPath, _T("\\*.*"));

	hSrch = FindFirstFile(DelPath, &wfd);
	if (hSrch == INVALID_HANDLE_VALUE)
	{
		if (recur > 0)
			RemoveDirectory(TempPath);

		return -1;
	}
	while (res)
	{
		wsprintf(FullPath, _T("%s\\%s"), TempPath, wfd.cFileName);

		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_READONLY)
		{
			SetFileAttributes(FullPath, FILE_ATTRIBUTE_NORMAL);
		}

		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (lstrcmp(wfd.cFileName, _T(".")) && lstrcmp(wfd.cFileName, _T("..")))
			{
				recur++;
				DeleteAllFiles(FullPath, recur);
				recur--;
			}
		}
		else
		{
			DeleteFile(FullPath);
		}
		res = FindNextFile(hSrch, &wfd);
	}
	FindClose(hSrch);

	if (recur > 0)
		RemoveDirectory(TempPath);

	return 0;
}

//=============================================================================
// Method		: MakeDirectory
// Access		: public  
// Returns		: void
// Parameter	: LPCTSTR szPath
// Qualifier	:
// Last Update	: 2016/4/20 - 19:24
// Desc.		:
//=============================================================================
void MakeDirectory(LPCTSTR szPath)
{
	CFileFind	filefind;
	// 해당 폴더가 없으면 생성한다.
	if (!filefind.FindFile(szPath))
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
}

//=============================================================================
// Method		: MakeSubDirectory
// Access		: public  
// Returns		: void
// Parameter	: LPCTSTR lpszDirName
// Qualifier	:
// Last Update	: 2016/9/23 - 17:09
// Desc.		:
//=============================================================================
void MakeSubDirectory(LPCTSTR lpszDirName)
{
	if (NULL == lpszDirName)
	{
		AfxMessageBox(_T("Folder Name is NULL!"));
		return;
	}

	CString strPath = lpszDirName;
	// 폴더가 존재하는지 체크한다.
	// 폴더가 없다면 폴더를 만든다.
	if (INVALID_FILE_ATTRIBUTES == GetFileAttributes(lpszDirName))
	{
		CString resToken;
		CString strMakePath;
		int curPos = 0;
		resToken = strPath.Tokenize(_T("\\"), curPos);
		while (resToken != _T(""))
		{
			strMakePath += resToken;
			if (INVALID_FILE_ATTRIBUTES == GetFileAttributes(strMakePath))
				if (!CreateDirectory(strMakePath, NULL))
				{
					DWORD dwErr = GetLastError();
				}

			strMakePath += '\\';

			resToken = strPath.Tokenize(_T("\\"), curPos);
		};
	}
}

//=============================================================================
// Method		: GetDirectoryFile
// Access		: public  
// Returns		: BOOL
// Parameter	: __in CString szDir
// Parameter	: __in CString szExt
// Parameter	: __out CString * lpszFile
// Parameter	: __out UINT & nFileCnt
// Qualifier	:
// Last Update	: 2018/12/9 - 13:56
// Desc.		:
//=============================================================================
BOOL GetDirectoryFile(__in CString szDir, __in CString szExt, __out CString* lpszFile, __out UINT &nFileCnt)
{
	if (TRUE == szDir.IsEmpty() || TRUE == szExt.IsEmpty())
		return FALSE;

	CFileFind	Finder;
	CString		strName;

	strName.Format(_T("%s\\*.%s"), szDir, szExt);

	BOOL bWorking = Finder.FindFile(strName);

	// 파일 수량 확인
	UINT nCnt = 0;

	while (bWorking)
	{
		bWorking = Finder.FindNextFile();

		if (TRUE == Finder.IsArchived())
		{
			lpszFile[nCnt] = Finder.GetFilePath();
			nCnt++;
		}
	}

	nFileCnt = nCnt;

	return TRUE;
}

//=============================================================================
// Method		: GetDirectoryFileTitle
// Access		: public  
// Returns		: BOOL
// Parameter	: __in CString szDir
// Parameter	: __in CString szExt
// Parameter	: __out CString * lpszFile
// Parameter	: __out UINT & nFileCnt
// Qualifier	:
// Last Update	: 2019/2/20 - 14:41
// Desc.		:
//=============================================================================
BOOL GetDirectoryFileTitle(__in CString szDir, __in CString szExt, __out CString* lpszFile, __out UINT &nFileCnt)
{
	if (TRUE == szDir.IsEmpty() || TRUE == szExt.IsEmpty())
		return FALSE;

	CFileFind	Finder;
	CString		strName;

	strName.Format(_T("%s\\*.%s"), szDir, szExt);

	BOOL bWorking = Finder.FindFile(strName);

	// 파일 수량 확인
	UINT nCnt = 0;

	while (bWorking)
	{
		bWorking = Finder.FindNextFile();

		if (TRUE == Finder.IsArchived())
		{
			lpszFile[nCnt] = Finder.GetFileTitle();
			nCnt++;
		}
	}

	nFileCnt = nCnt;

	return TRUE;
}

//=============================================================================
// Method		: WaitTerminateHandle
// Access		: public 
// Returns		: void
// Parameter	: HANDLE hParam
// Parameter	: DWORD dwTimeOut
// Qualifier	:
// Last Update	: 2010/11/5 - 13:52
// Desc.		: 핸들이 종료될때지 기다림
//=============================================================================
void WaitTerminateHandle( HANDLE hParam, DWORD dwTimeOut /*= INFINITE*/ )
{
	// 쓰레드 종료 대기	
	//while (WAIT_OBJECT_0 != WaitForSingleObject(hParam, 0))
	//{
	//	MSG msg;

	//	while (PeekMessage(&msg,NULL,NULL,NULL,PM_REMOVE))         
	//	{            
	//		TranslateMessage(&msg);            
	//		DispatchMessage(&msg);         
	//	} 
	//}
	DWORD	dwBufSize	= 0;
	BOOL	fDone		= TRUE;

	HANDLE     hArray[1];
	hArray[0] = hParam;	

	while (fDone)
	{
		DWORD dwResult = MsgWaitForMultipleObjects(1, hArray, FALSE, dwTimeOut, QS_ALLINPUT);

		switch (dwResult)
		{
		case WAIT_OBJECT_0:		// 쓰레드 종료됨		
			fDone = FALSE;			
			break;

		case WAIT_OBJECT_0 + 1:	// 프로그램 종료 이벤트 처리				
			DoEvents();				
			break;

		case WAIT_TIMEOUT:
			TRACE (_T("WaitTerminateThread is Timeout\n"));
			fDone	= FALSE;
			break;
		}
	}	
}

//=============================================================================
// Method		: WaitForThreadToTerminate
// Access		: public  
// Returns		: void
// Parameter	: HANDLE hThread
// Parameter	: DWORD dwTimeOut
// Qualifier	:
// Last Update	: 2016/2/3 - 14:44
// Desc.		:
//=============================================================================
void WaitForThreadToTerminate(HANDLE hThread, DWORD dwTimeOut /*= INFINITE*/)
{
	DWORD dwRet;
	MSG msg;
	do
	{
		dwRet = ::MsgWaitForMultipleObjects(1, &hThread, FALSE, dwTimeOut, QS_ALLINPUT);
		if (dwRet != WAIT_OBJECT_0)
		{
			while (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	} while ((dwRet != WAIT_OBJECT_0) && (dwRet != WAIT_FAILED));
}

//=============================================================================
// Method		: WaitForTimerIsNull
// Access		: public  
// Returns		: BOOL
// Parameter	: HANDLE hTimer
// Parameter	: DWORD dwTimeOut
// Qualifier	:
// Last Update	: 2016/2/3 - 15:23
// Desc.		:
//=============================================================================
BOOL WaitForTimerIsNull(HANDLE hTimer, DWORD dwTimeOut /*= INFINITE*/)
{
	DWORD dwStartTick = GetTickCount();	
	BOOL bCont = TRUE;
	MSG msg;
	while (bCont)
	{
		if (NULL == hTimer)
		{
			return TRUE;
		}
		else
		{
			if (dwTimeOut < (GetTickCount() - dwStartTick))
				return FALSE;

			while (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}

	return FALSE;
}

//=============================================================================
// Method		: ConvNumberToFormatString
// Access		: public 
// Returns		: CStringA
// Parameter	: DOUBLE dData
// Parameter	: BYTE byLength
// Parameter	: BYTE byPoint
// Qualifier	:
// Last Update	: 2010/02/24 - 10:29
// Desc.		: 
//=============================================================================
CString ConvNumberToFormatString (DOUBLE dData, BYTE byLength, BYTE byPoint)
{
	ASSERT(byPoint < byLength);

	CString strReturn;
	CString strLeft;
	CString strRight;

	BYTE byRightLen = byLength - byPoint;

	strReturn.Format (_T("%f"), dData);

	int iFind = strReturn.Find ('.', 0);

	// 소수점 왼쪽
	strLeft = strReturn.Left (iFind);
	if (iFind < byPoint)
	{
		for (int iCnt = 0; iCnt < (byPoint - iFind); iCnt++)
			strLeft.Insert (0, '0');
	}
	else if (byPoint < iFind)
	{
		strLeft = strLeft.Right (byPoint);
	}

	strReturn.Delete (0, iFind + 1);

	// 소수점 오른쪽
	strRight = strReturn.Left (byRightLen);
	if (strRight.GetLength () < byRightLen)
		for (int iCnt = 0; iCnt < (byRightLen - strRight.GetLength ()); iCnt++)
			strRight.Append (_T("0"));

	strReturn = strLeft + strRight;

	return strReturn;
}

//=============================================================================
// Method		: FixedSizeString
// Access		: public 
// Returns		: CStringA
// Parameter	: __in CStringA strSrc
// Parameter	: __in INT nSize
// Parameter	: __in CHAR chFill
// Parameter	: __in BOOL bLeftFill
// Qualifier	:
// Last Update	: 2013/6/19 - 21:53
// Desc.		:
//=============================================================================
//CString FixedSizeString (__in CString strSrc, __in INT nSize, __in CHAR chFill /*= 0x20*/, __in BOOL bLeftFill /*= FALSE*/)
CStringA	FixedSizeString	(__in LPCSTR szSrc, __in INT nSize, __in CHAR chFill /*= 0x20*/, __in BOOL bLeftFill /*= TRUE*/)
{
	CStringA strReturn;
	CStringA strFill;
	CStringA strSrc = szSrc;
	strFill.AppendChar(chFill);

	int iSrcLength = strSrc.GetLength();

	if(iSrcLength >= nSize)	
	{
		strReturn = strSrc.Left(nSize);
	}
	else
	{
		strReturn = strSrc;
		int iFillCount = nSize - strSrc.GetLength();
		for(int iCnt = 0; iCnt < iFillCount; iCnt++)
		{
			if (bLeftFill)
				strReturn = strFill + strReturn;
			else
				strReturn = strReturn + strFill;
		}
	}

	return strReturn;
}

//=============================================================================
// Method		: isStringDouble
// Access		: public  
// Returns		: BOOL
// Parameter	: char * szString
// Qualifier	:
// Last Update	: 2014/7/31 - 16:03
// Desc.		: 문자열이 숫자로 이루어 졌는지 판별
//=============================================================================
BOOL isStringDouble(char *szString)
{
	size_t size = strlen(szString);

	if (size == 0) 
		return FALSE; // 0바이트 문자열은 숫자가 아님

	for (int iDx = 0; iDx < (int) size; iDx++) 
	{
		if (szString[iDx] == '.') 
			continue;

		//if (!isalnum(szString[iDx]))
		if (szString[iDx] < '0' || szString[iDx] > '9') 
			return FALSE;
	}

	return TRUE;
}


//=============================================================================
// Method		: StringTimeToSystemTime
// Access		: public static 
// Returns		: BOOL
// Parameter	: LPCSTR lpszTime
// Parameter	: SYSTEMTIME & TimeTarget
// Qualifier	:
// Last Update	: 2010/02/22 - 18:17
// Desc.		: YYYYMMDDHHMMSS 형태의 문자열을 SYSTEMTIME 형식으로 변환한다.
//=============================================================================
BOOL StringTimeToSystemTime (LPCSTR lpszTime, SYSTEMTIME& TimeTarget)
{
	if (NULL == lpszTime)
		return FALSE;

	if (strlen (lpszTime) < 14)
		return FALSE;

	CStringA strSource = lpszTime;
	CStringA strValue;

	strValue = strSource.Left(4);
	strSource.Delete (0, 4);
	TimeTarget.wYear = (WORD) atoi (strValue);

	strValue = strSource.Left(2);
	strSource.Delete (0, 2);
	TimeTarget.wMonth = (WORD) atoi (strValue);

	strValue = strSource.Left(2);
	strSource.Delete (0, 2);
	TimeTarget.wDay = (WORD) atoi (strValue);

	strValue = strSource.Left(2);
	strSource.Delete (0, 2);
	TimeTarget.wHour = (WORD) atoi (strValue);

	strValue = strSource.Left(2);
	strSource.Delete (0, 2);
	TimeTarget.wMinute = (WORD) atoi (strValue);

	strValue = strSource.Left(2);
	strSource.Delete (0, 2);
	TimeTarget.wSecond = (WORD) atoi (strValue);

	return TRUE;
}

//=============================================================================
// Method		: SystemTimeToString
// Access		: public 
// Returns		: CString
// Parameter	: SYSTEMTIME TimeSource
// Qualifier	:
// Last Update	: 2011/3/2 - 11:26
// Desc.		: YYYYMMDDHHMMSS 형태의 문자열로 반환
//=============================================================================
CString SystemTimeToString( SYSTEMTIME TimeSource )
{
	CString strTemp;
	strTemp.Format(_T("%04d%02d%02d%02d%02d%02d"), TimeSource.wYear, TimeSource.wMonth, TimeSource.wDay,
												TimeSource.wHour, TimeSource.wMinute, TimeSource.wSecond);

	return strTemp;
}

//=============================================================================
// Method		: FormatStringToSystemTime
// Access		: public 
// Returns		: BOOL
// Parameter	: LPCSTR lpszTime
// Parameter	: SYSTEMTIME & TimeTarget
// Qualifier	:
// Last Update	: 2013/1/10 - 10:59
// Desc.		:
//=============================================================================
BOOL FormatStringToSystemTimeA( LPCSTR lpszTime, SYSTEMTIME& TimeTarget )
{
	if (NULL == lpszTime)
		return FALSE;

	if (strlen (lpszTime) < 19)
		return FALSE;

	CStringA strSource = lpszTime;
	CStringA strValue;

	strValue = strSource.Left(4);
	strSource.Delete (0, 5);
	TimeTarget.wYear = (WORD) atoi (strValue);

	strValue = strSource.Left(2);
	strSource.Delete (0, 3);
	TimeTarget.wMonth = (WORD) atoi (strValue);

	strValue = strSource.Left(2);
	strSource.Delete (0, 3);
	TimeTarget.wDay = (WORD) atoi (strValue);

	strValue = strSource.Left(2);
	strSource.Delete (0, 3);
	TimeTarget.wHour = (WORD) atoi (strValue);

	strValue = strSource.Left(2);
	strSource.Delete (0, 3);
	TimeTarget.wMinute = (WORD) atoi (strValue);

	strValue = strSource.Left(2);
	strSource.Delete (0, 2);
	TimeTarget.wSecond = (WORD) atoi (strValue);

	return TRUE;
}

BOOL FormatStringToSystemTime(LPCTSTR lpszTime, SYSTEMTIME& TimeTarget)
{
	if (NULL == lpszTime)
		return FALSE;

	if (_tcslen(lpszTime) < 19)
		return FALSE;

	CString strSource = lpszTime;
	CString strValue;

	strValue = strSource.Left(4);
	strSource.Delete(0, 5);
	TimeTarget.wYear = (WORD)_ttoi(strValue);

	strValue = strSource.Left(2);
	strSource.Delete(0, 3);
	TimeTarget.wMonth = (WORD)_ttoi(strValue);

	strValue = strSource.Left(2);
	strSource.Delete(0, 3);
	TimeTarget.wDay = (WORD)_ttoi(strValue);

	strValue = strSource.Left(2);
	strSource.Delete(0, 3);
	TimeTarget.wHour = (WORD)_ttoi(strValue);

	strValue = strSource.Left(2);
	strSource.Delete(0, 3);
	TimeTarget.wMinute = (WORD)_ttoi(strValue);

	strValue = strSource.Left(2);
	strSource.Delete(0, 2);
	TimeTarget.wSecond = (WORD)_ttoi(strValue);

	return TRUE;
}

//=============================================================================
// Method		: SystemTimeToFormatString
// Access		: public 
// Returns		: CString
// Parameter	: SYSTEMTIME TimeSource
// Qualifier	:
// Last Update	: 2013/1/10 - 10:59
// Desc.		:
//=============================================================================
CString SystemTimeToFormatString( SYSTEMTIME TimeSource )
{
	CString strTemp;
	strTemp.Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), TimeSource.wYear, TimeSource.wMonth, TimeSource.wDay,
		TimeSource.wHour, TimeSource.wMinute, TimeSource.wSecond);

	return strTemp;
}

CString SystemTimeToFormatString_Month(SYSTEMTIME TimeSource)
{
	CString strTemp;
	strTemp.Format(_T("%02d/%02d %02d:%02d:%02d"), TimeSource.wMonth, TimeSource.wDay,
		TimeSource.wHour, TimeSource.wMinute, TimeSource.wSecond);

	return strTemp;
}

// SYSTEMTIME 값을 비교하여 몇초 차이 나는지 계산하여 반환한다.
double CompareSystemTime(PSYSTEMTIME pEndTime, PSYSTEMTIME pStartTime)
{
	double dStartTime;
	double dEndTime;
	SystemTimeToVariantTime(pStartTime, &dStartTime);
	SystemTimeToVariantTime(pEndTime, &dEndTime);

	return (dEndTime - dStartTime);

	tm tmTime1, tmTime2;
	time_t timeTime1, timeTime2;

	tmTime1.tm_sec		= pEndTime->wSecond;
	tmTime1.tm_min		= pEndTime->wMinute;
	tmTime1.tm_hour		= pEndTime->wHour;
	tmTime1.tm_mday		= pEndTime->wDay;
	tmTime1.tm_mon		= pEndTime->wMonth - 1;
	tmTime1.tm_year		= pEndTime->wYear - 1900;
	tmTime1.tm_isdst	= 0;
	timeTime1			= ::mktime(&tmTime1);

	tmTime2.tm_sec		= pStartTime->wSecond;
	tmTime2.tm_min		= pStartTime->wMinute;
	tmTime2.tm_hour		= pStartTime->wHour;
	tmTime2.tm_mday		= pStartTime->wDay;
	tmTime2.tm_mon		= pStartTime->wMonth - 1;
	tmTime2.tm_year		= pStartTime->wYear - 1900;
	tmTime2.tm_isdst	= 0;
	timeTime2			= ::mktime(&tmTime2);

	// time2와 time1의 second 차이를 double 형으로 리턴
	return ::difftime(timeTime1, timeTime2);
}

// 테스트 시간 표시
CString		GetTestTime(DWORD start, DWORD end)
{
	CString str;
	int time = end - start;

	int hours = (time / (1000 * 60 * 60)) % 24;
	int min = (time / (1000 * 60)) % 60;
	int	seconds = (time / 1000) % 60;
	int	ms = (time % 1000);

	str.Format(_T("%02d:%02d.%03d"), min, seconds, ms);
	return str;
}

//=============================================================================
// Method		: GetVersionInfo
// Access		: public 
// Returns		: CString
// Parameter	: HMODULE hLib
// Parameter	: LPCTSTR csEntry
// Qualifier	:
// Last Update	: 2010/10/26 - 13:54
// Desc.		: 프로그램의 버전을 구해오는 함수
//=============================================================================
LPCTSTR GetVersionInfo(HMODULE hLib, LPCTSTR lpszEntry)
{
	//CString strReturn;
	LPVOID	lpOutBuffer	= NULL;

	if (hLib == NULL)
		hLib = AfxGetResourceHandle();

	HRSRC hVersion = FindResource( hLib, MAKEINTRESOURCE(VS_VERSION_INFO), RT_VERSION );
	if (hVersion != NULL)
	{
		HGLOBAL hGlobal = LoadResource( hLib, hVersion );
		if ( hGlobal != NULL)
		{
			LPVOID lpBlock = LockResource(hGlobal);
			if (lpBlock != NULL)
			{				
				struct LANGANDCODEPAGE 
				{
					WORD wLanguage;
					WORD wCodePage;
				} *lpTranslate;
				HRESULT	hr			= NULL;
				UINT	nTranslate	= 0;
				UINT	nBytes		= 0;			
				/*LPVOID	lpOutBuffer	= NULL;*/
				TCHAR	szSubBlock[1024] = {0};

				// Read the list of languages and code pages.
				VerQueryValue(lpBlock, TEXT("\\VarFileInfo\\Translation"), (LPVOID*)&lpTranslate, &nTranslate);

				// Read the file description for each language and code page.
				for( UINT nIndex = 0; nIndex < (nTranslate / sizeof(struct LANGANDCODEPAGE)); nIndex++ )
				{
					hr = StringCchPrintf(szSubBlock, 50, _T("\\StringFileInfo\\%04x%04x\\%s"), lpTranslate[nIndex].wLanguage, lpTranslate[nIndex].wCodePage, lpszEntry);
					if (FAILED(hr))
					{
						// TODO: write error handler.
					}

					// Retrieve file description for language and code page "i". 					
					if (VerQueryValue(lpBlock, szSubBlock, &lpOutBuffer, &nBytes))
					{
						//strReturn = (TCHAR*)lpOutBuffer;
						break;
					}
				}
			}
		}

		UnlockResource( hGlobal );
		FreeResource( hGlobal );
	}

	return (TCHAR*)lpOutBuffer;
	//return strReturn;
}

//=============================================================================
// Method		: GetVersionInfo
// Access		: public  
// Returns		: CString
// Parameter	: LPCTSTR lpszEntry
// Qualifier	:
// Last Update	: 2013/11/16 - 14:42
// Desc.		:
//=============================================================================
CString	GetVersionInfo (LPCTSTR lpszEntry)
{
	if(lpszEntry == _T(""))
		return _T("");

	DWORD	dwHandle	= 0;
	LPVOID	lpTranslate	= NULL;
	UINT	nTranslate	= 0;
	LPVOID	lpOutBuffer	= NULL;
	UINT	nBytes		= 0;

	// 현재 실행된 프로그램의 경로를 저장할 변수이다.
	TCHAR	szFileName[MAX_PATH];

	// 현재 실행된 프로그램의 경로를 얻는다.
	GetModuleFileName(AfxGetInstanceHandle(), szFileName, sizeof(szFileName));

	DWORD dwVersionInfoSize = GetFileVersionInfoSize(szFileName, &dwHandle);
	if(dwVersionInfoSize == 0)
		return _T("");

	LPVOID pFileInfo = (LPVOID) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwVersionInfoSize);

	GetFileVersionInfo(szFileName, dwHandle, dwVersionInfoSize, pFileInfo);
	VerQueryValue(pFileInfo, TEXT("\\VarFileInfo\\Translation"), &lpTranslate, &nTranslate);

	WORD *id = (WORD*) lpTranslate;
	TCHAR szString[255];
	_stprintf(szString, _T("\\StringFileInfo\\%04X%04X\\%s"), id[0], id[1], lpszEntry);

	VerQueryValue(pFileInfo, szString, &lpOutBuffer, &nBytes);
	if(nBytes == 0)
		return _T("");

	CString strInfo = (TCHAR *) lpOutBuffer;

	HeapFree(GetProcessHeap(), 0, pFileInfo);

	return strInfo;
}

//=============================================================================
// Method		: GetProductVersionInfo
// Access		: public  
// Returns		: LPCTSTR
// Qualifier	:
// Last Update	: 2013/11/16 - 14:07
// Desc.		:
//=============================================================================
LPCTSTR	GetProductVersionInfo ()
{
	BYTE       *block;
	DWORD FAR  *translation;
	DWORD FAR  *buffer;
	DWORD		handle;
	UINT		bytes;	
	TCHAR		name[512];
	TCHAR		data[256];
	CString		szString;

	// 현재 실행된 프로그램의 경로를 저장할 변수이다.
	TCHAR		temp_path[MAX_PATH];

	// 현재 실행된 프로그램의 경로를 얻는다.
	GetModuleFileName(AfxGetInstanceHandle(), temp_path, sizeof(temp_path));

	// 버전 정보의 실제 크기를 가져온다.
	bytes = (UINT)::GetFileVersionInfoSize( temp_path, &handle);

	if(bytes)
	{
		block = new BYTE[bytes];

		// 버전 정보에 대한 실제 블록을 가져온다.
		if( ::GetFileVersionInfo( temp_path, handle, bytes, block) )
		{
			if( ::VerQueryValue( block, _T("\\VarFileInfo\\Translation"), (LPVOID *)&translation, (UINT FAR *)&bytes) )
			{
				// File Version 정보
				wsprintf(name, _T("\\StringFileInfo\\%04x%04x\\FileVersion"), LOWORD(*translation), HIWORD(*translation) );

				if( ::VerQueryValue(block, name, (LPVOID *)&buffer, (UINT FAR *)&bytes) )
				{
					szString.Format(_T("Version %s"), (LPCTSTR)buffer );
				}
				else
					szString.Format(_T("Unable to get Version Info"), data);

				// Product Version 정보
				wsprintf(name, _T("\\StringFileInfo\\%04x%04x\\ProductVersion"), LOWORD(*translation), HIWORD(*translation) );

				if( ::VerQueryValue(block, name, (LPVOID *)&buffer, (UINT FAR *)&bytes) )
				{
					szString.Format( _T("%s"), (LPCTSTR)buffer );
				}
				else
					szString.Format( _T("Unable to get Product Version") );
			}
			else 
				szString.Format( _T("(Unable to get translation type)") );
		}
		else 
			szString.Format( _T("(Unable to get FileVersionInfo)") );

		delete [] block;
	}
	else
	{
		szString.Format( _T("(Unable to get FileVersionInfo size)") );
	}
	return szString;
}

//=============================================================================
// Method		: FormatVersion
// Access		: public 
// Returns		: CString
// Parameter	: CString cs
// Qualifier	:
// Last Update	: 2010/10/26 - 13:56
// Desc.		:
//  ex)    CString g_Version = FormatVersion(GetVersionInfo(NULL, "FileVersion"));
//=============================================================================
CString FormatVersion(CString cs)
{
	CString csRet;
	if (!cs.IsEmpty())
	{
		cs.TrimRight();
		int iPos = cs.Find(',');
		if (iPos == -1)
			return _T("");

		cs.TrimLeft();
		cs.TrimRight();

		csRet.Format(_T("%s."), cs.Left(iPos));

		while (1)
		{
			cs = cs.Mid(iPos + 1);
			cs.TrimLeft();
			iPos = cs.Find(',');
			if (iPos == -1)
			{
				csRet +=cs;
				break;
			}
			csRet += cs.Left(iPos);
			csRet += _T(".");
		}
	}

	return csRet;
}

//=============================================================================
// Method		: FormatIP
// Access		: public 
// Returns		: BOOL
// Parameter	: LPTSTR pszIPAddr
// Parameter	: UINT nSize
// Parameter	: DWORD dwAddr
// Qualifier	:
// Last Update	: 2010/12/5 - 13:40
// Desc.		:
//=============================================================================
BOOL FormatIP(LPTSTR pszIPAddr, UINT nSize, DWORD dwAddr)
{

	if ( pszIPAddr && nSize > 8)
	{		
		return (SUCCEEDED(StringCchPrintf(pszIPAddr, nSize, TEXT("%u.%u.%u.%u"),
												(UINT)(((PBYTE) &dwAddr)[0]),
												(UINT)(((PBYTE) &dwAddr)[1]),
												(UINT)(((PBYTE) &dwAddr)[2]),
												(UINT)(((PBYTE) &dwAddr)[3]))));
	}
	
	/*BYTE byIP[4];
	byIP[0] = (BYTE)FIRST_IPADDRESS(m_dwIP);
	byIP[1] = (BYTE)SECOND_IPADDRESS(m_dwIP);
	byIP[2] = (BYTE)THIRD_IPADDRESS(m_dwIP);
	byIP[3] = (BYTE)FOURTH_IPADDRESS(m_dwIP);
	m_strIP.Format("%d.%d.%d.%d", byIP[0], byIP[1], byIP[2], byIP[3]);*/

	return FALSE;
}

//=============================================================================
// Method		: ConvIPAddrToString
// Access		: public 
// Returns		: CString
// Parameter	: DWORD dwAddr
// Qualifier	:
// Last Update	: 2010/12/5 - 13:54
// Desc.		: DWORD 형태의 IP 주소를 문자열로 변환
//=============================================================================
CString	ConvIPAddrToString (DWORD dwAddr)
{
	CString strResturn;

	DWORD dwAddress = ntohl(dwAddr);
	strResturn = inet_ntoa(*(IN_ADDR*)&dwAddress);

	return strResturn;
}

//=============================================================================
// Method		: ConvIPStringToAddr
// Access		: public 
// Returns		: DWORD
// Parameter	: LPCSTR lpszIPString
// Qualifier	:
// Last Update	: 2010/12/5 - 13:54
// Desc.		: IP 주소 문자열을 DWORD 형태의 IP 주소로 변환
//=============================================================================
DWORD ConvIPStringToAddr (LPCSTR lpszIPString)
{
	return  ntohl(inet_addr(lpszIPString));
}

//=============================================================================
// Method		: GetMyIPAddress
// Access		: public 
// Returns		: CString
// Qualifier	:
// Last Update	: 2013/3/14 - 15:02
// Desc.		:
//=============================================================================
CString GetMyIPAddress()
{
	char		chName[255] = {0,};
	CString		sAddress;
	PHOSTENT	pHostEntry;
	IN_ADDR		inAddr;

	if( gethostname( chName, 255 ) != 0 )
	{
		return _T("");
	}
	else
	{
		if( ( pHostEntry = gethostbyname( chName ) ) == NULL )
		{
			return _T("");
		}
		else
		{
			memcpy( &inAddr, pHostEntry->h_addr, 4 );
			sAddress.Format( _T("%s"), inet_ntoa( inAddr ) );			
			return sAddress;
		}
	}
}

//=============================================================================
// Method		: EraseTrayIcon
// Access		: public 
// Returns		: void
// Qualifier	:
// Last Update	: 2010/12/28 - 13:26
// Desc.		: 트레이 아이콘 잔상이 남는 버그 발생시 잔상 제거용 함수
//=============================================================================
void EraseTrayIcon ()
{
	// 트레이 잔상 제거.
	HWND hShellTrayWnd = ::FindWindowEx(NULL, NULL, _T("Shell_TrayWnd"),NULL);
	if(hShellTrayWnd)
	{
		HWND hTrayNotifyWnd = ::FindWindowEx(hShellTrayWnd, NULL, _T("TrayNotifyWnd"), NULL);
		if(hTrayNotifyWnd)
		{
			HWND hSysPager = ::FindWindowEx(hTrayNotifyWnd, NULL, _T("SysPager"), NULL);
			if (hSysPager)
			{
				HWND hToolBarWindow = ::FindWindowEx(hSysPager, NULL, _T("ToolbarWindow32"), NULL);
				RECT rect;
				::GetClientRect( hToolBarWindow,&rect );
				for(int x = 0; x < rect.right - rect.left; x+=8)
				{
					for(int y = 0; y < rect.bottom - rect.top; y+=8)
					{
						::PostMessage(hToolBarWindow,WM_MOUSEMOVE,0,MAKELPARAM(x,y));
					}
				}
			}
		}
	}// 트레이 잔상 제거 끝.
}

#include <Tlhelp32.h.>
//=============================================================================
// Method		: FocredCloseProcess
// Access		: public 
// Returns		: BOOL
// Parameter	: LPCTSTR lpszProcessName
// Parameter	: LPCTSTR lpszFuncDesc
// Qualifier	:
// Last Update	: 2010/12/28 - 13:31
// Desc.		: 프로세스 강제 종료
//=============================================================================
BOOL FocredCloseProcess(LPCTSTR lpszProcessName, LPCTSTR lpszFuncDesc)
{
	HANDLE hSnapShot;
	hSnapShot=CreateToolhelp32Snapshot (TH32CS_SNAPALL, NULL);

	PROCESSENTRY32 pEntry;
	pEntry.dwSize =sizeof(pEntry);

	TCHAR szProcessInfo[255];

	Process32First (hSnapShot, &pEntry);	
	CString strGetName;

	//Iterate thru all processes
	while(1)
	{
		BOOL hRes=Process32Next (hSnapShot, &pEntry);

		if(hRes==FALSE)
			break;
		
		StringCbPrintf(szProcessInfo, sizeof(szProcessInfo), _T("%d"), pEntry.th32ProcessID );
		strGetName.Format(_T("%s"), pEntry.szExeFile);
		strGetName.MakeLower();		

		if(strGetName == lpszProcessName)
		{
			HANDLE hProcess = ::OpenProcess(
											PROCESS_TERMINATE, 
											FALSE,
											pEntry.th32ProcessID
											);

			if (NULL != hProcess)
			{
				if (::TerminateProcess(hProcess, 0))
				{
					return TRUE;
				}
				//else Error
			}
			else 
			{				
				LPVOID lpMsgBuf;
				CString lpDisplayBuf;
				DWORD dw = GetLastError(); 

				FormatMessage(
								FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
								NULL,
								dw,
								MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
								(LPTSTR) &lpMsgBuf,
								0,
								NULL );

				// Display the error message and exit the process
				lpDisplayBuf.Format (TEXT("%s failed with error %d: %s"), lpszFuncDesc, dw, (char*)lpMsgBuf);
				::MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK); 

				LocalFree(lpMsgBuf);
			}
		}
	}

	return FALSE;
}

//=============================================================================
// Method		: NTCreateProcess
// Access		: public 
// Returns		: BOOL
// Parameter	: PROCESS_INFORMATION & ProcessInformation
// Parameter	: LPCTSTR lpszCurrentDir
// Parameter	: LPCTSTR lpApplicationName
// Parameter	: LPCTSTR lpszWindowName
// Qualifier	:
// Last Update	: 2010/12/28 - 14:01
// Desc.		:
//=============================================================================
BOOL NTCreateProcess (PROCESS_INFORMATION &ProcessInformation, LPCTSTR lpszTargetName, LPTSTR lpszCurrentFullPath)
{
	TCHAR drive[_MAX_DRIVE];
	TCHAR dir[_MAX_DIR];	
	TCHAR file[_MAX_FNAME];
	TCHAR ext[_MAX_EXT];
	_tsplitpath_s(lpszCurrentFullPath, drive, _MAX_DRIVE, dir, _MAX_DIR, file, _MAX_FNAME, ext, _MAX_EXT);

	CString lpszCurrentDir;
	lpszCurrentDir.Format(_T("%s\\%s"), drive, dir);
	CString lpApplicationName;
	lpApplicationName.Format(_T("%s%s.exe"), lpszCurrentDir.GetBuffer(), lpszTargetName);	

	CString strCommandLine;
	strCommandLine.Format(_T("\"%s\""), lpszCurrentFullPath);
	//strCommandLine.Format(_T("%s"), lpszCurrentFullPath);

	STARTUPINFO			StartupInfo;

	memset(&StartupInfo,		0, sizeof(STARTUPINFO));
	memset(&ProcessInformation, 0, sizeof(PROCESS_INFORMATION));

	
	HWND hWnd = ::FindWindow(NULL, lpszTargetName);

	if(hWnd == NULL)
	{				
		BOOL bRet = CreateProcess(
									lpApplicationName.GetBuffer(),	// 실행파일 모듈의 이름에 대한 포인터 
									lpszCurrentFullPath,// 커맨드 문자열에 대한 포인터 
									NULL,				// 프로세스 보안 속성 포인터
									NULL,				// 스레드 보안속성 포인터
									FALSE,				// 핸들 상속 여부 플래그 
									0,					// 생성 플래그 
									NULL,				// 환경 블록에 대한 포인터 
									lpszCurrentDir.GetBuffer(),		// 현재 디렉토리 
									&StartupInfo,		// STARTUPINFO 포인터 
									&ProcessInformation // PROCESS_INFORMATION 포인터 
									);

		if(bRet==FALSE)
		{			
			return FALSE;
		}		 
	}

	return TRUE;
}

//=============================================================================
// Method		: NTExitProcess
// Access		: public 
// Returns		: void
// Parameter	: PROCESS_INFORMATION & ProcessInformation
// Qualifier	:
// Last Update	: 2010/12/28 - 13:54
// Desc.		:
//=============================================================================
void NTExitProcess(PROCESS_INFORMATION &ProcessInformation)
{
	if (ProcessInformation.hProcess)
	{
		DWORD       dwExitCode;
		GetExitCodeProcess( ProcessInformation.hProcess, &dwExitCode);
		TerminateProcess( ProcessInformation.hProcess, dwExitCode); 
		CloseHandle(ProcessInformation.hProcess);
	}

	//EraseTrayIcon();
}


#include <Psapi.h>
//=============================================================================
// Method		: ForcedExitProcess
// Access		: public 
// Returns		: void
// Parameter	: CString strTargetProcName
// Parameter	: DWORD dwExceptProcId
// Qualifier	:
// Last Update	: 2010/12/28 - 19:16
// Desc.		:
//=============================================================================
void ForcedExitProcess (CString strTargetProcName, DWORD dwExceptProcId)
{
	//DWORD aProcesses[1024]={0,}, cbNeeded, cProcesses;
	//unsigned int i = 0;

	////실행중인 프로세스를 모두 구한다
	//if (!EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded))
	//	return;

	////실행중인 프로세스의 개수를 구한다
	//cProcesses = cbNeeded / sizeof(DWORD);
	////각 프로세스에 대한 이름 및 프로세스 아이디를 구하고 타겟 프로세스를 강제로 종료시킨다
	//for ( i = 0; i < cProcesses; i++ )
	//{
	//	TCHAR szProcessName[MAX_PATH] = _T("unknown");
	//	HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, aProcesses[i] );

	//	if (NULL != hProcess )
	//	{
	//		HMODULE hMod;
	//		DWORD cbNeeded;

	//		if (EnumProcessModules( hProcess, &hMod, sizeof(hMod), &cbNeeded))
	//			GetModuleBaseName( hProcess, hMod, szProcessName, sizeof(szProcessName)); //프로세스 이름을 구한다(szProcessName)
	//	}

	//	BOOL bRet;
	//	CString strComp = szProcessName;
	//	//실행중인 프로세스(strComp)와 죽이려는 프로세스(strTargetProcName) 이름을 비교하여 같으면 해당 프로세스 종료
	//	//단 실행중인 나 자신은 죽이려는 프로세스에서 제외시킨다
	//	if((strComp.MakeLower() == strTargetProcName.MakeLower()) && (aProcesses[i] != dwExceptProcId))
	//	{
	//		HANDLE hKillProc = OpenProcess(PROCESS_TERMINATE, FALSE, aProcesses[i]);

	//		if(hKillProc != NULL)
	//		{
	//			DWORD ExitCode = 0;

	//			GetExitCodeProcess( hKillProc, &ExitCode );
	//			bRet = TerminateProcess(hKillProc, ExitCode );
	//			if( bRet )
	//				WaitForSingleObject(hKillProc, INFINITE);

	//			CloseHandle(hKillProc);
	//		}
	//	}
	//	CloseHandle( hProcess );
	//}
}

// 해당 동작 확인을 위해서는 프로젝트를 관리자 권한으로 실행하여 한다.
BOOL  IsFindProcess(CString szExt, CString szPath)
{
	BOOL bFind = FALSE;

	// 실행중인 모든 프로세스 찾기
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 processEntry32;

	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		exit(EXIT_FAILURE);
	}

	processEntry32.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(hProcessSnap, &processEntry32))
	{
		CloseHandle(hProcessSnap);
		exit(EXIT_FAILURE);
	}

	while (Process32Next(hProcessSnap, &processEntry32))
	{
		// 실행중인 프로세스 비교
		if (szExt.Compare(processEntry32.szExeFile) == 0)
		{
			// 실행중인 프로세스가 있다면
			HANDLE hExe = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processEntry32.th32ProcessID);

			HMODULE hMod;
			TCHAR szFullPath[MAX_PATH] = { 0, };

			if (hExe != NULL)
			{
				DWORD dwSize = 0;
				LPTSTR pszName = NULL;
				if (EnumProcessModules(hExe, &hMod, sizeof(hMod), &dwSize))
				{
					GetModuleFileNameEx(hExe, hMod, szFullPath, _countof(szFullPath));

					if (szPath.Compare(szFullPath) == 0)
						return TRUE;
				}
			}
		}
	}

	// 실행중인 프로세스 중에 찾는 프로세스가 없다면
	return bFind;
}

// 프로세스 ID 구하기
//GetWindowThreadProcessId(hWnd, &dwProcId);

//=============================================================================
// Method		: GetPingResultCodeString
// Access		: public 
// Returns		: CString
// Parameter	: ULONG lStatus
// Qualifier	:
// Last Update	: 2013/6/27 - 20:33
// Desc.		:
//=============================================================================
CString	GetPingResultCodeString	(ULONG lStatus)
{
	CString strStatus;	

	switch (lStatus)
	{
	case 11001:
		strStatus = "buffer too small";
		break;
	case 11002:
		strStatus = "Destination Net Unreachable";
		break;
	case 11003:
		strStatus = "Destination Host Unreachable";
		break;
	case 11004:
		strStatus = "Destination Protocol Unreachable";
		break;
	case 11005:
		strStatus = "Destination Port Unreachable";
		break;
	case 11006:
		strStatus = "No Resource";
		break;
	case 11007:
		strStatus = "Bad Option";
		break;
	case 11008:
		strStatus = "Hardware Error";
		break;
	case 11009:
		strStatus = "Packet Too Big";
		break;
	case 11010:
		strStatus = "request time out";
		break;
	case 11011:
		strStatus = "Bad request";
		break;
	case 11012:
		strStatus = "Bad Route";
		break;
	case 11013:
		strStatus = "Time To Live Expired Transit";
		break;
	case 11014:
		strStatus = "Time To Live Expired Reassembly";
		break;
	case 11015:
		strStatus = "Parameter Problem";
		break;
	case 11016:
		strStatus = "Source Quench";
		break;
	case 11017:
		strStatus = "Option Too Big";
		break;
	case 11018:
		strStatus = "Bad Destination";
		break;
	case 11032:
		strStatus = "Negotiating IPSEC";
		break;
	case 11050:
		strStatus = "General Failure";
		break;
	default:
		strStatus = "other failure";
		break;
	}

	return strStatus;
}

//=============================================================================
// Method		: _tinet_addr
// Access		: public  
// Returns		: DWORD
// Parameter	: const TCHAR * cp
// Qualifier	:
// Last Update	: 2016/9/28 - 11:39
// Desc.		:
//=============================================================================
DWORD _tinet_addr(const TCHAR *cp)
{
#ifdef UNICODE
	char IP[16];
	int Ret = 0;
	Ret = WideCharToMultiByte(CP_ACP, 0, cp, (int)_tcslen(cp), IP, 15, NULL, NULL);
	IP[Ret] = 0;
	return inet_addr(IP);
#else
	return inet_addr(cp);
#endif
}

// *** IP & Mac 어드레스 구하기 *** //
/*#pragma comment(lib, "Iphlpapi.lib")
#include <Iphlpapi.h>

//성공 1, 실패 0 리턴
BOOL GetIP(CStringArray& strIPArray)
{
	CStringA strIPAddress;

	PIP_ADAPTER_INFO pAdapterInfo;
	PIP_ADAPTER_INFO pAdapter = NULL;
	DWORD dwRetVal = 0;

	ULONG ulOutBufLen = sizeof (IP_ADAPTER_INFO);
	pAdapterInfo = new IP_ADAPTER_INFO[ulOutBufLen];

	if (pAdapterInfo == NULL)
		return FALSE;

	if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW)
	{
		delete pAdapterInfo;
		pAdapterInfo = new IP_ADAPTER_INFO[ulOutBufLen];
		if (pAdapterInfo == NULL)
			return FALSE;
	}

	if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR)
	{
		pAdapter = pAdapterInfo;
		while (pAdapter)
		{
			strIPAddress.Format("%s", pAdapter->IpAddressList.IpAddress.String);
			strIPArray.Add(CA2T(strIPAddress));
			pAdapter = pAdapter->Next;
		}
	}

	delete pAdapterInfo;
	pAdapterInfo = NULL;
	return TRUE;
}

//성공 1, 실패 0 리턴
BOOL GetMacAddress(CStringArray& strMacArray, CString strSpecifiedIP)
{
	PIP_ADAPTER_INFO pAdapterInfo;
	PIP_ADAPTER_INFO pAdapter = NULL;
	DWORD dwRetVal = 0;

	ULONG ulOutBufLen = sizeof (IP_ADAPTER_INFO);
	pAdapterInfo = new IP_ADAPTER_INFO[ulOutBufLen];

	if (pAdapterInfo == NULL)
		return FALSE;

	if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW)
	{
		delete pAdapterInfo;
		pAdapterInfo = new IP_ADAPTER_INFO[ulOutBufLen];
		if (pAdapterInfo == NULL)
			return FALSE;
	}

	if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR)
	{
		pAdapter = pAdapterInfo;
		while (pAdapter)
		{
			CStringA strMacAddress;
			strMacAddress.Format("%02X:%02X:%02X:%02X:%02X:%02X",
				pAdapter->Address[0],
				pAdapter->Address[1],
				pAdapter->Address[2],
				pAdapter->Address[3],
				pAdapter->Address[4],
				pAdapter->Address[5]);

			CStringA strIPAddress;
			strIPAddress.Format("%s", pAdapter->IpAddressList.IpAddress.String);

			if(strSpecifiedIP.GetLength() > 0)
			{
				if(strIPAddress.Find(CT2A(strSpecifiedIP)) >= 0)
				{
					strMacArray.Add(CA2T(strMacAddress));
					break;
				}
			}
			else
				strMacArray.Add(CA2T(strMacAddress));

			pAdapter = pAdapter->Next;
		}
	}

	delete pAdapterInfo;
	pAdapterInfo = NULL;
	return TRUE;
}*/

// bool isLeapYear (UINT nYear)
// {
// 	return (((nYear % 4) == 0) && ((nYear % 100) != 0) || (nYear % 400));
// }

//=============================================================================
// Method		: getDaysInMonth
// Access		: public  
// Returns		: UINT
// Parameter	: UINT nMonth
// Parameter	: UINT nYear
// Qualifier	:
// Last Update	: 2014/7/10 - 22:29
// Desc.		:
//=============================================================================
UINT getDaysInMonth (UINT nMonth, UINT nYear)
{
	static UINT aDaysInMonth[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	if (nMonth != 2)
		return aDaysInMonth[nMonth - 1];
	else
	{
		//if (isLeapYear (nYear))
		if (((nYear % 4) == 0) && ((nYear % 100) != 0) || (nYear % 400))
			return aDaysInMonth[nMonth - 1] + 1;
		else
			return aDaysInMonth[nMonth -1];
	}

}

//=============================================================================
// Method		: UTF8toANSI
// Access		: public  
// Returns		: char*
// Parameter	: char * pszCode
// Qualifier	:
// Last Update	: 2014/7/31 - 11:56
// Desc.		: 한글 깨짐
//=============================================================================
char* UTF8toANSI(char *pszCode)
{   
	BSTR    bstrWide;   
	char*   pszAnsi;   
	int     nLength;       

	// Get nLength of the Wide Char buffer   
	nLength = MultiByteToWideChar(CP_UTF8, 0, pszCode, lstrlenA(pszCode) + 1,NULL, NULL);   
	bstrWide = SysAllocStringLen(NULL, nLength);   

	// Change UTF-8 to Unicode (UTF-16)   
	MultiByteToWideChar(CP_UTF8, 0, pszCode, lstrlenA(pszCode) + 1, bstrWide,nLength);   


	// Get nLength of the multi byte buffer    
	nLength = WideCharToMultiByte(CP_ACP, 0, bstrWide, -1, NULL, 0, NULL, NULL);   
	pszAnsi = new char[nLength];   


	// Change from unicode to mult byte   
	WideCharToMultiByte(CP_ACP, 0, bstrWide, -1, pszAnsi, nLength, NULL, NULL);   

	SysFreeString(bstrWide);   


	return pszAnsi;   
} 

//=============================================================================
// Method		: IsWow64
// Access		: public  
// Returns		: BOOL
// Qualifier	:
// Last Update	: 2015/11/25 - 19:24
// Desc.		:
//=============================================================================
typedef BOOL(WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
LPFN_ISWOW64PROCESS fnIsWow64Process;
BOOL IsWow64()
{
	BOOL bIsWow64 = FALSE;

	fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandle(TEXT("Kernel32")), "IsWow64Process");

	if (fnIsWow64Process != NULL)
	{
		if (!fnIsWow64Process(GetCurrentProcess(), &bIsWow64))
		{
			// handle error
		}
	}

	return bIsWow64;
}

//=============================================================================
// Method		: Is64BitWindows
// Access		: public  
// Returns		: BOOL
// Qualifier	:
// Last Update	: 2015/11/25 - 19:24
// Desc.		:
//=============================================================================
BOOL Is64BitWindows()
{
#if defined(_WIN64)
	return TRUE;  // 64-bit programs run only on Win64
#elif defined(_WIN32)
	// 32-bit programs run on both 32-bit and 64-bit Windows
	// so must sniff
	BOOL f64 = FALSE;
	return IsWow64Process(GetCurrentProcess(), &f64) && f64;
#else
	return FALSE; // Win64 does not support Win16
#endif
}

CString GetDefaultPrinterName()
{
	CString strPrinterName = TEXT("");
	OSVERSIONINFO osv;

	// --- Get the operationg system versin info ---
	osv.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osv);

	BOOL bRet = FALSE;
	// If Windows 95 or 98, use EnumPrinters.
	if (osv.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
	{
		// The first EnumPrinters() tells you how big our buffer must
		// be to hold ALL of PRINTER_INFO_2. Note that this will
		// typically return FALSE. This only means that the buffer (the 4th
		// parameter) was not filled in. You do not want it filled in here.
		DWORD dwNeeded = 0, dwReturned = 0;
		SetLastError(0);
		bRet = EnumPrinters(PRINTER_ENUM_DEFAULT, NULL, 2, NULL, 0, &dwNeeded, &dwReturned);
		if ((GetLastError() != ERROR_INSUFFICIENT_BUFFER) || (dwNeeded == 0))
			return CString("");

		PRINTER_INFO_2 * ppi2 = reinterpret_cast<PRINTER_INFO_2 *>(new char[dwNeeded]);
		if (!ppi2)
			return CString("");

		// The second EnumPrinters() will fill in all the current information.
		bRet = EnumPrinters(PRINTER_ENUM_DEFAULT, NULL, 2, (LPBYTE)ppi2, dwNeeded, &dwNeeded, &dwReturned);
		if (!bRet)
		{
			char * pTemp = reinterpret_cast<char *>(ppi2);
			delete[]pTemp;

			return CString("");
		}

		// --- Get the printer name ---
		strPrinterName = ppi2->pPrinterName;

		// --- Free memory ---
		char * pTemp = reinterpret_cast<char *>(ppi2);
		delete[]pTemp;
	}

	// If Windows NT, use the GetDefaultPrinter API for Windows 2000,
	// or GetProfileString for version 4.0 and earlier.
	else if (osv.dwPlatformId == VER_PLATFORM_WIN32_NT)
	{
		// Windows 2000 or later (use explicit call)
		// --- Call GetDefaultPrinter() to get the printer name ---
		if (osv.dwMajorVersion >= 5)
		{
			// --- Load library winspool.drv ---
			HMODULE hSpoolDrv = LoadLibrary(_T("winspool.drv"));
			if (!hSpoolDrv)
				return CString("");

			// --- function type definition ---
			typedef BOOL(FAR PASCAL *FNGETPRINTER)(LPTSTR, LPDWORD);
			FNGETPRINTER fnGetPrinter = (FNGETPRINTER)GetProcAddress(hSpoolDrv, "GetDefaultPrinter");
			if (fnGetPrinter)
			{
				LPTSTR szPrinterName[MAX_PATH];
				DWORD nLen = MAX_PATH;
				bRet = fnGetPrinter((LPTSTR)szPrinterName, &nLen);
				// --- Function call succeeds, then set the printer name ---
				if (bRet)
					strPrinterName = (char *)szPrinterName;
			}
			FreeLibrary(hSpoolDrv);
		}
		else// --- NT4.0 or earlier ---
		{
			// Retrieve the default string from Win.ini (the registry).
			// String will be in form "printername,drivername,portname".
			TCHAR szBuffer[MAX_PATH];
			// --- HKEY_CURRENT_USER\Software\Microsoft\Windows NT\CurrentVersion\Windows\Version ---
			if (GetProfileString(_T("windows"), _T("device"), _T(",,,"), szBuffer, MAX_PATH) <= 0)
				return CString("");

			// Printer name precedes first "," character.
			_tcstok(szBuffer, _T(","));

			if (lstrlen(szBuffer) <= 0)
				return CString("");

			// --- Set the printer name ---
			strPrinterName = szBuffer;
		}
	}

	return strPrinterName;
}

//=============================================================================
// Method		: KeyBoardEvent
// Access		: public  
// Returns		: BOOL
// Parameter	: const char * lpcBuffer
// Parameter	: DWORD dwBufferSize
// Qualifier	:
// Last Update	: 2016/6/6 - 15:38
// Desc.		:
//=============================================================================
BOOL KeyBoardEvent(const char* lpcBuffer, DWORD dwBufferSize)
{
	if (GetKeyState(VK_HANGEUL))
	{
		TRACE(_T("*** 한->영 전환 \n"));
		keybd_event(VK_HANGEUL, MapVirtualKey(VK_HANGEUL, 0), 0, 0);
		keybd_event(VK_HANGEUL, MapVirtualKey(VK_HANGEUL, 0), KEYEVENTF_KEYUP, 0);
		Delay(10);
	}

	if (GetKeyState(VK_CAPITAL))
	{
		TRACE(_T("*** CapsLock 전환 \n"));
		keybd_event(VK_CAPITAL, MapVirtualKey(VK_CAPITAL, 0), 0, 0);
		keybd_event(VK_CAPITAL, MapVirtualKey(VK_CAPITAL, 0), KEYEVENTF_KEYUP, 0);
		Delay(1);
	}

	BOOL	bUseShift = FALSE;

	for (DWORD dwIndex = 0; dwIndex < dwBufferSize; dwIndex++)
	{
		if (isupper((int)lpcBuffer[dwIndex])) // 대문자
			bUseShift = TRUE;
		else // 소문자
			bUseShift = FALSE;

		if (bUseShift)
		{
			TRACE(_T("Shift"));
			keybd_event(VK_SHIFT, MapVirtualKey(VK_SHIFT, 0), 0, 0);
		}

		keybd_event(lpcBuffer[dwIndex], 0, 0, 0);
		keybd_event(lpcBuffer[dwIndex], 0, KEYEVENTF_KEYUP, 0);
		TRACE(_T("[%d] : %c \n"), dwIndex, lpcBuffer[dwIndex]);

		if (bUseShift)
		{
			keybd_event(VK_SHIFT, MapVirtualKey(VK_SHIFT, 0), KEYEVENTF_KEYUP, 0);
		}
	}

	TRACE(_T("\n"));
	return TRUE;
}

//=============================================================================
// Method		: RunTouchKeyboard
// Access		: public  
// Returns		: BOOL
// Qualifier	:
// Last Update	: 2016/10/27 - 11:30
// Desc.		:
//=============================================================================
BOOL RunTouchKeyboard()
{
	BOOL bReturn = TRUE;

	const int sysDirNameSize = 1024;
	TCHAR sysDir[sysDirNameSize];
	if (!GetSystemDirectory(sysDir, sysDirNameSize))
	{
		ASSERT(FALSE);
		return FALSE;
	}

	//CString osk_path = CString(sysDir) + _T("\\osk.exe");
	TCHAR szOsk_Path[MAX_PATH] = { 0, 0 };
	_stprintf_s(szOsk_Path, MAX_PATH, _T("%s\\osk.exe"), sysDir);
	PVOID pOldValue = NULL;

	__try
	{
//#ifdef _WIN64
		if (Is64BitWindows())
		{
			BOOL bRes = Wow64DisableWow64FsRedirection(&pOldValue);
			::ShellExecute(NULL, NULL, szOsk_Path, _T(""), sysDir, SW_SHOW);
			if (bRes)
				Wow64RevertWow64FsRedirection(pOldValue);
		}
		else
		{
			::ShellExecute(NULL, NULL, szOsk_Path, _T(""), sysDir, SW_SHOW);
		}
// #else
// 		::ShellExecute(NULL, NULL, szOsk_Path, _T(""), sysDir, SW_SHOW);
// 		//ShellExecute(NULL, TEXT("open"), TEXT("osk.exe"), NULL, NULL, SWP_SHOWWINDOW);
// #endif
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		bReturn = FALSE;
		AfxMessageBox(_T("시스템 화상 키보드를 시작할 수 없습니다."), MB_SYSTEMMODAL);
	}

	return TRUE;
}

//=============================================================================
// Method		: NoatepadDebug
// Access		: public  
// Returns		: void
// Parameter	: __in LPCTSTR szDebugString
// Qualifier	:
// Last Update	: 2018/12/2 - 8:17
// Desc.		:
//=============================================================================
void NoatepadDebug(__in LPCTSTR szDebugString)
{
	SYSTEMTIME _starttime;
	GetLocalTime(&_starttime);

	TCHAR szTime[MAX_PATH] = { 0, };

	_stprintf_s(szTime, MAX_PATH, _T("[%02d:%02d:%02d:%04d] ")
		, _starttime.wHour
		, _starttime.wMinute
		, _starttime.wSecond
		, _starttime.wMilliseconds);
	//  시스템 죽지않는 현상 해결 방법...............
	HWND	hNotePad(NULL);
	hNotePad = ::FindWindow(NULL, _T("notepaddebug.txt - Notepad"));
	if (hNotePad == NULL)
	{
		hNotePad = ::FindWindow(NULL, _T("notepaddebug - Notepad"));
		if (hNotePad == NULL)
		{
			hNotePad = ::FindWindow(NULL, _T("notepaddebug.txt - 메모장"));
			if (hNotePad == NULL)
				hNotePad = ::FindWindow(NULL, _T("notepaddebug - 메모장"));
		}
	}

	if (hNotePad != NULL)
	{
		HWND hContext = ::GetWindow(hNotePad, GW_CHILD);
		if (hContext)
		{
			LRESULT i = ::SendMessage(hContext, WM_GETTEXTLENGTH, 0, 0);
			::SendMessage(hContext, EM_SETSEL, i, i);
			::SendMessage(hContext, EM_REPLACESEL, 0, (LPARAM)(LPCTSTR)szTime);
			LRESULT j = ::SendMessage(hContext, WM_GETTEXTLENGTH, 0, 0);
			::SendMessage(hContext, EM_SETSEL, j, j);
			::SendMessage(hContext, EM_REPLACESEL, 0, (LPARAM)szDebugString);
			LRESULT k = ::SendMessage(hContext, WM_GETTEXTLENGTH, 0, 0);
			::SendMessage(hContext, EM_SETSEL, k, k);
		}
	}
}

//=============================================================================
// Method		: GetMin
// Access		: public  
// Returns		: double
// Parameter	: double* dbScore
// Parameter	: UINT nSize
// Qualifier	:
// Last Update	: 2018/1/16 - 14:33
// Desc.		:
//=============================================================================
double GetMin(double* dbScore, UINT nSize)
{
	double dbMin = dbScore[0];

	for (UINT nIdx = 1; nIdx < nSize; nIdx++)
	{
		if (dbScore[nIdx] < dbMin)
		{
			dbMin = dbScore[nIdx];
		}
	}

	return dbMin;
}

//=============================================================================
// Method		: GetMin
// Access		: public  
// Returns		: WORD
// Parameter	: WORD * wScore
// Parameter	: UINT nSize
// Qualifier	:
// Last Update	: 2018/11/24 - 15:47
// Desc.		:
//=============================================================================
WORD GetMin(WORD* wScore, UINT nSize)
{
	WORD wMin = wScore[0];

	for (UINT nIdx = 1; nIdx < nSize; nIdx++)
	{
		if (wScore[nIdx] < wMin)
		{
			wMin = wScore[nIdx];
		}
	}

	return wMin;
}

//=============================================================================
// Method		: GetMin
// Access		: public  
// Returns		: float
// Parameter	: float * fScore
// Parameter	: UINT nSize
// Qualifier	:
// Last Update	: 2018/11/24 - 15:47
// Desc.		:
//=============================================================================
float GetMin(float* fScore, UINT nSize)
{
	float fMin = fScore[0];

	for (UINT nIdx = 1; nIdx < nSize; nIdx++)
	{
		if (fScore[nIdx] < fMin)
		{
			fMin = fScore[nIdx];
		}
	}

	return fMin;
}

//=============================================================================
// Method		: GetAvg
// Access		: public  
// Returns		: double
// Parameter	: double * dbScore
// Parameter	: UINT nSize
// Qualifier	:
// Last Update	: 2019/1/10 - 18:35
// Desc.		:
//=============================================================================
double GetAvg(double* dbScore, UINT nSize)
{
	double dbSum = 0.0;

	for (UINT nIdx = 0; nIdx < nSize; nIdx++)
	{
		dbSum += dbScore[nIdx];
	}

	dbSum /= nSize;

	return dbSum;
}

//=============================================================================
// Method		: GetMax
// Access		: public  
// Returns		: double
// Parameter	: double * dbScore
// Parameter	: UINT nSize
// Qualifier	:
// Last Update	: 2018/10/30 - 13:29
// Desc.		:
//=============================================================================
double GetMax(double* dbScore, UINT nSize)
{
	double dbMax = dbScore[0];

	for (UINT nIdx = 1; nIdx < nSize; nIdx++)
	{
		if (dbScore[nIdx] > dbMax)
		{
			dbMax = dbScore[nIdx];
		}
	}

	return dbMax;
}

//=============================================================================
// Method		: GetMax
// Access		: public  
// Returns		: double
// Parameter	: DWORD * dwScore
// Parameter	: UINT nSize
// Qualifier	:
// Last Update	: 2018/11/24 - 13:39
// Desc.		:
//=============================================================================
WORD GetMax(WORD* wScore, UINT nSize)
{
	WORD wMax = wScore[0];

	for (UINT nIdx = 1; nIdx < nSize; nIdx++)
	{
		if (wScore[nIdx] > wMax)
		{
			wMax = wScore[nIdx];
		}
	}

	return wMax;
}

//=============================================================================
// Method		: GetMax
// Access		: public  
// Returns		: float
// Parameter	: float * fScore
// Parameter	: UINT nSize
// Qualifier	:
// Last Update	: 2018/11/24 - 13:54
// Desc.		:
//=============================================================================
float GetMax(float* fScore, UINT nSize)
{
	float fMax = fScore[0];

	for (UINT nIdx = 1; nIdx < nSize; nIdx++)
	{
		if (fScore[nIdx] > fMax)
		{
			fMax = fScore[nIdx];
		}
	}

	return fMax;
}

//=============================================================================
// Method		: GetMid
// Access		: public  
// Returns		: double
// Parameter	: double * dbScore
// Parameter	: UINT nSize
// Parameter	: UINT nFind
// Qualifier	:
// Last Update	: 2018/11/15 - 15:17
// Desc.		:
//=============================================================================
double GetMid(double* dbScore, UINT nSize, UINT nFind)
{
	if (0 >= nSize)
		return 0.0;

	for (UINT i = 0; i < nSize; i++)
	{
		for (UINT nIdx = 0; nIdx < nSize - i - 1; nIdx++)
		{
			if (dbScore[nIdx] < dbScore[nIdx + 1])
			{
				double dbTemp = 0.0;

				dbTemp = dbScore[nIdx];
				dbScore[nIdx] = dbScore[nIdx + 1];
				dbScore[nIdx + 1] = dbTemp;
			}
		}
	}

	double dbResult = dbScore[nFind];

	return dbResult;
}

//=============================================================================
// Method		: GetMedian
// Access		: public  
// Returns		: double
// Parameter	: double * dbScore
// Parameter	: UINT nSize
// Qualifier	:
// Last Update	: 2019/1/14 - 8:59
// Desc.		:
//=============================================================================
double GetMedian(double* dbScore, UINT nSize)
{
	if (0 >= nSize)
		return 0.0;

	double dbMedian;
	double dbTemp;

	for (UINT i = 0; i < nSize - 1; i++)
	{
		for (UINT j = 0; j < nSize - i - 1; j++)
		{
			if (dbScore[j + 1] < dbScore[j])
			{
				dbTemp = dbScore[j];
				dbScore[j] = dbScore[j + 1];
				dbScore[j + 1] = dbTemp;
			}
		}
	}

	UINT nCnt = nSize / 2;

	if (nSize % 2 == 0)
	{
		// dbMedian = (dbScore[nSize / 2] + dbScore[nSize / 2 - 1]) / 2;
		if (dbScore[nSize / 2] > dbScore[nSize / 2 - 1])
		{
			dbMedian = dbScore[nSize / 2];
		}
		else
		{
			dbMedian = dbScore[nSize / 2 - 1];
		}
	}
	else
	{
		dbMedian = dbScore[nSize / 2];
	}

	return dbMedian;
}

//=============================================================================
// Method		: SetRounding
// Access		: public  
// Returns		: double
// Parameter	: __in double dbData
// Parameter	: __in int iDigit
// Qualifier	:
// Last Update	: 2019/1/22 - 15:10
// Desc.		:
//=============================================================================
double SetRounding(__in double dbData, __in int iDigit)
{
	return (floor((dbData)* pow(float(10), iDigit) + 0.5f) / pow(float(10), iDigit));
}

void SetSystemTimePrivilege()
{
	HANDLE hToken;
	TOKEN_PRIVILEGES tp;

	LUID luid;

	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
	{
		LookupPrivilegeValue(NULL, SE_SYSTEMTIME_NAME, &luid);
		tp.PrivilegeCount = 1;
		tp.Privileges[0].Luid = luid;
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		AdjustTokenPrivileges(hToken, FALSE, &tp, 0, NULL, NULL);
		CloseHandle(hToken);
	}
}

void SetPcTime(__in SYSTEMTIME tm)
{
	SetSystemTimePrivilege();

	SetLocalTime(&tm);
}