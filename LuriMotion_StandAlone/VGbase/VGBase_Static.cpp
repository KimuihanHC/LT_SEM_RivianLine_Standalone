//*****************************************************************************
// Filename	: VGBase_Static.cpp
// Created	: 2013/3/21
// Modified	: 2013/3/21 - 16:51
//
// Author	: PiRing
//	
// Purpose	: 
//*****************************************************************************
#include "../pch.h"
#include "VGBase_Static.h"

//=============================================================================
//
//=============================================================================
CVGBase_Static::CVGBase_Static(void)
{	
	m_ShapeTheme		= ShapeTheme_Office2003;
	m_iCaptionHeight	= 18;
	m_iCaptionWidth		= 38;

	m_FontStyle			= FontStyleBold;
	m_FontSize			= 10.0f;
	m_pFontCaption		= NULL; // Font("Segoe UI", 8F, System.Drawing.FontStyle.Bold)
	m_strText			= L"";
	m_RightToLeft		= RightToLeft_No;
	m_TextAlignment		= StringAlignmentCenter;

	m_pImage			= NULL;	
	m_SmoothingMode		= SmoothingModeHighSpeed;
	m_TextRenderingHint	= TextRenderingHintClearTypeGridFit;
	m_BorderTickness	= 2;
	m_bShowBorder		= TRUE;
	//m_StaticColors.InitColor_Black();

	m_bRedrawPanel		= TRUE;

	m_pFontCaption		= new Gdiplus::Font(L"Segoe UI", m_FontSize, m_FontStyle);

	// 2nd 라인 출력용 (2016.11.21)
	m_pFontCaption_2nd	= new Gdiplus::Font(L"Segoe UI", m_FontSize, m_FontStyle);
	m_FontStyle_2nd		= FontStyleBold;;
	m_FontSize_2nd		= 10.0f;;
}

CVGBase_Static::~CVGBase_Static(void)
{
	m_strText.ReleaseBuffer();

	if (NULL != m_pFontCaption)
		delete m_pFontCaption;

	// 2nd 라인 출력용 (2016.11.21)
	m_strText_2nd.ReleaseBuffer();

	if (NULL != m_pFontCaption_2nd)
		delete m_pFontCaption_2nd;
}

//=============================================================================
// Method		: CVGBase_Static::DrawCaptionbar
// Access		: private 
// Returns		: void
// Parameter	: Graphics & graphics
// Parameter	: Rect rectClient
// Qualifier	:
// Last Update	: 2013/3/5 - 14:45
// Desc.		:
//=============================================================================
void CVGBase_Static::DrawCaptionbar( Graphics& graphics, Rect rectClient )
{	
	Gdiplus::Color colorGradientBegin	= m_StaticColors.CaptionGradientBegin;
	Gdiplus::Color colorGradientEnd		= m_StaticColors.CaptionGradientEnd;
	Gdiplus::Color colorGradientMiddle	= m_StaticColors.CaptionGradientMiddle;
	Gdiplus::Color colorText			= m_StaticColors.CaptionSelectedText;
	Gdiplus::Rect  rectCaption			= rectClient;

	switch (m_ShapeTheme)
	{
	case ShapeTheme_Office2003:
		{
			if (rectCaption.Height < rectCaption.Width)
				RenderDoubleBackgroundGradient(graphics, rectCaption, colorGradientBegin, colorGradientMiddle, colorGradientEnd, LinearGradientModeVertical, false);
			else
				RenderDoubleBackgroundGradient(graphics, rectCaption, colorGradientBegin, colorGradientMiddle, colorGradientEnd, LinearGradientModeHorizontal, false);
		}
		break;

	case ShapeTheme_Office2007:
		{
			RenderButtonBackground(graphics, rectCaption, colorGradientBegin, colorGradientMiddle, colorGradientEnd);
		}
		break;

	case ShapeTheme_Flat:
		{
			//RenderFlatButtonBackground(graphics, rectCaption, m_StaticColors.CaptionGradientBegin, m_StaticColors.CaptionGradientEnd, FALSE);
			RenderFlatButtonBackground(graphics, rectCaption, m_StaticColors.CaptionGradientEnd, m_StaticColors.CaptionGradientEnd, FALSE);
			DrawInnerBorders(graphics, rectClient, 0);
		}
		break;

	case SahpeTheme_Default:
	default:
		{
// 			SolidBrush solidBrush(colorGradientBegin);
// 			graphics.FillRectangle(&solidBrush, rectCaption);
		}
		break;
	}

	//rectCaption	= Rect (m_rectCaption.X, m_rectCaption.Y, m_rectCaption.Width, m_rectCaption.Height - (INT)(m_FontSize / 5.0f));
	if (m_strText_2nd.IsEmpty())
	{
		DrawImagesAndText(graphics, rectCaption, VG_SPACING, m_rectImage, m_pImage, m_RightToLeft, m_pFontCaption, colorText, m_strText.GetBuffer(), m_TextAlignment);
		m_strText.ReleaseBuffer();
	}
	else
	{
		m_strText += _T("\r\n  ");
		DrawImagesAndText(graphics, rectCaption, VG_SPACING, m_rectImage, m_pImage, m_RightToLeft, m_pFontCaption, colorText, m_strText.GetBuffer(), m_TextAlignment);
		
		rectCaption.Height = rectCaption.Height / 2;
		rectCaption.Y = rectCaption.Y + rectCaption.Height;
		DrawText(graphics, rectCaption, VG_SPACING, m_RightToLeft, m_pFontCaption_2nd, colorText, m_strText_2nd.GetBuffer(), m_TextAlignment);
		m_strText.ReleaseBuffer();
		m_strText_2nd.ReleaseBuffer();
	}
}

//=============================================================================
// Method		: CVGBase_Static::DrawBorders
// Access		: private 
// Returns		: void
// Parameter	: Graphics & graphics
// Parameter	: Rect rectClient
// Qualifier	:
// Last Update	: 2013/3/5 - 15:15
// Desc.		:
//=============================================================================
void CVGBase_Static::DrawBorders( Graphics& graphics, Rect rectClient)
{
	if (false == m_bShowBorder)
		return;

	GraphicsPath	graphicsPath;
	Gdiplus::Color	BorderColor = m_StaticColors.BorderColor;
	Pen				borderPen(BorderColor, (REAL)m_BorderTickness);

	INT iAdjust = m_BorderTickness / 2;	

	borderPen.SetColor(BorderColor);
	borderPen.SetWidth((REAL)m_BorderTickness);

	INT iAdjust2 = m_BorderTickness;	
	if (SmoothingModeAntiAlias <= graphics.GetSmoothingMode())
		iAdjust2 = ((INT)m_BorderTickness % 2) ? m_BorderTickness : m_BorderTickness + 1;

	graphics.DrawRectangle(&borderPen, RectF((REAL)rectClient.X + iAdjust, (REAL)rectClient.Y + iAdjust, (REAL)(rectClient.Width - iAdjust2), (REAL)(rectClient.Height - iAdjust2)));
}

//=============================================================================
// Method		: CVGBase_Static::DrawInnerBorders
// Access		: protected 
// Returns		: void
// Parameter	: Graphics & graphics
// Parameter	: Rect rectClient
// Parameter	: int PaddingAll
// Qualifier	:
// Last Update	: 2013/3/21 - 17:26
// Desc.		:
//=============================================================================
void CVGBase_Static::DrawInnerBorders( Graphics& graphics, Rect rectClient, int PaddingAll /*= 0*/ )
{
	if (false == m_bShowBorder)
		return;

	GraphicsPath graphicsPath;
	Gdiplus::Rect captionRectangle (m_rectCaption);				
	
	if (PaddingAll == 1)
	{
		//Left vertical borderline
		graphicsPath.AddLine(captionRectangle.X, captionRectangle.Y + captionRectangle.Height, captionRectangle.X, captionRectangle.Y + m_BorderTickness);
		if (rectClient.GetTop() == 0)
		{
			//Upper horizontal borderline
			graphicsPath.AddLine(captionRectangle.X, captionRectangle.Y, captionRectangle.X + captionRectangle.Width, captionRectangle.Y);
		}
		else
		{
			//Upper horizontal borderline
			graphicsPath.AddLine(captionRectangle.X, captionRectangle.Y + m_BorderTickness, captionRectangle.X + captionRectangle.Width, captionRectangle.Y + m_BorderTickness);
		}
	}

	Pen borderPen (m_StaticColors.InnerBorderColor);
	{
		graphics.DrawPath(&borderPen, &graphicsPath);
	}
}

//=============================================================================
// Method		: CVGBase_Static::CalculateStaticHeights
// Access		: protected 
// Returns		: void
// Parameter	: Gdiplus::Rect rectClient
// Qualifier	:
// Last Update	: 2013/3/21 - 17:25
// Desc.		:
//=============================================================================
void CVGBase_Static::CalculateStaticHeights( Gdiplus::Rect rectClient )
{	
	int iCaptionWidth = ((rectClient.Width / 2) < m_iCaptionWidth) ? (rectClient.Width / 2) : m_iCaptionWidth;

	//m_rectCaption = Rect (0, 0, rectClient.Width, m_iCaptionHeight);
	m_rectCaption = rectClient;
}

//=============================================================================
// Method		: CVGBase_Static::DrawBackground
// Access		: private 
// Returns		: void
// Parameter	: Graphics & graphics
// Parameter	: Rect rectClient
// Qualifier	:
// Last Update	: 2013/3/5 - 15:15
// Desc.		:
//=============================================================================
void CVGBase_Static::DrawBackground( Graphics& graphics, Rect rectClient )
{
	SolidBrush backgroundBrush(m_StaticColors.BackColor);

	graphics.FillRectangle(&backgroundBrush, rectClient);	
}

//=============================================================================
// Method		: CVGBase_Static::DrawForeground
// Access		: protected 
// Returns		: void
// Parameter	: Graphics & graphics
// Parameter	: Rect rectClient
// Qualifier	:
// Last Update	: 2013/3/21 - 17:25
// Desc.		:
//=============================================================================
void CVGBase_Static::DrawForeground( Graphics& graphics, Rect rectClient )
{
	if (IsZeroWidthOrHeight(m_rectCaption) == true)
	{
		return;
	}

	graphics.SetSmoothingMode(m_SmoothingMode);
	//graphics.SetTextRenderingHint(TextRenderingHintClearTypeGridFit);
	graphics.SetTextRenderingHint(m_TextRenderingHint);	

	DrawCaptionbar(graphics, rectClient);	
	DrawBorders(graphics, rectClient);
}

//=============================================================================
// Method		: CVGBase_Static::DrawStatic
// Access		: virtual protected 
// Returns		: void
// Parameter	: Graphics & graphics
// Parameter	: Rect rectClient
// Qualifier	:
// Last Update	: 2013/3/21 - 17:25
// Desc.		:
//=============================================================================
void CVGBase_Static::DrawStatic( Graphics& graphics, Rect rectClient )
{
	CalculateStaticHeights (rectClient);
	DrawBackground (graphics, rectClient);
	DrawForeground (graphics, rectClient);
}

//=============================================================================
// Method		: CVGBase_Static::ReDrawPanel
// Access		: virtual protected 
// Returns		: void
// Qualifier	:
// Last Update	: 2013/3/7 - 14:38
// Desc.		:
//=============================================================================
void CVGBase_Static::ReDrawPanel()
{
	m_bRedrawPanel = TRUE;
}

//-------------------------------------------------------------------------
// 멤버 변수 설정 함수
//-------------------------------------------------------------------------
//=============================================================================
// Method		: CVGBase_Static::ChangeColorStyle
// Access		: public 
// Returns		: void
// Parameter	: ColorStyle nColorStyle
// Qualifier	:
// Last Update	: 2013/3/12 - 15:45
// Desc.		:
//=============================================================================
void CVGBase_Static::SetColorStyle( ColorStyle nColorStyle )
{
	switch (nColorStyle)
	{
	case ColorStyle_Default:
	case ColorStyle_Sky:
		m_StaticColors.InitColor_Sky();
		break;
	case ColorStyle_SkyBlue:
		m_StaticColors.InitColor_Skyblue();
		break;
	case ColorStyle_Blue:
		m_StaticColors.InitColor_Blue();
		break;
	case ColorStyle_Blue2007:
		m_StaticColors.InitColor_Blue2007();
		break;
	case ColorStyle_SlateGray:
		m_StaticColors.InitColor_SlateGray();
		break;
	case ColorStyle_DarkGray:
		m_StaticColors.InitColor_DarkGray();
		break;
	case ColorStyle_DeepDarkGray:
		m_StaticColors.InitColor_DeepDarkGray();
		break;
	case ColorStyle_Black:
		m_StaticColors.InitColor_Black();
		break;
	case ColorStyle_Black2007:
		m_StaticColors.InitColor_Black2007();
		break;
	case ColorStyle_Red:
		m_StaticColors.InitColor_Red();
		break;
	case ColorStyle_DarkBrown:
		m_StaticColors.InitColor_DarkBrown();
		break;
	case ColorStyle_Orange:
		m_StaticColors.InitColor_Orange();
		break;
	case ColorStyle_Yellow:
		m_StaticColors.InitColor_Yellow();
		break;
	case ColorStyle_LightGreen:
		m_StaticColors.InitColor_LightGreen();
		break;
	case ColorStyle_Green:
		m_StaticColors.InitColor_Green();
		break;
	case ColorStyle_Turquois:
		m_StaticColors.InitColor_Turquoise();
		break;
	case ColorStyle_White:
		m_StaticColors.InitColor_White();
		break;
	case ColorStyle_Navy:
		m_StaticColors.InitColor_Navy();
		break;
	case ColorStyle_Violet:
		m_StaticColors.InitColor_Violet();
		break;

		// 노랑, 보라, 남색, 하양, 회색 추가해야 함.
	}

	ReDrawPanel();
}

//=============================================================================
// Method		: CVGBase_Static::SetFont
// Access		: public 
// Returns		: void
// Parameter	: IN const WCHAR * familyName
// Parameter	: REAL emSize
// Parameter	: FontStyle fontStyle
// Qualifier	:
// Last Update	: 2013/3/12 - 15:45
// Desc.		:
//=============================================================================
void CVGBase_Static::SetFont_Gdip( IN const WCHAR * familyName, REAL emSize /*= 8.0*/, FontStyle fontStyle /*= FontStyleBold*/ )
{
	if (NULL != m_pFontCaption)
	{
		delete m_pFontCaption;
	}

	m_FontSize		= emSize;
	m_FontStyle		= fontStyle;
	m_pFontCaption	= new Gdiplus::Font(familyName, emSize, fontStyle);

	ReDrawPanel();
}

//=============================================================================
// Method		: CVGBase_Static::GetCaptionText
// Access		: public 
// Returns		: void
// Parameter	: OUT CStringW & szValue
// Qualifier	:
// Last Update	: 2013/3/12 - 15:45
// Desc.		:
//=============================================================================
void CVGBase_Static::GetText( OUT CStringW& szValue )
{
	szValue = m_strText;

	ReDrawPanel();
}

//=============================================================================
// Method		: CVGBase_Static::SetCaptionText
// Access		: public 
// Returns		: void
// Parameter	: IN const WCHAR * szValue
// Qualifier	:
// Last Update	: 2013/3/12 - 15:45
// Desc.		:
//=============================================================================
void CVGBase_Static::SetText( IN const WCHAR* szValue )
{
	m_strText.Empty();
	m_strText = szValue;

	ReDrawPanel();
}

void CVGBase_Static::SetText( IN const CHAR* szValue )
{
	m_strText.Empty();
	m_strText_2nd.Empty();
	
	USES_CONVERSION;
	m_strText = CA2W(szValue);

	ReDrawPanel();
}

void CVGBase_Static::SetText(IN const WCHAR* szValue, IN const WCHAR* szValue_2nd)
{
	m_strText.Empty();
	m_strText_2nd.Empty();

	m_strText = szValue;
	m_strText_2nd = szValue_2nd;

	ReDrawPanel();
}

void CVGBase_Static::SetText(IN const CHAR* szValue, IN const CHAR* szValue_2nd)
{
	m_strText.Empty();
	m_strText.Empty();

	USES_CONVERSION;
	m_strText = CA2W(szValue);
	m_strText_2nd = CA2W(szValue_2nd);

	ReDrawPanel();
}

//=============================================================================
// Method		: CVGBase_Static::SetCaptionTextAlignment
// Access		: public 
// Returns		: void
// Parameter	: StringAlignment CationTextAlignment
// Qualifier	:
// Last Update	: 2013/3/12 - 15:45
// Desc.		:
//=============================================================================
void CVGBase_Static::SetTextAlignment( StringAlignment CationTextAlignment )
{
	m_TextAlignment = CationTextAlignment;

	ReDrawPanel();
}

//=============================================================================
// Method		: CVGBase_Static::SetCaptionTextColor
// Access		: public 
// Returns		: void
// Parameter	: Gdiplus::Color TextColor
// Parameter	: Gdiplus::Color SelectedTextColor
// Parameter	: Gdiplus::Color CollapsedTextColor
// Qualifier	:
// Last Update	: 2013/3/12 - 15:45
// Desc.		:
//=============================================================================
void CVGBase_Static::SetTextColor( Gdiplus::Color TextColor, Gdiplus::Color SelectedTextColor /*= Gdiplus::Color::Black*/, Gdiplus::Color CollapsedTextColor /*= Gdiplus::Color::Black*/ )
{
	m_StaticColors.CaptionText			= TextColor;
	m_StaticColors.CaptionSelectedText	= SelectedTextColor;
	m_StaticColors.CollapsedCaptionText	= CollapsedTextColor;

	ReDrawPanel();
}

void CVGBase_Static::SetTextColor_COLORREF(COLORREF TextColor, COLORREF SelectedTextColor /*= RGB(0,0,0)*/, COLORREF CollapsedTextColor /*= RGB(0,0,0)*/)
{
	Gdiplus::Color clrTemp;

	clrTemp.SetFromCOLORREF(TextColor);
	m_StaticColors.CaptionText = clrTemp;

	clrTemp.SetFromCOLORREF(SelectedTextColor);
	m_StaticColors.CaptionSelectedText = clrTemp;

	clrTemp.SetFromCOLORREF(CollapsedTextColor);
	m_StaticColors.CollapsedCaptionText = clrTemp;

	ReDrawPanel();
}

//=============================================================================
// Method		: CVGBase_Static::SetCaptionHeight
// Access		: public 
// Returns		: void
// Parameter	: int iHeight
// Qualifier	:
// Last Update	: 2013/3/12 - 15:45
// Desc.		:
//=============================================================================
void CVGBase_Static::SetHeight( int iHeight )
{
	m_iCaptionHeight = iHeight;

	ReDrawPanel();
}

//=============================================================================
// Method		: CVGBase_Static::SetStaticStyle
// Access		: public 
// Returns		: void
// Parameter	: StaticStyle nStyle
// Qualifier	:
// Last Update	: 2013/3/21 - 17:58
// Desc.		:
//=============================================================================
void CVGBase_Static::SetShapeTheme( ShapeTheme nStyle )
{
	if (m_ShapeTheme != nStyle)
	{
		m_ShapeTheme = nStyle;

		ReDrawPanel();
	}
}

//=============================================================================
// Method		: SetSmoothMode
// Access		: public  
// Returns		: void
// Parameter	: SmoothingMode nSmoothingMode
// Qualifier	:
// Last Update	: 2017/9/19 - 12:55
// Desc.		:
//=============================================================================
void CVGBase_Static::SetSmoothMode(SmoothingMode nSmoothingMode)
{
	if (m_SmoothingMode != nSmoothingMode)
	{
		m_SmoothingMode = nSmoothingMode;

		ReDrawPanel();
	}
}

//=============================================================================
// Method		: SetTextRenderingHint
// Access		: public  
// Returns		: void
// Parameter	: TextRenderingHint nTextRenderingHint
// Qualifier	:
// Last Update	: 2017/9/19 - 13:10
// Desc.		:
//=============================================================================
void CVGBase_Static::SetTextRenderingHint(TextRenderingHint nTextRenderingHint)
{
	if (m_TextRenderingHint != nTextRenderingHint)
	{
		m_TextRenderingHint = nTextRenderingHint;

		ReDrawPanel();
	}
}

//=============================================================================
// Method		: CVGBase_Static::SetBackColor
// Access		: public 
// Returns		: void
// Parameter	: Gdiplus::Color clrBack
// Qualifier	:
// Last Update	: 2013/3/22 - 13:43
// Desc.		:
//=============================================================================
void CVGBase_Static::SetBackColor( Gdiplus::Color clrBack )
{
	m_StaticColors.BackColor = clrBack;
	m_ResvBackColor = clrBack;

	ReDrawPanel();
}

void CVGBase_Static::SetBackColor_COLORREF( COLORREF clrBack )
{
	Gdiplus::Color clrTemp;
	clrTemp.SetFromCOLORREF(clrBack);

	SetBackColor (clrTemp);
}

Gdiplus::ARGB CVGBase_Static::GetBackColor()
{
	return m_StaticColors.BackColor.GetValue();
}

//=============================================================================
// Method		: SetBorderColor
// Access		: public  
// Returns		: void
// Parameter	: Gdiplus::Color clrBack
// Qualifier	:
// Last Update	: 2016/4/24 - 20:03
// Desc.		:
//=============================================================================
void CVGBase_Static::SetBorderColor(Gdiplus::Color clrBack)
{
	m_StaticColors.BorderColor = clrBack;

	ReDrawPanel();
}

void CVGBase_Static::SetBorderColor_COLORREF(COLORREF clrBack)
{
	Gdiplus::Color clrTemp;
	clrTemp.SetFromCOLORREF(clrBack);

	SetBorderColor (clrTemp);
}

void CVGBase_Static::SetUseBorder(BOOL bUse)
{
	m_bShowBorder = bUse;

	ReDrawPanel();
}

void CVGBase_Static::SetBorderTickness(INT iBorderTickness)
{
	m_BorderTickness = iBorderTickness;
}

//=============================================================================
// Method		: SetFont_Gdip_2nd
// Access		: public  
// Returns		: void
// Parameter	: IN const WCHAR * familyName
// Parameter	: REAL emSize
// Parameter	: FontStyle fontStyle
// Qualifier	:
// Last Update	: 2016/11/21 - 19:11
// Desc.		:
//=============================================================================
void CVGBase_Static::SetFont_Gdip_2nd(IN const WCHAR * familyName, REAL emSize /*= 8.0*/, FontStyle fontStyle /*= FontStyleBold*/)
{
	if (NULL != m_pFontCaption_2nd)
	{
		delete m_pFontCaption_2nd;
	}

	m_FontSize_2nd = emSize;
	m_FontStyle_2nd = fontStyle;
	m_pFontCaption_2nd = new Gdiplus::Font(familyName, emSize, fontStyle);

	ReDrawPanel();
}
