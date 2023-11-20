//*****************************************************************************
// Filename	: VGBase.cpp
// Created	: 2013/3/13
// Modified	: 2013/3/13 - 16:38
//
// Author	: PiRing
//	
// Purpose	: 
//*****************************************************************************
#include "../pch.h"
#include "VGBase.h"

//=============================================================================
//
//=============================================================================
CVGBase::CVGBase(void)
{
}

CVGBase::~CVGBase(void)
{
}

//=============================================================================
// Method		: CVGBase::DrawString
// Access		: protected 
// Returns		: void
// Parameter	: Graphics&			graphics
// Parameter	: RectF&			layoutRectangle
// Parameter	: Gdiplus::Font*	font
// Parameter	: Color				fontColor
// Parameter	: WCHAR*			strText
// Parameter	: RightToLeft		rightToLeft
// Parameter	: StringAlignment	stringAlignment
// Qualifier	:
// Last Update	: 2013/2/28 - 11:10
// Desc.		:
//=============================================================================
void CVGBase::DrawString( Graphics& graphics, RectF& layoutRectangle, Gdiplus::Font* font, Color fontColor, WCHAR* strText, RightToLeft rightToLeft, StringAlignment stringAlignment )
{
	SolidBrush		stringBrush(fontColor);
	StringFormat	stringFormat;

	stringFormat.SetFormatFlags(StringFormatFlagsNoWrap);
	if (rightToLeft == RightToLeft_Yes)
	{
		stringFormat.SetFormatFlags(stringFormat.GetFormatFlags() | StringFormatFlagsDirectionRightToLeft);
	}
	stringFormat.SetTrimming(StringTrimmingEllipsisCharacter);
	stringFormat.SetLineAlignment(StringAlignmentCenter);
	stringFormat.SetAlignment (stringAlignment);
	graphics.DrawString(strText, -1, font, layoutRectangle, &stringFormat, &stringBrush);
}

//=============================================================================
// Method		: CVGBase::DrawIcon
// Access		: protected 
// Returns		: void
// Parameter	: Graphics& graphics
// Parameter	: Image *	imgPanelIcon
// Parameter	: Color		foreColorImage
// Parameter	: int		iconPositionX
// Parameter	: int		iconPositionY
// Qualifier	:
// Last Update	: 2013/2/28 - 17:07
// Desc.		:
//=============================================================================
void CVGBase::DrawIcon( Graphics& graphics, Image* imgPanelIcon, Color foreColorImage, int iconPositionX, int iconPositionY )
{	
	int iconWidth		= imgPanelIcon->GetWidth();
	int iconHeight		= imgPanelIcon->GetHeight();

	Rect rectangleIcon(iconPositionX + (iconWidth / 2) - 1, iconPositionY + (iconHeight / 2) - 1, imgPanelIcon->GetWidth(), imgPanelIcon->GetHeight() - 1);

	ImageAttributes imageAttribute;
	imageAttribute.SetColorKey(Color::Magenta, Color::Magenta);
	ColorMap colorMap;

	colorMap.oldColor = Color(0, 60, 166);
	colorMap.newColor = foreColorImage;
	imageAttribute.SetRemapTable(1, &colorMap);

	graphics.DrawImage(imgPanelIcon, rectangleIcon, 0, 0, iconWidth, iconHeight, UnitPixel, &imageAttribute);
}

//=============================================================================
// Method		: CVGBase::DrawImage
// Access		: protected 
// Returns		: void
// Parameter	: Graphics& graphics
// Parameter	: Image *	image
// Parameter	: Rect &	imageRectangle
// Qualifier	:
// Last Update	: 2013/2/28 - 11:11
// Desc.		:
//=============================================================================
void CVGBase::DrawImage( Graphics& graphics, Image* image, Rect& imageRectangle )
{
	if (image != NULL)
	{
		graphics.DrawImage(image, imageRectangle);
	}
}

//=============================================================================
// Method		: CVGBase::DrawImagesAndText
// Access		: protected 
// Returns		: void
// Parameter	: Graphics & graphics
// Parameter	: Rect & captionRectangle
// Parameter	: int iSpacing
// Parameter	: Rect imageRectangle
// Parameter	: Image * image
// Parameter	: RightToLeft rightToLeft
// Parameter	: Gdiplus::Font * fontCaption
// Parameter	: Color captionForeColor
// Parameter	: WCHAR * strCaptionText
// Parameter	: StringAlignment TextAlignment
// Qualifier	:
// Last Update	: 2013/3/13 - 16:31
// Desc.		:
//=============================================================================
void CVGBase::DrawImagesAndText( Graphics& graphics, Rect& captionRectangle, int iSpacing, 
								Rect imageRectangle, Image* image, RightToLeft rightToLeft, Gdiplus::Font* fontCaption, 
								Color captionForeColor, WCHAR* strCaptionText, StringAlignment TextAlignment )
{
	//DrawImages
	int iTextPositionX1 = captionRectangle.GetLeft() + iSpacing;
	int iTextPositionX2 = captionRectangle.GetRight() - iSpacing;

	imageRectangle.Y = (captionRectangle.Height - imageRectangle.Height) / 2;

	if (rightToLeft == RightToLeft_No)
	{
		if (image != NULL)
		{
			DrawImage(graphics, image, imageRectangle);
			iTextPositionX1 += imageRectangle.Width + iSpacing;
		}
	}

	// Draw Caption text
	Rect textRectangle	= captionRectangle;
	textRectangle.X		= iTextPositionX1;
	textRectangle.Width -= (iSpacing + iSpacing);
	if (rightToLeft == RightToLeft_Yes)
	{
		if (image != NULL)
		{
			Rect imageRectangleRight = imageRectangle;
			imageRectangleRight.X = iTextPositionX2 - imageRectangle.Width;
			DrawImage(graphics, image, imageRectangleRight);
			iTextPositionX2 = imageRectangleRight.X - iSpacing;
		}
	}
	textRectangle.Width = iTextPositionX2 - iTextPositionX1;	

	DrawString(graphics, RectF((REAL)textRectangle.X, (REAL)textRectangle.Y, (REAL)textRectangle.Width, (REAL)textRectangle.Height), fontCaption, captionForeColor, strCaptionText, rightToLeft, TextAlignment);
}

//=============================================================================
// Method		: DrawText
// Access		: protected  
// Returns		: void
// Parameter	: Graphics & graphics
// Parameter	: Rect & captionRectangle
// Parameter	: int iSpacing
// Parameter	: RightToLeft rightToLeft
// Parameter	: Gdiplus::Font * fontCaption
// Parameter	: Color captionForeColor
// Parameter	: WCHAR * strCaptionText
// Parameter	: StringAlignment TextAlignment
// Qualifier	:
// Last Update	: 2016/11/21 - 20:18
// Desc.		:
//=============================================================================
void CVGBase::DrawText(Graphics& graphics, Rect& captionRectangle, int iSpacing, RightToLeft rightToLeft, Gdiplus::Font* fontCaption, Color captionForeColor, WCHAR* strCaptionText, StringAlignment TextAlignment)
{
	//DrawImages
	int iTextPositionX1 = captionRectangle.GetLeft() + iSpacing;
	int iTextPositionX2 = captionRectangle.GetRight() - iSpacing;

	// Draw Caption text
	Rect textRectangle = captionRectangle;
	textRectangle.X = iTextPositionX1;
	textRectangle.Width -= (iSpacing + iSpacing);
	textRectangle.Width = iTextPositionX2 - iTextPositionX1;

	DrawString(graphics, RectF((REAL)textRectangle.X, (REAL)textRectangle.Y, (REAL)textRectangle.Width, (REAL)textRectangle.Height), fontCaption, captionForeColor, strCaptionText, rightToLeft, TextAlignment);
}

//=============================================================================
// Method		: CVGBase::DrawVerticalImagesAndText
// Access		: protected 
// Returns		: void
// Parameter	: Graphics &		graphics
// Parameter	: Rect &			captionRectangle
// Parameter	: Rect &			panelRectangle
// Parameter	: Rect				imageRectangle
// Parameter	: DockStyle			dockStyle
// Parameter	: Image *			image
// Parameter	: RightToLeft		rightToLeft
// Parameter	: Gdiplus::Font *	captionFont
// Parameter	: Color				collapsedCaptionForeColor
// Parameter	: WCHAR *			strCaptionText
// Qualifier	:
// Last Update	: 2013/2/28 - 11:19
// Desc.		:
//=============================================================================
void CVGBase::DrawVerticalImagesAndText( Graphics& graphics, Rect& captionRectangle, Rect& panelRectangle, Rect imageRectangle, 
											 DockStyle dockStyle, Image* image, RightToLeft rightToLeft, 											 
											 Gdiplus::Font* captionFont, Color collapsedCaptionForeColor, WCHAR* strCaptionText )
{
	imageRectangle.Y = (captionRectangle.Height - imageRectangle.Height) / 2;

	int iTextPositionY1 = VG_SPACING;
	int iTextPositionY2 = panelRectangle.Height - VG_SPACING;

	if (image != NULL)
	{
		imageRectangle.Y = iTextPositionY2 - imageRectangle.Height;
		imageRectangle.X = (panelRectangle.Width - imageRectangle.Width) / 2;
		DrawImage(graphics, image, imageRectangle);
		iTextPositionY1 += imageRectangle.Height + VG_SPACING;
	}

	iTextPositionY2 -= captionRectangle.Height + iTextPositionY1;

	RectF textRectangle((REAL)iTextPositionY1, (REAL)panelRectangle.Y, (REAL)iTextPositionY2, (REAL)captionRectangle.Height);

	SolidBrush textBrush(collapsedCaptionForeColor);
	{
		if (dockStyle == DockStyle_Left)
		{
			graphics.TranslateTransform(0, (REAL)panelRectangle.Height);
			graphics.RotateTransform(-90);

			DrawString(graphics, textRectangle, captionFont, collapsedCaptionForeColor, strCaptionText, rightToLeft, StringAlignmentCenter);

			graphics.ResetTransform();
		}
		else if (dockStyle == DockStyle_Right)
		{
			graphics.TranslateTransform((REAL)panelRectangle.Width, 0);
			graphics.RotateTransform(90);

			DrawString(graphics, textRectangle, captionFont, collapsedCaptionForeColor, strCaptionText, rightToLeft, StringAlignmentCenter);

			graphics.ResetTransform();
		}
	}
}

//=============================================================================
// Method		: CVGBase::RenderDoubleBackgroundGradient
// Access		: protected 
// Returns		: void
// Parameter	: Graphics &			graphics
// Parameter	: Rect &				bounds
// Parameter	: Color					beginColor
// Parameter	: Color					middleColor
// Parameter	: Color					endColor
// Parameter	: int					firstGradientWidth
// Parameter	: int					secondGradientWidth
// Parameter	: LinearGradientMode	mode
// Parameter	: bool					flipHorizontal
// Qualifier	:
// Last Update	: 2013/2/28 - 11:18
// Desc.		:
//=============================================================================
void CVGBase::RenderDoubleBackgroundGradient( Graphics& graphics, Rect& bounds, Color beginColor, Color middleColor, Color endColor, int firstGradientWidth, int secondGradientWidth, LinearGradientMode mode, bool flipHorizontal )
{
	if ((bounds.Width != 0) && (bounds.Height != 0))
	{
		Rect rectangle1 = bounds;
		Rect rectangle2 = bounds;
		bool flag1 = true;
		if (mode == LinearGradientModeHorizontal)
		{
			if (flipHorizontal)
			{
				Color color1 = endColor;
				endColor = beginColor;
				beginColor = color1;
			}
			rectangle2.Width = firstGradientWidth;
			rectangle1.Width = secondGradientWidth + 1;
			rectangle1.X = bounds.GetRight() - rectangle1.Width;
			flag1 = bounds.Width > (firstGradientWidth + secondGradientWidth);
		}
		else
		{
			rectangle2.Height = firstGradientWidth;
			rectangle1.Height = secondGradientWidth + 1;
			rectangle1.Y = bounds.GetBottom() - rectangle1.Height;
			flag1 = bounds.Height > (firstGradientWidth + secondGradientWidth);
		}
		if (flag1)
		{
			SolidBrush brush1(middleColor);
			{
				graphics.FillRectangle(&brush1, bounds);
			}
			LinearGradientBrush brush2(rectangle2, beginColor, middleColor, mode);
			{
				graphics.FillRectangle(&brush2, rectangle2);
			}
			LinearGradientBrush brush3(rectangle1, middleColor, endColor, mode);
			{
				if (mode == LinearGradientModeHorizontal)
				{
					rectangle1.X++;
					rectangle1.Width--;
				}
				else
				{
					rectangle1.Y++;
					rectangle1.Height--;
				}
				graphics.FillRectangle(&brush3, rectangle1);
				return;
			}
		}
		LinearGradientBrush brush4(bounds, beginColor, endColor, mode);
		{
			graphics.FillRectangle(&brush4, bounds);
		}
	}
}

void CVGBase::RenderDoubleBackgroundGradient( Graphics& graphics, Rect& bounds, Color beginColor, Color middleColor, Color endColor, LinearGradientMode linearGradientMode, bool flipHorizontal )
{
	int iUpperHeight = bounds.Height / 2;
	int iLowerHeight = bounds.Height - iUpperHeight;

	RenderDoubleBackgroundGradient(graphics, bounds, beginColor, middleColor, endColor, iUpperHeight, iLowerHeight, linearGradientMode, flipHorizontal);
}

//=============================================================================
// Method		: CVGBase::RenderBackgroundGradient
// Access		: protected 
// Returns		: void
// Parameter	: Graphics &			graphics
// Parameter	: Rect &				bounds
// Parameter	: Color					beginColor
// Parameter	: Color					endColor
// Parameter	: LinearGradientMode	linearGradientMode
// Qualifier	:
// Last Update	: 2013/2/28 - 11:18
// Desc.		:
//=============================================================================
void CVGBase::RenderBackgroundGradient( Graphics& graphics, Rect& bounds, Color beginColor, Color endColor, LinearGradientMode linearGradientMode )
{
	if (IsZeroWidthOrHeight(bounds))
		return;

	LinearGradientBrush linearGradientBrush(bounds, beginColor, endColor, linearGradientMode);
	{
		graphics.FillRectangle(&linearGradientBrush, bounds);
	}
}

//=============================================================================
// Method		: CVGBase::RenderButtonBackground
// Access		: protected 
// Returns		: void
// Parameter	: Graphics &	graphics
// Parameter	: Rect &		bounds
// Parameter	: Color			colorGradientBegin
// Parameter	: Color			colorGradientMiddle
// Parameter	: Color			colorGradientEnd
// Qualifier	:
// Last Update	: 2013/2/28 - 11:19
// Desc.		:
//=============================================================================
void CVGBase::RenderButtonBackground( Graphics& graphics, Rect& bounds, Color colorGradientBegin, Color colorGradientMiddle, Color colorGradientEnd )
{
	if (bounds.Height < bounds.Width) // 가로가 세로보다 길때
	{
		RectF rectUpper ((REAL)bounds.X, (REAL)bounds.Y, (REAL)bounds.Width, (REAL)bounds.Height);
		rectUpper.Height = bounds.Height * 0.4f;		

		LinearGradientBrush upperLinearGradientBrush(rectUpper, colorGradientBegin, colorGradientMiddle, LinearGradientModeVertical);

		float Positions[] = { 0.0F, 1.0F };
		float Factors[] = { 0.0F, 0.6F };
		upperLinearGradientBrush.SetBlend(Factors, Positions, 2);
		graphics.FillRectangle(&upperLinearGradientBrush, rectUpper);

		RectF rectLower ((REAL)bounds.X, (REAL)bounds.Y, (REAL)bounds.Width, (REAL)bounds.Height);
		rectLower.Y = bounds.Y + rectUpper.Height;
		rectLower.Height = bounds.Height - rectUpper.Height;

		LinearGradientBrush lowerLinearGradientBrush(rectLower, colorGradientMiddle, colorGradientEnd, LinearGradientModeVertical);

		graphics.FillRectangle(&lowerLinearGradientBrush, rectLower);

		//At some captionheights there are drawing errors. This is the correction
		RectF correctionRectangle = rectLower;
		correctionRectangle.Y -= 1;
		correctionRectangle.Height = 2;		

		SolidBrush solidBrush(colorGradientMiddle);
		graphics.FillRectangle(&solidBrush, correctionRectangle);
	}
	else // 세로가 가로보다 길때
	{
		RectF rectLeft ((REAL)bounds.X, (REAL)bounds.Y, (REAL)bounds.Width, (REAL)bounds.Height);
		rectLeft.Width = bounds.Width * 0.4f;		

		LinearGradientBrush upperLinearGradientBrush(rectLeft, colorGradientBegin, colorGradientMiddle, LinearGradientModeHorizontal);

		float Positions[] = { 0.0F, 1.0F };
		float Factors[] = { 0.0F, 0.6F };				
		upperLinearGradientBrush.SetBlend(Factors, Positions, 2);
		graphics.FillRectangle(&upperLinearGradientBrush, rectLeft);

		RectF rectRight ((REAL)bounds.X, (REAL)bounds.Y, (REAL)bounds.Width, (REAL)bounds.Height);
		rectRight.X = bounds.X + rectLeft.Width;
		rectRight.Width = bounds.Width - rectLeft.Width;

		LinearGradientBrush lowerLinearGradientBrush(rectRight, colorGradientMiddle, colorGradientEnd, LinearGradientModeHorizontal);

		graphics.FillRectangle(&lowerLinearGradientBrush, rectRight);

		//At some captionheights there are drawing errors. This is the correction
		RectF correctionRectangle = rectRight;
		correctionRectangle.X -= 1;
		correctionRectangle.Width = 2;		

		SolidBrush solidBrush(colorGradientMiddle);
		graphics.FillRectangle(&solidBrush, correctionRectangle);
	}
}

//=============================================================================
// Method		: CVGBase::RenderFlatButtonBackground
// Access		: protected 
// Returns		: void
// Parameter	: Graphics &	graphics
// Parameter	: Rect &		bounds
// Parameter	: Color			colorGradientBegin
// Parameter	: Color			colorGradientEnd
// Parameter	: BOOL			bHover
// Qualifier	:
// Last Update	: 2013/2/28 - 11:19
// Desc.		:
//=============================================================================
void CVGBase::RenderFlatButtonBackground( Graphics& graphics, Rect& bounds, Color colorGradientBegin, Color colorGradientEnd, BOOL bHover )
{
	float Positions[]	= { 0.0F, 0.2F, 0.3F, 0.4F, 0.5F, 0.6F, 0.7F, 0.8F, 1.0F };
	float Factors[]		= { 0.4F, 0.5F, 0.6F, 0.8F, 1.0F, 0.8F, 0.6F, 0.5F, 0.4F };

	if (FALSE == bHover)
	{
		float FactorsHover[] = { 0.0F, 0.0F, 0.2F, 0.4F, 0.6F, 0.4F, 0.2F, 0.0F, 0.0F };
		memcpy (Factors, FactorsHover, (sizeof(float) * 9));
	}

	LinearGradientBrush gradientBrush(bounds, colorGradientBegin, colorGradientEnd, LinearGradientModeHorizontal);		

	gradientBrush.SetBlend(Factors, Positions, 9);
	graphics.FillRectangle(&gradientBrush, bounds);
}

void CVGBase::GenerateCapsule(IN RectF rectangle, OUT GraphicsPath& Path)
{
	REAL diameter;
	Gdiplus::RectF arc;

	try
	{
		if (rectangle.Width > rectangle.Height)
		{
			diameter = rectangle.Height;
			Gdiplus::SizeF sizeF(diameter, diameter);

			Gdiplus::PointF ptF;
			rectangle.GetLocation(&ptF);

			arc = Gdiplus::RectF(ptF, sizeF);
			Path.AddArc(arc, 90, 180);

			arc.X = rectangle.GetRight() - diameter;
			Path.AddArc(arc, 270, 180);
		}
		else if (rectangle.Width < rectangle.Height)
		{
			diameter = rectangle.Width;
			Gdiplus::SizeF sizeF(diameter, diameter);

			Gdiplus::PointF ptF;
			rectangle.GetLocation(&ptF);

			arc = Gdiplus::RectF(ptF, sizeF);
			Path.AddArc(arc, 180, 180);

			arc.Y = rectangle.GetBottom() - diameter;
			Path.AddArc(arc, 0, 180);
		}
		else
		{
			Path.AddEllipse(rectangle);
		}
	}
	catch (...)
	{
		Path.AddEllipse(rectangle);
	}
		
	Path.CloseFigure();
}

void CVGBase::GenerateRoundedRectangle(IN Graphics& graphics, IN RectF rectangle, IN REAL radius, IN int filter, OUT GraphicsPath& Path)
{
	REAL diameter;

	if (radius <= 0.0F || filter == RectangleEdgeFilter::None)
	{
		Path.AddRectangle(rectangle);
		Path.CloseFigure();
		return;
	}
	else
	{
		if (radius >= (min(rectangle.Width, rectangle.Height)) / 2.0)
		{
			GenerateCapsule(rectangle, Path);
			return;
		}

		diameter = radius * 2.0F;

		Gdiplus::SizeF sizeF((REAL)diameter, (REAL)diameter);
		Gdiplus::PointF ptF;
		rectangle.GetLocation(&ptF);
		Gdiplus::RectF arc(ptF, sizeF);

		if ((RectangleEdgeFilter::TopLeft & filter) == RectangleEdgeFilter::TopLeft)
		{
			Path.AddArc(arc, 180, 90);
		}
		else
		{
			Path.AddLine(arc.X, arc.Y + arc.Height, arc.X, arc.Y);
			Path.AddLine(arc.X, arc.Y, arc.X + arc.Width, arc.Y);
		}

		arc.X = rectangle.GetRight() - diameter;
		if ((RectangleEdgeFilter::TopRight & filter) == RectangleEdgeFilter::TopRight)
		{
			Path.AddArc(arc, 270, 90);
		}
		else
		{
			Path.AddLine(arc.X, arc.Y, arc.X + arc.Width, arc.Y);
			Path.AddLine(arc.X + arc.Width, arc.Y + arc.Height, arc.X + arc.Width, arc.Y);
		}

		arc.Y = rectangle.GetBottom() - diameter;
		if ((RectangleEdgeFilter::BottomRight & filter) == RectangleEdgeFilter::BottomRight)
		{
			Path.AddArc(arc, 0, 90);
		}
		else
		{
			Path.AddLine(arc.X + arc.Width, arc.Y, arc.X + arc.Width, arc.Y + arc.Height);
			Path.AddLine(arc.X, arc.Y + arc.Height, arc.X + arc.Width, arc.Y + arc.Height);
		}

		arc.X = rectangle.GetLeft();
		if ((RectangleEdgeFilter::BottomLeft & filter) == RectangleEdgeFilter::BottomLeft)
		{
			Path.AddArc(arc, 90, 90);
		}
		else
		{
			Path.AddLine(arc.X + arc.Width, arc.Y + arc.Height, arc.X, arc.Y + arc.Height);
			Path.AddLine(arc.X, arc.Y + arc.Height, arc.X, arc.Y);
		}

		Path.CloseFigure();
	}
}

void CVGBase::DrawRoundedRectangle(IN Graphics& graphics, IN Pen* pPen, IN REAL x, IN REAL y, IN REAL width, IN REAL height, IN REAL radius, IN int filter)
{
	Gdiplus::RectF rectangle(x, y, width, height);
	Gdiplus::GraphicsPath path;
	GenerateRoundedRectangle(graphics, rectangle, radius, filter, path);
	Gdiplus::SmoothingMode old = graphics.GetSmoothingMode();
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);
	graphics.DrawPath(pPen, &path);
	graphics.SetSmoothingMode(old);
}

void CVGBase::FillRoundedRectangle(IN Graphics& graphics, IN Brush* pBrush, IN REAL x, IN REAL y, IN REAL width, IN REAL height, IN REAL radius, IN int filter)
{
	Gdiplus::RectF rectangle(x, y, width, height);

	Gdiplus::GraphicsPath path;
	GenerateRoundedRectangle(graphics, rectangle, radius, filter, path);

	Gdiplus::SmoothingMode old = graphics.GetSmoothingMode();
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);
	graphics.FillPath(pBrush, &path);
	graphics.SetSmoothingMode(old);
}

//=============================================================================
// Method		: CVGBase::GetPath
// Access		: protected 
// Returns		: void
// Parameter	: Rect &			bounds
// Parameter	: int				radius
// Parameter	: GraphicsPath &	graphicsPath
// Qualifier	:
// Last Update	: 2013/2/28 - 11:19
// Desc.		:
//=============================================================================
void CVGBase::GetPath( Rect& bounds, int radius, GraphicsPath& graphicsPath )
{
	int x = bounds.X;
	int y = bounds.Y;
	int width = bounds.Width;
	int height = bounds.Height;

	graphicsPath.AddArc(x, y, radius, radius, 180, 90);				                    //Upper left corner
	graphicsPath.AddArc(x + width - radius, y, radius, radius, 270, 90);			    //Upper right corner
	graphicsPath.AddArc(x + width - radius, y + height - radius, radius, radius, 0, 90);//Lower right corner
	graphicsPath.AddArc(x, y + height - radius, radius, radius, 90, 90);			                    //Lower left corner
	graphicsPath.CloseFigure();
}

//=============================================================================
// Method		: CVGBase::GetUpperBackgroundPath
// Access		: protected 
// Returns		: void
// Parameter	: Rect &			bounds
// Parameter	: int				radius
// Parameter	: GraphicsPath &	graphicsPath
// Qualifier	:
// Last Update	: 2013/2/28 - 11:19
// Desc.		:
//=============================================================================
void CVGBase::GetUpperBackgroundPath( Rect& bounds, int radius, GraphicsPath& graphicsPath )
{
	int x = bounds.X;
	int y = bounds.Y;
	int width = bounds.Width;
	int height = bounds.Height;
	graphicsPath.AddLine(x, y + height, x, y - radius);                 //Left Line
	graphicsPath.AddArc(x, y, radius, radius, 180, 90);                 //Upper left corner
	graphicsPath.AddArc(x + width - radius, y, radius, radius, 270, 90);//Upper right corner
	graphicsPath.AddLine(x + width, y + radius, x + width, y + height); //Right Line
	graphicsPath.CloseFigure();
}

//=============================================================================
// Method		: CVGBase::GetBackgroundPath
// Access		: protected 
// Returns		: void
// Parameter	: Rect &			bounds
// Parameter	: int				radius
// Parameter	: GraphicsPath &	graphicsPath
// Qualifier	:
// Last Update	: 2013/2/28 - 11:19
// Desc.		:
//=============================================================================
void CVGBase::GetBackgroundPath( Rect& bounds, int radius, GraphicsPath& graphicsPath )
{
	int x = bounds.X;
	int y = bounds.Y;
	int width = bounds.Width;
	int height = bounds.Height;

	graphicsPath.AddArc(x, y, radius, radius, 180, 90);				                    //Upper left corner
	graphicsPath.AddArc(x + width - radius, y, radius, radius, 270, 90);			    //Upper right corner
	graphicsPath.AddArc(x + width - radius, y + height - radius, radius, radius, 0, 90);//Lower right corner
	graphicsPath.AddArc(x, y + height - radius, radius, radius, 90, 90);			    //Lower left corner
	graphicsPath.CloseFigure();
}

//=============================================================================
// Method		: CVGBase::IsZeroWidthOrHeight
// Access		: protected 
// Returns		: bool
// Parameter	: Rect & rectangle
// Qualifier	:
// Last Update	: 2013/2/28 - 11:19
// Desc.		:
//=============================================================================
bool CVGBase::IsZeroWidthOrHeight( Rect& rectangle )
{
	if (rectangle.Width != 0)
	{
		return (rectangle.Height == 0);
	}

	return true;
}
