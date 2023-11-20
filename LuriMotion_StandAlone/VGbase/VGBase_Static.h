//*****************************************************************************
// Filename	: VGBase_Static.h
// Created	: 2013/3/21
// Modified	: 2013/3/21 - 16:46
//
// Author	: PiRing
//	
// Purpose	: 
//*****************************************************************************
#ifndef VGBase_Static_h__
#define VGBase_Static_h__

#pragma once

#include "Define_ColorTable.h"
#include "VGBase.h"

//=============================================================================
//
//=============================================================================
class CVGBase_Static : public CVGBase
{
public:
	CVGBase_Static(void);
	virtual ~CVGBase_Static(void);

	enum ShapeTheme
	{
		SahpeTheme_Default,
		ShapeTheme_Office2003,
		ShapeTheme_Office2007,
		ShapeTheme_Flat,
	};

	enum ColorStyle
	{
		ColorStyle_Default	= 0,
		ColorStyle_Sky,
		ColorStyle_SkyBlue,
		ColorStyle_Blue,
		ColorStyle_Blue2007,
		ColorStyle_Black,
		ColorStyle_Black2007,
		ColorStyle_DarkBrown,
		ColorStyle_Orange,
		ColorStyle_LightGreen,
		ColorStyle_Turquois,
		ColorStyle_White,

		ColorStyle_Red,
		ColorStyle_Yellow,
		ColorStyle_Green,
		ColorStyle_SlateGray,
		ColorStyle_DarkGray,
		ColorStyle_DeepDarkGray,
		ColorStyle_Navy,
		ColorStyle_Violet,
	};

protected:

	ShapeTheme			m_ShapeTheme;
	int					m_iCaptionHeight;
	int					m_iCaptionWidth;

	Gdiplus::Font*		m_pFontCaption;
	CStringW			m_strText;
	RightToLeft			m_RightToLeft;

	FontStyle			m_FontStyle;
	REAL				m_FontSize;
	StringAlignment		m_TextAlignment;

	Image*				m_pImage;		// Caption에 표시될 아이콘 이미지
	Gdiplus::Rect		m_rectImage;	// Rectangle(CaptionSpacing, CaptionHeight, m_imageSize.Width, m_imageSize.Height);

	SmoothingMode		m_SmoothingMode;
	TextRenderingHint	m_TextRenderingHint;
	INT					m_BorderTickness;
	BOOL				m_bShowBorder;	// 외곽선 그리기 여부
	PanelColors			m_StaticColors;

	Gdiplus::Rect				m_rectCaption;	// Rect (0, 0, this.rectClient.Width, m_iCaptionHeight);	

	// 2nd 라인 출력용 (2016.11.21)
	CStringW			m_strText_2nd;
	Gdiplus::Font*		m_pFontCaption_2nd;
	FontStyle			m_FontStyle_2nd;
	REAL				m_FontSize_2nd;

protected:

	BOOL				m_bRedrawPanel;

	//-------------------------------------------------------------------------
	// Panel 그리기 함수
	//-------------------------------------------------------------------------
	void	DrawCaptionbar(Graphics& graphics, Gdiplus::Rect rectClient);
	void	DrawBorders(Graphics& graphics, Gdiplus::Rect rectClient);
	void	DrawInnerBorders(Graphics& graphics, Gdiplus::Rect rectClient, int PaddingAll = 0);

	void	CalculateStaticHeights		(Gdiplus::Rect rectClient);
	void	DrawBackground(Graphics& graphics, Gdiplus::Rect rectClient);
	void	DrawForeground(Graphics& graphics, Gdiplus::Rect rectClient);

	virtual void DrawStatic(Graphics& graphics, Gdiplus::Rect rectClient);

	virtual void ReDrawPanel			();

	Color	m_ResvBackColor;

public:

	//-------------------------------------------------------------------------
	// 멤버 변수 설정 함수
	//-------------------------------------------------------------------------
	void	SetColorStyle				(ColorStyle nColorStyle);
	void	SetShapeTheme				(ShapeTheme nStyle);
	void	SetSmoothMode				(SmoothingMode	nSmoothingMode);
	void	SetTextRenderingHint		(TextRenderingHint	nTextRenderingHint);

	// Caption Text
	void	SetFont_Gdip				(IN const WCHAR * familyName, REAL emSize = 8.0, FontStyle fontStyle = FontStyleBold);
	void	GetText						(OUT CStringW& szValue);
	void	SetText						(IN const WCHAR* szValue);
	void	SetText						(IN const CHAR* szValue);
	void	SetTextAlignment			(StringAlignment CationTextAlignment);
	void	SetTextColor				(Gdiplus::Color TextColor, Gdiplus::Color SelectedTextColor = Gdiplus::Color::Black, Gdiplus::Color CollapsedTextColor = Gdiplus::Color::Black);
	void	SetTextColor_COLORREF		(COLORREF TextColor, COLORREF SelectedTextColor = RGB(0,0,0), COLORREF CollapsedTextColor = RGB(0,0,0));
	void	SetHeight					(int iHeight);	

	void	SetBackColor				(Gdiplus::Color clrBack);
	void	SetBackColor_COLORREF		(COLORREF clrBack);
	Gdiplus::ARGB	GetBackColor		();
	void	SetBorderColor				(Gdiplus::Color clrBack);
	void	SetBorderColor_COLORREF		(COLORREF clrBack);
	void	SetUseBorder				(BOOL bUse);
	void	SetBorderTickness			(INT iBorderTickness);

	// 2nd 라인 출력용 (2016.11.21)
	void	SetFont_Gdip_2nd			(IN const WCHAR * familyName, REAL emSize = 8.0, FontStyle fontStyle = FontStyleBold);
	void	SetText						(IN const WCHAR* szValue, IN const WCHAR* szValue_2nd);
	void	SetText						(IN const CHAR* szValue, IN const CHAR* szValue_2nd);
};

#endif // VGBase_Static_h__
