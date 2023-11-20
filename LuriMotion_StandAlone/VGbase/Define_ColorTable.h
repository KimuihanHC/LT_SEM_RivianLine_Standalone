//*****************************************************************************
// Filename	: Define_ColorTable.h
// Created	: 2013/3/7
// Modified	: 2013/3/7 - 9:17
//
// Author	: PiRing
//	
// Purpose	: 
//*****************************************************************************
#ifndef Define_ColorTable_h__
#define Define_ColorTable_h__


typedef struct _tagPanelColors
{
	// The backcolor of a Panel.		
	Color BackColor;
	// The border color of the panel.
	Color BorderColor;
	// The inner border color of a Panel.
	Color InnerBorderColor;

	// The text color of a Panel.
	Color CaptionText;
	// The text color used when the Panel is selected.
	Color CaptionSelectedText;
	// The text color of a Panel when it's collapsed. 패널이 축소되어 헤더만 보일 경우
	Color CollapsedCaptionText;

	// The color of the gradient used in the Panel.
	Color ContentGradientBegin;
	Color ContentGradientEnd;

	// The color of the gradient of a flat Panel.
	Color FlatCaptionGradientBegin;
	Color FlatCaptionGradientEnd;
	
	// The color of the gradient of the Panel.
	Color CaptionGradientBegin;
	Color CaptionGradientMiddle;
	Color CaptionGradientEnd;		
			
	// The color of the gradient used when the Panel is pressed down.
	Color CaptionPressedGradientBegin;
	Color CaptionPressedGradientMid;
	Color CaptionPressedGradientEnd;
	
	// The color of the gradient used when the Panel is checked.
	Color CaptionCheckedGradientBegin;
	Color CaptionCheckedGradientMid;
	Color CaptionCheckedGradientEnd;

	// The color of the gradient used when the Panel is selected.
	Color CaptionSelectedGradientBegin;
	Color CaptionSelectedGradientMid;
	Color CaptionSelectedGradientEnd;
	

	_tagPanelColors ()
	{
		BackColor					= Color::Transparent;
		BorderColor					= Color(101, 147, 207);
		InnerBorderColor			= Color::White;
		
		CaptionText					= Color(21, 66, 139);
		CaptionSelectedText			= Color::White;
		CollapsedCaptionText		= Color(21, 66, 139);

		ContentGradientBegin		= Color(227, 239, 255);
		ContentGradientEnd			= Color(227, 239, 255);	
		FlatCaptionGradientBegin	= Color(214, 232, 255);
		FlatCaptionGradientEnd		= Color(253, 253, 254);			
		
		CaptionGradientBegin		= Color(227, 239, 255);
		CaptionGradientEnd			= Color(199, 224, 255);
		CaptionGradientMiddle		= Color(173, 209, 255);		

		CaptionCheckedGradientBegin	= Color(136, 144, 254);
		CaptionCheckedGradientMid	= Color(42, 52, 254);
		CaptionCheckedGradientEnd	= Color(111, 145, 255);
		
		CaptionPressedGradientBegin	= Color(106, 109, 228);
		CaptionPressedGradientMid	= Color(39, 39, 217);
		CaptionPressedGradientEnd	= Color(88, 111, 226);

		CaptionSelectedGradientBegin= Color(156, 163, 254);
		CaptionSelectedGradientMid	= Color(90, 98, 254);
		CaptionSelectedGradientEnd	= Color(139, 164, 255);
	};

	void InitColor_Black ()
	{
		// Black
		BackColor					= Color(0, 0, 0);//Color(240, 241, 242);
		BorderColor					= Color(0, 0, 0);
		InnerBorderColor			= Color(185, 185, 185);
		CaptionText					= Color::White;
		CaptionSelectedText			= Color::White;
		CollapsedCaptionText		= Color(0, 0, 0);

		CaptionGradientBegin		= Color(122, 122, 122);
		CaptionGradientMiddle		= Color(80, 80, 80);
		CaptionGradientEnd			= Color(0, 0, 0);			
		CaptionCheckedGradientBegin	= Color(155, 155, 155);
		CaptionCheckedGradientMid	= Color(0, 0, 0);
		CaptionCheckedGradientEnd	= Color(47, 47, 47);
		
		ContentGradientBegin		= Color(240, 241, 242);
		ContentGradientEnd			= Color(240, 241, 242);			
		FlatCaptionGradientBegin	= Color(90, 90, 90);
		FlatCaptionGradientEnd		= Color(155, 155, 155);
	};

	void InitColor_Black2007 ()
	{
		// Black - Office2007
		BackColor					= Color(50, 50, 50);//Color::Transparent;
		BorderColor					= Color(76, 83, 92);
		InnerBorderColor			= Color::White;
		CaptionText					= Color(55, 60, 67);
		CaptionSelectedText			= Color::Yellow;
		CollapsedCaptionText		= Color(0, 0, 0);

		CaptionGradientBegin		= Color(240, 241, 242);
		CaptionGradientMiddle		= Color(216, 219, 223);
		CaptionGradientEnd			= Color(189, 193, 200);
		
		CaptionCheckedGradientBegin	= Color(248, 248, 249);
		CaptionCheckedGradientMid	= Color(200, 204, 209);
		CaptionCheckedGradientEnd	= Color(219, 222, 226);

		ContentGradientBegin		= Color(240, 241, 242);
		ContentGradientEnd			= Color(240, 241, 242);
		FlatCaptionGradientBegin	= Color(212, 215, 219);
		FlatCaptionGradientEnd		= Color(253, 253, 254);
	};

	void InitColor_Blue2007 ()
	{		
		BackColor					= Color(173, 209, 255);//Color::Transparent;
		BorderColor					= Color(101, 147, 207);
		InnerBorderColor			= Color::White;
		CaptionText					= Color(22, 65, 139);
		CaptionSelectedText			= Color::Yellow;
		CollapsedCaptionText		= Color(21, 66, 139);

		CaptionGradientBegin		= Color(227, 239, 255);
		CaptionGradientMiddle		= Color(199, 224, 255);
		CaptionGradientEnd			= Color(173, 209, 255);
		
		CaptionCheckedGradientBegin	= Color(227, 239, 255);
		CaptionCheckedGradientMid	= Color(173, 209, 255);
		CaptionCheckedGradientEnd	= Color(199, 224, 255);			

		ContentGradientBegin		= Color(227, 239, 255);
		ContentGradientEnd			= Color(227, 239, 255);
		FlatCaptionGradientBegin	= Color(214, 232, 255);
		FlatCaptionGradientEnd		= Color(253, 253, 254);
	};

	void InitColor_Skyblue ()
	{				
		BackColor					= Color(57, 127, 209);//Color::White;
		BorderColor					= Color(42, 77, 171);
		
		CaptionGradientBegin		= Color(148, 197, 239);
		CaptionGradientEnd			= Color(57, 127, 209);
		CaptionGradientMiddle		= Color(103, 157, 222);
		CaptionSelectedGradientBegin= Color(153, 204, 255);
		CaptionSelectedGradientEnd	= Color(92, 149, 234);
		CaptionSelectedGradientMid	= Color(70, 134, 230);

		CaptionCheckedGradientBegin = Color(153, 204, 255);
		CaptionCheckedGradientEnd	= Color(103, 157, 222);
		CaptionCheckedGradientMid	= Color(57, 127, 209);
		CaptionPressedGradientBegin = Color(153, 204, 255);
		CaptionPressedGradientEnd	= Color(57, 127, 209);
		CaptionPressedGradientMid	= Color(103, 157, 222);
		
		CaptionSelectedText			= Color::White;
		CaptionText					= Color(217, 225, 246);
		FlatCaptionGradientBegin	= Color(248, 248, 248);
		FlatCaptionGradientEnd		= Color(252, 252, 252);
		InnerBorderColor			= Color::White;
	};
	
	void InitColor_DarkBrown ()
	{
		BackColor					= Color(93, 48, 20);//Color::Transparent;
		BorderColor					= Color(128, 129, 107);
		CaptionCheckedGradientBegin = Color(242, 126, 120);
		CaptionCheckedGradientEnd	= Color(196, 23, 18);
		CaptionCheckedGradientMid	= Color(109, 25, 13);
		CaptionGradientBegin		= Color(169, 123, 112);
		CaptionGradientEnd			= Color(93, 48, 20);
		CaptionGradientMiddle		= Color(56, 47, 22);
		CaptionPressedGradientBegin	= Color(255, 114, 107);
		CaptionPressedGradientEnd	= Color(212, 5, 0);
		CaptionPressedGradientMid	= Color(122, 14, 0);
		CaptionSelectedGradientBegin= Color(212, 143, 140);
		CaptionSelectedGradientEnd	= Color(192, 26, 21);
		CaptionSelectedGradientMid	= Color(104, 31, 17);
		CaptionSelectedText			= Color(246, 246, 247);
		CaptionText					= Color(246, 246, 247);
		FlatCaptionGradientBegin	= Color(248, 248, 248);
		FlatCaptionGradientEnd		= Color(252, 252, 252);
		InnerBorderColor			= Color::White;
	};

	void InitColor_Blue ()
	{
		BackColor					= Color::Blue;//Color(240, 241, 242);
		BorderColor					= Color::Black;
		CaptionCheckedGradientBegin	= Color(136, 144, 254);
		CaptionCheckedGradientEnd	= Color(111, 145, 255);
		CaptionCheckedGradientMid	= Color(42, 52, 254);
		CaptionGradientBegin		= Color(128, 128, 255);
		CaptionGradientEnd			= Color::Blue;
		CaptionGradientMiddle		= Color(0, 0, 192);
		CaptionPressedGradientBegin	= Color(106, 109, 228);
		CaptionPressedGradientEnd	= Color(88, 111, 226);
		CaptionPressedGradientMid	= Color(39, 39, 217);
		CaptionSelectedGradientBegin= Color(156, 163, 254);
		CaptionSelectedGradientEnd	= Color(139, 164, 255);
		CaptionSelectedGradientMid	= Color(90, 98, 254);
		CaptionSelectedText			= Color::White;
		CaptionText					= Color::White;
		FlatCaptionGradientBegin	= Color(248, 248, 248);
		FlatCaptionGradientEnd		= Color(252, 252, 252);
		InnerBorderColor			= Color::White;		
	};

	void InitColor_Orange ()
	{
		BackColor					= Color(201, 92, 25);//Color::White;
		BorderColor					= Color(190, 81, 14);
		CaptionCheckedGradientBegin	= Color(255, 166, 104);
		CaptionCheckedGradientEnd	= Color(201, 92, 25);
		CaptionCheckedGradientMid	= Color(240, 90, 26);
		CaptionGradientBegin		= Color(255, 166, 104);
		CaptionGradientEnd			= Color(201, 92, 25);
		CaptionGradientMiddle		= Color(240, 90, 26);
		CaptionPressedGradientBegin	= Color(255, 166, 104);
		CaptionPressedGradientEnd	= Color(201, 92, 25);
		CaptionPressedGradientMid	= Color(240, 90, 26);
		CaptionSelectedGradientBegin= Color(255, 166, 104);
		CaptionSelectedGradientEnd	= Color(201, 92, 25);
		CaptionSelectedGradientMid	= Color(240, 90, 26);
		CaptionSelectedText			= Color(255, 237, 200);
		CaptionText					= Color(255, 237, 200);
		FlatCaptionGradientBegin	= Color(248, 248, 248);
		FlatCaptionGradientEnd		= Color(252, 252, 252);
		InnerBorderColor			= Color(255, 196, 148);		
	};

	void InitColor_LightGreen ()
	{
		BackColor					= Color(69,	157, 69);//Color::White;
		BorderColor					= Color(7, 123, 55);

		CaptionCheckedGradientBegin	= Color(173, 255, 47);
		CaptionCheckedGradientEnd	= Color(139, 200, 120);
		CaptionCheckedGradientMid	= Color(155, 235, 80);

		CaptionGradientBegin		= Color(144, 238, 144);
		CaptionGradientEnd			= Color(69,	157, 69);
		CaptionGradientMiddle		= Color(109, 198, 103);

		CaptionPressedGradientBegin	= Color(123, 209, 217);
		CaptionPressedGradientEnd	= Color(18, 185, 196);
		CaptionPressedGradientMid	= Color(0, 162, 172);

		CaptionSelectedGradientBegin= Color(144, 175, 180);
		CaptionSelectedGradientEnd	= Color(10, 83, 89);
		CaptionSelectedGradientMid	= Color(10, 83, 89);

		CaptionSelectedText			= Color::White;
		CaptionText					= Color::White;
		FlatCaptionGradientBegin	= Color(248, 248, 248);
		FlatCaptionGradientEnd		= Color(252, 252, 252);
		InnerBorderColor			= Color::White;		
	};
	
	void InitColor_Turquoise ()
	{
		BackColor					= Color::White;
		BorderColor					= Color(7, 123, 155);
		CaptionCheckedGradientBegin	= Color(139, 200, 205);
		CaptionCheckedGradientEnd	= Color(7, 123, 155);
		CaptionCheckedGradientMid	= Color(14, 137, 145);

		CaptionGradientBegin		= Color(139, 200, 205);
		CaptionGradientEnd			= Color(7, 123, 155);
		CaptionGradientMiddle		= Color(14, 137, 145);

		CaptionPressedGradientBegin	= Color(123, 209, 217);
		CaptionPressedGradientEnd	= Color(18, 185, 196);
		CaptionPressedGradientMid	= Color(0, 162, 172);
		CaptionSelectedGradientBegin= Color(144, 175, 180);
		CaptionSelectedGradientEnd	= Color(10, 83, 89);
		CaptionSelectedGradientMid	= Color(10, 83, 89);
		CaptionSelectedText			= Color::White;
		CaptionText					= Color::White;
		FlatCaptionGradientBegin	= Color(248, 248, 248);
		FlatCaptionGradientEnd		= Color(252, 252, 252);
		InnerBorderColor			= Color::White;		
	};

	void InitColor_Sky ()
	{
		BackColor					= Color(252, 252, 252);//Color::White;
		BorderColor					= Color(184, 184, 184);
		CaptionCheckedGradientBegin	= Color(255, 166, 104);
		CaptionCheckedGradientEnd	= Color(201, 92, 25);
		CaptionCheckedGradientMid	= Color(240, 90, 26);
		CaptionGradientBegin		= Color(252, 252, 252);
		CaptionGradientEnd			= Color(252, 252, 252);
		CaptionGradientMiddle		= Color(248, 248, 248);
		CaptionPressedGradientBegin = Color(153, 204, 255);
		CaptionPressedGradientEnd	= Color(153, 204, 255);
		CaptionPressedGradientMid	= Color(153, 204, 255);
		CaptionSelectedGradientBegin= Color(194, 224, 255);
		CaptionSelectedGradientEnd	= Color(194, 224, 255);
		CaptionSelectedGradientMid	= Color(194, 224, 255);
		CaptionSelectedText			= Color::Black;
		CaptionText					= Color(21, 66, 139);
		FlatCaptionGradientBegin	= Color(248, 248, 248);
		FlatCaptionGradientEnd		= Color(252, 252, 252);
		InnerBorderColor			= Color::White;
	};

	void InitColor_White ()
	{
		BackColor					= Color::White;
		BorderColor					= Color::Black;
		InnerBorderColor			= Color(185, 185, 185);
		CaptionText					= Color::Black;
		CaptionSelectedText			= Color::Black;
		CollapsedCaptionText		= Color::Black;

		CaptionGradientBegin		= Color(255, 255, 255);
		CaptionGradientMiddle		= Color(195, 195, 195);
		CaptionGradientEnd			= Color(160, 160, 160);

		CaptionCheckedGradientBegin	= Color(155, 155, 155);
		CaptionCheckedGradientMid	= Color(255, 255, 255);
		CaptionCheckedGradientEnd	= Color(47, 47, 47);

		ContentGradientBegin		= Color(240, 241, 242);
		ContentGradientEnd			= Color(240, 241, 242);			
		FlatCaptionGradientBegin	= Color(90, 90, 90);
		FlatCaptionGradientEnd		= Color(155, 155, 155);
	};

	void InitColor_Default ()
	{
		BackColor					= Color::White;
		BorderColor					= Color(42, 77, 171);
		
		CaptionGradientBegin		= Color(148, 197, 239);
		CaptionGradientMiddle		= Color(103, 157, 222);
		CaptionGradientEnd			= Color(57, 127, 209);

		CaptionSelectedGradientBegin= Color(203, 254, 255);
		CaptionSelectedGradientMid	= Color(120, 184, 230);
		CaptionSelectedGradientEnd	= Color(142, 199, 234);		

		CaptionCheckedGradientBegin	= Color(255, 144, 147);
		CaptionCheckedGradientMid	= Color(162, 74, 70);
		CaptionCheckedGradientEnd	= Color(232, 35, 30);

		CaptionPressedGradientBegin	= Color(106, 109, 228);
		CaptionPressedGradientMid	= Color(39, 39, 217);
		CaptionPressedGradientEnd	= Color(88, 111, 226);		

		CaptionSelectedText			= Color::Yellow;
		CaptionText					= Color::White;		
		FlatCaptionGradientBegin	= Color(248, 248, 248);
		FlatCaptionGradientEnd		= Color(252, 252, 252);
		InnerBorderColor			= Color::White;
	};

	void InitColor_Red ()
	{
		BackColor					= Color(230,  51, 51);//Color::Transparent;
		BorderColor					= Color(255, 150, 150);
		
		CaptionCheckedGradientBegin = Color(242, 126, 120);
		CaptionCheckedGradientEnd	= Color(169, 85, 83);
		CaptionCheckedGradientMid	= Color(206, 103, 108);
		
		CaptionGradientBegin		= Color(247,  69, 62); //(255, 12, 11);
		CaptionGradientMiddle		= Color(173,  11, 17); //(210, 32, 17);
		CaptionGradientEnd			= Color(230,  51, 51); //(163, 48, 20);
		
		CaptionPressedGradientBegin	= Color(255, 114, 107);
		CaptionPressedGradientEnd	= Color(212, 5, 0);
		CaptionPressedGradientMid	= Color(122, 14, 0);
		
		CaptionSelectedGradientBegin= Color(212, 143, 140);
		CaptionSelectedGradientEnd	= Color(192, 26, 21);
		CaptionSelectedGradientMid	= Color(104, 31, 17);

		CaptionSelectedText			= Color(246, 246, 247);
		CaptionText					= Color(246, 246, 247);
		
		FlatCaptionGradientBegin	= Color(248, 248, 248);
		FlatCaptionGradientEnd		= Color(252, 252, 252);
		InnerBorderColor			= Color::White;
	};

	void InitColor_Yellow ()
	{
		BackColor					= Color(235, 199, 15);//Color::White;
		BorderColor					= Color(255, 255, 200);

		CaptionCheckedGradientBegin	= Color(253, 208, 51);
		CaptionCheckedGradientEnd	= Color(239, 188, 0);
		CaptionCheckedGradientMid	= Color(255, 199, 48);

		CaptionGradientBegin		= Color(255, 215, 0);
		CaptionGradientMiddle		= Color(218, 165, 32);
		CaptionGradientEnd			= Color(235, 199, 15);

		CaptionPressedGradientBegin	= Color(123, 209, 217);
		CaptionPressedGradientEnd	= Color(18, 185, 196);
		CaptionPressedGradientMid	= Color(0, 162, 172);

		CaptionSelectedGradientBegin= Color(144, 175, 180);
		CaptionSelectedGradientEnd	= Color(10, 83, 89);
		CaptionSelectedGradientMid	= Color(10, 83, 89);

		CaptionSelectedText			= Color::Black;
		CaptionText					= Color::Black;
		FlatCaptionGradientBegin	= Color(248, 248, 248);
		FlatCaptionGradientEnd		= Color(252, 252, 252);
		InnerBorderColor			= Color::White;		
	};

	void InitColor_Green ()
	{
		BackColor					= Color(125, 183, 91);//Color::White;
		BorderColor					= Color(140, 255, 165);

		CaptionCheckedGradientBegin	= Color(139, 200, 205);
		CaptionCheckedGradientEnd	= Color(7, 123, 155);
		CaptionCheckedGradientMid	= Color(14, 137, 145);

		CaptionGradientBegin		= Color(141, 193, 105);
		CaptionGradientMiddle		= Color(39, 160, 39);
		CaptionGradientEnd			= Color(125, 183, 91);
		
		CaptionPressedGradientBegin	= Color(123, 209, 217);
		CaptionPressedGradientEnd	= Color(18, 185, 196);
		CaptionPressedGradientMid	= Color(0, 162, 172);

		CaptionSelectedGradientBegin= Color(144, 175, 180);
		CaptionSelectedGradientEnd	= Color(10, 83, 89);
		CaptionSelectedGradientMid	= Color(10, 83, 89);

		CaptionSelectedText			= Color::White;
		CaptionText					= Color::White;
		FlatCaptionGradientBegin	= Color(248, 248, 248);
		FlatCaptionGradientEnd		= Color(252, 252, 252);
		InnerBorderColor			= Color::White;		
	};
	// MIS / 21.1010
	void InitColor_SlateGray()
	{
		BackColor = Color(112, 128, 144);
		BorderColor = Color(112, 128, 144); 
		InnerBorderColor = Color(112, 128, 144);
		CaptionText = Color::White;
		CaptionSelectedText = Color::White;
		CollapsedCaptionText = Color(0, 0, 0);

		CaptionGradientBegin = Color(109, 109, 109); 
		CaptionGradientMiddle = Color(20, 20, 20);	
		CaptionGradientEnd = Color(80, 80, 80);	

		CaptionCheckedGradientBegin = Color(155, 155, 155);
		CaptionCheckedGradientMid = Color(0, 0, 0);
		CaptionCheckedGradientEnd = Color(47, 47, 47);

		ContentGradientBegin = Color(240, 241, 242);
		ContentGradientEnd = Color(240, 241, 242);
		FlatCaptionGradientBegin = Color(90, 90, 90);
		FlatCaptionGradientEnd = Color(155, 155, 155);
	};

	void InitColor_DarkGray ()
	{
		// Black
		BackColor					= Color( 80,  80,  80);//Color(240, 241, 242);
		BorderColor					= Color(0, 0, 0); //Color(30, 30, 30);
		InnerBorderColor			= Color(185, 185, 185);
		CaptionText					= Color::White;
		CaptionSelectedText			= Color::White;
		CollapsedCaptionText		= Color(0, 0, 0);

		CaptionGradientBegin		= Color(109, 109, 109); //Color(169, 169, 169);
		CaptionGradientMiddle		= Color( 20,  20,  20);	//Color(80, 80, 80);
		CaptionGradientEnd			= Color( 80,  80,  80);	//Color(140, 140, 140);

		CaptionCheckedGradientBegin	= Color(155, 155, 155);
		CaptionCheckedGradientMid	= Color(0, 0, 0);
		CaptionCheckedGradientEnd	= Color(47, 47, 47);
		
		ContentGradientBegin		= Color(240, 241, 242);
		ContentGradientEnd			= Color(240, 241, 242);			
		FlatCaptionGradientBegin	= Color(90, 90, 90);
		FlatCaptionGradientEnd		= Color(155, 155, 155);
	};

	void InitColor_DeepDarkGray()
	{
		// Black
		BackColor					= Color(50, 50, 50);
		BorderColor					= Color(0, 0, 0); //Color(30, 30, 30);
		InnerBorderColor			= Color(185, 185, 185);
		CaptionText					= Color::White;
		CaptionSelectedText			= Color::White;
		CollapsedCaptionText		= Color(0, 0, 0);

		CaptionGradientBegin		= Color(109, 109, 109);
		CaptionGradientMiddle		= Color(20, 20, 20);
		CaptionGradientEnd			= Color(80, 80, 80);

		CaptionCheckedGradientBegin = Color(155, 155, 155);
		CaptionCheckedGradientMid	= Color(0, 0, 0);
		CaptionCheckedGradientEnd	= Color(47, 47, 47);

		ContentGradientBegin		= Color(240, 241, 242);
		ContentGradientEnd			= Color(240, 241, 242);
		FlatCaptionGradientBegin	= Color(90, 90, 90);
		FlatCaptionGradientEnd		= Color(155, 155, 155);
	};

	void InitColor_Navy ()
	{
		BackColor					= Color(90, 128, 203);//Color(240, 241, 242);
		BorderColor					= Color(100, 154, 255);

		CaptionCheckedGradientBegin	= Color(136, 144, 254);
		CaptionCheckedGradientEnd	= Color(111, 145, 255);
		CaptionCheckedGradientMid	= Color(42, 52, 254);

		CaptionGradientBegin		= Color(102, 142, 213);
		CaptionGradientMiddle		= Color(51, 102, 191);
		CaptionGradientEnd			= Color(90, 128, 203);

		CaptionPressedGradientBegin	= Color(106, 109, 228);
		CaptionPressedGradientEnd	= Color(88, 111, 226);
		CaptionPressedGradientMid	= Color(39, 39, 217);
		CaptionSelectedGradientBegin= Color(156, 163, 254);
		CaptionSelectedGradientEnd	= Color(139, 164, 255);
		CaptionSelectedGradientMid	= Color(90, 98, 254);
		CaptionSelectedText			= Color::White;
		CaptionText					= Color::White;
		FlatCaptionGradientBegin	= Color(248, 248, 248);
		FlatCaptionGradientEnd		= Color(252, 252, 252);
		InnerBorderColor			= Color::White;		
	};

	void InitColor_Violet ()
	{
		BackColor					= Color(218, 128, 203);//Color(240, 241, 242);
		BorderColor					= Color(248, 170, 248);

		CaptionGradientBegin		= Color(238, 130, 238);
		CaptionGradientMiddle		= Color(148, 0, 211);
		CaptionGradientEnd			= Color(218, 128, 203);

		CaptionText					= Color::White;
		FlatCaptionGradientBegin	= Color(248, 248, 248);
		FlatCaptionGradientEnd		= Color(252, 252, 252);
		InnerBorderColor			= Color::White;		
	};

}PanelColors;

typedef struct _tagColorTable
{
	Color ButtonPressedBorder;
	Color ButtonPressedGradientBegin;
	Color ButtonPressedGradientEnd;
	Color ButtonPressedGradientMiddle;

	Color ButtonSelectedGradientBegin;
	Color ButtonSelectedGradientEnd;
	Color ButtonSelectedGradientMiddle;
	Color ButtonSelectedHighlightBorder;

	Color GripDark;
	Color GripLight;
	Color ImageMarginGradientBegin;
	Color MenuBorder;
	
	Color MenuItemSelectedGradientBegin;
	Color MenuItemSelectedGradientEnd;
	
	Color MenuItemText;
	Color MenuItemTopLevelSelectedBorder;
	Color MenuItemTopLevelSelectedGradientBegin;
	
	Color MenuStripGradientBegin;
	Color MenuStripGradientEnd;
	
	Color OverflowButtonGradientBegin;
	Color OverflowButtonGradientEnd;
	Color OverflowButtonGradientMiddle;
	
	Color RaftingContainerGradientBegin;
	Color RaftingContainerGradientEnd;
	
	Color SeparatorDark;
	Color SeparatorLight;
	
	Color StatusStripGradientBegin;
	Color StatusStripGradientEnd;
	
	Color StatusStripText;
	Color ToolStripBorder;
	
	Color ToolStripContentGradientBegin;
	Color ToolStripContentGradientEnd;
	
	Color ToolStripDropDownBackground;
	
	Color ToolStripGradientBegin;
	Color ToolStripGradientEnd;
	Color ToolStripGradientMiddle;
	
	Color ToolStripPanelGradientBegin;
	Color ToolStripPanelGradientEnd;
	Color ToolStripText;

	void InitBlack ()
	{
		ButtonPressedBorder				= Color(145, 153, 164);
		ButtonPressedGradientBegin		= Color(141, 170, 253);
		ButtonPressedGradientEnd		= Color(98, 101, 252);
		ButtonPressedGradientMiddle		= Color(43, 93, 255);

		ButtonSelectedGradientBegin		= Color(106, 109, 228);
		ButtonSelectedGradientEnd		= Color(88, 111, 226);
		ButtonSelectedGradientMiddle	= Color(39, 39, 217);
		ButtonSelectedHighlightBorder	= Color(145, 153, 164);

		GripDark						= Color(102, 102, 102);
		GripLight						= Color(182, 182, 182);
		ImageMarginGradientBegin		= Color(239, 239, 239);
		MenuBorder						= Color(0, 0, 0);
		
		MenuItemSelectedGradientBegin	= Color(231, 239, 243);
		MenuItemSelectedGradientEnd		= Color(218, 235, 243);
		
		MenuItemText					= Color(255, 255, 255);
		MenuItemTopLevelSelectedBorder	= Color(145, 153, 164);
		MenuItemTopLevelSelectedGradientBegin= Color(205, 208, 213);
		
		MenuStripGradientBegin			= Color(102, 102, 102);
		MenuStripGradientEnd			= Color(0, 0, 0);
		
		OverflowButtonGradientBegin		= Color(136, 144, 254);
		OverflowButtonGradientEnd		= Color(111, 145, 255);
		OverflowButtonGradientMiddle	= Color(42, 52, 254);
		
		RaftingContainerGradientBegin	= Color(83, 83, 83);
		RaftingContainerGradientEnd		= Color(83, 83, 83);
		
		SeparatorDark					= Color(102, 102, 102);
		SeparatorLight					= Color(182, 182, 182);
		
		StatusStripGradientBegin		= Color(100, 100, 100);
		StatusStripGradientEnd			= Color(0, 0, 0);
		
		StatusStripText					= Color(255, 255, 255);
		ToolStripBorder					= Color(102, 102, 102);
		
		ToolStripContentGradientBegin	= Color(42, 42, 42);
		ToolStripContentGradientEnd		= Color(10, 10, 10);
		
		ToolStripDropDownBackground		= Color(250, 250, 250);
		
		ToolStripGradientBegin			= Color(102, 102, 102);
		ToolStripGradientEnd			= Color(0, 0, 0);
		ToolStripGradientMiddle			= Color(52, 52, 52);
		
		ToolStripPanelGradientBegin		= Color(12, 12, 12);
		ToolStripPanelGradientEnd		= Color(2, 2, 2);
		ToolStripText					= Color(255, 255, 255);
	};

	void InitBlue ()
	{
		ButtonPressedBorder				= Color(145, 153, 164);
		ButtonPressedGradientBegin		= Color(141, 170, 253);
		ButtonPressedGradientEnd		= Color(98, 101, 252);
		ButtonPressedGradientMiddle		= Color(43, 93, 255);
		ButtonSelectedGradientBegin		= Color(188, 205, 254);
		ButtonSelectedGradientEnd		= Color(153, 155, 253);
		ButtonSelectedGradientMiddle	= Color(111, 145, 255);
		ButtonSelectedHighlightBorder	= Color(145, 153, 164);
		GripDark						= Color(102, 102, 102);
		GripLight						= Color(182, 182, 182);
		ImageMarginGradientBegin		= Color(239, 239, 239);
		MenuBorder						= Color(0, 0, 0);
		MenuItemSelectedGradientBegin	= Color(231, 239, 243);
		MenuItemSelectedGradientEnd		= Color(218, 235, 243);
		MenuItemText					= Color(255, 255, 255);
		MenuItemTopLevelSelectedBorder	= Color(145, 153, 164);
		MenuItemTopLevelSelectedGradientBegin= Color(205, 208, 213);
		MenuStripGradientBegin			= Color(128, 128, 255);
		MenuStripGradientEnd			= Color(0, 0, 128);
		OverflowButtonGradientBegin		= Color(166, 189, 254);
		OverflowButtonGradientEnd		= Color(119, 123, 253);
		OverflowButtonGradientMiddle	= Color(63, 108, 253);
		RaftingContainerGradientBegin	= Color(83, 83, 83);
		RaftingContainerGradientEnd		= Color(83, 83, 83);
		SeparatorDark					= Color(102, 102, 102);
		SeparatorLight					= Color(182, 182, 182);
		StatusStripGradientBegin		= Color(128, 128, 255);
		StatusStripGradientEnd			= Color(0, 0, 128);
		StatusStripText					= Color(255, 255, 255);
		ToolStripBorder					= Color(102, 102, 102);
		ToolStripContentGradientBegin	= Color(0, 0, 139);
		ToolStripContentGradientEnd		= Color(0, 0, 128);
		ToolStripDropDownBackground		= Color(250, 250, 250);
		ToolStripGradientBegin			= Color(128, 128, 255);
		ToolStripGradientEnd			= Color(0, 0, 139);
		ToolStripGradientMiddle			= Color(0, 0, 128);
		ToolStripPanelGradientBegin		= Color(0, 0, 128);
		ToolStripPanelGradientEnd		= Color(0, 0, 128);
		ToolStripText					= Color(255, 255, 255);
	};

}ColorTable;

#endif // Define_ColorTable_h__
