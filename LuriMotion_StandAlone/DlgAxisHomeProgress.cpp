// DlgAxisHomeProgress.cpp: 구현 파일
//

#include "pch.h"
#include "LuriMotion_OQA_SFR_Wide.h"
#include "LuriMotion_OQA_SFR_Widedlg.h"
#include "DlgAxisHomeProgress.h"
#include "afxdialogex.h"
#include "IO.h"

#include "Device.h"

#define TIMER_INIT_EQUIP		1000
#define TIMER_INIT_INTLK		1001
#define TIMER_INIT_AXES			1002

// CDlgAxisHomeProgress 대화 상자

IMPLEMENT_DYNAMIC(CDlgAxisHomeProgress, CDialogEx)

CDlgAxisHomeProgress::CDlgAxisHomeProgress( CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_MOTOR_HOME_RATE, pParent)
{
	m_nInitRate = 0;
	m_bMoveHome = FALSE;

	m_nAxisNum = 0;
	nManualInitial_Axis = 0;
	m_nEachStep = 0;
	m_bFlag_InitStop = FALSE;

	for (int i = 0; i < (int)MOTOR_MAX_COUNT; i++)
	{
		m_bHomeFlag[i] = FALSE;
	}
	m_nTemp = 0;

	m_bAllAxis_CheckFlag = FALSE;
	m_nAllAxis_InitStep = 0;
}

CDlgAxisHomeProgress::~CDlgAxisHomeProgress()
{
}

void CDlgAxisHomeProgress::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STC_TITLE, m_stTitle);
	DDX_Control(pDX, IDC_PROGRESS1, m_AxisHomeProgress);
}

BEGIN_MESSAGE_MAP(CDlgAxisHomeProgress, CDialogEx)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_MOTOR_ORIGINALL, &CDlgAxisHomeProgress::OnBnClickedButtonMotorOriginall)
	ON_BN_CLICKED(IDC_BUTTON_MOTOR_STOP, &CDlgAxisHomeProgress::OnBnClickedButtonMotorStop)
	ON_BN_CLICKED(IDC_BUTTON_MOTOR_ORIGIN_Y, &CDlgAxisHomeProgress::OnBnClickedButtonMotorOriginY)
	ON_BN_CLICKED(IDC_BUTTON_MOTOR_ORIGIN_Z, &CDlgAxisHomeProgress::OnBnClickedButtonMotorOriginZ)
	ON_BN_CLICKED(IDC_BUTTON_MOTOR_STOP2, &CDlgAxisHomeProgress::OnBnClickedButtonMotorStop2)
END_MESSAGE_MAP()

BOOL CDlgAxisHomeProgress::PreTranslateMessage(MSG* pMsg)
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

BOOL CDlgAxisHomeProgress::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(205, 205, 205));	
	//pDC->FillSolidRect(rect, RGB(227, 255, 0));		// Lime
	return TRUE;
}
// CDlgAxisHomeProgress 메시지 처리기

int CDlgAxisHomeProgress::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

BOOL CDlgAxisHomeProgress::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitComponent_AxisMoveHome(MOTOR_MAX_COUNT,1);

	m_AxisHomeProgress.SetRange(0, 100);
	m_AxisHomeProgress.SetPos(0);

	COMMON->SetOriginPopUp();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgAxisHomeProgress::InitComponent_AxisMoveHome(BYTE nAxis, int nAxis_Z1, int nAxis_Z2, int nAxis_Z3)
{
	m_stTitle.SetStaticStyle(CVGStatic::StaticStyle_GroupHeader);
	m_stTitle.SetColorStyle(CVGStatic::ColorStyle_Green);
	m_stTitle.SetFont_Gdip(L"Arial", 9.0F);
	m_stTitle.SetText(_T("AXIS MOVE HOME PROGRSS"));
	m_HomeCompliate = 0;
	m_nZAxisNum[0] = nAxis_Z1;
	m_nZAxisNum[1] = nAxis_Z2;
	m_nZAxisNum[2] = nAxis_Z3;
	m_nAxisY = SOCKET_Y;

	if (nAxis == 1)
		(CMFCButton*)GetDlgItem(IDC_BUTTON_MOTOR_ORIGIN_Z)->ShowWindow(SW_HIDE);	
}

void CDlgAxisHomeProgress::OnTimer(UINT_PTR nIDEvent)
{
	BOOL b;

	auto EqpType = GET_INSPECTOR;

	switch (nIDEvent)
	{
	case TIMER_INIT_AXES:
	{
		if (CIO::MoveSocketToSaftyZone() == false)
		{
			COMMON->ShowMsgDlg_Domodal(_T("Failed move socket to safety zone."), MODE_QUESTION);
			KillTimer(TIMER_INIT_AXES);
			m_nEachStep = 0;
			break;
		}

		m_AxisHomeProgress.SetPos(25);

		if (FAS_MO->IsConnected() == FALSE)
		{
			COMMON->ShowMsgDlg_Domodal(_T("First, Connet Motor. After Initialize Motor."), MODE_QUESTION);
			KillTimer(TIMER_INIT_AXES);
		}
		
		if (m_bFlag_InitStop == TRUE)
		{
			for (int i = 0; i < MOTOR_MAX_COUNT; i++)
			{
				FAS_MO->MoveStop(i);
				KillTimer(TIMER_INIT_AXES);

				m_nEachStep = 0;
			}
		}

		if (FAS_MO->IsMotioning(nManualInitial_Axis) == TRUE && m_nEachStep == 0 && m_nAllAxis_InitStep == 0)
		{
			FAS_MO->MoveStop(nManualInitial_Axis);
			m_strInitial.Format(_T("[Intial] Motor is Moving. Send Stop Command"));
			COMMON->Write_Main_Log(m_strInitial);
			KillTimer(TIMER_INIT_AXES);
			break;
		}

		switch (m_nEachStep)
		{
		case 0:
		{
			if (FAS_MO->MoveHome(nManualInitial_Axis) == FALSE)
			{
				FAS_MO->MoveStop(nManualInitial_Axis);

				m_strInitial.Format(_T("[Intial] Move Home Commnad Fail."));
				COMMON->Write_Main_Log(m_strInitial);

				m_nEachStep = 0;
				KillTimer(TIMER_INIT_AXES);
			}
			else
			{
				m_AxisHomeProgress.SetPos(50);
				m_nEachStep = 1;
				m_dwEachStep_Init = GetTickCount64();

				m_strInitial.Format(_T("[Intial] Each Axis Step Move 0->1"));
				COMMON->Write_Main_Log(m_strInitial);
			}
		}
		break;

		case 1:
		{
			if (GetTickCount64() - m_dwEachStep_Init < TIMEOUT_DELAY_1000)
				break;

			if (GetTickCount64() - m_dwEachStep_Init > TIMEOUT_DELAY_5000 * 10)
			{
				m_strInitial.Format(_T("[Intial] %d Axis Inital TimeOut"), nManualInitial_Axis);
				COMMON->Write_Main_Log(m_strInitial);
				break;
			}

			if (FAS_MO->IsHomeOK(nManualInitial_Axis) == TRUE)
			{
				m_strInitial.Format(_T("[Intial] Each Axis Step Move 1->2"));
				COMMON->Write_Main_Log(m_strInitial);
				m_AxisHomeProgress.SetPos(75);
				m_nEachStep = 2;
			}
		}
		break;
		case 2:
		{
			COMMON->m_bMotorOrginFlag[nManualInitial_Axis] = TRUE;

			m_strInitial.Format(_T("[Intial] Each Axis Home Move Compleate"));
			COMMON->Write_Main_Log(m_strInitial);
			m_AxisHomeProgress.SetPos(100);
			m_nEachStep = 0;
			KillTimer(TIMER_INIT_AXES);
		}
		break;

		}
	}
	break;

	case TIMER_INIT_EQUIP: //1000
	{
		// [21.1101.1] 메시지는 다른곳, 안전센서 감지추가
		if (FALSE == AJIN_IO->GetInputIoBit(DIO_INPUT_X0_06_AREASENSOR1))
		{
			FAS_MO->MoveStop(m_nZAxisNum[0]);
			FAS_MO->MoveStop(m_nAxisY);
			break;
		}

		int progressPercent = 6;
		int nStep = m_nAllAxis_InitStep + 1;

		switch (m_nAllAxis_InitStep)
		{
		case 0:
		{
			//if (CIO::MoveSocketToSaftyZone() == false)
			//{
			//	//COMMON->ShowMsgDlg_Domodal(_T("Failed move socket to safety zone."), MODE_QUESTION);
			//	m_strInitial.Format(_T("[Intial All] Failed move socket to safety zone."));
			//	COMMON->Write_Main_Log(m_strInitial);
			//	KillTimer(TIMER_INIT_EQUIP);
			//	m_nAllAxis_InitStep = 0;
			//	break;
			//}

			m_strInitial.Format(_T("[Intial All] Socket Cylinder Down & In, Axis Step Move 0->1"));
			COMMON->Write_Main_Log(m_strInitial);
			m_AxisHomeProgress.SetPos(progressPercent * nStep);
			m_nAllAxis_InitStep = 1;
		}
		break;
		case 1:
		{
			if (m_nZAxisNum[0] == -1 && m_nZAxisNum[1] == -1 && m_nZAxisNum[2] == -1)
			{
				m_strInitial.Format(_T("[Intial All] No Z Axis 1->6"));
				COMMON->Write_Main_Log(m_strInitial);
				m_AxisHomeProgress.SetPos(progressPercent * nStep);
				m_dwAllAxis_Init = GetTickCount64();
				m_nAllAxis_InitStep = 6;
				break;
			}

			// 모든축 정지
			for (int i = 0; i < MOTOR_MAX_COUNT; i++)
			{
				if (FAS_MO->IsMotioning(i) == TRUE)
				{
					FAS_MO->MoveStop(i);
				}
			}
			// 모든축 홈 초기화
			for (int i = 0; i < MOTOR_MAX_COUNT; i++)
			{
				FAS_MO->InitHomeOK(i);
			}

//			m_ctrlLight_ODA_Q[0].Write_OFF();
			
			m_AxisHomeProgress.SetPos(progressPercent * nStep);
			m_strInitial.Format(_T("[Intial All] Axis Stop & Reset Home Flags, Axis Step Move 1->2"));
			COMMON->Write_Main_Log(m_strInitial);
			m_nAllAxis_InitStep = 2;

			Sleep(1000);
		}
		break;

		case 2:
		{
			// 서보오프
			for (int i = 0; i < MOTOR_MAX_COUNT; i++)
			{
				FAS_MO->Servo_Off(i);
				Sleep(200);
				FAS_MO->Servo_Enable(i);
				Sleep(200);
				FAS_MO->Servo_Alarm_Reset_All();
				Sleep(200);
			}

			m_strInitial.Format(_T("[Intial All] Axis Alarm Reset, Axis Step Move 2->3"));
			COMMON->Write_Main_Log(m_strInitial);
			m_AxisHomeProgress.SetPos(progressPercent * nStep);
			m_nAllAxis_InitStep = 3;
			m_dwAllAxis_Init = GetTickCount64();
		}
		break;

		case 3:
		{
			// 서보온온
			for (int i = 0; i < MOTOR_MAX_COUNT; i++)
			{
				FAS_MO->Servo_Off(i);
				Sleep(500);
				FAS_MO->Servo_Enable(i);
				Sleep(500);
				FAS_MO->Servo_Alarm_Reset_Single(i);
				Sleep(500);
			}

			m_strInitial.Format(_T("[Intial All] Axis Servo on, Axis Step Move 3->4"));
			COMMON->Write_Main_Log(m_strInitial);
			m_AxisHomeProgress.SetPos(progressPercent * nStep);
			m_nAllAxis_InitStep = 4;
			m_dwAllAxis_Init = GetTickCount64();
		}
		break;

		case 4:
		{
			// 서보온 확인
			m_bAllAxis_CheckFlag = FALSE;
			for (int i = 0; i < MOTOR_MAX_COUNT; i++)
			{
				if (FAS_MO->IsServoStatus(i) == FALSE)
				{
					m_bAllAxis_CheckFlag = TRUE;
				}
			}

			if (m_bAllAxis_CheckFlag == TRUE)
			{
				m_strInitial.Format(_T("[Intial All] All Axis is not servo on"));
				COMMON->Write_Main_Log(m_strInitial);

				KillTimer(TIMER_INIT_EQUIP);
				m_nAllAxis_InitStep = 0;
				break;
			}
			else
			{
//				m_ctrlLight_ODA_Q[0].Write_OFF();

				CDevice::PowerOff();
				m_AxisHomeProgress.SetPos(progressPercent * nStep);
				m_strInitial.Format(_T("[Intial All] All motor is servo on , Axis Step Move 4->5"));
				COMMON->Write_Main_Log(m_strInitial);

				switch (EqpType)
				{
				case SYS_OQC_SFR_MULTI_CL:
					m_nAllAxis_InitStep = 5;
					break;

				case SYS_OQC_SFR_SINGLE_CL:
				case SYS_DISTORTION_30:
					m_nAllAxis_InitStep = 8;
					break;

// 				case SYS_DISTORTION_30:
// 					m_nAllAxis_InitStep = 5;
// 					break;
				}
			}
		}
		break;

		//*********************[Z AXIS ORIGIN]*********************
		case 5:
		{
			// Z축 Homming 시작 
			for (int j = 0; j < 3; j++)
			{
				if (m_nZAxisNum[j] != -1)
				{
					FAS_MO->MoveHome(m_nZAxisNum[j]);
				}
			}

			m_strInitial.Format(_T("[Intial All] Z Axis home start, Axis Step Move 5->6"));
			COMMON->Write_Main_Log(m_strInitial);
			m_AxisHomeProgress.SetPos(progressPercent * nStep);
			m_bAllAxis_CheckFlag = FALSE;
			m_nAllAxis_InitStep = 6;
			m_dwAllAxis_Init = GetTickCount64();
		}
		break;

		case 6:
		{
			// Z축 구동 중 확인
			if (GetTickCount64() - m_dwAllAxis_Init < TIMEOUT_DELAY_1000)
			{

			}
			else if ((b = FAS_MO->IsHoming(m_nZAxisNum[0])) == FALSE)
			{
				m_strInitial.Format(_T("[Intial All] Z Axis homming end check, Axis Step Move 6->7"));
				COMMON->Write_Main_Log(m_strInitial);

				m_bAllAxis_CheckFlag = FALSE;
				m_AxisHomeProgress.SetPos(progressPercent * nStep);
				m_dwAllAxis_Init = GetTickCount64();
				m_nAllAxis_InitStep = 7;
			}
			else if (GetTickCount64() - m_dwAllAxis_Init > TIMEOUT_DELAY_10000)
			{
				FAS_MO->MoveStop(m_nZAxisNum[0]);

				m_strInitial.Format(_T("[Intial All] Z Axis Inital TimeOut"));
				COMMON->Write_Main_Log(m_strInitial);

				KillTimer(TIMER_INIT_EQUIP);
			}
		}
		break;

		case 7:
		{
			// Z축 Homming 완료 확인
			if (GetTickCount64() - m_dwAllAxis_Init < TIMEOUT_DELAY_1000)
			{

			}
			else if ((b = FAS_MO->IsHomeOK(m_nZAxisNum[0])) == TRUE)
			{
				m_strInitial.Format(_T("[Intial All] Z Axis homming ok, Axis Step Move 7->8"));
				COMMON->Write_Main_Log(m_strInitial);

				m_bAllAxis_CheckFlag = FALSE;
				m_AxisHomeProgress.SetPos(progressPercent * nStep);
				m_dwAllAxis_Init = GetTickCount64();
				m_nAllAxis_InitStep = 8;
			}
			else
			{
				m_strInitial.Format(_T("[Intial All] Z Axis Inital Failed"));
				COMMON->Write_Main_Log(m_strInitial);

				KillTimer(TIMER_INIT_EQUIP);
				break;
			}
		}
		break;

		//*********************[Y AXIS ORIGIN]*********************
		case 8:
		{
			// Y축 Homming 시작 
			FAS_MO->MoveHome(m_nAxisY);

			m_strInitial.Format(_T("[Intial All] Y Axis home start, Axis Step Move 8->9"));
			COMMON->Write_Main_Log(m_strInitial);
			m_AxisHomeProgress.SetPos(progressPercent * nStep);
			m_nAllAxis_InitStep = 9;
			m_dwAllAxis_Init = GetTickCount64();
		}
		break;

		case 9:
		{
			// Y축 구동 중 확인
			if (GetTickCount64() - m_dwAllAxis_Init < TIMEOUT_DELAY_1000)
			{

			}
			else if ((b = FAS_MO->IsHoming(m_nAxisY)) == FALSE)
			{
				m_strInitial.Format(_T("[Intial All] Y Axis homming end check, Axis Step Move 9->10"));
				COMMON->Write_Main_Log(m_strInitial);
				m_AxisHomeProgress.SetPos(progressPercent * nStep);
				m_nAllAxis_InitStep = 10;
				m_dwAllAxis_Init = GetTickCount64();
			}
			else if (GetTickCount64() - m_dwAllAxis_Init > TIMEOUT_DELAY_60000)
			{
				m_strInitial.Format(_T("[Intial All] Y Axis Inital TimeOut"));
				COMMON->Write_Main_Log(m_strInitial);
				FAS_MO->MoveStop(m_nAxisY);

				KillTimer(TIMER_INIT_EQUIP);
			}
		}
		break;

		case 10:
		{
			// Y축 Homming 완료 확인
			if (GetTickCount64() - m_dwAllAxis_Init < TIMEOUT_DELAY_1000)
			{

			}
			else if ((b = FAS_MO->IsHomeOK(m_nAxisY) == TRUE))
			{
				m_strInitial.Format(_T("[Intial All] Y Axis homming ok , Axis Step Move 10->11"));
				COMMON->Write_Main_Log(m_strInitial);

				m_bAllAxis_CheckFlag = FALSE;
				m_AxisHomeProgress.SetPos(progressPercent * nStep);
				m_dwAllAxis_Init = GetTickCount64();
				m_nAllAxis_InitStep = 11;
			}
			else
			{
				m_strInitial.Format(_T("[Intial All] Y Axis Initial Failed"));
				COMMON->Write_Main_Log(m_strInitial);
				FAS_MO->MoveStop(m_nAxisY);
				KillTimer(TIMER_INIT_EQUIP);
				break;
			}
		}
		break;

		case 11:
		{
			MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();
			TEACHING_PARAM	stTeach = *DATAMANAGER->GetCurrentTeachingData();

			double dPos = 0.0f;
			dPos = (double)stTeach.nSocketY_Wait_Pos;
			double dSpeed = (double)stMotion.nMotorSpeed[0][2];

			if ((b = FAS_MO->SingleAxisAbsMove(m_nAxisY, dPos, dSpeed)) == FALSE)
			{
				m_strInitial.Format(_T("[Intial All] Y Axis Wait Pos Move Command Failed"));
				COMMON->Write_Main_Log(m_strInitial);
				KillTimer(TIMER_INIT_EQUIP);
			}
			else
			{
				m_strInitial.Format(_T("[Intial All] Axis Step Move 11->12"));
				COMMON->Write_Main_Log(m_strInitial);
				m_AxisHomeProgress.SetPos(progressPercent * nStep);
				m_dwAllAxis_Init = GetTickCount64();
				m_nAllAxis_InitStep = 12;
			}
		}
		break;

		case 12:
		{
			if (GetTickCount64() - m_dwAllAxis_Init < TIMEOUT_DELAY_1000)
			{

			}
			else if ( (b = FAS_MO->IsMotioning(m_nAxisY)) == FALSE)
			{
				m_AxisHomeProgress.SetPos(progressPercent * nStep);

				switch (EqpType)
				{
				case SYS_OQC_SFR_MULTI_CL:
					m_nAllAxis_InitStep = 13;
					break;

				default:
					m_nAllAxis_InitStep = 14;
					break;
				}
			}
			else if (GetTickCount64() - m_dwAllAxis_Init > TIMEOUT_DELAY_60000)
			{
				m_strInitial.Format(_T("[Intial All] Y Axis Wait Pos Move TimeOut"));
				COMMON->Write_Main_Log(m_strInitial);
				FAS_MO->MoveStop(m_nAxisY);
				KillTimer(TIMER_INIT_EQUIP);
			}
		}
		break;

		case 13:
		{
			//if (GetTickCount64() - m_dwAllAxis_Init < TIMEOUT_DELAY_1000)
			//	break;

			MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();
			TEACHING_PARAM	stTeach = *DATAMANAGER->GetCurrentTeachingData();

			double dPos = 0.0f;
			dPos = (double)stTeach.nSocketZ_Wait_Pos;
			double dSpeed = (double)stMotion.nMotorSpeed[0][2];

			if ((b = FAS_MO->SingleAxisAbsMove(m_nZAxisNum[0], dPos, dSpeed)) == FALSE)
			{
				m_strInitial.Format(_T("[Intial All] Z Axis Wait Pos Move Command Failed"));
				COMMON->Write_Main_Log(m_strInitial);
				KillTimer(TIMER_INIT_EQUIP);
			}
			else
			{
				m_strInitial.Format(_T("[Intial All] Axis Step Move 13->14"));
				COMMON->Write_Main_Log(m_strInitial);
				m_AxisHomeProgress.SetPos(progressPercent * nStep);
				m_dwAllAxis_Init = GetTickCount64();
				m_nAllAxis_InitStep = 14;
			}
		}
		break;

		case 14:
		{
			switch (EqpType)
			{
			case SYS_OQC_SFR_MULTI_CL:
			{
				if (GetTickCount64() - m_dwAllAxis_Init < TIMEOUT_DELAY_1000)
				{

				}
				else if ((b = FAS_MO->IsMotioning(m_nZAxisNum[0])) == FALSE)
				{
					for (int i = 0; i < MOTOR_MAX_COUNT; i++)
					{
						COMMON->m_bMotorOrginFlag[i] = TRUE;
					}
					m_AxisHomeProgress.SetPos(progressPercent * nStep);
					m_nAllAxis_InitStep = 15;
				}
				else if (GetTickCount64() - m_dwAllAxis_Init > TIMEOUT_DELAY_30000)
				{
					m_strInitial.Format(_T("[Intial All] Z Axis Wait Pos Move TimeOut"));
					COMMON->Write_Main_Log(m_strInitial);
					FAS_MO->MoveStop(m_nZAxisNum[0]);
					KillTimer(TIMER_INIT_EQUIP);
				}
			}
			break;

			case SYS_OQC_SFR_SINGLE_CL:
			case SYS_DISTORTION_30:
				for (int i = 0; i < MOTOR_MAX_COUNT; i++)
				{
					COMMON->m_bMotorOrginFlag[i] = TRUE;
				}
				m_AxisHomeProgress.SetPos(progressPercent * nStep);
				m_nAllAxis_InitStep = 15;
				break;
			}
			
		}
		break;

		case 15:
		{
			KillTimer(TIMER_INIT_EQUIP);

			AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_01_START_SWITCH_LAMP, FALSE);
			AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_02_STOP_SWITCH_LAMP, FALSE);
			AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_04_RESET_SWITCH_LAMP, FALSE);

			CIO::IO_O_Camera_Fix_Cylinder_Off();
			CIO::IO_O_Socket_Down();
			CIO::IO_O_Socket_Out();
			CIO::IO_O_Socket_Up();
			CIO::IO_O_ProbePin_Down();

			//AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_14_PROBEPIN_CYLINDER_UP, FALSE);
			//AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_15_PROBEPIN_CYLINDER_DOWN, TRUE);
			//COMMON->SetTowerLampStatus(eModeStop);
			COMMON->SetEqStatus(eStop);

#if(GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL)
			AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_10_DOORLOCK_ONOFF_FRONT, TRUE);
			AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_11_DOORLOCK_ONOFF_LEFT1, TRUE);
			AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_12_DOORLOCK_ONOFF_LEFT2, TRUE);
			AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_13_DOORLOCK_ONOFF_RIGHT1, TRUE);
			AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_14_DOORLOCK_ONOFF_RIGHT2, TRUE);
			AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_15_DOORLOCK_ONOFF_BACK1, TRUE);
			AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_16_DOORLOCK_ONOFF_BACK2, TRUE);
#elif(GET_INSPECTOR == SYS_OQC_SFR_SINGLE_CL)
			AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_10_DOORLOCK_ONOFF_FRONT, TRUE);
			AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_11_DOORLOCK_ONOFF_LEFT1, TRUE);
			AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_12_DOORLOCK_ONOFF_RIGHT1, TRUE);
			AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_13_DOORLOCK_ONOFF_BACK1, TRUE);
			AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_14_DOORLOCK_ONOFF_BACK2, TRUE);
#endif

			AJIN_IO->BlinkStop(DIO_OUTPUT_Y0_01_START_SWITCH_LAMP, TRUE);
			AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_01_START_SWITCH_LAMP, FALSE);
			AJIN_IO->BlinkStop(DIO_OUTPUT_Y0_02_STOP_SWITCH_LAMP, TRUE);
			AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_02_STOP_SWITCH_LAMP, FALSE);
			m_strInitial.Format(_T("[Intial All] Axis Step Move 14->15"));
			COMMON->Write_Main_Log(m_strInitial);
			SEQUENCE->Clear_MainUiFlag();
			m_AxisHomeProgress.SetPos(100);

			m_nAllAxis_InitStep = 99;
			AJIN_IO->BlinkStop(DIO_OUTPUT_Y0_03_INIT_SWITCH_LAMP, FALSE);
			// [21.1214.6]
			AJIN_IO->ChangeBtnStatus(DIO_OUTPUT_Y0_01_START_SWITCH_LAMP, 1, FALSE);
			m_UdpTester[0].Reset_TesterFlag();
			m_UdpTester[0].Reset_ReadyFlag();
			COMMON->SetMOAllInitNeedFlag(FALSE);
			COMMON->SetEMOAllInitFlag(FALSE);
			COMMON->SetFirstInit();

			SEQUENCE->ErrorClear();
			COMMON->ResetOriginPopUp();

			m_strInitial.Format(_T("[Intial All] Axis Home Move Complete"));
			COMMON->Write_Main_Log(m_strInitial);
			KillTimer(TIMER_INIT_EQUIP);
			OnCancel();
		}
		break;

		case 99:
			break;

		}
	}
	break;
	
	case TIMER_INIT_INTLK:
	{
		// EMO 눌림
		if (FALSE == AJIN_IO->GetInputIoBit(DIO_INPUT_X0_04_EMO))
		{			
			FAS_MO->Disconnect_EziServo();

			KillTimer(TIMER_INIT_EQUIP);
			KillTimer(TIMER_INIT_INTLK);
			FAS_MO->MoveStop(0);
			FAS_MO->MoveStop(1);
			m_nAllAxis_InitStep = 0;
			COMMON->SetMOAllInitNeedFlag(TRUE);
			COMMON->SetEqStatus(eError);

			EQ_BASIC_PARAM	stEq = *DATAMANAGER->GetCurrentEqBasicData();
			if (stEq.nLanguage == 0)
				COMMON->ShowMsgDlg_Domodal(_T("[Origin] EMO버튼이 눌렸습니다. \nEMO 해제후 Reset 버튼을 눌러주십시오."), MODE_ERROR);
			else
				COMMON->ShowMsgDlg_Domodal(_T("[Origin] The EMO button has been pressed. \nAfter releasing the EMO, press the Reset button.."), MODE_ERROR);

			COMMON->SetEqStatus(eStop);
			OnCancel();
			break;
		}
		// 안전센서감지
		/*if (FALSE == AJIN_IO->GetInputIoBit(DIO_INPUT_X0_06_AREASENSOR1))
		{
			KillTimer(TIMER_INIT_EQUIP);
			KillTimer(TIMER_INIT_INTLK);
			FAS_MO->MoveStop(0);
			FAS_MO->MoveStop(1);
			m_nAllAxis_InitStep = 0;
			COMMON->SetMOAllInitNeedFlag(TRUE);
			COMMON->SetEqStatus(eError);

			EQ_BASIC_PARAM	stEq = *DATAMANAGER->GetCurrentEqBasicData();
			if (stEq.nLanguage == 0)
				COMMON->ShowMsgDlg_Domodal(_T("Area 센서 감지되었습니다."), MODE_ERROR);
			else
				COMMON->ShowMsgDlg_Domodal(_T("Area sensor detected"), MODE_ERROR);

			COMMON->SetEqStatus(eStop);
			OnCancel();
			break;
		}*/

		// 소켓 위치 확인
		/*if (FALSE == COMMON->GetInterlock_SocketInAndDown())
		{
			KillTimer(TIMER_INIT_EQUIP);
			KillTimer(TIMER_INIT_INTLK);
			FAS_MO->MoveStop(0);
			FAS_MO->MoveStop(1);
			m_nAllAxis_InitStep = 0;
			COMMON->SetMOAllInitNeedFlag(TRUE);
			COMMON->SetEqStatus(eError);

			EQ_BASIC_PARAM	stEq = *DATAMANAGER->GetCurrentEqBasicData();
			if (stEq.nLanguage == 0)
				COMMON->ShowMsgDlg_Domodal(_T("소켓 실린더가 안전 위치에 있지 않습니다. 소켓 위치를 바꿔주세요."), MODE_INFORMATION);
			else
				COMMON->ShowMsgDlg_Domodal(_T("Socket cylinder is not Safety Zone, Please Move socket position."), MODE_INFORMATION);

			COMMON->SetEqStatus(eStop);
			OnCancel();
			break;
		}*/
	}
	break;

	}

	CDialogEx::OnTimer(nIDEvent);
}

void CDlgAxisHomeProgress::OnBnClickedButtonMotorOriginall()
{
	CString strTemp = _T("");
	EQ_BASIC_PARAM	stEq = *DATAMANAGER->GetCurrentEqBasicData();
	if (SEQUENCE->GetRunMode() == eRunning)
	{
		if (stEq.nLanguage == 0)
			COMMON->ShowMsgDlg_Domodal(_T("AutoMode중에는 초기화를 하실수 없습니다."), MODE_INFORMATION);
		else
			COMMON->ShowMsgDlg_Domodal(_T("You cannot initialize while in AutoMode."), MODE_INFORMATION);
		return;
	}

	if (FAS_MO->IsConnected() == false)
	{
		MOTION_PARAM	stSpeed = *DATAMANAGER->GetCurrentMotionParam();
		DWORD BaudInitList_Motor[] = { 9600, 19200, 38400, 57600, 115200 };

		if (FAS_MO->Connect_EziServo(stSpeed.nPort[0], BaudInitList_Motor[stSpeed.nBaudRate[0]]) == false)
		{
			EQ_BASIC_PARAM	stEq = *DATAMANAGER->GetCurrentEqBasicData();
			if (stEq.nLanguage == 0)
				COMMON->ShowMsgDlg_Domodal(_T("모터를 연결해 주신후 초기화를 진행해 주십시오."), MODE_QUESTION);
			else
				COMMON->ShowMsgDlg_Domodal(_T("Please connect the motor and proceed with initialization."), MODE_QUESTION);
		}
	}

	if (COMMON->GetPermissionGrade() == LVL_OPER)
	{
		if (TRUE != COMMON->GetInterlock_DoorCheck())
		{
			for (int nOutSignal = 0; nOutSignal < EDoor_Max; nOutSignal++)
			{
				AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_10_DOORLOCK_ONOFF_FRONT + nOutSignal, TRUE);
			}
			Sleep(200);
			for (int nOutSignal = 0; nOutSignal < EDoor_Max; nOutSignal++)
			{
				AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_10_DOORLOCK_ONOFF_FRONT + nOutSignal, TRUE);
			}
			Sleep(200);
			if (TRUE != COMMON->GetInterlock_DoorCheck())
			{
				if (stEq.nLanguage == 0)
					COMMON->ShowMsgDlg_Domodal(_T("문이 열렸습니다. 문을 닫고 다시시도해주세요."), MODE_INFORMATION);
				else
					COMMON->ShowMsgDlg_Domodal(_T("Door is opened. Closed Door. Please try again."), MODE_INFORMATION);
				return;
			}
		}
	}

	if (CIO::IO_I_EMO() == false)
	{
		FAS_MO->Disconnect_EziServo();
		//FAS_MO->m_bConnected = FALSE;
		COMMON->SetMOAllInitNeedFlag(TRUE);
		COMMON->SetEqStatus(eError);

		if (stEq.nLanguage == 0)
			COMMON->ShowMsgDlg_Domodal(_T("[Origin] EMO버튼이 눌렸습니다. \nEMO 해제후 Reset 버튼을 눌러주십시오."), MODE_ERROR);
		else
			COMMON->ShowMsgDlg_Domodal(_T("[Origin] The EMO button has been pressed. \nPlease press the Reset button after releasing the EMO."), MODE_ERROR);
		COMMON->SetEqStatus(eStop);
		return;
	}

	CString strMsg;
	if (stEq.nLanguage == 0)
	{
		strMsg.Format(_T("초기화를 진행하시겠습니까?"));
		strTemp.Format(_T("장비초기화 취소."));
	}
	else
	{
		strMsg.Format(_T("Do you want initialization?"));
		strTemp.Format(_T("Cancel initialization of equipment."));
	}

	if (COMMON->ShowMsgDlg_Domodal(strMsg, MODE_QUESTION) == FALSE)
	{
		COMMON->Write_Main_Log(strTemp);
	}
	else
	{
		m_nAllAxis_InitStep = 0;
		
		SetTimer(TIMER_INIT_EQUIP, 250, NULL);
		SetTimer(TIMER_INIT_INTLK, 200, NULL);
	}
	
	COMMON->SetEqStatus(eHome);
	CLuriMotion_OQA_SFR_WideDlg *pParrentDlg = (CLuriMotion_OQA_SFR_WideDlg*)AfxGetApp()->GetMainWnd();
	// [22.0118.1]
	pParrentDlg->m_pDlgMain->EnableWindow(FALSE);
	pParrentDlg->m_pDlgSide->EnableWindow(FALSE);
	pParrentDlg->m_pDlgBtm->EnableWindow(FALSE);
	pParrentDlg->m_pDlgTitle->EnableWindow(FALSE);
	pParrentDlg->m_pDlgTeach->EnableWindow(FALSE);
	pParrentDlg->m_pDlgIO->EnableWindow(FALSE);
}

void CDlgAxisHomeProgress::OnBnClickedButtonMotorStop()
{
	m_bFlag_InitStop = TRUE;
}

void CDlgAxisHomeProgress::OnBnClickedButtonMotorOriginY()
{
	nManualInitial_Axis = 0;
	SetTimer(TIMER_INIT_AXES, 250, NULL);
}

void CDlgAxisHomeProgress::OnBnClickedButtonMotorOriginZ()
{
	nManualInitial_Axis = 1;
	SetTimer(TIMER_INIT_AXES, 250, NULL);
}

void CDlgAxisHomeProgress::OnBnClickedButtonMotorStop2()
{
	COMMON->ResetOriginPopUp();
	OnOK();
}
