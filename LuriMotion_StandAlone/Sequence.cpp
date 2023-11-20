#include "pch.h"
#include "Sequence.h"

#include "LuriMotion_OQA_SFR_Wide.h"
#include "LuriMotion_OQA_SFR_WideDlg.h"

#include "Def_Sequence.h"
#include "IO.h"
#include "Motion.h"
#include "Device.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CSequence::CSequence()
{
	m_pThreadRun = NULL;

	m_bFlagProcess_NormalFinish = FALSE;

	m_bFirstIo = true;

	SetStep(3);

	m_bRunMode = eStop;
}

CSequence::~CSequence()
{
	if (m_pThreadRun)
	{
		m_bThreadRun = FALSE;
		WaitForSingleObject(m_pThreadRun->m_hThread, 500);
	}
}

bool CSequence::Begin_Thread(ETHREAD_ID tid)
{
	if (tid == THREAD_MANUAL)
	{

	}
	else if (tid == THREAD_INITIAL)
	{

	}
	else if (tid == THREAD_INTLK)
	{
		if (m_pThread[tid])
		{
			End_Thread(tid);
		}
		m_bThread[tid] = TRUE;
		m_pThread[tid] = AfxBeginThread(Thread_Inlk, this, THREAD_PRIORITY_NORMAL);
	}
	else if (tid == THREAD_IO)
	{
		if (m_pThread[tid])
		{
			End_Thread(tid);
		}
		m_bThread[tid] = TRUE;
		m_pThread[tid] = AfxBeginThread(Thread_MonitoringIo, this, THREAD_PRIORITY_NORMAL);
	}
	return true;
}

void CSequence::End_Thread(ETHREAD_ID tid)
{
	if (m_bThread[tid] == TRUE)
	{
		m_bThread[tid] = FALSE;
		WaitForSingleObject(m_pThread[tid]->m_hThread, 500);
	}
}

bool CSequence::Begin_RunThread()
{
	if (m_pThreadRun)
	{
		End_RunThread();
	}
	m_bThreadRun = TRUE;
	m_pThreadRun = AfxBeginThread(Thread_Run, this, THREAD_PRIORITY_NORMAL);

	return true;
}

void CSequence::End_RunThread()
{
	if (m_pThreadRun)
	{
		m_bThreadRun = FALSE;
		WaitForSingleObject(m_pThreadRun->m_hThread, 500);				//Log Message 때문에 무한대기에 빠진다
	}
}

//bool CSequence::Begin_RunThreadStop()
//{
//	if (Run_Replay() == false)
//	{
//		return false;
//	}
//	
//	if (m_bThreadRunStop == TRUE)
//	{
//	}
//	else
//	{
//
//	}
//
//	return true;
//}

void CSequence::End_RunThreadStop()
{
	if (m_bThreadRunStop == TRUE)
	{
		m_bThreadRunStop = FALSE;
		WaitForSingleObject(m_pThreadRunStop->m_hThread, 500);				//Log Message 때문에 무한대기에 빠진다
	}
}

void CSequence::Start_Inlk()
{
	if (m_bThread[THREAD_INTLK] == FALSE)
	{
		Begin_Thread(THREAD_INTLK);
	}
	if (GetRunMode() == eRunning)
		return;

	WRITE_SEQ_LOG(_T("Equipment Start."));
}

void CSequence::Start_MonitoringIO()
{
	if (m_bThread[THREAD_IO] == FALSE)
	{
		Begin_Thread(THREAD_IO);
	}
	if (GetRunMode() == eRunning)
		return;

	WRITE_SEQ_LOG(_T("Io Monitoring Start."));
}
void CSequence::Start_StartupCheck(__in int nIdx, __in int nCaptureIdx)
{
	// 동작중일경우
	if (IsTesting_StartupCheck())
		return;

	if (m_hThreadStartupCheck)
	{
		CloseHandle(m_hThreadStartupCheck);
		m_hThreadStartupCheck = NULL;
	}

	m_bThreadStartupCheck = true;

	stThreadParam* pParam = new stThreadParam;
	pParam->pOwner = this;
	pParam->nPort = nCaptureIdx;
	pParam->nArg = nIdx;

	m_hThreadStartupCheck = HANDLE(_beginthreadex(NULL, 0, Thread_StartupCheck, pParam, 0, NULL));
}

void CSequence::Stop_StartupCheck()
{
	m_bThreadStartupCheck = false;
}

void CSequence::Start_Run()
{
	if (m_bThreadRun == FALSE)
		Begin_RunThread();
	
	if (GetRunMode() == eRunning)
		return;

	if (COMMON->GetAreaStop_CancelPush() == FALSE)
	{
		if( m_UdpTester[0].m_bTesterFlag_Ready == TRUE)
			AJIN_IO->ChangeBtnStatus(DIO_OUTPUT_Y0_01_START_SWITCH_LAMP, 1, TRUE);
	}		

	// step repla
	RestartStep();

	m_bRunMode = eRunning;
	WRITE_SEQ_LOG(_T("SEQUENCE Start."));
}
void CSequence::Stop_Run()
{
	m_bFlagProcess_NormalFinish = TRUE;
	m_bRunMode = eStop;
	WRITE_SEQ_LOG(_T("SEQUENCE Normal Stop."));
}
void CSequence::Stop_Run_nonNormal()//JYS 210930
{
	m_bRunMode = eStop;
	WRITE_SEQ_LOG(_T("SEQUENCE Non Normal Stop."));
}

void CSequence::Pause_Run()
{
	m_bRunMode = ePause;
	WRITE_SEQ_LOG(_T("SEQUENCE Normal Pause."));
}
void CSequence::Emergency_Run()
{
	m_bRunMode = eEStop;
	WRITE_SEQ_LOG(_T("SEQUENCE Emergency Stop."));
}

// JYS 210923 TEST3.
void CSequence::AutoMode_Run()
{
	m_bFlagProcess_NormalFinish = FALSE;
	m_bRunMode = eStandby;
	WRITE_SEQ_LOG(_T("SEQUENCE AutoMode START."));
}
void CSequence::ManualMode_Run()
{
	m_bFlagProcess_NormalFinish = TRUE;
	m_bRunMode = eStop;
	WRITE_SEQ_LOG(_T("SEQUENCE Manual Mode Stop."));
}
// JYS 210923 TEST3. END
void CSequence::Init_Compleate()
{
	m_bRunMode = eStandby;
	WRITE_SEQ_LOG(_T("SEQUENCE Init Compleate."));
}

void CSequence::SetAreaStopLampnSideFlag(BOOL bState)
{
	m_bFlagProcess_AReaStopLampnSide = bState;
}

BOOL CSequence::GetAreaStopLampnSideFlag()
{
	return m_bFlagProcess_AReaStopLampnSide;
}

void CSequence::SetStopswitchMode(BOOL bMode)
{
	m_bStopSwitchPushed = bMode;
}
BOOL CSequence::GetStopSwtichMode()
{
	return m_bStopSwitchPushed;
}

void CSequence::SetSFR_Inspection_Step()
{
	SetInspectionStep();
}

void CSequence::Clear_MainUiFlag()
{
	for (int i = 0; i < 19; i++)
	{
		COMMON->m_bMainUiFlag_StepView[i] = FALSE;
		COMMON->m_bMainUiFlag_StepView_Old[i] = FALSE;
	}

	for (int i = 0; i < 2; i++)
	{
		COMMON->m_nMainUiFlag_InspectResult[i] = -1;
		COMMON->m_nMainUiFlag_InspectResult_Old[i] = -1;
	}

	for (int i = 0; i < 4; i++)
	{
		COMMON->m_bMainUiFlag_MoveSocket[i] = FALSE;
		COMMON->m_bMainUiFlag_MoveSocket_Old[i] = FALSE;
	}

	for (int i = 0; i < 3; i++)
	{
		COMMON->m_bMainUiFlag_CurrentAction[i] = FALSE;
		COMMON->m_bMainUiFlag_CurrentAction_Old[i] = FALSE;
	}

	COMMON->m_nMainUiFlag_Refresh = TRUE;
}

bool CSequence::IsTesting_StartupCheck()
{
	DWORD dwStatus = NULL;

	if (GetExitCodeThread(m_hThreadStartupCheck, &dwStatus))
	{
		if (STILL_ACTIVE == dwStatus)
			return true;
		else
			return false;
	}
	return false;
}


UINT CSequence::Run_StartupCheck()
{
	auto err = 0;	

	while (IsThread_StartupCheck())
	{
#ifndef DevMode_Simulator
		err = Equipment_AreaSensor();
#endif
		
		if(err == MCEC_OK)
			err = StartOperation_Startup();

		if (err != MCEC_OK)
		{
			SHOW_ERROR((enMCErrCode)err);
			break;
		}

		if (StartUp_GetStep() == 1 || StartUp_GetStep() == 2)	// complete step
			break;
	}

	return err;
}

UINT CSequence::Thread_StartupCheck(__in LPVOID lParam)
{
	CSequence*	pThis = (CSequence*)((stThreadParam*)lParam)->pOwner;
	UINT		nIdx = ((stThreadParam*)lParam)->nArg;
	UINT		capture = ((stThreadParam*)lParam)->nPort;

	if (NULL != lParam)
		delete lParam;

	auto err = 0;

	pThis->Initialize_Startup(nIdx, capture);
	err = pThis->Run_StartupCheck();
	pThis->Finalize_Startup((enMCErrCode)err, nIdx, capture);
	return 0;
}

UINT CSequence::Thread_Inlk(LPVOID lpVoid)
{
	while (SEQUENCE->m_bThread[THREAD_INTLK] == TRUE)
	{
		Sleep(20);

#ifndef DevMode_Simulator
		SEQUENCE->Run_Equipment();
#endif
	}

	SEQUENCE->m_pThread[THREAD_INTLK] = NULL;
	SEQUENCE->m_bThread[THREAD_INTLK] = FALSE;

	return 0;
}

UINT CSequence::Thread_MonitoringIo(LPVOID lpParam)
{
	while (SEQUENCE->m_bThread[THREAD_IO] == TRUE)
	{
		Sleep(20);

		SEQUENCE->Run_IO();
	}

	SEQUENCE->m_pThread[THREAD_IO] = NULL;
	SEQUENCE->m_bThread[THREAD_IO] = FALSE;

	return 0;
}

RUN_MODE CSequence::GetRunMode()
{
	return /*(RUN_MODE)*/m_bRunMode;
}

void CSequence::SetRunMode(RUN_MODE rm)
{
	m_bRunMode = rm;
}

BOOL CSequence::Run_Replay()
{
	return TRUE;
}

BOOL CSequence::Run_IO()
{
	auto size = _countof(m_szDio_Output_Eng);

	for (int i = 0; i < size; i++)
	{
		auto status = (bool)AJIN_IO->GetInputIoBit(i);

		if (status != m_bOldIoInput[i])
		{
			m_bOldIoInput[i] = status;

			if (m_bFirstIo)
				OnDetectIoSingal(i, status);
		}
	}

	m_bFirstIo = true;
	return TRUE;
}

BOOL CSequence::Run_Equipment()
{
	// EMO 
	Equipment_EMO();

	// POWER OFF 되었을경우 
	Equipment_PowerOff();

	// Air 없을 경우 
	Equipment_MainAir();

	// 원점 안잡았을 경우 
	Equipment_Initialize();

	return TRUE;
}

void CSequence::Reset_SeqFlags()
{
// #if (GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL) || (GET_INSPECTOR == SYS_OQC_SFR_SINGLE_CL)
// 	if (COMMON->GetFlagStartUpMode() == TRUE)
// 		SetStep(SEQ_STEP_SFR_WIDE_STARTUPMODE_ACTION_WAIT);
// 	else
// 		SetStep(SEQ_STEP_SFR_WIDE_READY);
// 
// 	m_nStep_SFR_MoveWaitPos = SEQ_STEP_SFR_MOVEBACK_STEP001_CHECK_BACK_COMMAND;
// #else
// 	if (COMMON->GetFlagStartUpMode() == TRUE)
// 		m_nStep_StandAlone = SEQ_STEP_DISTORTION_STARTUPMODE_ACTION_WAIT;
// 	else
// 		m_nStep_StandAlone = SEQ_STEP_DISTORTION_READY;
// #endif
// 	m_bFlagProcess_NormalFinish = FALSE;
	ResetStepFlag();
}

UINT CSequence::Thread_Run(LPVOID lpVoid)
{
	/////////////////////////////////////////////
	CString m_strMsg; m_strMsg.Empty();

	enMCErrCode err = MCEC_OK;

	while (SEQUENCE->m_bThreadRun == TRUE)
	{
		Sleep(20);

		auto runmode = SEQUENCE->GetRunMode();
		auto initflag = COMMON->GetMOAllInitNeedFlag();

		BOOL b1, b2;
			
		if (SEQUENCE->GetRunMode() != eRunning)
			continue;

		err = MCEC_OK;

		// STOP 버튼 누르기 전
		b1 = SEQUENCE->IsStep_BeforePushStopSwitch();

		// STARTUP 모드 대기가 아닐경우..
		b2 = SEQUENCE->IsStep_NotStartupModeWait();

		if (b1 && b2)
		{
			err = COMMON->GetInterlock_AreaSensor();
		}

		if (err == MCEC_OK)
			err = SEQUENCE->Run_SFR_Inspection();
		

		if (err != MCEC_OK)
		{
			SEQUENCE->SetRunMode(RUN_MODE::eError);

			// 알람 메시지 박스 출력 
			SHOW_ERROR((enMCErrCode)err); 

			SEQUENCE->SetRunMode(RUN_MODE::eStop);
		}
	}

	SEQUENCE->m_pThreadRun = NULL;
	SEQUENCE->m_bThreadRun = FALSE;

	return 0;
}
UINT CSequence::Thread_RunStop(LPVOID lpVoid)
{

	/////////////////////////////////////////////

	while (SEQUENCE->m_bThreadRunStop == TRUE)
	{		
		Sleep(30);
		
		if (SEQUENCE->GetRunMode() == eRunning)
			continue;
	}

	SEQUENCE->m_pThreadRunStop = NULL;
	SEQUENCE->m_bThreadRunStop = FALSE;

	return 0;
}

enMCErrCode CSequence::Equipment_EMO()
{
	auto err = MCEC_OK;

	if (!CIO::IO_I_EMO())
	{
//#if(GET_INSPECTOR != SYS_DISTORTION_30)
		FAS_MO->Disconnect_EziServo();
		FAS_MO->m_bConnectedInitFlag = 0;

		CMotion::MO_AllAxis_Reset();
//#endif
		COMMON->SetEMOAllInitFlag(TRUE);
		COMMON->SetMOAllInitNeedFlag(TRUE);
		COMMON->SetForced_SideBtn_StopOn(TRUE);

		ResetStepFlag();

		CIO::IO_O_INIT(false);
		CIO::IO_O_RESET(false);

		SetRunMode(eError);

		EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();

		COMMON->DoEvent(100);
		if (stBasic.nLanguage == 0)
			COMMON->ShowMsgDlg_Domodal(_T("EMO버튼이 눌렸습니다. \r\n EMO 해제후 Reset 버튼을 눌러주십시오."), MODE_ERROR);
		else
			COMMON->ShowMsgDlg_Domodal(_T("The EMO button has been pressed. \r\n Please press the Reset button after releasing the EMO."), MODE_ERROR);

		err = MCEC_EMO;

		SetRunMode(eStop);
	}

	return err;
}

enMCErrCode CSequence::Equipment_PowerOff()
{
	auto err = MCEC_OK;

	if (CIO::IO_I_EMO())
	{
		if (!CIO::IO_I_PowerOff() && COMMON->GetSleepModeStep() != SLEEP_ENABLE && COMMON->GetSleepModeStep() != SLEEP_FINISH)
		{
			CIO::IO_O_RESET(true);

//#if(GET_INSPECTOR != SYS_DISTORTION_30)
			CMotion::MO_AllAxis_Reset();
//#endif

			COMMON->SetEMOAllInitFlag(TRUE);
			COMMON->SetMOAllInitNeedFlag(TRUE);
			SetRunMode(eError);

			EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();
			if (stBasic.nLanguage == 0)
				COMMON->ShowMsgDlg_Domodal(_T("PowerOff 상태입니다. \r\n Reset 버튼을 눌러주십시오."), MODE_ERROR);
			else
				COMMON->ShowMsgDlg_Domodal(_T("Equipment is Power Off.  \r\n Please press the Reset button "), MODE_ERROR);

			SetRunMode(eStop);

			err = MCEC_PowerOff;
		}
		else
		{
			CIO::IO_O_RESET(false);
		}
	}
	else
	{
		CIO::IO_O_RESET(false);
	}

	return err;
}

enMCErrCode CSequence::Equipment_MainAir()
{
	auto err = MCEC_OK;

	if (!CIO::IO_I_MainAirPress())
	{
		SetRunMode(eError);

		EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();
		if (stBasic.nLanguage == 0)
			COMMON->ShowMsgDlg_Domodal(_T("Main Air 공압이 없습니다. \r\n Main Air 상태 확인 바랍니다."), MODE_ERROR);
		else
			COMMON->ShowMsgDlg_Domodal(_T("Main Air Pressure Not Enough \r\n Please check main air "), MODE_ERROR);

		SetRunMode(eStop);

		err = MCEC_AirPressure;
	}

	return err;
}

enMCErrCode CSequence::Equipment_Door()
{
	auto err = MCEC_OK;
	return err;
}

enMCErrCode CSequence::Equipment_Initialize()
{
	auto err = MCEC_OK;

	if (CIO::IO_I_EMO())
	{
		if (CIO::IO_I_PowerOff())
		{
			if (COMMON->GetEMOAllInitFlag() || COMMON->GetFirstInit() == false)
			{
				if (COMMON->GetOriginPopUp())
					CIO::IO_O_INIT(false);
				else
					CIO::IO_O_INIT(true);
			}
			else
			{
				CIO::IO_O_INIT(false);
			}
		}
	}
	else
	{
		CIO::IO_O_INIT(false);
	}

	return err;
}

enMCErrCode CSequence::Equipment_AreaSensor()
{
	auto err = MCEC_OK;

	if (CIO::IO_I_AREA_SENSOR() == false)
	{
		COMMON->SetForced_SideBtn_StopOn(TRUE);
		err = MCEC_AreaSensorDetect;
	}
	else
		err = MCEC_OK;

	return err;
}

//----------------------------------------------------------------------------------
// Thread.
enMCErrCode	CSequence::Run_SFR_Inspection()
{	
	auto err = MCEC_OK;

	// 스위치 버튼 push 상태 (1초 동안 눌러야한다)
	if (COMMON->GetForcedStop_1Sec() == TRUE)
	{
		if(IsStep_UnderMoveY())
		{
			if (AJIN_IO->GetInputIoBit(DIO_INPUT_X0_01_START) == FALSE)
			{
				WRITE_SEQ_LOG(_T("MCH-Stop // START BTN PUSH 1sec less "));

				COMMON->m_dwForcedStop_Under_1Sec = 0;
				COMMON->SetForcedStop_1Sec(FALSE);

				AJIN_IO->ChangeBtnStatus(DIO_OUTPUT_Y0_01_START_SWITCH_LAMP, 0, TRUE);

				// [21.1208.3] Del
				/*COMMON->SetAreaStop_OkPush(TRUE);*/
				Pause_Run();
				return MCEC_OK;
			}
		}
		else
		{
			if (AJIN_IO->GetInputIoBit(DIO_INPUT_X0_01_START) == TRUE)
			{
				AJIN_IO->BlinkStop(DIO_OUTPUT_Y0_01_START_SWITCH_LAMP, TRUE);
				AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_01_START_SWITCH_LAMP, FALSE);
				AJIN_IO->BlinkStop(DIO_OUTPUT_Y0_02_STOP_SWITCH_LAMP, TRUE);
				AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_02_STOP_SWITCH_LAMP, FALSE);
			}
			else if (AJIN_IO->GetInputIoBit(DIO_INPUT_X0_01_START) == FALSE)
			{
				AJIN_IO->ChangeBtnStatus(DIO_OUTPUT_Y0_01_START_SWITCH_LAMP, 1, TRUE);

				COMMON->m_dwForcedStop_Under_1Sec = 0;
				COMMON->SetForcedStop_1Sec(FALSE);
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////

	// 설비별 시퀀스 동작
	err = (enMCErrCode)StartOperation_Run();

	if (err != MCEC_OK)
	{
		AJIN_IO->ChangeBtnStatus(DIO_OUTPUT_Y0_02_STOP_SWITCH_LAMP, 1, TRUE);

		// 알람 메시지 박스 출력 
		SHOW_ERROR((enMCErrCode)err);
		return err;
	}

	return err;
}

BOOL CSequence::Run_SFR_MoveWaitPos()
{
	TESTER_COMM		stTester = *DATAMANAGER->GetCurrentTesterCommData();
	TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();
	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();

	if (SEQUENCE->m_bRunMode == eStop)
		return TRUE;
	if (SEQUENCE->m_bRunMode == ePause)
		return TRUE;
	if (SEQUENCE->m_bRunMode == eStandby)
		return TRUE;

	return TRUE;
}

void CSequence::CameraInit() //james test
{
	m_UdpTester[0].Send_Tester_Communication(eComMode_Insp_PreviewStop);
	COMMON->SetProbePinCylinder_UpDown(_CYLINDER_DOWN);
	Sleep(2000);
	COMMON->SetProbePinCylinder_UpDown(_CYLINDER_UP);
	Sleep(1000);
	//m_ctrlLight_ODA_Q[0].Write_OFF();
	CDevice::PowerOff();
	Sleep(1000);
//	m_ctrlLight_ODA_Q[0].Wrtie_ON();
	CDevice::PowerOn();
	Sleep(1000);
	m_UdpTester[0].m_bTesterFlag_PreviewStartErrorCode = FALSE;
	m_UdpTester[0].Send_Tester_Communication(eComMode_PreviewStart);
	Sleep(3000);
	m_dwTimer_Preview = GetTickCount();
	m_nPreviewRetry = 2;

	Sleep(5000);
}

void CSequence::OnDetectIoSingal(__in int port, __in bool bOn)
{
	DIGITAL_INPUT idx = (DIGITAL_INPUT)port;

	switch (idx)
	{
	case DIO_INPUT_X0_00_POWER:
	{
		if (bOn)
		{
			CIO::IO_O_RESET(false);
		}
		else
		{
		/*	COMMON->SetEMOAllInitFlag(TRUE);
			FAS_MO->Disconnect_EziServo();
			FAS_MO->m_bConnectedInitFlag = 0;*/
		}
	}
	break;

	case DIO_INPUT_X0_01_START:
	{
		if (bOn)
		{
		}
		else
		{
		}
	}
	break;

	case DIO_INPUT_X0_02_STOP:
	{
		if (bOn)
		{

		}
		else
		{

		}
	}
	break;

	case DIO_INPUT_X0_03_INIT:
	{
		if (bOn)
		{
			if (GetRunMode() != RUN_MODE::eRunning )
			{
				CLuriMotion_OQA_SFR_WideDlg *pDlg = (CLuriMotion_OQA_SFR_WideDlg*)AfxGetApp()->GetMainWnd();
				pDlg->OnInitialize();
			}
		}
		else
		{

		}
	}
	break;
	
	case DIO_INPUT_X0_04_EMO:
	{
		if (bOn)
		{

		}
		else
		{

		}
	}
	break;

	case DIO_INPUT_X0_05_MAINAIR:
	{
		if (bOn)
		{

		}
		else
		{

		}
	}
	break;

	case DIO_INPUT_X0_06_AREASENSOR1:
	{
		if (bOn)
		{

		}
		else
		{

		}
	}
	break;

	default:
		break;
	}
}
