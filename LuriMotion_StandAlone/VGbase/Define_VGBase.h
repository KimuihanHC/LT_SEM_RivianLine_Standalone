//*****************************************************************************
// Filename	: Define_VGBase.h
// Created	: 2013/3/13
// Modified	: 2013/3/13 - 16:08
//
// Author	: PiRing
//	
// Purpose	: 
//*****************************************************************************
#ifndef Define_VGBase_h__
#define Define_VGBase_h__

typedef struct _tagContent
{
	BOOL	UseBackground;	// 배경 그리기 여부
	BOOL	UseBorder;		// 배경 그리기 여부
	Color	BackColor;		// 배경색
	Color	BorderColor;	// 보더 색
	REAL	BorderTickness;

	CStringW		Text;
	CStringW		FontName;
	FontStyle		fontStyle;
	REAL			fontSize;
	StringAlignment	TextAlignment;
	Color			TextColor;

	RectF	rectContent;

	_tagContent ()
	{
		UseBackground	= FALSE;
		UseBorder		= FALSE;
		BackColor		= Color::White;
		BorderColor		= Color::Black;
		BorderTickness	= 1.0f;

		Text			= L"";
		FontName		= L"Arial";
		fontStyle		= FontStyleRegular;
		fontSize		= 8.0f;
		TextAlignment	= StringAlignmentCenter;
		TextColor		= Color::Black;		
	};
}Content;


#endif // Define_VGBase_h__
