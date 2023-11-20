//*****************************************************************************
// Filename	: VGStatic.cpp
// Created	: 2013/3/21
// Modified	: 2013/3/21 - 15:51
//
// Author	: PiRing
//	
// Purpose	: 
//*****************************************************************************
#include "../pch.h"
#include "VGStatic.h"

//=============================================================================
// CVGStatic
//=============================================================================
IMPLEMENT_DYNAMIC(CVGStatic, CWnd)

CVGStatic::CVGStatic()
{
	m_pCachedBitmap		= NULL;

	m_StaticStyle		= StaticStyle_Default;
	m_ShapeTheme		= SahpeTheme_Default;
	//CVGBase_Static::SetFont (L"Segoe UI", 10.0f, FontStyleRegular);
	m_StaticColors.InitColor_White();
	m_BorderTickness	= 1;

	m_hTimer_Blink	= NULL;
	//m_hTimerQueue	= NULL;
	m_dwMainDelay	= 500;
	m_dwBlinkDelay	= 100;
	m_dwBlinkCycle	= 600;
	//CreateTimerQueue_ST();
}

CVGStatic::~CVGStatic()
{
	//DeleteTimerQueue_ST();

	if (NULL != m_pCachedBitmap)
		delete m_pCachedBitmap;

	DestroyWindow();
}


BEGIN_MESSAGE_MAP(CVGStatic, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_CANCELMODE()
	ON_WM_ENABLE()
	ON_MESSAGE(WM_SETTEXT, OnSetText)
	//ON_WM_CREATE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

//=============================================================================
// Method		: CVGStatic::Create
// Access		: virtual public 
// Returns		: BOOL
// Parameter	: LPCTSTR lpszText
// Parameter	: DWORD dwStyle
// Parameter	: const RECT & rect
// Parameter	: CWnd * pParentWnd
// Parameter	: UINT nID
// Qualifier	:
// Last Update	: 2013/3/21 - 15:52
// Desc.		:
//=============================================================================
BOOL CVGStatic::Create( LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID /*= 0xffff*/ )
{
	//return CStatic::Create(lpszText, dwStyle, rect, pParentWnd, nID);
	CWnd* pWnd = this;
	m_strText = CStringW(lpszText);
	return pWnd->Create(_T("STATIC"), lpszText, dwStyle, rect, pParentWnd, nID);
}

//=============================================================================
// Method		: CVGStatic::DrawItem
// Access		: virtual public 
// Returns		: void
// Parameter	: LPDRAWITEMSTRUCT lpDrawItemStruct
// Qualifier	:
// Last Update	: 2013/3/21 - 15:52
// Desc.		:
//=============================================================================
void CVGStatic::DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct )
{
	//CStatic::DrawItem(lpDrawItemStruct);
	ASSERT(FALSE);
}

//=============================================================================
// Method		: CVGStatic::OnChildNotify
// Access		: virtual protected 
// Returns		: BOOL
// Parameter	: UINT message
// Parameter	: WPARAM wParam
// Parameter	: LPARAM lParam
// Parameter	: LRESULT * pResult
// Qualifier	:
// Last Update	: 2013/3/21 - 15:52
// Desc.		:
//=============================================================================
BOOL CVGStatic::OnChildNotify( UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	//return OnChildNotify(message, wParam, lParam, pResult);
	if (message != WM_DRAWITEM)
		return CWnd::OnChildNotify(message, wParam, lParam, pResult);

	ASSERT(pResult == NULL);       // no return value expected
	UNUSED(pResult); // unused in release builds
	DrawItem((LPDRAWITEMSTRUCT)lParam);
	return TRUE;
}

//=============================================================================
// Method		: CVGStatic::OnPaint
// Access		: public 
// Returns		: void
// Qualifier	:
// Last Update	: 2013/3/21 - 16:23
// Desc.		:
//=============================================================================
void CVGStatic::OnPaint()
{
	CPaintDC dc(this);
	Graphics graphics(dc.GetSafeHdc());	

	if((m_pCachedBitmap == NULL) || (m_bRedrawPanel))
	{
		CRect rect;
		GetClientRect(&rect);

		PaintCachedBitmap (graphics, Rect(rect.left, rect.top, rect.Width(), rect.Height()));
	}	

	graphics.DrawCachedBitmap(m_pCachedBitmap, 0, 0);
	m_bRedrawPanel = FALSE;
}

//=============================================================================
// Method		: CVGStatic::OnSize
// Access		: public 
// Returns		: void
// Parameter	: UINT nType
// Parameter	: int cx
// Parameter	: int cy
// Qualifier	:
// Last Update	: 2013/3/21 - 16:23
// Desc.		:
//=============================================================================
void CVGStatic::OnSize( UINT nType, int cx, int cy )
{
	CWnd::OnSize(nType, cx, cy);	

	m_bRedrawPanel = TRUE;
}

//=============================================================================
// Method		: CVGStatic::OnEraseBkgnd
// Access		: public 
// Returns		: BOOL
// Parameter	: CDC * pDC
// Qualifier	:
// Last Update	: 2013/3/21 - 16:28
// Desc.		:
//=============================================================================
BOOL CVGStatic::OnEraseBkgnd( CDC* pDC )
{
	return TRUE;
}

//=============================================================================
// Method		: CVGStatic::OnSetText
// Access		: public 
// Returns		: LRESULT
// Parameter	: WPARAM
// Parameter	: LPARAM lParam
// Qualifier	:
// Last Update	: 2013/3/21 - 16:28
// Desc.		:
//=============================================================================
LRESULT CVGStatic::OnSetText( WPARAM, LPARAM lParam )
{
	ASSERT(lParam == 0 || AfxIsValidString((LPCTSTR)lParam));	

	TRY
	{
		if (lParam != NULL)
		{
			m_strText = CStringW((LPCTSTR)lParam);
			ReDrawPanel ();
		}

		//Invalidate ();
	}
	CATCH_ALL(e)
	{
		// Note: DELETE_EXCEPTION(e) not required
		return FALSE;
	}
	END_CATCH_ALL

	return TRUE;
}

//=============================================================================
// Method		: OnCancelMode
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2016/3/17 - 16:28
// Desc.		:
//=============================================================================
void CVGStatic::OnCancelMode()
{
	CWnd::OnCancelMode();
}

//=============================================================================
// Method		: OnEnable
// Access		: public  
// Returns		: void
// Parameter	: BOOL bEnable
// Qualifier	:
// Last Update	: 2016/3/17 - 16:28
// Desc.		:
//=============================================================================
void CVGStatic::OnEnable(BOOL bEnable)
{
	CWnd::OnEnable(bEnable);
	
	if (bEnable)
		Invalidate();
}

//=============================================================================
// Method		: OnTimer
// Access		: protected  
// Returns		: void
// Parameter	: UINT_PTR nIDEvent
// Qualifier	:
// Last Update	: 2017/3/2 - 19:47
// Desc.		:
//=============================================================================
void CVGStatic::OnTimer(UINT_PTR nIDEvent)
{
	m_StaticColors.BackColor = Gdiplus::Color::Red;
	ReDrawPanel();

	//Sleep(m_dwBlinkDelay);
	MSG		msg;
	DWORD	endTick;

	endTick = GetTickCount() + m_dwBlinkDelay;

	while (GetTickCount() < endTick)
	{
		if (PeekMessage(&msg, NULL, 0, 0, TRUE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	m_StaticColors.BackColor = m_ResvBackColor;
	ReDrawPanel();

	__super::OnTimer(nIDEvent);
}

//=============================================================================
// Method		: CVGStatic::ReDrawPanel
// Access		: virtual protected 
// Returns		: void
// Qualifier	:
// Last Update	: 2013/3/21 - 16:28
// Desc.		:
//=============================================================================
void CVGStatic::ReDrawPanel()
{
	//CVGBase_Panel::ReDrawPanel();
	m_bRedrawPanel = TRUE;

	if (GetSafeHwnd())
		Invalidate();
}

//=============================================================================
// Method		: CVGStatic::PaintCachedBitmap
// Access		: virtual protected 
// Returns		: void
// Parameter	: Graphics & gfx
// Parameter	: Rect & argRect
// Parameter	: BOOL bSelected
// Parameter	: int nChecked
// Qualifier	:
// Last Update	: 2013/3/21 - 16:28
// Desc.		:
//=============================================================================
void CVGStatic::PaintCachedBitmap( Graphics& gfx, Rect& argRect )
{
	Bitmap	*pBit	= new Bitmap(argRect.GetRight(), argRect.GetBottom(), &gfx);
	Graphics *memG	= new Graphics(pBit);	
	
	DrawStatic (*memG, argRect);	

	if (NULL != m_pCachedBitmap) 
	{
		delete m_pCachedBitmap;
		m_pCachedBitmap = NULL;
	}

	m_pCachedBitmap = new CachedBitmap(pBit, &gfx);
	delete pBit;
	delete memG;
}

//=============================================================================
// Method		: CreateTimerQueue
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2017/1/17 - 18:50
// Desc.		:
//=============================================================================
// void CVGStatic::CreateTimerQueue_ST()
// {
// 	__try
// 	{
// 		// Create the timer queue.
// 		if (NULL == m_hTimerQueue)
// 		{
// 			m_hTimerQueue = CreateTimerQueue();
// 			if (NULL == m_hTimerQueue)
// 			{
// 				TRACE(_T("CreateTimerQueue failed (%d)\n"), GetLastError());
// 				return;
// 			}
// 		}
// 	}
// 	__except (EXCEPTION_EXECUTE_HANDLER)
// 	{
// 		TRACE(_T("*** Exception Error : CreateTimerQueue_ST ()"));
// 	}
// }

// void CVGStatic::DeleteTimerQueue_ST()
// {
// 	__try
// 	{
// 		if (NULL != m_hTimerQueue)
// 		{
// 			//DeleteTimer_Blink();
// 
// 			if (DeleteTimerQueue(m_hTimerQueue))
// 			{
// 				m_hTimerQueue = NULL;
// 
// 				TRACE(_T("타이머 종료 : CVGStatic::DeleteTimerQueue_ST()\n"));
// 			}
// 			else
// 			{
// 				TRACE(_T("DeleteTimerQueue failed (%d)\n"), GetLastError());
// 			}
// 		}
// 	}
// 	__except (EXCEPTION_EXECUTE_HANDLER)
// 	{
// 		TRACE(_T("*** Exception Error : CVGStatic::DeleteTimerQueue_ST()\n"));
// 	}
// }

//=============================================================================
// Method		: CreateTimer_Blink
// Access		: virtual protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2017/1/17 - 18:55
// Desc.		:
//=============================================================================
void CVGStatic::CreateTimer_Blink()
{
	__try
	{
		// Time Check Timer
		if (NULL != m_hTimer_Blink)
		{
			DeleteTimer_Blink();
		}
		
		m_hTimer_Blink = SetTimer(1, m_dwBlinkCycle, NULL);

// 		if (!CreateTimerQueueTimer(&m_hTimer_Blink, m_hTimerQueue, (WAITORTIMERCALLBACK)TimerRoutine_Blink, (PVOID)this, 0, m_dwBlinkCycle, WT_EXECUTEDEFAULT))
// 		{
// 			TRACE(_T("CreateTimer_Blink failed (%d)\n"), GetLastError());
// 		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		TRACE(_T("*** Exception Error : CVGStatic::CreateTimer_Blink()\n"));
	}
}

void CVGStatic::DeleteTimer_Blink()
{
	__try
	{
		if (NULL != m_hTimer_Blink)
		{
			KillTimer(m_hTimer_Blink);  
			m_hTimer_Blink = NULL;

// 			if (DeleteTimerQueueTimer(m_hTimerQueue, m_hTimer_Blink, NULL))
// 			{
// 				m_hTimer_Blink = NULL;
// 				//TRACE(_T("타이머 종료 : CVGStatic::DeleteTimer_Blink()\n"));
// 			}
// 			else
// 			{
// 				DWORD dwError = GetLastError();
// 
// 				if (ERROR_IO_PENDING == dwError)
// 				{
// 					TRACE(_T("DeleteTimer_Blink : m_hTimer_Blink failed, Overlapped I/O operation in progress\n"));
// 				}
// 				else
// 				{
// 					TRACE(_T("DeleteTimer_Blink : m_hTimer_Blink failed (%d)\n"), GetLastError());
// 				}
// 				//ASSERT(::GetLastError() == ERROR_IO_PENDING, "::DeleteTimerQueueTimer failed with error %lu", ::GetLastError());
// 			}
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		TRACE(_T("*** Exception Error : CVGStatic::DeleteTimer_Blink()\n"));
	}
}

//=============================================================================
// Method		: TimerRoutine_Blink
// Access		: protected static  
// Returns		: VOID CALLBACK
// Parameter	: __in PVOID lpParam
// Parameter	: __in BOOLEAN TimerOrWaitFired
// Qualifier	:
// Last Update	: 2017/1/17 - 18:55
// Desc.		:
//=============================================================================
// VOID CALLBACK CVGStatic::TimerRoutine_Blink(__in PVOID lpParam, __in BOOLEAN TimerOrWaitFired)
// {
// 	CVGStatic* pThis = (CVGStatic*)lpParam;
// 
// 	pThis->m_StaticColors.BackColor = Gdiplus::Color::Red;
// 	pThis->ReDrawPanel();
// 
// 	Sleep(pThis->m_dwBlinkDelay);
// 
// 	pThis->m_StaticColors.BackColor = pThis->m_ResvBackColor;
// 	pThis->ReDrawPanel();
// }

//=============================================================================
// Method		: CVGStatic::SetStaticStyle
// Access		: public 
// Returns		: void
// Parameter	: StaticStyle nStyle
// Parameter	: REAL nFontSize
// Qualifier	:
// Last Update	: 2013/3/22 - 11:41
// Desc.		:
//=============================================================================
void CVGStatic::SetStaticStyle( StaticStyle nStyle, REAL nFontSize /*= 10.0f*/ )
{
	if (m_StaticStyle != nStyle)
	{
		m_StaticStyle = nStyle;

		switch (m_StaticStyle)
		{
		case StaticStyle_Default:
			m_ShapeTheme		= SahpeTheme_Default;			
			CVGBase_Static::SetFont_Gdip (L"Segoe UI", nFontSize, FontStyleBold);
			m_RightToLeft		= RightToLeft_No;
			m_TextAlignment		= StringAlignmentCenter;
			//m_SmoothingMode		= SmoothingModeHighSpeed;
			m_BorderTickness	= 2;
			m_bShowBorder		= TRUE;
			m_StaticColors.InitColor_Default();
			break;

		case StaticStyle_GroupHeader:
			m_ShapeTheme		= ShapeTheme_Office2003;			
			CVGBase_Static::SetFont_Gdip (L"Segoe UI", nFontSize, FontStyleBold);
			//m_SmoothingMode		= SmoothingModeHighSpeed;
			m_BorderTickness	= 2;
			//m_StaticColors.InitColor_Black();
			break;

		case StaticStyle_Title:
			m_ShapeTheme		= ShapeTheme_Office2007;
			CVGBase_Static::SetFont_Gdip (L"Segoe UI", nFontSize, FontStyleBold);
			//m_SmoothingMode		= SmoothingModeHighSpeed;
			m_BorderTickness	= 2;
			m_StaticColors.InitColor_Blue2007();
			m_StaticColors.CaptionSelectedText = Color::Black;
			m_StaticColors.CaptionText = Color::Black;
			break;

		case StaticStyle_Title_Sub:
			m_ShapeTheme		= ShapeTheme_Office2003;
			CVGBase_Static::SetFont_Gdip (L"Segoe UI", nFontSize, FontStyleBold);
			//m_SmoothingMode		= SmoothingModeHighSpeed;
			m_BorderTickness	= 2;
			m_StaticColors.InitColor_Skyblue();
			m_StaticColors.CaptionSelectedText = Color::Black;
			m_StaticColors.CaptionText = Color::Black;
			break;
			
		case StaticStyle_Title_Alt:
			m_ShapeTheme		= ShapeTheme_Flat;
			CVGBase_Static::SetFont_Gdip (L"Segoe UI", nFontSize, FontStyleBold);
			//m_SmoothingMode		= SmoothingModeHighSpeed;
			m_BorderTickness	= 2;
			m_StaticColors.InitColor_Blue();
			m_StaticColors.CaptionSelectedText = Color::Black;
			m_StaticColors.CaptionText = Color::Black;
			break;

		case StaticStyle_Data:
			m_ShapeTheme		= SahpeTheme_Default;
			CVGBase_Static::SetFont_Gdip (L"Segoe UI", nFontSize, FontStyleBold);
			//m_SmoothingMode		= SmoothingModeHighSpeed;
			m_BorderTickness	= 1;
			m_StaticColors.InitColor_White();			
			break;

		}

		ReDrawPanel();
	}
}


//=============================================================================
// Method		: SetUseBlinkTimer
// Access		: public  
// Returns		: void
// Parameter	: __in BOOL bUseBlinktiemr
// Qualifier	:
// Last Update	: 2017/1/23 - 19:57
// Desc.		:
//=============================================================================
void CVGStatic::SetUseBlinkTimer(__in BOOL bUseBlinktiemr)
{
	m_bUseBlinkTimer = bUseBlinktiemr;

	if (bUseBlinktiemr)
	{
		//CreateTimerQueue_ST();
	}
}

//=============================================================================
// Method		: SetBlink
// Access		: public  
// Returns		: void
// Parameter	: __in BOOL bStart
// Parameter	: __in DWORD dwMainDelay
// Parameter	: __in DWORD dwBlinkDelay
// Qualifier	:
// Last Update	: 2017/1/17 - 18:44
// Desc.		:
//=============================================================================
void CVGStatic::SetBlink(__in BOOL bStart, __in DWORD dwMainDelay /*= 500*/, __in DWORD dwBlinkDelay /*= 100*/)
{
	if (bStart)
	{
		m_dwMainDelay	= dwMainDelay;
		m_dwBlinkDelay	= dwBlinkDelay;
		m_dwBlinkCycle = dwMainDelay + dwBlinkDelay;

		if (m_dwBlinkCycle < 200)
		{
			m_dwBlinkCycle = 200;
		}

		CreateTimer_Blink();
	}
	else
	{
		DeleteTimer_Blink();
	}
}


