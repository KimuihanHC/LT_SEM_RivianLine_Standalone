//*****************************************************************************
// Filename	: Define_MonitorInfoFunc.h
// Created	: 2012/10/18
// Modified	: 2012/10/18 - 13:46
//
// Author	: PiRing
//	
// Purpose	: 모니터 정보 처리용 함수
//*****************************************************************************
#ifndef Define_MonitorInfoFunc_h__
#define Define_MonitorInfoFunc_h__

#include "afxwin.h"

typedef struct _tagMonitorInfo
{
	int nWidth;					// Width
	int nHeight;				// Height
	int nWidthVirtual;			// Width Virtual
	int nHeightVirtual;			// Height Virtual
	int nBitPerPixel;			// BitPerPixel
	int nRefresh;				// Refresh
	int nMonitor;				// Monitors
	int nMegaPixel;				// MegaPixel
	BOOL bSameDisplayFormat;	// SameDisplayFormat
	RECT rcVirtual;
	CArray< MONITORINFOEX, MONITORINFOEX > aryMonitors;

	_tagMonitorInfo ()
	{
		Reset ();
	};

	void Reset ()
	{
		nWidth				= 0;
		nHeight				= 0;
		nWidthVirtual		= 0;
		nHeightVirtual		= 0;
		nBitPerPixel		= 0;
		nRefresh			= 0;
		nMonitor			= 0;
		nMegaPixel			= 0;
		bSameDisplayFormat	= FALSE;
		rcVirtual.bottom	= 0;
		rcVirtual.top		= 0;
		rcVirtual.left		= 0;
		rcVirtual.right		= 0;

		aryMonitors.RemoveAll();
	};
}ST_MONITORINFO;

//-----------------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------------
static BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData);
static BOOL GetMonitorInformation(__out ST_MONITORINFO& stMonInfo);

//=============================================================================
// Method		: MonitorEnumProc
// Access		: public 
// Returns		: BOOL CALLBACK
// Parameter	: HMONITOR hMonitor
// Parameter	: HDC hdcMonitor
// Parameter	: LPRECT lprcMonitor
// Parameter	: LPARAM dwData
// Qualifier	:
// Last Update	: 2012/10/18 - 13:50
// Desc.		:
//=============================================================================
BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{
	MONITORINFOEX stMonInfoEx;

	stMonInfoEx.cbSize = sizeof(MONITORINFOEX);
	GetMonitorInfo (hMonitor, &stMonInfoEx);

	((ST_MONITORINFO*)dwData)->aryMonitors.Add(stMonInfoEx);

	return TRUE;
}

//=============================================================================
// Method		: GetMonitorInformation
// Access		: public 
// Returns		: BOOL
// Parameter	: __out ST_MONITORINFO & stMonInfo
// Qualifier	:
// Last Update	: 2012/10/18 - 13:50
// Desc.		:
//BOOL EnumDisplayMonitors		(__in HDC hdc, __in LPCRECT lprcClip, __in  MONITORENUMPROC lpfnEnum, __in  LPARAM dwData);
//BOOL CALLBACK MonitorEnumProc	(__in HMONITOR hMonitor, __in  HDC hdcMonitor, __in  LPRECT lprcMonitor, __in  LPARAM dwData);
//BOOL GetMonitorInfo			(__in HMONITOR hMonitor, __out LPMONITORINFO lpmi);
//=============================================================================
BOOL GetMonitorInformation(__out ST_MONITORINFO& stMonInfo)
{
	// 모니터 정보 초기화
	stMonInfo.Reset();

	stMonInfo.nMonitor			= ::GetSystemMetrics(SM_CMONITORS); 
	stMonInfo.bSameDisplayFormat= ::GetSystemMetrics(SM_SAMEDISPLAYFORMAT);

	stMonInfo.rcVirtual.left	= GetSystemMetrics(SM_XVIRTUALSCREEN);
	stMonInfo.rcVirtual.top		= GetSystemMetrics(SM_YVIRTUALSCREEN);
	stMonInfo.rcVirtual.right	= stMonInfo.rcVirtual.left + GetSystemMetrics(SM_CXVIRTUALSCREEN);
	stMonInfo.rcVirtual.bottom	= stMonInfo.rcVirtual.top + GetSystemMetrics(SM_CYVIRTUALSCREEN);

	stMonInfo.nWidth			= ::GetSystemMetrics(SM_CXSCREEN);			// 모니터의 해상도 x
	stMonInfo.nHeight			= ::GetSystemMetrics(SM_CYSCREEN);			// 모니터의 해상도 y
	stMonInfo.nWidthVirtual		= ::GetSystemMetrics(SM_CXVIRTUALSCREEN);	// 가상모니터의 해상도 x
	stMonInfo.nHeightVirtual	= ::GetSystemMetrics(SM_CYVIRTUALSCREEN);	// 가상모니터의 해상도 y

	stMonInfo.nMegaPixel = (stMonInfo.nWidth * stMonInfo.nHeight) / (1000 * 1000);
	stMonInfo.nMegaPixel = max(1, stMonInfo.nMegaPixel);


	HDC hDC=NULL;
	hDC = CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
	if(hDC)
	{
		EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, (LPARAM)&stMonInfo);
		stMonInfo.nBitPerPixel	= GetDeviceCaps(hDC, BITSPIXEL);
		stMonInfo.nRefresh		= GetDeviceCaps(hDC, VREFRESH);
		DeleteDC(hDC);
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

#endif // Define_MonitorInfoFunc_h__
