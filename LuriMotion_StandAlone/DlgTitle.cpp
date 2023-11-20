// DlgTitle.cpp: 구현 파일
//

#include "pch.h"
#include "LuriMotion_OQA_SFR_Wide.h"
#include "DlgTitle.h"
#include "afxdialogex.h"
#include "LuriMotion_OQA_SFR_WideDlg.h"

#include "CommonFunction.h"

// CDlgTitle 대화 상자

IMPLEMENT_DYNAMIC(CDlgTitle, CDialogEx)

CDlgTitle::CDlgTitle(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_TITLE, pParent)
{
	m_strTimer = _T("");
	m_strDate = _T("");

	for (int i = 0; i < 3; i++)
	{
		m_bBlinkBk[i] = 3;
		m_bBlinkMode[i] = FALSE;
		m_bBlinkState[i] = FALSE;
	}
	
	//m_Mode_Old = (RUN_MODE)eError;
	m_Mode_Old = (RUN_MODE)eNone;

	m_bProgramStartFlag = TRUE;
	m_bRunTitle = FALSE;
}

CDlgTitle::~CDlgTitle()
{
}

void CDlgTitle::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	
	DDX_Control(pDX, IDC_BTN_TITLE_1, m_btnTitleToolbar[0]);
	DDX_Control(pDX, IDC_BTN_TITLE_2, m_btnTitleToolbar[1]);
	DDX_Control(pDX, IDC_BTN_TITLE_3, m_btnTitleToolbar[2]);
	DDX_Control(pDX, IDC_BTN_TITLE_4, m_btnTitleToolbar[3]);

	DDX_Control(pDX, IDC_STC_TITLE_MCH_NAME, m_stTitleMchName);
	DDX_Control(pDX, IDC_STC_TITLE_MCH_NAME2, m_stTitleMchName_Recipe);
	

	DDX_Control(pDX, IDC_STC_TITLE_SUB_1, m_stTitleView[0]);
	DDX_Control(pDX, IDC_STC_TITLE_SUB_2, m_stTitleView[1]);
	DDX_Control(pDX, IDC_STC_TITLE_SUB_3, m_stTitleView[2]);
	DDX_Control(pDX, IDC_STC_TITLE_SUB_4, m_stTitleView[3]);
	DDX_Control(pDX, IDC_STC_TITLE_SUB_5, m_stTitleView[4]);
	DDX_Control(pDX, IDC_STC_TITLE_SUB_6, m_stTitleView[5]);

}


BEGIN_MESSAGE_MAP(CDlgTitle, CDialogEx)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_TITLE_1, &CDlgTitle::OnBnClickedBtnTitle1)
	ON_BN_CLICKED(IDC_BTN_TITLE_2, &CDlgTitle::OnBnClickedBtnTitle2)
	ON_BN_CLICKED(IDC_BTN_TITLE_3, &CDlgTitle::OnBnClickedBtnTitle3)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

BOOL CDlgTitle::PreTranslateMessage(MSG* pMsg)
{
	//ALT + F4 차단
	if (pMsg->message == WM_SYSKEYDOWN && pMsg->wParam == VK_F4)
	{
		return TRUE;
	}

	//ESC 차단
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		return TRUE;
	}

	//Enter 차단
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
// CDlgTitle 메시지 처리기
void CDlgTitle::OnLButtonDown(UINT nFlags, CPoint point)
{
	// [21.1021.2]
	CLuriMotion_OQA_SFR_WideDlg *pParrentDlg = (CLuriMotion_OQA_SFR_WideDlg*)AfxGetApp()->GetMainWnd();
	pParrentDlg->SendMessage(MESSAGE_UI_WINDOW_TOP, NULL, NULL);

	CDialogEx::OnLButtonDown(nFlags, point);
}

BOOL CDlgTitle::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(135, 206, 235));		// SkyBlue
	//pDC->FillSolidRect(rect, RGB(112, 146, 190));

	return TRUE;
}



int CDlgTitle::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	SetTimer(100, 1000, NULL);			// 시간 TImer
	SetTimer(101, 5000, NULL);			// 날짜 Timer
	
	SetTimer(200, 250, NULL);			// 타워램프 Timer
	SetTimer(201, 500, NULL);			// 타워램프 Timer Blink

	return 0;
}
void CDlgTitle::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (bShow)
	{
		m_t = CTime::GetCurrentTime();
		m_strTimer.Format(_T("%02dH %02dM %02dS"), m_t.GetHour(), m_t.GetMinute(), m_t.GetSecond());
		m_stTitleView[0].SetText(m_strTimer);
		
		m_strDate.Format(_T("%04dY %02dM %02dD"), m_t.GetYear(), m_t.GetMonth(), m_t.GetDay());
		m_stTitleView[1].SetText(m_strDate);
		// [21.1017.8] Modified
		m_stTitleView[3].SetText(COMMON->GetPermissionId());
		m_stTitleView[4].SetText(_T("-----"));
	}
	
}
BOOL CDlgTitle::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitComponent_Title();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgTitle::InitComponent_Title()
{
	
	CRect rt_Window;
	CRect rt;
	CRect rt_Move;

	GetClientRect(rt_Window);

	// 장비 이름을 기준으로 좌우로 나누자 
	// 우선은 장비 이름부터.
	// TITLE DLG의 x,y 길이는 1920,150 이다.이유는 전체화면으로 할떄..
	m_stTitleMchName.GetWindowRect(rt);
	int nCenterX = 1920 / 2;	
	int nCenterY = 150 / 2;

	int nTitle_Width = 400;
	int nTitle_Height = rt_Window.Height() / 2;

	rt_Move.left = nCenterX - nTitle_Width / 2;
	rt_Move.top = nCenterY - (nTitle_Height / 2) - 15;
	rt_Move.right = nCenterX + nTitle_Width / 2;
	rt_Move.bottom = nCenterY + (nTitle_Height / 2) - 15;

	m_stTitleMchName.MoveWindow(rt_Move, SWP_SHOWWINDOW);

	rt_Move.left = rt_Move.left;
	rt_Move.top = rt_Move.bottom + 5;
	rt_Move.right = rt_Move.right;
	rt_Move.bottom = rt_Move.top + 30 + 5;
	//rt_Move.right = rt_Move.left;
	//rt_Move.bottom = rt_Move.top;
	m_stTitleMchName_Recipe.MoveWindow(rt_Move, SWP_SHOWWINDOW);

	// 왼쪽 버튼 4개
	int nStart_X = 50;
	int nStart_Y = 25;

	int nTitleBtn_Width = 150;
	int nTitleBtn_Height = 100;
	int nGap = 15;

	for (int i = 0; i < 4; i++)
	{
		rt_Move.left = nStart_X + (nTitleBtn_Width* i) + (nGap * i);
		rt_Move.top = nStart_Y;
		// MIS / 21.1005
		if (i == 0 || i == 1 /*|| i == 2*/)
		{
			rt_Move.right = rt_Move.left + (nTitleBtn_Width);
			rt_Move.bottom = nStart_Y + nTitleBtn_Height;
		}
		else
		{
			rt_Move.right = rt_Move.left;
			rt_Move.bottom = rt_Move.top;
		}
		m_btnTitleToolbar[i].MoveWindow(rt_Move, SWP_SHOWWINDOW);
	}

	// 우측 Static 2개 시간/날짜

	int nStart_Time_X = 1225;
	int nStart_Time_Y = 25;

	int nWidth_Time = 150;
	int nHeith_Time = 50;

	int nPermissionStatic = 100;

	// 시간
	rt_Move.left = nStart_Time_X;
	rt_Move.top = nStart_Time_Y - 1;
	rt_Move.right = nStart_Time_X+ nWidth_Time;
	rt_Move.bottom = rt_Move.top+ nHeith_Time;
	m_stTitleView[0].MoveWindow(rt_Move, SWP_SHOWWINDOW);

	// 날짜
	rt_Move.left = nStart_Time_X;
	rt_Move.top = nStart_Time_Y + nHeith_Time + 1;
	rt_Move.right = nStart_Time_X + nWidth_Time;
	rt_Move.bottom = rt_Move.top + nHeith_Time;
	m_stTitleView[1].MoveWindow(rt_Move, SWP_SHOWWINDOW);

	// 권한 Static
	rt_Move.left = nStart_Time_X + nWidth_Time + 90;
	rt_Move.top = nStart_Time_Y - 1;
	rt_Move.right = nStart_Time_X + nWidth_Time + nPermissionStatic + 90;
	rt_Move.bottom = rt_Move.top + (nHeith_Time*2) + 2;
	m_stTitleView[2].MoveWindow(rt_Move, SWP_SHOWWINDOW);

	// 권한 이름
	rt_Move.left = nStart_Time_X + nWidth_Time + nPermissionStatic + 90 + 3;
	rt_Move.top = nStart_Time_Y - 1;
	rt_Move.right = rt_Move.left + nWidth_Time;
	rt_Move.bottom = rt_Move.top + nHeith_Time;
	m_stTitleView[3].MoveWindow(rt_Move, SWP_SHOWWINDOW);

	// 권한 level
	rt_Move.left = nStart_Time_X + nWidth_Time + nPermissionStatic + 90 + 3;
	rt_Move.top = nStart_Time_Y + nHeith_Time + 1;
	rt_Move.right = rt_Move.left + nWidth_Time;
	rt_Move.bottom = rt_Move.top + nHeith_Time;
	m_stTitleView[4].MoveWindow(rt_Move, SWP_SHOWWINDOW);

	// TowerLamp 색상
	rt_Move.left = nStart_Time_X + nWidth_Time + nPermissionStatic + 90 + 3 + nWidth_Time + 70;
	rt_Move.top = nStart_Time_Y -1 ;
	rt_Move.right = rt_Move.left + 90;
	rt_Move.bottom = rt_Move.top + (nHeith_Time*2) + 2;
	m_stTitleView[5].MoveWindow(rt_Move, SWP_SHOWWINDOW);

	EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();

	m_stTitleMchName.SetStaticStyle(CVGStatic::StaticStyle_GroupHeader);
	m_stTitleMchName.SetColorStyle(CVGStatic::ColorStyle_Black);
	m_stTitleMchName.SetFont_Gdip(L"Gothic", 18.0F);

	CString titleMachineName = _T("");

	titleMachineName = g_szInsptrSysType[GET_INSPECTOR];

#ifdef _DRY_RUN_MODULE_NONE
	titleMachineName += _T(" {DRY RUN}");
#endif // _DRY_RUN_MODULE_NONE
	m_stTitleMchName.SetText(titleMachineName);

	m_stTitleMchName_Recipe.SetStaticStyle(CVGStatic::StaticStyle_Default);
	m_stTitleMchName_Recipe.SetColorStyle(CVGStatic::ColorStyle_SkyBlue);
	m_stTitleMchName_Recipe.SetFont_Gdip(L"Arial", 12.0F);
	m_stTitleMchName_Recipe.SetText(_T("Recipe Name"));	

	// UI 입히기
	for (int i = 0; i < 4; i++)
	{
		m_btnTitleToolbar[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Alt);
		m_btnTitleToolbar[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_btnTitleToolbar[i].SetFont_Gdip(L"MS Ghthic", 12.0F);

		if (stBasic.nLanguage == 0 )
			m_btnTitleToolbar[i].SetText(m_szTitleMenuKor[i]);
		else
			m_btnTitleToolbar[i].SetText(m_szTitleMenuEng[i]);
	}

	for (int i = 0; i < 6; i++)
	{
		m_stTitleView[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Alt);
		m_stTitleView[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_stTitleView[i].SetFont_Gdip(L"Arial", 11.0F);
		if( i == 5) m_stTitleView[i].SetColorStyle(CVGStatic::ColorStyle_Yellow);
		if (i == 2) 
		{
			if (stBasic.nLanguage == 0)
				m_stTitleView[2].SetText(_T("계정\n\n권한"));
			else
				m_stTitleView[2].SetText(_T("Account\n\nPermission"));
		}
			
		else
			m_stTitleView[i].SetText(_T("."));
	}
}


void CDlgTitle::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnTimer(nIDEvent);

	switch (nIDEvent)
	{
	case 100:
	{
		m_t = CTime::GetCurrentTime();
		m_strTimer.Format(_T("%02d : %02d : %02d"), m_t.GetHour(), m_t.GetMinute(), m_t.GetSecond());

		m_stTitleView[0].SetText(m_strTimer);

		if (m_bProgramStartFlag == TRUE)
		{
			EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();
			// [21.1021.9] S/W VER
			//m_stTitleMchName_Recipe.SetText(stEq.strLastRecipe);
			//m_stTitleMchName_Recipe.SetText(SW_VERSION);
			//m_bProgramStartFlag = FALSE;

			CString szVer;
			szVer.Format(_T("[S/W Ver1] %s.%s"), GetVersionInfo(_T("ProductVersion")), GetVersionInfo(_T("FileVersion")));
			m_stTitleMchName_Recipe.SetText(szVer);
			m_bProgramStartFlag = FALSE;
		}

		if (COMMON->m_bRecipeChange == TRUE)
		{
//			m_stTitleMchName_Recipe.SetText(DATAMANAGER->m_strCurrenctRecipeName);
//			COMMON->m_bRecipeChange = FALSE;
		}

	}
	break;

	case 101:
	{
		m_t = CTime::GetCurrentTime();
		m_strDate.Format(_T("%04d / %02d / %02d"), m_t.GetYear(), m_t.GetMonth(), m_t.GetDay());

		m_stTitleView[1].SetText(m_strDate);
	}
	break;

	case 200:
	{
		EQ_TOWERLAMP	stTower = *DATAMANAGER->GetCurrentTowerLampData();
		RUN_MODE rm = SEQUENCE->GetRunMode();

		if (SEQUENCE->GetRunMode() == (RUN_MODE)eHome && m_Mode_Old != SEQUENCE->GetRunMode())
		{
			m_Mode_Old = SEQUENCE->GetRunMode();

			for (int i = 0; i < 3; i++)
			{
				m_bBlinkMode[i] = stTower.nTowerLampSetting[ETwrMode::eModeReady][i];
			}
		}
		// [21.1214.1]
		else if (SEQUENCE->GetRunMode() == (RUN_MODE)eRunning || SEQUENCE->GetRunMode() == (RUN_MODE)eStandby)
		{
			m_Mode_Old = SEQUENCE->GetRunMode();

			for (int i = 0; i < 3; i++)
			{
				m_bBlinkMode[i] = stTower.nTowerLampSetting[ETwrMode::eModeRun][i];
			}
		}
		else if (SEQUENCE->GetRunMode() == (RUN_MODE)eStop && m_Mode_Old != SEQUENCE->GetRunMode())
		{
			m_Mode_Old = SEQUENCE->GetRunMode();


			for (int i = 0; i < 3; i++)
			{
				m_bBlinkMode[i] = stTower.nTowerLampSetting[ETwrMode::eModeStop][i];
			}
		}
		else if (SEQUENCE->GetRunMode() == (RUN_MODE)eError && m_Mode_Old != SEQUENCE->GetRunMode())
		{
			m_Mode_Old = SEQUENCE->GetRunMode();


			for (int i = 0; i < 3; i++)
			{
				m_bBlinkMode[i] = stTower.nTowerLampSetting[ETwrMode::eModeError][i];
			}
		}
		else if (SEQUENCE->GetRunMode() == (RUN_MODE)ePause && m_Mode_Old != SEQUENCE->GetRunMode())
		{
			m_Mode_Old = SEQUENCE->GetRunMode();


			for (int i = 0; i < 3; i++)
			{
				m_bBlinkMode[i] = stTower.nTowerLampSetting[ETwrMode::eModePause][i];
			}
		}
		// [21.1117.1] All lamp is off
		bool b1, b2, b3;
		b1 = (m_bBlinkMode[eTowerRed] == eTowerLamp_Off ? true : false);
		b2 = (m_bBlinkMode[eTowerYellow] == eTowerLamp_Off ? true : false);
		b3 = (m_bBlinkMode[eTowerGreen] == eTowerLamp_Off ? true : false);

		if (b1 && b2 && b3)
		{
			COMMON->Write_Comm_Log(_T("All Twr lamp off\n"));
			TRACE(_T("All Twr lamp off\n"));
		}
	}
	break;

	case 201:
	{
		// Twr Red
		if (m_bBlinkMode[eTowerRed] == eTowerLamp_Off)
		{
			if (m_bBlinkBk[eTowerRed] != m_bBlinkMode[eTowerRed])
			{
				m_bBlinkBk[eTowerRed] = m_bBlinkMode[eTowerRed];
				AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_06_TOWERLAMP_RED, FALSE);
			}
		}
		else if (m_bBlinkMode[eTowerRed] == eTowerLamp_On)
		{
			if (m_bBlinkBk[eTowerRed] != m_bBlinkMode[eTowerRed])
			{
				m_bBlinkBk[eTowerRed] = m_bBlinkMode[eTowerRed];
				m_stTitleView[5].SetColorStyle(CVGStatic::ColorStyle_Red);
				AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_06_TOWERLAMP_RED, TRUE);
			}
		}
		else if (m_bBlinkMode[eTowerRed] == eTowerLamp_Blink)
		{
			m_bBlinkBk[eTowerRed] = m_bBlinkMode[eTowerRed];

			if (m_bBlinkState[eTowerRed] == FALSE)
			{
				AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_06_TOWERLAMP_RED, TRUE);
				m_stTitleView[5].SetColorStyle(CVGStatic::ColorStyle_Red);
				m_bBlinkState[eTowerRed] = TRUE;
			}
			else
			{
				AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_06_TOWERLAMP_RED, FALSE);
				m_stTitleView[5].SetColorStyle(CVGStatic::ColorStyle_White);
				m_bBlinkState[eTowerRed] = FALSE;
			}
		}
		// Twr Yellow
		if (m_bBlinkMode[eTowerYellow] == eTowerLamp_Off)
		{
			if (m_bBlinkBk[eTowerYellow] != m_bBlinkMode[eTowerYellow])
			{
				m_bBlinkBk[eTowerYellow] = m_bBlinkMode[eTowerYellow];
				AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_07_TOWERLAMP_YELLOW, FALSE);
			}
		}
		else if (m_bBlinkMode[eTowerYellow] == eTowerLamp_On)
		{
			if (m_bBlinkBk[eTowerYellow] != m_bBlinkMode[eTowerYellow])
			{
				m_bBlinkBk[eTowerYellow] = m_bBlinkMode[eTowerYellow];
				m_stTitleView[5].SetColorStyle(CVGStatic::ColorStyle_Yellow);
				AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_07_TOWERLAMP_YELLOW, TRUE);
			}
		}
		else if (m_bBlinkMode[eTowerYellow] == eTowerLamp_Blink)
		{
			m_bBlinkBk[eTowerYellow] = m_bBlinkMode[eTowerYellow];

			if (m_bBlinkState[eTowerYellow] == FALSE)
			{
				AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_07_TOWERLAMP_YELLOW, TRUE);
				m_stTitleView[5].SetColorStyle(CVGStatic::ColorStyle_Yellow);
				m_bBlinkState[eTowerYellow] = TRUE;
			}
			else
			{
				AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_07_TOWERLAMP_YELLOW, FALSE);
				m_stTitleView[5].SetColorStyle(CVGStatic::ColorStyle_White);
				m_bBlinkState[eTowerYellow] = FALSE;
			}
		}
		// Twr Green
		if (m_bBlinkMode[eTowerGreen] == eTowerLamp_Off)
		{
			if (m_bBlinkBk[eTowerGreen] != m_bBlinkMode[eTowerGreen])
			{
				m_bBlinkBk[eTowerGreen] = m_bBlinkMode[eTowerGreen];
				AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_08_TOWERLAMP_GREEN, FALSE);
			}
		}
		else if (m_bBlinkMode[eTowerGreen] == eTowerLamp_On)
		{
			if (m_bBlinkBk[eTowerGreen] != m_bBlinkMode[eTowerGreen])
			{
				m_bBlinkBk[eTowerGreen] = m_bBlinkMode[eTowerGreen];
				m_stTitleView[5].SetColorStyle(CVGStatic::ColorStyle_Green);
				AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_08_TOWERLAMP_GREEN, TRUE);
			}
		}
		else if (m_bBlinkMode[eTowerGreen] == eTowerLamp_Blink)
		{
			if (m_bBlinkState[eTowerGreen] == FALSE)
			{
				AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_08_TOWERLAMP_GREEN, TRUE);
				m_stTitleView[5].SetColorStyle(CVGStatic::ColorStyle_Green);
				m_bBlinkState[eTowerGreen] = TRUE;
			}
			else
			{
				AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_08_TOWERLAMP_GREEN, FALSE);
				m_stTitleView[5].SetColorStyle(CVGStatic::ColorStyle_White);
				m_bBlinkState[eTowerGreen] = FALSE;
			}
		}
	}
	break;

	}
}


void CDlgTitle::OnBnClickedBtnTitle1()
{
	if (m_bRunTitle == TRUE)
		return;

	m_bRunTitle = TRUE;

	EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();

	m_btnTitleToolbar[0].SetStaticStyle(CVGStatic::StaticStyle_Title_Alt);
	m_btnTitleToolbar[0].SetColorStyle(CVGStatic::ColorStyle_Green);
	m_btnTitleToolbar[0].SetFont_Gdip(L"MS Ghthic", 12.0F);

	if( stBasic.nLanguage == 0 )
		m_btnTitleToolbar[0].SetText(m_szTitleMenuKor[0]);
	else
		m_btnTitleToolbar[0].SetText(m_szTitleMenuEng[0]);

	CDlgLanguage Dlg;
	
	if (Dlg.DoModal() == IDOK)
	{
		EQ_BASIC_PARAM	stBasic_Change = *DATAMANAGER->GetCurrentEqBasicData();

		m_btnTitleToolbar[0].SetStaticStyle(CVGStatic::StaticStyle_Title_Alt);
		m_btnTitleToolbar[0].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_btnTitleToolbar[0].SetFont_Gdip(L"MS Ghthic", 12.0F);

		if (stBasic_Change.nLanguage == 0)
			m_btnTitleToolbar[0].SetText(m_szTitleMenuKor[0]);
		else
			m_btnTitleToolbar[0].SetText(m_szTitleMenuEng[0]);

		if (Dlg.GetChangeLanguage() != -1)
		{
			for (int i = 0; i < 4; i++)
			{
				m_btnTitleToolbar[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Alt);
				m_btnTitleToolbar[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
				m_btnTitleToolbar[i].SetFont_Gdip(L"MS Ghthic", 12.0F);

				if (stBasic_Change.nLanguage == 0)
					m_btnTitleToolbar[i].SetText(m_szTitleMenuKor[i]);
				else
					m_btnTitleToolbar[i].SetText(m_szTitleMenuEng[i]);
			}

			m_stTitleView[2].SetStaticStyle(CVGStatic::StaticStyle_Data);
			m_stTitleView[2].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
			m_stTitleView[2].SetFont_Gdip(L"Arial", 11.0F);

			if (stBasic_Change.nLanguage == 0)
				m_stTitleView[2].SetText(_T("계정\n\n권한"));
			else
				m_stTitleView[2].SetText(_T("Account\n\nPermission"));

			// 타이머는 돌고 있는 상태라서 UI 상으로는 바로 변경이 안되니 종료후 다시 시작하자.
			KillTimer(100);
			KillTimer(101);
			KillTimer(200);
			KillTimer(201);

			//for (int i = 0; i < 3; i++)
			//{
			//	m_bBlinkMode[i] = FALSE;
			//	m_bBlinkState[i] = FALSE;
			//}

			SetTimer(100, 1000, NULL);			// 시간 TImer
			SetTimer(101, 5000, NULL);			// 날짜 Timer
			SetTimer(200, 250, NULL);			// 타워램프 Timer
			SetTimer(201, 500, NULL);			// 타워램프 Timer Blink

			// 2. 전체 DLG의 UI를 변경해야 한다.
			CLuriMotion_OQA_SFR_WideDlg *pParrentDlg = (CLuriMotion_OQA_SFR_WideDlg*)AfxGetApp()->GetMainWnd();
			pParrentDlg->ChangeUi_ByLanguage();
		}
	}
	else
	{
		m_btnTitleToolbar[0].SetStaticStyle(CVGStatic::StaticStyle_Title_Alt);
		m_btnTitleToolbar[0].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_btnTitleToolbar[0].SetFont_Gdip(L"MS Ghthic", 12.0F);
	}
	m_bRunTitle = FALSE;

}
// [21.1017.9] Modified
void CDlgTitle::OnBnClickedBtnTitle2()
{
	if (m_bRunTitle == TRUE)
		return;

	m_bRunTitle = TRUE;

	int nMode = COMMON->GetPermissionGrade();

	m_btnTitleToolbar[1].SetStaticStyle(CVGStatic::StaticStyle_Title_Alt);
	m_btnTitleToolbar[1].SetColorStyle(CVGStatic::ColorStyle_Green);
	m_btnTitleToolbar[1].SetFont_Gdip(L"MS Ghthic", 12.0F);

	CDlgAdmin Dlg(nMode);

	if (Dlg.DoModal() == IDOK)
	{
		m_btnTitleToolbar[1].SetStaticStyle(CVGStatic::StaticStyle_Title_Alt);
		m_btnTitleToolbar[1].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_btnTitleToolbar[1].SetFont_Gdip(L"MS Ghthic", 12.0F);

		if (Dlg.GetChangFlag() == TRUE)
		{
			CString strTemp = _T("");
			if (COMMON->GetPermissionGrade() == LVL_ADMIN)
				strTemp = _T("ADMINISTRATOR");
			else if (COMMON->GetPermissionGrade() == LVL_MAINT)
				strTemp = _T("MAINTENANCE");
			else if (COMMON->GetPermissionGrade() == LVL_OPER)
				strTemp = _T("OPERATOR");

			m_stTitleView[3].SetText(COMMON->GetPermissionId());
			m_stTitleView[4].SetText(strTemp);

			CLuriMotion_OQA_SFR_WideDlg *pParrentDlg = (CLuriMotion_OQA_SFR_WideDlg*)AfxGetApp()->GetMainWnd();
			pParrentDlg->ChangeUi_ByLevel();
		}
	}
	else
	{
		m_btnTitleToolbar[1].SetStaticStyle(CVGStatic::StaticStyle_Title_Alt);
		m_btnTitleToolbar[1].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_btnTitleToolbar[1].SetFont_Gdip(L"MS Ghthic", 12.0F);
	}

	m_bRunTitle = FALSE;
}


void CDlgTitle::OnBnClickedBtnTitle3()
{
	if (m_bRunTitle == TRUE)
		return;

	m_bRunTitle = TRUE;

	EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();

	m_btnTitleToolbar[2].SetStaticStyle(CVGStatic::StaticStyle_Title_Alt);
	m_btnTitleToolbar[2].SetColorStyle(CVGStatic::ColorStyle_Green);
	m_btnTitleToolbar[2].SetFont_Gdip(L"MS Ghthic", 12.0F);;
	if (stBasic.nLanguage == 0)
		m_btnTitleToolbar[2].SetText(m_szTitleMenuKor[2]);
	else
		m_btnTitleToolbar[2].SetText(m_szTitleMenuEng[2]);


	CDlgTowerLamp Dlg(0);

	if (Dlg.DoModal() == IDOK)
	{
		m_btnTitleToolbar[2].SetStaticStyle(CVGStatic::StaticStyle_Title_Alt);
		m_btnTitleToolbar[2].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_btnTitleToolbar[2].SetFont_Gdip(L"MS Ghthic", 12.0F);;
		if (stBasic.nLanguage == 0)
			m_btnTitleToolbar[2].SetText(m_szTitleMenuKor[2]);
		else
			m_btnTitleToolbar[2].SetText(m_szTitleMenuEng[2]);
	}
	else
	{
		m_btnTitleToolbar[2].SetStaticStyle(CVGStatic::StaticStyle_Title_Alt);
		m_btnTitleToolbar[2].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_btnTitleToolbar[2].SetFont_Gdip(L"MS Ghthic", 12.0F);;
		if (stBasic.nLanguage == 0)
			m_btnTitleToolbar[2].SetText(m_szTitleMenuKor[2]);
		else
			m_btnTitleToolbar[2].SetText(m_szTitleMenuEng[2]);
	}

	m_bRunTitle = FALSE;
}


