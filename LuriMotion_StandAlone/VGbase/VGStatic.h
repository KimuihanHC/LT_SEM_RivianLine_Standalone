//*****************************************************************************
// Filename	: VGStatic.h
// Created	: 2013/3/21
// Modified	: 2013/3/21 - 15:45
//
// Author	: PiRing
//	
// Purpose	: 
//*****************************************************************************
#ifndef VGStatic_h__
#define VGStatic_h__

#pragma once

#include "VGBase_Static.h"

//=============================================================================
// CVGStatic
//=============================================================================
class CVGStatic : public CWnd, public CVGBase_Static
{
	DECLARE_DYNAMIC(CVGStatic)

public:
	CVGStatic();
	virtual ~CVGStatic();
	
	enum StaticStyle
	{
		StaticStyle_Default,
		StaticStyle_GroupHeader,
		StaticStyle_Title,
		StaticStyle_Title_Sub,
		StaticStyle_Title_Alt,
		StaticStyle_Data,
	};

protected:
	afx_msg void	OnPaint					();
	afx_msg void	OnSize					(UINT nType, int cx, int cy);
	afx_msg BOOL	OnEraseBkgnd			(CDC* pDC);
	afx_msg LRESULT OnSetText				(WPARAM, LPARAM lParam);
	afx_msg void	OnCancelMode			();
	afx_msg void	OnEnable				(BOOL bEnable);
	afx_msg void	OnTimer					(UINT_PTR nIDEvent);

public:
	virtual BOOL	Create					(LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID = 0xffff);

protected:
	virtual void	DrawItem				(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual BOOL	OnChildNotify			(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()

	CachedBitmap*	m_pCachedBitmap;
	StaticStyle		m_StaticStyle;

	virtual void	ReDrawPanel				();	
	virtual void	PaintCachedBitmap(Graphics& gfx, Gdiplus::Rect& argRect);

	// 깜박임 기능 추가용도
	BOOL			m_bUseBlinkTimer;
	//HANDLE			m_hTimer_Blink;
	UINT_PTR		m_hTimer_Blink;
	//HANDLE			m_hTimerQueue;
	DWORD			m_dwMainDelay;
	DWORD			m_dwBlinkDelay;
	DWORD			m_dwBlinkCycle;
// 	void			CreateTimerQueue_ST		();
// 	void			DeleteTimerQueue_ST		();	
	virtual void	CreateTimer_Blink		();
	virtual void	DeleteTimer_Blink		();
//	static VOID CALLBACK TimerRoutine_Blink	(__in PVOID lpParam, __in BOOLEAN TimerOrWaitFired);

public:

	void			SetStaticStyle			(StaticStyle nStyle, REAL nFontSize = 10.0f);

	void			SetUseBlinkTimer		(__in BOOL bUseBlinktiemr);
	void			SetBlink				(__in BOOL bStart, __in DWORD dwMainDelay = 500, __in DWORD dwBlinkDelay = 100);
	
};

#endif // VGStatic_h__

