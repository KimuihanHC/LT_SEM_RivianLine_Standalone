// DlgMain.cpp: 구현 파일
//

#include "pch.h"
#include "LuriMotion_OQA_SFR_Wide.h"
#include "LuriMotion_OQA_SFR_WideDlg.h"
#include "DlgMain.h"
#include "afxdialogex.h"
#include <tlhelp32.h>   // CreateToolhelp32Snapshot() 를 사용하기 위한 해더포함

#include "IO.h"
#include "Device.h"

#include "System_Define.h"

#define	MESSAGE_UI_CHANGE_HOME				(WM_USER + 1)
#define	MESSAGE_UI_CHANGE_LIMIT				(WM_USER + 2)
#define	MESSAGE_UI_CHANGE_ALARM				(WM_USER + 3)
#define	MESSAGE_UI_CHANGE_INMOTION			(WM_USER + 4)
// CDlgMain 대화 상자

IMPLEMENT_DYNAMIC(CDlgMain, CDialogEx)
CDlgMain *CDlgMain::m_pInstance = NULL;

CDlgMain *CDlgMain::Get_Instance(CWnd *pParent)
{
	if (!m_pInstance) {
		m_pInstance = new CDlgMain(pParent);
		if (!m_pInstance->m_hWnd) {
			m_pInstance->Create(IDD_DLG_MAIN, pParent);
		}
	}
	return m_pInstance;
}

void CDlgMain::Delete_Instance()
{
	if (m_pInstance->m_hWnd) m_pInstance->DestroyWindow();
	if (m_pInstance) delete m_pInstance;
	m_pInstance = NULL;
}

CDlgMain::CDlgMain(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_MAIN, pParent)
{
	VERIFY(m_font_Normal.CreateFont(
		15,							// nHeight
		0,							// nWidth
		0,							// nEscapement
		0,							// nOrientation
		FW_NORMAL,					// nWeight
		FALSE,						// bItalic
		FALSE,						// bUnderline
		0,							// cStrikeOut
		ANSI_CHARSET,				// nCharSet
		OUT_DEFAULT_PRECIS,			// nOutPrecision
		CLIP_DEFAULT_PRECIS,		// nClipPrecision
		DEFAULT_QUALITY,			// nQuality
		VARIABLE_PITCH,				// nPitchAndFamily
		_T("Arial")));			// lpszFacename


	m_nBtnUiMode = 1;
	m_lCount = 0;

	for (int i = 0; i < (int)MOTOR_MAX_COUNT; i++)
	{
		m_bMotorHome_Done[i] = FALSE;
		m_bMotorHome_Done_Old[i] = FALSE;

		m_bMotorLimit_M[i] = FALSE;
		m_bMotorLimit_P[i] = FALSE;

		m_bMotorAlarm[i] = FALSE;
		m_bMotorInMotion[i] = FALSE;
	}
	m_bMchState_View_1 = FALSE;
	m_bMchState_View_2 = FALSE;
	m_bMchState_View_3 = FALSE;
	m_bMchState_View_4 = FALSE;
	m_bMchState_View_5 = FALSE;

	m_nPowerSupplyReadMode = 0;
	COMMON->SetFlagStartUpModeAction(-1);
	m_bChangeUIReplaceFlag = TRUE;
}

CDlgMain::~CDlgMain()
{
}

void CDlgMain::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	
	DDX_Control(pDX, IDC_STC_MAIN_MOTOR_STATUS_NUM_1, m_stMainMotor_Number[0]);
	DDX_Control(pDX, IDC_STC_MAIN_MOTOR_STATUS_NUM_2, m_stMainMotor_Number[1]);
	DDX_Control(pDX, IDC_STC_MAIN_MOTOR_STATUS_NUM_3, m_stMainMotor_Number[2]);
	DDX_Control(pDX, IDC_STC_MAIN_MOTOR_STATUS_NUM_4, m_stMainMotor_Number[3]);
	DDX_Control(pDX, IDC_STC_MAIN_MOTOR_STATUS_NUM_5, m_stMainMotor_Number[4]);
	DDX_Control(pDX, IDC_STC_MAIN_MOTOR_STATUS_NUM_6, m_stMainMotor_Number[5]);

	DDX_Control(pDX, IDC_STC_MAIN_MOTOR_SUBTITLE_1, m_stMainMotor_Title[0]);
	DDX_Control(pDX, IDC_STC_MAIN_MOTOR_SUBTITLE_2, m_stMainMotor_Title[1]);
	DDX_Control(pDX, IDC_STC_MAIN_MOTOR_SUBTITLE_3, m_stMainMotor_Title[2]);
	DDX_Control(pDX, IDC_STC_MAIN_MOTOR_SUBTITLE_4, m_stMainMotor_Title[3]);
	DDX_Control(pDX, IDC_STC_MAIN_MOTOR_SUBTITLE_5, m_stMainMotor_Title[4]);
	DDX_Control(pDX, IDC_STC_MAIN_MOTOR_SUBTITLE_6, m_stMainMotor_Title[5]);
	DDX_Control(pDX, IDC_STC_MAIN_MOTOR_SUBTITLE_7, m_stMainMotor_Title[6]);

	DDX_Control(pDX, IDC_STC_MOTION_NAME_1, m_StMainMotor_Name[0]);
	DDX_Control(pDX, IDC_STC_MOTION_NAME_2, m_StMainMotor_Name[1]);
	DDX_Control(pDX, IDC_STC_MOTION_NAME_3, m_StMainMotor_Name[2]);
	DDX_Control(pDX, IDC_STC_MOTION_NAME_4, m_StMainMotor_Name[3]);
	DDX_Control(pDX, IDC_STC_MOTION_NAME_5, m_StMainMotor_Name[4]);
	DDX_Control(pDX, IDC_STC_MOTION_NAME_6, m_StMainMotor_Name[5]);

	DDX_Control(pDX, IDC_STC_MAIN_CUR_POS_1, m_stMainMotor_Pos[0]);
	DDX_Control(pDX, IDC_STC_MAIN_CUR_POS_2, m_stMainMotor_Pos[1]);
	DDX_Control(pDX, IDC_STC_MAIN_CUR_POS_3, m_stMainMotor_Pos[2]);
	DDX_Control(pDX, IDC_STC_MAIN_CUR_POS_4, m_stMainMotor_Pos[3]);
	DDX_Control(pDX, IDC_STC_MAIN_CUR_POS_5, m_stMainMotor_Pos[4]);
	DDX_Control(pDX, IDC_STC_MAIN_CUR_POS_6, m_stMainMotor_Pos[5]);

	DDX_Control(pDX, IDC_STC_MAIN_MOTOR_LIMIT_PLUS_1, m_stMainMotor_Limit_P[0]);
	DDX_Control(pDX, IDC_STC_MAIN_MOTOR_LIMIT_PLUS_2, m_stMainMotor_Limit_P[1]);
	DDX_Control(pDX, IDC_STC_MAIN_MOTOR_LIMIT_PLUS_3, m_stMainMotor_Limit_P[2]);
	DDX_Control(pDX, IDC_STC_MAIN_MOTOR_LIMIT_PLUS_4, m_stMainMotor_Limit_P[3]);
	DDX_Control(pDX, IDC_STC_MAIN_MOTOR_LIMIT_PLUS_5, m_stMainMotor_Limit_P[4]);
	DDX_Control(pDX, IDC_STC_MAIN_MOTOR_LIMIT_PLUS_6, m_stMainMotor_Limit_P[5]);

	DDX_Control(pDX, IDC_STC_MAIN_MOTOR_LIMIT_MINUS_1, m_stMainMotor_Limit_M[0]);
	DDX_Control(pDX, IDC_STC_MAIN_MOTOR_LIMIT_MINUS_2, m_stMainMotor_Limit_M[1]);
	DDX_Control(pDX, IDC_STC_MAIN_MOTOR_LIMIT_MINUS_3, m_stMainMotor_Limit_M[2]);
	DDX_Control(pDX, IDC_STC_MAIN_MOTOR_LIMIT_MINUS_4, m_stMainMotor_Limit_M[3]);
	DDX_Control(pDX, IDC_STC_MAIN_MOTOR_LIMIT_MINUS_5, m_stMainMotor_Limit_M[4]);
	DDX_Control(pDX, IDC_STC_MAIN_MOTOR_LIMIT_MINUS_6, m_stMainMotor_Limit_M[5]);

	DDX_Control(pDX, IDC_STC_MAIN_MOTOR_ERROR_1, m_stMainMotor_Error[0]);
	DDX_Control(pDX, IDC_STC_MAIN_MOTOR_ERROR_2, m_stMainMotor_Error[1]);
	DDX_Control(pDX, IDC_STC_MAIN_MOTOR_ERROR_3, m_stMainMotor_Error[2]);
	DDX_Control(pDX, IDC_STC_MAIN_MOTOR_ERROR_4, m_stMainMotor_Error[3]);
	DDX_Control(pDX, IDC_STC_MAIN_MOTOR_ERROR_5, m_stMainMotor_Error[4]);
	DDX_Control(pDX, IDC_STC_MAIN_MOTOR_ERROR_6, m_stMainMotor_Error[5]);

	DDX_Control(pDX, IDC_STC_MAIN_MOTOR_INMOTION_1, m_stMainMotor_InMotion[0]);
	DDX_Control(pDX, IDC_STC_MAIN_MOTOR_INMOTION_2, m_stMainMotor_InMotion[1]);
	DDX_Control(pDX, IDC_STC_MAIN_MOTOR_INMOTION_3, m_stMainMotor_InMotion[2]);
	DDX_Control(pDX, IDC_STC_MAIN_MOTOR_INMOTION_4, m_stMainMotor_InMotion[3]);
	DDX_Control(pDX, IDC_STC_MAIN_MOTOR_INMOTION_5, m_stMainMotor_InMotion[4]);
	DDX_Control(pDX, IDC_STC_MAIN_MOTOR_INMOTION_6, m_stMainMotor_InMotion[5]);

	DDX_Control(pDX, IDC_STC_MAIN_HOME_1, m_stMainMotor_Home[0]);
	DDX_Control(pDX, IDC_STC_MAIN_HOME_2, m_stMainMotor_Home[1]);
	DDX_Control(pDX, IDC_STC_MAIN_HOME_3, m_stMainMotor_Home[2]);
	DDX_Control(pDX, IDC_STC_MAIN_HOME_4, m_stMainMotor_Home[3]);
	DDX_Control(pDX, IDC_STC_MAIN_HOME_5, m_stMainMotor_Home[4]);
	DDX_Control(pDX, IDC_STC_MAIN_HOME_6, m_stMainMotor_Home[5]);

	DDX_Control(pDX, IDC_STC_COMMUNICATION_STATUS_1, m_stMainComm_Status[0]);
	DDX_Control(pDX, IDC_STC_COMMUNICATION_STATUS_2, m_stMainComm_Status[1]);

	DDX_Control(pDX, IDC_STC_COMMUNICATION_NAME_1, m_stMainComm_LanSerial[0][0]);
	DDX_Control(pDX, IDC_STC_COMMUNICATION_NAME_2, m_stMainComm_LanSerial[1][0]);
	DDX_Control(pDX, IDC_STC_COMMUNICATION_NAME_3, m_stMainComm_LanSerial[2][0]);
	DDX_Control(pDX, IDC_STC_COMMUNICATION_NAME_4, m_stMainComm_LanSerial[3][0]);
	DDX_Control(pDX, IDC_STC_COMMUNICATION_NAME_5, m_stMainComm_LanSerial[4][0]);

	DDX_Control(pDX, IDC_STC_COMM_CONNECT_1, m_stMainComm_LanSerial[0][1]);
	DDX_Control(pDX, IDC_STC_COMM_CONNECT_2, m_stMainComm_LanSerial[1][1]);
	DDX_Control(pDX, IDC_STC_COMM_CONNECT_3, m_stMainComm_LanSerial[2][1]);
	DDX_Control(pDX, IDC_STC_COMM_CONNECT_4, m_stMainComm_LanSerial[3][1]);
	DDX_Control(pDX, IDC_STC_COMM_CONNECT_5, m_stMainComm_LanSerial[4][1]);

	DDX_Control(pDX, IDC_STC_MAIN_IO_FRONT_1, m_stMainComm_IO_Input_Status[0][0]);
	DDX_Control(pDX, IDC_STC_MAIN_IO_FRONT_2, m_stMainComm_IO_Input_Status[1][0]);
	DDX_Control(pDX, IDC_STC_MAIN_IO_FRONT_3, m_stMainComm_IO_Input_Status[2][0]);
	DDX_Control(pDX, IDC_STC_MAIN_IO_FRONT_RESULT_1, m_stMainComm_IO_Input_Status[0][1]);
	DDX_Control(pDX, IDC_STC_MAIN_IO_FRONT_RESULT_2, m_stMainComm_IO_Input_Status[1][1]);
	DDX_Control(pDX, IDC_STC_MAIN_IO_FRONT_RESULT_3, m_stMainComm_IO_Input_Status[2][1]);

	DDX_Control(pDX, IDC_STC_MAIN_IO_REAR_1, m_stMainComm_IO_Output_Status[0][0]);
	DDX_Control(pDX, IDC_STC_MAIN_IO_REAR_2, m_stMainComm_IO_Output_Status[1][0]);
	DDX_Control(pDX, IDC_STC_MAIN_IO_REAR_3, m_stMainComm_IO_Output_Status[2][0]);
	DDX_Control(pDX, IDC_STC_MAIN_IO_REAR_RESULT_1, m_stMainComm_IO_Output_Status[0][1]);
	DDX_Control(pDX, IDC_STC_MAIN_IO_REAR_RESULT_2, m_stMainComm_IO_Output_Status[1][1]);
	DDX_Control(pDX, IDC_STC_MAIN_IO_REAR_RESULT_3, m_stMainComm_IO_Output_Status[2][1]);

	DDX_Control(pDX, IDC_STC_POWERSUPPLY_CURRENT, m_stPowerSupplyInfo[0]);
	DDX_Control(pDX, IDC_STC_POWERSUPPLY_VOLT, m_stPowerSupplyInfo[1]);
	

	DDX_Control(pDX, IDC_STC_YIELD_TITLE_1, m_stMainYield_Title);
	DDX_Control(pDX, IDC_STC_BARCODE_TITLE, m_stMainBarcode_Title);
	DDX_Control(pDX, IDC_STC_BARCODE_TITLE2, m_stMainBarcode_Title2);
	DDX_Control(pDX, IDC_STC_BARCODE_TITLE3, m_stMainBarcode_Title3);

	DDX_Control(pDX, IDC_STC_LIGHT_VOLT, m_stFN300Light_Title);

	DDX_Control(pDX, IDC_STC_YIELD_SUB_TITLE_1, m_stMainYield_Title_Sub[0][0]);
	DDX_Control(pDX, IDC_STC_YIELD_SUB_TITLE_2, m_stMainYield_Title_Sub[1][0]);
	DDX_Control(pDX, IDC_STC_YIELD_SUB_TITLE_3, m_stMainYield_Title_Sub[2][0]);
	DDX_Control(pDX, IDC_STC_YIELD_SUB_TITLE_4, m_stMainYield_Title_Sub[3][0]);
	DDX_Control(pDX, IDC_STC_YIELD_COUNT_1, m_stMainYield_Title_Sub[0][1]);
	DDX_Control(pDX, IDC_STC_YIELD_COUNT_2, m_stMainYield_Title_Sub[1][1]);
	DDX_Control(pDX, IDC_STC_YIELD_COUNT_3, m_stMainYield_Title_Sub[2][1]);
	DDX_Control(pDX, IDC_STC_YIELD_COUNT_4, m_stMainYield_Title_Sub[3][1]);

	DDX_Control(pDX, IDC_STC_FN300_L_1, m_stFN300Light_Status[0]);
	DDX_Control(pDX, IDC_STC_FN300_L_2, m_stFN300Light_Status[1]);

	DDX_Control(pDX, IDC_BTN_YIELD_RESET, m_btnMainYield_Reset);

	DDX_Control(pDX, IDC_BTN_STARTUP_MODE1, m_btnMainStartUpMode[0]);
	DDX_Control(pDX, IDC_BTN_STARTUP_MODE2, m_btnMainStartUpMode[1]);
	DDX_Control(pDX, IDC_BTN_STARTUP_MODE3, m_btnMainStartUpMode[2]);
	DDX_Control(pDX, IDC_BTN_STARTUP_MODE4, m_btnMainStartUpMode[3]);

	DDX_Control(pDX, IDC_BTN_STARTUP_MODE5, m_btnMainStartUpMode[4]);
	DDX_Control(pDX, IDC_BTN_STARTUP_MODE6, m_btnMainStartUpMode[5]);
	DDX_Control(pDX, IDC_BTN_STARTUP_MODE7, m_btnMainStartUpMode[6]);
	DDX_Control(pDX, IDC_BTN_STARTUP_MODE8, m_btnMainStartUpMode[7]);
	
	DDX_Control(pDX, IDC_BUTTON_FN300_L1_ON, m_btnMainFN300Light[0]);
	DDX_Control(pDX, IDC_BUTTON_FN300_L2_ON, m_btnMainFN300Light[1]);

	for( int i =0 ; i<19; i++)
		DDX_Control(pDX, IDC_STC_UI_SHOW_STATUS1+i, m_stMainUI_StepView[i]);

	DDX_Control(pDX, IDC_STC_UI_STAGE_1, m_stMainUI_ZoneView[0]);
	DDX_Control(pDX, IDC_STC_UI_STAGE_2, m_stMainUI_ZoneView[1]);
	DDX_Control(pDX, IDC_STC_UI_STAGE_3, m_stMainUI_ZoneView[2]);

	DDX_Control(pDX, IDC_STC_UI_MOVINGFLOW1, m_stMainUI_MovingView[0]);
	DDX_Control(pDX, IDC_STC_UI_MOVINGFLOW2, m_stMainUI_MovingView[1]);
	DDX_Control(pDX, IDC_STC_UI_MOVINGFLOW3, m_stMainUI_MovingView[2]);
	DDX_Control(pDX, IDC_STC_UI_MOVINGFLOW4, m_stMainUI_MovingView[3]);

	DDX_Control(pDX, IDC_MAIN_EDIT_SF1, m_edtFileName[0]);
	DDX_Control(pDX, IDC_MAIN_EDIT_SF2, m_edtFileName[1]);
	DDX_Control(pDX, IDC_MAIN_EDIT_SF3, m_edtFileName[2]);
	DDX_Control(pDX, IDC_MAIN_EDIT_SF4, m_edtFileName[3]);
	DDX_Control(pDX, IDC_MAIN_STATIC_SF1, m_sttFileProc[0]);
	DDX_Control(pDX, IDC_MAIN_STATIC_SF2, m_sttFileProc[1]);
	DDX_Control(pDX, IDC_MAIN_STATIC_SF3, m_sttFileProc[2]);
	DDX_Control(pDX, IDC_MAIN_STATIC_SF4, m_sttFileProc[3]);

	DDX_Control(pDX, IDC_STATIC_TESTER_MAIN_1, m_stFileName[0]);
	DDX_Control(pDX, IDC_STATIC_TESTER_MAIN_2, m_stFileName[1]);
	DDX_Control(pDX, IDC_STATIC_TESTER_MAIN_3, m_stFileName[2]);
	DDX_Control(pDX, IDC_STATIC_TESTER_MAIN_4, m_stFileName[3]);

	DDX_Control(pDX, IDC_STATIC_SW_FIND, m_stComponent_Title);
	DDX_Control(pDX, IDC_STATIC_SW_FIND2, m_stComponent_subTitle[0]);
	DDX_Control(pDX, IDC_STATIC_SW_FIND3, m_stComponent_subTitle[1]);
}


BEGIN_MESSAGE_MAP(CDlgMain, CDialogEx)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgMain::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDlgMain::OnBnClickedButton2)
	
	ON_MESSAGE(MESSAGE_UI_CHANGE_HOME, &CDlgMain::SetMainUIStatusChange_Home)
	ON_MESSAGE(MESSAGE_UI_CHANGE_LIMIT, &CDlgMain::SetMainUIStatusChange_Limit)
	ON_MESSAGE(MESSAGE_UI_CHANGE_ALARM, &CDlgMain::SetMainUIStatusChange_AxisAlarm)
	ON_MESSAGE(MESSAGE_UI_CHANGE_INMOTION, &CDlgMain::SetMainUIStatusChange_InMotion)
	ON_BN_CLICKED(IDC_BTN_YIELD_RESET, &CDlgMain::OnBnClickedBtnYieldReset)
	ON_BN_CLICKED(IDC_BUTTON3, &CDlgMain::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CDlgMain::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON7, &CDlgMain::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &CDlgMain::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON5, &CDlgMain::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON9, &CDlgMain::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON10, &CDlgMain::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON11, &CDlgMain::OnBnClickedButton11)
	ON_BN_CLICKED(IDC_BUTTON12, &CDlgMain::OnBnClickedButton12)
	ON_STN_DBLCLK(IDC_STC_LOG_MAIN, &CDlgMain::OnStnDblclickStcLogMain)
	ON_STN_DBLCLK(IDC_STC_LOG_COMM, &CDlgMain::OnStnDblclickStcLogComm)
	ON_STN_DBLCLK(IDC_STC_LOG_SEQ, &CDlgMain::OnStnDblclickStcLogSeq)
	ON_BN_CLICKED(IDC_BUTTON_CLEARLOG1, &CDlgMain::OnBnClickedButtonClearlog1)
	ON_BN_CLICKED(IDC_BUTTON_CLEARLOG2, &CDlgMain::OnBnClickedButtonClearlog2)
	ON_BN_CLICKED(IDC_BUTTON_CLEARLOG3, &CDlgMain::OnBnClickedButtonClearlog3)
	ON_BN_CLICKED(IDC_BTN_STARTUP_MODE1, &CDlgMain::OnBnClickedBtnStartupMode1)
	ON_BN_CLICKED(IDC_BTN_STARTUP_MODE2, &CDlgMain::OnBnClickedBtnStartupMode2)
	ON_BN_CLICKED(IDC_BTN_STARTUP_MODE3, &CDlgMain::OnBnClickedBtnStartupMode3)
	ON_BN_CLICKED(IDC_BUTTON_FN300_L1_ON, &CDlgMain::OnBnClickedButtonFn300_L1)
	ON_BN_CLICKED(IDC_BUTTON_FN300_L2_ON, &CDlgMain::OnBnClickedButtonFn300_L2)
	//ON_BN_CLICKED(IDC_BUTTON_PROC_FIND, &CDlgMain::OnBnClickedBtnProcFind)
	ON_BN_CLICKED(IDC_BTN_MAIN_DOOR_LOCK, &CDlgMain::OnBnClickedBtnDoorLock)
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BTN_STARTUP_MODE4, &CDlgMain::OnBnClickedBtnStartupMode4)
	ON_BN_CLICKED(IDC_BTN_STARTUP_MODE5, &CDlgMain::OnBnClickedBtnStartupMode5)
	ON_BN_CLICKED(IDC_BTN_STARTUP_MODE6, &CDlgMain::OnBnClickedBtnStartupMode6)
	ON_BN_CLICKED(IDC_BTN_STARTUP_MODE7, &CDlgMain::OnBnClickedBtnStartupMode7)
	ON_BN_CLICKED(IDC_BTN_STARTUP_MODE8, &CDlgMain::OnBnClickedBtnStartupMode8)
END_MESSAGE_MAP()

BOOL CDlgMain::PreTranslateMessage(MSG* pMsg)
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

void CDlgMain::OnLButtonDown(UINT nFlags, CPoint point)
{
	// [21.1021.3]
	CLuriMotion_OQA_SFR_WideDlg *pParrentDlg = (CLuriMotion_OQA_SFR_WideDlg*)AfxGetApp()->GetMainWnd();
	pParrentDlg->SendMessage(MESSAGE_UI_WINDOW_TOP, NULL, NULL);

	CDialogEx::OnLButtonDown(nFlags, point);
}

int CDlgMain::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	InitComponent_Main();
	return 0;
}

void CDlgMain::InitUIEqpType()
{
	auto EqpType = GET_INSPECTOR;

	switch (EqpType)
	{
	case SYS_OQC_SFR_MULTI_CL:
		
		break;

	case SYS_OQC_SFR_SINGLE_CL:
		GetDlgItem(IDC_STC_MAIN_MOTOR_STATUS_NUM_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_MOTION_NAME_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_MAIN_CUR_POS_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_MAIN_MOTOR_LIMIT_PLUS_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_MAIN_MOTOR_LIMIT_MINUS_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_MAIN_MOTOR_ERROR_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_MAIN_MOTOR_INMOTION_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_MAIN_HOME_2)->ShowWindow(SW_HIDE);
		break;

	case SYS_DISTORTION_30:
		GetDlgItem(IDC_STC_MAIN_MOTOR_STATUS_NUM_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_MOTION_NAME_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_MAIN_CUR_POS_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_MAIN_MOTOR_LIMIT_PLUS_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_MAIN_MOTOR_LIMIT_MINUS_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_MAIN_MOTOR_ERROR_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_MAIN_MOTOR_INMOTION_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_MAIN_HOME_2)->ShowWindow(SW_HIDE);

		/*GetDlgItem(IDC_STC_MAIN_MOTOR_SUBTITLE_1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_MAIN_MOTOR_SUBTITLE_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_MAIN_MOTOR_SUBTITLE_3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_MAIN_MOTOR_SUBTITLE_4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_MAIN_MOTOR_SUBTITLE_5)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_MAIN_MOTOR_SUBTITLE_6)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_MAIN_MOTOR_SUBTITLE_7)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_MAIN_MOTOR_STATUS_NUM_1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_MOTION_NAME_1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_MAIN_CUR_POS_1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_MAIN_MOTOR_LIMIT_PLUS_1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_MAIN_MOTOR_LIMIT_MINUS_1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_MAIN_MOTOR_ERROR_1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_MAIN_MOTOR_INMOTION_1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_MAIN_HOME_1)->ShowWindow(SW_HIDE);
		

		GetDlgItem(IDC_STC_MAIN_MOTOR_STATUS_NUM_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_MOTION_NAME_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_MAIN_CUR_POS_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_MAIN_MOTOR_LIMIT_PLUS_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_MAIN_MOTOR_LIMIT_MINUS_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_MAIN_MOTOR_ERROR_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_MAIN_MOTOR_INMOTION_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_MAIN_HOME_2)->ShowWindow(SW_HIDE);*/
		break;

	default:
		break;
	}
}

BOOL CDlgMain::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CRect rect;
	GetClientRect(rect);
	//pDC->FillSolidRect(rect, RGB(62, 94, 136));
	pDC->FillSolidRect(rect, RGB(83, 169, 155));
	//pDC->FillSolidRect(rect, RGB(112, 146, 190));		
	return TRUE;
}

BOOL CDlgMain::ShowError(__in enMCErrCode err)
{
	CLuriMotion_OQA_SFR_WideDlg *pParrentDlg = (CLuriMotion_OQA_SFR_WideDlg*)AfxGetApp()->GetMainWnd();
	EQ_BASIC_PARAM	stEq = *DATAMANAGER->GetCurrentEqBasicData();

	CString sz;

	sz.Format(_T("Error : %s"), COMMON->GetErrorString(err));
	WRITE_SEQ_LOG(sz);

	sz += _T("\r\n");
	if (pParrentDlg->RunMessageDlg_Domodal(sz, MODE_ERROR) == TRUE)
		return TRUE;

	return FALSE;
}

void CDlgMain::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);
	BOOL b;
	RUN_MODE rMode;
	if (bShow == TRUE)
	{
		int bState = COMMON->GetFlagStartUpMode() == TRUE ? SW_SHOW : SW_HIDE;

		if (COMMON->GetFlagStartUpMode() == TRUE)
			ShowStartUp();
		else
			HideStartUp();

		/*for(auto i = 0; i<_countof(m_btnMainStartUpMode); i++)
			m_btnMainStartUpMode[i].ShowWindow(bState);*/

		SetTimer(MAIN_TIMER_MOTOR_STATUS, 250, NULL);
		SetTimer(MAIN_TIMER_POWERSUPPLY, 2000, NULL);
		SetTimer(1234, 250, NULL);

		rMode = SEQUENCE->GetRunMode();
		
		EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();
		//if (stEq.nLightSelected == 0)	// 180
		//{
		//	SendMessage(WM_COMMAND, IDC_BUTTON_FN300_L1_ON, 0);
		//}
		//else // 82.5
		//{
		//	SendMessage(WM_COMMAND, IDC_BUTTON_FN300_L2_ON, 0);
		//}

		TESTER_COMM	stTester = *DATAMANAGER->GetCurrentTesterCommData();
		
// 		for (int i = 0; i < 4; i++)
// 		{
// 			if (nSelAngle == 0) {
// 				m_stFileName[i].SetWindowText(stTester.strName[i]);
// 				m_edtFileName[i].SetWindowText(stTester.strName[i]);
// 			}
// 			else {
// 				m_stFileName[i].SetWindowText(stTester.strName[i+4]);
// 				m_edtFileName[i].SetWindowText(stTester.strName[i+4]);
// 			}
// 		}

		//SendMessage(WM_COMMAND, IDC_BUTTON_PROC_FIND, 0);
	}
	else
	{
		KillTimer(MAIN_TIMER_MOTOR_STATUS);
	}
}

void CDlgMain::ChangeDectectedUI_Status()
{
	BOOL b1 = FALSE;
	BOOL b2 = FALSE;
	BOOL b3 = FALSE;

	//전면
#if(GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL) || (GET_INSPECTOR == SYS_OQC_SFR_SINGLE_CL) || (GET_INSPECTOR == SYS_DISTORTION_30)
	if (AJIN_IO->GetInputIoBit(DIO_INPUT_X0_10_FRONT_DOORLOCK_OPENCLOSE) == FALSE)
	{
		m_stMainComm_IO_Input_Status[0][1].SetStaticStyle(CVGStatic::StaticStyle_Data);
		m_stMainComm_IO_Input_Status[0][1].SetColorStyle(CVGStatic::ColorStyle_Green);
		m_stMainComm_IO_Input_Status[0][1].SetFont_Gdip(L"Arial", 9.0F);
	}
	else
	{
		m_stMainComm_IO_Input_Status[0][1].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stMainComm_IO_Input_Status[0][1].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_stMainComm_IO_Input_Status[0][1].SetFont_Gdip(L"Arial", 9.0F);
	}
#endif

	//좌측
#if(GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL)
	b1 = AJIN_IO->GetInputIoBit(DIO_INPUT_X0_11_LEFT_DOORLOCK1_OPENCLOSE);
	b2 = AJIN_IO->GetInputIoBit(DIO_INPUT_X0_12_LEFT_DOORLOCK2_OPENCLOSE);
#elif(GET_INSPECTOR == SYS_OQC_SFR_SINGLE_CL)
	b1 = AJIN_IO->GetInputIoBit(DIO_INPUT_X0_11_LEFT_DOORLOCK1_OPENCLOSE);
#elif(GET_INSPECTOR == SYS_DISTORTION_30)
	b1 = AJIN_IO->GetInputIoBit(DIO_INPUT_X0_11_LEFT_DOORLOCK1_OPENCLOSE);
	b2 = AJIN_IO->GetInputIoBit(DIO_INPUT_X0_12_LEFT_DOORLOCK2_OPENCLOSE);
#endif

	if (b1 == FALSE && b2 == FALSE)
	{
		m_stMainComm_IO_Input_Status[1][1].SetStaticStyle(CVGStatic::StaticStyle_Data);
		m_stMainComm_IO_Input_Status[1][1].SetColorStyle(CVGStatic::ColorStyle_Green);
		m_stMainComm_IO_Input_Status[1][1].SetFont_Gdip(L"Arial", 9.0F);
	}
	else
	{
		m_stMainComm_IO_Input_Status[1][1].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stMainComm_IO_Input_Status[1][1].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_stMainComm_IO_Input_Status[1][1].SetFont_Gdip(L"Arial", 9.0F);
	}

	//후면
#if(GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL)
	b1 = AJIN_IO->GetInputIoBit(DIO_INPUT_X0_15_BACK_DOORLOCK1_OPENCLOSE);
	b2 = AJIN_IO->GetInputIoBit(DIO_INPUT_X0_16_BACK_DOORLOCK2_OPENCLOSE);
#elif(GET_INSPECTOR == SYS_OQC_SFR_SINGLE_CL)
	b1 = AJIN_IO->GetInputIoBit(DIO_INPUT_X0_13_BACK_DOORLOCK1_OPENCLOSE);
	b2 = AJIN_IO->GetInputIoBit(DIO_INPUT_X0_14_BACK_DOORLOCK2_OPENCLOSE);
#elif(GET_INSPECTOR == SYS_DISTORTION_30)
	b1 = AJIN_IO->GetInputIoBit(DIO_INPUT_X0_15_BACK_DOORLOCK1_OPENCLOSE);
	b2 = AJIN_IO->GetInputIoBit(DIO_INPUT_X0_16_BACK_DOORLOCK2_OPENCLOSE);
#elif(GET_INSPECTOR == SYS_FILM_PEEL_OFF)
	b1 = AJIN_IO->GetInputIoBit(DIO_INPUT_X0_16_REAR_DOORLOCK1_OPENCLOSE);
	b2 = AJIN_IO->GetInputIoBit(DIO_INPUT_X0_17_REAR_DOORLOCK2_OPENCLOSE);
	b3 = AJIN_IO->GetInputIoBit(DIO_INPUT_X0_18_REAR_DOORLOCK3_OPENCLOSE);
#endif

#if(GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL) || (GET_INSPECTOR == SYS_OQC_SFR_SINGLE_CL) || (GET_INSPECTOR == SYS_DISTORTION_30)
	if (b1 == FALSE && b2 == FALSE)
#elif(GET_INSPECTOR == SYS_FILM_PEEL_OFF)
	if (b1 == FALSE && b2 == FALSE && b3 == FALSE)
	{
		m_stMainComm_IO_Output_Status[0][1].SetStaticStyle(CVGStatic::StaticStyle_Data);
		m_stMainComm_IO_Output_Status[0][1].SetColorStyle(CVGStatic::ColorStyle_Green);
		m_stMainComm_IO_Output_Status[0][1].SetFont_Gdip(L"Arial", 9.0F);
	}
	else
	{
		m_stMainComm_IO_Output_Status[0][1].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stMainComm_IO_Output_Status[0][1].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_stMainComm_IO_Output_Status[0][1].SetFont_Gdip(L"Arial", 9.0F);
	}
#endif

	//우측
#if(GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL)
	b1 = AJIN_IO->GetInputIoBit(DIO_INPUT_X0_15_BACK_DOORLOCK1_OPENCLOSE);
	b2 = AJIN_IO->GetInputIoBit(DIO_INPUT_X0_16_BACK_DOORLOCK2_OPENCLOSE);
#elif(GET_INSPECTOR == SYS_OQC_SFR_SINGLE_CL)
	b1 = AJIN_IO->GetInputIoBit(DIO_INPUT_X0_12_RIGHT_DOORLOCK1_OPENCLOSE);
#elif(GET_INSPECTOR == SYS_DISTORTION_30)
	b1 = AJIN_IO->GetInputIoBit(DIO_INPUT_X0_13_RIGHT_DOORLOCK1_OPENCLOSE);
	b2 = AJIN_IO->GetInputIoBit(DIO_INPUT_X0_14_RIGHT_DOORLOCK2_OPENCLOSE);
#endif

	if (b1 == FALSE && b2 == FALSE)
	{
		m_stMainComm_IO_Output_Status[1][1].SetStaticStyle(CVGStatic::StaticStyle_Data);
		m_stMainComm_IO_Output_Status[1][1].SetColorStyle(CVGStatic::ColorStyle_Green);
		m_stMainComm_IO_Output_Status[1][1].SetFont_Gdip(L"Arial", 9.0F);
	}
	else
	{
		m_stMainComm_IO_Output_Status[1][1].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stMainComm_IO_Output_Status[1][1].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_stMainComm_IO_Output_Status[1][1].SetFont_Gdip(L"Arial", 9.0F);
	}

	//EMO
	if (CIO::IO_I_EMO() == TRUE)
	{
		m_stMainComm_IO_Output_Status[2][1].SetStaticStyle(CVGStatic::StaticStyle_Data);
		m_stMainComm_IO_Output_Status[2][1].SetColorStyle(CVGStatic::ColorStyle_Green);
		m_stMainComm_IO_Output_Status[2][1].SetFont_Gdip(L"Arial", 9.0F);
	}
	else
	{
		m_stMainComm_IO_Output_Status[2][1].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stMainComm_IO_Output_Status[2][1].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_stMainComm_IO_Output_Status[2][1].SetFont_Gdip(L"Arial", 9.0F);
	}

	//AREA SENSOR
	if (CIO::IO_I_AREA_SENSOR())
	{
		m_stMainComm_IO_Input_Status[2][1].SetStaticStyle(CVGStatic::StaticStyle_Data);
		m_stMainComm_IO_Input_Status[2][1].SetColorStyle(CVGStatic::ColorStyle_Green);
		m_stMainComm_IO_Input_Status[2][1].SetFont_Gdip(L"Arial", 9.0F);
	}
	else
	{
		m_stMainComm_IO_Input_Status[2][1].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stMainComm_IO_Input_Status[2][1].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_stMainComm_IO_Input_Status[2][1].SetFont_Gdip(L"Arial", 9.0F);
	}
}

void CDlgMain::OnTimer(UINT_PTR nIDEvent)
{
	CDialogEx::OnTimer(nIDEvent);

	switch (nIDEvent)
	{
	case MAIN_TIMER_POWERSUPPLY:
	{
		//PowerSupply Info   JYS 210923 TEST4.
	//	Read_PowerSupply();
	}
	break;

	case MAIN_TIMER_MOTOR_STATUS:
	{
		// Cur Pos
		GetCurrentPosition();
		if (COMMON->GetReplaceFlag() == TRUE)
			ChangeDayProduct();

		// Motor Status
		ReadMotorStatus_Fastech();

		// Comm Status
		MchState_View();

		ChangeDectectedUI_Status();

		//Monitoring.
		RunStatus_Monitoring();

		//findtester
		UpdateSemcoVer();
	}
	break;
	case MAIN_TIMER_TOWERLAMP:
	{
		if (m_bIsTower_Blink == FALSE)
			break;

		EQ_TOWERLAMP stTower = *DATAMANAGER->GetCurrentTowerLampData();

		for (int i = 0; i < 3; i++)
		{
			if (stTower.nTowerLampSetting[m_nCurrentMode][i] == eTowerLamp_Blink)
			{
				if (AJIN_IO->GetOutputIoBit(DIO_OUTPUT_Y0_08_TOWERLAMP_GREEN - i) == TRUE)
				{
					AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_08_TOWERLAMP_GREEN - i, FALSE);
				}
				else
				{
					AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_08_TOWERLAMP_GREEN - i, TRUE);
				}
			}
		}
	}
	break;
	case 1234:
	{
		CLuriMotion_OQA_SFR_WideDlg *pParrentDlg = (CLuriMotion_OQA_SFR_WideDlg*)AfxGetApp()->GetMainWnd();

		if (pParrentDlg->m_bStartCompleate == TRUE)
		{
	//		SEQUENCE->Begin_RunThread();

			//SetTowerLamp(1);
			KillTimer(1234);			
		}
	}
	break;
	}
}

// CDlgMain 메시지 처리기
void CDlgMain::InitComponent_Main()
{
	EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();

	CString strTemp = _T("");

	for (int i = 0; i < 7; i++)
	{
		m_stMainMotor_Title[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stMainMotor_Title[i].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_stMainMotor_Title[i].SetFont_Gdip(L"Arial", 9.0F);

		if (stBasic.nLanguage == 0)
			m_stMainMotor_Title[i].SetText(m_szMainMotorSubTitle_Kor[i]);
		else
			m_stMainMotor_Title[i].SetText(m_szMainMotorSubTitle_Eng[i]);
	}
	for (int i = 0; i < 6; i++)
	{
		strTemp.Format(_T("%d"), i);
		m_stMainMotor_Number[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stMainMotor_Number[i].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_stMainMotor_Number[i].SetFont_Gdip(L"Arial", 9.0F);
		m_stMainMotor_Number[i].SetText(strTemp);
	}
	for (int i = 0; i < 6; i++)
	{
		m_StMainMotor_Name[i].SetStaticStyle(CVGStatic::StaticStyle_Data);
		m_StMainMotor_Name[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_StMainMotor_Name[i].SetFont_Gdip(L"Arial", 9.0F);
		if (stBasic.nLanguage == 0)
			m_StMainMotor_Name[i].SetText(m_szMainMotorSubTitle_Kor[i]);
		else
			m_StMainMotor_Name[i].SetText(m_szMainMotorSubTitle_Eng[i]);
	}

	for (int i = 0; i < 6; i++)
	{
		m_StMainMotor_Name[i].SetStaticStyle(CVGStatic::StaticStyle_Data);
		m_StMainMotor_Name[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_StMainMotor_Name[i].SetFont_Gdip(L"Arial", 9.0F);
		if (stBasic.nLanguage == 0)
			m_StMainMotor_Name[i].SetText(m_szMainMotorName_Kor[i]);
		else
			m_StMainMotor_Name[i].SetText(m_szMainMotorName_Eng[i]);
	}
	for (int i = 0; i < 6; i++)
	{
		m_stMainMotor_Pos[i].SetStaticStyle(CVGStatic::StaticStyle_Data);
		m_stMainMotor_Pos[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_stMainMotor_Pos[i].SetFont_Gdip(L"Arial", 9.0F);
		m_stMainMotor_Pos[i].SetText(_T("0.000"));
	}

	for (int i = 0; i < 6; i++)
	{
		m_stMainMotor_Limit_P[i].SetStaticStyle(CVGStatic::StaticStyle_Data);
		m_stMainMotor_Limit_P[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_stMainMotor_Limit_P[i].SetFont_Gdip(L"Arial", 9.0F);
		m_stMainMotor_Pos[i].SetText(_T(""));
	}
	for (int i = 0; i < 6; i++)
	{
		m_stMainMotor_Limit_M[i].SetStaticStyle(CVGStatic::StaticStyle_Data);
		m_stMainMotor_Limit_M[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_stMainMotor_Limit_M[i].SetFont_Gdip(L"Arial", 9.0F);
		m_stMainMotor_Limit_M[i].SetText(_T(""));
	}
	for (int i = 0; i < 6; i++)
	{
		m_stMainMotor_Error[i].SetStaticStyle(CVGStatic::StaticStyle_Data);
		m_stMainMotor_Error[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_stMainMotor_Error[i].SetFont_Gdip(L"Arial", 9.0F);
		m_stMainMotor_Error[i].SetText(_T(""));
	}
	for (int i = 0; i < 6; i++)
	{
		m_stMainMotor_InMotion[i].SetStaticStyle(CVGStatic::StaticStyle_Data);
		m_stMainMotor_InMotion[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_stMainMotor_InMotion[i].SetFont_Gdip(L"Arial", 9.0F);
		m_stMainMotor_InMotion[i].SetText(_T(""));
	}
	for (int i = 0; i < 6; i++)
	{
		m_stMainMotor_Home[i].SetStaticStyle(CVGStatic::StaticStyle_Data);
		m_stMainMotor_Home[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_stMainMotor_Home[i].SetFont_Gdip(L"Arial", 9.0F);
		m_stMainMotor_Home[i].SetText(_T(""));
	}

	for (int i = 0; i < 2; i++)
	{
		m_stMainComm_Status[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stMainComm_Status[i].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_stMainComm_Status[i].SetFont_Gdip(L"Arial", 9.0F);

		if (stBasic.nLanguage == 0)
		{
			if (i == 0)
				m_stMainComm_Status[i].SetText(_T("연결 확인"));
			else
				m_stMainComm_Status[i].SetText(_T("IO 인터락 확인"));
		}
		else
		{
			if (i == 0)
				m_stMainComm_Status[i].SetText(_T("Comm Status"));
			else
				m_stMainComm_Status[i].SetText(_T("IO Interlock Check"));
		}
	}

	for (int i = 0; i < 2; i++)
	{
		m_stPowerSupplyInfo[i].SetStaticStyle(CVGStatic::StaticStyle_Data);
		m_stPowerSupplyInfo[i].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_stPowerSupplyInfo[i].SetFont_Gdip(L"Arial", 9.0F);

		if (i == 0)
		{
			if (stBasic.nLanguage == 0)
				m_stPowerSupplyInfo[i].SetText(_T("전압 : "));
			else
				m_stPowerSupplyInfo[i].SetText(_T("Volt :"));
		}
		else
		{
			if (stBasic.nLanguage == 0)
				m_stPowerSupplyInfo[i].SetText(_T("전류 :"));
			else
				m_stPowerSupplyInfo[i].SetText(_T("Current :"));
		}
	}

	for (int i = 0; i < 5; i++)
	{
		m_stMainComm_LanSerial[i][0].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stMainComm_LanSerial[i][0].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_stMainComm_LanSerial[i][0].SetFont_Gdip(L"Arial", 9.0F);

		if (stBasic.nLanguage == 0)
			m_stMainComm_LanSerial[i][0].SetText(m_szMainCommName_Kor[i]);
		else
			m_stMainComm_LanSerial[i][0].SetText(m_szMainCommName_Eng[i]);

		m_stMainComm_LanSerial[i][1].SetStaticStyle(CVGStatic::StaticStyle_Data);
		m_stMainComm_LanSerial[i][1].SetColorStyle(CVGStatic::ColorStyle_Yellow);
		m_stMainComm_LanSerial[i][1].SetFont_Gdip(L"Arial", 9.0F);
//		m_stMainComm_LanSerial[i][1].SetText(_T("DisConnect"));
	}

	for (int i = 0; i < 3; i++)
	{
		m_stMainComm_IO_Input_Status[i][0].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stMainComm_IO_Input_Status[i][0].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_stMainComm_IO_Input_Status[i][0].SetFont_Gdip(L"Arial", 9.0F);
		if (stBasic.nLanguage == 0)
			m_stMainComm_IO_Input_Status[i][0].SetText(m_szMainCommIO_Kor[i]);
		else
			m_stMainComm_IO_Input_Status[i][0].SetText(m_szMainCommIO_Eng[i]);

		m_stMainComm_IO_Input_Status[i][1].SetStaticStyle(CVGStatic::StaticStyle_Data);
		m_stMainComm_IO_Input_Status[i][1].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_stMainComm_IO_Input_Status[i][1].SetFont_Gdip(L"Arial", 9.0F);
		m_stMainComm_IO_Input_Status[i][1].SetText(_T(""));
	}

	for (int i = 0; i < 3; i++)
	{
		m_stMainComm_IO_Output_Status[i][0].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stMainComm_IO_Output_Status[i][0].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_stMainComm_IO_Output_Status[i][0].SetFont_Gdip(L"Arial", 9.0F);
		if (stBasic.nLanguage == 0)
			m_stMainComm_IO_Output_Status[i][0].SetText(m_szMainCommIO_Kor[3 + i]);
		else
			m_stMainComm_IO_Output_Status[i][0].SetText(m_szMainCommIO_Eng[3 + i]);

		m_stMainComm_IO_Output_Status[i][1].SetStaticStyle(CVGStatic::StaticStyle_Data);
		m_stMainComm_IO_Output_Status[i][1].SetColorStyle(CVGStatic::ColorStyle_Green);
		m_stMainComm_IO_Output_Status[i][1].SetFont_Gdip(L"Arial", 9.0F);
		m_stMainComm_IO_Output_Status[i][1].SetText(_T(""));
	}

	m_stMainYield_Title.SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
	m_stMainYield_Title.SetColorStyle(CVGStatic::ColorStyle_Black);
	m_stMainYield_Title.SetFont_Gdip(L"Arial", 9.0F);
	if (stBasic.nLanguage == 0)
		m_stMainYield_Title.SetText(_T("일일생산량"));
	else
		m_stMainYield_Title.SetText(_T("Yield"));

	m_btnMainYield_Reset.SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
	m_btnMainYield_Reset.SetColorStyle(CVGStatic::ColorStyle_Black);
	m_btnMainYield_Reset.SetFont_Gdip(L"Arial", 9.0F);
	if (stBasic.nLanguage == 0)
		m_btnMainYield_Reset.SetText(m_szMainYield_Kor[4]);
	else
		m_btnMainYield_Reset.SetText(m_szMainYield_Eng[4]);

	
	for (int i = 0; i < _countof(m_btnMainStartUpMode); i++)
	{
		m_btnMainStartUpMode[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_btnMainStartUpMode[i].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_btnMainStartUpMode[i].SetFont_Gdip(L"Arial", 9.0F);
		if (stBasic.nLanguage == 0)
			m_btnMainStartUpMode[i].SetText(m_szMainStartUpMode_Kor[i]);
		else
			m_btnMainStartUpMode[i].SetText(m_szMainStartUpMode_Eng[i]);
	}


	for (int i = 0; i < 4; i++)
	{
		m_stMainYield_Title_Sub[i][0].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stMainYield_Title_Sub[i][0].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_stMainYield_Title_Sub[i][0].SetFont_Gdip(L"Arial", 9.0F);
		if (stBasic.nLanguage == 0)
			m_stMainYield_Title_Sub[i][0].SetText(m_szMainYield_Kor[i]);
		else
			m_stMainYield_Title_Sub[i][0].SetText(m_szMainYield_Eng[i]);

		m_stMainYield_Title_Sub[i][1].SetStaticStyle(CVGStatic::StaticStyle_Data);
		m_stMainYield_Title_Sub[i][1].SetColorStyle(CVGStatic::ColorStyle_Green);
		m_stMainYield_Title_Sub[i][1].SetFont_Gdip(L"Arial", 9.0F);
		m_stMainYield_Title_Sub[i][1].SetText(_T(""));
	}

	m_stMainBarcode_Title.SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
	m_stMainBarcode_Title.SetColorStyle(CVGStatic::ColorStyle_Black);
	m_stMainBarcode_Title.SetFont_Gdip(L"Arial", 9.0F);
	if (stBasic.nLanguage == 0)
		m_stMainBarcode_Title.SetText(_T("바코드(Barcode)"));
	else
		m_stMainBarcode_Title.SetText(_T("Barcode"));

	m_stMainBarcode_Title2.SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
	m_stMainBarcode_Title2.SetColorStyle(CVGStatic::ColorStyle_Black);
	m_stMainBarcode_Title2.SetFont_Gdip(L"Arial", 9.0F);
	if (stBasic.nLanguage == 0)
		m_stMainBarcode_Title2.SetText(_T("검사프로그램 메뉴얼 통신"));
	else
		m_stMainBarcode_Title2.SetText(_T("Tester Manual Communication"));

	m_stMainBarcode_Title3.SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
	m_stMainBarcode_Title3.SetColorStyle(CVGStatic::ColorStyle_Black);
	m_stMainBarcode_Title3.SetFont_Gdip(L"Arial", 9.0F);
	if (stBasic.nLanguage == 0)
		m_stMainBarcode_Title3.SetText(_T("파워 서플라이 메뉴얼"));
	else
		m_stMainBarcode_Title3.SetText(_T("Power Supply Manual"));

	m_stFN300Light_Title.SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
	m_stFN300Light_Title.SetColorStyle(CVGStatic::ColorStyle_Black);
	m_stFN300Light_Title.SetFont_Gdip(L"Arial", 9.0F);
	if (stBasic.nLanguage == 0)
		m_stFN300Light_Title.SetText(_T("멀티 조명"));
	else
		m_stFN300Light_Title.SetText(_T("Multi Light"));
	// #.#
	for (int i = 0; i < 2; i++)
	{
		m_stFN300Light_Status[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stFN300Light_Status[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
	}

	for (int i = 0; i < 3; i++)
	{
		m_stMainUI_ZoneView[i].SetStaticStyle(CVGStatic::StaticStyle_Data);
		m_stMainUI_ZoneView[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_stMainUI_ZoneView[i].SetFont_Gdip(L"Arial", 9.0F);
		if (stBasic.nLanguage == 0)
			m_stMainUI_ZoneView[i].SetText(m_szMainStageFlow_Kor[i]);
		else
			m_stMainUI_ZoneView[i].SetText(m_szMainStageFlow_Eng[i]);
	}

	for (int i = 0; i < 19; i++)
	{
		m_stMainUI_StepView[i].SetStaticStyle(CVGStatic::StaticStyle_Data);
		m_stMainUI_StepView[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_stMainUI_StepView[i].SetFont_Gdip(L"Arial", 9.0F);
		if (stBasic.nLanguage == 0)
			m_stMainUI_StepView[i].SetText(m_szMainMonitoring_Kor[i]);
		else
			m_stMainUI_StepView[i].SetText(m_szMainMonitoring_Eng[i]);
	}

	for (int i = 0; i < 4; i++)
	{
		m_stMainUI_MovingView[i].SetStaticStyle(CVGStatic::StaticStyle_Data);
		m_stMainUI_MovingView[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_stMainUI_MovingView[i].SetFont_Gdip(L"Arial", 9.0F);
		if (stBasic.nLanguage == 0)
			m_stMainUI_MovingView[i].SetText(m_szMainMovingFlow_Kor[i]);
		else
			m_stMainUI_MovingView[i].SetText(m_szMainMovingFlow_Eng[i]);
	}

	for (int i = 0; i < 4; i++)
	{
		m_sttFileProc[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_sttFileProc[i].SetColorStyle(CVGStatic::ColorStyle_SlateGray);
	}
	m_stComponent_Title.SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
	m_stComponent_Title.SetColorStyle(CVGStatic::ColorStyle_Black);
	m_stComponent_Title.SetFont_Gdip(L"Arial", 9.0F);
	m_stComponent_Title.SetText(_T("S/W Find"));
	for (int i = 0; i < 2; i++) {
		m_stComponent_subTitle[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stComponent_subTitle[i].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_stComponent_subTitle[i].SetFont_Gdip(L"Arial", 9.0F);
		if (i == 0) {
#if (GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL)
			m_stComponent_subTitle[i].SetText(_T("180"));
#elif (GET_INSPECTOR == SYS_OQC_SFR_SINGLE_CL)
			m_stComponent_subTitle[i].SetText(_T("30"));
#endif
		}
		else {
			m_stComponent_subTitle[i].SetText(_T("83"));
		}
	}

}
void CDlgMain::MchState_View()
{
	EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();
	CString strTemp = _T("");

	// [21.1021.10] 테스터연결 다시 확인
	HWND hTester = ::FindWindow(_T("SENSORTEST30_32770"), NULL);
	if (hTester) {
		if (m_UdpTester[0].m_bConnected == FALSE)
		{
			if (m_lCount++ == 0)
			{
				m_UdpTester[0].Send_Tester_Communication(eComMode_SwInit);

			}
			else if (m_lCount < 20)
			{
			}
			else
			{
				m_lCount = 0;
			}
		}
	}
	else
	{
		m_UdpTester[0].m_bConnected = FALSE;
		m_lCount = 0;
	}

	for (int i = 0; i < 6; i++)
	{
		if (i == 0)		/// tester 연결상태
		{
			if (m_bMchState_View_1 == FALSE)
			{
				if (m_UdpTester[0].m_bConnected == TRUE)
				{
					m_stMainComm_LanSerial[0][1].SetStaticStyle(CVGStatic::StaticStyle_Data);
					m_stMainComm_LanSerial[0][1].SetColorStyle(CVGStatic::ColorStyle_Green);
					m_stMainComm_LanSerial[0][1].SetText(_T(""));
					m_bMchState_View_1 = TRUE;
				}
			}
			else if (m_bMchState_View_1 == TRUE)
			{
				if (m_UdpTester[0].m_bConnected == FALSE)
				{
					m_stMainComm_LanSerial[0][1].SetStaticStyle(CVGStatic::StaticStyle_Data);
					m_stMainComm_LanSerial[0][1].SetColorStyle(CVGStatic::ColorStyle_Yellow);
					m_stMainComm_LanSerial[0][1].SetText(_T(""));
					m_bMchState_View_1 = FALSE;
				}
			}

			if (m_bChangeUIReplaceFlag == TRUE)
			{
				if (m_UdpTester[0].m_bConnected == TRUE)
				{
					m_stMainComm_LanSerial[0][1].SetStaticStyle(CVGStatic::StaticStyle_Data);
					m_stMainComm_LanSerial[0][1].SetColorStyle(CVGStatic::ColorStyle_Green);
					m_stMainComm_LanSerial[0][1].SetText(_T(""));
					m_bMchState_View_1 = TRUE;
				}
				else
				{
					m_stMainComm_LanSerial[0][1].SetStaticStyle(CVGStatic::StaticStyle_Data);
					m_stMainComm_LanSerial[0][1].SetColorStyle(CVGStatic::ColorStyle_Yellow);
					m_stMainComm_LanSerial[0][1].SetText(_T(""));
					m_bMchState_View_1 = FALSE;
				}
			}
		}
		else if (i == 1)		// 광원 컨트롤 연결상태 
		{
#if(GET_INSPECTOR == SYS_DISTORTION_30)
			if (m_bMchState_View_2 == FALSE)
			{
				if (m_ctrlLightController_Youth_DPS[0].Is_Connected() == TRUE)
				{
					m_stMainComm_LanSerial[1][1].SetStaticStyle(CVGStatic::StaticStyle_Data);
					m_stMainComm_LanSerial[1][1].SetColorStyle(CVGStatic::ColorStyle_Green);
					m_stMainComm_LanSerial[1][1].SetText(_T(""));
					m_bMchState_View_2 = TRUE;
				}
			}
			else if (m_bMchState_View_2 == TRUE)
			{
				if (m_ctrlLightController_Youth_DPS[0].Is_Connected() == FALSE)
				{
					m_stMainComm_LanSerial[1][1].SetStaticStyle(CVGStatic::StaticStyle_Data);
					m_stMainComm_LanSerial[1][1].SetColorStyle(CVGStatic::ColorStyle_Yellow);
					m_stMainComm_LanSerial[1][1].SetText(_T(""));
					m_bMchState_View_2 = FALSE;
				}
			}
			if (m_bChangeUIReplaceFlag == TRUE)
			{
				if (m_ctrlLightController_Youth_DPS[0].Is_Connected() == TRUE)
				{
					m_stMainComm_LanSerial[1][1].SetStaticStyle(CVGStatic::StaticStyle_Data);
					m_stMainComm_LanSerial[1][1].SetColorStyle(CVGStatic::ColorStyle_Green);
					m_stMainComm_LanSerial[1][1].SetText(_T(""));
					m_bMchState_View_2 = TRUE;
				}
				else
				{
					m_stMainComm_LanSerial[1][1].SetStaticStyle(CVGStatic::StaticStyle_Data);
					m_stMainComm_LanSerial[1][1].SetColorStyle(CVGStatic::ColorStyle_Yellow);
					m_stMainComm_LanSerial[1][1].SetText(_T(""));
					m_bMchState_View_2 = FALSE;
				}
			}
#else
			if (m_bMchState_View_2 == FALSE)
			{
				if (FAS_MO->IsConnected() == TRUE)
				{
					m_stMainComm_LanSerial[1][1].SetStaticStyle(CVGStatic::StaticStyle_Data);
					m_stMainComm_LanSerial[1][1].SetColorStyle(CVGStatic::ColorStyle_Green);
					m_stMainComm_LanSerial[1][1].SetText(_T(""));
					m_bMchState_View_2 = TRUE;
				}
			}
			else if (m_bMchState_View_2 == TRUE)
			{
				if (FAS_MO->IsConnected() == FALSE)
				{
					m_stMainComm_LanSerial[1][1].SetStaticStyle(CVGStatic::StaticStyle_Data);
					m_stMainComm_LanSerial[1][1].SetColorStyle(CVGStatic::ColorStyle_Yellow);
					m_stMainComm_LanSerial[1][1].SetText(_T(""));
					m_bMchState_View_2 = FALSE;
				}
			}
			if (m_bChangeUIReplaceFlag == TRUE)
			{
				if (FAS_MO->IsConnected() == TRUE)
				{
					m_stMainComm_LanSerial[1][1].SetStaticStyle(CVGStatic::StaticStyle_Data);
					m_stMainComm_LanSerial[1][1].SetColorStyle(CVGStatic::ColorStyle_Green);
					m_stMainComm_LanSerial[1][1].SetText(_T(""));
					m_bMchState_View_2 = TRUE;
				}
				else
				{
					m_stMainComm_LanSerial[1][1].SetStaticStyle(CVGStatic::StaticStyle_Data);
					m_stMainComm_LanSerial[1][1].SetColorStyle(CVGStatic::ColorStyle_Yellow);
					m_stMainComm_LanSerial[1][1].SetText(_T(""));
					m_bMchState_View_2 = FALSE;
				}
			}
#endif			
		}
		else if (i == 2)	// io 카드 연결 상태 
		{
			if (m_bMchState_View_3 == FALSE)
			{
				if (AJIN_IO->m_bInitDIO == TRUE)
				{
					m_stMainComm_LanSerial[2][1].SetStaticStyle(CVGStatic::StaticStyle_Data);
					m_stMainComm_LanSerial[2][1].SetColorStyle(CVGStatic::ColorStyle_Green);
					m_stMainComm_LanSerial[2][1].SetText(_T(""));
					m_bMchState_View_3 = TRUE;
				}
			}
			else if (m_bMchState_View_3 == TRUE)
			{
				if (AJIN_IO->m_bInitDIO == FALSE)
				{
					m_stMainComm_LanSerial[2][1].SetStaticStyle(CVGStatic::StaticStyle_Data);
					m_stMainComm_LanSerial[2][1].SetColorStyle(CVGStatic::ColorStyle_Yellow);
					m_stMainComm_LanSerial[2][1].SetText(_T(""));
					m_bMchState_View_3 = FALSE;
				}
			}

			if (m_bChangeUIReplaceFlag == TRUE)
			{
				if (AJIN_IO->m_bInitDIO == TRUE || m_bChangeUIReplaceFlag == TRUE)
				{
					m_stMainComm_LanSerial[2][1].SetStaticStyle(CVGStatic::StaticStyle_Data);
					m_stMainComm_LanSerial[2][1].SetColorStyle(CVGStatic::ColorStyle_Green);
					m_stMainComm_LanSerial[2][1].SetText(_T(""));
					m_bMchState_View_3 = TRUE;
				}
				else
				{
					m_stMainComm_LanSerial[2][1].SetStaticStyle(CVGStatic::StaticStyle_Data);
					m_stMainComm_LanSerial[2][1].SetColorStyle(CVGStatic::ColorStyle_Yellow);
					m_stMainComm_LanSerial[2][1].SetText(_T(""));
					m_bMchState_View_3 = FALSE;
				}
			}
		}
		/// POWER SUPPLY_220830
		else if (i == 3)		// 파워 서플라이 연결상태 
		{
			if (m_bMchState_View_4 == FALSE)
			{
		//		if (m_ctrlLight_ODA_Q[0].Is_Connected())
				if (CDevice::IsConnected_Power())
				{
					m_stMainComm_LanSerial[3][1].SetStaticStyle(CVGStatic::StaticStyle_Data);
					m_stMainComm_LanSerial[3][1].SetColorStyle(CVGStatic::ColorStyle_Green);
					m_stMainComm_LanSerial[3][1].SetText(_T(""));
					m_bMchState_View_4 = TRUE;
				}
			}
			else if (m_bMchState_View_4 == TRUE)
			{
		//		if (m_ctrlLight_ODA_Q[0].Is_Connected() == FALSE)
				if (CDevice::IsConnected_Power() == FALSE)
				{
					m_stMainComm_LanSerial[3][1].SetStaticStyle(CVGStatic::StaticStyle_Data);
					m_stMainComm_LanSerial[3][1].SetColorStyle(CVGStatic::ColorStyle_Yellow);
					m_stMainComm_LanSerial[3][1].SetText(_T(""));
					m_bMchState_View_4 = FALSE;
				}
			}

			if (m_bChangeUIReplaceFlag == TRUE)
			{
			//	if (m_ctrlLight_ODA_Q[0].Is_Connected())
				if (CDevice::IsConnected_Power())
				{
					m_stMainComm_LanSerial[3][1].SetStaticStyle(CVGStatic::StaticStyle_Data);
					m_stMainComm_LanSerial[3][1].SetColorStyle(CVGStatic::ColorStyle_Green);
					m_stMainComm_LanSerial[3][1].SetText(_T(""));
					m_bMchState_View_4 = TRUE;
				}
				else
				{
					m_stMainComm_LanSerial[3][1].SetStaticStyle(CVGStatic::StaticStyle_Data);
					m_stMainComm_LanSerial[3][1].SetColorStyle(CVGStatic::ColorStyle_Yellow);
					m_stMainComm_LanSerial[3][1].SetText(_T(""));
					m_bMchState_View_4 = FALSE;
				}
			}
		}
		else if (i == 4)		// 핸디바코드 연결상태 
		{
			if (m_bMchState_View_5 == FALSE)
			{
				if (CDevice::IsConnected_HandyBCR())
				{
					m_stMainComm_LanSerial[4][1].SetStaticStyle(CVGStatic::StaticStyle_Data);
					m_stMainComm_LanSerial[4][1].SetColorStyle(CVGStatic::ColorStyle_Green);
					m_stMainComm_LanSerial[4][1].SetText(_T(""));
					m_bMchState_View_5 = TRUE;
				}
			}
			else if (m_bMchState_View_5 == TRUE)
			{
				if (CDevice::IsConnected_HandyBCR() == FALSE)
				{
					m_stMainComm_LanSerial[4][1].SetStaticStyle(CVGStatic::StaticStyle_Data);
					m_stMainComm_LanSerial[4][1].SetColorStyle(CVGStatic::ColorStyle_Yellow);
					m_stMainComm_LanSerial[4][1].SetText(_T(""));
					m_bMchState_View_5 = FALSE;
				}
			}

			if (m_bChangeUIReplaceFlag == TRUE)
			{
				if (CDevice::IsConnected_HandyBCR())
				{
					m_stMainComm_LanSerial[4][1].SetStaticStyle(CVGStatic::StaticStyle_Data);
					m_stMainComm_LanSerial[4][1].SetColorStyle(CVGStatic::ColorStyle_Green);
					m_stMainComm_LanSerial[4][1].SetText(_T(""));
					m_bMchState_View_5 = TRUE;
				}
				else
				{
					m_stMainComm_LanSerial[4][1].SetStaticStyle(CVGStatic::StaticStyle_Data);
					m_stMainComm_LanSerial[4][1].SetColorStyle(CVGStatic::ColorStyle_Yellow);
					m_stMainComm_LanSerial[4][1].SetText(_T(""));
					m_bMchState_View_5 = FALSE;
				}
			}
		}
	}
	m_bChangeUIReplaceFlag = FALSE;
}

void CDlgMain::ChangeUI_Language()
{
	EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();

	CString strTemp = _T("");

	for (int i = 0; i < 7; i++)
	{
		m_stMainMotor_Title[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stMainMotor_Title[i].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_stMainMotor_Title[i].SetFont_Gdip(L"Arial", 9.0F);

		if(stBasic.nLanguage == 0)
			m_stMainMotor_Title[i].SetText(m_szMainMotorSubTitle_Kor[i]);
		else
			m_stMainMotor_Title[i].SetText(m_szMainMotorSubTitle_Eng[i]);
	}
	for (int i = 0; i < 6; i++)
	{
		strTemp.Format(_T("%d"), i);
		m_stMainMotor_Number[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stMainMotor_Number[i].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_stMainMotor_Number[i].SetFont_Gdip(L"Arial", 9.0F);
		m_stMainMotor_Number[i].SetText(strTemp);
	}
	for (int i = 0; i < 6; i++)
	{
		m_StMainMotor_Name[i].SetStaticStyle(CVGStatic::StaticStyle_Data);
		m_StMainMotor_Name[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_StMainMotor_Name[i].SetFont_Gdip(L"Arial", 9.0F);
		if (stBasic.nLanguage == 0)
			m_StMainMotor_Name[i].SetText(m_szMainMotorSubTitle_Kor[i]);
		else
			m_StMainMotor_Name[i].SetText(m_szMainMotorSubTitle_Eng[i]);
	}

	for (int i = 0; i < 6; i++)
	{
		m_StMainMotor_Name[i].SetStaticStyle(CVGStatic::StaticStyle_Data);
		m_StMainMotor_Name[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_StMainMotor_Name[i].SetFont_Gdip(L"Arial", 9.0F);
		if (stBasic.nLanguage == 0)
			m_StMainMotor_Name[i].SetText(m_szMainMotorName_Kor[i]);
		else
			m_StMainMotor_Name[i].SetText(m_szMainMotorName_Eng[i]);
	}
	for (int i = 0; i < 6; i++)
	{
		m_stMainMotor_Pos[i].SetStaticStyle(CVGStatic::StaticStyle_Data);
		m_stMainMotor_Pos[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_stMainMotor_Pos[i].SetFont_Gdip(L"Arial", 9.0F);
		m_stMainMotor_Pos[i].SetText(_T("0.000"));
	}

	for (int i = 0; i < 6; i++)
	{
		m_stMainMotor_Limit_P[i].SetStaticStyle(CVGStatic::StaticStyle_Data);
		m_stMainMotor_Limit_P[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_stMainMotor_Limit_P[i].SetFont_Gdip(L"Arial", 9.0F);
		m_stMainMotor_Pos[i].SetText(_T(""));
	}
	for (int i = 0; i < 6; i++)
	{
		m_stMainMotor_Limit_M[i].SetStaticStyle(CVGStatic::StaticStyle_Data);
		m_stMainMotor_Limit_M[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_stMainMotor_Limit_M[i].SetFont_Gdip(L"Arial", 9.0F);
		m_stMainMotor_Limit_M[i].SetText(_T(""));
	}
	for (int i = 0; i < 6; i++)
	{
		m_stMainMotor_Error[i].SetStaticStyle(CVGStatic::StaticStyle_Data);
		m_stMainMotor_Error[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_stMainMotor_Error[i].SetFont_Gdip(L"Arial", 9.0F);
		m_stMainMotor_Error[i].SetText(_T(""));
	}
	for (int i = 0; i < 6; i++)
	{
		m_stMainMotor_InMotion[i].SetStaticStyle(CVGStatic::StaticStyle_Data);
		m_stMainMotor_InMotion[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_stMainMotor_InMotion[i].SetFont_Gdip(L"Arial", 9.0F);
		m_stMainMotor_InMotion[i].SetText(_T(""));
	}
	for (int i = 0; i < 6; i++)
	{
		m_stMainMotor_Home[i].SetStaticStyle(CVGStatic::StaticStyle_Data);
		m_stMainMotor_Home[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_stMainMotor_Home[i].SetFont_Gdip(L"Arial", 9.0F);
		m_stMainMotor_Home[i].SetText(_T(""));
	}

	for (int i = 0; i < 2; i++)
	{
		m_stMainComm_Status[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stMainComm_Status[i].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_stMainComm_Status[i].SetFont_Gdip(L"Arial", 9.0F);

		if (stBasic.nLanguage == 0)
		{
			if (i == 0)
				m_stMainComm_Status[i].SetText(_T("통신상태 확인"));
			else
				m_stMainComm_Status[i].SetText(_T("IO 상태 확인"));
		}
		else
		{
			if (i == 0)
				m_stMainComm_Status[i].SetText(_T("Comm Status"));
			else
				m_stMainComm_Status[i].SetText(_T("IO Status"));
		}
	}

	for (int i = 0; i < 4; i++)
	{
		m_stMainComm_LanSerial[i][0].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stMainComm_LanSerial[i][0].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_stMainComm_LanSerial[i][0].SetFont_Gdip(L"Arial", 9.0F);
		if (stBasic.nLanguage == 0)
			m_stMainComm_LanSerial[i][0].SetText(m_szMainCommName_Kor[i]);
		else
			m_stMainComm_LanSerial[i][0].SetText(m_szMainCommName_Eng[i]);

		m_stMainComm_LanSerial[i][1].SetStaticStyle(CVGStatic::StaticStyle_Data);
		m_stMainComm_LanSerial[i][1].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_stMainComm_LanSerial[i][1].SetFont_Gdip(L"Arial", 9.0F);
		m_stMainComm_LanSerial[i][1].SetText(_T(""));
	}

	for (int i = 0; i < 3; i++)
	{
		m_stMainComm_IO_Input_Status[i][0].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stMainComm_IO_Input_Status[i][0].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_stMainComm_IO_Input_Status[i][0].SetFont_Gdip(L"Arial", 9.0F);
		if (stBasic.nLanguage == 0)
			m_stMainComm_IO_Input_Status[i][0].SetText(m_szMainCommIO_Kor[i]);
		else
			m_stMainComm_IO_Input_Status[i][0].SetText(m_szMainCommIO_Eng[i]);

		m_stMainComm_IO_Input_Status[i][1].SetStaticStyle(CVGStatic::StaticStyle_Data);
		m_stMainComm_IO_Input_Status[i][1].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_stMainComm_IO_Input_Status[i][1].SetFont_Gdip(L"Arial", 9.0F);
		m_stMainComm_IO_Input_Status[i][1].SetText(_T(""));
	}

	for (int i = 0; i < 2; i++)
	{
		m_stPowerSupplyInfo[i].SetStaticStyle(CVGStatic::StaticStyle_Data);
		m_stPowerSupplyInfo[i].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_stPowerSupplyInfo[i].SetFont_Gdip(L"Arial", 9.0F);

		if (i == 0)
		{
			if (stBasic.nLanguage == 0)
				m_stPowerSupplyInfo[i].SetText(_T("전압 : "));
			else
				m_stPowerSupplyInfo[i].SetText(_T("Volt :"));
		}
		else
		{
			if (stBasic.nLanguage == 0)
				m_stPowerSupplyInfo[i].SetText(_T("전류 :"));
			else
				m_stPowerSupplyInfo[i].SetText(_T("Current :"));
		}
	}

	for (int i = 0; i < 3; i++)
	{
		m_stMainComm_IO_Output_Status[i][0].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stMainComm_IO_Output_Status[i][0].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_stMainComm_IO_Output_Status[i][0].SetFont_Gdip(L"Arial", 9.0F);
		if (stBasic.nLanguage == 0)
			m_stMainComm_IO_Output_Status[i][0].SetText(m_szMainCommIO_Kor[3 + i]);
		else
			m_stMainComm_IO_Output_Status[i][0].SetText(m_szMainCommIO_Eng[3 + i]);

		m_stMainComm_IO_Output_Status[i][1].SetStaticStyle(CVGStatic::StaticStyle_Data);
		m_stMainComm_IO_Output_Status[i][1].SetColorStyle(CVGStatic::ColorStyle_Green);
		m_stMainComm_IO_Output_Status[i][1].SetFont_Gdip(L"Arial", 9.0F);
		m_stMainComm_IO_Output_Status[i][1].SetText(_T(""));
	}
	for (int i = 0; i < _countof(m_btnMainStartUpMode); i++)
	{
		m_btnMainStartUpMode[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_btnMainStartUpMode[i].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_btnMainStartUpMode[i].SetFont_Gdip(L"Arial", 9.0F);
		if (stBasic.nLanguage == 0)
			m_btnMainStartUpMode[i].SetText(m_szMainStartUpMode_Kor[i]);
		else
			m_btnMainStartUpMode[i].SetText(m_szMainStartUpMode_Eng[i]);
	}
	m_stMainYield_Title.SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
	m_stMainYield_Title.SetColorStyle(CVGStatic::ColorStyle_Black);
	m_stMainYield_Title.SetFont_Gdip(L"Arial", 9.0F);
	if (stBasic.nLanguage == 0)
		m_stMainYield_Title.SetText(_T("일일생산량"));
	else
		m_stMainYield_Title.SetText(_T("Yield"));

	m_btnMainYield_Reset.SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
	m_btnMainYield_Reset.SetColorStyle(CVGStatic::ColorStyle_Black);
	m_btnMainYield_Reset.SetFont_Gdip(L"Arial", 9.0F);
	if (stBasic.nLanguage == 0)
		m_btnMainYield_Reset.SetText(m_szMainYield_Kor[4]);
	else
		m_btnMainYield_Reset.SetText(m_szMainYield_Eng[4]);

	for (int i = 0; i < 4; i++)
	{
		m_stMainYield_Title_Sub[i][0].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stMainYield_Title_Sub[i][0].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_stMainYield_Title_Sub[i][0].SetFont_Gdip(L"Arial", 9.0F);
		if (stBasic.nLanguage == 0)
			m_stMainYield_Title_Sub[i][0].SetText(m_szMainYield_Kor[i]);
		else
			m_stMainYield_Title_Sub[i][0].SetText(m_szMainYield_Eng[i]);

		m_stMainYield_Title_Sub[i][1].SetStaticStyle(CVGStatic::StaticStyle_Data);
		m_stMainYield_Title_Sub[i][1].SetColorStyle(CVGStatic::ColorStyle_Green);
		m_stMainYield_Title_Sub[i][1].SetFont_Gdip(L"Arial", 9.0F);
		m_stMainYield_Title_Sub[i][1].SetText(_T(""));
	}

	m_stMainBarcode_Title.SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
	m_stMainBarcode_Title.SetColorStyle(CVGStatic::ColorStyle_Black);
	m_stMainBarcode_Title.SetFont_Gdip(L"Arial", 9.0F);
	if (stBasic.nLanguage == 0)
		m_stMainBarcode_Title.SetText(_T("바코드(Barcode)"));
	else
		m_stMainBarcode_Title.SetText(_T("Barcode"));

	for (int i = 0; i < 3; i++)
	{
		m_stMainUI_ZoneView[i].SetStaticStyle(CVGStatic::StaticStyle_Data);
		m_stMainUI_ZoneView[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_stMainUI_ZoneView[i].SetFont_Gdip(L"Arial", 9.0F);
		if (stBasic.nLanguage == 0)
			m_stMainUI_ZoneView[i].SetText(m_szMainStageFlow_Kor[i]);
		else
			m_stMainUI_ZoneView[i].SetText(m_szMainStageFlow_Eng[i]);
	}

	for (int i = 0; i < 19; i++)
	{
		m_stMainUI_StepView[i].SetStaticStyle(CVGStatic::StaticStyle_Data);
		m_stMainUI_StepView[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_stMainUI_StepView[i].SetFont_Gdip(L"Arial", /*7.0F*/9.0F);
		if (stBasic.nLanguage == 0)
			m_stMainUI_StepView[i].SetText(m_szMainMonitoring_Kor[i]);
		else
			m_stMainUI_StepView[i].SetText(m_szMainMonitoring_Eng[i]);
	}

	for (int i = 0; i < 4; i++)
	{
		m_stMainUI_MovingView[i].SetStaticStyle(CVGStatic::StaticStyle_Data);
		m_stMainUI_MovingView[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_stMainUI_MovingView[i].SetFont_Gdip(L"Arial", /*8.0F*/9.0F);
		if (stBasic.nLanguage == 0)
			m_stMainUI_MovingView[i].SetText(m_szMainMovingFlow_Kor[i]);
		else
			m_stMainUI_MovingView[i].SetText(m_szMainMovingFlow_Eng[i]);
	}
	m_stMainBarcode_Title3.SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
	m_stMainBarcode_Title3.SetColorStyle(CVGStatic::ColorStyle_Black);
	m_stMainBarcode_Title3.SetFont_Gdip(L"Arial", 9.0F);
	if (stBasic.nLanguage == 0)
		m_stMainBarcode_Title3.SetText(_T("파워 서플라이 메뉴얼"));
	else
		m_stMainBarcode_Title3.SetText(_T("Power Supply Manual"));

	if (stBasic.nLanguage == 0)
		m_stFN300Light_Title.SetText(_T("FN300 조명"));
	else
		m_stFN300Light_Title.SetText(_T("FN300 Light"));

	m_bChangeUIReplaceFlag = TRUE;
}

void CDlgMain::Read_PowerSupply()
{
	// JYS 210923 TEST4.
	EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();
	CString strTemp = _T("");

	if (m_nPowerSupplyReadMode == 0)
	{
		float fVolt = 0.0f;	
		CDevice::GetMeasure_Volt(fVolt);

		if (stBasic.nLanguage == 0)
		{
			strTemp.Format(_T("전압: %0.2f"), fVolt);
			m_stPowerSupplyInfo[0].SetText(strTemp);
		}
		else
		{
			strTemp.Format(_T("Volt: %0.2f"), fVolt);
			m_stPowerSupplyInfo[0].SetText(strTemp);
		}

		m_nPowerSupplyReadMode = 1;
	}
	else if (m_nPowerSupplyReadMode == 1)
	{
		float fCurrent = 0.0f;

//		ODA_Q->Send_Meas_Current(fCurrent);
		CDevice::GetMeasure_Current(fCurrent);

		if (stBasic.nLanguage == 0)
		{
			strTemp.Format(_T("전류: %0.2f"), fCurrent);
			m_stPowerSupplyInfo[1].SetText(strTemp);
		}
		else
		{
			strTemp.Format(_T("Current: %0.2f"), fCurrent);
			m_stPowerSupplyInfo[1].SetText(strTemp);
		}

		m_nPowerSupplyReadMode = 0;
	}
}

void CDlgMain::RunStatus_Monitoring()
{
	if (COMMON->GetFlagStartUpMode() == TRUE)
		return;
	if (COMMON->m_nMainUiFlag_Refresh == TRUE)
	{
		for (int i = 0; i < 18; i++)
		{
			m_stMainUI_StepView[i].SetStaticStyle(CVGStatic::StaticStyle_Data);
			m_stMainUI_StepView[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		}

		for (int i = 0; i < 3; i++)
		{
			m_stMainUI_ZoneView[i].SetStaticStyle(CVGStatic::StaticStyle_Data);
			m_stMainUI_ZoneView[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		}

		for (int i = 0; i < 4; i++)
		{
			m_stMainUI_MovingView[i].SetStaticStyle(CVGStatic::StaticStyle_Data);
			m_stMainUI_MovingView[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		}

		COMMON->m_nMainUiFlag_Refresh = FALSE;
		return;
	}

	for (int i = 0; i < 19; i++)
	{
		if (COMMON->m_bMainUiFlag_StepView_Old[i] == TRUE)
			continue;

		if (COMMON->m_bMainUiFlag_StepView[i] != COMMON->m_bMainUiFlag_StepView_Old[i])
		{
			if (COMMON->m_bMainUiFlag_StepView[i] == TRUE)
			{
				m_stMainUI_StepView[i].SetStaticStyle(CVGStatic::StaticStyle_Data);
				m_stMainUI_StepView[i].SetColorStyle(CVGStatic::ColorStyle_Green);

				COMMON->m_bMainUiFlag_StepView_Old[i] = TRUE;
			}
		}
	}

	for (int i = 0; i < 3; i++)
	{
		if (COMMON->m_bMainUiFlag_CurrentAction_Old[i] == TRUE)
			continue;

		if (COMMON->GetMainUiFlag_CurrentPos(i) != COMMON->m_bMainUiFlag_CurrentAction_Old[i])
		{
			if (COMMON->GetMainUiFlag_CurrentPos(i) == TRUE)
			{
				m_stMainUI_ZoneView[0].SetStaticStyle(CVGStatic::StaticStyle_Data);
				m_stMainUI_ZoneView[0].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
				m_stMainUI_ZoneView[1].SetStaticStyle(CVGStatic::StaticStyle_Data);
				m_stMainUI_ZoneView[1].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
				m_stMainUI_ZoneView[2].SetStaticStyle(CVGStatic::StaticStyle_Data);
				m_stMainUI_ZoneView[2].SetColorStyle(CVGStatic::ColorStyle_DarkGray);

				m_stMainUI_ZoneView[i].SetStaticStyle(CVGStatic::StaticStyle_Data);
				m_stMainUI_ZoneView[i].SetColorStyle(CVGStatic::ColorStyle_Green);

				COMMON->m_bMainUiFlag_CurrentAction_Old[i] = TRUE;
			}
		}
	}

	for (int i = 0; i < 4; i++)
	{
		if (COMMON->m_bMainUiFlag_MoveSocket_Old[i] == TRUE)
			continue;

		if (COMMON->m_bMainUiFlag_MoveSocket[i] != COMMON->m_bMainUiFlag_MoveSocket_Old[i])
		{
			if (COMMON->m_bMainUiFlag_StepView[i] == TRUE)
			{
				m_stMainUI_MovingView[i].SetStaticStyle(CVGStatic::StaticStyle_Data);
				m_stMainUI_MovingView[i].SetColorStyle(CVGStatic::ColorStyle_Green);

				COMMON->m_bMainUiFlag_MoveSocket_Old[i] = TRUE;
			}
		}
	}
}

BOOL CDlgMain::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CWnd *Wnd = GetDlgItem(IDC_STC_LOG_MAIN);
	CWnd *Wnd2 = GetDlgItem(IDC_STC_LOG_SEQ);
	CWnd *Wnd3 = GetDlgItem(IDC_STC_LOG_COMM);
	CWnd *Wnd4 = GetDlgItem(IDC_STC_LOG_BARCODE);

	WINDOWPLACEMENT wndPlacement;
	WINDOWPLACEMENT wndPlacement2;
	WINDOWPLACEMENT wndPlacement3;
	WINDOWPLACEMENT wndPlacement4;

	//
	m_MainLog.SetWidth(1000);
	m_MainLog.SetMode(0);
	m_MainLog.Create(CDlgLog::IDD, this);

	m_SequenceLog.SetWidth(5300);
	m_SequenceLog.SetMode(1);
	m_SequenceLog.Create(CDlgLog::IDD, this);

	m_CommLog.SetWidth(500);
	m_CommLog.SetMode(2);
	m_CommLog.Create(CDlgLog::IDD, this);

	m_BarcodeLog.SetWidth(300);
	m_BarcodeLog.SetMode(3);
	m_BarcodeLog.Create(CDlgLog::IDD, this);

	Wnd->GetWindowPlacement(&wndPlacement);
	Wnd2->GetWindowPlacement(&wndPlacement2);
	Wnd3->GetWindowPlacement(&wndPlacement3);
	Wnd4->GetWindowPlacement(&wndPlacement4);

	
	m_MainLog.SetWindowPlacement(&wndPlacement);
	m_SequenceLog.SetWindowPlacement(&wndPlacement2);
	m_CommLog.SetWindowPlacement(&wndPlacement3);
	//m_BarcodeLog.SetWindowPlacement(&wndPlacement4);

	m_edtFileName[0].SetFont(&m_font_Normal);
	m_edtFileName[1].SetFont(&m_font_Normal);
	m_edtFileName[2].SetFont(&m_font_Normal);
	m_edtFileName[3].SetFont(&m_font_Normal);

	InitUIEqpType();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgMain::Write_Main_Log(CString strLog)
{
	m_MainLog.ListPrint(strLog);
}
void CDlgMain::Write_Comm_Log(CString strLog)
{
	m_CommLog.ListPrint(strLog);
}
void CDlgMain::Write_Sequence_Log(CString strLog)
{
	m_SequenceLog.ListPrint(strLog);
}
void CDlgMain::Write_Barcode_Log(CString strLog)
{
	m_BarcodeLog.ListPrint(strLog);
}

//---------------------------------------------------------
// tower lamp
//void CDlgMain::SetTowerLamp(int nMode)
//{
//	EQ_TOWERLAMP stTower = *DATAMANAGER->GetCurrentTowerLampData();
//
//	BOOL bIsBlink = FALSE;
//
//	if (m_bIsTower_Blink == TRUE)
//		KillTimer(MAIN_TIMER_TOWERLAMP);
//
//	for (int i = 0; i < 3; i++)
//	{
//		int nTemp = i;
//		if (i == 1) nTemp = i + 17;
//
//		if (stTower.nTowerLampSetting[nMode][i] == eTowerLamp_On)
//			AJIN_IO->SendOutputBit((int)DIO_OUTPUT_Y0_06_TOWERLAMP_RED+ nTemp, TRUE);
//		else if (stTower.nTowerLampSetting[nMode][i] == eTowerLamp_Off)
//			AJIN_IO->SendOutputBit((int)DIO_OUTPUT_Y0_06_TOWERLAMP_RED + nTemp,  FALSE);
//		else
//		{
//			if (stTower.nTowerLampSetting[nMode][i] == eTowerLamp_Blink)
//				bIsBlink = TRUE;
//		}
//		
//	}
//
//	m_bIsTower_Blink = bIsBlink;
//	if (m_bIsTower_Blink == TRUE)
//		SetTimer(MAIN_TIMER_TOWERLAMP, 800, NULL);
//
//	m_nCurrentMode = (int)nMode;
//
//}

//=======================================================================================================
// Message
void CDlgMain::ReadMotorStatus_Fastech()
{
	// Home
	for (int i = 0; i < (int)MOTOR_MAX_COUNT; i++)
	{
		if (m_bMotorHome_Done[i] != (BOOL)FAS_MO->IsHomeOK(i))
		{
			m_bMotorHome_Done[i] = (BOOL)FAS_MO->IsHomeOK(i);
			SendMessage(MESSAGE_UI_CHANGE_HOME, i, m_bMotorHome_Done[i]);
		}
		if (m_bChangeUIReplaceFlag == TRUE)
		{
			for (int i = 0; i < (int)MOTOR_MAX_COUNT; i++) 
			{
				SendMessage(MESSAGE_UI_CHANGE_HOME, i, m_bMotorHome_Done[i]);
				SendMessage(MESSAGE_UI_CHANGE_LIMIT, i, m_bMotorLimit_P[i]);
				SendMessage(MESSAGE_UI_CHANGE_LIMIT, i + 10, m_bMotorLimit_M[i]);
				SendMessage(MESSAGE_UI_CHANGE_ALARM, i, m_bMotorAlarm[i]);
				SendMessage(MESSAGE_UI_CHANGE_INMOTION, i, m_bMotorInMotion[i]);
			}
		}
	}

	//Limit
	for (int i = 0; i < (int)MOTOR_MAX_COUNT; i++)
	{
		if (m_bMotorLimit_P[i] != (BOOL)FAS_MO->IsHWPlusLimitStatus(i))
		{
			m_bMotorLimit_P[i] = (BOOL)FAS_MO->IsHWPlusLimitStatus(i);
			SendMessage(MESSAGE_UI_CHANGE_LIMIT, i, m_bMotorLimit_P[i]);
		}

		if (m_bMotorLimit_M[i] != (BOOL)FAS_MO->IsHWMinusLimitStatus(i))
		{
			m_bMotorLimit_M[i] = (BOOL)FAS_MO->IsHWMinusLimitStatus(i);
			SendMessage(MESSAGE_UI_CHANGE_LIMIT, i + 10, m_bMotorLimit_M[i]);
		}
	}
	// Motor Alarm
	for (int i = 0; i < (int)MOTOR_MAX_COUNT; i++)
	{
		if (m_bMotorAlarm[i] != (BOOL)FAS_MO->IsAlarmStatus(i))
		{
			m_bMotorAlarm[i] = (BOOL)FAS_MO->IsAlarmStatus(i);
			SendMessage(MESSAGE_UI_CHANGE_ALARM, i, m_bMotorAlarm[i]);
		}
	}

	// Inmotion
	for (int i = 0; i < (int)MOTOR_MAX_COUNT; i++)
	{
		if (m_bMotorInMotion[i] != (BOOL)FAS_MO->IsInposStatus(i))
		{
			m_bMotorInMotion[i] = (BOOL)FAS_MO->IsInposStatus(i);
			SendMessage(MESSAGE_UI_CHANGE_INMOTION, i, m_bMotorInMotion[i]);
		}
	}
}

void CDlgMain::ReadMotorStatus_Ajin()
{
	// Home
	for (int i = 0; i < (int)MOTOR_MAX_COUNT; i++)
	{
		if (m_bMotorHome_Done[i] != (BOOL)AJIN_MO->m_bReadHome[i])
		{
			m_bMotorHome_Done[i] = (BOOL)AJIN_MO->m_bReadHome[i];
			SendMessage(MESSAGE_UI_CHANGE_HOME, i, m_bMotorHome_Done[i]);
		}
	}
	//Limit
	for (int i = 0; i < (int)MOTOR_MAX_COUNT; i++)
	{
		if (m_bMotorLimit_P[i] != (BOOL)AJIN_MO->m_bReadLimitP[i])
		{
			m_bMotorLimit_P[i] = (BOOL)AJIN_MO->m_bReadLimitP[i];
			SendMessage(MESSAGE_UI_CHANGE_LIMIT, i, m_bMotorLimit_P[i]);
		}

		if (m_bMotorLimit_M[i] != (BOOL)AJIN_MO->m_bReadLimitM[i])
		{
			m_bMotorLimit_M[i] = (BOOL)AJIN_MO->m_bReadLimitM[i];
			SendMessage(MESSAGE_UI_CHANGE_LIMIT, i + 10, m_bMotorLimit_M[i]);
		}
	}
	// Motor Alarm
	for (int i = 0; i < (int)MOTOR_MAX_COUNT; i++)
	{
		if (m_bMotorAlarm[i] != (BOOL)AJIN_MO->m_bReadError[i])
		{
			m_bMotorAlarm[i] = (BOOL)AJIN_MO->m_bReadError[i];
			SendMessage(MESSAGE_UI_CHANGE_ALARM, i, m_bMotorAlarm[i]);
		}
	}

	// Inmotion
	for (int i = 0; i < (int)MOTOR_MAX_COUNT; i++)
	{
		if (m_bMotorInMotion[i] != (BOOL)AJIN_MO->m_bReadInPos[i])
		{
			m_bMotorInMotion[i] = (BOOL)AJIN_MO->m_bReadInPos[i];
			SendMessage(MESSAGE_UI_CHANGE_INMOTION, i, m_bMotorInMotion[i]);
		}
	}
}
void CDlgMain::GetCurrentPosition()
{
	CString strPos = _T("");
	double dPos = 0.0f;

	for (int i = 0; i < (int)MOTOR_MAX_COUNT; i++)
	{
		dPos = FAS_MO->GetAxisCurrnetActualPos(i);

		strPos.Format(_T("%0.3f"), dPos);

		m_stMainMotor_Pos[i].SetText(strPos);
	}

}

LRESULT CDlgMain::SetMainUIStatusChange_Home(WPARAM wParam, LPARAM lParam)
{
	int nCnt = wParam;
	BOOL bChange = lParam;

	if (bChange == TRUE)
	{
		m_stMainMotor_Home[nCnt].SetBackColor(Gdiplus::Color::Green);
		m_stMainMotor_Home[nCnt].SetTextColor(Gdiplus::Color::White, Gdiplus::Color::White);
	}
	else
	{
		m_stMainMotor_Home[nCnt].SetBackColor(Gdiplus::Color::DarkGray);
		m_stMainMotor_Home[nCnt].SetTextColor(Gdiplus::Color::White, Gdiplus::Color::White);
	}
	
	return 0;
}

LRESULT CDlgMain::SetMainUIStatusChange_Limit(WPARAM wParam, LPARAM lParam)
{
	int nCnt = wParam;
	BOOL bChange = lParam;

	if ((nCnt / 10) == 0)
	{
		nCnt = nCnt % 10;
		if (bChange == TRUE)
		{
			m_stMainMotor_Limit_P[nCnt].SetBackColor(Gdiplus::Color::Red);
			m_stMainMotor_Limit_P[nCnt].SetTextColor(Gdiplus::Color::White, Gdiplus::Color::White);
		}
		else
		{
			m_stMainMotor_Limit_P[nCnt].SetBackColor(Gdiplus::Color::DarkGray);
			m_stMainMotor_Limit_P[nCnt].SetTextColor(Gdiplus::Color::White, Gdiplus::Color::White);
		}
	}
	else
	{
		nCnt = nCnt % 10;
		if (bChange == TRUE)
		{
			m_stMainMotor_Limit_M[nCnt].SetBackColor(Gdiplus::Color::Red);
			m_stMainMotor_Limit_M[nCnt].SetTextColor(Gdiplus::Color::White, Gdiplus::Color::White);
		}
		else
		{
			m_stMainMotor_Limit_M[nCnt].SetBackColor(Gdiplus::Color::DarkGray);
			m_stMainMotor_Limit_M[nCnt].SetTextColor(Gdiplus::Color::White, Gdiplus::Color::White);
		}
	}

	return 0;
}

LRESULT CDlgMain::SetMainUIStatusChange_AxisAlarm(WPARAM wParam, LPARAM lParam)
{
	int nCnt = wParam;
	BOOL bChange = lParam;

	{
		if (bChange == TRUE)
		{
			m_stMainMotor_Error[nCnt].SetBackColor(Gdiplus::Color::Red);
			m_stMainMotor_Error[nCnt].SetTextColor(Gdiplus::Color::White, Gdiplus::Color::White);
		}
		else
		{
			m_stMainMotor_Error[nCnt].SetBackColor(Gdiplus::Color::DarkGray);
			m_stMainMotor_Error[nCnt].SetTextColor(Gdiplus::Color::White, Gdiplus::Color::White);
		}
	}
	return 0;
}

LRESULT CDlgMain::SetMainUIStatusChange_InMotion(WPARAM wParam, LPARAM lParam)
{
	int nCnt = wParam;
	BOOL bChange = lParam;

	{
		if (bChange == TRUE)
		{
			m_stMainMotor_InMotion[nCnt].SetBackColor(Gdiplus::Color::Green);
			m_stMainMotor_InMotion[nCnt].SetTextColor(Gdiplus::Color::White, Gdiplus::Color::White);
		}
		else
		{
			m_stMainMotor_InMotion[nCnt].SetBackColor(Gdiplus::Color::DarkGray);
			m_stMainMotor_InMotion[nCnt].SetTextColor(Gdiplus::Color::White, Gdiplus::Color::White);
		}
	}

	return 0;
}

void CDlgMain::HideStartUp()
{
	//m_btnMainStartUpMode[0].ShowWindow(SW_HIDE);
	//m_btnMainStartUpMode[1].ShowWindow(SW_HIDE);
	//m_btnMainStartUpMode[2].ShowWindow(SW_HIDE);
	//m_btnMainStartUpMode[3].ShowWindow(SW_HIDE);

	for(int i = 0; i<_countof(m_btnMainStartUpMode); i++)
		m_btnMainStartUpMode[i].ShowWindow(SW_HIDE);
}

void CDlgMain::ShowStartUp()
{
	auto EqpType = GET_INSPECTOR;

	for (int i = 0; i < _countof(m_btnMainStartUpMode); i++)
	{
		m_btnMainStartUpMode[i].ShowWindow(SW_SHOW);
		m_btnMainStartUpMode[i].EnableWindow(FALSE);
		m_btnMainStartUpMode[i].SetColorStyle(CVGStatic::ColorStyle_SlateGray);
	}		

	switch (EqpType)
	{
	case SYS_DISTORTION_30:		
		break;

	case SYS_OQC_SFR_MULTI_CL:
	case SYS_OQC_SFR_SINGLE_CL:
		m_btnMainStartUpMode[4].ShowWindow(SW_HIDE);
		m_btnMainStartUpMode[5].ShowWindow(SW_HIDE);
		m_btnMainStartUpMode[6].ShowWindow(SW_HIDE);
		m_btnMainStartUpMode[7].ShowWindow(SW_HIDE);
		break;
	}

	m_btnMainStartUpMode[3].EnableWindow(TRUE);
	m_btnMainStartUpMode[3].SetColorStyle(CVGStatic::ColorStyle_Black);

	/*m_btnMainStartUpMode[0].ShowWindow(SW_SHOW);
	m_btnMainStartUpMode[1].ShowWindow(SW_SHOW);
	m_btnMainStartUpMode[2].ShowWindow(SW_SHOW);
	m_btnMainStartUpMode[3].ShowWindow(SW_SHOW);

	m_btnMainStartUpMode[0].EnableWindow(FALSE);
	m_btnMainStartUpMode[1].EnableWindow(FALSE);
	m_btnMainStartUpMode[2].EnableWindow(FALSE);*/

	/*m_btnMainStartUpMode[3].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
	m_btnMainStartUpMode[3].SetColorStyle(CVGStatic::ColorStyle_Black);

	m_btnMainStartUpMode[0].SetColorStyle(CVGStatic::ColorStyle_SlateGray);
	m_btnMainStartUpMode[1].SetColorStyle(CVGStatic::ColorStyle_SlateGray);
	m_btnMainStartUpMode[2].SetColorStyle(CVGStatic::ColorStyle_SlateGray);*/
}

void CDlgMain::Startup_Start(__in UINT nIdx, __in int nCapture)
{
	for (int i = 0; i < _countof(m_btnMainStartUpMode); i++)
	{
		m_btnMainStartUpMode[i].EnableWindow(FALSE);
		m_btnMainStartUpMode[i].SetColorStyle(CVGStatic::ColorStyle_SlateGray);
	}

	/*m_btnMainStartUpMode[0].EnableWindow(FALSE);
	m_btnMainStartUpMode[1].EnableWindow(FALSE);
	m_btnMainStartUpMode[2].EnableWindow(FALSE);
	m_btnMainStartUpMode[3].EnableWindow(FALSE);

	m_btnMainStartUpMode[0].SetColorStyle(CVGStatic::ColorStyle_SlateGray);
	m_btnMainStartUpMode[1].SetColorStyle(CVGStatic::ColorStyle_SlateGray);
	m_btnMainStartUpMode[2].SetColorStyle(CVGStatic::ColorStyle_SlateGray);
	m_btnMainStartUpMode[3].SetColorStyle(CVGStatic::ColorStyle_SlateGray);*/

	if (nIdx == 1)
	{
		switch (nCapture)
		{
		case 0:
			m_btnMainStartUpMode[2].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
			m_btnMainStartUpMode[2].SetColorStyle(CVGStatic::ColorStyle_Green);
			break;

		case 1:
			m_btnMainStartUpMode[4].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
			m_btnMainStartUpMode[4].SetColorStyle(CVGStatic::ColorStyle_Green);
			break;
		case 2:
			m_btnMainStartUpMode[5].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
			m_btnMainStartUpMode[5].SetColorStyle(CVGStatic::ColorStyle_Green);
			break;
		case 3:
			m_btnMainStartUpMode[6].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
			m_btnMainStartUpMode[6].SetColorStyle(CVGStatic::ColorStyle_Green);
			break;
		case 4:
			m_btnMainStartUpMode[2].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
			m_btnMainStartUpMode[2].SetColorStyle(CVGStatic::ColorStyle_Green);
			break;
		case 5:
			m_btnMainStartUpMode[7].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
			m_btnMainStartUpMode[7].SetColorStyle(CVGStatic::ColorStyle_Green);
			break;
		}
	}
	else if (nIdx == 2)
	{
		m_btnMainStartUpMode[1].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_btnMainStartUpMode[1].SetColorStyle(CVGStatic::ColorStyle_Green);
	}
	else
	{
		m_btnMainStartUpMode[nIdx].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_btnMainStartUpMode[nIdx].SetColorStyle(CVGStatic::ColorStyle_Green);
	}
}

void CDlgMain::Startup_End(__in UINT nIdx, __in bool bResult)
{
	auto EqpType = GET_INSPECTOR;

	switch (nIdx)
	{
	case 0:	// load
		if (bResult)
		{
			if (EqpType == SYS_DISTORTION_30)
			{
				SetEnable_MoveCapture(true);

				/*m_btnMainStartUpMode[1].EnableWindow(TRUE);
				m_btnMainStartUpMode[1].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
				m_btnMainStartUpMode[1].SetColorStyle(CVGStatic::ColorStyle_Black);*/

				m_btnMainStartUpMode[3].EnableWindow(TRUE);
				m_btnMainStartUpMode[3].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
				m_btnMainStartUpMode[3].SetColorStyle(CVGStatic::ColorStyle_Black);
			}
			else
			{
				SetEnable_MoveCapture(false);

				m_btnMainStartUpMode[0].EnableWindow(FALSE);
				m_btnMainStartUpMode[2].EnableWindow(FALSE);
				m_btnMainStartUpMode[1].EnableWindow(TRUE);
				m_btnMainStartUpMode[3].EnableWindow(TRUE);

				m_btnMainStartUpMode[1].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
				m_btnMainStartUpMode[1].SetColorStyle(CVGStatic::ColorStyle_Black);

				m_btnMainStartUpMode[3].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
				m_btnMainStartUpMode[3].SetColorStyle(CVGStatic::ColorStyle_Black);

				m_btnMainStartUpMode[0].SetColorStyle(CVGStatic::ColorStyle_SlateGray);
				m_btnMainStartUpMode[2].SetColorStyle(CVGStatic::ColorStyle_SlateGray);
			}
		}
		else
		{
			SetEnable_MoveCapture(false);

			m_btnMainStartUpMode[0].EnableWindow(FALSE);
			m_btnMainStartUpMode[1].EnableWindow(FALSE);
			m_btnMainStartUpMode[2].EnableWindow(FALSE);
			m_btnMainStartUpMode[3].EnableWindow(TRUE);

			m_btnMainStartUpMode[3].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
			m_btnMainStartUpMode[3].SetColorStyle(CVGStatic::ColorStyle_Black);

			m_btnMainStartUpMode[0].SetColorStyle(CVGStatic::ColorStyle_SlateGray);
			m_btnMainStartUpMode[1].SetColorStyle(CVGStatic::ColorStyle_SlateGray);
			m_btnMainStartUpMode[2].SetColorStyle(CVGStatic::ColorStyle_SlateGray);
		}
		break;

	case 1: // blemish
		if (bResult)
		{
			if (EqpType == SYS_DISTORTION_30)
			{
				SetEnable_MoveCapture(true);

				m_btnMainStartUpMode[1].EnableWindow(TRUE);
				m_btnMainStartUpMode[3].EnableWindow(TRUE);

				m_btnMainStartUpMode[1].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
				m_btnMainStartUpMode[1].SetColorStyle(CVGStatic::ColorStyle_Black);

				m_btnMainStartUpMode[3].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
				m_btnMainStartUpMode[3].SetColorStyle(CVGStatic::ColorStyle_Black);
			}
			else
			{
				SetEnable_MoveCapture(false);

				m_btnMainStartUpMode[0].EnableWindow(FALSE);
				m_btnMainStartUpMode[1].EnableWindow(TRUE);
				m_btnMainStartUpMode[2].EnableWindow(FALSE);
				m_btnMainStartUpMode[3].EnableWindow(TRUE);

				m_btnMainStartUpMode[1].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
				m_btnMainStartUpMode[1].SetColorStyle(CVGStatic::ColorStyle_Black);

				m_btnMainStartUpMode[3].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
				m_btnMainStartUpMode[3].SetColorStyle(CVGStatic::ColorStyle_Black);

				m_btnMainStartUpMode[0].SetColorStyle(CVGStatic::ColorStyle_SlateGray);
				m_btnMainStartUpMode[2].SetColorStyle(CVGStatic::ColorStyle_SlateGray);
			}
		}
		else
		{
			if (EqpType != SYS_DISTORTION_30)
				SetEnable_MoveCapture(false);
		
			m_btnMainStartUpMode[0].EnableWindow(FALSE);
			m_btnMainStartUpMode[1].EnableWindow(FALSE);
			m_btnMainStartUpMode[2].EnableWindow(FALSE);
			m_btnMainStartUpMode[3].EnableWindow(TRUE);

			m_btnMainStartUpMode[3].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
			m_btnMainStartUpMode[3].SetColorStyle(CVGStatic::ColorStyle_Black);

			m_btnMainStartUpMode[0].SetColorStyle(CVGStatic::ColorStyle_SlateGray);
			m_btnMainStartUpMode[1].SetColorStyle(CVGStatic::ColorStyle_SlateGray);
			m_btnMainStartUpMode[2].SetColorStyle(CVGStatic::ColorStyle_SlateGray);
		}
		break;

	case 2: // sfr
		if (bResult)
		{
			if (EqpType == SYS_DISTORTION_30)
			{
				SetEnable_MoveCapture(true);

				m_btnMainStartUpMode[1].EnableWindow(TRUE);
				m_btnMainStartUpMode[3].EnableWindow(TRUE);

				m_btnMainStartUpMode[1].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
				m_btnMainStartUpMode[1].SetColorStyle(CVGStatic::ColorStyle_Black);

				m_btnMainStartUpMode[3].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
				m_btnMainStartUpMode[3].SetColorStyle(CVGStatic::ColorStyle_Black);
			}
			else
			{
				SetEnable_MoveCapture(false);

				m_btnMainStartUpMode[0].EnableWindow(FALSE);
				m_btnMainStartUpMode[1].EnableWindow(FALSE);
				m_btnMainStartUpMode[2].EnableWindow(TRUE);
				m_btnMainStartUpMode[3].EnableWindow(TRUE);

				m_btnMainStartUpMode[2].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
				m_btnMainStartUpMode[2].SetColorStyle(CVGStatic::ColorStyle_Black);

				m_btnMainStartUpMode[3].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
				m_btnMainStartUpMode[3].SetColorStyle(CVGStatic::ColorStyle_Black);

				m_btnMainStartUpMode[0].SetColorStyle(CVGStatic::ColorStyle_SlateGray);
				m_btnMainStartUpMode[1].SetColorStyle(CVGStatic::ColorStyle_SlateGray);
			}
		}
		else
		{
			SetEnable_MoveCapture(false);

			m_btnMainStartUpMode[0].EnableWindow(FALSE);
			m_btnMainStartUpMode[1].EnableWindow(FALSE);
			m_btnMainStartUpMode[2].EnableWindow(FALSE);
			m_btnMainStartUpMode[3].EnableWindow(TRUE);

			m_btnMainStartUpMode[3].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
			m_btnMainStartUpMode[3].SetColorStyle(CVGStatic::ColorStyle_Black);

			m_btnMainStartUpMode[0].SetColorStyle(CVGStatic::ColorStyle_SlateGray);
			m_btnMainStartUpMode[1].SetColorStyle(CVGStatic::ColorStyle_SlateGray);
			m_btnMainStartUpMode[2].SetColorStyle(CVGStatic::ColorStyle_SlateGray);
		}
		break;

	case 3:	// wait pos
		if (bResult)
		{
			SetEnable_MoveCapture(false);

			m_btnMainStartUpMode[0].EnableWindow(TRUE);
			m_btnMainStartUpMode[1].EnableWindow(FALSE);
			m_btnMainStartUpMode[2].EnableWindow(FALSE);
			m_btnMainStartUpMode[3].EnableWindow(TRUE);

			m_btnMainStartUpMode[0].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
			m_btnMainStartUpMode[0].SetColorStyle(CVGStatic::ColorStyle_Black);

			m_btnMainStartUpMode[3].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
			m_btnMainStartUpMode[3].SetColorStyle(CVGStatic::ColorStyle_Black);

			m_btnMainStartUpMode[1].SetColorStyle(CVGStatic::ColorStyle_SlateGray);
			m_btnMainStartUpMode[2].SetColorStyle(CVGStatic::ColorStyle_SlateGray);
		}
		else
		{
			SetEnable_MoveCapture(false);

			m_btnMainStartUpMode[0].EnableWindow(FALSE);
			m_btnMainStartUpMode[1].EnableWindow(FALSE);
			m_btnMainStartUpMode[2].EnableWindow(FALSE);
			m_btnMainStartUpMode[3].EnableWindow(TRUE);

			m_btnMainStartUpMode[3].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
			m_btnMainStartUpMode[3].SetColorStyle(CVGStatic::ColorStyle_Black);

			m_btnMainStartUpMode[0].SetColorStyle(CVGStatic::ColorStyle_SlateGray);
			m_btnMainStartUpMode[1].SetColorStyle(CVGStatic::ColorStyle_SlateGray);
			m_btnMainStartUpMode[2].SetColorStyle(CVGStatic::ColorStyle_SlateGray);
		}
		break;
	}

	m_btnMainStartUpMode[nIdx].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
	m_btnMainStartUpMode[nIdx].SetColorStyle(CVGStatic::ColorStyle_Black);
}

// Message End
//=======================================================================================================

void CDlgMain::OnBnClickedButton1()
{
	if (m_UdpTester[0].m_bThreadRun == TRUE)
	{
		EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();
		if( stEq.nLanguage == 0)
			WRITE_MAIN_LOG(_T("이미 접속되어 있습니다."));
		else
			WRITE_MAIN_LOG(_T("Already Connected."));
		return;
	}

	TESTER_COMM stTester = *DATAMANAGER->GetCurrentTesterCommData();
	m_UdpTester[0].Connect_Tester(stTester.dwTesterIP[0], stTester.nTesterPort[0], stTester.strTesterConnect[0], 1);

	COMMON->m_bConnectTesterFlag[0] = TRUE;
}

void CDlgMain::OnBnClickedButton2()
{
	CString strTemp = _T("");

	TESTER_COMM stTester = *DATAMANAGER->GetCurrentTesterCommData();

	m_UdpTester[0].Send_Tester_Communication(eComMode_Ready);
}

void CDlgMain::OnBnClickedButton3()
{
	CString strTemp = _T("");

	TESTER_COMM stTester = *DATAMANAGER->GetCurrentTesterCommData();

	m_UdpTester[0].Send_Tester_Communication(eComMode_PreviewStart);
}


void CDlgMain::OnBnClickedButton4()
{
	CString strTemp = _T("");

	TESTER_COMM stTester = *DATAMANAGER->GetCurrentTesterCommData();

	m_UdpTester[0].Send_Tester_Communication(eComMode_Insp_SFR);
}


void CDlgMain::OnBnClickedButton7()
{
	CString strTemp = _T("");

	TESTER_COMM stTester = *DATAMANAGER->GetCurrentTesterCommData();

	m_UdpTester[0].Send_Tester_Communication(eComMode_Insp_TEST);
}


void CDlgMain::OnBnClickedButton8()
{
	CString strTemp = _T("");

	TESTER_COMM stTester = *DATAMANAGER->GetCurrentTesterCommData();

	m_UdpTester[0].Send_Tester_Communication(eComMode_Insp_PreviewStop);
}

void CDlgMain::OnBnClickedBtnYieldReset()
{
	EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();
	CString strTemp;
	if (stBasic.nLanguage == 0)
		strTemp.Format(_T("일일생산량을 초기화 하시겠습니까?"));
	else
		strTemp.Format(_T("Do you want to reset the daily production?"));


	m_btnMainYield_Reset.SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
	m_btnMainYield_Reset.SetColorStyle(CVGStatic::ColorStyle_Green);
	m_btnMainYield_Reset.SetFont_Gdip(L"Arial", 9.0F);
	if (stBasic.nLanguage == 0)
		m_btnMainYield_Reset.SetText(m_szMainYield_Kor[4]);
	else
		m_btnMainYield_Reset.SetText(m_szMainYield_Eng[4]);


	if (COMMON->ShowMsgDlg_Domodal(strTemp, MODE_INFORMATION) == TRUE)
	{
		for (int i = 0; i < 4; i++)
		{
			COMMON->SetClearCounts();
			m_stMainYield_Title_Sub[i][1].SetStaticStyle(CVGStatic::StaticStyle_Data);
			m_stMainYield_Title_Sub[i][1].SetColorStyle(CVGStatic::ColorStyle_Green);
			m_stMainYield_Title_Sub[i][1].SetFont_Gdip(L"Arial", 9.0F);
			m_stMainYield_Title_Sub[i][1].SetText(_T("0"));
		}
	}

	m_btnMainYield_Reset.SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
	m_btnMainYield_Reset.SetColorStyle(CVGStatic::ColorStyle_Black);
	m_btnMainYield_Reset.SetFont_Gdip(L"Arial", 9.0F);
	if (stBasic.nLanguage == 0)
		m_btnMainYield_Reset.SetText(m_szMainYield_Kor[4]);
	else
		m_btnMainYield_Reset.SetText(m_szMainYield_Eng[4]);
}

void CDlgMain::OnBnClickedButton5()
{
	/*EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();

	if (COMMON->GetPermissionGrade() < 1)
	{
		if (stBasic.nLanguage == 0)
			WRITE_MAIN_LOG(_T("권한이 없습니다."));
		else
			WRITE_MAIN_LOG(_T("You don;t have Permision."));

		return;
	}

	if (m_ctrlLight_ODA_Q[0].OpenPort(6, 0, WM_LIGHT_CONTROL_MSG, 0, 1, 0, 0) == FALSE)
	{
		WRITE_MAIN_LOG(_T("FAIL CONNECT"));
		AfxMessageBox(_T("Connect Fail"), MB_OK);
	}
	else
	{
		WRITE_MAIN_LOG(_T("CONNECT SUCCESS"));
		AfxMessageBox(_T("Connect Success"), MB_OK);
	}*/
}

void CDlgMain::OnBnClickedButton9()
{
	/*m_ctrlLight_ODA_Q[0].Write_Channel(1);
	Sleep(1000);

	m_ctrlLight_ODA_Q[0].Write_Volt((double)12);
	Sleep(1000);

	m_ctrlLight_ODA_Q[0].Write_Current((double)1);*/
}

void CDlgMain::OnBnClickedButton10()
{
	EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();

	if (COMMON->GetPermissionGrade() < 1)
	{
		if (stBasic.nLanguage == 0)
			WRITE_MAIN_LOG(_T("권한이 없습니다."));
		else
			WRITE_MAIN_LOG(_T("You don;t have Permision."));

		return;
	}

	/*m_ctrlLight_ODA_Q[0].Write_Channel(1);
	Sleep(500);

	m_ctrlLight_ODA_Q[0].Write_Volt((double)12);
	Sleep(500);

	m_ctrlLight_ODA_Q[0].Write_Current((double)1);

	Sleep(500);
	m_ctrlLight_ODA_Q[0].Wrtie_ON();	*/

	CDevice::PowerOn();
	AfxMessageBox(_T("Power On Completed"), MB_OK);
}

void CDlgMain::OnBnClickedButton11()
{
	EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();

	if (COMMON->GetPermissionGrade() < 1)
	{
		if (stBasic.nLanguage == 0)
			WRITE_MAIN_LOG(_T("권한이 없습니다."));
		else
			WRITE_MAIN_LOG(_T("You don;t have Permision.."));

		return;
	}

//	m_ctrlLight_ODA_Q[0].Write_OFF();	
	CDevice::PowerOff();
}

void CDlgMain::OnBnClickedButton12()
{
	m_UdpTester[0].m_bTesterFlag_Ready = TRUE;
}

void CDlgMain::ChangeDayProduct()
{
	COMMON->SetTotalCnt();

	if (COMMON->GetTotalCnt() - COMMON->GetNGCnt() != COMMON->GetOKCnt())
		COMMON->SetOKCnt();

	SetDlgItemInt(IDC_STC_YIELD_COUNT_1, COMMON->GetTotalCnt());
	SetDlgItemInt(IDC_STC_YIELD_COUNT_2, COMMON->GetOKCnt());
	SetDlgItemInt(IDC_STC_YIELD_COUNT_3, COMMON->GetNGCnt());
	double fYield = 0.0f;
	CString strYield = _T("");
	fYield = COMMON->GetOKCnt() * ((float)100/ COMMON->GetTotalCnt());
	strYield.Format(_T("%0.2f"),fYield);
	SetDlgItemText(IDC_STC_YIELD_COUNT_4, strYield);

	COMMON->SetReplaceFlag(FALSE);
}


void CDlgMain::OnStnDblclickStcLogMain()
{
	if (COMMON->GetPermissionGrade() < 1)
		return;
	EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();
	CString szStr = _T("");

	if (stBasic.nLanguage == 0)
		szStr.Format(_T("로그를 지우시겠습니까?"));
	else
		szStr.Format(_T("Do you want to log clear?"));

	if (AfxMessageBox(szStr, MB_YESNO) == IDYES)
		m_MainLog.ListClear();
	else
		return;
}


void CDlgMain::OnStnDblclickStcLogComm()
{
	if (COMMON->GetPermissionGrade() < 1)
		return;

	EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();
	CString szStr = _T("");

	if (stBasic.nLanguage == 0)
		szStr.Format(_T("로그를 지우시겠습니까?"));
	else
		szStr.Format(_T("Do you want to log clear?"));

	if (AfxMessageBox(szStr, MB_YESNO) == IDYES)
		m_CommLog.ListClear();
	else
		return;
}


void CDlgMain::OnStnDblclickStcLogSeq()
{
	if (COMMON->GetPermissionGrade() < 1)
		return;

	EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();
	CString szStr = _T("");

	if (stBasic.nLanguage == 0)
		szStr.Format(_T("로그를 지우시겠습니까?"));
	else
		szStr.Format(_T("Do you want to log clear?"));

	if (AfxMessageBox(szStr, MB_YESNO) == IDYES)
		m_SequenceLog.ListClear();
	else
		return;
}

void CDlgMain::OnBnClickedButtonClearlog1()
{
	//if (COMMON->GetPermissionGrade() < 1)
	//	return;
	EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();
	CString szStr = _T("");

	if (stBasic.nLanguage == 0)
		szStr.Format(_T("로그를 지우시겠습니까?"));
	else
		szStr.Format(_T("Do you want to log clear?"));

	if (AfxMessageBox(szStr, MB_YESNO) == IDYES)
		m_MainLog.ListClear();
}


void CDlgMain::OnBnClickedButtonClearlog2()
{
	//if (COMMON->GetPermissionGrade() < 1)
	//	return;

	EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();
	CString szStr = _T("");

	if (stBasic.nLanguage == 0)
		szStr.Format(_T("로그를 지우시겠습니까?"));
	else
		szStr.Format(_T("Do you want to log clear?"));

	if (AfxMessageBox(szStr, MB_YESNO) == IDYES)
		m_CommLog.ListClear();
}


void CDlgMain::OnBnClickedButtonClearlog3()
{
	//if (COMMON->GetPermissionGrade() < 1)
	//	return;

	EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();
	CString szStr = _T("");

	if (stBasic.nLanguage == 0)
		szStr.Format(_T("로그를 지우시겠습니까?"));
	else
		szStr.Format(_T("Do you want to log clear?"));

	if (AfxMessageBox(szStr, MB_YESNO) == IDYES)
		m_SequenceLog.ListClear();
}

// startup load
void CDlgMain::OnBnClickedBtnStartupMode1()
{
	SEQUENCE->Start_StartupCheck(0);
}

// startup sfr
void CDlgMain::OnBnClickedBtnStartupMode2()
{
	SEQUENCE->Start_StartupCheck(2);
}

void CDlgMain::OnBnClickedBtnStartupMode4()
{
	SEQUENCE->Start_StartupCheck(3);
}

// startup blemish
void CDlgMain::OnBnClickedBtnStartupMode3()
{
	SEQUENCE->Start_StartupCheck(1, 4);
}

void CDlgMain::OnBnClickedBtnStartupMode5()
{
	SEQUENCE->Start_StartupCheck(1, 1);
}

void CDlgMain::OnBnClickedBtnStartupMode6()
{
	SEQUENCE->Start_StartupCheck(1, 2);
}

void CDlgMain::OnBnClickedBtnStartupMode7()
{
	SEQUENCE->Start_StartupCheck(1, 3);
}

void CDlgMain::OnBnClickedBtnStartupMode8()
{
	SEQUENCE->Start_StartupCheck(1, 5);
}

BOOL CDlgMain::IsCameraExistOrSocektClosed()
{
	EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();

#if (GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL) || (GET_INSPECTOR == SYS_OQC_SFR_SINGLE_CL)
	if (AJIN_IO->GetInputIoBit(DIO_INPUT_X0_08_CAMERA_DETECT_SENSOR) != TRUE)
	{
		if (stBasic.nLanguage == 0)
			COMMON->ShowMsgDlg_Domodal(_T("카메라 센서에 감지되지 않았습니다.."), MODE_INFORMATION);
		else
			COMMON->ShowMsgDlg_Domodal(_T("Camera Sensor is not detected"), MODE_INFORMATION);
		return FALSE;
	}
#else
	/*
	if (AJIN_IO->GetInputIoBit(DIO_INPUT_X0_08_MANUALSOCEKT_OPENCLOSE1) != TRUE || AJIN_IO->GetInputIoBit(DIO_INPUT_X0_09_MANUALSOCEKT_OPENCLOSE2) != TRUE)
	{
		if (stBasic.nLanguage == 0)
			COMMON->ShowMsgDlg_Domodal(_T("소켓을 닫아주세요."), MODE_INFORMATION);
		else
			COMMON->ShowMsgDlg_Domodal(_T("Please Close Socket"), MODE_INFORMATION);
		return FALSE;
	}
	*/
#endif

	return TRUE;
}


void CDlgMain::OnBnClickedButtonFn300_L1()
{
	TESTER_COMM	stTester = *DATAMANAGER->GetCurrentTesterCommData();
	int nCate, nValue;

	//분류1 온
	nCate = 0;
#if (GET_INSPECTOR != SYS_DISTORTION_30)
// 	for (int nChan = 0; nChan < 9; nChan++)
// 	{
// 		if (m_ctrlLightController_FN1500[0].Is_Connected())
// 		{
// 			nValue = stTester.stLightData[0].nVoltage[nCate][nChan];
// 			m_ctrlLightController_FN1500[0].Set_FN1500_C(nChan + 1, nValue);
// 		}
// 	}

	for (int nChan = 0; nChan < 5; nChan++)
	{
		if (FN300->IsConnected())
		{
			nValue = stTester.stLightData[0].nVoltage[nCate][nChan];
			FN300->Send_SetChannelData(nChan + 1, nValue);
		}
	}
#else
	if (m_ctrlLightController_Youth_DPS[0].Is_Connected())
	{
		nValue = stTester.stLightData[0].nVoltage[nCate][0];
		m_ctrlLightController_Youth_DPS[0].LightOn(nValue);
	}
#endif

	m_stFN300Light_Status[0].SetColorStyle(CVGStatic::ColorStyle_LightGreen);
	m_sttFileProc[0].SetColorStyle(CVGStatic::ColorStyle_LightGreen);
	m_sttFileProc[1].SetColorStyle(CVGStatic::ColorStyle_LightGreen);
	m_stFN300Light_Status[1].SetColorStyle(CVGStatic::ColorStyle_SlateGray);
	m_sttFileProc[2].SetColorStyle(CVGStatic::ColorStyle_SlateGray);
	m_sttFileProc[3].SetColorStyle(CVGStatic::ColorStyle_SlateGray);
	m_stFileName[2].SetColorStyle(CVGStatic::ColorStyle_White);
	m_stFileName[3].SetColorStyle(CVGStatic::ColorStyle_White);
	bSelectCam = TRUE;		//TRUE = 180, FALSE = 83

	// 선택된 조명 모델 저장
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();
	stEq.nLightSelected = nCate;
	DATAMANAGER->SetEqData(stEq);
}

void CDlgMain::OnBnClickedButtonFn300_L2()
{
	TESTER_COMM	stTester = *DATAMANAGER->GetCurrentTesterCommData();
	int nCate, nValue;

	//분류2 온
	nCate = 1;
#if (GET_INSPECTOR != SYS_DISTORTION_30)
// 	for (int nChan = 0; nChan < 9; nChan++)
// 	{
// 		if (m_ctrlLightController_FN1500[0].Is_Connected())
// 		{
// 			nValue = stTester.stLightData[0].nVoltage[nCate][nChan];
// 			m_ctrlLightController_FN1500[0].Set_FN1500_C(nChan + 1, nValue);
// 		}
// 	}
	for (int nChan = 0; nChan < 5; nChan++)
	{
		if (FN300->IsConnected())
		{
			nValue = stTester.stLightData[0].nVoltage[nCate][nChan];
			FN300->Send_SetChannelData(nChan + 1, nValue);
		}
	}
#else
	if (m_ctrlLightController_Youth_DPS[0].Is_Connected())
	{
		nValue = stTester.stLightData[0].nVoltage[nCate][0];
		m_ctrlLightController_Youth_DPS[0].LightOn(nValue);
	}
#endif

	m_stFN300Light_Status[0].SetColorStyle(CVGStatic::ColorStyle_SlateGray);
	m_sttFileProc[0].SetColorStyle(CVGStatic::ColorStyle_SlateGray);
	m_sttFileProc[1].SetColorStyle(CVGStatic::ColorStyle_SlateGray);
	m_stFileName[0].SetColorStyle(CVGStatic::ColorStyle_White);
	m_stFileName[1].SetColorStyle(CVGStatic::ColorStyle_White);
	m_stFN300Light_Status[1].SetColorStyle(CVGStatic::ColorStyle_LightGreen);
	m_sttFileProc[2].SetColorStyle(CVGStatic::ColorStyle_LightGreen);
	m_sttFileProc[3].SetColorStyle(CVGStatic::ColorStyle_LightGreen);
	bSelectCam = FALSE;		//TRUE = 180, FALSE = 83

	// 선택된 조명 모델 저장
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();
	stEq.nLightSelected = nCate;
	DATAMANAGER->SetEqData(stEq);
}

// [22.0117.6] Door Lock
void CDlgMain::OnBnClickedBtnDoorLock()
{
	BOOL bDoorClose;
	BOOL bDoorLock;
	enDoorLIst eDoorId;
	for (int i = 0; i < EDoor_Max; i++)
	{
		eDoorId = (enDoorLIst)i;
		bDoorClose = COMMON->GetDoorClose(eDoorId);

		if (bDoorClose == TRUE)
		{
			COMMON->SetDoorLock(eDoorId, EUnlock);
		}
	}
	Sleep(500);
	for (int i = 0; i < EDoor_Max; i++)
	{
		eDoorId = (enDoorLIst)i;
		bDoorClose = COMMON->GetDoorClose(eDoorId);

		if (bDoorClose == TRUE)
		{
			COMMON->SetDoorLock(eDoorId, ELock);
		}
	}

}

void CDlgMain::UpdateSemcoVer() {

	TESTER_COMM	stTester = *DATAMANAGER->GetCurrentTesterCommData();

	CString strLocate[8];
	CString strFName[8];
	TCHAR cName[MAX_PATH] = { 0, };
	TCHAR cExt[MAX_PATH] = { 0, };

	int sAngle = 0;
	int sel = 0;

	if (bSelectCam) {
		sel = 0;
		if (nSelAngle == 0) {
			sAngle = 0;
		}
		else {
			sAngle = 4;
		}
	}
	else if (!bSelectCam) {
		sel = 2;
		if (nSelAngle == 0) {
			sAngle = 2;
		}
		else {
			sAngle = 6;
		}
	}

	for (int i = 0; i < 2; i++) 
	{
		strLocate[i+sAngle] = stTester.strSemcoVer[i + sAngle];
		_wsplitpath(strLocate[i + sAngle], NULL, NULL, cName, cExt);
		strFName[i + sAngle].Format(_T("%s%s"), cName, cExt);
	}

// 	for (int i = 0; i < 2; i++) 
// 	{
// 		if (FALSE == strFName[i + sAngle].IsEmpty()) 
// 		{
// 			if (IsFindProcess(strFName[i + sAngle], strLocate[i + sAngle])) 
// 			{
// 				m_stFileName[i + sel].SetColorStyle(CVGStatic::ColorStyle_LightGreen);
// 			}
// 			else {
// 				m_stFileName[i + sel].SetColorStyle(CVGStatic::ColorStyle_Red);
// 			}
// 		}
// 		else {
// 			m_stFileName[i + sel].SetColorStyle(CVGStatic::ColorStyle_White);
// 		}
// 	}
}

BOOL CDlgMain::IsFindProcess(CString szExt, CString szPath) {

	BOOL bFind = FALSE;

	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 processEntry32;

	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		exit(EXIT_FAILURE);
	}
	processEntry32.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(hProcessSnap, &processEntry32))
	{
		CloseHandle(hProcessSnap);
		exit(EXIT_FAILURE);
	}

	while (Process32Next(hProcessSnap, &processEntry32))
	{
		// 실행중인 프로세스 비교
		if (szExt.Compare(processEntry32.szExeFile) == 0)
		{
			// 실행중인 프로세스가 있다면
			HANDLE hExe = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processEntry32.th32ProcessID);

			HMODULE hMod;
			TCHAR szFullPath[MAX_PATH] = { 0, };

			if (hExe != NULL)
			{
				DWORD dwSize = 0;
				LPTSTR pszName = NULL;
				if (EnumProcessModules(hExe, &hMod, sizeof(hMod), &dwSize))
				{
					GetModuleFileNameEx(hExe, hMod, szFullPath, _countof(szFullPath));

					if (szPath.Compare(szFullPath) == 0)
						return TRUE;
				}
			}
		}
	}

	// 실행중인 프로세스 중에 찾는 프로세스가 없다면
	return bFind;
}

void CDlgMain::SetEnable_MoveCapture(__in bool enable)
{
	m_btnMainStartUpMode[2].EnableWindow(enable);
	m_btnMainStartUpMode[4].EnableWindow(enable);
	m_btnMainStartUpMode[5].EnableWindow(enable);
	m_btnMainStartUpMode[6].EnableWindow(enable);
	m_btnMainStartUpMode[7].EnableWindow(enable);

	if (enable == false)
	{
		m_btnMainStartUpMode[2].SetColorStyle(CVGStatic::ColorStyle_SlateGray);
		m_btnMainStartUpMode[4].SetColorStyle(CVGStatic::ColorStyle_SlateGray);
		m_btnMainStartUpMode[5].SetColorStyle(CVGStatic::ColorStyle_SlateGray);
		m_btnMainStartUpMode[6].SetColorStyle(CVGStatic::ColorStyle_SlateGray);
		m_btnMainStartUpMode[7].SetColorStyle(CVGStatic::ColorStyle_SlateGray);
	}
	else
	{
		m_btnMainStartUpMode[2].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_btnMainStartUpMode[4].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_btnMainStartUpMode[5].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_btnMainStartUpMode[6].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_btnMainStartUpMode[7].SetColorStyle(CVGStatic::ColorStyle_Black);
	}	
}
