// DlgMotorTeach.cpp: 구현 파일
//

#include "pch.h"
#include "LuriMotion_OQA_SFR_Wide.h"
#include "LuriMotion_OQA_SFR_WideDlg.h"
#include "DlgMotorTeach.h"
#include "afxdialogex.h"
#include "DlgSaveAsModel.h"

#include "Motion.h"

// CDlgMotorTeach 대화 상자

IMPLEMENT_DYNAMIC(CDlgMotorTeach, CDialogEx)

CDlgMotorTeach::CDlgMotorTeach(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_MOTOR_TEACHING, pParent)
{
	m_nJogMode				= 0;
	m_nSelectDistance		= 0;
	m_nSelectSpeed			= 0;
	m_dwOldAxisStatus		= 0;
	m_dMoveDistance			= 0.0f;
	m_nSetSpeed_AxisNum = 0;
	m_nSelectAxisNum = 0;
	m_bJogBtnBlockFlag = FALSE;
}

CDlgMotorTeach::~CDlgMotorTeach()
{
}

void CDlgMotorTeach::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STC_MOTOR_TITLE_1, m_stMotorTitle[0]);
	DDX_Control(pDX, IDC_STC_MOTOR_TITLE_2, m_stMotorTitle[1]);
	DDX_Control(pDX, IDC_STC_MOTOR_TITLE_3, m_stMotorTitle[2]);
	DDX_Control(pDX, IDC_STC_MOTOR_TITLE_4, m_stMotorTitle[3]);
	DDX_Control(pDX, IDC_STC_MOTOR_TITLE_5, m_stMotorTitle[4]);

	DDX_Control(pDX, IDC_STC_AXIS_01,		m_stMotorCurPos[0][0]);
	DDX_Control(pDX, IDC_STC_AXIS_02,		m_stMotorCurPos[0][1]);
	DDX_Control(pDX, IDC_STC_AXIS_03,		m_stMotorCurPos[0][2]);
	DDX_Control(pDX, IDC_STC_AXIS_04,		m_stMotorCurPos[0][3]);
	DDX_Control(pDX, IDC_STC_AXIS_05,		m_stMotorCurPos[0][4]);
	DDX_Control(pDX, IDC_STC_AXIS_06,		m_stMotorCurPos[0][5]);
	DDX_Control(pDX, IDC_STC_AXIS_07,		m_stMotorCurPos[0][6]);
	DDX_Control(pDX, IDC_STC_AXIS_08,		m_stMotorCurPos[0][7]);
	DDX_Control(pDX, IDC_STC_AXIS_09,		m_stMotorCurPos[0][8]);
	DDX_Control(pDX, IDC_STC_AXIS_10,		m_stMotorCurPos[0][9]);
	DDX_Control(pDX, IDC_STC_AXIS_CURPOS_01, m_stMotorCurPos[1][0]);
	DDX_Control(pDX, IDC_STC_AXIS_CURPOS_02, m_stMotorCurPos[1][1]);
	DDX_Control(pDX, IDC_STC_AXIS_CURPOS_03, m_stMotorCurPos[1][2]);
	DDX_Control(pDX, IDC_STC_AXIS_CURPOS_04, m_stMotorCurPos[1][3]);
	DDX_Control(pDX, IDC_STC_AXIS_CURPOS_05, m_stMotorCurPos[1][4]);
	DDX_Control(pDX, IDC_STC_AXIS_CURPOS_06, m_stMotorCurPos[1][5]);
	DDX_Control(pDX, IDC_STC_AXIS_CURPOS_07, m_stMotorCurPos[1][6]);
	DDX_Control(pDX, IDC_STC_AXIS_CURPOS_08, m_stMotorCurPos[1][7]);
	DDX_Control(pDX, IDC_STC_AXIS_CURPOS_09, m_stMotorCurPos[1][8]);
	DDX_Control(pDX, IDC_STC_AXIS_CURPOS_10, m_stMotorCurPos[1][9]);
	
	DDX_Control(pDX, IDC_STC_JOG_SELECT_1, m_stMotorJogTitleSub[0]);
	DDX_Control(pDX, IDC_STC_JOG_SELECT_2, m_stMotorJogTitleSub[1]);
	DDX_Control(pDX, IDC_STC_JOG_SELECT_3, m_stMotorJogTitleSub[2]);
	DDX_Control(pDX, IDC_STC_JOG_SELECT_4, m_stMotorJogTitleSub[3]);

	DDX_Control(pDX, IDC_BTN_JOG_SELECT_MODE_1, m_btnMotorJogSelect_Mode[0]);
	DDX_Control(pDX, IDC_BTN_JOG_SELECT_MODE_2, m_btnMotorJogSelect_Mode[1]);

	DDX_Control(pDX, IDC_BTN_JOG_SELECT_AXIS_1, m_btnMotorJogSelect_Axis[0]);
	DDX_Control(pDX, IDC_BTN_JOG_SELECT_AXIS_2, m_btnMotorJogSelect_Axis[1]);
	DDX_Control(pDX, IDC_BTN_JOG_SELECT_AXIS_3, m_btnMotorJogSelect_Axis[2]);
	DDX_Control(pDX, IDC_BTN_JOG_SELECT_AXIS_4, m_btnMotorJogSelect_Axis[3]);
	DDX_Control(pDX, IDC_BTN_JOG_SELECT_AXIS_5, m_btnMotorJogSelect_Axis[4]);
	DDX_Control(pDX, IDC_BTN_JOG_SELECT_AXIS_6, m_btnMotorJogSelect_Axis[5]);
	DDX_Control(pDX, IDC_BTN_JOG_SELECT_AXIS_7, m_btnMotorJogSelect_Axis[6]);
	DDX_Control(pDX, IDC_BTN_JOG_SELECT_AXIS_8, m_btnMotorJogSelect_Axis[7]);
	DDX_Control(pDX, IDC_BTN_JOG_SELECT_AXIS_9, m_btnMotorJogSelect_Axis[8]);
	DDX_Control(pDX, IDC_BTN_JOG_SELECT_AXIS_10, m_btnMotorJogSelect_Axis[9]);

	DDX_Control(pDX, IDC_BTN_JOG_DISTANCE_1, m_btnMotorJogSelect_Distance[0]);
	DDX_Control(pDX, IDC_BTN_JOG_DISTANCE_2, m_btnMotorJogSelect_Distance[1]);
	DDX_Control(pDX, IDC_BTN_JOG_DISTANCE_3, m_btnMotorJogSelect_Distance[2]);
	DDX_Control(pDX, IDC_BTN_JOG_DISTANCE_4, m_btnMotorJogSelect_Distance[3]);
	DDX_Control(pDX, IDC_BTN_JOG_DISTANCE_5, m_btnMotorJogSelect_Distance[4]);

	DDX_Control(pDX, IDC_BTN_JOG_SPEED_1, m_btnMotorJogSelect_Speed[0]);
	DDX_Control(pDX, IDC_BTN_JOG_SPEED_2, m_btnMotorJogSelect_Speed[1]);
	DDX_Control(pDX, IDC_BTN_JOG_SPEED_3, m_btnMotorJogSelect_Speed[2]);

	DDX_Control(pDX, IDC_BTN_JOG_MOVE_1, m_btnMotorJogSelect_Move[0]);
	DDX_Control(pDX, IDC_BTN_JOG_MOVE_2, m_btnMotorJogSelect_Move[1]);
	DDX_Control(pDX, IDC_BTN_JOG_MOVE_3, m_btnMotorJogSelect_Home);

	DDX_Text(pDX, IDC_EDIT1, m_strInput_JogDistance);
	DDX_Control(pDX, IDC_EDIT1, m_edInput_JogDistance);


	DDX_Control(pDX, IDC_STC_CYLINDER_1, m_stMotorManual[0]);
	DDX_Control(pDX, IDC_STC_CYLINDER_2, m_stMotorManual[1]);
	DDX_Control(pDX, IDC_STC_CYLINDER_3, m_stMotorManual[2]);
	DDX_Control(pDX, IDC_STC_CYLINDER_4, m_stMotorManual[3]);
	DDX_Control(pDX, IDC_STC_CYLINDER_5, m_stMotorManual[4]);
	DDX_Control(pDX, IDC_STC_CYLINDER_6, m_stMotorManual[5]);
	DDX_Control(pDX, IDC_STC_CYLINDER_7, m_stMotorManual[6]);
	DDX_Control(pDX, IDC_STC_CYLINDER_8, m_stMotorManual[7]);

	DDX_Control(pDX, IDC_BTN_CYLINDER_1, m_btnMotorManual[0][0]);
	DDX_Control(pDX, IDC_BTN_CYLINDER_2, m_btnMotorManual[0][1]);
	DDX_Control(pDX, IDC_BTN_CYLINDER_3, m_btnMotorManual[0][2]);
	DDX_Control(pDX, IDC_BTN_CYLINDER_4, m_btnMotorManual[0][3]);
	DDX_Control(pDX, IDC_BTN_CYLINDER_5, m_btnMotorManual[0][4]);
	DDX_Control(pDX, IDC_BTN_CYLINDER_6, m_btnMotorManual[0][5]);
	DDX_Control(pDX, IDC_BTN_CYLINDER_7, m_btnMotorManual[0][6]);
	DDX_Control(pDX, IDC_BTN_CYLINDER_8, m_btnMotorManual[0][7]);
	DDX_Control(pDX, IDC_BTN_CYLINDER_9, m_btnMotorManual[1][0]);
	DDX_Control(pDX, IDC_BTN_CYLINDER_10, m_btnMotorManual[1][1]);
	DDX_Control(pDX, IDC_BTN_CYLINDER_11, m_btnMotorManual[1][2]);
	DDX_Control(pDX, IDC_BTN_CYLINDER_12, m_btnMotorManual[1][3]);
	DDX_Control(pDX, IDC_BTN_CYLINDER_13, m_btnMotorManual[1][4]);
	DDX_Control(pDX, IDC_BTN_CYLINDER_14, m_btnMotorManual[1][5]);
	DDX_Control(pDX, IDC_BTN_CYLINDER_15, m_btnMotorManual[1][6]);
	DDX_Control(pDX, IDC_BTN_CYLINDER_16, m_btnMotorManual[1][7]);


	DDX_Control(pDX, IDC_COMBO_PORTNO1, m_cbFASBoard_Port);
	DDX_Control(pDX, IDC_COMBO_BAUDRATE1, m_cbFASBoard_BaudRate);


	DDX_Control(pDX, IDC_BTN_MOTOR_CONNECT1,		m_stMotorConnect[0]);
	DDX_Control(pDX, IDC_BTN_MOTOR_DISCONNECT1, m_stMotorConnect[1]);

	DDX_Control(pDX, IDC_BTN_MOTOR_SERVO_ON,	m_btnMotorState[0]);
	DDX_Control(pDX, IDC_BTN_MOTOR_SERVO_OFF,	m_btnMotorState[1]);
	DDX_Control(pDX, IDC_BTN_MOTOR_STOP,		m_btnMotorState[2]);
	DDX_Control(pDX, IDC_BTN_MOTOR_ALARMRESET,	m_btnMotorState[3]);
	
	
	DDX_Control(pDX, IDC_STC_SPEED_AXIS_1, m_stMotorSpeedSet_Sub[0]);
	DDX_Control(pDX, IDC_STC_SPEED_AXIS_2, m_stMotorSpeedSet_Sub[1]);

	DDX_Control(pDX, IDC_BTN_SPEED_AXIS_SELECT_1, m_btnMotorSpeedSet_AxisNum[0]);
	DDX_Control(pDX, IDC_BTN_SPEED_AXIS_SELECT_2, m_btnMotorSpeedSet_AxisNum[1]);
	DDX_Control(pDX, IDC_BTN_SPEED_AXIS_SELECT_3, m_btnMotorSpeedSet_AxisNum[2]);
	DDX_Control(pDX, IDC_BTN_SPEED_AXIS_SELECT_4, m_btnMotorSpeedSet_AxisNum[3]);
	DDX_Control(pDX, IDC_BTN_SPEED_AXIS_SELECT_5, m_btnMotorSpeedSet_AxisNum[4]);
	DDX_Control(pDX, IDC_BTN_SPEED_AXIS_SELECT_6, m_btnMotorSpeedSet_AxisNum[5]);
	DDX_Control(pDX, IDC_BTN_SPEED_AXIS_SELECT_7, m_btnMotorSpeedSet_AxisNum[6]);
	DDX_Control(pDX, IDC_BTN_SPEED_AXIS_SELECT_8, m_btnMotorSpeedSet_AxisNum[7]);
	DDX_Control(pDX, IDC_BTN_SPEED_AXIS_SELECT_9, m_btnMotorSpeedSet_AxisNum[8]);
	DDX_Control(pDX, IDC_BTN_SPEED_AXIS_SELECT_10, m_btnMotorSpeedSet_AxisNum[9]);
	
	DDX_Control(pDX, IDC_STC_SPEED_TITLE_1, m_stMotorSpeedSet_Thirdsub[0][0]);
	DDX_Control(pDX, IDC_STC_SPEED_TITLE_2, m_stMotorSpeedSet_Thirdsub[1][0]);
	DDX_Control(pDX, IDC_STC_SPEED_TITLE_3, m_stMotorSpeedSet_Thirdsub[2][0]);
	DDX_Control(pDX, IDC_STC_SPEED_TITLE_4, m_stMotorSpeedSet_Thirdsub[3][0]);
	DDX_Control(pDX, IDC_STC_SPEED_TITLE_5, m_stMotorSpeedSet_Thirdsub[4][0]);

	DDX_Control(pDX, IDC_STC_SPEED_SETTING_1, m_stMotorSpeedSet_Thirdsub[0][1]);
	DDX_Control(pDX, IDC_STC_SPEED_SETTING_2, m_stMotorSpeedSet_Thirdsub[1][1]);
	DDX_Control(pDX, IDC_STC_SPEED_SETTING_3, m_stMotorSpeedSet_Thirdsub[2][1]);
	DDX_Control(pDX, IDC_STC_SPEED_SETTING_4, m_stMotorSpeedSet_Thirdsub[3][1]);
	DDX_Control(pDX, IDC_STC_SPEED_SETTING_5, m_stMotorSpeedSet_Thirdsub[4][1]);

	DDX_Control(pDX, IDC_STC_SOCKET_Y_POS, m_stTeach_Y);
	DDX_Control(pDX, IDC_STC_Y_POS_1, m_stTeach_Y_Sub[0]);
	DDX_Control(pDX, IDC_STC_Y_POS_2, m_stTeach_Y_Sub[1]);
	DDX_Control(pDX, IDC_STC_Y_POS_3, m_stTeach_Y_Sub[2]);

	DDX_Control(pDX, IDC_STC_MODECHANGE_TIME, m_stModeChange);
	DDX_Control(pDX, IDC_STC_SOCKET_Z_POS, m_stTeach_Z);
	DDX_Control(pDX, IDC_STC_Z_POS_1, m_stTeach_Z_Sub[0]);
	DDX_Control(pDX, IDC_STC_Z_POS_2, m_stTeach_Z_Sub[1]);
	DDX_Control(pDX, IDC_STC_Z_POS_3, m_stTeach_Z_Sub[2]);
	DDX_Control(pDX, IDC_STC_Z_POS_4, m_stTeach_Z_Sub[3]);

	DDX_Control(pDX, IDC_STATIC_COMPORT1, m_stStatic_Text[0]);
	DDX_Control(pDX, IDC_STATIC_BAUDRATE1, m_stStatic_Text[1]);
	DDX_Control(pDX, IDC_STATIC_SHOW_MOTORNUM, m_stStatic_Text[2]);

	for (int nState = 0; nState < AXIS_MOTOR_STATUES; nState++)
	{
		DDX_Control(pDX, IDC_STATIC_MOTOR_STATUS1+ nState, m_stStatic_Motor_Statuses[nState]);
	}

	DDX_Text(pDX, IDC_EDIT_Y_POS_1, m_strTeach_AxisY_Pos[0]);
	DDX_Control(pDX, IDC_EDIT_Y_POS_1, m_edTeach_AxisY_Pos[0]);
	DDX_Text(pDX, IDC_EDIT_Y_POS_2, m_strTeach_AxisY_Pos[1]);
	DDX_Control(pDX, IDC_EDIT_Y_POS_2, m_edTeach_AxisY_Pos[1]);
	DDX_Text(pDX, IDC_EDIT_Y_POS_3, m_strTeach_AxisY_Pos[2]);
	DDX_Control(pDX, IDC_EDIT_Y_POS_3, m_edTeach_AxisY_Pos[2]);

	DDX_Text(pDX, IDC_EDIT_Y_POS_3, m_strTeach_AxisY_Pos[2]);
	DDX_Control(pDX, IDC_EDIT_Y_POS_3, m_edTeach_AxisY_Pos[2]);

	for (int i = 0; i < 5; i++)
	{
		DDX_Text(pDX, IDC_EDIT_Y_POS_4 + i, m_strTeach_AxisY_Pos2[i]);
		DDX_Control(pDX, IDC_EDIT_Y_POS_4 + i, m_edTeach_AxisY_Pos2[i]);
	}

	DDX_Text(pDX, IDC_EDIT_Z_POS_1, m_strTeach_AxisZ_Pos[0]);
	DDX_Control(pDX, IDC_EDIT_Z_POS_1, m_edTeach_AxisZ_Pos[0]);
	DDX_Text(pDX, IDC_EDIT_Z_POS_2, m_strTeach_AxisZ_Pos[1]);
	DDX_Control(pDX, IDC_EDIT_Z_POS_2, m_edTeach_AxisZ_Pos[1]);
	DDX_Text(pDX, IDC_EDIT_Z_POS_3, m_strTeach_AxisZ_Pos[2]);
	DDX_Control(pDX, IDC_EDIT_Z_POS_3, m_edTeach_AxisZ_Pos[2]);
	DDX_Text(pDX, IDC_EDIT_Z_POS_4, m_strTeach_AxisZ_Pos[3]);
	DDX_Control(pDX, IDC_EDIT_Z_POS_4, m_edTeach_AxisZ_Pos[3]);

	DDX_Text(pDX, IDC_EDIT_MODECHANGE_TIME, m_strTeach_ModeChangeTime);
	DDX_Control(pDX, IDC_EDIT_MODECHANGE_TIME, m_edTeach_ModeChangeTime);

	DDX_Control(pDX, IDC_STC_SOCKET_Y_POS2, m_stTeach_Y_Pos2);
	DDX_Control(pDX, IDC_STC_Y_POS_4, m_stTeach_Y_SubPos2[0]);
	DDX_Control(pDX, IDC_STC_Y_POS_5, m_stTeach_Y_SubPos2[1]);
	DDX_Control(pDX, IDC_STC_Y_POS_6, m_stTeach_Y_SubPos2[2]);
	DDX_Control(pDX, IDC_STC_Y_POS_7, m_stTeach_Y_SubPos2[3]);
	DDX_Control(pDX, IDC_STC_Y_POS_8, m_stTeach_Y_SubPos2[4]);
}

BEGIN_MESSAGE_MAP(CDlgMotorTeach, CDialogEx)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_MESSAGE(MESSAGE_STEP_MOVE, OnStepMove)

	ON_BN_CLICKED(IDC_BTN_JOG_SELECT_MODE_1, &CDlgMotorTeach::OnBnClickedBtnJogSelectMode1)
	ON_BN_CLICKED(IDC_BTN_JOG_SELECT_MODE_2, &CDlgMotorTeach::OnBnClickedBtnJogSelectMode2)
	ON_BN_CLICKED(IDC_BTN_JOG_SELECT_AXIS_1, &CDlgMotorTeach::OnBnClickedBtnJogSelectAxis1)
	ON_BN_CLICKED(IDC_BTN_JOG_SELECT_AXIS_2, &CDlgMotorTeach::OnBnClickedBtnJogSelectAxis2)
	ON_BN_CLICKED(IDC_BTN_JOG_SELECT_AXIS_3, &CDlgMotorTeach::OnBnClickedBtnJogSelectAxis3)
	ON_BN_CLICKED(IDC_BTN_JOG_SELECT_AXIS_4, &CDlgMotorTeach::OnBnClickedBtnJogSelectAxis4)
	ON_BN_CLICKED(IDC_BTN_JOG_SELECT_AXIS_5, &CDlgMotorTeach::OnBnClickedBtnJogSelectAxis5)
	ON_BN_CLICKED(IDC_BTN_JOG_SELECT_AXIS_6, &CDlgMotorTeach::OnBnClickedBtnJogSelectAxis6)
	ON_BN_CLICKED(IDC_BTN_JOG_SELECT_AXIS_7, &CDlgMotorTeach::OnBnClickedBtnJogSelectAxis7)
	ON_BN_CLICKED(IDC_BTN_JOG_SELECT_AXIS_8, &CDlgMotorTeach::OnBnClickedBtnJogSelectAxis8)
	ON_BN_CLICKED(IDC_BTN_JOG_SELECT_AXIS_9, &CDlgMotorTeach::OnBnClickedBtnJogSelectAxis9)
	ON_BN_CLICKED(IDC_BTN_JOG_SELECT_AXIS_10, &CDlgMotorTeach::OnBnClickedBtnJogSelectAxis10)
	ON_BN_CLICKED(IDC_BTN_JOG_DISTANCE_1, &CDlgMotorTeach::OnBnClickedBtnJogDistance1)
	ON_BN_CLICKED(IDC_BTN_JOG_DISTANCE_2, &CDlgMotorTeach::OnBnClickedBtnJogDistance2)
	ON_BN_CLICKED(IDC_BTN_JOG_DISTANCE_3, &CDlgMotorTeach::OnBnClickedBtnJogDistance3)
	ON_BN_CLICKED(IDC_BTN_JOG_DISTANCE_4, &CDlgMotorTeach::OnBnClickedBtnJogDistance4)
	ON_BN_CLICKED(IDC_BTN_JOG_DISTANCE_5, &CDlgMotorTeach::OnBnClickedBtnJogDistance5)
	ON_BN_CLICKED(IDC_BTN_JOG_SPEED_1, &CDlgMotorTeach::OnBnClickedBtnJogSpeed1)
	ON_BN_CLICKED(IDC_BTN_JOG_SPEED_2, &CDlgMotorTeach::OnBnClickedBtnJogSpeed2)
	ON_BN_CLICKED(IDC_BTN_JOG_SPEED_3, &CDlgMotorTeach::OnBnClickedBtnJogSpeed3)

	ON_BN_CLICKED(IDC_BTN_JOG_MOVE_3, &CDlgMotorTeach::OnBnClickedBtnJogMove3)			// 원점 복귀

	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_CYLINDER_1, IDC_BTN_CYLINDER_16, &CDlgMotorTeach::CylinderManual_Acitve)
	
	ON_BN_CLICKED(IDC_BTN_SPEED_AXIS_SELECT_1, &CDlgMotorTeach::OnBnClickedBtnSpeedAxisSelect1)
	ON_BN_CLICKED(IDC_BTN_SPEED_AXIS_SELECT_2, &CDlgMotorTeach::OnBnClickedBtnSpeedAxisSelect2)
	ON_BN_CLICKED(IDC_BTN_SPEED_AXIS_SELECT_3, &CDlgMotorTeach::OnBnClickedBtnSpeedAxisSelect3)
	ON_BN_CLICKED(IDC_BTN_SPEED_AXIS_SELECT_4, &CDlgMotorTeach::OnBnClickedBtnSpeedAxisSelect4)
	ON_BN_CLICKED(IDC_BTN_SPEED_AXIS_SELECT_5, &CDlgMotorTeach::OnBnClickedBtnSpeedAxisSelect5)
	ON_BN_CLICKED(IDC_BTN_SPEED_AXIS_SELECT_6, &CDlgMotorTeach::OnBnClickedBtnSpeedAxisSelect6)
	ON_BN_CLICKED(IDC_BTN_SPEED_AXIS_SELECT_7, &CDlgMotorTeach::OnBnClickedBtnSpeedAxisSelect7)
	ON_BN_CLICKED(IDC_BTN_SPEED_AXIS_SELECT_8, &CDlgMotorTeach::OnBnClickedBtnSpeedAxisSelect8)
	ON_BN_CLICKED(IDC_BTN_SPEED_AXIS_SELECT_9, &CDlgMotorTeach::OnBnClickedBtnSpeedAxisSelect9)
	ON_BN_CLICKED(IDC_BTN_SPEED_AXIS_SELECT_10, &CDlgMotorTeach::OnBnClickedBtnSpeedAxisSelect10)

	//ON_CONTROL_RANGE(BN_CLICKED, IDC_STC_SPEED_SETTING_1, IDC_STC_SPEED_SETTING_5, &CDlgMotorTeach::MotorSpeedChange )
	ON_STN_CLICKED(IDC_STC_SPEED_SETTING_1, &CDlgMotorTeach::OnStnClickedStcSpeedSetting1)
	ON_STN_CLICKED(IDC_STC_SPEED_SETTING_2, &CDlgMotorTeach::OnStnClickedStcSpeedSetting2)
	ON_STN_CLICKED(IDC_STC_SPEED_SETTING_3, &CDlgMotorTeach::OnStnClickedStcSpeedSetting3)
	ON_STN_CLICKED(IDC_STC_SPEED_SETTING_4, &CDlgMotorTeach::OnStnClickedStcSpeedSetting4)
	ON_STN_CLICKED(IDC_STC_SPEED_SETTING_5, &CDlgMotorTeach::OnStnClickedStcSpeedSetting5)

	ON_BN_CLICKED(IDC_BTN_TEACH_SAVE_1, &CDlgMotorTeach::OnBnClickedBtnTeachSave1)
	ON_BN_CLICKED(IDC_BTN_TEACH_SAVE_2, &CDlgMotorTeach::OnBnClickedBtnTeachSave2)
	ON_BN_CLICKED(IDC_BTN_TEACH_SAVE_3, &CDlgMotorTeach::OnBnClickedBtnTeachSave3)
	ON_BN_CLICKED(IDC_BTN_MOTOR_CONNECT1, &CDlgMotorTeach::OnBnClickedBtnMotorConnect1)
	ON_BN_CLICKED(IDC_BTN_MOTOR_DISCONNECT1, &CDlgMotorTeach::OnBnClickedBtnMotorDisconnect1)
	ON_BN_CLICKED(IDC_BTN_MOTOR_SERVO_ON, &CDlgMotorTeach::OnBnClickedBtnMotorServoOn)
	ON_BN_CLICKED(IDC_BTN_MOTOR_SERVO_OFF, &CDlgMotorTeach::OnBnClickedBtnMotorServoOff)
	ON_BN_CLICKED(IDC_BTN_MOTOR_STOP, &CDlgMotorTeach::OnBnClickedBtnMotorStop)
	ON_BN_CLICKED(IDC_BTN_MOTOR_ALARMRESET, &CDlgMotorTeach::OnBnClickedBtnMotorAlarmreset)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_TEACHING_POS_Y_1, &CDlgMotorTeach::OnBnClickedBtnTeachingPosY1)
	ON_BN_CLICKED(IDC_BTN_TEACHING_POS_Y_2, &CDlgMotorTeach::OnBnClickedBtnTeachingPosY2)
	ON_BN_CLICKED(IDC_BTN_TEACHING_POS_Y_3, &CDlgMotorTeach::OnBnClickedBtnTeachingPosY3)
	ON_BN_CLICKED(IDC_BTN_TEACHING_POS_Y_4, &CDlgMotorTeach::OnBnClickedBtnTeachingPosY4)
	ON_BN_CLICKED(IDC_BTN_TEACHING_POS_Y_5, &CDlgMotorTeach::OnBnClickedBtnTeachingPosY5)
	ON_BN_CLICKED(IDC_BTN_TEACHING_POS_Y_6, &CDlgMotorTeach::OnBnClickedBtnTeachingPosY6)
	ON_BN_CLICKED(IDC_BTN_TEACHING_POS_Y_7, &CDlgMotorTeach::OnBnClickedBtnTeachingPosY7)
	ON_BN_CLICKED(IDC_BTN_TEACHING_MOVE_Y_1, &CDlgMotorTeach::OnBnClickedBtnTeachingMoveY1)
	ON_BN_CLICKED(IDC_BTN_TEACHING_MOVE_Y_2, &CDlgMotorTeach::OnBnClickedBtnTeachingMoveY2)
	ON_BN_CLICKED(IDC_BTN_TEACHING_MOVE_Y_3, &CDlgMotorTeach::OnBnClickedBtnTeachingMoveY3)
	ON_BN_CLICKED(IDC_BTN_TEACHING_MOVE_Y_4, &CDlgMotorTeach::OnBnClickedBtnTeachingMoveY4)
	ON_BN_CLICKED(IDC_BTN_TEACHING_MOVE_Y_5, &CDlgMotorTeach::OnBnClickedBtnTeachingMoveY5)
	ON_BN_CLICKED(IDC_BTN_TEACHING_MOVE_Y_6, &CDlgMotorTeach::OnBnClickedBtnTeachingMoveY6)
	ON_BN_CLICKED(IDC_BTN_TEACHING_MOVE_Y_7, &CDlgMotorTeach::OnBnClickedBtnTeachingMoveY7)
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BTN_JOG_MOVE_2, &CDlgMotorTeach::OnBnClickedBtnJogMove2)
	ON_BN_CLICKED(IDC_BTN_TEACHING_POS_Y_8, &CDlgMotorTeach::OnBnClickedBtnTeachingPosY8)
	ON_BN_CLICKED(IDC_BTN_TEACHING_POS_Y_9, &CDlgMotorTeach::OnBnClickedBtnTeachingPosY9)
	ON_BN_CLICKED(IDC_BTN_TEACHING_POS_Y_10, &CDlgMotorTeach::OnBnClickedBtnTeachingPosY10)
	ON_BN_CLICKED(IDC_BTN_TEACHING_POS_Y_11, &CDlgMotorTeach::OnBnClickedBtnTeachingPosY11)
	ON_BN_CLICKED(IDC_BTN_TEACHING_POS_Y_12, &CDlgMotorTeach::OnBnClickedBtnTeachingPosY12)
	ON_BN_CLICKED(IDC_BTN_TEACHING_MOVE_Y_8, &CDlgMotorTeach::OnBnClickedBtnTeachingMoveY8)
	ON_BN_CLICKED(IDC_BTN_TEACHING_MOVE_Y_9, &CDlgMotorTeach::OnBnClickedBtnTeachingMoveY9)
	ON_BN_CLICKED(IDC_BTN_TEACHING_MOVE_Y_10, &CDlgMotorTeach::OnBnClickedBtnTeachingMoveY10)
	ON_BN_CLICKED(IDC_BTN_TEACHING_MOVE_Y_11, &CDlgMotorTeach::OnBnClickedBtnTeachingMoveY11)
	ON_BN_CLICKED(IDC_BTN_TEACHING_MOVE_Y_12, &CDlgMotorTeach::OnBnClickedBtnTeachingMoveY12)
END_MESSAGE_MAP()

BOOL CDlgMotorTeach::PreTranslateMessage(MSG* pMsg)
{
	CString strLog = _T("");
	double dCurPos = 0.0f;
	double dTargetPos = 0.0f;
	long   lAxisNum = 0;

	switch (pMsg->message)
	{
	case WM_LBUTTONDOWN:
	{
		EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();

		if (pMsg->hwnd == GetDlgItem(IDC_BTN_JOG_MOVE_1)->GetSafeHwnd())
		{
			/*if (COMMON->GetInterlock_SocketIn() == FALSE)
			{
				if (stEq.nLanguage == 0)
					COMMON->ShowMsgDlg_Domodal(_T("소켓을 넣어주세요."), MODE_INFORMATION);
				else
					COMMON->ShowMsgDlg_Domodal(_T("Please Move In the socket"), MODE_INFORMATION);
				return FALSE;
			}

			UpdateData(TRUE);*/

			//선택 축이 모션 운동 중이면
			if (FAS_MO->IsMotioning(m_nSelectAxisNum))
			{
				strLog.Format(_T("[JOG MOVE FAIL] Axis %d is Moving. \n"), m_nSelectAxisNum);
				m_bFlagJogError = true;
				return FALSE;
			}

			if (m_nJogMode == 0)
			{
				// Velocity Move
				if (!FAS_MO->MoveVelocity((long)m_nSelectAxisNum, m_dMoveSpeed, 0))
				{
					return FALSE;
				}
			}
			else
			{
				if (m_bJogBtnBlockFlag == TRUE)
					return TRUE;

				PostMessage(MESSAGE_STEP_MOVE, m_nSelectAxisNum, -1);
			}
		}
		else if (pMsg->hwnd == GetDlgItem(IDC_BTN_JOG_MOVE_2)->GetSafeHwnd())
		{
			/*if (COMMON->GetInterlock_SocketIn() == FALSE)
			{
				if (stEq.nLanguage == 0)
					COMMON->ShowMsgDlg_Domodal(_T("소켓을 넣어주세요."), MODE_INFORMATION);
				else
					COMMON->ShowMsgDlg_Domodal(_T("Please Move In the socket"), MODE_INFORMATION);
				return FALSE;
			}
			UpdateData(TRUE);*/

			//선택 축이 모션 운동 중이면
			if (FAS_MO->IsMotioning(m_nSelectAxisNum))
			{
				strLog.Format(_T("[JOG MOVE FAIL] Axis %d is Moving. \n"), m_nSelectAxisNum);
				m_bFlagJogError = true;
				return FALSE;
			}

			if (m_nJogMode == 0)
			{
				// Velocity Move
				if (!FAS_MO->MoveVelocity((long)m_nSelectAxisNum, m_dMoveSpeed, 1))
				{					return FALSE;
				}
			}
			else
			{
				if (m_bJogBtnBlockFlag == TRUE)
					return TRUE;
				//m_bJogBtnBlockFlag = TRUE;
				//dTargetPos = (m_dMoveDistance * 500);
				//if (m_nSelectAxisNum == 1) dTargetPos *= 20;
				//// Distance
				//if (!FAS_MO->SingleAxisIncMove((long)m_nSelectAxisNum, dTargetPos, (int)m_dMoveSpeed))
				//{
				//	return FALSE;
				//}
				PostMessage(MESSAGE_STEP_MOVE, m_nSelectAxisNum, +1);
			}
		}
		if (pMsg->hwnd == GetDlgItem(IDC_BTN_MOTOR_ALARMRESET)->GetSafeHwnd())
		{
			m_btnMotorState[3].SetStaticStyle(CVGStatic::StaticStyle_Data);
			m_btnMotorState[3].SetColorStyle(CVGStatic::ColorStyle_Green);
			m_btnMotorState[3].SetFont_Gdip(L"Arial", 9.0F);
		}
		if (pMsg->hwnd == GetDlgItem(IDC_BTN_MOTOR_STOP)->GetSafeHwnd())
		{
			m_btnMotorState[2].SetStaticStyle(CVGStatic::StaticStyle_Data);
			m_btnMotorState[2].SetColorStyle(CVGStatic::ColorStyle_Green);
			m_btnMotorState[2].SetFont_Gdip(L"Arial", 9.0F);
		}
	}
	break;

	case WM_LBUTTONUP:
	{
		if ((pMsg->hwnd == GetDlgItem(IDC_BTN_JOG_MOVE_1)->GetSafeHwnd()))
		{
			m_bJogBtnBlockFlag = FALSE;
			//if (FAS_MO->IsMotioning(m_nSelectAxisNum) && m_nJogMode == 1)
			//{
			//	return TRUE;
			//}
			//선택 축이 모션 운동 중이면
			if (m_bFlagJogError)
			{
				m_bFlagJogError = FALSE;
				return FALSE;
			}

			if (!FAS_MO->MoveStop(m_nSelectAxisNum))
			{
				return FALSE;
			}
		}

		if ((pMsg->hwnd == GetDlgItem(IDC_BTN_JOG_MOVE_2)->GetSafeHwnd()))
		{
			m_bJogBtnBlockFlag = FALSE;
			//if (FAS_MO->IsMotioning(m_nSelectAxisNum) && m_nJogMode == 1)
			//{
			//	return TRUE;
			//}

			//선택 축이 모션 운동 중이면
			if (m_bFlagJogError)
			{
				m_bFlagJogError = FALSE;
				return FALSE;
			}

			if (!FAS_MO->MoveStop(m_nSelectAxisNum))
			{
				return FALSE;
			}
		}

		if (pMsg->hwnd == GetDlgItem(IDC_BTN_MOTOR_ALARMRESET)->GetSafeHwnd())
		{
			m_btnMotorState[3].SetStaticStyle(CVGStatic::StaticStyle_Data);
			m_btnMotorState[3].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
			m_btnMotorState[3].SetFont_Gdip(L"Arial", 9.0F);
		}
		if (pMsg->hwnd == GetDlgItem(IDC_BTN_MOTOR_STOP)->GetSafeHwnd())
		{
			m_btnMotorState[2].SetStaticStyle(CVGStatic::StaticStyle_Data);
			m_btnMotorState[2].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
			m_btnMotorState[2].SetFont_Gdip(L"Arial", 9.0F);
		}
	}
	break;
	}

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
void CDlgMotorTeach::OnLButtonDown(UINT nFlags, CPoint point)
{
	// [21.1021.6]
	CLuriMotion_OQA_SFR_WideDlg *pParrentDlg = (CLuriMotion_OQA_SFR_WideDlg*)AfxGetApp()->GetMainWnd();
	pParrentDlg->SendMessage(MESSAGE_UI_WINDOW_TOP, NULL, NULL);
	COMMON->SetSleepModeStep(SLEEP_OFF);

	CDialogEx::OnLButtonDown(nFlags, point);
}

LRESULT CDlgMotorTeach::OnStepMove(WPARAM wParam, LPARAM lParam)
{
	CString strDistance = _T("");
	double dTargetPos = 0.0f;
	m_edInput_JogDistance.GetWindowText(strDistance);
	dTargetPos = ((m_dMoveDistance = _wtof(strDistance))* lParam);

	if (fabs(dTargetPos) > 10.0)
	{
		COMMON->ShowMsgDlg_Domodal(_T("Step Input range is over, Motor cannot being moved"), MODE_INFORMATION);
		return FALSE;
	}

	m_bJogBtnBlockFlag = TRUE;

	// Distance
	if (m_nSelectAxisNum == 0)
		dTargetPos = dTargetPos * PULSE_PER_UNIT_Y;
	else
		dTargetPos = dTargetPos * PULSE_PER_UNIT_Z;

	if (!FAS_MO->SingleAxisIncMove((long)wParam, dTargetPos, (DWORD)m_dMoveSpeed))
	{
		return FALSE;
	}
	return TRUE;
}

void CDlgMotorTeach::OnTimer(UINT_PTR nIDEvent)
{
	CDialogEx::OnTimer(nIDEvent);

	switch (nIDEvent)
	{
	case TEACH_TIMER_MOTOR_STATUS:
	{
		double dPos = 0.0f;
		CString strTemp = _T("");
		if (FAS_MO->IsConnected() == TRUE )
		{
			for (int i = 0; i < (int)MOTOR_MAX_COUNT; i++)
			{
				dPos = FAS_MO->GetAxisCurrnetActualPos(i);

				strTemp.Format(_T("%0.3f"), dPos);

				m_stMotorCurPos[1][i].SetFont_Gdip(L"Arial", 9.0F);
				m_stMotorCurPos[1][i].SetText(strTemp);
			}


			if (m_nSelectAxisNum >= 0 && m_nSelectAxisNum < (int)MOTOR_MAX_COUNT)
			{
				if (FAS_MO->IsServoStatus(m_nSelectAxisNum) == TRUE)
					SetAxisStatus(FAS_MO->GetAxisStatus(m_nSelectAxisNum));
			}
		}
	}
	break;
	}
}

BOOL CDlgMotorTeach::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(83, 169, 155));
	//pDC->FillSolidRect(rect, RGB(227, 255, 0));		// Lime
	return TRUE;
}

int CDlgMotorTeach::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	

	return 0;
}
BOOL CDlgMotorTeach::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	InitComponent_MotorTeach();
	InitComponent_MotorTeach_EditBox();

	InitComponent_AxisY_Teach_EditBox();
	InitComponent_AxisZ_Teach_EditBox();

	EqpType_UI();

	// [21.1017.16]
	ChangeUI_Level();
	// [21.1027.3]
	ChangeUI_Jog(m_nJogMode);
	// [21.1104.3]
	m_dMoveSpeed = 2.0f;
	m_dMoveDistance = 1.0f;
	m_bFlagJogError = FALSE;
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgMotorTeach::EqpType_UI()
{
	auto eqp = GET_INSPECTOR;

	std::vector<int>	vHide;

	switch (eqp)
	{
		case SYS_DISTORTION_30:
		{
			vHide.push_back(IDC_STC_SOCKET_Z_POS);
			vHide.push_back(IDC_STC_Z_POS_1);
			vHide.push_back(IDC_EDIT_Z_POS_1);
			vHide.push_back(IDC_BTN_TEACHING_POS_Y_4);
			vHide.push_back(IDC_BTN_TEACHING_MOVE_Y_4);
			vHide.push_back(IDC_STC_Z_POS_2);
			vHide.push_back(IDC_EDIT_Z_POS_2);
			vHide.push_back(IDC_BTN_TEACHING_POS_Y_5);
			vHide.push_back(IDC_BTN_TEACHING_MOVE_Y_5);
			vHide.push_back(IDC_STC_Z_POS_3);
			vHide.push_back(IDC_EDIT_Z_POS_3);
			vHide.push_back(IDC_BTN_TEACHING_POS_Y_6);
			vHide.push_back(IDC_BTN_TEACHING_MOVE_Y_6);
			vHide.push_back(IDC_STC_Z_POS_4);
			vHide.push_back(IDC_EDIT_Z_POS_4);
			vHide.push_back(IDC_BTN_TEACHING_POS_Y_7);
			vHide.push_back(IDC_STC_AXIS_02);
			vHide.push_back(IDC_STC_AXIS_CURPOS_02);
			vHide.push_back(IDC_BTN_JOG_SELECT_AXIS_2);
			vHide.push_back(IDC_BTN_SPEED_AXIS_SELECT_2);

			vHide.push_back(IDC_STC_Y_POS_2);
			vHide.push_back(IDC_EDIT_Y_POS_2);
			vHide.push_back(IDC_BTN_TEACHING_POS_Y_2);
			vHide.push_back(IDC_BTN_TEACHING_MOVE_Y_2);
			vHide.push_back(IDC_STC_Y_POS_3);
			vHide.push_back(IDC_EDIT_Y_POS_3);
			vHide.push_back(IDC_BTN_TEACHING_POS_Y_3);
			vHide.push_back(IDC_BTN_TEACHING_MOVE_Y_3);
			break;
		}

		default:
		{
			vHide.push_back(IDC_STC_SOCKET_Y_POS2);
			vHide.push_back(IDC_STC_Y_POS_4);
			vHide.push_back(IDC_EDIT_Y_POS_4);
			vHide.push_back(IDC_BTN_TEACHING_POS_Y_8);
			vHide.push_back(IDC_BTN_TEACHING_MOVE_Y_8);
			vHide.push_back(IDC_STC_Y_POS_5);
			vHide.push_back(IDC_EDIT_Y_POS_5);
			vHide.push_back(IDC_BTN_TEACHING_POS_Y_9);
			vHide.push_back(IDC_BTN_TEACHING_MOVE_Y_9);

			vHide.push_back(IDC_STC_Y_POS_6);
			vHide.push_back(IDC_EDIT_Y_POS_6);
			vHide.push_back(IDC_BTN_TEACHING_POS_Y_10);
			vHide.push_back(IDC_BTN_TEACHING_MOVE_Y_10);

			vHide.push_back(IDC_STC_Y_POS_7);
			vHide.push_back(IDC_EDIT_Y_POS_7);
			vHide.push_back(IDC_BTN_TEACHING_POS_Y_11);
			vHide.push_back(IDC_BTN_TEACHING_MOVE_Y_11);

			vHide.push_back(IDC_STC_Y_POS_8);
			vHide.push_back(IDC_EDIT_Y_POS_8);
			vHide.push_back(IDC_BTN_TEACHING_POS_Y_12);
			vHide.push_back(IDC_BTN_TEACHING_MOVE_Y_12);
		}
			break;
	}

	for (auto & ref : vHide)
		(CWnd*)GetDlgItem(ref)->ShowWindow(SW_HIDE);
}

void CDlgMotorTeach::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (bShow)
	{
		SetTimer(TEACH_TIMER_MOTOR_STATUS, 250, NULL);
		Load_TeachingData();
		CheckSelectJogMode();
		CheckSelectDistance();
		CheckSelectAxis();
		CheckSelectSpeed();
		CheckSelectMotorSpeed_AxisNum();
		if (FAS_MO->IsConnected() == TRUE)
		{
			GetDlgItem(IDC_COMBO_PORTNO1)->EnableWindow(FALSE);
			GetDlgItem(IDC_COMBO_BAUDRATE1)->EnableWindow(FALSE);
			ChangeConnectUI(TRUE);
			if (m_nSelectAxisNum < 0) m_nSelectAxisNum = 0;
				
				ChangeServoUI(TRUE);
				m_btnMotorJogSelect_Axis[m_nSelectAxisNum].SetStaticStyle(CVGStatic::StaticStyle_Data);
				m_btnMotorJogSelect_Axis[m_nSelectAxisNum].SetColorStyle(CVGStatic::ColorStyle_Green);
				m_btnMotorJogSelect_Axis[m_nSelectAxisNum].SetFont_Gdip(L"Arial", 9.0F);
		}
	}
	else
	{
		KillTimer(TEACH_TIMER_MOTOR_STATUS);
	}
}
void CDlgMotorTeach::InitComponent_MotorTeach_EditBox()
{
	CClientDC dc(GetDlgItem(IDC_EDIT1));
	CRect rt;

	GetDlgItem(IDC_EDIT1)->GetClientRect(&rt);

	rt.left += 5;
	rt.right -= 5;

	rt.top += 7;
	rt.bottom -= 7;

	((CEdit*)GetDlgItem(IDC_EDIT1))->SetRect(&rt);

	UpdateData(FALSE);
}
void CDlgMotorTeach::InitComponent_AxisY_Teach_EditBox()
{
	CClientDC dc1(GetDlgItem(IDC_EDIT_Y_POS_1));
	CClientDC dc2(GetDlgItem(IDC_EDIT_Y_POS_2));
	CClientDC dc3(GetDlgItem(IDC_EDIT_Y_POS_3));
	CRect rt1;
	CRect rt2;
	CRect rt3;

	GetDlgItem(IDC_EDIT_Y_POS_1)->GetClientRect(&rt1);

	rt1.left += 30;
	rt1.right -= 30;

	rt1.top += 7;
	rt1.bottom -= 7;

	((CEdit*)GetDlgItem(IDC_EDIT_Y_POS_1))->SetRect(&rt1);

	GetDlgItem(IDC_EDIT_Y_POS_2)->GetClientRect(&rt2);

	rt2.left += 30;
	rt2.right -= 30;

	rt2.top += 7;
	rt2.bottom -= 7;

	((CEdit*)GetDlgItem(IDC_EDIT_Y_POS_2))->SetRect(&rt2);

	GetDlgItem(IDC_EDIT_Y_POS_3)->GetClientRect(&rt3);

	rt3.left += 30;
	rt3.right -= 30;

	rt3.top += 7;
	rt3.bottom -= 7;

	((CEdit*)GetDlgItem(IDC_EDIT_Y_POS_3))->SetRect(&rt3);

	UpdateData(FALSE);
}

void CDlgMotorTeach::InitComponent_AxisZ_Teach_EditBox()
{
	CClientDC dc1(GetDlgItem(IDC_EDIT_Z_POS_1));
	CClientDC dc2(GetDlgItem(IDC_EDIT_Z_POS_2));
	CClientDC dc3(GetDlgItem(IDC_EDIT_Z_POS_3));
	CClientDC dc4(GetDlgItem(IDC_EDIT_Z_POS_4));
	CRect rt1;
	CRect rt2;
	CRect rt3;
	CRect rt4;

	GetDlgItem(IDC_EDIT_Z_POS_1)->GetClientRect(&rt1);

	rt1.left += 30;
	rt1.right -= 30;

	rt1.top += 7;
	rt1.bottom -= 7;

	((CEdit*)GetDlgItem(IDC_EDIT_Z_POS_1))->SetRect(&rt1);

	GetDlgItem(IDC_EDIT_Z_POS_2)->GetClientRect(&rt2);

	rt2.left += 30;
	rt2.right -= 30;

	rt2.top += 7;
	rt2.bottom -= 7;

	((CEdit*)GetDlgItem(IDC_EDIT_Z_POS_2))->SetRect(&rt2);

	GetDlgItem(IDC_EDIT_Z_POS_3)->GetClientRect(&rt3);

	rt3.left += 30;
	rt3.right -= 30;

	rt3.top += 7;
	rt3.bottom -= 7;

	((CEdit*)GetDlgItem(IDC_EDIT_Z_POS_3))->SetRect(&rt3);

	GetDlgItem(IDC_EDIT_Z_POS_3)->GetClientRect(&rt4);

	rt4.left += 30;
	rt4.right -= 30;

	rt4.top += 7;
	rt4.bottom -= 7;

	((CEdit*)GetDlgItem(IDC_EDIT_Z_POS_4))->SetRect(&rt4);

	UpdateData(FALSE);
}

void CDlgMotorTeach::InitComponent_MotorTeach()
{
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();

	CString strTemp = _T("");
	for (int i = 0; i < 5; i++)
	{
		m_stMotorTitle[i].SetStaticStyle(CVGStatic::StaticStyle_GroupHeader);
		m_stMotorTitle[i].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_stMotorTitle[i].SetFont_Gdip(L"Arial", 9.0F);

		if (stEq.nLanguage == 0)
			m_stMotorTitle[i].SetText(m_szTeachTitleKor[i]);
		else
			m_stMotorTitle[i].SetText(m_szTeachTitleEng[i]);
	}

	//--------------------------------------------------------------------------
	// 현재위치
	for (int i = 0; i < 10; i++)
	{
		if (i < MOTOR_MAX_COUNT)
		{
			m_stMotorCurPos[0][i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
			m_stMotorCurPos[0][i].SetColorStyle(CVGStatic::ColorStyle_Turquois);

			m_stMotorCurPos[0][i].SetFont_Gdip(L"Arial", 9.0F);

			if (stEq.nLanguage == 0)
				m_stMotorCurPos[0][i].SetText(m_szTeachCurPosAxisnNameKor[i]);
			else
				m_stMotorCurPos[0][i].SetText(m_szTeachCurPosAxisnNameEng[i]);

		}
		else
		{
			m_stMotorCurPos[0][i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
			m_stMotorCurPos[0][i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		}


	}

	for (int i = 0; i < 10; i++)
	{
		strTemp = _T("0.0");

		if (i < MOTOR_MAX_COUNT)
		{
			m_stMotorCurPos[1][i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
			m_stMotorCurPos[1][i].SetColorStyle(CVGStatic::ColorStyle_Turquois);

			m_stMotorCurPos[1][i].SetFont_Gdip(L"Arial", 9.0F);
			m_stMotorCurPos[1][i].SetText(strTemp);
		}
		else
		{
			m_stMotorCurPos[1][i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
			m_stMotorCurPos[1][i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		}


	}


	//--------------------------------------------------------------------------
	// 조그이동
	for (int i = 0; i < 4; i++)
	{
		m_stMotorJogTitleSub[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stMotorJogTitleSub[i].SetColorStyle(CVGStatic::ColorStyle_Turquois);
		m_stMotorJogTitleSub[i].SetFont_Gdip(L"Arial", 9.0F);

		if (stEq.nLanguage == 0)
			m_stMotorJogTitleSub[i].SetText(m_szTeachJogKor[i]);
		else
			m_stMotorJogTitleSub[i].SetText(m_szTeachJogEng[i]);
	}

	for (int i = 0; i < 2; i++)
	{
		m_btnMotorJogSelect_Mode[i].SetStaticStyle(CVGStatic::StaticStyle_Data);
		m_btnMotorJogSelect_Mode[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_btnMotorJogSelect_Mode[i].SetFont_Gdip(L"Arial", 9.0F);

		if (stEq.nLanguage == 0)
			m_btnMotorJogSelect_Mode[i].SetText(m_szTeachJogBtnKor[i]);
		else
			m_btnMotorJogSelect_Mode[i].SetText(m_szTeachJogBtnEng[i]);
	}

	for (int i = 0; i < 10; i++)
	{
		m_btnMotorJogSelect_Axis[i].SetStaticStyle(CVGStatic::StaticStyle_Data);
		m_btnMotorJogSelect_Axis[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_btnMotorJogSelect_Axis[i].SetFont_Gdip(L"Arial", 9.0F);

		if (stEq.nLanguage == 0)
			m_btnMotorJogSelect_Axis[i].SetText(m_szTeachJogBtnKor[2 + i]);
		else
			m_btnMotorJogSelect_Axis[i].SetText(m_szTeachJogBtnEng[2 + i]);

	}

	for (int i = 0; i < 5; i++)
	{
		m_btnMotorJogSelect_Distance[i].SetStaticStyle(CVGStatic::StaticStyle_Data);
		m_btnMotorJogSelect_Distance[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_btnMotorJogSelect_Distance[i].SetFont_Gdip(L"Arial", 9.0F);

		if (stEq.nLanguage == 0)
			m_btnMotorJogSelect_Distance[i].SetText(m_szTeachJogBtnKor[12 + i]);
		else
			m_btnMotorJogSelect_Distance[i].SetText(m_szTeachJogBtnEng[12 + i]);
	}

	for (int i = 0; i < 3; i++)
	{
		m_btnMotorJogSelect_Speed[i].SetStaticStyle(CVGStatic::StaticStyle_Data);
		m_btnMotorJogSelect_Speed[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_btnMotorJogSelect_Speed[i].SetFont_Gdip(L"Arial", 9.0F);

		if (stEq.nLanguage == 0)
			m_btnMotorJogSelect_Speed[i].SetText(m_szTeachJogBtnKor[17 + i]);
		else
			m_btnMotorJogSelect_Speed[i].SetText(m_szTeachJogBtnEng[17 + i]);
	}

	// [21.1017.18] Modified
	for (int i = 0; i < 2; i++)
	{
		m_btnMotorJogSelect_Move[i].SetTooltip(_T("JOGGING"));
		m_btnMotorJogSelect_Move[i].SetTextColor(CVGStatic::ColorStyle_Red);
		//m_btnMotorJogSelect_Move[i].SetFont_Gdip(L"Arial", 9.0F);

		if (stEq.nLanguage == 0)
			m_btnMotorJogSelect_Move[i].SetWindowText(m_szTeachJogBtnKor[20 + i]);
		else
			m_btnMotorJogSelect_Move[i].SetWindowText(m_szTeachJogBtnEng[20 + i]);
	}

	m_btnMotorJogSelect_Home.SetTooltip(_T("HOMMING"));
	m_btnMotorJogSelect_Home.SetTextColor(CVGStatic::ColorStyle_Red);
	//m_btnMotorJogSelect_Home.SetFont_Gdip(L"Arial", 9.0F);

	if (stEq.nLanguage == 0)
		m_btnMotorJogSelect_Home.SetWindowText(m_szTeachJogBtnKor[22]);
	else
		m_btnMotorJogSelect_Home.SetWindowText(m_szTeachJogBtnEng[22]);

	//--------------------------------------------------------------------------
	// 메뉴얼 UI
	for (int i = 0; i < 8; i++)
	{
		m_stMotorManual[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stMotorManual[i].SetColorStyle(CVGStatic::ColorStyle_Turquois);
		m_stMotorManual[i].SetFont_Gdip(L"Arial", 9.0F);

		if (stEq.nLanguage == 0)
			m_stMotorManual[i].SetText(m_szTeachCylinderKor[i]);
		else
			m_stMotorManual[i].SetText(m_szTeachCylinderEng[i]);
	}
	if (stEq.nLanguage == 0)
	{
		GetDlgItem(IDC_BTN_TEACH_SAVE_1)->SetWindowTextW(_T("저장"));
		GetDlgItem(IDC_BTN_TEACH_SAVE_2)->SetWindowTextW(_T("다른이름으로 저장"));
	}
	else
	{
		GetDlgItem(IDC_BTN_TEACH_SAVE_1)->SetWindowTextW(_T("Save"));
		GetDlgItem(IDC_BTN_TEACH_SAVE_2)->SetWindowTextW(_T("Save as"));
	}

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			m_btnMotorManual[i][j].SetStaticStyle(CVGStatic::StaticStyle_Data);
			m_btnMotorManual[i][j].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
			m_btnMotorManual[i][j].SetFont_Gdip(L"Arial", 9.0F);

			if (stEq.nLanguage == 0)
			{
				if (i == 0)
					m_btnMotorManual[i][j].SetText(_T("잠금"));
				else
					m_btnMotorManual[i][j].SetText(_T("열림"));
			}
			else
			{
				if (i == 0)
					m_btnMotorManual[i][j].SetText(_T("Lock"));
				else
					m_btnMotorManual[i][j].SetText(_T("Unlock"));
			}

		}
	}
	for (int nIndex = 0; nIndex < 4; nIndex++)
	{
		m_btnMotorState[nIndex].SetStaticStyle(CVGStatic::StaticStyle_Data);
		m_btnMotorState[nIndex].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_btnMotorState[nIndex].SetFont_Gdip(L"Arial", 9.0F);
	}

	if (stEq.nLanguage == 0)
	{
		m_btnMotorState[0].SetText(_T("Servo On"));
		m_btnMotorState[1].SetText(_T("Servo Off"));
		m_btnMotorState[2].SetText(_T("정지"));
		m_btnMotorState[3].SetText(_T("알람 리셋"));
	}
	else
	{
		m_btnMotorState[0].SetText(_T("Servo On"));
		m_btnMotorState[1].SetText(_T("Servo Off"));
		m_btnMotorState[2].SetText(_T("Stop"));
		m_btnMotorState[3].SetText(_T("Alarm Reset"));
	}

		m_stMotorConnect[0].SetStaticStyle(CVGStatic::StaticStyle_Data);
		m_stMotorConnect[0].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_stMotorConnect[0].SetFont_Gdip(L"Arial", 9.0F);

		m_stMotorConnect[1].SetStaticStyle(CVGStatic::StaticStyle_Data);
		m_stMotorConnect[1].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_stMotorConnect[1].SetFont_Gdip(L"Arial", 9.0F);

		if (stEq.nLanguage == 0)
		{
				m_stMotorConnect[0].SetText(_T("연결"));
				m_stMotorConnect[1].SetText(_T("해제"));
		}
		else
		{
				m_stMotorConnect[0].SetText(_T("Connect"));
				m_stMotorConnect[1].SetText(_T("Disconnect"));
		}

		

		// Port
		m_cbFASBoard_Port.ResetContent();
		m_cbFASBoard_BaudRate.ResetContent();

		COMMCONFIG lpCC;
		CString str, str1;
		unsigned long ccsize = sizeof(COMMCONFIG);
		int index = 0;
		BOOL bExistFlag = FALSE;


		for (int i = 1; i < 15; i++)
		{
			str = _T("COM");
			str1.Format(_T("%d"), i);
			str += str1;
			bExistFlag = FALSE;

			// 중요 포인트 return값이 TRUE이면 COMPORT존재
			bExistFlag = GetDefaultCommConfig(str, &lpCC, &ccsize);

			//if (bExistFlag)
			{
				index = m_cbFASBoard_Port.AddString(str);
				m_cbFASBoard_Port.SetItemData(index, i);

			}
		}
		TCHAR *BaudList_Motor[] = { _T("9600"),_T("19200"),_T("38400"),_T("57600"),_T("115200") };

		for (int i = 0; i < 5; i++)
		{
			str.Format(_T("%s"), BaudList_Motor[i]);
			index = m_cbFASBoard_BaudRate.AddString(str);
			m_cbFASBoard_BaudRate.SetItemData(index, i);

		}



	//--------------------------------------------------------------------------
	// Speed Setting

	for (int i = 0; i < 2; i++)
	{
		m_stMotorSpeedSet_Sub[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stMotorSpeedSet_Sub[i].SetColorStyle(CVGStatic::ColorStyle_Turquois);
		m_stMotorSpeedSet_Sub[i].SetFont_Gdip(L"Arial", 9.0F);

		if (stEq.nLanguage == 0)
			m_stMotorSpeedSet_Sub[i].SetText(m_szTeachMotorSpeedKor[i]);
		else
			m_stMotorSpeedSet_Sub[i].SetText(m_szTeachMotorSpeedEng[i]);
	}

	for (int i = 0; i < 10; i++)
	{
		strTemp.Format(_T("%d"), i);

		m_btnMotorSpeedSet_AxisNum[i].SetStaticStyle(CVGStatic::StaticStyle_Data);
		m_btnMotorSpeedSet_AxisNum[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_btnMotorSpeedSet_AxisNum[i].SetFont_Gdip(L"Arial", 9.0F);
		m_btnMotorSpeedSet_AxisNum[i].SetText(strTemp);
	}

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			if (j == 0)
			{
				m_stMotorSpeedSet_Thirdsub[i][j].SetStaticStyle(CVGStatic::StaticStyle_Data);
				m_stMotorSpeedSet_Thirdsub[i][j].SetColorStyle(CVGStatic::ColorStyle_SkyBlue);
			}
			else
			{
				m_stMotorSpeedSet_Thirdsub[i][j].SetStaticStyle(CVGStatic::StaticStyle_Data);
				m_stMotorSpeedSet_Thirdsub[i][j].SetColorStyle(CVGStatic::ColorStyle_White);
			}

			m_stMotorSpeedSet_Thirdsub[i][j].SetFont_Gdip(L"Arial", 9.0F);

			if (stEq.nLanguage == 0)
			{
				if (j == 0)
					m_stMotorSpeedSet_Thirdsub[i][j].SetText(m_szTeachMotorSpeedKor[i + 2]);
			}
			else
			{
				if (j == 0)
					m_stMotorSpeedSet_Thirdsub[i][j].SetText(m_szTeachMotorSpeedEng[i + 2]);
			}
		}
	}


	//--------------------------------------------------------------------------
	// Teaching  
	m_stTeach_Y.SetStaticStyle(CVGStatic::StaticStyle_GroupHeader);
	m_stTeach_Y.SetColorStyle(CVGStatic::ColorStyle_Black);
	m_stTeach_Y.SetFont_Gdip(L"Arial", 9.0F);
	m_stTeach_Y.SetText(_T("SOCKET Y AXIS"));

	m_stTeach_Z.SetStaticStyle(CVGStatic::StaticStyle_GroupHeader);
	m_stTeach_Z.SetColorStyle(CVGStatic::ColorStyle_Black);
	m_stTeach_Z.SetFont_Gdip(L"Arial", 9.0F);
	m_stTeach_Z.SetText(_T("SOCKET Z AXIS"));

	m_stModeChange.SetStaticStyle(CVGStatic::StaticStyle_GroupHeader);
	m_stModeChange.SetColorStyle(CVGStatic::ColorStyle_Black);
	m_stModeChange.SetFont_Gdip(L"Arial", 9.0F);
	if (stEq.nLanguage == 0)
		m_stModeChange.SetText(_T("모드 변경 시간"));
	else
		m_stModeChange.SetText(_T("WAIT TIME [Unit : min]"));

	for (int i = 0; i < 3; i++)
	{
		m_stTeach_Y_Sub[i].SetStaticStyle(CVGStatic::StaticStyle_GroupHeader);
		m_stTeach_Y_Sub[i].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_stTeach_Y_Sub[i].SetFont_Gdip(L"Arial", 9.0F);
		if (stEq.nLanguage == 0)
			m_stTeach_Y_Sub[i].SetText(m_szTeachTrinity_AxisList_Kor[i + 1]);
		else
			m_stTeach_Y_Sub[i].SetText(m_szTeachTrinity_AxisList_Eng[i + 1]);
	}
	for (int i = 0; i < 4; i++)
	{
		m_stTeach_Z_Sub[i].SetStaticStyle(CVGStatic::StaticStyle_GroupHeader);
		m_stTeach_Z_Sub[i].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_stTeach_Z_Sub[i].SetFont_Gdip(L"Arial", 9.0F);
		if (stEq.nLanguage == 0)
			m_stTeach_Z_Sub[i].SetText(m_szTeachTrinity_AxisList_Kor[i + 1]);
		else
			m_stTeach_Z_Sub[i].SetText(m_szTeachTrinity_AxisList_Eng[i + 1]);
	}

	for (int i = 0; i < 3; i++)
	{
		m_stStatic_Text[i].SetStaticStyle(CVGStatic::StaticStyle_GroupHeader);
		m_stStatic_Text[i].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_stStatic_Text[i].SetFont_Gdip(L"Arial", 9.0F);
		if (i == 0) m_stStatic_Text[i].SetText(_T("ComPort"));
		if (i == 1) m_stStatic_Text[i].SetText(_T("BaudRate"));
		if (i == 2)
		{
			if (stEq.nLanguage == 0)
				m_stStatic_Text[i].SetText(_T("축 1"));
			else
				m_stStatic_Text[i].SetText(_T("Axis 1"));
		}
	}

	for (int nState = 0; nState < AXIS_MOTOR_STATUES; nState++)
	{
		m_stStatic_Motor_Statuses[nState].SetStaticStyle(CVGStatic::StaticStyle_GroupHeader);
		m_stStatic_Motor_Statuses[nState].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_stStatic_Motor_Statuses[nState].SetFont_Gdip(L"Arial", 7.0F, FontStyleBoldItalic);
		m_stStatic_Motor_Statuses[nState].SetText(m_szTeachTrinity_AxisStatus_List[nState ]);
	}

	// Y축 포지션2
	m_stTeach_Y_Pos2.SetStaticStyle(CVGStatic::StaticStyle_GroupHeader);
	m_stTeach_Y_Pos2.SetColorStyle(CVGStatic::ColorStyle_Black);
	m_stTeach_Y_Pos2.SetFont_Gdip(L"Arial", 9.0F);
	m_stTeach_Y_Pos2.SetText(_T("SOCKET Y INTRISIC"));

	for (int i = 0; i < 5; i++)
	{
		m_stTeach_Y_SubPos2[i].SetStaticStyle(CVGStatic::StaticStyle_GroupHeader);
		m_stTeach_Y_SubPos2[i].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_stTeach_Y_SubPos2[i].SetFont_Gdip(L"Arial", 9.0F);
		m_stTeach_Y_SubPos2[i].SetText(m_szTeachIntrisic_AxisList_KorEng[i + 1]);
	}
}

void CDlgMotorTeach::ChangeUI_Language()
{
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();

	CString strTemp = _T("");
	for (int i = 0; i < 5; i++)
	{
		m_stMotorTitle[i].SetStaticStyle(CVGStatic::StaticStyle_GroupHeader);
		m_stMotorTitle[i].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_stMotorTitle[i].SetFont_Gdip(L"Arial", 9.0F);

		if( stEq.nLanguage == 0)
			m_stMotorTitle[i].SetText(m_szTeachTitleKor[i]);
		else
			m_stMotorTitle[i].SetText(m_szTeachTitleEng[i]);
	}

	//--------------------------------------------------------------------------
	// 현재위치
	for (int i = 0; i < MOTOR_MAX_COUNT; i++)
	{
		m_stMotorCurPos[0][i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stMotorCurPos[0][i].SetColorStyle(CVGStatic::ColorStyle_Turquois);
		m_stMotorCurPos[0][i].SetFont_Gdip(L"Arial", 9.0F);

		if (stEq.nLanguage == 0)
			m_stMotorCurPos[0][i].SetText(m_szTeachCurPosAxisnNameKor[i]);
		else
			m_stMotorCurPos[0][i].SetText(m_szTeachCurPosAxisnNameEng[i]);
	}
	if (stEq.nLanguage == 0)
	{
		GetDlgItem(IDC_BTN_TEACH_SAVE_1)->SetWindowTextW(_T("저장"));
		GetDlgItem(IDC_BTN_TEACH_SAVE_2)->SetWindowTextW(_T("다른이름으로 저장"));
	}
	else
	{
		GetDlgItem(IDC_BTN_TEACH_SAVE_1)->SetWindowTextW(_T("Save"));
		GetDlgItem(IDC_BTN_TEACH_SAVE_2)->SetWindowTextW(_T("Save as"));
	}
	for (int i = 0; i < MOTOR_MAX_COUNT; i++)
	{
		strTemp = _T("0.0");

		m_stMotorCurPos[1][i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stMotorCurPos[1][i].SetColorStyle(CVGStatic::ColorStyle_Turquois);
		m_stMotorCurPos[1][i].SetFont_Gdip(L"Arial", 9.0F);
		m_stMotorCurPos[1][i].SetText(strTemp);
	}

	for (int i = 0; i < 3; i++)
	{
		if (stEq.nLanguage == 0)
			m_stTeach_Y_Sub[i].SetText(m_szTeachTrinity_AxisList_Kor[i + 1]);
		else
			m_stTeach_Y_Sub[i].SetText(m_szTeachTrinity_AxisList_Eng[i + 1]);
	}
	for (int i = 0; i < 4; i++)
	{
		if (stEq.nLanguage == 0)
			m_stTeach_Z_Sub[i].SetText(m_szTeachTrinity_AxisList_Kor[i + 1]);
		else
			m_stTeach_Z_Sub[i].SetText(m_szTeachTrinity_AxisList_Eng[i + 1]);
	}
	//--------------------------------------------------------------------------
	// 조그이동
	for (int i = 0; i < 4; i++)
	{
		m_stMotorJogTitleSub[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stMotorJogTitleSub[i].SetColorStyle(CVGStatic::ColorStyle_Turquois);
		m_stMotorJogTitleSub[i].SetFont_Gdip(L"Arial", 9.0F);

		if (stEq.nLanguage == 0)
			m_stMotorJogTitleSub[i].SetText(m_szTeachJogKor[i]);
		else
			m_stMotorJogTitleSub[i].SetText(m_szTeachJogEng[i]);
	}

	for (int i = 0; i < 2; i++)
	{
		m_btnMotorJogSelect_Mode[i].SetStaticStyle(CVGStatic::StaticStyle_Data);
		m_btnMotorJogSelect_Mode[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_btnMotorJogSelect_Mode[i].SetFont_Gdip(L"Arial", 9.0F);

		if (stEq.nLanguage == 0)
			m_btnMotorJogSelect_Mode[i].SetText(m_szTeachJogBtnKor[i]);
		else
			m_btnMotorJogSelect_Mode[i].SetText(m_szTeachJogBtnEng[i]);
	}

	for (int i = 0; i < 10; i++)
	{
		m_btnMotorJogSelect_Axis[i].SetStaticStyle(CVGStatic::StaticStyle_Data);
		m_btnMotorJogSelect_Axis[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_btnMotorJogSelect_Axis[i].SetFont_Gdip(L"Arial", 9.0F);
		
		if (stEq.nLanguage == 0)
			m_btnMotorJogSelect_Axis[i].SetText(m_szTeachJogBtnKor[2 + i]);
		else
			m_btnMotorJogSelect_Axis[i].SetText(m_szTeachJogBtnEng[2 + i]);

	}

	for (int i = 0; i < 5; i++)
	{
		m_btnMotorJogSelect_Distance[i].SetStaticStyle(CVGStatic::StaticStyle_Data);
		m_btnMotorJogSelect_Distance[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_btnMotorJogSelect_Distance[i].SetFont_Gdip(L"Arial", 9.0F);

		if (stEq.nLanguage == 0)
			m_btnMotorJogSelect_Distance[i].SetText(m_szTeachJogBtnKor[12 + i]);
		else
			m_btnMotorJogSelect_Distance[i].SetText(m_szTeachJogBtnEng[12 + i]);
	}

	for (int i = 0; i < 3; i++)
	{
		m_btnMotorJogSelect_Speed[i].SetStaticStyle(CVGStatic::StaticStyle_Data);
		m_btnMotorJogSelect_Speed[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_btnMotorJogSelect_Speed[i].SetFont_Gdip(L"Arial", 9.0F);

		if (stEq.nLanguage == 0)
			m_btnMotorJogSelect_Speed[i].SetText(m_szTeachJogBtnKor[17 + i]);
		else
			m_btnMotorJogSelect_Speed[i].SetText(m_szTeachJogBtnEng[17 + i]);
	}

	// [21.1017.19] Modified
	for (int i = 0; i < 2; i++)
	{
		m_btnMotorJogSelect_Move[i].SetTooltip(_T("JOGGING"));;
		m_btnMotorJogSelect_Move[i].SetTextColor(CVGStatic::ColorStyle_Red);
		//m_btnMotorJogSelect_Move[i].SetFont_Gdip(L"Arial", 9.0F);

		if (stEq.nLanguage == 0)
			m_btnMotorJogSelect_Move[i].SetWindowText(m_szTeachJogBtnKor[20 + i]);
		else
			m_btnMotorJogSelect_Move[i].SetWindowText(m_szTeachJogBtnEng[20 + i]);
	}

	m_btnMotorJogSelect_Home.SetTooltip(_T("HOMMING"));
	m_btnMotorJogSelect_Home.SetTextColor(CVGStatic::ColorStyle_Red);
	//m_btnMotorJogSelect_Home.SetFont_Gdip(L"Arial", 9.0F);

	if (stEq.nLanguage == 0)
		m_btnMotorJogSelect_Home.SetWindowText(m_szTeachJogBtnKor[22]);
	else
		m_btnMotorJogSelect_Home.SetWindowText(m_szTeachJogBtnEng[22]);

	//--------------------------------------------------------------------------
	// 메뉴얼 UI
	for (int i = 0; i < 8; i++)
	{
		m_stMotorManual[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stMotorManual[i].SetColorStyle(CVGStatic::ColorStyle_Turquois);
		m_stMotorManual[i].SetFont_Gdip(L"Arial", 9.0F);
		
		if (stEq.nLanguage == 0)
			m_stMotorManual[i].SetText(m_szTeachCylinderKor[i]);
		else
			m_stMotorManual[i].SetText(m_szTeachCylinderEng[i]);
	}

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			m_btnMotorManual[i][j].SetStaticStyle(CVGStatic::StaticStyle_Data);
			m_btnMotorManual[i][j].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
			m_btnMotorManual[i][j].SetFont_Gdip(L"Arial", 9.0F);

			if (stEq.nLanguage == 0)
			{
				if (i == 0)
					m_btnMotorManual[i][j].SetText(_T("잠금"));
				else
					m_btnMotorManual[i][j].SetText(_T("열림"));
			}
			else
			{
				if (i == 0)
					m_btnMotorManual[i][j].SetText(_T("Lock"));
				else
					m_btnMotorManual[i][j].SetText(_T("Unlock"));
			}

		}
	}

	for (int i = 0; i < 3; i++)
	{
		m_stStatic_Text[i].SetStaticStyle(CVGStatic::StaticStyle_GroupHeader);
		m_stStatic_Text[i].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_stStatic_Text[i].SetFont_Gdip(L"Arial", 9.0F);
		if (i == 0) m_stStatic_Text[i].SetText(_T("ComPort"));
		if (i == 1) m_stStatic_Text[i].SetText(_T("BaudRate"));
		if (i == 2)
		{
			if (stEq.nLanguage == 0)
				m_stStatic_Text[i].SetText(_T("축 1"));
			else
				m_stStatic_Text[i].SetText(_T("Axis 1"));
		}
	}

	for (int nIndex = 0; nIndex < 4; nIndex++)
	{
		m_btnMotorState[nIndex].SetStaticStyle(CVGStatic::StaticStyle_Data);
		m_btnMotorState[nIndex].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_btnMotorState[nIndex].SetFont_Gdip(L"Arial", 9.0F);
	}

	if (stEq.nLanguage == 0)
	{
		m_btnMotorState[0].SetText(_T("Servo On"));
		m_btnMotorState[1].SetText(_T("Servo Off"));
		m_btnMotorState[2].SetText(_T("정지"));
		m_btnMotorState[3].SetText(_T("알람 리셋"));
	}
	else
	{
		m_btnMotorState[0].SetText(_T("Servo On"));
		m_btnMotorState[1].SetText(_T("Servo Off"));
		m_btnMotorState[2].SetText(_T("Stop"));
		m_btnMotorState[3].SetText(_T("Alarm Reset"));
	}

	m_stMotorConnect[0].SetStaticStyle(CVGStatic::StaticStyle_Data);
	m_stMotorConnect[0].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
	m_stMotorConnect[0].SetFont_Gdip(L"Arial", 9.0F);

	m_stMotorConnect[1].SetStaticStyle(CVGStatic::StaticStyle_Data);
	m_stMotorConnect[1].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
	m_stMotorConnect[1].SetFont_Gdip(L"Arial", 9.0F);

	if (stEq.nLanguage == 0)
	{
		m_stMotorConnect[0].SetText(_T("연결"));
		m_stMotorConnect[1].SetText(_T("해제"));
	}
	else
	{
		m_stMotorConnect[0].SetText(_T("Connect"));
		m_stMotorConnect[1].SetText(_T("Disconnect"));
	}


	m_stModeChange.SetStaticStyle(CVGStatic::StaticStyle_GroupHeader);
	m_stModeChange.SetColorStyle(CVGStatic::ColorStyle_Black);
	m_stModeChange.SetFont_Gdip(L"Arial", 9.0F);
	if (stEq.nLanguage == 0)
		m_stModeChange.SetText(_T("모드 변경 시간"));
	else
		m_stModeChange.SetText(_T("CHANGE MODE TIME"));

	//--------------------------------------------------------------------------
	// Speed Setting

	for (int i = 0; i < 2; i++)
	{
		m_stMotorSpeedSet_Sub[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stMotorSpeedSet_Sub[i].SetColorStyle(CVGStatic::ColorStyle_Turquois);
		m_stMotorSpeedSet_Sub[i].SetFont_Gdip(L"Arial", 9.0F);
		
		if (stEq.nLanguage == 0)
			m_stMotorSpeedSet_Sub[i].SetText(m_szTeachMotorSpeedKor[i]);
		else
			m_stMotorSpeedSet_Sub[i].SetText(m_szTeachMotorSpeedEng[i]);
	}

	for (int i = 0; i < 10; i++)
	{
		strTemp.Format(_T("%d"), i);

		m_btnMotorSpeedSet_AxisNum[i].SetStaticStyle(CVGStatic::StaticStyle_Data);
		m_btnMotorSpeedSet_AxisNum[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_btnMotorSpeedSet_AxisNum[i].SetFont_Gdip(L"Arial", 9.0F);
		m_btnMotorSpeedSet_AxisNum[i].SetText(strTemp);
	}

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			if (j == 0)
			{
				m_stMotorSpeedSet_Thirdsub[i][j].SetStaticStyle(CVGStatic::StaticStyle_Data);
				m_stMotorSpeedSet_Thirdsub[i][j].SetColorStyle(CVGStatic::ColorStyle_SkyBlue);
			}
			else
			{
				m_stMotorSpeedSet_Thirdsub[i][j].SetStaticStyle(CVGStatic::StaticStyle_Data);
				m_stMotorSpeedSet_Thirdsub[i][j].SetColorStyle(CVGStatic::ColorStyle_White);
			}

			m_stMotorSpeedSet_Thirdsub[i][j].SetFont_Gdip(L"Arial", 9.0F);

			if (stEq.nLanguage == 0)
			{
				if (j == 0)
					m_stMotorSpeedSet_Thirdsub[i][j].SetText(m_szTeachMotorSpeedKor[i + 2]);
			}
			else
			{
				if (j == 0)
					m_stMotorSpeedSet_Thirdsub[i][j].SetText(m_szTeachMotorSpeedEng[i + 2]);
			}
		}
	}
	OnShowWindow(1,1);
}

// [21.1017.20] Button Enable / Disable
void CDlgMotorTeach::ChangeUI_Level()
{
	int nLevel = COMMON->GetPermissionGrade();
	if (nLevel == LVL_ADMIN)
	{
		m_btnMotorJogSelect_Move[0].EnableWindow(TRUE);
		m_btnMotorJogSelect_Move[1].EnableWindow(TRUE);
		m_btnMotorJogSelect_Home.EnableWindow(TRUE);

		GetDlgItem(IDC_BTN_TEACHING_POS_Y_1)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_TEACHING_POS_Y_2)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_TEACHING_POS_Y_3)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_TEACHING_POS_Y_4)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_TEACHING_POS_Y_5)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_TEACHING_POS_Y_6)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_TEACHING_POS_Y_7)->EnableWindow(TRUE);
		

		GetDlgItem(IDC_BTN_TEACHING_MOVE_Y_1)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_Y_2)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_Y_3)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_Y_4)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_Y_5)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_Y_6)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_Y_7)->EnableWindow(TRUE);

		GetDlgItem(IDC_BTN_TEACH_SAVE_1)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_TEACH_SAVE_2)->EnableWindow(TRUE);

		GetDlgItem(IDC_EDIT_Y_POS_1)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_Y_POS_2)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_Y_POS_3)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_Z_POS_1)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_Z_POS_2)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_Z_POS_3)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_Z_POS_4)->EnableWindow(TRUE);

		GetDlgItem(IDC_STC_Y_POS_4)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_Y_POS_4)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_TEACHING_POS_Y_8)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_Y_8)->EnableWindow(TRUE);

		GetDlgItem(IDC_STC_Y_POS_5)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_Y_POS_5)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_TEACHING_POS_Y_9)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_Y_9)->EnableWindow(TRUE);

		GetDlgItem(IDC_STC_Y_POS_6)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_Y_POS_6)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_TEACHING_POS_Y_10)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_Y_10)->EnableWindow(TRUE);

		GetDlgItem(IDC_STC_Y_POS_7)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_Y_POS_7)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_TEACHING_POS_Y_11)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_Y_11)->EnableWindow(TRUE);

		GetDlgItem(IDC_STC_Y_POS_8)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_Y_POS_8)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_TEACHING_POS_Y_12)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_Y_12)->EnableWindow(TRUE);
	}
	else
	{
		m_btnMotorJogSelect_Move[0].EnableWindow(FALSE);
		m_btnMotorJogSelect_Move[1].EnableWindow(FALSE);
		m_btnMotorJogSelect_Home.EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_POS_Y_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_POS_Y_2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_POS_Y_3)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_POS_Y_4)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_POS_Y_5)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_POS_Y_6)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_POS_Y_7)->EnableWindow(FALSE);

		GetDlgItem(IDC_BTN_TEACHING_MOVE_Y_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_Y_2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_Y_3)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_Y_4)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_Y_5)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_Y_6)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_Y_7)->EnableWindow(FALSE);

		GetDlgItem(IDC_BTN_TEACH_SAVE_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACH_SAVE_2)->EnableWindow(FALSE);

		GetDlgItem(IDC_EDIT_Y_POS_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_Y_POS_2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_Y_POS_3)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_Z_POS_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_Z_POS_2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_Z_POS_3)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_Z_POS_4)->EnableWindow(FALSE);

		GetDlgItem(IDC_STC_Y_POS_4)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_Y_POS_4)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_POS_Y_8)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_Y_8)->EnableWindow(FALSE);

		GetDlgItem(IDC_STC_Y_POS_5)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_Y_POS_5)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_POS_Y_9)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_Y_9)->EnableWindow(FALSE);

		GetDlgItem(IDC_STC_Y_POS_6)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_Y_POS_6)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_POS_Y_10)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_Y_10)->EnableWindow(FALSE);

		GetDlgItem(IDC_STC_Y_POS_7)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_Y_POS_7)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_POS_Y_11)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_Y_11)->EnableWindow(FALSE);

		GetDlgItem(IDC_STC_Y_POS_8)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_Y_POS_8)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_POS_Y_12)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_Y_12)->EnableWindow(FALSE);
	}
}

// [21.1027.2] Button Enable / Disable
void CDlgMotorTeach::ChangeUI_Jog(int mode)
{
	BOOL bVisible1, bVisible2;
	if (mode == MOVE_DISTANCE)
	{
		bVisible1 = TRUE;
		bVisible2 = FALSE;
	}
	else
	{
		bVisible1 = FALSE;
		bVisible2 = TRUE;
	}

	for (int i = 0; i < 5; i++)
		m_btnMotorJogSelect_Distance[i].ShowWindow(bVisible1);
	m_edInput_JogDistance.ShowWindow(bVisible1);
	m_stMotorJogTitleSub[2].ShowWindow(bVisible1);

	for (int i = 0; i < 3; i++)
		m_btnMotorJogSelect_Speed[i].ShowWindow(bVisible2);
	m_stMotorJogTitleSub[3].ShowWindow(bVisible2);
}

//---------------------------------------------------------------------------------------------------
// Teaching Data Load/Save
void CDlgMotorTeach::Save_TeachingData()
{
	TEACHING_PARAM	stTeach = *DATAMANAGER->GetCurrentTeachingData();
	MOTION_PARAM	stSpeed = *DATAMANAGER->GetCurrentMotionParam();
	int index = 0;

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			stSpeed.nMotorSpeed[i][j] = m_nMotorSpeed[i][j];
		}
	}

	int nTemp = 0;
	double dtemp = 0.0f;
	
	UpdateData(TRUE);

	index = m_cbFASBoard_Port.GetCurSel();
	stSpeed.nPort[0] = (int)m_cbFASBoard_Port.GetItemData(index);
	index = m_cbFASBoard_BaudRate.GetCurSel();
	stSpeed.nBaudRate[0] = (int)m_cbFASBoard_BaudRate.GetItemData(index);
	// #.#
	// 모터 Y
	dtemp = _ttof(m_strTeach_AxisY_Pos[0]);
	nTemp = dtemp * PULSE_PER_UNIT_Y;
	stTeach.nSocketY_Wait_Pos = nTemp;

	dtemp = _ttof(m_strTeach_AxisY_Pos[1]);
	nTemp = dtemp * PULSE_PER_UNIT_Y;
	stTeach.nSocketY_Blemishi_Pos = nTemp;

	dtemp = _ttof(m_strTeach_AxisY_Pos[2]);
	nTemp = dtemp * PULSE_PER_UNIT_Y;
	stTeach.nSocketY_SFR_Pos = nTemp;

	//모터 Z
	dtemp = _ttof(m_strTeach_AxisZ_Pos[0]);
	nTemp = dtemp * PULSE_PER_UNIT_Z;
	stTeach.nSocketZ_Wait_Pos = nTemp;

	dtemp = _ttof(m_strTeach_AxisZ_Pos[1]);
	nTemp = dtemp * PULSE_PER_UNIT_Z;
	stTeach.nSocketZ_Blemishi_Pos = nTemp;

	dtemp = _ttof(m_strTeach_AxisZ_Pos[2]);
	nTemp = dtemp * PULSE_PER_UNIT_Z;
	stTeach.nSocketZ_SFR_180_Pos = nTemp;

	dtemp = _ttof(m_strTeach_AxisZ_Pos[3]);
	nTemp = dtemp * PULSE_PER_UNIT_Z;
	stTeach.nSocketZ_SFR_825_Pos = nTemp;

	// 모터Y Intrinsic cal
	for (auto i = 0; i < _countof(m_strTeach_AxisY_Pos2); i++)
	{
		dtemp = _ttof(m_strTeach_AxisY_Pos2[i]);
		nTemp = dtemp * PULSE_PER_UNIT_Y;
		stTeach.nSocketY_Intrinsic_Pos[i] = nTemp;
	}
	
	dtemp = _ttof(m_strTeach_AxisY_Pos[1]);
	nTemp = dtemp * PULSE_PER_UNIT_Y;
	stTeach.nSocketY_Blemishi_Pos = nTemp;

	dtemp = _ttof(m_strTeach_AxisY_Pos[2]);
	nTemp = dtemp * PULSE_PER_UNIT_Y;
	stTeach.nSocketY_SFR_Pos = nTemp;

	nTemp = _ttoi(m_strTeach_ModeChangeTime) * 1000 * 60;
	stTeach.nChangeMode_Time = nTemp;

	DATAMANAGER->SetMotionParam(stSpeed);
	DATAMANAGER->SetTeachingData(stTeach);
}

void CDlgMotorTeach::Load_TeachingData()
{
	TEACHING_PARAM	stTeach = *DATAMANAGER->GetCurrentTeachingData();
	MOTION_PARAM	stSpeed = *DATAMANAGER->GetCurrentMotionParam();

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			m_nMotorSpeed[i][j] = stSpeed.nMotorSpeed[i][j];
		}
	}

	CString strTemp = _T("");
	//모터 Y
	strTemp.Format(_T("%0.3f"), (double)((double)stTeach.nSocketY_Wait_Pos / PULSE_PER_UNIT_Y));
	m_strTeach_AxisY_Pos[0] = strTemp;

	strTemp.Format(_T("%0.3f"), (double)((double)stTeach.nSocketY_Blemishi_Pos / PULSE_PER_UNIT_Y));
	m_strTeach_AxisY_Pos[1] = strTemp;

	strTemp.Format(_T("%0.3f"), (double)((double)stTeach.nSocketY_SFR_Pos / PULSE_PER_UNIT_Y));
	m_strTeach_AxisY_Pos[2] = strTemp;

	//모터 Z
	strTemp.Format(_T("%0.3f"), (double)((double)stTeach.nSocketZ_Wait_Pos / PULSE_PER_UNIT_Z));
	m_strTeach_AxisZ_Pos[0] = strTemp;

	strTemp.Format(_T("%0.3f"), (double)((double)stTeach.nSocketZ_Blemishi_Pos / PULSE_PER_UNIT_Z));
	m_strTeach_AxisZ_Pos[1] = strTemp;

	strTemp.Format(_T("%0.3f"), (double)((double)stTeach.nSocketZ_SFR_180_Pos / PULSE_PER_UNIT_Z));
	m_strTeach_AxisZ_Pos[2] = strTemp;

	strTemp.Format(_T("%0.3f"), (double)((double)stTeach.nSocketZ_SFR_825_Pos / PULSE_PER_UNIT_Z));
	m_strTeach_AxisZ_Pos[3] = strTemp;

	strTemp.Format(_T("%0.1f"), (double)((double)stTeach.nChangeMode_Time / (1000 * 60)));
	m_strTeach_ModeChangeTime = strTemp;
	
	//모터 Y 2
	strTemp.Format(_T("%0.3f"), (double)((double)stTeach.nSocketY_Intrinsic_Pos[0] / PULSE_PER_UNIT_Y));
	m_strTeach_AxisY_Pos2[0] = strTemp;

	strTemp.Format(_T("%0.3f"), (double)((double)stTeach.nSocketY_Intrinsic_Pos[1] / PULSE_PER_UNIT_Y));
	m_strTeach_AxisY_Pos2[1] = strTemp;

	strTemp.Format(_T("%0.3f"), (double)((double)stTeach.nSocketY_Intrinsic_Pos[2] / PULSE_PER_UNIT_Y));
	m_strTeach_AxisY_Pos2[2] = strTemp;

	strTemp.Format(_T("%0.3f"), (double)((double)stTeach.nSocketY_Intrinsic_Pos[3] / PULSE_PER_UNIT_Y));
	m_strTeach_AxisY_Pos2[3] = strTemp;

	strTemp.Format(_T("%0.3f"), (double)((double)stTeach.nSocketY_Intrinsic_Pos[4] / PULSE_PER_UNIT_Y));
	m_strTeach_AxisY_Pos2[4] = strTemp;


	for(int i = 0; i < (int)MOTOR_MAX_COUNT; i++)
	{
		SetComboBox_Serial(&m_cbFASBoard_Port, stSpeed.nPort[i]);
		SetComboBox_Serial(&m_cbFASBoard_BaudRate, stSpeed.nBaudRate[i]);
	}

	UpdateData(FALSE);
}


void CDlgMotorTeach::OnBnClickedBtnJogSelectMode1()
{
	m_nJogMode = 0;		// 속도모드
	CheckSelectJogMode();
	ChangeUI_Jog(m_nJogMode);
}
void CDlgMotorTeach::OnBnClickedBtnJogSelectMode2()
{
	m_nJogMode = 1;		// 상대이동 모드
	CheckSelectJogMode();
	ChangeUI_Jog(m_nJogMode);
}
void CDlgMotorTeach::CheckSelectJogMode()
{
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();

	for (int i = 0; i < 2; i++)
	{
		if (i == m_nJogMode)
		{
			m_btnMotorJogSelect_Mode[i].SetStaticStyle(CVGStatic::StaticStyle_Data);
			m_btnMotorJogSelect_Mode[i].SetColorStyle(CVGStatic::ColorStyle_Green);
			m_btnMotorJogSelect_Mode[i].SetFont_Gdip(L"Arial", 9.0F);

			if( stEq.nLanguage == 0)
				m_btnMotorJogSelect_Mode[i].SetText(m_szTeachJogBtnKor[i]);
			else
				m_btnMotorJogSelect_Mode[i].SetText(m_szTeachJogBtnEng[i]);
		}
		else
		{
			m_btnMotorJogSelect_Mode[i].SetStaticStyle(CVGStatic::StaticStyle_Data);
			m_btnMotorJogSelect_Mode[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
			m_btnMotorJogSelect_Mode[i].SetFont_Gdip(L"Arial", 9.0F);

			if (stEq.nLanguage == 0)
				m_btnMotorJogSelect_Mode[i].SetText(m_szTeachJogBtnKor[i]);
			else
				m_btnMotorJogSelect_Mode[i].SetText(m_szTeachJogBtnEng[i]);
		}
	}
}

void CDlgMotorTeach::OnBnClickedBtnJogSelectAxis1()
{
	m_nSelectAxisNum = 0;	
	CheckSelectAxis();
}
void CDlgMotorTeach::OnBnClickedBtnJogSelectAxis2()
{
	m_nSelectAxisNum = 1;	
	CheckSelectAxis();
}
void CDlgMotorTeach::OnBnClickedBtnJogSelectAxis3()
{
	m_nSelectAxisNum = 2;	
	CheckSelectAxis();
}
void CDlgMotorTeach::OnBnClickedBtnJogSelectAxis4()
{
	m_nSelectAxisNum = 3;	
	CheckSelectAxis();
}
void CDlgMotorTeach::OnBnClickedBtnJogSelectAxis5()
{
	m_nSelectAxisNum = 4;	
	CheckSelectAxis();
}
void CDlgMotorTeach::OnBnClickedBtnJogSelectAxis6()
{
	m_nSelectAxisNum = 5;	
	CheckSelectAxis();
}
void CDlgMotorTeach::OnBnClickedBtnJogSelectAxis7()
{
	m_nSelectAxisNum = 6;	
	CheckSelectAxis();
}
void CDlgMotorTeach::OnBnClickedBtnJogSelectAxis8()
{
	m_nSelectAxisNum = 7;	
	CheckSelectAxis();
}
void CDlgMotorTeach::OnBnClickedBtnJogSelectAxis9()
{
	m_nSelectAxisNum = 8;	
	CheckSelectAxis();
}
void CDlgMotorTeach::OnBnClickedBtnJogSelectAxis10()
{
	m_nSelectAxisNum = 9;	
	CheckSelectAxis();
}

void CDlgMotorTeach::CheckSelectAxis()
{
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();
	CString strText = _T("");

	if (m_nSelectAxisNum >= (int)MOTOR_MAX_COUNT)
	{
		m_nSelectAxisNum = -1;
	}
	else
	{
		if (stEq.nLanguage == 0)
			strText.Format(_T("축 %d"), m_nSelectAxisNum + 1);
		else
			strText.Format(_T("Axis %d"), m_nSelectAxisNum + 1);
		m_stStatic_Text[2].SetText(strText);
	}

	for (int i = 0; i < 10; i++)
	{
		
		m_btnMotorJogSelect_Axis[i].SetStaticStyle(CVGStatic::StaticStyle_Data);

		if (i == m_nSelectAxisNum)
			m_btnMotorJogSelect_Axis[i].SetColorStyle(CVGStatic::ColorStyle_Green);
		else
			m_btnMotorJogSelect_Axis[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);

		m_btnMotorJogSelect_Axis[i].SetFont_Gdip(L"Arial", 9.0F);

		if( stEq.nLanguage == 0)
			m_btnMotorJogSelect_Axis[i].SetText(m_szTeachJogBtnKor[2 + i]);
		else
			m_btnMotorJogSelect_Axis[i].SetText(m_szTeachJogBtnEng[2 + i]);
	}

	if (m_nSelectAxisNum == -1)
	{
		//COMMON->ShowMsgDlg_Domodal(_T("조그이동에 사용할 모터 번호를 선택하세요."), MODE_INFORMATION);
		COMMON->ShowMsg_Thread(_T("조그이동에 사용할 모터 번호를 선택하세요."), MODE_INFORMATION);
	}	
}

void CDlgMotorTeach::OnBnClickedBtnJogDistance1()
{
	m_nSelectDistance = 0;
	m_dMoveDistance = 1.0f;

	CheckSelectDistance();
}
void CDlgMotorTeach::OnBnClickedBtnJogDistance2()
{
	m_nSelectDistance = 1;
	m_dMoveDistance = 3.0f;

	CheckSelectDistance();
}
void CDlgMotorTeach::OnBnClickedBtnJogDistance3()
{
	m_nSelectDistance = 2;
	m_dMoveDistance = 5.0f;

	CheckSelectDistance();
}
void CDlgMotorTeach::OnBnClickedBtnJogDistance4()
{
	m_nSelectDistance = 3;
	m_dMoveDistance = 10.0f;

	CheckSelectDistance();
}
void CDlgMotorTeach::OnBnClickedBtnJogDistance5()
{
	m_nSelectDistance = 4;

	CString strTemp = _T("");
	double dDistance = 0.0f;

	GetDlgItemText(IDC_EDIT1, strTemp);
	dDistance = _ttof(strTemp);

	m_dMoveDistance = dDistance;

	CheckSelectDistance();
}
void CDlgMotorTeach::CheckSelectDistance()
{
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();

	for (int i = 0; i < 5; i++)
	{
		if (m_nSelectDistance == i)
		{
			m_btnMotorJogSelect_Distance[i].SetStaticStyle(CVGStatic::StaticStyle_Data);
			m_btnMotorJogSelect_Distance[i].SetColorStyle(CVGStatic::ColorStyle_Green);
			m_btnMotorJogSelect_Distance[i].SetFont_Gdip(L"Arial", 9.0F);

			if (stEq.nLanguage == 0)
				m_btnMotorJogSelect_Distance[i].SetText(m_szTeachJogBtnKor[12 + i]);
			else
				m_btnMotorJogSelect_Distance[i].SetText(m_szTeachJogBtnEng[12 + i]);
		}
		else
		{
			m_btnMotorJogSelect_Distance[i].SetStaticStyle(CVGStatic::StaticStyle_Data);
			m_btnMotorJogSelect_Distance[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
			m_btnMotorJogSelect_Distance[i].SetFont_Gdip(L"Arial", 9.0F);

			if (stEq.nLanguage == 0)
				m_btnMotorJogSelect_Distance[i].SetText(m_szTeachJogBtnKor[12 + i]);
			else
				m_btnMotorJogSelect_Distance[i].SetText(m_szTeachJogBtnEng[12 + i]);
		}
	}
	CString str = _T("");
	str.Format(_T("%.3f"), m_dMoveDistance);
	m_edInput_JogDistance.SetWindowText(str);

}

void CDlgMotorTeach::OnBnClickedBtnJogSpeed1()
{
	m_nSelectSpeed = 0;
	m_dMoveSpeed = 2.0f;

	CheckSelectSpeed();
}
void CDlgMotorTeach::OnBnClickedBtnJogSpeed2()
{
	m_nSelectSpeed = 1;
	m_dMoveSpeed = 10.0f;

	CheckSelectSpeed();
}
void CDlgMotorTeach::OnBnClickedBtnJogSpeed3()
{
	m_nSelectSpeed = 2;
	m_dMoveSpeed = 50.0f;

	CheckSelectSpeed();
}

void CDlgMotorTeach::CheckSelectSpeed()
{
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();

	for (int i = 0; i < 3; i++)
	{
		if (m_nSelectSpeed == i)
		{
			m_btnMotorJogSelect_Speed[i].SetStaticStyle(CVGStatic::StaticStyle_Data);
			m_btnMotorJogSelect_Speed[i].SetColorStyle(CVGStatic::ColorStyle_Green);
			m_btnMotorJogSelect_Speed[i].SetFont_Gdip(L"Arial", 9.0F);

			if(stEq.nLanguage == 0 )
				m_btnMotorJogSelect_Speed[i].SetText(m_szTeachJogBtnKor[17 + i]);
			else
				m_btnMotorJogSelect_Speed[i].SetText(m_szTeachJogBtnEng[17 + i]);
		}
		else
		{
			m_btnMotorJogSelect_Speed[i].SetStaticStyle(CVGStatic::StaticStyle_Data);
			m_btnMotorJogSelect_Speed[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
			m_btnMotorJogSelect_Speed[i].SetFont_Gdip(L"Arial", 9.0F);

			if (stEq.nLanguage == 0)
				m_btnMotorJogSelect_Speed[i].SetText(m_szTeachJogBtnKor[17 + i]);
			else
				m_btnMotorJogSelect_Speed[i].SetText(m_szTeachJogBtnEng[17 + i]);
		}
	}
}

void CDlgMotorTeach::OnBnClickedBtnJogMove3()
{
	if (m_nSelectAxisNum < 0)
	{
		if (m_nSelectAxisNum == -1)
			COMMON->ShowMsgDlg_Domodal(_T("모터축이 선택되지 않았습니다. 모터축을 선택해주세요."), MODE_INFORMATION);

		return;
	}

	CDlgAxisHomeProgress dlg(0);
	dlg.DoModal();
}

void CDlgMotorTeach::CylinderManual_Acitve(UINT nID)
{
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();

	int ID = nID - IDC_BTN_CYLINDER_1;

	int nFront = 0;
	int nBack = 0;
	int nReverse = 0;
	int nOutSignal = 0;
	nFront = ID % 8;
	nBack = ID / 8;

	if (nBack == 1)
		nReverse = 0;
	else
		nReverse = 1;

	if (ID < 8)
		nOutSignal = ID;
	else
		nOutSignal = ID % 8;

	if (nOutSignal < 7)
	{
#if(GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL) || (GET_INSPECTOR == SYS_OQC_SFR_SINGLE_CL) || (GET_INSPECTOR == SYS_DISTORTION_30)
		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_10_DOORLOCK_ONOFF_FRONT + nOutSignal, nReverse);
#elif(GET_INSPECTOR == SYS_FILM_PEEL_OFF)
		AJIN_IO->SendOutputBit(DIO_INPUT_X0_16_REAR_DOORLOCK1_OPENCLOSE + nOutSignal, nReverse);
#endif
		m_btnMotorManual[nBack][nFront].SetStaticStyle(CVGStatic::StaticStyle_Data);
		m_btnMotorManual[nBack][nFront].SetColorStyle(CVGStatic::ColorStyle_Green);
		m_btnMotorManual[nBack][nFront].SetFont_Gdip(L"Arial", 9.0F);
	
		if (stEq.nLanguage == 0)
		{
			if (nBack == 0)
				m_btnMotorManual[nBack][nFront].SetText(_T("잠금"));
			else
				m_btnMotorManual[nBack][nFront].SetText(_T("열림"));
		}
		else
		{
			if (nBack == 0)
				m_btnMotorManual[nBack][nFront].SetText(_T("Lock"));
			else
				m_btnMotorManual[nBack][nFront].SetText(_T("Unlock"));
		}



		m_btnMotorManual[nReverse][nFront].SetStaticStyle(CVGStatic::StaticStyle_Data);
		m_btnMotorManual[nReverse][nFront].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_btnMotorManual[nReverse][nFront].SetFont_Gdip(L"Arial", 9.0F);

		if (stEq.nLanguage == 0)
		{
			if (nReverse == 1)
				m_btnMotorManual[nReverse][nFront].SetText(_T("열림"));
			else
				m_btnMotorManual[nReverse][nFront].SetText(_T("잠금"));
		}
		else
		{
			if (nReverse == 1)
				m_btnMotorManual[nReverse][nFront].SetText(_T("Unlock"));
			else
				m_btnMotorManual[nReverse][nFront].SetText(_T("Lock"));
		}
	}
}

void CDlgMotorTeach::OnBnClickedBtnSpeedAxisSelect1()
{
	m_nSetSpeed_AxisNum = 0;
	CheckSelectMotorSpeed_AxisNum();
}
void CDlgMotorTeach::OnBnClickedBtnSpeedAxisSelect2()
{
	m_nSetSpeed_AxisNum = 1;
	CheckSelectMotorSpeed_AxisNum();
}
void CDlgMotorTeach::OnBnClickedBtnSpeedAxisSelect3()
{
	m_nSetSpeed_AxisNum = 2;
	CheckSelectMotorSpeed_AxisNum();
}
void CDlgMotorTeach::OnBnClickedBtnSpeedAxisSelect4()
{
	m_nSetSpeed_AxisNum = 3;
	CheckSelectMotorSpeed_AxisNum();
}
void CDlgMotorTeach::OnBnClickedBtnSpeedAxisSelect5()
{
	m_nSetSpeed_AxisNum = 4;
	CheckSelectMotorSpeed_AxisNum();
}
void CDlgMotorTeach::OnBnClickedBtnSpeedAxisSelect6()
{
	m_nSetSpeed_AxisNum = 5;
	CheckSelectMotorSpeed_AxisNum();
}
void CDlgMotorTeach::OnBnClickedBtnSpeedAxisSelect7()
{
	m_nSetSpeed_AxisNum = 6;
	CheckSelectMotorSpeed_AxisNum();
}
void CDlgMotorTeach::OnBnClickedBtnSpeedAxisSelect8()
{
	m_nSetSpeed_AxisNum = 7;
	CheckSelectMotorSpeed_AxisNum();
}
void CDlgMotorTeach::OnBnClickedBtnSpeedAxisSelect9()
{
	m_nSetSpeed_AxisNum = 8;
	CheckSelectMotorSpeed_AxisNum();
}
void CDlgMotorTeach::OnBnClickedBtnSpeedAxisSelect10()
{
	m_nSetSpeed_AxisNum = 9;
	CheckSelectMotorSpeed_AxisNum();
}
void CDlgMotorTeach::CheckSelectMotorSpeed_AxisNum()
{
	if (m_nSetSpeed_AxisNum < 0 || m_nSetSpeed_AxisNum > 9)
		return;
	CString strTemp = _T("");

	for (int i = 0; i < 10; i++)
	{
		strTemp.Format(_T("%d"), i);

		if (m_nSetSpeed_AxisNum == i)
		{
			m_btnMotorSpeedSet_AxisNum[i].SetStaticStyle(CVGStatic::StaticStyle_Data);
			m_btnMotorSpeedSet_AxisNum[i].SetColorStyle(CVGStatic::ColorStyle_Green);
			m_btnMotorSpeedSet_AxisNum[i].SetFont_Gdip(L"Arial", 9.0F);
			m_btnMotorSpeedSet_AxisNum[i].SetText(strTemp);
		}
		else
		{
			m_btnMotorSpeedSet_AxisNum[i].SetStaticStyle(CVGStatic::StaticStyle_Data);
			m_btnMotorSpeedSet_AxisNum[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
			m_btnMotorSpeedSet_AxisNum[i].SetFont_Gdip(L"Arial", 9.0F);
			m_btnMotorSpeedSet_AxisNum[i].SetText(strTemp);
		}	
	}

	for (int i = 0; i < 5; i++)
	{
		strTemp.Format(_T("%d"), m_nMotorSpeed[m_nSetSpeed_AxisNum][i]);
		m_stMotorSpeedSet_Thirdsub[i][1].SetText(strTemp);
	}
}

void CDlgMotorTeach::MotorSpeedChange(UINT nID)
{
	int ID = nID - IDC_STC_SPEED_SETTING_1;

	int nNum = 0;
	CString strTemp = _T("");

	CDlgNumPad Dlg;
	Dlg.SetValue(nNum = m_nMotorSpeed[m_nSelectAxisNum][ID]);
	if (Dlg.DoModal() == IDOK)
	{
		nNum = _ttoi(Dlg.GetCurrentNum());
		strTemp.Format(_T("%d"), nNum);

		m_nMotorSpeed[m_nSetSpeed_AxisNum][ID] = nNum;
		m_stMotorSpeedSet_Thirdsub[ID][1].SetText(strTemp);
	}
}

void CDlgMotorTeach::OnStnClickedStcSpeedSetting1()
{
	if (m_nSetSpeed_AxisNum < 0)
	{
		COMMON->ShowMsgDlg_Domodal(_T("모터축이 선택되지 않았습니다. 모터축을 선택해주세요."), MODE_INFORMATION);
		return;
	}
		
	int nNum = 0;
	CString strTemp = _T("");

	CDlgNumPad Dlg;
	Dlg.SetValue(nNum = m_nMotorSpeed[m_nSelectAxisNum][0]);
	if (Dlg.DoModal() == IDOK)
	{
		nNum = _ttoi(Dlg.GetCurrentNum());
		if (FALSE == SetInserValue(0, nNum))
			return;
		strTemp.Format(_T("%d"), nNum);

		m_nMotorSpeed[m_nSetSpeed_AxisNum][0] = nNum;
		m_stMotorSpeedSet_Thirdsub[0][1].SetText(strTemp);

		UpdateData(FALSE);
	}
}
void CDlgMotorTeach::OnStnClickedStcSpeedSetting2()
{
	if (m_nSetSpeed_AxisNum < 0)
	{
		COMMON->ShowMsgDlg_Domodal(_T("모터축이 선택되지 않았습니다. 모터축을 선택해주세요."), MODE_INFORMATION);
		return;
	}

	int nNum = 0;
	CString strTemp = _T("");

	CDlgNumPad Dlg;
	Dlg.SetValue(nNum = m_nMotorSpeed[m_nSelectAxisNum][1]);
	if (Dlg.DoModal() == IDOK)
	{
		nNum = _ttoi(Dlg.GetCurrentNum());
		if (FALSE == SetInserValue(0, nNum))
			return;
		strTemp.Format(_T("%d"), nNum);

		m_nMotorSpeed[m_nSetSpeed_AxisNum][1] = nNum;
		m_stMotorSpeedSet_Thirdsub[1][1].SetText(strTemp);

		UpdateData(FALSE);
	}
}
void CDlgMotorTeach::OnStnClickedStcSpeedSetting3()
{
	if (m_nSetSpeed_AxisNum < 0)
	{
		COMMON->ShowMsgDlg_Domodal(_T("모터축이 선택되지 않았습니다. 모터축을 선택해주세요."), MODE_INFORMATION);
		return;
	}

	int nNum = 0;
	CString strTemp = _T("");

	CDlgNumPad Dlg;
	Dlg.SetValue(nNum = m_nMotorSpeed[m_nSelectAxisNum][2]);
	if (Dlg.DoModal() == IDOK)
	{
		nNum = _ttoi(Dlg.GetCurrentNum());
		if (FALSE == SetInserValue(0, nNum))
			return;
		strTemp.Format(_T("%d"), nNum);

		m_nMotorSpeed[m_nSetSpeed_AxisNum][2] = nNum;
		m_stMotorSpeedSet_Thirdsub[2][1].SetText(strTemp);

		UpdateData(FALSE);
	}
}
void CDlgMotorTeach::OnStnClickedStcSpeedSetting4()
{
	if (m_nSetSpeed_AxisNum < 0)
	{
		COMMON->ShowMsgDlg_Domodal(_T("모터축이 선택되지 않았습니다. 모터축을 선택해주세요."), MODE_INFORMATION);
		return;
	}

	int nNum = 0;
	CString strTemp = _T("");

	CDlgNumPad Dlg;
	Dlg.SetValue(nNum = m_nMotorSpeed[m_nSelectAxisNum][3]);
	if (Dlg.DoModal() == IDOK)
	{
		nNum = _ttoi(Dlg.GetCurrentNum());
		if (FALSE == SetInserValue(1, nNum))
			return;
		strTemp.Format(_T("%d"), nNum);

		m_nMotorSpeed[m_nSetSpeed_AxisNum][3] = nNum;
		m_stMotorSpeedSet_Thirdsub[3][1].SetText(strTemp);

		UpdateData(FALSE);
	}
}
void CDlgMotorTeach::OnStnClickedStcSpeedSetting5()
{
	if (m_nSetSpeed_AxisNum < 0)
	{
		COMMON->ShowMsgDlg_Domodal(_T("모터축이 선택되지 않았습니다. 모터축을 선택해주세요."), MODE_INFORMATION);
		return;
	}

	int nNum = 0;
	CString strTemp = _T("");

	CDlgNumPad Dlg;
	Dlg.SetValue(nNum = m_nMotorSpeed[m_nSelectAxisNum][4]);
	if (Dlg.DoModal() == IDOK)
	{
		nNum = _ttoi(Dlg.GetCurrentNum());
		if (FALSE == SetInserValue(1, nNum))
			return;
		strTemp.Format(_T("%d"), nNum);

		m_nMotorSpeed[m_nSetSpeed_AxisNum][4] = nNum;
		m_stMotorSpeedSet_Thirdsub[4][1].SetText(strTemp);

		UpdateData(FALSE);
	}
}


void CDlgMotorTeach::OnBnClickedBtnTeachSave1()
{
	Save_TeachingData();
}


void CDlgMotorTeach::OnBnClickedBtnTeachSave2()
{
	CDlgSaveAsModel Dlg;
	Dlg.DoModal();

}


void CDlgMotorTeach::OnBnClickedBtnTeachSave3()
{
	// Load
}


void CDlgMotorTeach::OnBnClickedBtnMotorConnect1()
{
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();
	MOTION_PARAM	stSpeed = *DATAMANAGER->GetCurrentMotionParam();
	
	if (FAS_MO->IsConnected() == TRUE)
	{
		WRITE_MAIN_LOG(_T("Connected Already"));
		return;
	}
	
	int nPort = stSpeed.nPort[0];
	DWORD dwBaudRate = 0;

	CString strTemp = _T("");
	TCHAR *BaudList_Motor[] = { _T("9600"),_T("19200"),_T("38400"),_T("57600"),_T("115200") };
	strTemp.Format(_T("%s"), BaudList_Motor[ stSpeed.nBaudRate[0] ]);

	dwBaudRate = _ttoi(strTemp);

	bool bRet = false;
	bRet = FAS_MO->Connect_EziServo(nPort, dwBaudRate);

	if (bRet == true)
	{
		ChangeConnectUI(TRUE);
		if(stEq.nLanguage ==0)
			WRITE_MAIN_LOG(_T("[CONNCECT] Connect에 성공하였습니다 \n"));
		else
			WRITE_MAIN_LOG(_T("[CONNCECT] Connected Success \n"));
		m_dwOldAxisStatus = 0x00;

		OnServoInitMotor();

		FAS_MO->Servo_Alarm_Reset_All();
	}
	else
	{
		if (stEq.nLanguage == 0)
			WRITE_MAIN_LOG(_T("[CONNCECT] Connect에 실패하였습니다 \n"));
		else
			WRITE_MAIN_LOG(_T("[CONNCECT] Connected Falied \n"));
	}


	if (stEq.nLanguage == 0)
	{
		m_stMotorConnect[0].SetText(_T("연결"));
		m_stMotorConnect[1].SetText(_T("해제"));
	}
	else
	{
		m_stMotorConnect[0].SetText(_T("Connect"));
		m_stMotorConnect[1].SetText(_T("Disconnect"));
	}
}

void CDlgMotorTeach::OnBnClickedBtnMotorDisconnect1()
{
	FAS_MO->Disconnect_EziServo();
	ChangeConnectUI(FALSE);
}

void CDlgMotorTeach::SetComboBox_Serial(CComboBox *pBox, int Data)
{
	int count;
	count = pBox->GetCount();
	for (int i = 0; i < count; i++)
	{
		if (Data == pBox->GetItemData(i))
		{
			pBox->SetCurSel(i);
			break;
		}
	}
}

void CDlgMotorTeach::OnBnClickedBtnMotorServoOn()
{
	FAS_MO->m_nSlaveNo = m_nSelectAxisNum;
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();
	//Servo On 실패 시
	if (!FAS_MO->Servo_Enable_Init())
	{
		if(stEq.nLanguage ==0)
			WRITE_MAIN_LOG(_T("[SERVO ON] Servo On에 실패하였습니다 \n"));
		else
			WRITE_MAIN_LOG(_T("[SERVO ON] Failed Servo On \n"));

		return;
	}
	ChangeServoUI(TRUE);
	if (stEq.nLanguage == 0)
		WRITE_MAIN_LOG(_T("[SERVO ON] Servo On에 성공하였습니다 \n"));
	else
		WRITE_MAIN_LOG(_T("[SERVO ON] Success Servo On \n"));
}

void CDlgMotorTeach::OnBnClickedBtnMotorServoOff()
{
	FAS_MO->m_nSlaveNo = m_nSelectAxisNum;
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();
	FAS_MO->m_nSlaveNo = m_nSelectAxisNum;
	//Servo Off 실패 시
	if (!FAS_MO->Servo_Off(m_nSelectAxisNum))
	{
		if (stEq.nLanguage == 0)
			WRITE_MAIN_LOG(_T("[SERVO ON] Servo Off에 실패하였습니다 \n"));
		else
			WRITE_MAIN_LOG(_T("[SERVO ON] Failed Servo Off \n"));
		return;
	}
	ChangeServoUI(FALSE);
	if (stEq.nLanguage == 0)
		WRITE_MAIN_LOG(_T("[SERVO ON] Servo Off에 성공하였습니다 \n"));
	else
		WRITE_MAIN_LOG(_T("[SERVO ON] Success Servo Off \n"));
}

void CDlgMotorTeach::OnBnClickedBtnMotorStop()
{
	//선택한 축 Stop
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();
	if (!FAS_MO->MoveStop(m_nSelectAxisNum))
	{
		if (stEq.nLanguage == 0)
			WRITE_MAIN_LOG(_T("[MOTOR STOP] Servo Stop에 실패하였습니다 \n"));
		else
			WRITE_MAIN_LOG(_T("[MOTOR STOP] Failed motor stop \n"));
		return;
	}
	if (stEq.nLanguage == 0)
		WRITE_MAIN_LOG(_T("[MOTOR STOP] Servo Stop에 성공하였습니다 \n"));
	else
		WRITE_MAIN_LOG(_T("[MOTOR STOP] Success motor stop \n"));
}


void CDlgMotorTeach::OnBnClickedBtnMotorAlarmreset()
{
	//Alarm reset 실패 시
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();
	if (!FAS_MO->Servo_Alarm_Reset_Single(m_nSelectAxisNum))
	{
		if (stEq.nLanguage == 0)
			WRITE_MAIN_LOG(_T("[ALARM] Alarm Reset에 실패하였습니다 \n"));
		else
			WRITE_MAIN_LOG(_T("[ALARM] Failed Alarm reset \n"));
		return;
	}
	if (stEq.nLanguage == 0)
		WRITE_MAIN_LOG(_T("[ALARM] Alarm Reset에 성공하였습니다 \n"));
	else
		WRITE_MAIN_LOG(_T("[ALARM] Success Alarm reset \n"));
}

void CDlgMotorTeach::OnServoInitMotor()
{
	for (int i = 0; i < MOTOR_MAX_COUNT; i++)
		FAS_MO->Servo_Enable(i);
}

void CDlgMotorTeach::ChangeConnectUI(BOOL bState)
{
		MOTION_PARAM	stSpeed = *DATAMANAGER->GetCurrentMotionParam();

		m_cbFASBoard_Port.SetCurSel(stSpeed.nPort[0]-1);
		m_cbFASBoard_BaudRate.SetCurSel(stSpeed.nBaudRate[0]);
		m_stMotorConnect[!bState].SetStaticStyle(CVGStatic::StaticStyle_Data);
		m_stMotorConnect[!bState].SetColorStyle(CVGStatic::ColorStyle_Green);
		m_stMotorConnect[!bState].SetFont_Gdip(L"Arial", 9.0F);
		m_stMotorConnect[bState].SetStaticStyle(CVGStatic::StaticStyle_Data);
		m_stMotorConnect[bState].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_stMotorConnect[bState].SetFont_Gdip(L"Arial", 9.0F);
}

void CDlgMotorTeach::ChangeServoUI(BOOL bState)
{
		m_btnMotorState[!bState].SetStaticStyle(CVGStatic::StaticStyle_Data);
		m_btnMotorState[!bState].SetColorStyle(CVGStatic::ColorStyle_Green);
		m_btnMotorState[!bState].SetFont_Gdip(L"Arial", 9.0F);
		m_btnMotorState[bState].SetStaticStyle(CVGStatic::StaticStyle_Data);
		m_btnMotorState[bState].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_btnMotorState[bState].SetFont_Gdip(L"Arial", 9.0F);
		SetAxisStatus(FAS_MO->GetAxisStatus(m_nSelectAxisNum));
}

void CDlgMotorTeach::SetAxisStatus(DWORD dwValue)
{
	//CStatic* pStatic;
	//char	chValue[12];
	DWORD	dwXOR;
	EZISERVO_AXISSTATUS AxisStatus;
	CString strValue = _T("");

	dwXOR = dwValue ^ m_dwOldAxisStatus;

	if (dwXOR == 0)
	{
		return;
	}

	m_dwOldAxisStatus = dwValue;
	AxisStatus.dwValue = dwValue;
//	sprintf_s(chValue, "0x%08X", dwValue);

//	strValue = (CString)chValue;

//	SetDlgItemText(IDC_STATIC_AXISSTATUS, strValue);

	for (int i = 0; i < AXIS_MOTOR_STATUES; i++)
	{
		if (IS_BIT_SET(dwXOR, i))
		{
			IS_BIT_SET(dwValue, i) ? m_stStatic_Motor_Statuses[i].SetColorStyle(CVGStatic::ColorStyle_Red) : m_stStatic_Motor_Statuses[i].SetColorStyle(CVGStatic::ColorStyle_Black);
		}
	}
}

void CDlgMotorTeach::OnBnClickedBtnTeachingPosY1()
{
	TEACHING_PARAM	stTeach = *DATAMANAGER->GetCurrentTeachingData();

	double dPrevPos, dCurPos;
	CString str;
	str.Format(_T("Are you going to change ? \r\n \r\nPrev Pos :  %.3f \r\n  \r\nCurr Pos :  %.3f"), dPrevPos = stTeach.nSocketY_Wait_Pos / PULSE_PER_UNIT_Y, dCurPos = FAS_MO->GetActualPos(0)/ PULSE_PER_UNIT_Y);
	if (IDCANCEL == AfxMessageBox(str, MB_OKCANCEL))
	{
		return;
	}
	CString strPos;
	strPos.Format(_T("%0.3f"), (double)dCurPos/* / 500*/);
	GetDlgItem(IDC_EDIT_Y_POS_1)->SetWindowTextW(strPos);
}


void CDlgMotorTeach::OnBnClickedBtnTeachingPosY2()
{
	TEACHING_PARAM	stTeach = *DATAMANAGER->GetCurrentTeachingData();

	double dPrevPos, dCurPos;
	CString str;
	str.Format(_T("Are you going to change ? \r\n \r\nPrev Pos :  %.3f \r\n  \r\nCurr Pos :  %.3f"), dPrevPos = stTeach.nSocketY_Blemishi_Pos / PULSE_PER_UNIT_Y, dCurPos = FAS_MO->GetActualPos(0) / PULSE_PER_UNIT_Y);
	if (IDCANCEL == AfxMessageBox(str, MB_OKCANCEL))
	{
		return;
	}
	CString strPos;
	strPos.Format(_T("%0.3f"), (double)dCurPos /*/ 500*/);
	GetDlgItem(IDC_EDIT_Y_POS_2)->SetWindowTextW(strPos);
}


void CDlgMotorTeach::OnBnClickedBtnTeachingPosY3()
{
	TEACHING_PARAM	stTeach = *DATAMANAGER->GetCurrentTeachingData();

	double dPrevPos, dCurPos;
	CString str;
	str.Format(_T("Are you going to change ? \r\n \r\nPrev Pos :  %.3f \r\n  \r\nCurr Pos :  %.3f"), dPrevPos = stTeach.nSocketY_SFR_Pos / PULSE_PER_UNIT_Y, dCurPos = FAS_MO->GetActualPos(0) / PULSE_PER_UNIT_Y);
	if (IDCANCEL == AfxMessageBox(str, MB_OKCANCEL))
	{
		return;
	}
	CString strPos;
	strPos.Format(_T("%0.3f"), (double)dCurPos/* / 500*/);
	GetDlgItem(IDC_EDIT_Y_POS_3)->SetWindowTextW(strPos);
}

// Y LOAD 위치
void CDlgMotorTeach::OnBnClickedBtnTeachingMoveY1()
{
#if(GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL)
	if (CMotion::MO_CheckZ_Zero() == false)
	{
		AfxMessageBox(_T("The Axis Z is not Zero Position"));
		return;
	}
#endif

	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();

	if (COMMON->m_bMotorOrginFlag[SOCKET_Y] == FALSE)
		return;

	if (COMMON->GetInterlock_SocketIn() == FALSE)
	{
		if (stEq.nLanguage == 0)
			COMMON->ShowMsgDlg_Domodal(_T("소켓을 넣어주세요."), MODE_INFORMATION);
		else
			COMMON->ShowMsgDlg_Domodal(_T("Please Move In the socket"), MODE_INFORMATION);
		return;
	}
	CString str;
	str.Format(_T("Are you going to Move ?") );
	if (IDCANCEL == AfxMessageBox(str, MB_OKCANCEL))
	{
		return;
	}

	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();
	CString strPos = 0;
	double dPos;
	GetDlgItemText(IDC_EDIT_Y_POS_1, strPos);
	dPos = _ttof(strPos);

	if (CMotion::MO_ManualMove(SOCKET_Y, dPos) != MCEC_OK)
	{
		if (stEq.nLanguage == 0)
			WRITE_MAIN_LOG(_T("[TEACHING] Y 자재받는위치 이동에 실패하였습니다 \n"));
		else
			WRITE_MAIN_LOG(_T("[Teaching] Failed to move where you receive the materials. \n"));
	}
	else
	{
		if (stEq.nLanguage == 0)
			WRITE_MAIN_LOG(_T("[TEACHING] Y 자재받는위치 이동에 성공하였습니다 \n"));
		else
			WRITE_MAIN_LOG(_T("[Teaching] Success to move where you receive the materials. \n"));
	}
}

void CDlgMotorTeach::OnBnClickedBtnTeachingMoveY2()
{
// 	if (FAS_MO->GetActualPos(SOCKET_Z) > 1)
// 		return;

#if(GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL)
	if (CMotion::MO_CheckZ_Zero() == false)
	{
		AfxMessageBox(_T("The Axis Z is not Zero Position"));
		return;
	}
#endif

	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();
	if (COMMON->m_bMotorOrginFlag[SOCKET_Y] == FALSE)
		return;

	if (COMMON->GetInterlock_SocketIn() == FALSE)
	{
		if (stEq.nLanguage == 0)
			COMMON->ShowMsgDlg_Domodal(_T("소켓을 넣어주세요."), MODE_INFORMATION);
		else
			COMMON->ShowMsgDlg_Domodal(_T("Please Move In the socket"), MODE_INFORMATION);
		return;
	}

	CString str;
	str.Format(_T("Are you going to Move ?"));
	if (IDCANCEL == AfxMessageBox(str, MB_OKCANCEL))
		return;
	
	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();
	CString strPos = 0;
	double dPos;
	GetDlgItemText(IDC_EDIT_Y_POS_2, strPos);
	dPos = _ttof(strPos);

	if (CMotion::MO_ManualMove(SOCKET_Y, dPos) != MCEC_OK)
	{
		if (stEq.nLanguage == 0)
			WRITE_MAIN_LOG(_T("[TEACHING] Y 이물광원위치 이동에 실패하였습니다 \n"));
		else
			WRITE_MAIN_LOG(_T("[TEACHING] You failed to move to the Y Blemishi light source. \n"));
	}
	else
	{
		if (stEq.nLanguage == 0)
			WRITE_MAIN_LOG(_T("[TEACHING] Y 이물광원위치 이동에 성공하였습니다 \n"));
		else
			WRITE_MAIN_LOG(_T("[TEACHING] You success to move to the Y Blemishi light source. \n"));
	}
	
// 	dPos = dPos * PULSE_PER_UNIT_Y;
// 	DWORD dwSpeed = stMotion.nMotorSpeed[0][2];
// 	if (FAS_MO->SingleAxisAbsMove(SOCKET_Y, dPos, dwSpeed) == FALSE)
// 	{
// 		if (stEq.nLanguage == 0)
// 			WRITE_MAIN_LOG(_T("[TEACHING] Y 이물광원위치 이동에 실패하였습니다 \n"));
// 		else
// 			WRITE_MAIN_LOG(_T("[TEACHING] You failed to move to the Y Blemishi light source. \n"));
// 	}
// 	else
// 	{
// 		if (stEq.nLanguage == 0)
// 			WRITE_MAIN_LOG(_T("[TEACHING] Y 이물광원위치 이동에 성공하였습니다 \n"));
// 		else
// 			WRITE_MAIN_LOG(_T("[TEACHING] You success to move to the Y Blemishi light source. \n"));
// 
// 		GetDlgItem(IDC_BTN_TEACHING_MOVE_Y_5)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_BTN_TEACHING_MOVE_Y_6)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_BTN_TEACHING_MOVE_Y_7)->EnableWindow(FALSE);
// 	}
}


void CDlgMotorTeach::OnBnClickedBtnTeachingMoveY3()
{
#if(GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL)
	if (CMotion::MO_CheckZ_Zero() == false)
	{
		AfxMessageBox(_T("The Axis Z is not Zero Position"));
		return;
	}
#endif

// 	if (FAS_MO->GetActualPos(SOCKET_Z) > 1)
// 		return;

	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();
	if (COMMON->m_bMotorOrginFlag[SOCKET_Y] == FALSE)
		return;

	if (COMMON->GetInterlock_SocketIn() == FALSE)
	{
		if (stEq.nLanguage == 0)
			COMMON->ShowMsgDlg_Domodal(_T("소켓을 넣어주세요."), MODE_INFORMATION);
		else
			COMMON->ShowMsgDlg_Domodal(_T("Please Move In the socket"), MODE_INFORMATION);
		return;
	}
	CString str;
	str.Format(_T("Are you going to Move ?"));
	if (IDCANCEL == AfxMessageBox(str, MB_OKCANCEL))
	{
		return;
	}

	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();
	CString strPos = 0;
	double dPos;
	GetDlgItemText(IDC_EDIT_Y_POS_3, strPos);
	dPos = _ttof(strPos);

	if (CMotion::MO_ManualMove(SOCKET_Y, dPos) != MCEC_OK)
	{
		if (stEq.nLanguage == 0)
			WRITE_MAIN_LOG(_T("[TEACHING] Y SFR 측정위치 이동에 실패하였습니다 \n"));
		else
			WRITE_MAIN_LOG(_T("[TEACHING] You failed to move the SFR measurement position. \n"));
	}
	else
	{
		if (stEq.nLanguage == 0)
			WRITE_MAIN_LOG(_T("[TEACHING] Y SFR 측정위치 이동에 성공하였습니다 \n"));
		else
			WRITE_MAIN_LOG(_T("[TEACHING] You success to move the SFR measurement position. \n"));
	}

// 	dPos = dPos * PULSE_PER_UNIT_Y;
// 	DWORD dwSpeed = stMotion.nMotorSpeed[0][2];
// 	if (FAS_MO->SingleAxisAbsMove(SOCKET_Y, dPos, dwSpeed) == FALSE)
// 	{
// 		if (stEq.nLanguage == 0)
// 			WRITE_MAIN_LOG(_T("[TEACHING] Y SFR 측정위치 이동에 실패하였습니다 \n"));
// 		else
// 			WRITE_MAIN_LOG(_T("[TEACHING] You failed to move the SFR measurement position. \n"));
// 	}
// 	else
// 	{
// 		if (stEq.nLanguage == 0)
// 			WRITE_MAIN_LOG(_T("[TEACHING] Y SFR 측정위치 이동에 성공하였습니다 \n"));
// 		else
// 			WRITE_MAIN_LOG(_T("[TEACHING] You success to move the SFR measurement position. \n"));
// 		GetDlgItem(IDC_BTN_TEACHING_MOVE_Y_5)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_BTN_TEACHING_MOVE_Y_6)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_BTN_TEACHING_MOVE_Y_7)->EnableWindow(TRUE);		
// 	}
}

void CDlgMotorTeach::OnBnClickedBtnTeachingPosY4()
{
	TEACHING_PARAM	stTeach = *DATAMANAGER->GetCurrentTeachingData();

	double dPrevPos, dCurPos;
	CString str;
	str.Format(_T("Are you going to change ? \r\n \r\nPrev Pos :  %.3f \r\n  \r\nCurr Pos :  %.3f"), dPrevPos = stTeach.nSocketZ_Wait_Pos / 1000.0, dCurPos = FAS_MO->GetActualPos(1) / PULSE_PER_UNIT_Z);
	if (IDCANCEL == AfxMessageBox(str, MB_OKCANCEL))
		return;
	
	CString strPos;
	strPos.Format(_T("%0.3f"), (double)dCurPos /*/ 10000*/);
	GetDlgItem(IDC_EDIT_Z_POS_1)->SetWindowTextW(strPos);
}


void CDlgMotorTeach::OnBnClickedBtnTeachingPosY5()
{
	TEACHING_PARAM	stTeach = *DATAMANAGER->GetCurrentTeachingData();

	double dPrevPos, dCurPos;
	CString str;
	str.Format(_T("Are you going to change ? \r\n \r\nPrev Pos :  %.3f \r\n  \r\nCurr Pos :  %.3f"), dPrevPos = stTeach.nSocketZ_Blemishi_Pos / 1000.0, dCurPos = FAS_MO->GetActualPos(1) / PULSE_PER_UNIT_Z);
	if (IDCANCEL == AfxMessageBox(str, MB_OKCANCEL))
	{
		return;
	}
	CString strPos;
	strPos.Format(_T("%0.3f"), (double)dCurPos/*/ 10000*/);
	GetDlgItem(IDC_EDIT_Z_POS_2)->SetWindowTextW(strPos);
}


void CDlgMotorTeach::OnBnClickedBtnTeachingPosY6()
{
	TEACHING_PARAM	stTeach = *DATAMANAGER->GetCurrentTeachingData();

	double dPrevPos, dCurPos;
	CString str;
	str.Format(_T("Are you going to change ? \r\n \r\nPrev Pos :  %.3f \r\n  \r\nCurr Pos :  %.3f"), dPrevPos = stTeach.nSocketZ_SFR_180_Pos / 1000.0, dCurPos = FAS_MO->GetActualPos(1) / PULSE_PER_UNIT_Z);
	if (IDCANCEL == AfxMessageBox(str, MB_OKCANCEL))
	{
		return;
	}
	CString strPos;
	strPos.Format(_T("%0.3f"), (double)dCurPos/*/ 10000*/);
	GetDlgItem(IDC_EDIT_Z_POS_3)->SetWindowTextW(strPos);
}

void CDlgMotorTeach::OnBnClickedBtnTeachingPosY7()
{
	TEACHING_PARAM	stTeach = *DATAMANAGER->GetCurrentTeachingData();

	double dPrevPos, dCurPos;
	CString str;
	str.Format(_T("Are you going to change ? \r\n \r\nPrev Pos :  %.3f \r\n  \r\nCurr Pos :  %.3f"), dPrevPos = stTeach.nSocketZ_SFR_825_Pos / 1000.0, dCurPos = FAS_MO->GetActualPos(1) / PULSE_PER_UNIT_Z);
	if (IDCANCEL == AfxMessageBox(str, MB_OKCANCEL))
	{
		return;
	}
	CString strPos;
	strPos.Format(_T("%0.3f"), (double)dCurPos /*/ 10000*/);
	GetDlgItem(IDC_EDIT_Z_POS_4)->SetWindowTextW(strPos);
}

void CDlgMotorTeach::OnBnClickedBtnTeachingMoveY4()
{
	// 자재 받는 위치에서만 동작
	/*if (CMotion::MO_CheckY_Load() == false)
	{
		AfxMessageBox(_T("The Axis Y is not Wait Position!"));
		return;
	}*/

	if (COMMON->m_bMotorOrginFlag[SOCKET_Z] == FALSE)
		return;

	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();

	if (COMMON->GetInterlock_SocketIn() == FALSE)
	{
		if (stEq.nLanguage == 0)
			COMMON->ShowMsgDlg_Domodal(_T("소켓을 넣어주세요."), MODE_INFORMATION);
		else
			COMMON->ShowMsgDlg_Domodal(_T("Please Move In the socket"), MODE_INFORMATION);
		return;
	}
	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();
	CString strPos = 0;
	double dPos;
	GetDlgItemText(IDC_EDIT_Z_POS_1, strPos);
	dPos = _ttof(strPos);

	if (CMotion::MO_ManualMove(SOCKET_Z, dPos) != MCEC_OK)
	{
 		if (stEq.nLanguage == 0)
 			WRITE_MAIN_LOG(_T("[TEACHING] Z 자재받는위치 이동에 실패하였습니다 \n"));
 		else
 			WRITE_MAIN_LOG(_T("[TEACHING] You failed to move the Z material. \n"));
	}
	else
	{
 		if (stEq.nLanguage == 0)
 			WRITE_MAIN_LOG(_T("[TEACHING] Z 자재받는위치 이동에 성공하였습니다 \n"));
 		else
 			WRITE_MAIN_LOG(_T("[TEACHING] You success to move the Z material. \n"));
	}

// 	dPos = dPos * PULSE_PER_UNIT_Z;
// 	DWORD dwSpeed = stMotion.nMotorSpeed[0][2];
// 	if (FAS_MO->SingleAxisAbsMove(SOCKET_Z, dPos, dwSpeed) == FALSE)
// 	{
// 		if (stEq.nLanguage == 0)
// 			WRITE_MAIN_LOG(_T("[TEACHING] Z 자재받는위치 이동에 실패하였습니다 \n"));
// 		else
// 			WRITE_MAIN_LOG(_T("[TEACHING] You failed to move the Z material. \n"));
// 	}
// 	else
// 	{
// 		if (stEq.nLanguage == 0)
// 			WRITE_MAIN_LOG(_T("[TEACHING] Z 자재받는위치 이동에 성공하였습니다 \n"));
// 		else
// 			WRITE_MAIN_LOG(_T("[TEACHING] You success to move the Z material. \n"));
// 		GetDlgItem(IDC_BTN_TEACHING_MOVE_Y_1)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_BTN_TEACHING_MOVE_Y_2)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_BTN_TEACHING_MOVE_Y_3)->EnableWindow(TRUE);
// 	}
}

void CDlgMotorTeach::OnBnClickedBtnTeachingMoveY5()
{
	// 이물광원 위치에서만 동작 
	if (CMotion::MO_CheckY_Blemish() == false)
	{
		AfxMessageBox(_T("The Axis Y is not Blemish Position!"));
		return;
	}

	if (COMMON->m_bMotorOrginFlag[SOCKET_Z] == FALSE)
		return;

	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();

	if (COMMON->GetInterlock_SocketIn() == FALSE)
	{
		if (stEq.nLanguage == 0)
			COMMON->ShowMsgDlg_Domodal(_T("소켓을 넣어주세요."), MODE_INFORMATION);
		else
			COMMON->ShowMsgDlg_Domodal(_T("Please Move In the socket"), MODE_INFORMATION);
		return;
	}
	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();
	CString strPos = 0;
	double dPos;
	GetDlgItemText(IDC_EDIT_Z_POS_2, strPos);
	dPos = _ttof(strPos);

	if (CMotion::MO_ManualMove(SOCKET_Z, dPos) != MCEC_OK)
	{
		if (stEq.nLanguage == 0)
			WRITE_MAIN_LOG(_T("[TEACHING] Z 이물광원위치 이동에 실패하였습니다 \n"));
		else
			WRITE_MAIN_LOG(_T("[TEACHING] You falied moved to the location of the Z Blemishi light source. \n"));
	}
	else
	{
		if (stEq.nLanguage == 0)
			WRITE_MAIN_LOG(_T("[TEACHING] Z 이물광원위치 이동에 성공하였습니다 \n"));
		else
			WRITE_MAIN_LOG(_T("[TEACHING] You successfully moved to the location of the Z Blemishi light source. \n"));
	}

// 	dPos = dPos * PULSE_PER_UNIT_Z;
// 	DWORD dwSpeed = stMotion.nMotorSpeed[0][2];
// 
// 	CString strPosChk = 0;
// 	double dPosChk;
// 	GetDlgItemText(IDC_EDIT_Y_POS_2, strPosChk);
// 	dPosChk = _ttof(strPosChk);
// 	dPosChk = dPosChk * PULSE_PER_UNIT_Z;
// 
// 	if (fabs(dPosChk) > fabs(dPosChk + 0.1))
// 	{
// 		if (stEq.nLanguage == 0)
// 			COMMON->ShowMsgDlg_Domodal(_T("위치를 체크 해주세요."), MODE_INFORMATION);
// 		else
// 			COMMON->ShowMsgDlg_Domodal(_T("Please Check Move Pos"), MODE_INFORMATION);
// 		return;
// 	}
// 
// 	if (FAS_MO->SingleAxisAbsMove(SOCKET_Z, dPos, dwSpeed) == FALSE)
// 	{
// 		if (stEq.nLanguage == 0)
// 			WRITE_MAIN_LOG(_T("[TEACHING] Z 이물광원위치 이동에 실패하였습니다 \n"));
// 		else
// 			WRITE_MAIN_LOG(_T("[TEACHING] You falied moved to the location of the Z Blemishi light source. \n"));
// 	}
// 	else
// 	{
// 		if (stEq.nLanguage == 0)
// 			WRITE_MAIN_LOG(_T("[TEACHING] Z 이물광원위치 이동에 성공하였습니다 \n"));
// 		else
// 			WRITE_MAIN_LOG(_T("[TEACHING] You successfully moved to the location of the Z Blemishi light source. \n"));
// 		GetDlgItem(IDC_BTN_TEACHING_MOVE_Y_1)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_BTN_TEACHING_MOVE_Y_2)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_BTN_TEACHING_MOVE_Y_3)->EnableWindow(FALSE);
// 	}
}

void CDlgMotorTeach::OnBnClickedBtnTeachingMoveY6()
{
	// sfr 검사 위치에서만 동작 
	if (CMotion::MO_CheckY_SFR() == false)
	{
		AfxMessageBox(_T("The Axis Y is not Sfr Position!"));
		return;
	}

	if (COMMON->m_bMotorOrginFlag[SOCKET_Z] == FALSE)
		return;

	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();

	if (COMMON->GetInterlock_SocketIn() == FALSE)
	{
		if (stEq.nLanguage == 0)
			COMMON->ShowMsgDlg_Domodal(_T("소켓을 넣어주세요."), MODE_INFORMATION);
		else
			COMMON->ShowMsgDlg_Domodal(_T("Please Move In the socket"), MODE_INFORMATION);
		return;
	}

	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();
	CString strPos = 0;
	double dPos;
	GetDlgItemText(IDC_EDIT_Z_POS_3, strPos);
	dPos = _ttof(strPos);

	if (CMotion::MO_ManualMove(SOCKET_Z, dPos) != MCEC_OK)
	{
		if (stEq.nLanguage == 0)
			WRITE_MAIN_LOG(_T("[TEACHING] Z SFR 측정위치 이동에 실패하였습니다 \n"));
		else
			WRITE_MAIN_LOG(_T("[TEACHING] You failed to move the ZSFR measurement position. \n"));
	}
	else
	{
		if (stEq.nLanguage == 0)
			WRITE_MAIN_LOG(_T("[TEACHING] Z SFR 측정위치 이동에 성공하였습니다 \n"));
		else
			WRITE_MAIN_LOG(_T("[TEACHING] You success to move the ZSFR measurement position. \n"));
	}

// 	DWORD dwSpeed = stMotion.nMotorSpeed[0][2];
// 	CString strPosChk = 0;
// 	double dPosChk;
// 	GetDlgItemText(IDC_EDIT_Y_POS_3, strPosChk);
// 	dPosChk = _ttof(strPosChk);
// 	dPosChk = dPosChk * PULSE_PER_UNIT_Z;
// 	if (fabs(dPosChk) > fabs(dPosChk + 0.1))
// 	{
// 		if (stEq.nLanguage == 0)
// 			COMMON->ShowMsgDlg_Domodal(_T("위치를 체크 해주세요."), MODE_INFORMATION);
// 		else
// 			COMMON->ShowMsgDlg_Domodal(_T("Please Check Move Pos"), MODE_INFORMATION);
// 		return;
// 	}
// 
// 	if (FAS_MO->SingleAxisAbsMove(SOCKET_Z, dPos, dwSpeed) == FALSE)
// 	{
// 		if (stEq.nLanguage == 0)
// 			WRITE_MAIN_LOG(_T("[TEACHING] Z SFR 측정위치 이동에 실패하였습니다 \n"));
// 		else
// 			WRITE_MAIN_LOG(_T("[TEACHING] You failed to move the ZSFR measurement position. \n"));
// 	}
// 	else
// 	{
// 		if (stEq.nLanguage == 0)
// 			WRITE_MAIN_LOG(_T("[TEACHING] Z SFR 측정위치 이동에 성공하였습니다 \n"));
// 		else
// 			WRITE_MAIN_LOG(_T("[TEACHING] You success to move the ZSFR measurement position. \n"));
// 		GetDlgItem(IDC_BTN_TEACHING_MOVE_Y_1)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_BTN_TEACHING_MOVE_Y_2)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_BTN_TEACHING_MOVE_Y_3)->EnableWindow(FALSE);
// 	}
}

void CDlgMotorTeach::OnBnClickedBtnTeachingMoveY7()
{
	if (COMMON->m_bMotorOrginFlag[SOCKET_Z] == FALSE)
		return;
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();

	if (COMMON->GetInterlock_SocketIn() == FALSE)
	{
		if (stEq.nLanguage == 0)
			COMMON->ShowMsgDlg_Domodal(_T("소켓을 넣어주세요."), MODE_INFORMATION);
		else
			COMMON->ShowMsgDlg_Domodal(_T("Please Move In the socket"), MODE_INFORMATION);
		return;
	}
	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();
	CString strPos = 0;
	double dPos;
	GetDlgItemText(IDC_EDIT_Z_POS_4, strPos);
	dPos = _ttof(strPos);
	dPos = dPos * PULSE_PER_UNIT_Z;
	DWORD dwSpeed = stMotion.nMotorSpeed[0][2];

	CString strPosChk = 0;
	double dPosChk;
	GetDlgItemText(IDC_EDIT_Y_POS_3, strPosChk);
	dPosChk = _ttof(strPosChk);
	dPosChk = dPosChk * PULSE_PER_UNIT_Z;
	if (fabs(dPosChk) > fabs(dPosChk + 0.1))
	{
		if (stEq.nLanguage == 0)
			COMMON->ShowMsgDlg_Domodal(_T("위치를 체크 해주세요."), MODE_INFORMATION);
		else
			COMMON->ShowMsgDlg_Domodal(_T("Please Check Move Pos"), MODE_INFORMATION);
		return;
	}

	if (FAS_MO->SingleAxisAbsMove(SOCKET_Z, dPos, dwSpeed) == FALSE)
	{
		if (stEq.nLanguage == 0)
			WRITE_MAIN_LOG(_T("[TEACHING] Z SFR 측정위치 이동에 실패하였습니다 \n"));
		else
			WRITE_MAIN_LOG(_T("[TEACHING] You failed to move the ZSFR measurement position. \n"));
	}
	else
	{
		if (stEq.nLanguage == 0)
			WRITE_MAIN_LOG(_T("[TEACHING] Z SFR 측정위치 이동에 성공하였습니다 \n"));
		else
			WRITE_MAIN_LOG(_T("[TEACHING] You success to move the ZSFR measurement position. \n"));
		GetDlgItem(IDC_BTN_TEACHING_MOVE_Y_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_Y_2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_Y_3)->EnableWindow(FALSE);
	}
}


BOOL CDlgMotorTeach::SetInserValue(int nMode, int nAxisSpeed)
{
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();
	if (nMode == 0)
	{
		if (nAxisSpeed > 400 || nAxisSpeed < 1)
		{
			if(stEq.nLanguage ==0)
				COMMON->ShowMsgDlg_Domodal(_T("저속 중속 고속 속도 입력범위는 1~400까지입니다. 다시입력해주세요"), MODE_INFORMATION);
			else
				COMMON->ShowMsgDlg_Domodal(_T("The input range of low-speed medium-speed and high-speed ranges from 1 to 400. Please input it again."), MODE_INFORMATION);
			return FALSE;
		}
	}
	else if (nMode == 1)
	{
		if (nAxisSpeed > 1000 || nAxisSpeed < 1)
		{
			if (stEq.nLanguage == 0)
				COMMON->ShowMsgDlg_Domodal(_T("가속 감속 속도 입력범위는 1~1000까지입니다. 다시입력해주세요"), MODE_INFORMATION);
			else
				COMMON->ShowMsgDlg_Domodal(_T("The acceleration deceleration speed input range is from 1 to 1000. Please input it again."), MODE_INFORMATION);
			return FALSE;
		}
	}
	return TRUE;
}


void CDlgMotorTeach::OnBnClickedBtnJogMove2()
{
	// TODO: Add your control notification handler code here
}

void CDlgMotorTeach::OnBnClickedBtnTeachingPosY8()
{
	TEACHING_PARAM	stTeach = *DATAMANAGER->GetCurrentTeachingData();

	double dPrevPos, dCurPos;
	CString str;
	str.Format(_T("Are you going to change ? \r\n \r\nPrev Pos :  %.3f \r\n  \r\nCurr Pos :  %.3f"), dPrevPos = stTeach.nSocketY_Intrinsic_Pos[0] / PULSE_PER_UNIT_Y, dCurPos = FAS_MO->GetActualPos(0) / PULSE_PER_UNIT_Y);
	if (IDCANCEL == AfxMessageBox(str, MB_OKCANCEL))
	{
		return;
	}
	CString strPos;
	strPos.Format(_T("%0.3f"), (double)dCurPos/* / 500*/);
	GetDlgItem(IDC_EDIT_Y_POS_4)->SetWindowTextW(strPos);
}


void CDlgMotorTeach::OnBnClickedBtnTeachingPosY9()
{
	TEACHING_PARAM	stTeach = *DATAMANAGER->GetCurrentTeachingData();

	double dPrevPos, dCurPos;
	CString str;
	str.Format(_T("Are you going to change ? \r\n \r\nPrev Pos :  %.3f \r\n  \r\nCurr Pos :  %.3f"), dPrevPos = stTeach.nSocketY_Intrinsic_Pos[1] / PULSE_PER_UNIT_Y, dCurPos = FAS_MO->GetActualPos(0) / PULSE_PER_UNIT_Y);
	if (IDCANCEL == AfxMessageBox(str, MB_OKCANCEL))
	{
		return;
	}
	CString strPos;
	strPos.Format(_T("%0.3f"), (double)dCurPos/* / 500*/);
	GetDlgItem(IDC_EDIT_Y_POS_5)->SetWindowTextW(strPos);
}


void CDlgMotorTeach::OnBnClickedBtnTeachingPosY10()
{
	TEACHING_PARAM	stTeach = *DATAMANAGER->GetCurrentTeachingData();

	double dPrevPos, dCurPos;
	CString str;
	str.Format(_T("Are you going to change ? \r\n \r\nPrev Pos :  %.3f \r\n  \r\nCurr Pos :  %.3f"), dPrevPos = stTeach.nSocketY_Intrinsic_Pos[2] / PULSE_PER_UNIT_Y, dCurPos = FAS_MO->GetActualPos(0) / PULSE_PER_UNIT_Y);
	if (IDCANCEL == AfxMessageBox(str, MB_OKCANCEL))
	{
		return;
	}
	CString strPos;
	strPos.Format(_T("%0.3f"), (double)dCurPos/* / 500*/);
	GetDlgItem(IDC_EDIT_Y_POS_6)->SetWindowTextW(strPos);
}


void CDlgMotorTeach::OnBnClickedBtnTeachingPosY11()
{
	TEACHING_PARAM	stTeach = *DATAMANAGER->GetCurrentTeachingData();

	double dPrevPos, dCurPos;
	CString str;
	str.Format(_T("Are you going to change ? \r\n \r\nPrev Pos :  %.3f \r\n  \r\nCurr Pos :  %.3f"), dPrevPos = stTeach.nSocketY_Intrinsic_Pos[3] / PULSE_PER_UNIT_Y, dCurPos = FAS_MO->GetActualPos(0) / PULSE_PER_UNIT_Y);
	if (IDCANCEL == AfxMessageBox(str, MB_OKCANCEL))
	{
		return;
	}
	CString strPos;
	strPos.Format(_T("%0.3f"), (double)dCurPos/* / 500*/);
	GetDlgItem(IDC_EDIT_Y_POS_7)->SetWindowTextW(strPos);
}


void CDlgMotorTeach::OnBnClickedBtnTeachingPosY12()
{
	TEACHING_PARAM	stTeach = *DATAMANAGER->GetCurrentTeachingData();

	double dPrevPos, dCurPos;
	CString str;
	str.Format(_T("Are you going to change ? \r\n \r\nPrev Pos :  %.3f \r\n  \r\nCurr Pos :  %.3f"), dPrevPos = stTeach.nSocketY_Intrinsic_Pos[4] / PULSE_PER_UNIT_Y, dCurPos = FAS_MO->GetActualPos(0) / PULSE_PER_UNIT_Y);
	if (IDCANCEL == AfxMessageBox(str, MB_OKCANCEL))
	{
		return;
	}
	CString strPos;
	strPos.Format(_T("%0.3f"), (double)dCurPos/* / 500*/);
	GetDlgItem(IDC_EDIT_Y_POS_8)->SetWindowTextW(strPos);
}

void CDlgMotorTeach::OnBnClickedBtnTeachingMoveY8()
{
#if(GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL)
	if (CMotion::MO_CheckZ_Zero() == false)
	{
		AfxMessageBox(_T("The Axis Z is not Zero Position"));
		return;
	}
#endif

	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();

	if (COMMON->m_bMotorOrginFlag[SOCKET_Y] == FALSE)
		return;

	if (COMMON->GetInterlock_SocketIn() == FALSE)
	{
		if (stEq.nLanguage == 0)
			COMMON->ShowMsgDlg_Domodal(_T("소켓을 넣어주세요."), MODE_INFORMATION);
		else
			COMMON->ShowMsgDlg_Domodal(_T("Please Move In the socket"), MODE_INFORMATION);
		return;
	}
	CString str;
	str.Format(_T("Are you going to Move ?"));
	if (IDCANCEL == AfxMessageBox(str, MB_OKCANCEL))
	{
		return;
	}

	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();
	CString strPos = 0;
	double dPos;
	GetDlgItemText(IDC_EDIT_Y_POS_4, strPos);
	dPos = _ttof(strPos);

	if (CMotion::MO_ManualMove(SOCKET_Y, dPos) != MCEC_OK)
	{
		if (stEq.nLanguage == 0)
			WRITE_MAIN_LOG(_T("[TEACHING] Y 자재받는위치 이동에 실패하였습니다 \n"));
		else
			WRITE_MAIN_LOG(_T("[Teaching] Failed to move where you receive the materials. \n"));
	}
	else
	{
		if (stEq.nLanguage == 0)
			WRITE_MAIN_LOG(_T("[TEACHING] Y 자재받는위치 이동에 성공하였습니다 \n"));
		else
			WRITE_MAIN_LOG(_T("[Teaching] Success to move where you receive the materials. \n"));
	}
}

void CDlgMotorTeach::OnBnClickedBtnTeachingMoveY9()
{
#if(GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL)
	if (CMotion::MO_CheckZ_Zero() == false)
	{
		AfxMessageBox(_T("The Axis Z is not Zero Position"));
		return;
	}
#endif

	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();

	if (COMMON->m_bMotorOrginFlag[SOCKET_Y] == FALSE)
		return;

	if (COMMON->GetInterlock_SocketIn() == FALSE)
	{
		if (stEq.nLanguage == 0)
			COMMON->ShowMsgDlg_Domodal(_T("소켓을 넣어주세요."), MODE_INFORMATION);
		else
			COMMON->ShowMsgDlg_Domodal(_T("Please Move In the socket"), MODE_INFORMATION);
		return;
	}
	CString str;
	str.Format(_T("Are you going to Move ?"));
	if (IDCANCEL == AfxMessageBox(str, MB_OKCANCEL))
	{
		return;
	}

	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();
	CString strPos = 0;
	double dPos;
	GetDlgItemText(IDC_EDIT_Y_POS_5, strPos);
	dPos = _ttof(strPos);

	if (CMotion::MO_ManualMove(SOCKET_Y, dPos) != MCEC_OK)
	{
		if (stEq.nLanguage == 0)
			WRITE_MAIN_LOG(_T("[TEACHING] Y 자재받는위치 이동에 실패하였습니다 \n"));
		else
			WRITE_MAIN_LOG(_T("[Teaching] Failed to move where you receive the materials. \n"));
	}
	else
	{
		if (stEq.nLanguage == 0)
			WRITE_MAIN_LOG(_T("[TEACHING] Y 자재받는위치 이동에 성공하였습니다 \n"));
		else
			WRITE_MAIN_LOG(_T("[Teaching] Success to move where you receive the materials. \n"));
	}
}

void CDlgMotorTeach::OnBnClickedBtnTeachingMoveY10()
{
#if(GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL)
	if (CMotion::MO_CheckZ_Zero() == false)
	{
		AfxMessageBox(_T("The Axis Z is not Zero Position"));
		return;
	}
#endif

	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();

	if (COMMON->m_bMotorOrginFlag[SOCKET_Y] == FALSE)
		return;

	if (COMMON->GetInterlock_SocketIn() == FALSE)
	{
		if (stEq.nLanguage == 0)
			COMMON->ShowMsgDlg_Domodal(_T("소켓을 넣어주세요."), MODE_INFORMATION);
		else
			COMMON->ShowMsgDlg_Domodal(_T("Please Move In the socket"), MODE_INFORMATION);
		return;
	}
	CString str;
	str.Format(_T("Are you going to Move ?"));
	if (IDCANCEL == AfxMessageBox(str, MB_OKCANCEL))
	{
		return;
	}

	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();
	CString strPos = 0;
	double dPos;
	GetDlgItemText(IDC_EDIT_Y_POS_6, strPos);
	dPos = _ttof(strPos);

	if (CMotion::MO_ManualMove(SOCKET_Y, dPos) != MCEC_OK)
	{
		if (stEq.nLanguage == 0)
			WRITE_MAIN_LOG(_T("[TEACHING] Y 자재받는위치 이동에 실패하였습니다 \n"));
		else
			WRITE_MAIN_LOG(_T("[Teaching] Failed to move where you receive the materials. \n"));
	}
	else
	{
		if (stEq.nLanguage == 0)
			WRITE_MAIN_LOG(_T("[TEACHING] Y 자재받는위치 이동에 성공하였습니다 \n"));
		else
			WRITE_MAIN_LOG(_T("[Teaching] Success to move where you receive the materials. \n"));
	}
}

void CDlgMotorTeach::OnBnClickedBtnTeachingMoveY11()
{
#if(GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL)
	if (CMotion::MO_CheckZ_Zero() == false)
	{
		AfxMessageBox(_T("The Axis Z is not Zero Position"));
		return;
	}
#endif

	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();

	if (COMMON->m_bMotorOrginFlag[SOCKET_Y] == FALSE)
		return;

	if (COMMON->GetInterlock_SocketIn() == FALSE)
	{
		if (stEq.nLanguage == 0)
			COMMON->ShowMsgDlg_Domodal(_T("소켓을 넣어주세요."), MODE_INFORMATION);
		else
			COMMON->ShowMsgDlg_Domodal(_T("Please Move In the socket"), MODE_INFORMATION);
		return;
	}
	CString str;
	str.Format(_T("Are you going to Move ?"));
	if (IDCANCEL == AfxMessageBox(str, MB_OKCANCEL))
	{
		return;
	}

	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();
	CString strPos = 0;
	double dPos;
	GetDlgItemText(IDC_EDIT_Y_POS_7, strPos);
	dPos = _ttof(strPos);

	if (CMotion::MO_ManualMove(SOCKET_Y, dPos) != MCEC_OK)
	{
		if (stEq.nLanguage == 0)
			WRITE_MAIN_LOG(_T("[TEACHING] Y 자재받는위치 이동에 실패하였습니다 \n"));
		else
			WRITE_MAIN_LOG(_T("[Teaching] Failed to move where you receive the materials. \n"));
	}
	else
	{
		if (stEq.nLanguage == 0)
			WRITE_MAIN_LOG(_T("[TEACHING] Y 자재받는위치 이동에 성공하였습니다 \n"));
		else
			WRITE_MAIN_LOG(_T("[Teaching] Success to move where you receive the materials. \n"));
	}
}

void CDlgMotorTeach::OnBnClickedBtnTeachingMoveY12()
{
#if(GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL)
	if (CMotion::MO_CheckZ_Zero() == false)
	{
		AfxMessageBox(_T("The Axis Z is not Zero Position"));
		return;
	}
#endif

	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();

	if (COMMON->m_bMotorOrginFlag[SOCKET_Y] == FALSE)
		return;

	if (COMMON->GetInterlock_SocketIn() == FALSE)
	{
		if (stEq.nLanguage == 0)
			COMMON->ShowMsgDlg_Domodal(_T("소켓을 넣어주세요."), MODE_INFORMATION);
		else
			COMMON->ShowMsgDlg_Domodal(_T("Please Move In the socket"), MODE_INFORMATION);
		return;
	}
	CString str;
	str.Format(_T("Are you going to Move ?"));
	if (IDCANCEL == AfxMessageBox(str, MB_OKCANCEL))
	{
		return;
	}

	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();
	CString strPos = 0;
	double dPos;
	GetDlgItemText(IDC_EDIT_Y_POS_8, strPos);
	dPos = _ttof(strPos);

	if (CMotion::MO_ManualMove(SOCKET_Y, dPos) != MCEC_OK)
	{
		if (stEq.nLanguage == 0)
			WRITE_MAIN_LOG(_T("[TEACHING] Y 자재받는위치 이동에 실패하였습니다 \n"));
		else
			WRITE_MAIN_LOG(_T("[Teaching] Failed to move where you receive the materials. \n"));
	}
	else
	{
		if (stEq.nLanguage == 0)
			WRITE_MAIN_LOG(_T("[TEACHING] Y 자재받는위치 이동에 성공하였습니다 \n"));
		else
			WRITE_MAIN_LOG(_T("[Teaching] Success to move where you receive the materials. \n"));
	}
}
