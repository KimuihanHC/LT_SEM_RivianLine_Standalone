//*****************************************************************************
// Filename	: VGBase.h
// Created	: 2013/3/13
// Modified	: 2013/3/13 - 16:27
//
// Author	: PiRing
//	
// Purpose	: 
//*****************************************************************************
#ifndef VGBase_h__
#define VGBase_h__

#pragma once

#define  VG_SPACING		2 //6

//=============================================================================
// CVGBase
//=============================================================================
class CVGBase
{
public:
	CVGBase(void);
	~CVGBase(void);

	enum RightToLeft
	{
		// 요약: The text reads from left to right. This is the default.
		RightToLeft_No		= 0,
		// 요약: The text reads from right to left.
		RightToLeft_Yes		= 1,
		// 요약: The direction the text read is inherited from the parent control.
		RightToLeft_Inherit = 2,
	};

	// 요약: Specifies the position and manner in which a control is docked.
	enum DockStyle
	{
		// 요약: The control is not docked.
		DockStyle_None		= 0,
		// 요약: The control's top edge is docked to the top of its containing control.
		DockStyle_Top		= 1,
		// 요약: The control's bottom edge is docked to the bottom of its containing control.
		DockStyle_Bottom	= 2,
		// 요약: The control's left edge is docked to the left edge of its containing control.
		DockStyle_Left		= 3,
		// 요약: The control's right edge is docked to the right edge of its containing control.
		DockStyle_Right		= 4,
		// 요약: All the control's edges are docked to the all edges of its containing control and sized appropriately.
		DockStyle_Fill		= 5,
	};

	enum RectangleEdgeFilter
	{
		None			= 0x00,
		TopLeft			= 0x01,
		TopRight		= 0x02,
		BottomLeft		= 0x04,
		BottomRight		= 0x08,
		All = TopLeft | TopRight | BottomLeft | BottomRight
	};

protected:
	//-------------------------------------------------------------------------
	// GDI+ 그리기 함수
	//-------------------------------------------------------------------------
	// Draws the specified text string on the specified caption surface; within the specified bounds; and in the specified font, color. 
	void DrawString						(Graphics& graphics, RectF& layoutRectangle, Gdiplus::Font* font, Color fontColor, WCHAR* strText, RightToLeft rightToLeft, StringAlignment stringAlignment);
	void DrawIcon						(Graphics& graphics, Image* imgPanelIcon, Color foreColorImage, int iconPositionX, int iconPositionY );
	void DrawImage						(Graphics& graphics, Image* image, Gdiplus::Rect& imageRectangle);

	// Draws the text and image objects at the specified location. 
	void DrawImagesAndText(Graphics& graphics, Gdiplus::Rect& captionRectangle, int iSpacing, Gdiplus::Rect imageRectangle, Image* image, RightToLeft rightToLeft, Gdiplus::Font* fontCaption, Color captionForeColor, WCHAR* strCaptionText, StringAlignment TextAlignment);
	void DrawText(Graphics& graphics, Gdiplus::Rect& captionRectangle, int iSpacing, RightToLeft rightToLeft, Gdiplus::Font* fontCaption, Color captionForeColor, WCHAR* strCaptionText, StringAlignment TextAlignment);
	void DrawVerticalImagesAndText(Graphics& graphics, Gdiplus::Rect& captionRectangle, Gdiplus::Rect& panelRectangle, Gdiplus::Rect imageRectangle, DockStyle dockStyle, Image* image, RightToLeft rightToLeft, Gdiplus::Font* captionFont, Color collapsedCaptionForeColor, WCHAR* strCaptionText);

	// Renders the background of the caption bar
	void RenderDoubleBackgroundGradient(Graphics& graphics, Gdiplus::Rect& bounds, Color beginColor, Color middleColor, Color endColor, LinearGradientMode linearGradientMode, bool flipHorizontal);
	void RenderDoubleBackgroundGradient(Graphics& graphics, Gdiplus::Rect& bounds, Color beginColor, Color middleColor, Color endColor, int firstGradientWidth, int secondGradientWidth, LinearGradientMode mode, bool flipHorizontal);

	// bounds : Rect structure that specifies the backgrounds location.
	void RenderBackgroundGradient(Graphics& graphics, Gdiplus::Rect& bounds, Color beginColor, Color endColor, LinearGradientMode linearGradientMode);
	void RenderButtonBackground(Graphics& graphics, Gdiplus::Rect& bounds, Color colorGradientBegin, Color colorGradientMiddle, Color colorGradientEnd);
	void RenderFlatButtonBackground(Graphics& graphics, Gdiplus::Rect& bounds, Color colorGradientBegin, Color colorGradientEnd, BOOL bHover);

	// 둥근 사각형 그리기 (2016.03.11)
	void GenerateCapsule				(IN RectF rectangle, OUT GraphicsPath& Path);
	void GenerateRoundedRectangle		(IN Graphics& graphics, IN RectF rectangle, IN REAL radius, IN int filter, OUT GraphicsPath& Path);
	void DrawRoundedRectangle			(IN Graphics& graphics, IN Pen* pPen, IN REAL x, IN REAL y, IN REAL width, IN REAL height, IN REAL radius, IN int filter);
	void FillRoundedRectangle			(IN Graphics& graphics, IN Brush* pBrush, IN REAL x, IN REAL y, IN REAL width, IN REAL height, IN REAL radius, IN int filter);

	// Gets a GraphicsPath. 
	void GetPath(Gdiplus::Rect& bounds, int radius, OUT GraphicsPath& graphicsPath);

	// Gets a GraphicsPath with rounded corners on the upper side.
	void GetUpperBackgroundPath(Gdiplus::Rect& bounds, int radius, OUT GraphicsPath& graphicsPath);

	// Gets a GraphicsPath.
	void GetBackgroundPath(Gdiplus::Rect& bounds, int radius, OUT GraphicsPath& graphicsPath);

	// Checks if the rectangle width or height is equal to 0.
	// returns : true if the with or height of the rectangle is 0 else false
	bool IsZeroWidthOrHeight(Gdiplus::Rect& rectangle);

};

#endif // VGBase_h__
