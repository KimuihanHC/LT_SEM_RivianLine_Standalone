
#include "pch.h"
#include "SeqFilmPeelOff.h"

#include "IO.h"
#include "Device.h"
#include "Motion.h"
#include "LuriMotion_OQA_SFR_Wide.h"
#include "LuriMotion_OQA_SFR_WideDlg.h"

#if(GET_INSPECTOR == SYS_FILM_PEEL_OFF)
CFilmPeelOff::CFilmPeelOff()
{

}

CFilmPeelOff::~CFilmPeelOff()
{

}

void CFilmPeelOff::Initialize_Startup(__in UINT nIdx, __in int nCapture)
{
	switch (nIdx)
	{
	case 0:
		StartUp_SetStep(SEQ_STEP_DISTORTION_STARTUP_LOAD_START);
		break;

	case 1:
		m_stInspectionInfo.SetSubCaptrueIndex(nCapture);
		StartUp_SetStep(SEQ_STEP_DISTORTION_STARTUP_CAPTURE_START);
		break;

	case 2:
		StartUp_SetStep(SEQ_STEP_DISTORTION_STARTUP_CAL_START);
		break;

	case 3:
		StartUp_SetStep(SEQ_STEP_DISTORTION_STARTUP_UNLOAD_START);
		break;

	default:
		break;
	}

	// UI 업데이트
	Update_Startup_Start(nIdx, nCapture);
}

void CFilmPeelOff::Finalize_Startup(__in enMCErrCode err, __in int nIdx, __in int nCapture)
{
	auto bResultOk = (err == enMCErrCode::MCEC_OK) ? true : false;

	// UI 업데이트
	Update_Startup_End(nIdx, bResultOk);
}

UINT CFilmPeelOff::StartOperation_Startup()
{
	auto err = MCEC_OK;

	switch (StartUp_GetStep())
	{
	case SEQ_STEP_DISTORTION_STARTUP_STANDBY:
		break;

	case SEQ_STEP_DISTORTION_STARTUP_COMPLETE:
		break;

	case SEQ_STEP_DISTORTION_STARTUP_ERROR:
		break;

	case SEQ_STEP_DISTORTION_STARTUP_LOAD_START:
	{
#ifdef DevMode_Simulator

#else 

#endif
		StartUp_SetStep(SEQ_STEP_DISTORTION_STARTUP_LOAD_CAMERA_FIX_ON);
	}
		break;

	case SEQ_STEP_DISTORTION_STARTUP_LOAD_COVER_OPEN_CHECK:				// cover 닫힘 확인 
	{
#ifdef DevMode_Simulator

#else 
		if (CIO::IO_I_SocketCoverOpen())
		{
			err = MCEC_Io_SocketCoverOpen;
			break;
		}
#endif
		StartUp_SetStep(SEQ_STEP_DISTORTION_STARTUP_LOAD_CAMERA_FIX_ON);
	}
	
		break;

	case SEQ_STEP_DISTORTION_STARTUP_LOAD_CAMERA_FIX_ON:					// camera fix on
	{
#ifdef DevMode_Simulator

#else
		CIO::IO_O_Camera_Fix_Cylinder_On(false);
#endif 
		StartUp_SetStep(SEQ_STEP_DISTORTION_STARTUP_LOAD_CAMERA_FIX_ON_CHECK);
	}

		break;

	case SEQ_STEP_DISTORTION_STARTUP_LOAD_CAMERA_FIX_ON_CHECK:
	{
#ifdef DevMode_Simulator

#else
		if (StartUp_GetElapTime() < TIMEOUT_DELAY_1000)
			break;

#endif
		StartUp_SetStep(SEQ_STEP_DISTORTION_STARTUP_LOAD_PROBE_PIN_UP);
	}

		break;

	case SEQ_STEP_DISTORTION_STARTUP_LOAD_PROBE_PIN_UP:					// camera probe pin up
	{
#ifdef DevMode_Simulator

#else
		CIO::IO_O_ProbePin_Up(false);
#endif
		StartUp_SetStep(SEQ_STEP_DISTORTION_STARTUP_LOAD_PROBE_PIN_UP_CHECK);
	}
		break;

	case SEQ_STEP_DISTORTION_STARTUP_LOAD_PROBE_PIN_UP_CHECK:
	{
#ifdef DevMode_Simulator

#else
		if (StartUp_GetElapTime() > TIMEOUT_DELAY_3000)
		{
			err = MCEC_Io_ProbePinUp_Timeout;
			break;
		}
#endif

		StartUp_SetStep(SEQ_STEP_DISTORTION_STARTUP_LOAD_POWER_ON);
	}
		break;

	case SEQ_STEP_DISTORTION_STARTUP_LOAD_POWER_ON:						// power on
	{
#ifdef DevMode_Simulator

#else
		CDevice::PowerOn();
		CDevice::LightOn();
#endif
		StartUp_SetStep(SEQ_STEP_DISTORTION_STARTUP_LOAD_PREVIEW_START);
	}

		break;

	case SEQ_STEP_DISTORTION_STARTUP_LOAD_PREVIEW_START:					// preview start
	{
#ifdef DevMode_Simulator

#else
		if (StartUp_GetElapTime() < TIMEOUT_DELAY_200)
			break;

		Sleep(200);
		CDevice::SEMCO_PrewviewStart();
		m_nPreviewRetry = 0;
#endif

		StartUp_SetStep(SEQ_STEP_DISTORTION_STARTUP_LOAD_PREVIEW_START_ACK_CHECK);
	}
		break;

	case SEQ_STEP_DISTORTION_STARTUP_LOAD_PREVIEW_START_ACK_CHECK:		// preview start check
	{
#ifdef DevMode_Simulator
		StartUp_SetStep(SEQ_STEP_DISTORTION_STARTUP_LOAD_END);
#else
		TESTER_COMM	stTester = *DATAMANAGER->GetCurrentTesterCommData();
		int strWaitTime = _ttoi(stTester.strWaitTimeLIst[1]);
		if (StartUp_GetElapTime() > TIMEOUT_DELAY_30000)
		{
			CDevice::PowerOff();
			StartUp_SetStep(SEQ_STEP_DISTORTION_STARTUP_LOAD_END);
			break;
		}

		if (m_UdpTester[0].m_bTesterFlag_PreviewStart == TRUE)
		{
			if (0 == m_UdpTester[0].m_bTesterFlag_PreviewStartErrorCode)
			{
				StartUp_SetStep(SEQ_STEP_DISTORTION_STARTUP_LOAD_END);
			}
			else if (0 != m_UdpTester[0].m_bTesterFlag_PreviewStartErrorCode && m_nPreviewRetry < 3)		// retry
			{
				m_UdpTester[0].Send_Tester_Communication(eComMode_Insp_PreviewStop);

				CDevice::PowerOff();

				CIO::IO_O_ProbePin_Down();
				CDevice::PowerReset();
				CIO::IO_O_ProbePin_Up();

				m_dwTimer_Preview = GetTickCount();
				m_nPreviewRetry++;

				StartUp_SetStep(SEQ_STEP_DISTORTION_STARTUP_LOAD_RETRY_PREVIEW_START);
			}
			else
			{
				EQ_BASIC_PARAM	stEq = *DATAMANAGER->GetCurrentEqBasicData();
				if (stEq.nLanguage == 0)
					WRITE_SEQ_LOG(_T("카메라 준비신호 실패"));
				else
					WRITE_SEQ_LOG(_T("Camera ready signal failed."));
				COMMON->SetNGCnt();
				m_strMsg.Format(_T("[Preview start failed] NG %02d"), m_UdpTester[0].m_bTesterFlag_PreviewStartErrorCode);
				COMMON->ShowMsg_Thread(m_strMsg, MODE_NO_CLOSE);

				StartUp_SetStep(SEQ_STEP_DISTORTION_STARTUP_LOAD_END);
			}
		}
#endif		
	}
		break;

	case SEQ_STEP_DISTORTION_STARTUP_LOAD_RETRY_PREVIEW_START:			// retry prew start 
	{
#ifdef DevMode_Simulator

#else
		CDevice::PowerOn();

		Sleep(200);

		m_UdpTester[0].Reset_TesterFlag();

		CDevice::SEMCO_PrewviewStart();
		StartUp_SetStep(SEQ_STEP_DISTORTION_STARTUP_LOAD_PREVIEW_START_ACK_CHECK);
#endif

	}
		break;

	case SEQ_STEP_DISTORTION_STARTUP_LOAD_END:			
		StartUp_SetStep(SEQ_STEP_DISTORTION_STARTUP_COMPLETE);
		break;

	case SEQ_STEP_DISTORTION_STARTUP_UNLOAD_START:
	{
#ifdef DevMode_Simulator

#else
		
#endif
		StartUp_SetStep(SEQ_STEP_DISTORTION_STARTUP_UNLOAD_PREVIEW_STOP);
	}
		break;

	case SEQ_STEP_DISTORTION_STARTUP_UNLOAD_PREVIEW_STOP:					// preview stop
	{
#ifdef DevMode_Simulator
		
#else
		m_UdpTester[0].Send_Tester_Communication(eComMode_Insp_PreviewStop);
#endif
		StartUp_SetStep(SEQ_STEP_DISTORTION_STARTUP_UNLOAD_PREVIEW_STOP_ACK_CHECK);
	}
		break;

	case SEQ_STEP_DISTORTION_STARTUP_UNLOAD_PREVIEW_STOP_ACK_CHECK:
	{
#ifdef DevMode_Simulator
		StartUp_SetStep(SEQ_STEP_DISTORTION_STARTUP_UNLOAD_POWER_OFF);
#else
		TESTER_COMM	stTester = *DATAMANAGER->GetCurrentTesterCommData();
		int strWaitTime = _ttoi(stTester.strWaitTimeLIst[4]);

		if (StartUp_GetElapTime() > strWaitTime)
		{
			StartUp_SetStep(SEQ_STEP_DISTORTION_STARTUP_UNLOAD_POWER_OFF);
			break;
		}

		if (m_UdpTester[0].m_bTesterFlag_PreviewStop == TRUE)
		{
			StartUp_SetStep(SEQ_STEP_DISTORTION_STARTUP_UNLOAD_POWER_OFF);
		}
#endif
	}
		break;

	case SEQ_STEP_DISTORTION_STARTUP_UNLOAD_POWER_OFF:						// power off
	{
#ifdef DevMode_Simulator

#else
		CDevice::PowerOff();

		CMotion::MO_MoveY_Load();
#endif
	}
		StartUp_SetStep(SEQ_STEP_DISTORTION_STARTUP_UNLOAD_PROBE_PIN_DN);
		break;

	case SEQ_STEP_DISTORTION_STARTUP_UNLOAD_PROBE_PIN_DN:					// probe dn
	{
#ifdef DevMode_Simulator

#else
		CIO::IO_O_ProbePin_Down(false);
#endif
	}		
		StartUp_SetStep(SEQ_STEP_DISTORTION_STARTUP_UNLOAD_PROBE_PIN_DN_CHECK);
		break;

	case SEQ_STEP_DISTORTION_STARTUP_UNLOAD_PROBE_PIN_DN_CHECK:
	{
#ifdef DevMode_Simulator
		StartUp_SetStep(SEQ_STEP_DISTORTION_STARTUP_UNLOAD_CAMERA_FIX_OFF);
#else
		if (StartUp_GetElapTime() > TIMEOUT_DELAY_3000)
		{
			err = MCEC_Io_ProbePinDn_Timeout;
			break;
		}

		if (CIO::IO_I_ProbePin_Down())
			StartUp_SetStep(SEQ_STEP_DISTORTION_STARTUP_UNLOAD_CAMERA_FIX_OFF);
#endif
	}	
		break;

	case SEQ_STEP_DISTORTION_STARTUP_UNLOAD_CAMERA_FIX_OFF:				// camera fix off
	{
#ifdef DevMode_Simulator
		StartUp_SetStep(SEQ_STEP_DISTORTION_STARTUP_UNLOAD_CAMERA_FIX_OFF_CHECK);
#else
		CIO::IO_O_Camera_Fix_Cylinder_Off(false);
		StartUp_SetStep(SEQ_STEP_DISTORTION_STARTUP_UNLOAD_CAMERA_FIX_OFF_CHECK);
#endif
	}

		break;

	case SEQ_STEP_DISTORTION_STARTUP_UNLOAD_CAMERA_FIX_OFF_CHECK:
	{
#ifdef DevMode_Simulator
		StartUp_SetStep(SEQ_STEP_DISTORTION_STARTUP_UNLOAD_END);
#else
		if (StartUp_GetElapTime() > TIMEOUT_DELAY_3000)
		{
			err = MCEC_Io_CameraFix_Off_Timeout;
			break;
		}

		if (CIO::IO_I_Camera_Fix_Cylinder_Off() && CMotion::MO_CheckY_Load())
		{
			StartUp_SetStep(SEQ_STEP_DISTORTION_STARTUP_UNLOAD_END);				// end
		}
#endif
	}

		break;

	case SEQ_STEP_DISTORTION_STARTUP_UNLOAD_END:
	{
#ifdef DevMode_Simulator
		
#else

#endif
	}
		StartUp_SetStep(SEQ_STEP_DISTORTION_STARTUP_COMPLETE);
		break;

	case SEQ_STEP_DISTORTION_STARTUP_CAPTURE_START:
	{
#ifdef DevMode_Simulator

#else
		m_UdpTester[0].Reset_TesterFlag();
#endif
	}

		StartUp_SetStep(SEQ_STEP_DISTORTION_STARTUP_MOVE_CAPTURE);
		break;

	case SEQ_STEP_DISTORTION_STARTUP_MOVE_CAPTURE:
	{
#ifdef DevMode_Simulator
		StartUp_SetStep(SEQ_STEP_DISTORTION_STARTUP_MOVE_CAPTURE_CHECK);
#else
		err = CMotion::MO_MoveY_Intrinsic(m_stInspectionInfo.GetSubCaptureIndex());

		if (err == MCEC_OK)
			StartUp_SetStep(SEQ_STEP_DISTORTION_STARTUP_MOVE_CAPTURE_CHECK);
#endif
	}		
		break;

	case SEQ_STEP_DISTORTION_STARTUP_MOVE_CAPTURE_CHECK:
	{
#ifdef DevMode_Simulator
		StartUp_SetStep(SEQ_STEP_DISTORTION_STARTUP_CAPTURE_CAPTURE);
#else
		// 모터 이동 완료인지 확인 하자
		if (StartUp_GetElapTime() > TIMEOUT_DELAY_10000)
		{
			err = MCEC_MotionY_MovingTimeout;
			break;
		}

		if (CMotion::MO_CheckY_Intrinsic(m_stInspectionInfo.GetSubCaptureIndex()))
			StartUp_SetStep(SEQ_STEP_DISTORTION_STARTUP_CAPTURE_CAPTURE);
#endif

	}
		break;

	case SEQ_STEP_DISTORTION_STARTUP_CAPTURE_CAPTURE:
	{
#ifdef DevMode_Simulator
	
#else
		m_UdpTester[0].Send_Tester_Communication(eComMode_Insp_Distortion, m_stInspectionInfo.GetSubCaptureIndex());
#endif
	}
		StartUp_SetStep(SEQ_STEP_DISTORTION_STARTUP_CAPTURE_CAPTURE_ACK_CHECK);
		break;

	case SEQ_STEP_DISTORTION_STARTUP_CAPTURE_CAPTURE_ACK_CHECK:
	{
#ifdef DevMode_Simulator
		StartUp_SetStep(SEQ_STEP_DISTORTION_STARTUP_CAPTURE_END);
#else
		if (StartUp_GetElapTime() < TIMEOUT_DELAY_200)
			break;

		TESTER_COMM	stTester = *DATAMANAGER->GetCurrentTesterCommData();
		int strWaitTime = _ttoi(stTester.strWaitTimeLIst[3]);

		int nCapture = m_stInspectionInfo.GetSubCaptureIndex();

		if (StartUp_GetElapTime() > strWaitTime)
		{
			StartUp_SetStep(SEQ_STEP_DISTORTION_STARTUP_CAPTURE_END);
			break;
		}

		if (m_UdpTester[0].m_bTesterFlag_Distortion[nCapture] == TRUE)
		{
			COMMON->m_nMainUiFlag_InspectResult[0] = m_UdpTester[0].m_nTesterFlag_Inspection_DistortionErrorCode[nCapture];

			if (0 != m_UdpTester[0].m_nTesterFlag_Inspection_DistortionErrorCode[nCapture])
			{
				StartUp_SetStep(SEQ_STEP_DISTORTION_STARTUP_CAPTURE_END);
				break;
			}

			StartUp_SetStep(SEQ_STEP_DISTORTION_STARTUP_CAPTURE_END);
		}
#endif
		
	}
		break;

	case SEQ_STEP_DISTORTION_STARTUP_CAPTURE_END:
	{
#ifdef DevMode_Simulator
		
#else

#endif
	}
		StartUp_SetStep(SEQ_STEP_DISTORTION_STARTUP_COMPLETE);
		break;

	case SEQ_STEP_DISTORTION_STARTUP_CAL_START:
	{
#ifdef DevMode_Simulator

#else
		m_UdpTester[0].Reset_TesterFlag();
#endif
	}
		StartUp_SetStep(SEQ_STEP_DISTORTION_STARTUP_CAL_CACULATION);
		break;

	case SEQ_STEP_DISTORTION_STARTUP_CAL_CACULATION:
	{
#ifdef DevMode_Simulator

#else
		m_UdpTester[0].Send_Tester_Communication(eComMode_Insp_Distortion, 0);
#endif
	}
		StartUp_SetStep(SEQ_STEP_DISTORTION_STARTUP_CAL_CACULATION_ACK_CHECK);
		break;

	case SEQ_STEP_DISTORTION_STARTUP_CAL_CACULATION_ACK_CHECK:
	{
#ifdef DevMode_Simulator
		StartUp_SetStep(SEQ_STEP_DISTORTION_STARTUP_CAL_END);
#else
		if (StartUp_GetElapTime() < TIMEOUT_DELAY_200)
			break;

		TESTER_COMM	stTester = *DATAMANAGER->GetCurrentTesterCommData();
		int strWaitTime = _ttoi(stTester.strWaitTimeLIst[3]);

		if (StartUp_GetElapTime() > strWaitTime)
		{
			StartUp_SetStep(SEQ_STEP_DISTORTION_STARTUP_CAL_END);
			break;
		}

		if (m_UdpTester[0].m_bTesterFlag_Distortion[0] == TRUE)
		{
			COMMON->m_nMainUiFlag_InspectResult[0] = m_UdpTester[0].m_nTesterFlag_Inspection_DistortionErrorCode[0];

			if (0 != m_UdpTester[0].m_nTesterFlag_Inspection_DistortionErrorCode[0])
			{
				StartUp_SetStep(SEQ_STEP_DISTORTION_STARTUP_CAL_END);
				break;

			}

			StartUp_SetStep(SEQ_STEP_DISTORTION_STARTUP_CAL_END);
		}
#endif		
	}		
		break;

	case SEQ_STEP_DISTORTION_STARTUP_CAL_END:
		StartUp_SetStep(SEQ_STEP_DISTORTION_STARTUP_COMPLETE);
		break;

	default:
		break;
	}

	return (UINT)err;
}

UINT CFilmPeelOff::StartOperation_Run()
{
	TESTER_COMM		stTester		= *DATAMANAGER->GetCurrentTesterCommData();
	TEACHING_PARAM	stTeaching		= *DATAMANAGER->GetCurrentTeachingData();
	MOTION_PARAM	stMotion		= *DATAMANAGER->GetCurrentMotionParam();
	EQ_BASIC_PARAM	stEq			= *DATAMANAGER->GetCurrentEqBasicData();

	auto err = MCEC_OK;

	switch (GetStep())
	{
	case SEQ_STEP_DISTORTION_READY:
	{
		ERROR_LIST	stErr = *DATAMANAGER->GetCurrentErrorList();
		EQ_BASIC_PARAM	stEq = *DATAMANAGER->GetCurrentEqBasicData();

		m_stInspectionInfo.Reset();

		// 정상적으로 루틴을 한번 탔으면 정지 하자.
		if (SEQUENCE->m_bFlagProcess_NormalFinish == TRUE)
		{
			SEQUENCE->SetStopswitchMode(FALSE);
			SEQUENCE->Clear_MainUiFlag();
			COMMON->SetAreaStop_OkPush(FALSE);
			COMMON->SetAccpet_ReciveBarcode(TRUE);

			m_UdpTester[0].Reset_TesterFlag();
			SEQUENCE->AutoMode_Run();

			SEQUENCE->m_bFlagProcess_NormalFinish = FALSE;
			break;
		}

		// AREA 센서 감지 되었을경우 
		if (m_bFlagProcess_AreaStopFinish == TRUE)
		{
			Clear_MainUiFlag();
			COMMON->SetAccpet_ReciveBarcode(TRUE);
			m_UdpTester[0].Reset_TesterFlag();

			AJIN_IO->BlinkStop(DIO_OUTPUT_Y0_01_START_SWITCH_LAMP, TRUE);
			AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_01_START_SWITCH_LAMP, FALSE);
			AJIN_IO->BlinkStop(DIO_OUTPUT_Y0_02_STOP_SWITCH_LAMP, TRUE);
			AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_02_STOP_SWITCH_LAMP, FALSE);

			COMMON->SetForced_SideBtn_StopOn(TRUE);

			if (COMMON->GetAreaStop_CancelPush() == TRUE)
			{
				SetAreaStopLampnSideFlag(TRUE);
			}
			COMMON->SetAreaStop_CancelPush(FALSE);

			m_bFlagProcess_AreaStopFinish = FALSE;

			Stop_Run_nonNormal();
			break;
		}

#ifndef _DRY_RUN_MODULE_NONE
		// Ready를 받아야 동작한다.
		if (m_UdpTester[0].m_bTesterFlag_Ready != TRUE)
			break;
		else
		{
//			COMMON->m_bMainUiFlag_StepView[0] = TRUE;
//			COMMON->m_bMainUiFlag_StepView[2] = TRUE; 
		}

		// 제품 안착이 안되었으면 시작 안함 
		if (CIO::IO_I_DetectCamera() == false)
			break;

		// 커버 안닫혔을 경우 시작 안함 
		if (CIO::IO_I_SocketCoverOpen())
			break;

		CDevice::LightOn();

#else
		COMMON->m_bMainUiFlag_StepView[0] = TRUE;
		COMMON->m_bMainUiFlag_StepView[2] = TRUE;
#endif

		if (COMMON->GetInterlock_SocketClose() == FALSE)
			break;
		else
			COMMON->m_bMainUiFlag_StepView[1] = TRUE;

		// 소켓 닫았는지 확인 

#ifndef _DRY_RUN_MODULE_NONE
		if (m_UdpTester[0].m_bConnected != TRUE)
		{
			break;
		}

		SetStep(SEQ_STEP_DISTORTION_INTERLOCK_CHECK);
#else
		SetStep(SEQ_STEP_DISTORTION_INTERLOCK_CHECK);
#endif
	}
	break;
	case SEQ_STEP_DISTORTION_INTERLOCK_CHECK:
	{
		if (COMMON->GetInterlock_CameraDetected() == FALSE || COMMON->GetInterlock_SocketClose() == FALSE)
		{
			Error();
			err = MCEC_Io_SocketCoverOpen;
			break;
		}

		SEQUENCE->SetStopswitchMode(FALSE);
		COMMON->SetMainUiFlag_CurrentPos(0);

		SetStep(SEQ_STEP_DISTORTION_CAMERA_FIX_CYLINDER_ON);
	}
	break;

	case SEQ_STEP_DISTORTION_CAMERA_FIX_CYLINDER_ON:
	{
		CIO::IO_O_Camera_Fix_Cylinder_On(false);
		SetStep(SEQ_STEP_DISTORTION_CAMERA_FIX_CYLINDER_ON_CHK);
	}
	break;
	case SEQ_STEP_DISTORTION_CAMERA_FIX_CYLINDER_ON_CHK:
	{
		if (GetElapTime() < TIMEOUT_DELAY_1000)
			break;
		
		SetStep(SEQ_STEP_DISTORTION_PROBE_PIN_UP);
	}
	break;
	case SEQ_STEP_DISTORTION_PROBE_PIN_UP:
	{
		CIO::IO_O_ProbePin_Up(false);
		SetStep(SEQ_STEP_DISTORTION_PROBE_PIN_UP_CHK);
	}
	break;
	case SEQ_STEP_DISTORTION_PROBE_PIN_UP_CHK:
	{
		if (CIO::IO_I_ProbePin_Up() == true)
		{
			COMMON->m_bMainUiFlag_StepView[3] = TRUE;

			CDevice::LightOn();

			CDevice::PowerOn();

			COMMON->m_bMainUiFlag_StepView[4] = TRUE;
			m_nPreviewRetry = 1;

			SetStep(SEQ_STEP_DISTORTION_INSPECTION_PREVIEW_START);
		}
		else if (GetElapTime() > TIMEOUT_SENSOR_5000)
		{
			err = MCEC_Io_ProbePinUp_Timeout;
			break;
		}
	}
	break;

	case SEQ_STEP_DISTORTION_INSPECTION_PREVIEW_START:
	{
		if (GetElapTime() < TIMEOUT_DELAY_200)
			break;

		Sleep(200);

		m_UdpTester[0].Reset_TesterFlag();
		m_UdpTester[0].Send_Tester_Communication(eComMode_PreviewStart);//START PREVIEW 1		

		m_dwTimer_Preview = GetTickCount();

		SetPowerVoltage();

		SetStep(SEQ_STEP_DISTORTION_INSPECTION_MOVE_CAPTURE_INIT_POSITION);
	}
	break;

	case SEQ_STEP_DISTORTION_INSPECTION_MOVE_CAPTURE_INIT_POSITION:		// 230707
	{
		err = CMotion::MO_MoveY_Intrinsic(m_stInspectionInfo.GetCaptureIndex());

		if(err == MCEC_OK)
			SetStep(SEQ_STEP_DISTORTION_INSPECTION_PREVIEW_START_CHK);
	}
		break;

	case SEQ_STEP_DISTORTION_INSPECTION_PREVIEW_START_CHK:
	{
		TESTER_COMM	stTester = *DATAMANAGER->GetCurrentTesterCommData();
		int strWaitTime = _ttoi(stTester.strWaitTimeLIst[1]);
		if (GetElapTime() > strWaitTime)
		{
			// ng code		
			SetStep(SEQ_STEP_DISTORTION_INSPECTION_MOVE_UNLOAD);
			break;
		}

		// [22.0224.1] 리트라이
		if (m_UdpTester[0].m_bTesterFlag_PreviewStart == TRUE)
		{
			if (0 == m_UdpTester[0].m_bTesterFlag_PreviewStartErrorCode)
			{
				// 전압체크
				int nRetCode = 0;

				nRetCode = (int)CheckPowerVoltage();

				if(nRetCode == 0)
					SetStep(SEQ_STEP_DISTORTION_INSPECTION_MOVE_CAPTURE_INIT_POSITION_CHK);
				else
				{
					EQ_BASIC_PARAM	stEq = *DATAMANAGER->GetCurrentEqBasicData();
					if (stEq.nLanguage == 0)
						WRITE_SEQ_LOG(_T("전압 셋팅 실패"));
					else
						WRITE_SEQ_LOG(_T("Power Voltage Set failed."));

					m_strMsg = _T("[Power Voltage failed]");
					COMMON->ShowMsg_Thread(m_strMsg, MODE_NO_CLOSE);
					SetStep(SEQ_STEP_DISTORTION_INSPECTION_MOVE_UNLOAD);
				}
			}
			else if (0 != m_UdpTester[0].m_bTesterFlag_PreviewStartErrorCode && m_nPreviewRetry < 3)		// retry
			{
#ifndef _DRY_RUN_MODULE_NONE
				m_UdpTester[0].Send_Tester_Communication(eComMode_Insp_PreviewStop);
				//COMMON->SetProbePinCylinder_UpDown(_CYLINDER_DOWN);
				Sleep(500);
				//COMMON->SetProbePinCylinder_UpDown(_CYLINDER_UP);

				CDevice::PowerReset();

				m_dwTimer_Preview = GetTickCount();
				m_nPreviewRetry++;

				SetStep(SEQ_STEP_DISTORTION_INSPECTION_PREVIEW_START);
#else
				SetStep(SEQ_STEP_DISTORTION_INSPECTION_MOVE_CAPTURE_INIT_POSITION_CHK);
#endif
			}
			else
			{
				EQ_BASIC_PARAM	stEq = *DATAMANAGER->GetCurrentEqBasicData();
				if (stEq.nLanguage == 0)
					WRITE_SEQ_LOG(_T("카메라 준비신호 실패"));
				else
					WRITE_SEQ_LOG(_T("Camera ready signal failed."));
				COMMON->SetNGCnt();
				m_strMsg.Format(_T("[Preview start failed] NG %02d"), m_UdpTester[0].m_bTesterFlag_PreviewStartErrorCode);
				COMMON->ShowMsg_Thread(m_strMsg, MODE_NO_CLOSE);

				SetStep(SEQ_STEP_DISTORTION_INSPECTION_MOVE_UNLOAD);
			}
		}
	}
	break;

	case SEQ_STEP_DISTORTION_INSPECTION_MOVE_CAPTURE_INIT_POSITION_CHK:		// 0707
	{
		// 모터 이동 완료인지 확인 하자
		if (GetElapTime() > TIMEOUT_DELAY_10000)
		{
			err = MCEC_MotionY_MovingTimeout;
			break;
		}

		if (CMotion::MO_CheckY_Intrinsic(m_stInspectionInfo.GetCaptureIndex()))
			SetStep(SEQ_STEP_DISTORTION_INSEPCTION_CORNERS);
	}
	break;

	case SEQ_STEP_DISTORTION_INSEPCTION_CORNERS:
	{
		m_UdpTester[0].Send_Tester_Communication(eComMode_Insp_Distortion, m_stInspectionInfo.GetCaptureIndex());

		SetStep(SEQ_STEP_DISTORTION_INSEPCTION_CORNERS_WAIT_RECIVE);
	}
	break;
	case SEQ_STEP_DISTORTION_INSEPCTION_CORNERS_WAIT_RECIVE:
	{
		if (GetElapTime() < TIMEOUT_DELAY_200)
			break;

		TESTER_COMM	stTester = *DATAMANAGER->GetCurrentTesterCommData();
		int strWaitTime = _ttoi(stTester.strWaitTimeLIst[3]);

		if (GetElapTime() > TIMEOUT_DELAY_5000)
		{
			// ng code
			SetStep(SEQ_STEP_DISTORTION_INSPECTION_MOVE_UNLOAD);
			break;
		}

#ifndef _DRY_RUN_MODULE_NONE
		if (m_UdpTester[0].m_bTesterFlag_Distortion[m_stInspectionInfo.GetCaptureIndex()] == TRUE)
		{
			COMMON->m_nMainUiFlag_InspectResult[0] = m_UdpTester[0].m_nTesterFlag_Inspection_DistortionErrorCode[m_stInspectionInfo.GetCaptureIndex()];

			if (0 != m_UdpTester[0].m_nTesterFlag_Inspection_DistortionErrorCode[m_stInspectionInfo.GetCaptureIndex()])
			{
				EQ_BASIC_PARAM	stEq = *DATAMANAGER->GetCurrentEqBasicData();
				if (stEq.nLanguage == 0)
					WRITE_SEQ_LOG(_T("Inspection Corners 검사실패"));
				else
					WRITE_SEQ_LOG(_T("Inspection Corners test failed."));

				//Main Err부분 추가 되면 UI변경
				COMMON->SetProbePinCylinder_UpDown(_CYLINDER_DOWN);
				COMMON->SetNGCnt();
				m_strMsg.Format(_T("NG %02d"), m_UdpTester[0].m_nTesterFlag_Inspection_DistortionErrorCode[m_stInspectionInfo.GetCaptureIndex()]);
				COMMON->ShowMsg_Thread(m_strMsg, MODE_NO_CLOSE, FALSE);

				SetStep(SEQ_STEP_DISTORTION_INSPECTION_MOVE_UNLOAD);
				break;
			}

			m_stInspectionInfo.CaptureNext();
			if (m_stInspectionInfo.GetCaptureIndex() > 5)
				SetStep(SEQ_STEP_DISTORTION_INSEPCTION_INTRINSIC_CAL);
			else
				SetStep(SEQ_STEP_DISTORTION_INSPECTION_MOVE_CAPTURE_POSITION);
		}

#else 
		//		SetStep(SEQ_STEP_DISTORTION_INSPECTION_PREVIEW_STOP);

		m_stInspectionInfo.CaptureNext();
		if (m_stInspectionInfo.GetCaptureIndex() > 5)
			SetStep(SEQ_STEP_DISTORTION_INSEPCTION_INTRINSIC_CAL);
		else
			SetStep(SEQ_STEP_DISTORTION_INSPECTION_MOVE_CAPTURE_POSITION);
#endif
	}
	break;

	case SEQ_STEP_DISTORTION_INSPECTION_MOVE_CAPTURE_POSITION:		// 0707
	{
#ifndef _DRY_RUN_MODULE_NONE
//		if (m_UdpTester[0].m_bTesterFlag_Distortion[1] == TRUE)
//		{
//			COMMON->m_nMainUiFlag_InspectResult[0] = m_UdpTester[0].m_nTesterFlag_Inspection_DistortionErrorCode[1];
//
//			if (0 != m_UdpTester[0].m_nTesterFlag_Inspection_DistortionErrorCode[1])
//			{
//				EQ_BASIC_PARAM	stEq = *DATAMANAGER->GetCurrentEqBasicData();
//				if (stEq.nLanguage == 0)
//					WRITE_SEQ_LOG(_T("Inspection Corners 검사실패"));
//				else
//					WRITE_SEQ_LOG(_T("Inspection Corners test failed."));
//
//				//Main Err부분 추가 되면 UI변경
//				COMMON->SetProbePinCylinder_UpDown(_CYLINDER_DOWN);
//				COMMON->SetNGCnt();
//				m_strMsg.Format(_T("NG %02d"), m_UdpTester[0].m_nTesterFlag_Inspection_DistortionErrorCode[1]);
//				COMMON->ShowMsg_Thread(m_strMsg, MODE_NO_CLOSE, FALSE);
//
//				SetStep(SEQ_STEP_DISTORTION_INSPECTION_PREVIEW_STOP);
//				break;
//			}
//
////			SetStep(SEQ_STEP_DISTORTION_INSEPCTION_INTRINSIC_CAL);
//			SetStep(SEQ_STEP_DISTORTION_INSPECTION_MOVE_CAPTURE_POSITION_CHK);
//		}
		err = CMotion::MO_MoveY_Intrinsic(m_stInspectionInfo.GetCaptureIndex());

		if (err == MCEC_OK)
			SetStep(SEQ_STEP_DISTORTION_INSPECTION_MOVE_CAPTURE_POSITION_CHK);
#else 
		err = CMotion::MO_MoveY_Intrinsic(m_stInspectionInfo.GetCaptureIndex());

		if (err == MCEC_OK)
			SetStep(SEQ_STEP_DISTORTION_INSPECTION_MOVE_CAPTURE_POSITION_CHK);
#endif
	}
	break;

	case SEQ_STEP_DISTORTION_INSPECTION_MOVE_CAPTURE_POSITION_CHK:		// 0707
	{
		if (GetElapTime() > TIMEOUT_DELAY_10000)
		{
			err = MCEC_MotionY_MovingTimeout;
			break;
		}

		if (CMotion::MO_CheckY_Intrinsic(m_stInspectionInfo.GetCaptureIndex()))
			SetStep(SEQ_STEP_DISTORTION_INSEPCTION_CORNERS);
	}
	break;

	case SEQ_STEP_DISTORTION_INSEPCTION_INTRINSIC_CAL:
	{
		m_UdpTester[0].Send_Tester_Communication(eComMode_Insp_Distortion, 0);
		SetStep(SEQ_STEP_DISTORTION_INSPECTION_MOVE_UNLOAD);
	}
	break;

	case SEQ_STEP_DISTORTION_INSPECTION_MOVE_UNLOAD:			// 0707
	{
		if (GetElapTime() < TIMEOUT_DELAY_200)
			break;

		err = CMotion::MO_MoveY_Load();
		if (err == MCEC_OK)
		{
			if (m_stInspectionInfo.GetCaptureIndex() == 6)
				SetStep(SEQ_STEP_DISTORTION_INSEPCTION_INTRINSIC_CAL_WAIT_RECIVE);
			else
				SetStep(SEQ_STEP_DISTORTION_INSPECTION_PREVIEW_STOP);
		}
	}
	break;

	case SEQ_STEP_DISTORTION_INSEPCTION_INTRINSIC_CAL_WAIT_RECIVE:
	{
#ifndef _DRY_RUN_MODULE_NONE
		if (GetElapTime() < TIMEOUT_DELAY_200)
			break;

		TESTER_COMM	stTester = *DATAMANAGER->GetCurrentTesterCommData();
		int strWaitTime = _ttoi(stTester.strWaitTimeLIst[3]);

		if (GetElapTime() > TIMEOUT_DELAY_30000)
		{
			// ng code
			SetStep(SEQ_STEP_DISTORTION_INSPECTION_PREVIEW_STOP);
			break;
		}

		if (m_UdpTester[0].m_bTesterFlag_Distortion[0] == TRUE)
		{
			COMMON->m_bMainUiFlag_StepView[7] = TRUE;
			COMMON->m_nMainUiFlag_InspectResult[0] = m_UdpTester[0].m_nTesterFlag_Inspection_DistortionErrorCode[0];

			if (0 != m_UdpTester[0].m_nTesterFlag_Inspection_DistortionErrorCode[0])
			{
				WRITE_SEQ_LOG(_T("Intrinsic Cal. test failed."));

				COMMON->SetProbePinCylinder_UpDown(_CYLINDER_DOWN);
				COMMON->SetNGCnt();
				m_strMsg.Format(_T("NG %02d"), m_UdpTester[0].m_nTesterFlag_Inspection_DistortionErrorCode[0]);
				COMMON->ShowMsg_Thread(m_strMsg, MODE_NO_CLOSE, FALSE);

				SetStep(SEQ_STEP_DISTORTION_INSPECTION_PREVIEW_STOP);
				break;
			}
			m_strMsg.Format(_T("PASS"));
			SetStep(SEQ_STEP_DISTORTION_INSPECTION_PREVIEW_STOP);
			COMMON->ShowMsg_Thread(m_strMsg, MODE_NO_CLOSE);
		}
#else
		SetStep(SEQ_STEP_DISTORTION_INSPECTION_PREVIEW_STOP);
#endif
//
//#ifndef _DRY_RUN_MODULE_NONE
//			m_strMsg.Format(_T("PASS"));
//			COMMON->ShowMsg_Thread(m_strMsg, MODE_NO_CLOSE);
//#endif
//			SetStep(SEQ_STEP_DISTORTION_INSPECTION_PREVIEW_STOP);
//		}
	}
	break;

	///////////////////////////////////////////////////////////////
	/*종료시퀀스*/

	case SEQ_STEP_DISTORTION_INSPECTION_PREVIEW_STOP:
	{
		m_UdpTester[0].Send_Tester_Communication(eComMode_Insp_PreviewStop);

		SetStep(SEQ_STEP_DISTORTION_INSPECTION_PREVIEW_STOP_CHK);
	}
	break;

	case SEQ_STEP_DISTORTION_INSPECTION_PREVIEW_STOP_CHK:
	{
		TESTER_COMM	stTester = *DATAMANAGER->GetCurrentTesterCommData();
		int strWaitTime = _ttoi(stTester.strWaitTimeLIst[4]);

		if (GetElapTime() > strWaitTime)
		{
			// ng code			
			SetStep(SEQ_STEP_DISTORTION_INSPECTION_PREVIEW_STOP);
			break;
		}

		if (m_UdpTester[0].m_bTesterFlag_PreviewStop == TRUE)
		{
			COMMON->m_bMainUiFlag_StepView[8] = TRUE;

#ifndef _DRY_RUN_MODULE_NONE
			if (0 != m_UdpTester[0].m_bTesterFlag_PreviewStopErrorCode)
			{
				WRITE_SEQ_LOG(_T("Falied camera off"));
			}

			CDevice::PowerOff();

			SetStep(SEQ_STEP_DISTORTION_PROBE_PIN_DOWN);
#else
		//	if (m_ctrlLight_ODA_Q[0].Write_OFF() == TRUE)
			if (CDevice::PowerOff() == TRUE)
				COMMON->m_bMainUiFlag_StepView[9] = TRUE;

			SetStep(SEQ_STEP_DISTORTION_PROBE_PIN_DOWN);
#endif
		}
	}
	break;

	case SEQ_STEP_DISTORTION_PROBE_PIN_DOWN:
	{
		CIO::IO_O_ProbePin_Down(false);

		SetStep(SEQ_STEP_DISTORTION_PROBE_PIN_DOWN_CHK);
	}
	break;

	case SEQ_STEP_DISTORTION_PROBE_PIN_DOWN_CHK:
	{
		if (CIO::IO_I_ProbePin_Down() == true)
		{
			SetStep(SEQ_STEP_DISTORTION_MOVE_CAMERA_FIX_CYLINDER_OFF);
		}
		else if (GetElapTime() > 1000)
		{
			err = MCEC_Io_ProbePinDn_Timeout;
		}
	}
	break;

	case SEQ_STEP_DISTORTION_MOVE_CAMERA_FIX_CYLINDER_OFF:
	{
		CIO::IO_O_Camera_Fix_Cylinder_Off(false);

		SetStep(SEQ_STEP_DISTORTION_MOVE_CAMERA_FIX_CYLINDER_OFF_CHK);
	}
	break;

	case SEQ_STEP_DISTORTION_MOVE_CAMERA_FIX_CYLINDER_OFF_CHK:
	{
		if (CIO::IO_I_Camera_Fix_Cylinder_Off() == true)
		{
			AJIN_IO->ChangeBtnStatus(DIO_OUTPUT_Y0_02_STOP_SWITCH_LAMP, 0, TRUE);

			SetStep(SEQ_STEP_DISTORTION_INSPECTION_MOVE_UNLOAD_CHK);
		}
		else if (GetElapTime() > 1000)
		{
			err = MCEC_Io_CameraFix_Off_Timeout;
		}
	}
	break;


	case SEQ_STEP_DISTORTION_INSPECTION_MOVE_UNLOAD_CHK:			// 0707
	{
		if (GetElapTime() < TIMEOUT_DELAY_200)
			break;

		if (GetElapTime() > TIMEOUT_DELAY_10000)
		{
			err = MCEC_MotionY_MovingTimeout;
			break;
		}

		if (CMotion::MO_CheckY_Load())
		{
			AJIN_IO->ChangeBtnStatus(DIO_OUTPUT_Y0_02_STOP_SWITCH_LAMP, 0, TRUE);

			SetStep(SEQ_STEP_DISTORTION_WAIT_STOP_SWITCH_PUSH);
		}
	}
	break;

	case SEQ_STEP_DISTORTION_WAIT_STOP_SWITCH_PUSH:
	{
#ifndef _DRY_RUN_MODULE_NONE
		if (SEQUENCE->GetStopSwtichMode() == TRUE || COMMON->GetAreaStop_CancelPush() == TRUE)
		{
			COMMON->SetReplaceFlag(TRUE);

			AJIN_IO->BlinkStop(DIO_OUTPUT_Y0_01_START_SWITCH_LAMP, TRUE);
			AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_01_START_SWITCH_LAMP, FALSE);
			AJIN_IO->BlinkStop(DIO_OUTPUT_Y0_02_STOP_SWITCH_LAMP, TRUE);
			AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_02_STOP_SWITCH_LAMP, FALSE);

			SetStep(SEQ_STEP_DISTORTION_CYCLE_COMPLEATE);
		}
#else
		COMMON->SetReplaceFlag(TRUE);

		AJIN_IO->BlinkStop(DIO_OUTPUT_Y0_01_START_SWITCH_LAMP, TRUE);
		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_01_START_SWITCH_LAMP, FALSE);
		AJIN_IO->BlinkStop(DIO_OUTPUT_Y0_02_STOP_SWITCH_LAMP, TRUE);
		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_02_STOP_SWITCH_LAMP, FALSE);

		SetStep(SEQ_STEP_DISTORTION_CYCLE_COMPLEATE);
#endif		
	}
	break;

	case SEQ_STEP_DISTORTION_SEND_INSPECTION_WAIT_STOP_SWITCH_SIGNAL:
	{
#ifndef _DRY_RUN_MODULE_NONE
		if (SEQUENCE->GetStopSwtichMode() == TRUE)
		{
			SetStep(SEQ_STEP_DISTORTION_CYCLE_COMPLEATE);
		}
#else
		SetStep(SEQ_STEP_DISTORTION_CYCLE_COMPLEATE);
#endif
	}
	break;

	case SEQ_STEP_DISTORTION_CYCLE_COMPLEATE:
	{
#ifndef _DRY_RUN_MODULE_NONE
		if (COMMON->GetAreaStop_CancelPush() == TRUE)
			SEQUENCE->m_bFlagProcess_AreaStopFinish = TRUE;
		else
			SEQUENCE->m_bFlagProcess_NormalFinish = TRUE;

		COMMON->SetProbePinCylinder_UpDown(_CYLINDER_DOWN);

		SetStep(SEQ_STEP_DISTORTION_READY);
		m_UdpTester[0].Reset_ReadyFlag();
		m_stInspectionInfo.Reset();
#else
		COMMON->SetProbePinCylinder_UpDown(_CYLINDER_DOWN);
		SEQUENCE->m_bFlagProcess_NormalFinish = TRUE;

		SetStep(SEQ_STEP_DISTORTION_DRYRUN_MODE);
		m_stInspectionInfo.Reset();
#endif
	}
	break;
	case SEQ_STEP_DISTORTION_DRYRUN_MODE:
	{
		if (GetElapTime() < TIMEOUT_DELAY_500)
			break;

		SetStep(SEQ_STEP_DISTORTION_READY);
		break;
	}
	}
	return (UINT)err;
}

UINT CFilmPeelOff::Maintence(__in enMaintFunId id)
{
	auto err = 0;
	switch (id)
	{
	case enMaintFunId::Load:
		// fix on
		if (CIO::IO_O_Camera_Fix_Cylinder_On() == false)
		{
			err = MCEC_Io_CameraFix_On_Timeout;
			break;
		}

		// probe pin on 
		if (CIO::IO_O_ProbePin_Up() == false)
		{
			err = MCEC_Io_ProbePinUp_Timeout;
			break;
		}
		break;

	case enMaintFunId::Unload:
		// fix off
		if (CIO::IO_O_Camera_Fix_Cylinder_Off() == false)
		{
			err = MCEC_Io_CameraFix_Off_Timeout;
			break;
		}
		// probe pin off
		if (CIO::IO_O_ProbePin_Down() == false)
		{
			err = MCEC_Io_ProbePinDn_Timeout;
			break;
		}
		break;

	default:
		break;
	}

	return err;
}

void CFilmPeelOff::SetInspectionStep()
{
	SetStep(SEQ_STEP_DISTORTION_INSPECTION_PREVIEW_STOP);
}

void CFilmPeelOff::ResetStepFlag()
{
	/*if (COMMON->GetFlagStartUpMode() == TRUE)
		SetStep(SEQ_STEP_DISTORTION_STARTUPMODE_ACTION_WAIT);
	else*/
	SetStep(SEQ_STEP_DISTORTION_READY);

	m_bFlagProcess_NormalFinish = FALSE;
}

void CFilmPeelOff::RestartStep()
{
	switch (GetStep())
	{
	case SEQ_STEP_DISTORTION_READY:
	case SEQ_STEP_DISTORTION_INTERLOCK_CHECK:
		SetStep(SEQ_STEP_DISTORTION_READY);
		break;

	case SEQ_STEP_DISTORTION_CAMERA_FIX_CYLINDER_ON:
	case SEQ_STEP_DISTORTION_CAMERA_FIX_CYLINDER_ON_CHK:
		SetStep(SEQ_STEP_DISTORTION_CAMERA_FIX_CYLINDER_ON);
		break;

	case SEQ_STEP_DISTORTION_PROBE_PIN_UP:
	case SEQ_STEP_DISTORTION_PROBE_PIN_UP_CHK:
		SetStep(SEQ_STEP_DISTORTION_PROBE_PIN_UP);
		break;

	case SEQ_STEP_DISTORTION_INSPECTION_PREVIEW_START:
	case SEQ_STEP_DISTORTION_INSPECTION_PREVIEW_START_CHK:
		SetStep(SEQ_STEP_DISTORTION_INSPECTION_PREVIEW_START);
		break;

	case SEQ_STEP_DISTORTION_INSEPCTION_CORNERS:
	case SEQ_STEP_DISTORTION_INSEPCTION_CORNERS_WAIT_RECIVE:
	case SEQ_STEP_DISTORTION_INSEPCTION_INTRINSIC_CAL:
	case SEQ_STEP_DISTORTION_INSEPCTION_INTRINSIC_CAL_WAIT_RECIVE:
		SetStep(SEQ_STEP_DISTORTION_INSEPCTION_CORNERS);
		break;

	case SEQ_STEP_DISTORTION_INSPECTION_PREVIEW_STOP:
	case SEQ_STEP_DISTORTION_INSPECTION_PREVIEW_STOP_CHK:
		SetStep(SEQ_STEP_DISTORTION_INSPECTION_PREVIEW_STOP);
		break;

	case SEQ_STEP_DISTORTION_PROBE_PIN_DOWN:
	case SEQ_STEP_DISTORTION_PROBE_PIN_DOWN_CHK:
		SetStep(SEQ_STEP_DISTORTION_PROBE_PIN_DOWN);
		break;

	case SEQ_STEP_DISTORTION_MOVE_CAMERA_FIX_CYLINDER_OFF:
	case SEQ_STEP_DISTORTION_MOVE_CAMERA_FIX_CYLINDER_OFF_CHK:
		SetStep(SEQ_STEP_DISTORTION_MOVE_CAMERA_FIX_CYLINDER_OFF);
		break;

	default:
		break;
	}
}

bool CFilmPeelOff::ReadyToStart()
{
	auto bRet = false;
	bRet = CIO::IO_I_DetectCamera() && CIO::IO_I_SocketCoverOpen() == false;
	return bRet;
}

bool CFilmPeelOff::IsStep_Ready()
{
	return GetStep() == SEQ_STEP_DISTORTION_READY;
}

bool CFilmPeelOff::IsStep_WaitForPushSwitch()
{
	return GetStep() == SEQ_STEP_DISTORTION_WAIT_STOP_SWITCH_PUSH;
}

bool CFilmPeelOff::IsStep_UnderMoveY()
{
	return GetStep() < SEQ_STEP_DISTORTION_INSPECTION_PREVIEW_START;
}

bool CFilmPeelOff::IsStep_BeforePushStopSwitch()
{
	return GetStep() < SEQ_STEP_DISTORTION_WAIT_STOP_SWITCH_PUSH;
}

bool CFilmPeelOff::IsStep_NotStartupModeWait()
{
	return GetStep() != SEQ_STEP_DISTORTION_STARTUPMODE_ACTION_WAIT;
}

enMCErrCode CFilmPeelOff::CheckPowerVoltage()
{
	auto err = MCEC_OK;

	TESTER_COMM stTester = *DATAMANAGER->GetCurrentTesterCommData();

	auto volt = stTester.fVolt;

	float readvolt = GetPowerVoltage();

	CString strLog;
	strLog.Format(_T("Power Supply Measure Volt: %f"), readvolt);
	WRITE_SEQ_LOG(strLog);

	if (abs(volt - readvolt) > 0.5)
		err = MCEC_SetPowerVoltage_Fail;

	return err;
}

void CFilmPeelOff::SetPowerVoltage()
{
	float fVolt = 0.0;
	m_stInspectionInfo.fPowerVoltage = 0.0f;

	Sleep(1500);
	CDevice::POWERSUPPLY_MeasureVolt(fVolt);

	// 테스트
//	fVolt = 8.4;
	m_stInspectionInfo.SetPowerVoltage(fVolt);
};

float CFilmPeelOff::GetPowerVoltage()
{
	return m_stInspectionInfo.GetPowerVoltage();
};
#endif