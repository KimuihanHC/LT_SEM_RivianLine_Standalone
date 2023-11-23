// DlgSide.cpp: 구현 파일
//

#include "pch.h"
#include "LuriMotion_OQA_SFR_Wide.h"
#include "DlgSide.h"
#include "afxdialogex.h"
#include "DlgMain.h"
#include "DlgSleepMode.h"
#include "DlgLotoMode.h"

#include "LuriMotion_OQA_SFR_WideDlg.h"
#include "Def_Sequence.h"

#include "IO.h"
#include "Device.h"
// CDlgSide 대화 상자

IMPLEMENT_DYNAMIC(CDlgSide, CDialogEx)

CDlgSide::CDlgSide(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_SIDE, pParent)
{
	m_bFlagSampleInput_Block_Foced = FALSE;
	m_bFlagSampleOutput_Block_Foced = FALSE;
	m_bStartFlag = FALSE;
	m_bStopFlag = FALSE;
	m_bInitFlagIsActive = FALSE;
	m_bStartFlagIsActive = FALSE;
	m_bStartFlagIsActive = FALSE;
	m_bStartFlagIsActive = FALSE;
	COMMON->SetSleepModeStep(SLEEP_OFF);
	COMMON->bUseSleepMode = TRUE;
	
	m_MainAutoFlag = FALSE;
	m_MainManualFlag = FALSE;
	m_bFlagStartButtonLamp = FALSE;

	m_bChangeModeWaitTime = FALSE;

	// [21.1029.2] 
	m_bPowerStatus = FALSE;
	m_bEMOSignalCheck = FALSE;
}

CDlgSide::~CDlgSide()
{
}

void CDlgSide::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BTN_SIDE_COMMAND_1, m_btnSideMenu[0]);
	DDX_Control(pDX, IDC_BTN_SIDE_COMMAND_2, m_btnSideMenu[1]);
	DDX_Control(pDX, IDC_BTN_SIDE_COMMAND_3, m_btnSideMenu[2]);
	DDX_Control(pDX, IDC_BTN_SIDE_COMMAND_4, m_btnSideMenu[3]);
	DDX_Control(pDX, IDC_BTN_SIDE_COMMAND_5, m_btnSideMenu[4]);
	DDX_Control(pDX, IDC_BTN_SIDE_COMMAND_6, m_btnSideMenu[5]);
	DDX_Control(pDX, IDC_BTN_SIDE_COMMAND_7, m_btnSideMenu[6]);
	DDX_Control(pDX, IDC_BTN_SIDE_COMMAND_8, m_btnSideMenu[7]);	
}

BEGIN_MESSAGE_MAP(CDlgSide, CDialogEx)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BTN_SIDE_COMMAND_1, &CDlgSide::OnBnClickedBtnSideCommand1)
	ON_BN_CLICKED(IDC_BTN_SIDE_COMMAND_2, &CDlgSide::OnBnClickedBtnSideCommand2)
	ON_BN_CLICKED(IDC_BTN_SIDE_COMMAND_3, &CDlgSide::OnBnClickedBtnSideCommand3)
	ON_BN_CLICKED(IDC_BTN_SIDE_COMMAND_4, &CDlgSide::OnBnClickedBtnSideCommand4)
	ON_BN_CLICKED(IDC_BTN_SIDE_COMMAND_5, &CDlgSide::OnBnClickedBtnSideCommand5)
	ON_BN_CLICKED(IDC_BTN_SIDE_COMMAND_6, &CDlgSide::OnBnClickedBtnSideCommand6)
	ON_BN_CLICKED(IDC_BTN_SIDE_COMMAND_7, &CDlgSide::OnBnClickedBtnSideCommand7)
	ON_BN_CLICKED(IDC_BTN_SIDE_COMMAND_8, &CDlgSide::OnBnClickedBtnSideCommand8)
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

BOOL CDlgSide::PreTranslateMessage(MSG* pMsg)
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

void CDlgSide::OnLButtonDown(UINT nFlags, CPoint point)
{
	// [21.1021.5]
	CLuriMotion_OQA_SFR_WideDlg *pParrentDlg = (CLuriMotion_OQA_SFR_WideDlg*)AfxGetApp()->GetMainWnd();
	pParrentDlg->SendMessage(MESSAGE_UI_WINDOW_TOP, NULL, NULL);

	CDialogEx::OnLButtonDown(nFlags, point);
}

// CDlgSide 메시지 처리기
int CDlgSide::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	return 0;
}
void CDlgSide::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (bShow)
	{
		SetTimer(TIMER_100_SWITCH_LAMPCHECK, 200, NULL);
		SetTimer(TIMER_200_EMO_POWER_CHECK, 200, NULL);
		SetTimer(TIMER_300_CHANGE_MODE, 1000, NULL);
	}
	else
	{
//$		KillTimer(TIMER_SWITCH_LAMPCHECK);
	}

}
BOOL CDlgSide::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitComponent_Side();
	InitUI_EqpType();

	COMMON->SetEqStatus(eStop);

	SetSIdeBarStopButton_LampOn();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgSide::InitUI_EqpType()
{
	auto EqpType = GET_INSPECTOR;

	switch (EqpType)
	{
	case SYS_DISTORTION_30:
//		GetDlgItem(IDC_BTN_SIDE_COMMAND_8)->ShowWindow(SW_HIDE);
		break;

	case SYS_OQC_SFR_MULTI_CL:
		break;

	case SYS_OQC_SFR_SINGLE_CL:
		break;
	default:
		break;
	}
}

void CDlgSide::InitComponent_Side()
{
	int nGap = 10;
	CRect rt;
	CRect rt_Move;

	m_btnSideMenu[0].GetWindowRect(rt);

	int nStart_X = 20;
	int nStart_Y = 10;

	int nHeight = rt.Height();

	for (int i = 0; i < 8; i++)
	{
		rt_Move.left = nStart_X;
		rt_Move.top = nStart_Y + ((rt.Height() * 2) * i) + (nGap * i);
		rt_Move.right = nStart_X + rt.Width()+27;
		rt_Move.bottom = rt_Move.top + (rt.Height() * 2);

		m_btnSideMenu[i].MoveWindow(rt_Move, SWP_SHOWWINDOW);
	}

	EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();

	// [21.1018.10] Modified
	for (int i = 0; i < Menu_Max; i++)
	{
		m_btnSideMenu[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Alt);
		m_btnSideMenu[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_btnSideMenu[i].SetFont_Gdip(L"Arial", 9.0F);

		//if (i != 3)
		{
			if (stBasic.nLanguage == 0)
				m_btnSideMenu[i].SetText(m_szSideMenuKor[i]);
			else
				m_btnSideMenu[i].SetText(m_szSideMenuEng[i]);
		}
		/*else
		{
			if (stBasic.nLanguage == 0)
				m_btnSideMenu[i].SetText(m_szSideMenuKor[5]);
			else
				m_btnSideMenu[i].SetText(m_szSideMenuEng[5]);
		}

		if (i >= 3 && i < 7)
			m_btnSideMenu[i].ShowWindow(SW_HIDE);*/
	}
}
void CDlgSide::ChangeUI_Language()
{
	EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();

	for (int i = 0; i < Menu_Max; i++)
	{
		m_btnSideMenu[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Alt);
		m_btnSideMenu[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_btnSideMenu[i].SetFont_Gdip(L"Arial", 9.0F);

		if ((SEQUENCE->GetRunMode() == (RUN_MODE)eModeRun || SEQUENCE->GetRunMode() == eRunning || m_bFlagStartButtonLamp == TRUE))
		{
			m_btnSideMenu[0].SetStaticStyle(CVGStatic::StaticStyle_Title_Alt);
			m_btnSideMenu[0].SetColorStyle(CVGStatic::ColorStyle_Green);
			m_btnSideMenu[0].SetFont_Gdip(L"Arial", 9.0F);
			m_btnSideMenu[1].SetStaticStyle(CVGStatic::StaticStyle_Title_Alt);
			m_btnSideMenu[1].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
			m_btnSideMenu[1].SetFont_Gdip(L"Arial", 9.0F);
		}
		else
		{
			m_btnSideMenu[0].SetStaticStyle(CVGStatic::StaticStyle_Title_Alt);
			m_btnSideMenu[0].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
			m_btnSideMenu[0].SetFont_Gdip(L"Arial", 9.0F);
			m_btnSideMenu[1].SetStaticStyle(CVGStatic::StaticStyle_Title_Alt);
			m_btnSideMenu[1].SetColorStyle(CVGStatic::ColorStyle_Orange);
			m_btnSideMenu[1].SetFont_Gdip(L"Arial", 9.0F);
		}

		if (stBasic.nLanguage == 0)
			m_btnSideMenu[i].SetText(m_szSideMenuKor[i]);
		else
			m_btnSideMenu[i].SetText(m_szSideMenuEng[i]);

	}

	//for (int i = 0; i < 8; i++)
	//{
	//	m_btnSideMenu[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Alt);
	//	m_btnSideMenu[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
	//	m_btnSideMenu[i].SetFont_Gdip(L"Arial", 9.0F);

	//	if(stBasic.nLanguage == 0 )
	//		m_btnSideMenu[i].SetText(m_szSideMenuKor[i]);
	//	else
	//		m_btnSideMenu[i].SetText(m_szSideMenuEng[i]);

	//}
}
BOOL CDlgSide::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(135, 206, 235));		// SkyBlue
	//pDC->FillSolidRect(rect, RGB(112, 146, 190));		

	return TRUE;
}

void CDlgSide::SetSIdeBarStartButton_LampOn()
{
	EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();

	m_btnSideMenu[0].SetStaticStyle(CVGStatic::StaticStyle_Title_Alt);
	m_btnSideMenu[0].SetColorStyle(CVGStatic::ColorStyle_Green);
	m_btnSideMenu[0].SetFont_Gdip(L"Arial", 9.0F);

	if (stBasic.nLanguage == 0)
		m_btnSideMenu[0].SetText(m_szSideMenuKor[0]);
	else
		m_btnSideMenu[0].SetText(m_szSideMenuEng[0]);



	m_btnSideMenu[1].SetStaticStyle(CVGStatic::StaticStyle_Title_Alt);
	m_btnSideMenu[1].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
	m_btnSideMenu[1].SetFont_Gdip(L"Arial", 9.0F);
	if (stBasic.nLanguage == 0)
		m_btnSideMenu[1].SetText(m_szSideMenuKor[1]);
	else
		m_btnSideMenu[1].SetText(m_szSideMenuEng[1]);

	m_btnSideMenu[2].SetStaticStyle(CVGStatic::StaticStyle_Title_Alt);
	m_btnSideMenu[2].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
	m_btnSideMenu[2].SetFont_Gdip(L"Arial", 9.0F);
	if (stBasic.nLanguage == 0)
		m_btnSideMenu[2].SetText(m_szSideMenuKor[2]);
	
	//COMMON->SetEMODisable(TRUE);

}
void CDlgSide::SetSIdeBarStopButton_LampOn()
{
	EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();

	m_btnSideMenu[1].SetStaticStyle(CVGStatic::StaticStyle_Title_Alt);
	m_btnSideMenu[1].SetColorStyle(CVGStatic::ColorStyle_Orange);
	m_btnSideMenu[1].SetFont_Gdip(L"Arial", 9.0F);
	if (stBasic.nLanguage == 0)
		m_btnSideMenu[1].SetText(m_szSideMenuKor[1]);
	else
		m_btnSideMenu[1].SetText(m_szSideMenuEng[1]);


	m_btnSideMenu[0].SetStaticStyle(CVGStatic::StaticStyle_Title_Alt);
	m_btnSideMenu[0].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
	m_btnSideMenu[0].SetFont_Gdip(L"Arial", 9.0F);

	if (stBasic.nLanguage == 0)
		m_btnSideMenu[0].SetText(m_szSideMenuKor[0]);
	else
		m_btnSideMenu[0].SetText(m_szSideMenuEng[0]);

	m_btnSideMenu[2].SetStaticStyle(CVGStatic::StaticStyle_Title_Alt);
	m_btnSideMenu[2].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
	m_btnSideMenu[2].SetFont_Gdip(L"Arial", 9.0F);
	if (stBasic.nLanguage == 0)
		m_btnSideMenu[2].SetText(m_szSideMenuKor[2]);
	else
		m_btnSideMenu[2].SetText(m_szSideMenuEng[2]);
	//COMMON->SetEMODisable(FALSE);
	m_bFlagStartButtonLamp = FALSE;
}
void CDlgSide::SetSIdeBarDryRunButton_LampOn(int nOn)
{
	EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();

	m_btnSideMenu[0].SetStaticStyle(CVGStatic::StaticStyle_Title_Alt);
	m_btnSideMenu[0].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
	m_btnSideMenu[0].SetFont_Gdip(L"Arial", 9.0F);

	if (stBasic.nLanguage == 0)
		m_btnSideMenu[0].SetText(m_szSideMenuKor[0]);
	else
		m_btnSideMenu[0].SetText(m_szSideMenuEng[0]);

	m_btnSideMenu[1].SetStaticStyle(CVGStatic::StaticStyle_Title_Alt);
	m_btnSideMenu[1].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
	m_btnSideMenu[1].SetFont_Gdip(L"Arial", 9.0F);

	if (stBasic.nLanguage == 0)
		m_btnSideMenu[1].SetText(m_szSideMenuKor[1]);
	else
		m_btnSideMenu[1].SetText(m_szSideMenuEng[1]);
	if (nOn == TRUE)
	{
		m_btnSideMenu[2].SetStaticStyle(CVGStatic::StaticStyle_Title_Alt);
		m_btnSideMenu[2].SetColorStyle(CVGStatic::ColorStyle_LightGreen);
		m_btnSideMenu[2].SetFont_Gdip(L"Arial", 9.0F);
		if (stBasic.nLanguage == 0)
			m_btnSideMenu[2].SetText(m_szSideMenuKor[2]);
		else
			m_btnSideMenu[2].SetText(m_szSideMenuEng[2]);
	}
	else
	{
		m_btnSideMenu[2].SetStaticStyle(CVGStatic::StaticStyle_Title_Alt);
		m_btnSideMenu[2].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_btnSideMenu[2].SetFont_Gdip(L"Arial", 9.0F);
		if (stBasic.nLanguage == 0)
			m_btnSideMenu[2].SetText(m_szSideMenuKor[2]);
		else
			m_btnSideMenu[2].SetText(m_szSideMenuEng[2]);
	}
}
void CDlgSide::SetSIdeBarIntialButton_LampOn(int nOn)
{
	EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();

	if (nOn == TRUE)
	{
		m_btnSideMenu[7].SetStaticStyle(CVGStatic::StaticStyle_Title_Alt);
		m_btnSideMenu[7].SetColorStyle(CVGStatic::ColorStyle_Orange);
		m_btnSideMenu[7].SetFont_Gdip(L"Arial", 9.0F);
		if (stBasic.nLanguage == 0)
			m_btnSideMenu[7].SetText(m_szSideMenuKor[7]);
		else
			m_btnSideMenu[7].SetText(m_szSideMenuEng[7]);
	}
	else
	{
		m_btnSideMenu[7].SetStaticStyle(CVGStatic::StaticStyle_Title_Alt);
		m_btnSideMenu[7].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_btnSideMenu[7].SetFont_Gdip(L"Arial", 9.0F);
		if (stBasic.nLanguage == 0)
			m_btnSideMenu[7].SetText(m_szSideMenuKor[7]);
		else
			m_btnSideMenu[7].SetText(m_szSideMenuEng[7]);
	}
}
BOOL CDlgSide::Check_Start()
{
	CString strMsg;
	EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();
	// 1. 통신 연결이 되었는가?

	if (m_UdpTester[0].m_bConnected == FALSE)
	{
		TESTER_COMM stTester = *DATAMANAGER->GetCurrentTesterCommData();
		m_UdpTester[0].Connect_Tester(stTester.dwTesterIP[0], stTester.nTesterPort[0], stTester.strTesterConnect[0], 1);
	}

//#if (GET_INSPECTOR != SYS_DISTORTION_30)
	// 2. 모터가 초기화가 되어있는가?
	BOOL bTemp = FALSE;
	if (FAS_MO->IsConnected() == TRUE)
	{
		for (int i = 0; i < (int)MOTOR_MAX_COUNT; i++)
		{
			bTemp = FAS_MO->IsHomeOK(i);

			if (bTemp == FALSE)
			{
				strMsg.Format(_T("%d Motor is Not Initialize.\n"), i);
				WRITE_MAIN_LOG(strMsg);
				if(stBasic.nLanguage ==0)
					COMMON->ShowMsgDlg_Domodal(_T("[AutoRun Start] 모터가 초기화 되지 않았습니다."), MODE_INFORMATION);
				else
					COMMON->ShowMsgDlg_Domodal(_T("[AutoRun Start] Motor is  not initialize."), MODE_INFORMATION);
				return FALSE;
			}
		}
	}
	else
	{
		strMsg.Format(_T("Motor is Not Initialize.\n"));
		WRITE_MAIN_LOG(strMsg);
		if (stBasic.nLanguage == 0)
			COMMON->ShowMsgDlg_Domodal(_T("[AutoRun Start] 모터가 초기화 되지 않았습니다."), MODE_INFORMATION);
		else
			COMMON->ShowMsgDlg_Domodal(_T("[AutoRun Start] Motor is  not initialize."), MODE_INFORMATION);
		return FALSE;
	}
//#endif

	// 3. 모터는 정상인가?
	// 4. Door Lock 은 닫혀있는가?
	// [21.1027.4] Added, 닫혀있으면 락을 실시함

	BOOL b[7];
#if(GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL) || (GET_INSPECTOR == SYS_OQC_SFR_SINGLE_CL) || (GET_INSPECTOR == SYS_DISTORTION_30)
	b[0] = (AJIN_IO->GetInputIoBit(DIO_INPUT_X0_10_FRONT_DOORLOCK_OPENCLOSE) == 0 ? TRUE : FALSE);
	b[1] = (AJIN_IO->GetInputIoBit(DIO_INPUT_X0_11_LEFT_DOORLOCK1_OPENCLOSE) == 0 ? TRUE : FALSE);
#elif(GET_INSPECTOR == SYS_FILM_PEEL_OFF)
	b[0] = (AJIN_IO->GetInputIoBit(DIO_INPUT_X0_16_REAR_DOORLOCK1_OPENCLOSE) == 0 ? TRUE : FALSE);
	b[1] = (AJIN_IO->GetInputIoBit(DIO_INPUT_X0_17_REAR_DOORLOCK2_OPENCLOSE) == 0 ? TRUE : FALSE);
	b[2] = (AJIN_IO->GetInputIoBit(DIO_INPUT_X0_18_REAR_DOORLOCK3_OPENCLOSE) == 0 ? TRUE : FALSE);
#endif

#if(GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL)
	b[2] = (AJIN_IO->GetInputIoBit(DIO_INPUT_X0_12_LEFT_DOORLOCK2_OPENCLOSE) == 0 ? TRUE : FALSE);
	b[3] = (AJIN_IO->GetInputIoBit(DIO_INPUT_X0_13_RIGHT_DOORLOCK1_OPENCLOSE) == 0 ? TRUE : FALSE);
	b[4] = (AJIN_IO->GetInputIoBit(DIO_INPUT_X0_14_RIGHT_DOORLOCK2_OPENCLOSE) == 0 ? TRUE : FALSE);
	b[5] = (AJIN_IO->GetInputIoBit(DIO_INPUT_X0_15_BACK_DOORLOCK1_OPENCLOSE) == 0 ? TRUE : FALSE);
	b[6] = (AJIN_IO->GetInputIoBit(DIO_INPUT_X0_16_BACK_DOORLOCK2_OPENCLOSE) == 0 ? TRUE : FALSE);

	if (b[0])	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_10_DOORLOCK_ONOFF_FRONT, TRUE);
	if (b[1])	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_11_DOORLOCK_ONOFF_LEFT1, TRUE);
	if (b[2])	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_12_DOORLOCK_ONOFF_LEFT2, TRUE);
	if (b[3])	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_13_DOORLOCK_ONOFF_RIGHT1, TRUE);
	if (b[4])	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_14_DOORLOCK_ONOFF_RIGHT2, TRUE);
	if (b[5])	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_15_DOORLOCK_ONOFF_BACK1, TRUE);
	if (b[6])	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_16_DOORLOCK_ONOFF_BACK2, TRUE);

#elif(GET_INSPECTOR == SYS_OQC_SFR_SINGLE_CL)

	b[2] = (AJIN_IO->GetInputIoBit(DIO_INPUT_X0_12_RIGHT_DOORLOCK1_OPENCLOSE) == 0 ? TRUE : FALSE);
	b[3] = (AJIN_IO->GetInputIoBit(DIO_INPUT_X0_13_BACK_DOORLOCK1_OPENCLOSE) == 0 ? TRUE : FALSE);
	b[4] = (AJIN_IO->GetInputIoBit(DIO_INPUT_X0_14_BACK_DOORLOCK2_OPENCLOSE) == 0 ? TRUE : FALSE);

	if (b[0])	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_10_DOORLOCK_ONOFF_FRONT, TRUE);
	if (b[1])	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_11_DOORLOCK_ONOFF_LEFT1, TRUE);
	if (b[2])	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_12_DOORLOCK_ONOFF_RIGHT1, TRUE);
	if (b[3])	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_13_DOORLOCK_ONOFF_BACK1, TRUE);
	if (b[4])	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_14_DOORLOCK_ONOFF_BACK2, TRUE);

#endif
		
	Sleep(500);
	if (TRUE != COMMON->GetInterlock_DoorCheck())
	{
		if (stBasic.nLanguage == 0)
			COMMON->ShowMsgDlg_Domodal(_T("[AutoRun Start] 문을닫아주십시오. DoorLock신호를 모두 켜주십시오."), MODE_INFORMATION);
		else
			COMMON->ShowMsgDlg_Domodal(_T("[AutoRun Start] Close the door and then turn on the doorlock output signal"), MODE_INFORMATION);
		return FALSE;
	}

#if (GET_INSPECTOR == SYS_DISTORTION_30)
	// 5. Socket은 닫혀 있는가?
	/*
	if ((AJIN_IO->GetInputIoBit(DIO_INPUT_X0_08_MANUALSOCEKT_OPENCLOSE1) != TRUE) || (AJIN_IO->GetInputIoBit(DIO_INPUT_X0_09_MANUALSOCEKT_OPENCLOSE2) != TRUE))
	{
		strMsg.Format(_T("Manual Socket is Not Close.\n"));
		WRITE_MAIN_LOG(strMsg);
		if (stBasic.nLanguage == 0)
			COMMON->ShowMsgDlg_Domodal(_T("[AutoRun Start] 소켓을 닫아주십시오."), MODE_INFORMATION);
		else
			COMMON->ShowMsgDlg_Domodal(_T("[AutoRun Start] Please Close the Socket."), MODE_INFORMATION);
		return FALSE;
	}
	*/
#else
	// 5. 카메라는 있는가?
#ifndef _DRY_RUN_MODULE_NONE
// 	if (AJIN_IO->GetInputIoBit(DIO_INPUT_X0_08_CAMERA_DETECT_SENSOR) != TRUE)
// 	{
// 		strMsg.Format(_T("Camera sensor is not detected.\n"));
// 		WRITE_MAIN_LOG(strMsg);
// 		if (stBasic.nLanguage == 0)
// 			COMMON->ShowMsgDlg_Domodal(_T("[AutoRun Start] 카메라 센서에 감지되지 않았습니다."), MODE_INFORMATION);
// 		else
// 			COMMON->ShowMsgDlg_Domodal(_T("[AutoRun Start] Camera sensor is not detected."), MODE_INFORMATION);
// 		return FALSE;
// 	}
#endif
#endif


	return TRUE;
}

// auto mode 버튼
void CDlgSide::OnBnClickedBtnSideCommand1()
{
	// START BUTTON
	CString strTemp = _T("");
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();
//	if (COMMON->GetFlagStartUpMode() == TRUE)
//	{
		SetSIdeBarDryRunButton_LampOn(FALSE);
		COMMON->SetFlagStartUpMode(FALSE);
		
//		SEQUENCE->Reset_SeqFlags();
//		m_UdpTester[0].Reset_TesterFlag();

		// [21.1208.1] Add
		CDlgMain *pMain = CDlgMain::Get_Instance();
		pMain->HideStartUp();

		pMain->m_btnMainStartUpMode[0].ShowWindow(SW_HIDE);
		pMain->m_btnMainStartUpMode[1].ShowWindow(SW_HIDE);
		pMain->m_btnMainStartUpMode[2].ShowWindow(SW_HIDE);
		pMain->m_btnMainStartUpMode[3].ShowWindow(SW_HIDE);
		pMain->m_btnMainFN300Light[0].EnableWindow(FALSE);
		pMain->m_btnMainFN300Light[1].EnableWindow(FALSE);
//	}

#ifndef DevMode_Simulator
		if (Check_Start() == FALSE)
		{

		}
		else
		{
			SetOperationMode(OperationMode::Auto);

			CLuriMotion_OQA_SFR_WideDlg *pParrentDlg = (CLuriMotion_OQA_SFR_WideDlg*)AfxGetApp()->GetMainWnd();

			SetChangeMcgModeFlag(TRUE);
			SEQUENCE->AutoMode_Run();
			m_SideStopFlag = FALSE;

	#ifdef _DRY_RUN_MODULE_NONE
				if (stEq.nLanguage == 0)
					strTemp.Format(_T("DRYRUN MODE 버튼[UI] 누름."));
				else
					strTemp.Format(_T("DRYRUN MODE BUTTON[UI] PRESSED."));

	#else
				if (stEq.nLanguage == 0)
					strTemp.Format(_T("AUTO MODE 버튼[UI] 누름."));
				else
					strTemp.Format(_T("AUTO MODE BUTTON[UI] PRESSED."));
	#endif

			WRITE_MAIN_LOG(strTemp);
			// [21.1208.4] Add
			COMMON->SetAreaStop_OkPush(FALSE);
			COMMON->SetAreaStop_CancelPush(FALSE);
			COMMON->SetAccpet_ReciveBarcode(TRUE);

			m_bChangeModeWaitTime = FALSE;
			SEQUENCE->SetAreaStopLampnSideFlag(FALSE);
		}
#else
		SetOperationMode(OperationMode::Auto);

		CLuriMotion_OQA_SFR_WideDlg *pParrentDlg = (CLuriMotion_OQA_SFR_WideDlg*)AfxGetApp()->GetMainWnd();

		SetChangeMcgModeFlag(TRUE);
		SEQUENCE->AutoMode_Run();
		m_SideStopFlag = FALSE;

		if (stEq.nLanguage == 0)
			strTemp.Format(_T("AUTO MODE 버튼[UI] 누름."));
		else
			strTemp.Format(_T("AUTO MODE BUTTON[UI] PRESSED."));

		WRITE_MAIN_LOG(strTemp);
		// [21.1208.4] Add
		COMMON->SetAreaStop_OkPush(FALSE);
		COMMON->SetAreaStop_CancelPush(FALSE);
		COMMON->SetAccpet_ReciveBarcode(TRUE);

		m_bChangeModeWaitTime = FALSE;
		SEQUENCE->SetAreaStopLampnSideFlag(FALSE);
#endif
}

// manual mode 버튼
void CDlgSide::OnBnClickedBtnSideCommand2()
{
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();
//	if (COMMON->GetFlagStartUpMode() == TRUE)
//	{
		CDlgMain *pMain = CDlgMain::Get_Instance();
//		pMain->m_btnMainStartUpMode[0].ShowWindow(SW_HIDE);
//		pMain->m_btnMainStartUpMode[1].ShowWindow(SW_HIDE);
//		pMain->m_btnMainStartUpMode[2].ShowWindow(SW_HIDE);
//		pMain->m_btnMainStartUpMode[3].ShowWindow(SW_HIDE);

		pMain->HideStartUp();
		SetSIdeBarDryRunButton_LampOn(FALSE);
		COMMON->SetFlagStartUpMode(FALSE);
//		SEQUENCE->Reset_SeqFlags();
//		m_UdpTester[0].Reset_TesterFlag();
		pMain->m_btnMainFN300Light[0].EnableWindow(TRUE);
		pMain->m_btnMainFN300Light[1].EnableWindow(TRUE);
//	}
	SetOperationMode(OperationMode::Manual);
	SEQUENCE->ManualMode_Run();

	// STOP BUTTOn
//	SetSIdeBarStopButton_LampOn();
	SetChangeMcgModeFlag(FALSE);
	COMMON->SetAccpet_ReciveBarcode(FALSE);

	// Lock release
//	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_10_DOORLOCK_ONOFF_FRONT, FALSE);

	m_bFlagStartButtonLamp = FALSE;
	CString strTemp = _T("");

	// [21.1214.2]
	//if(COMMON->GetMOAllInitNeedFlag() == FALSE) /*COMMON->SetTowerLampStatus(eModeStop)*/ COMMON->SetEqStatus(eStop);

	if(stEq.nLanguage ==0)
		strTemp.Format(_T("MANUAL MODE 버튼[UI] 누름."));
	else
		strTemp.Format(_T("MANUAL MODE BUTTON[UI] PRESSED."));
	COMMON->Write_Main_Log(strTemp);
}

void CDlgSide::OnBnClickedBtnSideCommand3()
{
// 	if (SEQUENCE->GetRunMode() == eRunning)
// 		return;

	CDlgMain *pMain = CDlgMain::Get_Instance();
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();
	if (COMMON->GetFlagStartUpMode() == TRUE)
	{
		SetSIdeBarDryRunButton_LampOn(FALSE);
		COMMON->SetFlagStartUpMode(FALSE);
		SEQUENCE->Reset_SeqFlags();
		m_UdpTester[0].Reset_TesterFlag();


		pMain->HideStartUp();
	}
	else
	{
		SetOperationMode(OperationMode::StartUpCheck);

	//	SetSIdeBarDryRunButton_LampOn(TRUE);
		COMMON->SetFlagStartUpMode(TRUE);
		COMMON->SetFlagStartUpModeAction(-1);

		SEQUENCE->Reset_SeqFlags();
		m_UdpTester[0].Reset_TesterFlag();
		m_UdpTester[0].Reset_ReadyFlag();

		pMain->ShowStartUp();

		SetChangeMcgModeFlag(TRUE);	
		// [21.1209.3]
	//	SEQUENCE->AutoMode_Run();
		//COMMON->SetTowerLampStatus(eModeRun);
		//COMMON->SetEqStatus(eRunning);
		if(stEq.nLanguage ==0)
			WRITE_MAIN_LOG(_T("시업점검 버튼[UI] 누름"));
		else
			WRITE_MAIN_LOG(_T("START-UP BUTTON[UI] PRESSED"));
		SEQUENCE->SetAreaStopLampnSideFlag(FALSE);

	}
//	pMain->m_btnMainFN300Light[0].EnableWindow(TRUE);
//	pMain->m_btnMainFN300Light[1].EnableWindow(TRUE);
}

void CDlgSide::OnBnClickedBtnSideCommand4()
{
	CString strTemp = _T("");
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();

	// [21.1018.4] 인터락
	if (SEQUENCE->GetRunMode() != (RUN_MODE)eStop)
	{
		if(stEq.nLanguage ==0)
			strTemp.Format(_T("Stop 상태가 아님. Loto Mode 사용할 수 없음."));
		else
			strTemp.Format(_T("Not in Stop state. Loto mode is not available.."));

		COMMON->Write_Main_Log(strTemp);

		CDlgMessageBox dlg;
		dlg.SetMsg(strTemp, MODE_INFORMATION);
		dlg.DoModal();
		return;
	}
	if (COMMON->GetPermissionGrade() < LVL_MAINT)
	{
		if (stEq.nLanguage == 0)
			COMMON->ShowMsgDlg_Domodal(_T("권한이 없습니다."), MODE_INFORMATION);
		else
			COMMON->ShowMsgDlg_Domodal(_T("You don't have permission."), MODE_INFORMATION);
		return;
	}

	SetOperationMode(OperationMode::Loto);

	if(stEq.nLanguage ==0)
		strTemp.Format(_T("LOTO MODE 버튼[UI] 누름."));
	else
		strTemp.Format(_T("LOTO MODE BUTTON[UI] PREESED."));
	COMMON->Write_Main_Log(strTemp);

	CDlgLotoMode Dlg;
	if (IDOK == Dlg.DoModal())
	{

	}
}

void CDlgSide::OnBnClickedBtnSideCommand5()
{
}

void CDlgSide::OnBnClickedBtnSideCommand6()
{	
	if (COMMON->GetDryRunModeFlag() == TRUE)
	{
		SetSIdeBarDryRunButton_LampOn(FALSE);
		COMMON->SetDryRunModeFlag(FALSE);
	}
	else
	{
		SetSIdeBarDryRunButton_LampOn(TRUE);
		COMMON->SetDryRunModeFlag(TRUE);
	}
}

void CDlgSide::OnBnClickedBtnSideCommand7()
{
	if (COMMON->bUseSleepMode = COMMON->bUseSleepMode == TRUE ? FALSE : TRUE)
	{
		SetDlgItemTextW(IDC_BTN_SIDE_COMMAND_7, _T(""));
	}
	else
		SetDlgItemTextW(IDC_BTN_SIDE_COMMAND_7, _T("."));
}

// [21.1209.2] Flag 로 코드 감싸기
void CDlgSide::OnBnClickedBtnSideCommand8()
{
	OnInitialize();
}

// initialize 버튼 
void CDlgSide::OnInitialize()
{
	auto EqpType = GET_INSPECTOR;

// 	if (EqpType == SYS_DISTORTION_30)
// 		return;

	if (m_bInitFlagIsActive != FALSE)
		return;

	m_bInitFlagIsActive = TRUE;

	CLuriMotion_OQA_SFR_WideDlg *pParrentDlg = (CLuriMotion_OQA_SFR_WideDlg*)AfxGetApp()->GetMainWnd();
	CDlgMain *pMain = CDlgMain::Get_Instance();
	EQ_BASIC_PARAM	stEq = *DATAMANAGER->GetCurrentEqBasicData();
	if (SEQUENCE->GetRunMode() == eRunning || SEQUENCE->GetRunMode() == eStandby)
	{
		if (stEq.nLanguage == 0)
			COMMON->ShowMsgDlg_Domodal(_T("AutoMode중에는 초기화를 하실수 없습니다."), MODE_INFORMATION);
		else
			COMMON->ShowMsgDlg_Domodal(_T("You cannot initialize while in AutoMode."), MODE_INFORMATION);
		m_bInitFlagIsActive = FALSE;
		return;
	}

	if (COMMON->GetEMOAllInitFlag())
	{
		CString sz;
		sz.Format(_T("Reconnect Device"));
		COMMON->ShowMsg_Thread(sz, MODE_INFORMATION);
		WRITE_MAIN_LOG(sz);

		CDevice::ReconnectDeviez();

		COMMON->Close_MsgBox();
	}

	pMain->KillTimer(MAIN_TIMER_POWERSUPPLY);

	SetSIdeBarIntialButton_LampOn(TRUE);
	// [21.1214.3]
	COMMON->SetEMODisable(TRUE);
	// [22.0117.1]
	pParrentDlg->m_pDlgMain->EnableWindow(FALSE);
	pParrentDlg->m_pDlgSide->EnableWindow(FALSE);
	pParrentDlg->m_pDlgBtm->EnableWindow(FALSE);
	pParrentDlg->m_pDlgTitle->EnableWindow(FALSE);
	pParrentDlg->m_pDlgTeach->EnableWindow(FALSE);
	pParrentDlg->m_pDlgIO->EnableWindow(FALSE);
	// 초기화
	CDlgAxisHomeProgress Dlg;
	Dlg.DoModal();

	pParrentDlg->m_pDlgMain->EnableWindow(TRUE);
	pParrentDlg->m_pDlgSide->EnableWindow(TRUE);
	pParrentDlg->m_pDlgBtm->EnableWindow(TRUE);
	pParrentDlg->m_pDlgTitle->EnableWindow(TRUE);
	pParrentDlg->m_pDlgTeach->EnableWindow(TRUE);
	pParrentDlg->m_pDlgIO->EnableWindow(TRUE);
	COMMON->SetEMODisable(FALSE);

	if (m_UdpTester[0].m_bConnected == FALSE)
	{
		TESTER_COMM stTester = *DATAMANAGER->GetCurrentTesterCommData();
		m_UdpTester[0].Connect_Tester(stTester.dwTesterIP[0], stTester.nTesterPort[0], stTester.strTesterConnect[0], 1);

		COMMON->m_bConnectTesterFlag[0] = TRUE;
	}

	pMain->m_btnMainStartUpMode[0].ShowWindow(SW_HIDE);
	pMain->m_btnMainStartUpMode[1].ShowWindow(SW_HIDE);
	pMain->m_btnMainStartUpMode[2].ShowWindow(SW_HIDE);
	SetSIdeBarDryRunButton_LampOn(FALSE);
	SetSIdeBarIntialButton_LampOn(FALSE);
	pMain->m_btnMainFN300Light[0].EnableWindow(TRUE);
	pMain->m_btnMainFN300Light[1].EnableWindow(TRUE);
	pMain->SetTimer(MAIN_TIMER_POWERSUPPLY, 2000, NULL);

	COMMON->SetFlagStartUpMode(FALSE);
	SEQUENCE->Reset_SeqFlags();
	//m_UdpTester[0].Reset_TesterFlag();
	OnBnClickedBtnSideCommand2();

	m_bInitFlagIsActive = FALSE;
}

void CDlgSide::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	int mode = COMMON->GetSleepModeStep();

	switch (nIDEvent)
	{
	case TIMER_100_SWITCH_LAMPCHECK:
	{
#ifdef _DRY_RUN_MODULE_NONE
		if (SEQUENCE->GetRunMode() == eStandby)
		{
			if (SEQUENCE->IsStep_Ready())
			{
				if (m_bTemp1 == FALSE)
				{
					m_UdpTester[0].Reset_TesterFlag();
					m_dwTemp1 = GetTickCount();
					m_bTemp1 = TRUE;
				}
				else
				{
					if (GetTickCount() - m_dwTemp1 > TIMEOUT_DELAY_5000)
					{
						m_UdpTester[0].ForcedLive_Ready();
						m_bTemp1 = FALSE;
						SEQUENCE->Start_Run();
					}
				}
			}
		}
#endif
		// [21.1018.5] Loto Mode 시 Start, Init 금지
		if (COMMON->GetLotoMode() == TRUE)
			break;

		if (COMMON->GetSleepModeStep() == SLEEP_ENABLE)
			break;

		// start 버튼 램프 상태 변경 부분
		if (Check_Switch_Start() == FALSE)
			break;

		if (Check_Switch_Stop() == FALSE)
			break;

		/*if (Check_Switch_Init() == FALSE)
			break;*/

		if (SEQUENCE->GetRunMode() == (RUN_MODE)eStop && m_SideStopFlag == FALSE)
		{
			//SetSIdeBarStopButton_LampOn();
			m_SideStopFlag = TRUE;
		}

		if (COMMON->bUseSleepMode == TRUE)
		{
			if (SEQUENCE->GetRunMode() == (RUN_MODE)eStop)
			{
				if (mode == SLEEP_OFF)
				{
					COMMON->m_tSleepStart = GetTickCount64();
					COMMON->SetSleepModeStep(SLEEP_ON);
				}
				else if (mode == SLEEP_ON)
				{
					TESTER_COMM	stTester = *DATAMANAGER->GetCurrentTesterCommData();
					int nTime = stTester.nSleepModeTimer < 10 ? 10000 : stTester.nSleepModeTimer * 1000;
					COMMON->m_tSleepElapsed = GetTickCount64() - COMMON->m_tSleepStart;

					if (COMMON->m_tSleepElapsed > nTime)
					{
						COMMON->SetSleepModeStep(SLEEP_ENABLE);
#if(GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL) || (GET_INSPECTOR == SYS_OQC_SFR_SINGLE_CL) || (GET_INSPECTOR == SYS_DISTORTION_30)
						AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_0B_SLEEP_MODE_ON, TRUE);
#elif(GET_INSPECTOR == SYS_FILM_PEEL_OFF)
						AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_11_SLEEP_MODE_ON, FALSE);
#endif
						CDlgSleepMode Dlg;
						if (IDOK == Dlg.DoModal())
						{
#if(GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL) || (GET_INSPECTOR == SYS_OQC_SFR_SINGLE_CL) || (GET_INSPECTOR == SYS_DISTORTION_30)
							AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_0B_SLEEP_MODE_ON, FALSE);
#elif(GET_INSPECTOR == SYS_FILM_PEEL_OFF)
							AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_11_SLEEP_MODE_ON, FALSE);
#endif
							Sleep(300);
							FAS_MO->Disconnect_EziServo();
							COMMON->SetSleepModeStep(SLEEP_FINISH);
						}
					}
				}
				else if (mode == SLEEP_FINISH)
				{
					EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();
					COMMON->SetSleepModeStep(SLEEP_SUCCESS);
					if (stEq.nLanguage == 0)
						COMMON->ShowMsgDlg_Domodal(_T("Sleep모드가 해제되었습니다. Reset 버튼을 눌러주십시오."), MODE_INFORMATION);
					else
						COMMON->ShowMsgDlg_Domodal(_T("Sleep mode has been released. Please press the Reset button."), MODE_INFORMATION);
					COMMON->SetSleepModeStep(SLEEP_OFF);
					COMMON->SetMOAllInitNeedFlag(TRUE);
				}
			}
			else
			{
				COMMON->SetSleepModeStep(SLEEP_OFF);
			}
		}
	}
	break;

	// [21.1029.3] Add, 메인파워 오프시 Reset Lamp 점멸
	// [21.1214.4] Power off 추가
	case TIMER_200_EMO_POWER_CHECK:
	{
//#ifndef _DEBUG
		//if (COMMON->GetEMODisable() == FALSE &&  m_bEMOSignalCheck == FALSE && mode < SLEEP_ENABLE)
		//{
		//	//if (AJIN_IO->GetInputIoBit(DIO_INPUT_X0_04_EMO) == FALSE)
		//	if (AJIN_IO->GetInputIoBit(DIO_INPUT_X0_00_POWER) == FALSE)
		//	{
		//		m_bEMOSignalCheck = TRUE;
		//		EQ_BASIC_PARAM	stEq = *DATAMANAGER->GetCurrentEqBasicData();
		//		if (FAS_MO->IsConnected() == TRUE) FAS_MO->Disconnect_EziServo();

		//		COMMON->SetMOAllInitNeedFlag(TRUE);
		//		COMMON->SetEqStatus(eError);

		//		if (stEq.nLanguage == 0)
		//			COMMON->ShowMsgDlg_Domodal(_T("EMO버튼이 눌렸습니다. \nEMO 해제후 Reset 버튼을 눌러주십시오."), MODE_ERROR);
		//		else
		//			COMMON->ShowMsgDlg_Domodal(_T("The EMO button has been pressed. \nPlease press the Reset button after releasing the EMO."), MODE_ERROR);
		//		COMMON->SetEqStatus(eStop);
		//		m_bEMOSignalCheck = FALSE;
		//	}
		//	else if (AJIN_IO->GetInputIoBit(DIO_INPUT_X0_00_POWER) == FALSE)
		//	{
		//		m_bEMOSignalCheck = TRUE;
		//		EQ_BASIC_PARAM	stEq = *DATAMANAGER->GetCurrentEqBasicData();
		//		if (FAS_MO->IsConnected() == TRUE) FAS_MO->Disconnect_EziServo();

		//		COMMON->SetMOAllInitNeedFlag(TRUE);
		//		COMMON->SetEqStatus(eError);

		//		if (stEq.nLanguage == 0)
		//			COMMON->ShowMsgDlg_Domodal(_T("메인파워가 공급되지 않습니다. Reset 버튼을 눌러주십시오"), MODE_ERROR);
		//		else
		//			COMMON->ShowMsgDlg_Domodal(_T("Main Power is not supplied. Please press the Reset button."), MODE_ERROR);
		//		COMMON->SetEqStatus(eStop);
		//		m_bEMOSignalCheck = FALSE;
		//	}
		//}

		// EMO, Reset, Init Lamp
	//	Lamp_EqpType();
//#endif
	}
	break;

	case TIMER_300_CHANGE_MODE:
	{
		if (SEQUENCE->GetRunMode() == eRunning || SEQUENCE->GetRunMode() == eStandby)
		{
			TEACHING_PARAM	stTester = *DATAMANAGER->GetCurrentTeachingData();

			if (m_UdpTester[0].m_bTesterFlag_Ready == FALSE && m_bChangeModeWaitTime == FALSE)
			{
				m_tChangeModeWaitTime = GetTickCount64();
				m_bChangeModeWaitTime = TRUE;
			}
			else if (m_bChangeModeWaitTime == TRUE)
			{
				if (GetTickCount64() - m_tChangeModeWaitTime < stTester.nChangeMode_Time)
				{
					if (m_UdpTester[0].m_bTesterFlag_Ready == TRUE || COMMON->GetFlagStartUpMode() == TRUE) //211029 - 26번 COMMON->GetFlagStartUpMode() == TRUE) 추가, 기존 Ready신호 해제에 시업점검 모드면 해제되도록 추가
					{
						m_bChangeModeWaitTime = FALSE;
					}
				}
				else
				{
					// if (GetTickCount64() - m_tChangeModeWaitTime < stTester.nChangeMode_Time )
					SEQUENCE->Stop_Run();
					SetSIdeBarStopButton_LampOn();

					CString strTemp = _T("");
					strTemp.Format(_T("TIme Over %d min."), stTester.GetChangeModeTime());
					WRITE_MAIN_LOG(strTemp);
				}
			}
		}

		/*
		if (m_UdpTester[0].m_bTesterFlag_Ready == FALSE)
		{
			TEACHING_PARAM	stTester = *DATAMANAGER->GetCurrentTeachingData();
			if (m_bChangeModeWaitTime == FALSE)
			{
				m_tChangeModeWaitTime = GetTickCount64();
				m_bChangeModeWaitTime = TRUE;
			}
			if (GetTickCount64() - m_tChangeModeWaitTime > stTester.nChangeMode_Time * 1000)
				OnBnClickedBtnSideCommand2();
		}
		*/
	}
	break;

	};

	CDialogEx::OnTimer(nIDEvent);
}
BOOL CDlgSide::Check_Switch_Start()
{
	if (COMMON->GetAreaStop_CancelPush() == TRUE)
		return TRUE;

	if (FALSE == AJIN_IO->GetInputIoBit(DIO_INPUT_X0_04_EMO))
		return TRUE;

	if (COMMON->GetAreaStop_OkPush() == TRUE)
	{
		if (TRUE == AJIN_IO->GetInputIoBit(DIO_INPUT_X0_01_START))
		{
			AJIN_IO->ChangeBtnStatus(DIO_OUTPUT_Y0_01_START_SWITCH_LAMP, 1, TRUE);
			COMMON->SetAreaStop_OkPush(FALSE);
		}
	}

	if (SEQUENCE->ReadyToStart() && SEQUENCE->GetRunMode() != eStop)
	{
		if (m_UdpTester[0].m_bTesterFlag_Ready == TRUE)
			//	if(SEQUENCE->ReadyToStart())
		{
			if (SEQUENCE->GetAreaStopLampnSideFlag() == TRUE)
			{
				m_UdpTester[0].Reset_TesterFlag();
				COMMON->SetAccpet_ReciveBarcode(FALSE);
				return FALSE;
			}
			if (m_bFlagStartButtonLamp == false)
			{
				m_tTimeOutReadySignal = GetTickCount64();
				m_bFlagStartButtonLamp = TRUE;
				AJIN_IO->ChangeBtnStatus(DIO_OUTPUT_Y0_01_START_SWITCH_LAMP, 0, TRUE);
			}

			if (TRUE == AJIN_IO->GetInputIoBit(DIO_INPUT_X0_01_START))
			{
				if (m_MainManualFlag == TRUE)
					return FALSE;
				WRITE_MAIN_LOG(_T("Start switch pushed."));

				m_tTimeOutReadySignal = GetTickCount64();

				if (m_bStartFlag == FALSE)
				{
					m_tStartBtnPress = GetTickCount64();
					m_bStartFlag = TRUE;
				}

				if (GetTickCount64() - m_tStartBtnPress > 100 && m_bStartFlagIsActive == FALSE)
				{
					COMMON->SetForcedStop_1Sec(TRUE);
					COMMON->m_dwForcedStop_Under_1Sec = GetTickCount();

					m_bStartFlagIsActive = TRUE;

					if (Check_Start() == FALSE)
					{
						m_bStartFlagIsActive = FALSE;
						WRITE_MAIN_LOG(_T("Start Run Fail."));
					}
					else
					{
						if (m_UdpTester[0].m_bConnected == FALSE)
						{
							TESTER_COMM stTester = *DATAMANAGER->GetCurrentTesterCommData();
							m_UdpTester[0].Connect_Tester(stTester.dwTesterIP[0], stTester.nTesterPort[0], stTester.strTesterConnect[0], 1);
						}

						m_SideStopFlag = FALSE;
						SetSIdeBarStartButton_LampOn();
						SEQUENCE->Start_Run();
						WRITE_MAIN_LOG(_T("Start Run Success."));
					}
				}
			}
			else if (m_bStartFlag == TRUE)
			{
				m_bStartFlagIsActive = FALSE;
				m_bStartFlag = FALSE;
			}
			else if (SEQUENCE->GetRunMode() != eRunning && SEQUENCE->GetRunMode() != eStandby)
			{
				TESTER_COMM	stTester = *DATAMANAGER->GetCurrentTesterCommData();
				if (GetTickCount64() - m_tTimeOutReadySignal > _ttoi(stTester.strWaitTimeLIst[5]))
				{
					m_UdpTester[0].Reset_TesterFlag();
					SEQUENCE->Reset_SeqFlags();
					AJIN_IO->BlinkStop(DIO_OUTPUT_Y0_01_START_SWITCH_LAMP, FALSE);
					m_bFlagStartButtonLamp = FALSE;
				}
			}
		}
		else
		{
			// 검사중이 아닐경우
			if (SEQUENCE->GetRunMode() != eRunning)
			{

			}
		}
	}
	else
	{
		AJIN_IO->BlinkStop(DIO_OUTPUT_Y0_01_START_SWITCH_LAMP, FALSE);
		m_bFlagStartButtonLamp = FALSE;
	}
		
	return TRUE;
}

BOOL CDlgSide::Check_Switch_Stop()
{
	if (COMMON->GetAreaStop_OkPush() == TRUE )
		return TRUE;

	if (FALSE == AJIN_IO->GetInputIoBit(DIO_INPUT_X0_04_EMO))
		return TRUE;

	if (COMMON->m_bStopFlag_MissStart == TRUE)
	{
		SetSIdeBarStopButton_LampOn();
		m_bFlagStartButtonLamp = FALSE;
		COMMON->m_bStopFlag_MissStart = FALSE;
		return TRUE;
	}

	if (SEQUENCE->IsStep_WaitForPushSwitch())
	{
		if (TRUE == AJIN_IO->GetInputIoBit(DIO_INPUT_X0_02_STOP))
		{
			m_bFlagStartButtonLamp = FALSE;

			SEQUENCE->SetStopswitchMode(TRUE);
			return TRUE;
		}	
	}
	
	if (COMMON->GetAreaStop_CancelPush() == TRUE)
	{
		if (TRUE == AJIN_IO->GetInputIoBit(DIO_INPUT_X0_02_STOP))
		{
			AJIN_IO->BlinkStop(DIO_OUTPUT_Y0_01_START_SWITCH_LAMP, TRUE);
			AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_01_START_SWITCH_LAMP, FALSE);
			AJIN_IO->BlinkStop(DIO_OUTPUT_Y0_02_STOP_SWITCH_LAMP, TRUE);
			AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_02_STOP_SWITCH_LAMP, FALSE);

			COMMON->SetAreaStop_CancelPush(FALSE);	// #.#
			SEQUENCE->SetSFR_Inspection_Step();
			SEQUENCE->Start_Run();
			return TRUE;
		}
	}

	if (COMMON->GetFOrced_SideBtn_StopOn() == TRUE)
	{
		SetSIdeBarStopButton_LampOn();
		COMMON->SetForced_SideBtn_StopOn(FALSE);
		return TRUE;
	}

	if (TRUE == AJIN_IO->GetInputIoBit(DIO_INPUT_X0_02_STOP))
	{
		if (m_bStopFlag == FALSE) 
			m_tStopBtnPress = GetTickCount64();
		
		m_bStopFlag = TRUE;

		if (GetTickCount64() - m_tStopBtnPress > 100 && m_bStopFlagIsActive == FALSE)
		{
			m_bFlagStartButtonLamp = FALSE;
			m_bStopFlagIsActive = TRUE;
			
			SEQUENCE->SetStopswitchMode(TRUE);
		}
	}
	else if (m_bStopFlag == TRUE)
	{
		if (SEQUENCE->IsStep_Ready())
		{
			COMMON->SetStopSwitchPushed(TRUE);
		}

		m_bStopFlagIsActive = FALSE;
		m_bStopFlag = FALSE;
	}

	return TRUE;
}

// [21.1209.1]
BOOL CDlgSide::Check_Switch_Init()
{
	if (FALSE == AJIN_IO->GetInputIoBit(DIO_INPUT_X0_04_EMO))
		return FALSE;

	if (TRUE == AJIN_IO->GetInputIoBit(DIO_INPUT_X0_03_INIT))
	{
		//EQ_BASIC_PARAM	stEq = *DATAMANAGER->GetCurrentEqBasicData();
		//if (SEQUENCE->GetRunMode() == eRunning || SEQUENCE->GetRunMode() == eStandby)
		//{
		//	if (stEq.nLanguage == 0)
		//		COMMON->ShowMsgDlg_Domodal(_T("AutoMode중에는 초기화를 하실수 없습니다."), MODE_INFORMATION);
		//	else
		//		COMMON->ShowMsgDlg_Domodal(_T("You cannot initialize while in AutoMode."), MODE_INFORMATION);
		//	return TRUE;
		//}

		if (m_bInitFlag == FALSE) m_tInitBtnPress = GetTickCount64();
		m_bInitFlag = TRUE;

		//AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_03_INIT_SWITCH_LAMP, TRUE);
		if (GetTickCount64() - m_tInitBtnPress > 200 && m_bInitFlagIsActive == FALSE)
		{
			//SetSIdeBarIntialButton_LampOn(TRUE);
			//m_bInitFlagIsActive = TRUE;
			//CDlgAxisHomeProgress Dlg;
			//if (IDCANCEL == Dlg.DoModal())
			//{
			//	m_UdpTester[0].Reset_TesterFlag();
			//	SEQUENCE->Reset_SeqFlags();
			//}
			//SetSIdeBarIntialButton_LampOn(FALSE);
			SendMessage(WM_COMMAND, IDC_BTN_SIDE_COMMAND_8, 0);	// 홈잉
		}
	}
	else if (m_bInitFlag == TRUE)
	{
		//m_bInitFlagIsActive = FALSE;
		//AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_03_INIT_SWITCH_LAMP, FALSE);
		m_bInitFlag = FALSE;
	}

	return TRUE;
}

void CDlgSide::SetChangeMcgModeFlag(BOOL bState)
{
	if(bState == TRUE)
	{
		m_MainAutoFlag = TRUE;
		m_MainManualFlag = FALSE;
	}
	else
	{
		m_MainAutoFlag= FALSE;
		m_MainManualFlag = TRUE;
	}
}

void CDlgSide::Lamp_EqpType()
{
	auto EqpType = GET_INSPECTOR;

	switch (EqpType)
	{
	case SYS_DISTORTION_30:
	{
		if (m_bPowerStatus == 0) // 분기확인
		{
			if (TRUE == AJIN_IO->GetInputIoBit(DIO_INPUT_X0_00_POWER))
				m_bPowerStatus = 1; // 온
			else
				m_bPowerStatus = 2; // 오프
		}
		else if (m_bPowerStatus == 1 && TRUE == AJIN_IO->GetInputIoBit(DIO_INPUT_X0_00_POWER))
		{
			AJIN_IO->BlinkStop(DIO_OUTPUT_Y0_04_RESET_SWITCH_LAMP, FALSE);
			if (COMMON->GetMOAllInitNeedFlag() == TRUE)
				AJIN_IO->BlinkStart(DIO_OUTPUT_Y0_03_INIT_SWITCH_LAMP, TRUE, 500, 500);
			m_bPowerStatus = 2;
		}
		else if (m_bPowerStatus == 2 && FALSE == AJIN_IO->GetInputIoBit(DIO_INPUT_X0_00_POWER))
		{
			AJIN_IO->BlinkStart(DIO_OUTPUT_Y0_04_RESET_SWITCH_LAMP, TRUE, 500, 500);
			m_bPowerStatus = 1;
		}
	}
	break;

	default:
	{
		if (m_bPowerStatus == 0) // 분기확인
		{
			if (TRUE == AJIN_IO->GetInputIoBit(DIO_INPUT_X0_00_POWER))
				m_bPowerStatus = 1; // 온
			else
				m_bPowerStatus = 2; // 오프
		}
		else if (m_bPowerStatus == 1 && TRUE == AJIN_IO->GetInputIoBit(DIO_INPUT_X0_00_POWER))
		{
			AJIN_IO->BlinkStop(DIO_OUTPUT_Y0_04_RESET_SWITCH_LAMP, FALSE);
			if (COMMON->GetMOAllInitNeedFlag() == TRUE)
				AJIN_IO->BlinkStart(DIO_OUTPUT_Y0_03_INIT_SWITCH_LAMP, TRUE, 500, 500);
			m_bPowerStatus = 2;
		}
		else if (m_bPowerStatus == 2 && FALSE == AJIN_IO->GetInputIoBit(DIO_INPUT_X0_00_POWER))
		{
			AJIN_IO->BlinkStart(DIO_OUTPUT_Y0_04_RESET_SWITCH_LAMP, TRUE, 500, 500);
			m_bPowerStatus = 1;
		}
	}
	break;
	}
}

void CDlgSide::SetOperationMode(__in OperationMode mode)
{
	COMMON->SetOperationMode(mode);

	switch (mode)
	{
	case OperationMode::Auto:
		SetSIdeBarStartButton_LampOn();
		WRITE_MAIN_LOG(_T("Operation : Auto Mode"));
		break;

	case OperationMode::Manual:
		SetSIdeBarStopButton_LampOn();
		WRITE_MAIN_LOG(_T("Operation : Manual Mode"));
		break;

	case OperationMode::StartUpCheck:
		SetSIdeBarDryRunButton_LampOn(TRUE);
		WRITE_MAIN_LOG(_T("Operation : Startup Check Mode"));
		break;

	case OperationMode::Loto:
		WRITE_MAIN_LOG(_T("Operation : Loto Mode"));
		break;
	}
}
