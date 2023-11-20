//*****************************************************************************
// Filename	: 	CommonFunction.h
//
// Created	:	
// Modified	:	2010/01/15 - 17:05
//
// Author	:	PiRing
//	
// Purpose	:	여러가지 기능들이 함수들 모음
//*****************************************************************************
#ifndef __COMMONFUNCTION_H__
#define __COMMONFUNCTION_H__

// Macro ----------------------------------------------------------------------
// 리틀/빅 엔디언 판별 매크로
//const int iIs_bigendian = 1;
//#define is_bigendian() ( (*(char*)&iIs_bigendian) == 0 )

//#define CHECK(x,y,n) ( (x > 0 && x < n && y > 0 && y < n) ? 1 : 0)
#define CHECK_RANGE(x, und, ovr) ( (x >= und && x <= ovr ) ? 1 : 0)
#define RANGE_CHECK(value, min, max, fail_return_value) { VALIDATE(min<=value && value<=max, return fail_return_value); }

#define AbsoluteValue(Num)	(Num>=0 ? Num : -Num)
//#define GetPrecentage(Numerator, Denominator)	(Numerator * 100.0 / Denominator)
// static inline double GetPrecentage(double Numerator, double Denominator) 
// {
// 	return (Numerator * 100.0) / Denominator;
// };

template <class T> inline T GetPrecentage(const T& Numerator, const T&Denominator)
{
	return (Numerator * T(100)) / Denominator;
}


#define ClearBits(Data, Area, Loc)		((Data) &= ~((area) << (loc)))		// 연속된 bit Clear
//#define SetBit(Data, Loc)				((Data) |= (0x01 << (loc)))			// 한 bit Set
#define SetBits(Data, Loc)				((Data) |= ((area) << (loc)))		// 연속된 bit
#define InvertBit(Data, Loc)			((Data) ^= (0x1 << (loc)))			// 한 bit 반전
#define InvertBits(Data, Area, Loc)		((Data) ^= ((area) << (loc)))		// 연속된 bit 반전
#define CheckBit(Data, Loc)				((Data) & (0x01 << (loc)))			// 비트 검사
#define ExtractBit(Data, Area, Loc)		(((Data) >> (Loc)) & (Area))		// 비트 추출


#define SetBit(value,bit)			(value) |= 1 << (bit)
//#define SetBit(value, place)		(value | (1 << place))
#define ClearBit(value, place)		(value & (~(1 << place)))
#define ToggleBit(value, place)		(value ^ (1 << place))
#define GetBit(value, place)		((value >> place) & 1)
#define GetMSB(value, datatype)		GetBit(value, (sizeof(datatype) * 4 - 1))
#define GetLSB(value)				GetBit(value, 0)
// Macro ----------------------------------------------------------------------


//=============================================================================
// 문자열 처리
//=============================================================================
//-------------------------------------------------------------------
// 헥스 문자열을 10진수로 바꾼다.
//-------------------------------------------------------------------
int			HexStringToDecimal		(const char *s , int count);
int			HexStringToDecimal		(LPCTSTR szHex);
//-------------------------------------------------------------------
// 헥스 데이타를 아스키 형태의 스트링으로 바꾼다.
//-------------------------------------------------------------------
CString		HexToCString			(LPVOID lpBuff, UINT nBuffSize);
BOOL		HexToString				(LPCSTR lpInBuff, UINT nInSize, LPSTR lpOutBuff, UINT& nOutSize);

//=============================================================================
// Method		: HexToString
// Access		: public  
// Returns		: BOOL
// Parameter	: __in const CHAR * lpInBuff
// Parameter	: __in UINT nInSize
// Parameter	: __out CHAR * lpOutBuff
// Parameter	: __out UINT & nOutSize
// Last Update	: 2016/3/28 - 19:14
// Desc.		:
//=============================================================================
BOOL HexToString (__in const CHAR* lpInBuff, __in UINT nInSize, __out CHAR* lpOutBuff, __out UINT& nOutSize);

//-------------------------------------------------------------------
// Null 문자('\0')가 포함된 데이타를 스페이스 문자(0x20)로 변경한다.
//-------------------------------------------------------------------
CString		NullCharToSpaceChar		(LPVOID lpBuff, DWORD dwBuffSize);

char		HexCharzToByte			(char cFirstChar, char cSecondChar);
BYTE		HexCharToByte			(char cHexChar);

//-------------------------------------------------------------------
// 초 단위의 수치를 시간, 분, 초 단위의 스트링으로 변환한다.
//-------------------------------------------------------------------
CString		SecondToTimeString		(DWORD dwSecond);
CString		MilisecondToTimeString	(DWORD dwMilisecond);

BOOL		StringTimeToSystemTime	(LPCSTR lpszTime, SYSTEMTIME& TimeTarget);
CString		SystemTimeToString		(SYSTEMTIME TimeSource);

BOOL		FormatStringToSystemTimeA(LPCSTR lpszTime, SYSTEMTIME& TimeTarget);
BOOL		FormatStringToSystemTime(LPCTSTR lpszTime, SYSTEMTIME& TimeTarget);
CString		SystemTimeToFormatString(SYSTEMTIME TimeSource);
CString		SystemTimeToFormatString_Month(SYSTEMTIME TimeSource);
double		CompareSystemTime		(PSYSTEMTIME pEndTime, PSYSTEMTIME pStartTime);

CString		GetTestTime				(DWORD start, DWORD end);

//-------------------------------------------------------------------
// 일반 숫자 스트링을 콤마가 포함된 스트링으로 변환
//-------------------------------------------------------------------
CString		DigitToCommaDigitStr	(long long llDigit);

CString	ConvNumberToFormatString(DOUBLE dData, BYTE byLength, BYTE byPoint);

//-------------------------------------------------------------------
// 고정된 길이의 스트링으로 변환
//-------------------------------------------------------------------
//CString	FixedSizeString			(__in CString strSrc, __in INT nSize, __in CHAR chFill = 0x20, __in BOOL bLeftFill = TRUE);
CStringA	FixedSizeString			(__in LPCSTR szSrc, __in INT nSize, __in CHAR chFill = 0x20, __in BOOL bLeftFill = TRUE);

BOOL isStringDouble(char *szString);


//=============================================================================
// 파일 / 디렉토리 처리
//=============================================================================
//-------------------------------------------------------------------
// 파일의 마지막 변경 시간을 얻어온다.
//-------------------------------------------------------------------
BOOL		GetLastWriteTime		(HANDLE hFile, LPTSTR lpszString, DWORD dwSize);
BOOL		GetLastWriteTime		(__in LPCTSTR lpszFile, __out SYSTEMTIME& tmFile);
BOOL		GetLastWriteTime		(__in LPCTSTR lpszFile, __out FILETIME& tmFile);

BOOL		UtilIsDirectoryExists	(LPCTSTR directory);
int			UtilCreateDirectory		(LPCTSTR directory);
int			DeleteAllFiles			(LPCTSTR szDir, int recur);
void		MakeDirectory			(LPCTSTR szPath);
// 디렉토리 검사 및 하위폴더 자동생성
void		MakeSubDirectory		(LPCTSTR lpszDirName);
BOOL		GetDirectoryFile		(__in CString szDir, __in CString szExt, __out CString* lpszFile, __out UINT &nFileCnt);
BOOL		GetDirectoryFileTitle	(__in CString szDir, __in CString szExt, __out CString* lpszFile, __out UINT &nFileCnt);
//=============================================================================
// 동기화 처리
//=============================================================================
//-------------------------------------------------------------------
// 윈도우 메세지 처리하면서 대기하는 기능
//-------------------------------------------------------------------
void		DoEvents				();
void		DoEvents				(DWORD dwMiliSeconds);
BOOL		WaitWithMessageLoop		(HANDLE hEvent, int nTimeout);
void		Delay					(DWORD nTimeMs);
void		GetAppDir				(CString& strAppDir);
void		WaitTerminateHandle		(HANDLE hParam, DWORD dwTimeOut = INFINITE);
void		WaitForThreadToTerminate(HANDLE hThread, DWORD dwTimeOut = INFINITE);
BOOL		WaitForTimerIsNull		(HANDLE hTimer, DWORD dwTimeOut = INFINITE);
//=============================================================================
// 시스템 처리
//=============================================================================
//-------------------------------------------------------------------
// 한글/영문 전환 기능
//-------------------------------------------------------------------
//BOOL __fastcall GetHanMode		(void);
//void __fastcall ToggleHanMode		(void);

//=============================================================================
// 프로그램 정보 구하기
//=============================================================================
// 프로그램 버전 구하는 함수
LPCTSTR		GetVersionInfo			(HMODULE hLib, LPCTSTR lpszEntry);
CString		GetVersionInfo			(LPCTSTR lpszEntry);
LPCTSTR		GetProductVersionInfo	();
CString		FormatVersion			(CString cs);

//=============================================================================
// IP 주소 변환
//=============================================================================
BOOL		FormatIP				(LPTSTR pszIPAddr, UINT nSize, DWORD dwAddr);
CString		ConvIPAddrToString		(DWORD dwAddr);
DWORD		ConvIPStringToAddr		(LPCSTR lpszIPString);
CString		GetMyIPAddress			();

//=============================================================================
// 트레이 아이콘 잔상 제거
//=============================================================================
void		EraseTrayIcon			();
// 프로세스 강제 종료
BOOL		FocredCloseProcess		(LPCTSTR lpszProcessName, LPCTSTR lpszFuncDesc);
BOOL		NTCreateProcess			(PROCESS_INFORMATION &ProcessInformation, LPCTSTR lpszTargetName, LPSTR lpszCurrentFullPath);
void		NTExitProcess			(PROCESS_INFORMATION &ProcessInformation);
void		ForcedExitProcess		(CString strTargetProcName, DWORD dwExceptProcId);
BOOL		IsFindProcess			(CString szExt, CString szPath);

//=============================================================================
// 네트워크 관련
//=============================================================================
CString		GetPingResultCodeString	(ULONG lStatus);
DWORD		_tinet_addr				(const TCHAR *cp);

//=============================================================================
// 날짜 처리 관련
//=============================================================================
UINT getDaysInMonth (UINT nMonth, UINT nYear);

//=============================================================================
// 한글 처리 관련
//=============================================================================
char* UTF8toANSI(char *pszCode);

//=============================================================================
// 32bit/64bit 관련
//=============================================================================
//32비트 64비트 프로세스 확인
BOOL IsWow64();
//운영체제(OS)의 32비트 64비트 확인
BOOL Is64BitWindows();

CString GetDefaultPrinterName();

//=============================================================================
// 디버그 문자 찍기
//=============================================================================
void NoatepadDebug(__in LPCTSTR szDebugString);

//=============================================================================
//
//=============================================================================
BOOL	KeyBoardEvent		( const char* lpcBuffer, DWORD dwBufferSize );
BOOL	RunTouchKeyboard	();

//=============================================================================
// 최소값 
//=============================================================================
double	GetMin	(double* dbScore, UINT nSize);
WORD	GetMin	(WORD* wScore, UINT nSize);
float	GetMin	(float* fScore, UINT nSize);

double	GetMax	(double* dbScore, UINT nSize);
WORD	GetMax	(WORD* wScore, UINT nSize);
float	GetMax	(float* fScore, UINT nSize);

double	GetAvg	(double* dbScore, UINT nSize);

double	GetMid	(double* dbScore, UINT nSize, UINT nFind);


double	GetMedian(double* dbScore, UINT nSize);
double	SetRounding(__in double dbData, __in int iDigit);

void	SetSystemTimePrivilege		();
void	SetPcTime					(__in SYSTEMTIME tm);



#endif __COMMONFUNCTION_H__