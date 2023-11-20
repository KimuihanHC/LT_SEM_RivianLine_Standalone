#include "pch.h"
#include "SeqOqcMulti.h"

#include "IO.h"
#include "Motion.h"
#include "Device.h"

#include "LuriMotion_OQA_SFR_Wide.h"
#include "LuriMotion_OQA_SFR_WideDlg.h"

#if(GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL)

CSeqOqcMulti::CSeqOqcMulti()
{

}

CSeqOqcMulti::~CSeqOqcMulti()
{

}

void CSeqOqcMulti::Initialize_Startup(__in UINT nIdx, __in int nCapture)
{
	switch (nIdx)
	{
	case 0:
		StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_LOAD_START);
		break;

	case 1:
		StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_BLEMISH_START);
		break;

	case 2:
		StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_SFR_START);
		break;

	case 3:
		StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_START);
		break;

	default:
		break;
	}

	// UI 업데이트
	Update_Startup_Start(nIdx);
}

void CSeqOqcMulti::Finalize_Startup(__in enMCErrCode err, __in int nIdx, __in int nCapture)
{
	auto bResultOk = (err == enMCErrCode::MCEC_OK) ? true : false;

	// UI 업데이트
	Update_Startup_End(nIdx, bResultOk);
}

UINT CSeqOqcMulti::StartOperation_Startup()
{
	auto err = MCEC_OK;

	switch (StartUp_GetStep())
	{
	case SEQ_STEP_OQC_MULTI_STARTUP_STANDBY:
		break;
	case SEQ_STEP_OQC_MULTI_STARTUP_COMPLETE:
		break;
	case SEQ_STEP_OQC_MULTI_STARTUP_ERROR:
		break;
			// 0
	case SEQ_STEP_OQC_MULTI_STARTUP_LOAD_START:		// camera detect
		StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_LOAD_CAMERA_DETECT_CHECK);
		break;
	case SEQ_STEP_OQC_MULTI_STARTUP_LOAD_CAMERA_DETECT_CHECK:		// camera detect
		if (CIO::IO_I_DetectCamera() == false)
		{
			err = MCEC_Io_CameraNotDetect;
			break;
		}

		if (CIO::IO_I_SocketCoverOpen())
		{
			err = MCEC_Io_SocketCoverOpen;
			break;
		}

		StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_LOAD_CAMERA_FIX_OFF);
		break;

	case SEQ_STEP_OQC_MULTI_STARTUP_LOAD_CAMERA_FIX_OFF:			// camera fix off
		CIO::IO_O_Camera_Fix_Cylinder_Off(false);
		StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_LOAD_CAMERA_FIX_OFF_CHECK);
		break;

	case SEQ_STEP_OQC_MULTI_STARTUP_LOAD_CAMERA_FIX_OFF_CHECK:	// camera fix off check
		if (StartUp_GetElapTime() > TIMEOUT_DELAY_5000)
		{
			err = MCEC_Io_CameraFix_Off_Timeout;
			break;
		}

		if(CIO::IO_I_Camera_Fix_Cylinder_Off())
			StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_LOAD_SOCKET_DN);
		break;

	case SEQ_STEP_OQC_MULTI_STARTUP_LOAD_SOCKET_DN:				// socket dn
		CIO::IO_O_Socket_Down(false);
		StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_LOAD_SOCKET_DN_CHECK);
		break;

	case SEQ_STEP_OQC_MULTI_STARTUP_LOAD_SOCKET_DN_CHECK:			// socket dn check
		if (StartUp_GetElapTime() > TIMEOUT_DELAY_5000)
		{
			err = MCEC_Io_Socket_Down_Timeout;
			break;
		}

		if(CIO::IO_I_Socket_Down())
			StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_LOAD_SOCKET_IN);
		break;

	case SEQ_STEP_OQC_MULTI_STARTUP_LOAD_SOCKET_IN:				// socket in
		CIO::IO_O_Socket_In(false);
		StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_LOAD_SOCKET_IN_CHECK);
		break;

	case SEQ_STEP_OQC_MULTI_STARTUP_LOAD_SOCKET_IN_CHECK:			// socket in check
		if (StartUp_GetElapTime() > TIMEOUT_DELAY_5000)
		{
			err = MCEC_Io_Socket_In_Timeout;
			break;
		}

		if(CIO::IO_I_Socket_In())
			StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_LOAD_SOCKET_UP);
		break;

	case SEQ_STEP_OQC_MULTI_STARTUP_LOAD_SOCKET_UP:				// socket up
		CIO::IO_O_Socket_Up(false);
		StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_LOAD_SOCKET_UP_CHECK);
		break;

	case SEQ_STEP_OQC_MULTI_STARTUP_LOAD_SOCKET_UP_CHECK:			// socket up check
		if (StartUp_GetElapTime() > TIMEOUT_DELAY_5000)
		{
			err = MCEC_Io_Socket_Up_Timeout;
			break;
		}

		if(CIO::IO_I_Socket_Up())
			StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_LOAD_CAMERA_FIX_ON);
		break;

	case SEQ_STEP_OQC_MULTI_STARTUP_LOAD_CAMERA_FIX_ON:			// camera fix on
		if (StartUp_GetElapTime() < TIMEOUT_DELAY_1000)
			break;

		CIO::IO_O_Camera_Fix_Cylinder_On(false);
		StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_LOAD_CAMERA_FIX_ON_CHECK);
		break;

	case SEQ_STEP_OQC_MULTI_STARTUP_LOAD_CAMERA_FIX_ON_CHECK:		// camera fix on check
		if (StartUp_GetElapTime() > TIMEOUT_DELAY_5000)
		{
			err = MCEC_Io_CameraFix_On_Timeout;
			break;
		}

		if(CIO::IO_I_Camera_Fix_Cylinder_On())
			StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_LOAD_PROBE_PIN_UP);
		break;

	case SEQ_STEP_OQC_MULTI_STARTUP_LOAD_PROBE_PIN_UP:			// probe pin up
		CIO::IO_O_ProbePin_Up(false);
		StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_LOAD_PROBE_PIN_UP_CHECK);
		break;

	case SEQ_STEP_OQC_MULTI_STARTUP_LOAD_PROBE_PIN_UP_CHECK:		// probe pin up check
		if (StartUp_GetElapTime() > TIMEOUT_DELAY_5000)
		{
			err = MCEC_Io_ProbePinUp_Timeout;
			break;
		}

		if(CIO::IO_I_ProbePin_Up())
			StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_LOAD_POWER_ON);
		break;

	case SEQ_STEP_OQC_MULTI_STARTUP_LOAD_POWER_ON:				// power on
		CDevice::PowerOn();
		StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_LOAD_PREVIEWSTART);
		break;

	case SEQ_STEP_OQC_MULTI_STARTUP_LOAD_PREVIEWSTART:			// preview start
		CDevice::SEMCO_PrewviewStart();
		m_dwTimer_Preview = GetTickCount();
		m_nPreviewRetry = 0;

		StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_LOAD_PREVIEWSTART_ACK_CHECK);
		break;

	case SEQ_STEP_OQC_MULTI_STARTUP_LOAD_PREVIEWSTART_ACK_CHECK:	// preview start check
	{
		TESTER_COMM	stTester = *DATAMANAGER->GetCurrentTesterCommData();
		int strWaitTime = _ttoi(stTester.strWaitTimeLIst[1]);
		if (StartUp_GetElapTime() > strWaitTime)
		{
			StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_LOAD_END);
			break;
		}

		// [22.0224.1] 리트라이
		if (m_UdpTester[0].m_bTesterFlag_PreviewStart == TRUE)
		{
			if (0 == m_UdpTester[0].m_bTesterFlag_PreviewStartErrorCode)
			{
				StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_LOAD_END);
			}
			else if (0 != m_UdpTester[0].m_bTesterFlag_PreviewStartErrorCode && m_nPreviewRetry < 3)		// retry
			{
				m_UdpTester[0].Send_Tester_Communication(eComMode_Insp_PreviewStop);
				CDevice::PowerOff();

				CIO::IO_O_ProbePin_Down();
				Sleep(500);
				CIO::IO_O_ProbePin_Up();

				CDevice::PowerReset();

				m_dwTimer_Preview = GetTickCount();
				m_nPreviewRetry++;

				StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_LOAD_RETRY_PREVIEWSTART);
			}
			else
			{
				StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_LOAD_END);
			}
		}
	}
		break;

	case SEQ_STEP_OQC_MULTI_STARTUP_LOAD_RETRY_PREVIEWSTART:		// retry preview start
		m_dwTimer_Preview = GetTickCount();
		CDevice::SEMCO_PrewviewStart();
		StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_LOAD_END);
		break;

	case SEQ_STEP_OQC_MULTI_STARTUP_LOAD_END:		// camera detect
		StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_COMPLETE);
		break;

	// 3
	case SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_START:	
		StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_WAIT_MOVE_Z);
		break;

	case SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_WAIT_MOVE_Z:			// wait move z position
		err = CMotion::MO_MoveZ_Zero();
		if(err == MCEC_OK)
			StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_WAIT_MOVE_Z_CHECK);
		break;

	case SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_WAIT_MOVE_Z_CHECK:	// wait move z position moving check
		if (StartUp_GetElapTime() > TIMEOUT_DELAY_10000)
		{
			err = MCEC_MotionZ_Not_Zero;
			break;
		}

		if(CMotion::MO_CheckZ_Zero())
			StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_WAIT_MOVE_Y);
		break;

	case SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_WAIT_MOVE_Y:			// wait move z position
		err = CMotion::MO_MoveY_Load();
		if(err == MCEC_OK)
			StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_WAIT_MOVE_Y_CHECK);
		break;

	case SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_WAIT_MOVE_Y_CHECK:	// wait move z position moving check
		if (StartUp_GetElapTime() > TIMEOUT_DELAY_10000)
		{
			err = MCEC_MotionY_MovingTimeout;
			break;
		}

		if (CMotion::MO_CheckY_Load())
			StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_PREVIEW_STOP);
		break;

	case SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_PREVIEW_STOP:			// prewview stop
		m_UdpTester[0].Send_Tester_Communication(eComMode_Insp_PreviewStop);
		StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_PREVIEW_STOP_CHECK);
		break;

	case SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_PREVIEW_STOP_CHECK:	// preview stop check
	{
		TESTER_COMM	stTester = *DATAMANAGER->GetCurrentTesterCommData();
		int strWaitTime = _ttoi(stTester.strWaitTimeLIst[4]);

		if (StartUp_GetElapTime() > strWaitTime)
		{
			StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_POWER_OFF);
			break;
		}

		if (m_UdpTester[0].m_bTesterFlag_PreviewStop == TRUE)
		{
			StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_POWER_OFF);
		}
	}
	break;

	case SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_POWER_OFF:			// power off
		CDevice::PowerOff();
		StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_PROBE_DN);
		break;

	case SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_PROBE_DN:				// probe pin dn
		CIO::IO_O_ProbePin_Down(false);
		StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_PROBE_DN_CHECK);
		break;

	case SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_PROBE_DN_CHECK:		// probe pin dn check
		if (StartUp_GetElapTime() > TIMEOUT_DELAY_3000)
		{
			err = MCEC_Io_ProbePinDn_Timeout;
			break;
		}

		if(CIO::IO_I_ProbePin_Down())
			StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_CAMERA_FIX_OFF);
		break;

	case SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_CAMERA_FIX_OFF:		// camera fix off
		CIO::IO_O_Camera_Fix_Cylinder_Off(false);
		StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_CAMERA_FIX_OFF_CHECK);
		break;

	case SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_CAMERA_FIX_OFF_CHECK:	// camera fix off check
		if (StartUp_GetElapTime() > TIMEOUT_DELAY_3000)
		{
			err = MCEC_Io_CameraFix_Off_Timeout;
			break;
		}

		if(CIO::IO_I_Camera_Fix_Cylinder_Off())
			StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_SOCKET_DN);
		break;

	case SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_SOCKET_DN:			// socket dn
		CIO::IO_O_Socket_Down(false);
		StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_SOCKET_DN_CHECK);
		break;

	case SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_SOCKET_DN_CHECK:		// socket dn check
		if (StartUp_GetElapTime() > TIMEOUT_DELAY_3000)
		{
			err = MCEC_Io_Socket_Down_Timeout;
			break;
		}

		if(CIO::IO_I_Socket_Down())
			StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_SOCKET_OUT);
		break;

	case SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_SOCKET_OUT:			// socket out
		CIO::IO_O_Socket_Out(false);
		StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_SOCKET_OUT_CHECK);
		break;

	case SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_SOCKET_OUT_CHECK:		// socket out check
		if (StartUp_GetElapTime() > TIMEOUT_DELAY_3000)
		{
			err = MCEC_Io_Socket_Out_Timeout;
			break;
		}

		if(CIO::IO_I_Socket_Out())
			StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_SOCKET_UP);
		break;

	case SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_SOCKET_UP:			// socket up
		CIO::IO_O_Socket_Up(false);
		StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_SOCKET_UP_CHECK);
		break;

	case SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_SOCKET_UP_CHECK:		// socket up check
		if (StartUp_GetElapTime() > TIMEOUT_DELAY_3000)
		{
			err = MCEC_Io_Socket_Up_Timeout;
			break;
		}

		if(CIO::IO_I_Socket_Up())
			StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_END);
		break;

	case SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_END:
		StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_COMPLETE);
		break;

	// 1
	case SEQ_STEP_OQC_MULTI_STARTUP_BLEMISH_START:	
		StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_BLEMISH_SOCKET_POS_CHECK);
		break;

	case SEQ_STEP_OQC_MULTI_STARTUP_BLEMISH_SOCKET_POS_CHECK:	// socket position check 
		StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_BLEMISH_MOTOR_POS_CHECK);
		break;

	case SEQ_STEP_OQC_MULTI_STARTUP_BLEMISH_MOTOR_POS_CHECK:		// blemish position check
		if (CMotion::MO_CheckZ_Blemish() && CMotion::MO_CheckY_Blemish())
			StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_BLEMISH_SEND_COMMAND);
		else
			StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_BLEMISH_MOVE_Z_ZERO);
		break;

	case SEQ_STEP_OQC_MULTI_STARTUP_BLEMISH_MOVE_Z_ZERO:			// blemish move z zero
		err = CMotion::MO_MoveZ_Zero();

		if(err == MCEC_OK)
			StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_BLEMISH_MOVE_Z_ZERO_CHECK);
		break;

	case SEQ_STEP_OQC_MULTI_STARTUP_BLEMISH_MOVE_Z_ZERO_CHECK:	// blemish move z zero moving check
		if (StartUp_GetElapTime() > TIMEOUT_DELAY_10000)
		{
			err = MCEC_MotionZ_Not_Zero;
			break;
		}

		if(CMotion::MO_CheckZ_Zero())
			StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_BLEMISH_MOVE_Y);
		break;

	case SEQ_STEP_OQC_MULTI_STARTUP_BLEMISH_MOVE_Y:				// blemish move y position
		err = CMotion::MO_MoveY_Blemish();
		if(err == MCEC_OK)
			StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_BLEMISH_MOVE_Y_CHECK);
		break;

	case SEQ_STEP_OQC_MULTI_STARTUP_BLEMISH_MOVE_Y_CHECK:		// blemish move y moving check
		if (StartUp_GetElapTime() > TIMEOUT_DELAY_10000)
		{
			err = MCEC_MotionY_MovingTimeout;
			break;
		}

		if(CMotion::MO_CheckY_Blemish())
			StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_BLEMISH_MOVE_Z);
		break;

	case SEQ_STEP_OQC_MULTI_STARTUP_BLEMISH_MOVE_Z:				// blemish move z position
		err = CMotion::MO_MoveZ_Blemish();
		if(err == MCEC_OK)
			StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_BLEMISH_MOVE_Z_CHECK);
		break;

	case SEQ_STEP_OQC_MULTI_STARTUP_BLEMISH_MOVE_Z_CHECK:		// blemish move z moving check
		if (StartUp_GetElapTime() > TIMEOUT_DELAY_10000)
		{
			err = MCEC_MotionZ_MovingTimeout;
			break;
		}

		if(CMotion::MO_CheckZ_Blemish())
			StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_BLEMISH_SEND_COMMAND);
		break;

	case SEQ_STEP_OQC_MULTI_STARTUP_BLEMISH_SEND_COMMAND:		// blemish send command
		m_UdpTester[0].Send_Tester_Communication(eComMode_Insp_TEST);
		StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_BLEMISH_SEND_COMMAND_CHECK);
		break;

	case SEQ_STEP_OQC_MULTI_STARTUP_BLEMISH_SEND_COMMAND_CHECK:	// blemish ack check
	{
		if (StartUp_GetElapTime() < TIMEOUT_DELAY_200)
			break;

		TESTER_COMM	stTester = *DATAMANAGER->GetCurrentTesterCommData();
		int strWaitTime = _ttoi(stTester.strWaitTimeLIst[2]);
		if (StartUp_GetElapTime() > strWaitTime)
		{
			StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_BLEMISH_END);
			break;
		}

		if (m_UdpTester[0].m_bTesterFlag_Inspection_TEST == TRUE)
		{
			COMMON->m_nMainUiFlag_InspectResult[1] = m_UdpTester[0].m_bTesterFlag_Inspection_TESTErrorCode;

			if (0 != m_UdpTester[0].m_bTesterFlag_Inspection_TESTErrorCode)
			{
				StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_BLEMISH_END);
				break;
			}

			StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_BLEMISH_END);
		}
	}
		break;

	case SEQ_STEP_OQC_MULTI_STARTUP_BLEMISH_END:	
		StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_COMPLETE);
		break;

	// 2
	case SEQ_STEP_OQC_MULTI_STARTUP_SFR_START:
		StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_SFR_SOCKET_POS_CHECK);
		break;

	case SEQ_STEP_OQC_MULTI_STARTUP_SFR_SOCKET_POS_CHECK:	// socket position check 
		StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_SFR_MOTOR_POS_CHECK);
		break;

	case SEQ_STEP_OQC_MULTI_STARTUP_SFR_MOTOR_POS_CHECK:		// SFR position check
		if (CMotion::MO_CheckY_SFR() && CMotion::MO_CheckZ_SFR())
			StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_SFR_SEND_COMMAND);
		else
			StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_SFR_MOVE_Z_ZERO);
		break;

	case SEQ_STEP_OQC_MULTI_STARTUP_SFR_MOVE_Z_ZERO:			// SFR move z zero
		err = CMotion::MO_MoveZ_Zero();

		if(err == MCEC_OK)
			StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_SFR_MOVE_Z_ZERO_CHECK);
		break;

	case SEQ_STEP_OQC_MULTI_STARTUP_SFR_MOVE_Z_ZERO_CHECK:	// SFR move z zero moving check
		if (StartUp_GetElapTime() > TIMEOUT_DELAY_10000)
		{
			err = MCEC_MotionZ_MovingTimeout;
			break;
		}

		if(CMotion::MO_CheckZ_Zero())
			StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_SFR_MOVE_Y);
		break;

	case SEQ_STEP_OQC_MULTI_STARTUP_SFR_MOVE_Y:				// SFR move y position
		err = CMotion::MO_MoveY_SFR();
		CDevice::LightOn();
		if(err == MCEC_OK)
			StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_SFR_MOVE_Y_CHECK);
		break;

	case SEQ_STEP_OQC_MULTI_STARTUP_SFR_MOVE_Y_CHECK:		// SFR move y moving check
		if (StartUp_GetElapTime() > TIMEOUT_DELAY_10000)
		{
			err = MCEC_MotionY_MovingTimeout;
			break;
		}

		if(CMotion::MO_CheckY_SFR())
			StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_SFR_MOVE_Z);
		break;

	case SEQ_STEP_OQC_MULTI_STARTUP_SFR_MOVE_Z:				// SFR move z position
		err = CMotion::MO_MoveZ_SFR();

		if(err == MCEC_OK)
			StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_SFR_MOVE_Z_CHECK);
		break;

	case SEQ_STEP_OQC_MULTI_STARTUP_SFR_MOVE_Z_CHECK:		// SFR move z moving check
		if (StartUp_GetElapTime() > TIMEOUT_DELAY_10000)
		{
			err = MCEC_MotionZ_MovingTimeout;
			break;
		}

		if(CMotion::MO_CheckZ_SFR())
			StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_SFR_SEND_COMMAND);
		break;

	case SEQ_STEP_OQC_MULTI_STARTUP_SFR_SEND_COMMAND:		// SFR send command
		m_UdpTester[0].Send_Tester_Communication(eComMode_Insp_SFR);
		StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_SFR_SEND_COMMAND_CHECK);
		break;

	case SEQ_STEP_OQC_MULTI_STARTUP_SFR_SEND_COMMAND_CHECK:	// SFR ack check
	{
		if (StartUp_GetElapTime() < TIMEOUT_DELAY_200)
			break;

		TESTER_COMM	stTester = *DATAMANAGER->GetCurrentTesterCommData();
		int strWaitTime = _ttoi(stTester.strWaitTimeLIst[3]);

		if (StartUp_GetElapTime() > strWaitTime)
		{
			StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_SFR_END);
			break;
		}

		if (m_UdpTester[0].m_bTesterFlag_Inspection_SFR == TRUE)
		{
			if (0 != m_UdpTester[0].m_bTesterFlag_Inspection_SFRErrorCode)
			{
				StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_SFR_END);
				break;
			}
			StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_SFR_END);
		}
	}
		break;

	case SEQ_STEP_OQC_MULTI_STARTUP_SFR_END:
		StartUp_SetStep(SEQ_STEP_OQC_MULTI_STARTUP_COMPLETE);
		break;

	default:
		break;
	}		
	return (UINT)err;
}

UINT CSeqOqcMulti::StartOperation_Run()
{
	TESTER_COMM		stTester = *DATAMANAGER->GetCurrentTesterCommData();
	TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();
	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();
	EQ_BASIC_PARAM	stEq = *DATAMANAGER->GetCurrentEqBasicData();

	auto err = MCEC_OK;

	switch (GetStep())
	{
	case SEQ_STEP_SFR_WIDE_READY:
	{
		ERROR_LIST	stErr = *DATAMANAGER->GetCurrentErrorList();
		EQ_BASIC_PARAM	stEq = *DATAMANAGER->GetCurrentEqBasicData();

		// 정상적으로 루틴을 한번 탔으면 정지 하자.
		if (m_bFlagProcess_NormalFinish == TRUE)
		{
			SetStopswitchMode(FALSE);
			Clear_MainUiFlag();
			COMMON->SetAreaStop_OkPush(FALSE);
			COMMON->SetAccpet_ReciveBarcode(TRUE);

			m_UdpTester[0].Reset_TesterFlag();
			AutoMode_Run();
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
			COMMON->m_bMainUiFlag_StepView[0] = TRUE;
			COMMON->m_bMainUiFlag_StepView[2] = TRUE;
		}

		// 제품 안착이 안되었으면 시작 안함 
		if (CIO::IO_I_DetectCamera() == false)
			break;

#else
		COMMON->m_bMainUiFlag_StepView[0] = TRUE;
		COMMON->m_bMainUiFlag_StepView[2] = TRUE;
#endif

#ifndef _DRY_RUN_MODULE_NONE

#if (GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL) || (GET_INSPECTOR == SYS_OQC_SFR_SINGLE_CL)
		// 카메라가 있어야 동작한다.
		if (AJIN_IO->GetInputIoBit(DIO_INPUT_X0_08_CAMERA_DETECT_SENSOR) == FALSE)
			break;
		else
#endif
			COMMON->m_bMainUiFlag_StepView[1] = TRUE;
#endif
	//	CDevice::PowerOn();

#ifndef _DRY_RUN_MODULE_NONE
		if (m_UdpTester[0].m_bConnected != TRUE)
		{
			// NG CODE
			break;
		}

		SetStep(SEQ_STEP_SFR_WIDE_INTERLOCK_CHECK);
#else
		// 드라이 런일때
		SetStep(SEQ_STEP_SFR_WIDE_INTERLOCK_CHECK);
#endif
	}
	break;
	case SEQ_STEP_SFR_WIDE_INTERLOCK_CHECK:
	{
#ifndef _DRY_RUN_MODULE_NONE
		if (COMMON->GetInterlock_CameraDetected() == FALSE || COMMON->GetInterlock_SocketClose() == FALSE)
		{
			err = MCEC_Io_SocketCoverOpen;
			break;
		}
#endif

		SetStopswitchMode(FALSE);
		COMMON->SetMainUiFlag_CurrentPos(0);

		SetStep(SEQ_STEP_SFR_WIDE_MOVE_SOCKET_CYLINDER_DOWN);
	}
	break;

	case SEQ_STEP_SFR_WIDE_MOVE_SOCKET_CYLINDER_DOWN:
	{
		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_1E_SOCKET_UP, FALSE);
		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_1F_SOCKET_DOWN, TRUE);

		SetStep(SEQ_STEP_SFR_WIDE_MOVE_SOCKET_CYLINDER_DOWN_CHK);
	}
	break;

	case SEQ_STEP_SFR_WIDE_MOVE_SOCKET_CYLINDER_DOWN_CHK:
	{
		if (CIO::IO_I_Socket_Down() == true)
		{
			SetStep(SEQ_STEP_SFR_WIDE_MOVE_SOCKET_CYLINDER_IN);
		}
		else if (GetElapTime() > TIMEOUT_SENSOR_5000)
		{
			err = MCEC_Io_Socket_Down_Timeout;
		}
	}
	break;
	case SEQ_STEP_SFR_WIDE_MOVE_SOCKET_CYLINDER_IN:
	{
		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_1D_SOCKET_OUT, FALSE);
		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_1C_SOCKET_IN, TRUE);

		SetStep(SEQ_STEP_SFR_WIDE_MOVE_SOCKET_CYLINDER_IN_CHK);
	}
	break;
	case SEQ_STEP_SFR_WIDE_MOVE_SOCKET_CYLINDER_IN_CHK:
	{
		if (CIO::IO_I_Socket_In() == true)
		{
			SetStep(SEQ_STEP_SFR_WIDE_MOVE_SOCKET_CYLINDER_UP);
		}
		else if (GetElapTime() > TIMEOUT_SENSOR_5000)
		{
			err = MCEC_Io_Socket_In_Timeout;
		}
	}
	break;

	// CYLINDER UP
	case SEQ_STEP_SFR_WIDE_MOVE_SOCKET_CYLINDER_UP:
	{
		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_1F_SOCKET_DOWN, FALSE);
		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_1E_SOCKET_UP, TRUE);

		SetStep(SEQ_STEP_SFR_WIDE_MOVE_SOCKET_CYLINDER_UP_CHK);
	}
	break;

	// CYLINDER UP CHK
	case SEQ_STEP_SFR_WIDE_MOVE_SOCKET_CYLINDER_UP_CHK:
	{
		if (CIO::IO_I_Socket_Up() == true)
		{
#ifdef _DRY_RUN_MODULE_NONE
			SetStep(SEQ_STEP_SFR_WIDE_MOVE_PROBE_CYLINDER_UP);
#else
			SetStep(SEQ_STEP_SFR_WIDE_MOVE_CAMERA_FIX_CYLINDER_ON);
#endif
		}
		else if (GetElapTime() > TIMEOUT_SENSOR_5000)
		{
			err = MCEC_Io_Socket_Up_Timeout;
		}
	}
	break;

	case SEQ_STEP_SFR_WIDE_MOVE_CAMERA_FIX_CYLINDER_ON:
	{
		if (GetElapTime() < TIMEOUT_DELAY_1000)
			break;

		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_0D_CAMERA_FIX_CYL_OFF, FALSE);
		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_0C_CAMERA_FIX_CYL_ON, TRUE);

		SetStep(SEQ_STEP_SFR_WIDE_MOVE_CAMERA_FIX_CYLINDER_ON_CHK);
	}
	break;

	case SEQ_STEP_SFR_WIDE_MOVE_CAMERA_FIX_CYLINDER_ON_CHK:
	{
		if (CIO::IO_I_Camera_Fix_Cylinder_On() == true)
			SetStep(SEQ_STEP_SFR_WIDE_MOVE_PROBE_CYLINDER_UP);
		else if (GetElapTime() > TIMEOUT_SENSOR_5000)
			err = MCEC_Io_CameraFix_On_Timeout;
	}
	break;

	case SEQ_STEP_SFR_WIDE_MOVE_PROBE_CYLINDER_UP:
	{
		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_0F_PROBEPIN_CYLINDER_DOWN, FALSE);
		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_0E_PROBEPIN_CYLINDER_UP, TRUE);

		SetStep(SEQ_STEP_SFR_WIDE_MOVE_PROBE_CYLINDER_UP_CHK);
	}
	break;

	case SEQ_STEP_SFR_WIDE_MOVE_PROBE_CYLINDER_UP_CHK:
	{
		if (CIO::IO_I_ProbePin_Up())
		{
			CDevice::PowerOn();
			SetStep(SEQ_STEP_SFR_WIDE_SEND_INSPECTION_PREVIEW_START);
		}
		else if (GetElapTime() > TIMEOUT_SENSOR_5000)
			err = MCEC_Io_Socket_Up_Timeout;
	}
	break;

	case SEQ_STEP_SFR_WIDE_SEND_INSPECTION_PREVIEW_START:
	{
		if (GetElapTime() < TIMEOUT_DELAY_200)
			break;

		Sleep(200);

		m_UdpTester[0].Reset_TesterFlag();
		m_UdpTester[0].Send_Tester_Communication(eComMode_PreviewStart);	//START PREVIEW 1		

		SetPowerVoltage();

		SetStep(SEQ_STEP_SFR_WIDE_MOVE_Z_ZERO_2);

		m_dwTimer_Preview = GetTickCount();
	}
	break;

	case SEQ_STEP_SFR_WIDE_MOVE_Z_ZERO_2:
	{
		err = CMotion::MO_MoveZ_Zero();

		if(err == MCEC_OK)
			SetStep(SEQ_STEP_SFR_WIDE_MOVE_Z_ZERO_2_CHK);
	}
	break;
	case SEQ_STEP_SFR_WIDE_MOVE_Z_ZERO_2_CHK:
	{
		if (GetElapTime() < TIMEOUT_DELAY_200)
			break;

		if (GetElapTime() > TIMEOUT_DELAY_10000)
		{
			err = MCEC_MotionZ_MovingTimeout;
			break;
		}

		if (CMotion::MO_CheckZ_Zero())
		{
			COMMON->m_bMainUiFlag_StepView[16] = TRUE;
			SetStep(SEQ_STEP_SFR_WIDE_MOVE_CYLINDER_UP_Y_POS);
		}
	}
	break;

	case SEQ_STEP_SFR_WIDE_MOVE_CYLINDER_UP_Y_POS:
	{
		err = CMotion::MO_MoveY_Blemish();
		if(err == MCEC_OK)
		{
			COMMON->m_bMainUiFlag_MoveSocket[0] = TRUE;
			SetStep(SEQ_STEP_SFR_WIDE_MOVE_CYLINDER_UP_Y_POS_CHK);
		}
	}
	break;

	case SEQ_STEP_SFR_WIDE_MOVE_CYLINDER_UP_Y_POS_CHK:
	{
		if (GetElapTime() < TIMEOUT_DELAY_200)
			break;

		if (GetElapTime() > TIMEOUT_DELAY_30000)
		{
			err = MCEC_MotionY_MovingTimeout;
			break;
		}

		if(CMotion::MO_CheckY_Blemish())
		{
			COMMON->SetMainUiFlag_CurrentPos(1);
			COMMON->SetProbePinCylinder_UpDown(_CYLINDER_UP);

			SetStep(SEQ_STEP_SFR_WIDE_MOVE_SFR_Y_POS);
		}
	}
	break;

	case SEQ_STEP_SFR_WIDE_MOVE_SFR_Y_POS:
	{
		err = CMotion::MO_MoveY_SFR();
		CDevice::LightOn();

		if(err == MCEC_OK)
		{
			COMMON->m_bMainUiFlag_MoveSocket[1] = TRUE;
			SetStep(SEQ_STEP_SFR_WIDE_MOVE_SFR_Y_POS_CHK);
		}		
	}
	break;

	case SEQ_STEP_SFR_WIDE_MOVE_SFR_Y_POS_CHK:
	{
		if (GetElapTime() < TIMEOUT_DELAY_200)
			break;

		if (GetElapTime() > TIMEOUT_DELAY_10000)
		{
			err = MCEC_MotionY_MovingTimeout;
			break;
		}

		if(CMotion::MO_CheckY_SFR())
		{
			COMMON->SetMainUiFlag_CurrentPos(2);
			SetStep(SEQ_STEP_SFR_WIDE_MOVE_SFR_Z_POS);
		}
	}
	break;

	case SEQ_STEP_SFR_WIDE_MOVE_SFR_Z_POS:
	{
		if (GetElapTime() < TIMEOUT_DELAY_500)
			break;

		err = CMotion::MO_MoveZ_SFR();

		if(err == MCEC_OK)
			SetStep(SEQ_STEP_SFR_WIDE_MOVE_SFR_Z_POS_CHK);
	}
	break;

	case SEQ_STEP_SFR_WIDE_MOVE_SFR_Z_POS_CHK:
	{
		if (GetElapTime() < TIMEOUT_DELAY_200)
			break;

		if (GetElapTime() > TIMEOUT_DELAY_10000)
		{
			err = MCEC_MotionZ_MovingTimeout;
			break;
		}

		if(CMotion::MO_CheckZ_SFR())
		{
			m_nPreviewRetry = 1;
			SetStep(SEQ_STEP_SFR_WIDE_SEND_INSPECTION_PREVIEW_START_CHK);
		}
	}
	break;

	case SEQ_STEP_SFR_WIDE_SEND_INSPECTION_PREVIEW_RESTART:
	{
		if (GetElapTime() < TIMEOUT_DELAY_200)
			break;

		Sleep(200);

		m_dwTimer_Preview = GetTickCount();

		CDevice::SEMCO_PrewviewStart();

		SetPowerVoltage();

		SetStep(SEQ_STEP_SFR_WIDE_SEND_INSPECTION_PREVIEW_START_CHK);
	}
	break;

	case SEQ_STEP_SFR_WIDE_SEND_INSPECTION_PREVIEW_START_CHK:
	{
#ifndef _DRY_RUN_MODULE_NONE
		TESTER_COMM	stTester = *DATAMANAGER->GetCurrentTesterCommData();
		int strWaitTime = _ttoi(stTester.strWaitTimeLIst[1]);
		if (GetElapTime() > strWaitTime)
		{
			// ng code

			SetStep(SEQ_STEP_SFR_WIDE_SEND_INSPECTION_PREVIEW_STOP);
			break;
		}

		// [22.0224.1] 리트라이
		if (m_UdpTester[0].m_bTesterFlag_PreviewStart == TRUE)
		{
			if (0 == m_UdpTester[0].m_bTesterFlag_PreviewStartErrorCode)
			{
				int nRetCode = 0;

				nRetCode = (int)CheckPowerVoltage();

				if(nRetCode == 0)
					SetStep(SEQ_STEP_SFR_WIDE_INSPECTION_MEASURE_CURRENT);
				else
				{
					EQ_BASIC_PARAM	stEq = *DATAMANAGER->GetCurrentEqBasicData();
					if (stEq.nLanguage == 0)
						WRITE_SEQ_LOG(_T("전압 셋팅 실패"));
					else
						WRITE_SEQ_LOG(_T("Power Voltage Set failed."));

					m_strMsg = _T("[Power Voltage failed]");
					COMMON->ShowMsg_Thread(m_strMsg, MODE_NO_CLOSE);

					SetStep(SEQ_STEP_SFR_WIDE_SEND_INSPECTION_PREVIEW_STOP);
				}
			}
			else if (0 != m_UdpTester[0].m_bTesterFlag_PreviewStartErrorCode && m_nPreviewRetry < 3)		// retry
			{
				CDevice::SEMCO_PrewviewStop();

				COMMON->SetProbePinCylinder_UpDown(_CYLINDER_DOWN);
				Sleep(500);
				COMMON->SetProbePinCylinder_UpDown(_CYLINDER_UP);

				CDevice::PowerReset();				

				m_nPreviewRetry++;
				SetStep(SEQ_STEP_SFR_WIDE_SEND_INSPECTION_PREVIEW_RESTART);
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

				SetStep(SEQ_STEP_SFR_WIDE_SEND_INSPECTION_PREVIEW_STOP);
			}
		}
#else
		SetStep(SEQ_STEP_SFR_WIDE_INSPECTION_MEASURE_CURRENT);
#endif
	}
	break;

	case SEQ_STEP_SFR_WIDE_INSPECTION_MEASURE_CURRENT:
	{
#ifndef _DRY_RUN_MODULE_NONE

		CString sz, szTemp;

		// 전류 측정
		CDevice::SEMCO_SendCurrentVoltage();
		CDevice::SEMCO_WaitCurrentVoltageAck(1000, sz);

		szTemp.Format(_T("Voltage And Current : %s"), sz);
		WRITE_SEQ_LOG(sz);

		SetStep(SEQ_STEP_SFR_WIDE_SEND_INSEPCTION_SFR);
#else
		SetStep(SEQ_STEP_SFR_WIDE_SEND_INSEPCTION_SFR);
#endif
	}
		break;

	case SEQ_STEP_SFR_WIDE_SEND_INSEPCTION_SFR:
	{
		if (m_UdpTester[0].Send_Tester_Communication(eComMode_Insp_SFR) == TRUE)
			COMMON->m_bMainUiFlag_StepView[17] = TRUE;

		SetStep(SEQ_STEP_SFR_WIDE_SEND_INSEPCTION_SFR_WAIT_RECIVE);
	}
	break;
	case SEQ_STEP_SFR_WIDE_SEND_INSEPCTION_SFR_WAIT_RECIVE:
	{
#ifndef _DRY_RUN_MODULE_NONE
		if (GetElapTime() < TIMEOUT_DELAY_200)
			break;

		TESTER_COMM	stTester = *DATAMANAGER->GetCurrentTesterCommData();
		int strWaitTime = _ttoi(stTester.strWaitTimeLIst[3]);

		if (GetElapTime() > strWaitTime)
		{
			// ng code

			SetStep(SEQ_STEP_SFR_WIDE_SEND_INSPECTION_PREVIEW_STOP);
			break;
		}

		if (m_UdpTester[0].m_bTesterFlag_Inspection_SFR == TRUE)
		{
			COMMON->m_bMainUiFlag_StepView[18] = TRUE;
			COMMON->m_nMainUiFlag_InspectResult[0] = m_UdpTester[0].m_bTesterFlag_Inspection_SFRErrorCode;

			if (0 != m_UdpTester[0].m_bTesterFlag_Inspection_SFRErrorCode)
			{
				EQ_BASIC_PARAM	stEq = *DATAMANAGER->GetCurrentEqBasicData();
				if (stEq.nLanguage == 0)
					WRITE_SEQ_LOG(_T("SFR 검사실패"));
				else
					WRITE_SEQ_LOG(_T("SFR test failed."));

				//Main Err부분 추가 되면 UI변경
				COMMON->SetProbePinCylinder_UpDown(_CYLINDER_DOWN);
				COMMON->SetNGCnt();
				m_strMsg.Format(_T("NG %02d"), m_UdpTester[0].m_bTesterFlag_Inspection_SFRErrorCode);
				COMMON->ShowMsg_Thread(m_strMsg, MODE_NO_CLOSE, FALSE);

				SetStep(SEQ_STEP_SFR_WIDE_SEND_INSPECTION_PREVIEW_STOP);
				break;
			}
			SetStep(SEQ_STEP_SFR_WIDE_MOVE_Z_ZERO_1);
		}
#else
		SetStep(SEQ_STEP_SFR_WIDE_MOVE_Z_ZERO_1);
#endif
	}
	break;


	///////////
	case SEQ_STEP_SFR_WIDE_MOVE_Z_ZERO_1:
	{
		err = CMotion::MO_MoveZ_Zero();
		if(err == MCEC_OK)
			SetStep(SEQ_STEP_SFR_WIDE_MOVE_Z_ZERO_1_CHK);		
	}
	break;
	case SEQ_STEP_SFR_WIDE_MOVE_Z_ZERO_1_CHK:
	{
		if (GetElapTime() < TIMEOUT_DELAY_500)
			break;

		if (GetElapTime() > TIMEOUT_DELAY_10000)
		{
			err = MCEC_MotionZ_MovingTimeout;
			break;
		}

		if(CMotion::MO_CheckZ_Zero())
			SetStep(SEQ_STEP_SFR_WIDE_MOVE_BLEMISHI_Y_POS);
	}
	break;

	case SEQ_STEP_SFR_WIDE_MOVE_BLEMISHI_Y_POS:
	{
		err = CMotion::MO_MoveY_Blemish();
		if(err == MCEC_OK)
		{
			COMMON->m_bMainUiFlag_MoveSocket[2] = TRUE;
			SetStep(SEQ_STEP_SFR_WIDE_MOVE_BLEMISHI_Y_POS_CHK);
		}
	}
	break;

	case SEQ_STEP_SFR_WIDE_MOVE_BLEMISHI_Y_POS_CHK:
	{
		if (GetElapTime() > TIMEOUT_DELAY_10000)
		{
			err = MCEC_MotionY_MovingTimeout;
			break;
		}

		if(CMotion::MO_CheckY_Blemish())
		{
			COMMON->m_bMainUiFlag_CurrentAction_Old[1] = FALSE;
			COMMON->SetMainUiFlag_CurrentPos(1);

			SetStep(SEQ_STEP_SFR_WIDE_MOVE_BLEMISHI_Z_POS);
		}
	}
	break;

	case SEQ_STEP_SFR_WIDE_MOVE_BLEMISHI_Z_POS:
	{
		if (GetElapTime() < TIMEOUT_DELAY_500)
			break;

		err = CMotion::MO_MoveZ_Blemish();
		if(err == MCEC_OK)
			SetStep(SEQ_STEP_SFR_WIDE_MOVE_BLEMISHI_Z_POS_CHK);
	}
	break;

	case SEQ_STEP_SFR_WIDE_MOVE_BLEMISHI_Z_POS_CHK:
	{
		if (GetElapTime() < TIMEOUT_DELAY_200)
			break;

		if (GetElapTime() > TIMEOUT_DELAY_10000)
		{
			err = MCEC_MotionZ_MovingTimeout;
			break;
		}

		if(CMotion::MO_CheckZ_Blemish())
			SetStep(SEQ_STEP_SFR_WIDE_SEND_INSEPCTION_BLEMISHI);
	}
	break;


	case SEQ_STEP_SFR_WIDE_SEND_INSEPCTION_BLEMISHI:
	{
		if (GetElapTime() < TIMEOUT_DELAY_500)
			break;

		if (m_UdpTester[0].Send_Tester_Communication(eComMode_Insp_TEST) == TRUE)
			COMMON->m_bMainUiFlag_StepView[11] = TRUE;

		SetStep(SEQ_STEP_SFR_WIDE_SEND_INSEPCTION_BLEMISHI_WAIT_RECIVE);
	}
	break;
	case SEQ_STEP_SFR_WIDE_SEND_INSEPCTION_BLEMISHI_WAIT_RECIVE:
	{
#ifndef _DRY_RUN_MODULE_NONE
		if (GetElapTime() < TIMEOUT_DELAY_200)
			break;

		TESTER_COMM	stTester = *DATAMANAGER->GetCurrentTesterCommData();
		int strWaitTime = _ttoi(stTester.strWaitTimeLIst[2]);
		if (GetElapTime() > strWaitTime)
		{
			// ng code

			SetStep(SEQ_STEP_SFR_WIDE_SEND_INSPECTION_PREVIEW_STOP);
			break;
		}

		if (m_UdpTester[0].m_bTesterFlag_Inspection_TEST == TRUE)
		{
			COMMON->m_bMainUiFlag_StepView[12] = TRUE;

			COMMON->m_nMainUiFlag_InspectResult[1] = m_UdpTester[0].m_bTesterFlag_Inspection_TESTErrorCode;

			if (0 != m_UdpTester[0].m_bTesterFlag_Inspection_TESTErrorCode)
			{
				EQ_BASIC_PARAM	stEq = *DATAMANAGER->GetCurrentEqBasicData();
				if (stEq.nLanguage == 0)
					WRITE_SEQ_LOG(_T("이물 감지됨"));
				else
					WRITE_SEQ_LOG(_T("Blemishi object detected."));

				COMMON->SetNGCnt();
				m_strMsg.Format(_T("NG %02d"), m_UdpTester[0].m_bTesterFlag_Inspection_TESTErrorCode);
				COMMON->ShowMsg_Thread(m_strMsg, MODE_NO_CLOSE /*MODE_INFORMATION, FALSE*/);

				SetStep(SEQ_STEP_SFR_WIDE_SEND_INSPECTION_PREVIEW_STOP);
				break;
			}

			m_strMsg = _T("PASS");
			COMMON->ShowMsg_Thread(m_strMsg, MODE_NO_CLOSE);
			SetStep(SEQ_STEP_SFR_WIDE_SEND_INSPECTION_PREVIEW_STOP);
		}
#else
		SetStep(SEQ_STEP_SFR_WIDE_SEND_INSPECTION_PREVIEW_STOP);
#endif
	}
	break;

	///////////////////////////////////////////////////////////////
	/*종료시퀀스*/

	case SEQ_STEP_SFR_WIDE_SEND_INSPECTION_PREVIEW_STOP:
	{
		if (m_UdpTester[0].Send_Tester_Communication(eComMode_Insp_PreviewStop) == TRUE)
			COMMON->m_bMainUiFlag_StepView[15] = TRUE;

		SetStep(SEQ_STEP_SFR_WIDE_MOVE_Z_ZERO_3);
	}
	break;

	case SEQ_STEP_SFR_WIDE_MOVE_Z_ZERO_3:
	{
		if (GetElapTime() < TIMEOUT_DELAY_500)
			break;

		err = CMotion::MO_MoveZ_Zero();
		if(err == MCEC_OK)
			SetStep(SEQ_STEP_SFR_WIDE_MOVE_Z_ZERO_3_CHK);
	}
	break;
	case SEQ_STEP_SFR_WIDE_MOVE_Z_ZERO_3_CHK:
	{
		if (GetElapTime() < TIMEOUT_DELAY_200)
			break;

		if (GetElapTime() > TIMEOUT_DELAY_10000)
		{
			err = MCEC_MotionZ_MovingTimeout;
			break;
		}

		if(CMotion::MO_CheckZ_Zero())
		{
			SetStep(SEQ_STEP_SFR_WIDE_MOVE_Y_WAIT_POS);
		}
	}
	break;

	case SEQ_STEP_SFR_WIDE_MOVE_Y_WAIT_POS:
	{
		if (GetElapTime() < TIMEOUT_DELAY_200)
			break;

		err = CMotion::MO_MoveY_Load();
		if(err == MCEC_OK)
		{
			COMMON->m_bMainUiFlag_MoveSocket[3] = TRUE;
			SetStep(SEQ_STEP_SFR_WIDE_MOVE_Y_WAIT_POS_CHK);
		}		
	}
	break;

	case SEQ_STEP_SFR_WIDE_MOVE_Y_WAIT_POS_CHK:
	{
		if (GetElapTime() < TIMEOUT_DELAY_200)
			break;

		if (GetElapTime() > TIMEOUT_DELAY_30000)
		{
		//	if (m_ctrlLight_ODA_Q[0].Write_OFF() == TRUE)
			if (CDevice::PowerOff() == TRUE)
				COMMON->m_bMainUiFlag_StepView[8] = TRUE;

			err = MCEC_MotionY_MovingTimeout;
			break;
		}

		if(CMotion::MO_CheckY_Load())
		{
	//		m_ctrlLight_ODA_Q[0].Write_OFF();
			ODA_Q->Send_Output(enSwitchOnOff::Switch_OFF);

			SetStep(SEQ_STEP_SFR_WIDE_MOVE_Z_WAIT_POS);
		}
	}
	break;

	case SEQ_STEP_SFR_WIDE_MOVE_Z_WAIT_POS:
	{
		if (GetElapTime() < TIMEOUT_DELAY_200)
			break;

		err = CMotion::MO_MoveZ_Load();
		if(err == MCEC_OK)
			SetStep(SEQ_STEP_SFR_WIDE_MOVE_Z_WAIT_POS_CHK);
	}
	break;

	case SEQ_STEP_SFR_WIDE_MOVE_Z_WAIT_POS_CHK:
	{
		if (GetElapTime() < TIMEOUT_DELAY_200)
			break;

		if (GetElapTime() > TIMEOUT_DELAY_30000)
		{
			err = MCEC_MotionZ_MovingTimeout;
			break;
		}

		if(CMotion::MO_CheckZ_Load())
			SetStep(SEQ_STEP_SFR_WIDE_SEND_INSPECTION_PREVIEW_STOP_CHK);
	}
	break;

	case SEQ_STEP_SFR_WIDE_SEND_INSPECTION_PREVIEW_STOP_CHK:
	{
#ifndef _DRY_RUN_MODULE_NONE
		TESTER_COMM	stTester = *DATAMANAGER->GetCurrentTesterCommData();
		int strWaitTime = _ttoi(stTester.strWaitTimeLIst[4]);

		if (m_UdpTester[0].m_bConnected)
		{
			if (GetElapTime() < strWaitTime)
			{
				// ng code
				SetStep(SEQ_STEP_SFR_WIDE_MOVE_CAMERA_FIX_CYLINDER_OFF);
				break;
			}
		}

		if (m_UdpTester[0].m_bTesterFlag_PreviewStop == TRUE)
		{
			COMMON->m_bMainUiFlag_StepView[13] = TRUE;

			if (0 != m_UdpTester[0].m_bTesterFlag_PreviewStopErrorCode)
			{
				EQ_BASIC_PARAM	stEq = *DATAMANAGER->GetCurrentEqBasicData();
				if (stEq.nLanguage == 0)
					WRITE_SEQ_LOG(_T("카메라 끄기 실패"));
				else
					WRITE_SEQ_LOG(_T("Falied camera off"));
				//Main Err부분 추가 되면 UI변경
			}

		//	if (m_ctrlLight_ODA_Q[0].Write_OFF() == TRUE)
			if (CDevice::PowerOff() == TRUE)
				COMMON->m_bMainUiFlag_StepView[9] = TRUE;

			SetStep(SEQ_STEP_SFR_WIDE_MOVE_CAMERA_FIX_CYLINDER_OFF);
		}
		else
		{
			SetStep(SEQ_STEP_SFR_WIDE_MOVE_CAMERA_FIX_CYLINDER_OFF);
		}
#else
// 		if (m_ctrlLight_ODA_Q[0].Write_OFF() == TRUE)
// 			COMMON->m_bMainUiFlag_StepView[9] = TRUE;

		SetStep(SEQ_STEP_SFR_WIDE_MOVE_CAMERA_FIX_CYLINDER_OFF);
#endif
	}
	break;

	case SEQ_STEP_SFR_WIDE_MOVE_CAMERA_FIX_CYLINDER_OFF:
	{
		//CIO::IO_O_Camera_Fix_Cylinder_Off();

		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_0C_CAMERA_FIX_CYL_ON, FALSE);
		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_0D_CAMERA_FIX_CYL_OFF, TRUE);

		SetStep(SEQ_STEP_SFR_WIDE_MOVE_CAMERA_FIX_CYLINDER_OFF_CHK);
	}
	break;

	// FIX CYLINDER OFF CHK
	case SEQ_STEP_SFR_WIDE_MOVE_CAMERA_FIX_CYLINDER_OFF_CHK:
	{
		if (GetElapTime() < TIMEOUT_DELAY_200)
			break;

#ifndef _DRY_RUN_MODULE_NONE

		if (CIO::IO_O_Camera_Fix_Cylinder_Off() == true)
		{
			SetStep(SEQ_STEP_SFR_WIDE_MOVE_CAMERA_CYLINDER_DOWN);
		}
		else if (GetElapTime() > TIMEOUT_SENSOR_5000)
		{
			err = MCEC_Io_CameraFix_Off_Timeout;			
		}
#else
		SetStep(SEQ_STEP_SFR_WIDE_MOVE_CAMERA_CYLINDER_DOWN);
#endif
	}
	break;

	// CYLINDER DOWN
	case SEQ_STEP_SFR_WIDE_MOVE_CAMERA_CYLINDER_DOWN:
	{
		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_1E_SOCKET_UP, FALSE);
		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_1F_SOCKET_DOWN, TRUE);

		SetStep(SEQ_STEP_SFR_WIDE_MOVE_CAMERA_CYLINDER_DOWN_CHK);
	}
	break;

	// CYLINDER DOWN CHK
	case SEQ_STEP_SFR_WIDE_MOVE_CAMERA_CYLINDER_DOWN_CHK:
	{
		if (CIO::IO_I_Socket_Down() == true)
		{
			SetStep(SEQ_STEP_SFR_WIDE_MOVE_SOCKET_CYLINDER_OUT);
		}
		else if (GetElapTime() > TIMEOUT_SENSOR_5000)
		{
			err = MCEC_Io_Socket_Down_Timeout;
		}
	}
	break;

	case SEQ_STEP_SFR_WIDE_MOVE_SOCKET_CYLINDER_OUT:
	{
		CIO::IO_O_Socket_Out(false);

		SetStep(SEQ_STEP_SFR_WIDE_MOVE_SOCKET_CYLINDER_OUT_CHK);
	}
	break;
	case SEQ_STEP_SFR_WIDE_MOVE_SOCKET_CYLINDER_OUT_CHK:
	{
		if (CIO::IO_I_Socket_Out() == true)
		{
			SetStep(SEQ_STEP_SFR_WIDE_UNLOAD_MOVE_SOCKET_CYLINDER_UP);
		}
		else if (GetElapTime() > TIMEOUT_SENSOR_5000)
		{
			err = MCEC_Io_Socket_Out_Timeout;
		}
	}
	break;
	case SEQ_STEP_SFR_WIDE_UNLOAD_MOVE_SOCKET_CYLINDER_UP:
	{
		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_1F_SOCKET_DOWN, FALSE);
		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_1E_SOCKET_UP, TRUE);

		SetStep(SEQ_STEP_SFR_WIDE_UNLOAD_MOVE_SOCKET_CYLINDER_UP_CHK);
	}
	break;
	case SEQ_STEP_SFR_WIDE_UNLOAD_MOVE_SOCKET_CYLINDER_UP_CHK:
	{
		AJIN_IO->ChangeBtnStatus(DIO_OUTPUT_Y0_02_STOP_SWITCH_LAMP, 0, TRUE);

		if (CIO::IO_I_Socket_Up() == true)
		{
			SetStep(SEQ_STEP_SFR_WIDE_WAIT_STOP_SWITCH_PUSH);
		}
		else if (GetElapTime() > TIMEOUT_SENSOR_5000)
		{
			err = MCEC_Io_Socket_Up_Timeout;			
		}
	}
	break;

	case SEQ_STEP_SFR_WIDE_WAIT_STOP_SWITCH_PUSH:
	{
#ifndef _DRY_RUN_MODULE_NONE
		if (GetStopSwtichMode() == TRUE || COMMON->GetAreaStop_CancelPush() == TRUE)
#endif
		{
			AJIN_IO->BlinkStop(DIO_OUTPUT_Y0_01_START_SWITCH_LAMP, TRUE);
			AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_01_START_SWITCH_LAMP, FALSE);
			AJIN_IO->BlinkStop(DIO_OUTPUT_Y0_02_STOP_SWITCH_LAMP, TRUE);
			AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_02_STOP_SWITCH_LAMP, FALSE);
			COMMON->SetReplaceFlag(TRUE);

			SetStep(SEQ_STEP_SFR_WIDE_CYCLE_COMPLEATE);
		}
	}
	break;

	case SEQ_STEP_SFR_WIDE_CYCLE_COMPLEATE:
	{
#ifndef _DRY_RUN_MODULE_NONE
		if (COMMON->GetAreaStop_CancelPush() == TRUE)
			m_bFlagProcess_AreaStopFinish = TRUE;
		else
			m_bFlagProcess_NormalFinish = TRUE;

		COMMON->SetProbePinCylinder_UpDown(_CYLINDER_DOWN);
		COMMON->m_bMainUiFlag_StepView[10] = TRUE;
		m_UdpTester[0].Reset_ReadyFlag();

		SetStep(SEQ_STEP_SFR_WIDE_READY);
#else
		COMMON->SetProbePinCylinder_UpDown(_CYLINDER_DOWN);
		m_bFlagProcess_NormalFinish = TRUE;

		SetStep(SEQ_STEP_SFR_WIDE_DRYRUN_MODE);
#endif
	}
	break;
	case SEQ_STEP_SFR_WIDE_DRYRUN_MODE:
	{
		if (GetElapTime() < TIMEOUT_DELAY_500)
			break;

		SetStep(SEQ_STEP_SFR_WIDE_READY);
		break;
	}

	}
	return (UINT)err;
}

UINT CSeqOqcMulti::Maintence(__in enMaintFunId id)
{
	auto err = 0;
	switch (id)
	{
	case enMaintFunId::Load:

		// fix off
		if (CIO::IO_O_Camera_Fix_Cylinder_Off() == false)
		{
			err = MCEC_Io_CameraFix_Off_Timeout;
			break;
		}

		// socket dn
		if (CIO::IO_O_Socket_Down() == false)
		{
			err = MCEC_Io_Socket_Down_Timeout;
			break;
		}

		// socket in
		if (CIO::IO_O_Socket_In() == false)
		{
			err = MCEC_Io_Socket_In_Timeout;
			break;
		}

		// socket up
		if (CIO::IO_O_Socket_Up() == false)
		{
			err = MCEC_Io_Socket_Up_Timeout;
			break;
		}

		// 1sec delay
		Sleep(1000);

		// camera fix on
		if (CIO::IO_I_DetectCamera() == true)
		{
			if (CIO::IO_O_Camera_Fix_Cylinder_On() == false)
			{
				err = MCEC_Io_CameraFix_On_Timeout;
				break;
			}
		}		

		// probe up
		if (CIO::IO_O_ProbePin_Up() == false)
		{
			err = MCEC_Io_ProbePinUp_Timeout;
			break;
		}
		break;

	case enMaintFunId::Unload:

		if (CIO::IO_I_DetectCamera() == true)
		{
			// camera fix off
			if (CIO::IO_O_Camera_Fix_Cylinder_Off() == false)
			{
				err = MCEC_Io_CameraFix_Off_Timeout;
				break;
			}

			// probe dn
			if (CIO::IO_O_ProbePin_Down() == false)
			{
				err = MCEC_Io_ProbePinDn_Timeout;
				break;
			}

			// socket dn
			if (CIO::IO_O_Socket_Down() == false)
			{
				err = MCEC_Io_Socket_Down_Timeout;
				break;
			}
		}
		else
		{
			// probe dn
			if (CIO::IO_O_ProbePin_Down() == false)
			{
				err = MCEC_Io_ProbePinDn_Timeout;
				break;
			}

			// socket dn
			if (CIO::IO_O_Socket_Down() == false)
			{
				err = MCEC_Io_Socket_Down_Timeout;
				break;
			}

			// camera fix off
			if (CIO::IO_O_Camera_Fix_Cylinder_Off() == false)
			{
				err = MCEC_Io_CameraFix_Off_Timeout;
				break;
			}
		}		

		// socket out
		if (CIO::IO_O_Socket_Out() == false)
		{
			err = MCEC_Io_Socket_Out_Timeout;
			break;
		}

		// socket up
		if (CIO::IO_O_Socket_Up() == false)
		{
			err = MCEC_Io_Socket_Up_Timeout;
			break;
		}

		break;

	default:
		break;
	}

	return err;
}

void CSeqOqcMulti::SetInspectionStep()
{
	SetStep(SEQ_STEP_SFR_WIDE_SEND_INSPECTION_PREVIEW_STOP);
}

void CSeqOqcMulti::ResetStepFlag()
{
	SetStep(SEQ_STEP_SFR_WIDE_READY);

	m_nStep_SFR_MoveWaitPos = SEQ_STEP_SFR_MOVEBACK_STEP001_CHECK_BACK_COMMAND;
	m_bFlagProcess_NormalFinish = FALSE;
}

void CSeqOqcMulti::RestartStep()
{
	switch (GetStep())
	{
	case SEQ_STEP_SFR_WIDE_MOVE_SOCKET_CYLINDER_DOWN:
	case SEQ_STEP_SFR_WIDE_MOVE_SOCKET_CYLINDER_DOWN_CHK:
		SetStep(SEQ_STEP_SFR_WIDE_MOVE_SOCKET_CYLINDER_DOWN);
		break;

	case SEQ_STEP_SFR_WIDE_MOVE_SOCKET_CYLINDER_IN:
	case SEQ_STEP_SFR_WIDE_MOVE_SOCKET_CYLINDER_IN_CHK:
		SetStep(SEQ_STEP_SFR_WIDE_MOVE_SOCKET_CYLINDER_IN);
		break;

	case SEQ_STEP_SFR_WIDE_MOVE_SOCKET_CYLINDER_UP:
	case SEQ_STEP_SFR_WIDE_MOVE_SOCKET_CYLINDER_UP_CHK:
		SetStep(SEQ_STEP_SFR_WIDE_MOVE_SOCKET_CYLINDER_UP);
		break;

	case SEQ_STEP_SFR_WIDE_MOVE_CAMERA_FIX_CYLINDER_ON:
	case SEQ_STEP_SFR_WIDE_MOVE_CAMERA_FIX_CYLINDER_ON_CHK:
		SetStep(SEQ_STEP_SFR_WIDE_MOVE_CAMERA_FIX_CYLINDER_ON);
		break;

	case SEQ_STEP_SFR_WIDE_MOVE_PROBE_CYLINDER_UP:
	case SEQ_STEP_SFR_WIDE_MOVE_PROBE_CYLINDER_UP_CHK:
		SetStep(SEQ_STEP_SFR_WIDE_MOVE_PROBE_CYLINDER_UP);
		break;

	case SEQ_STEP_SFR_WIDE_MOVE_Z_ZERO_2:
	case SEQ_STEP_SFR_WIDE_MOVE_Z_ZERO_2_CHK:
		SetStep(SEQ_STEP_SFR_WIDE_MOVE_Z_ZERO_2);
		break;

	case SEQ_STEP_SFR_WIDE_MOVE_CYLINDER_UP_Y_POS:
	case SEQ_STEP_SFR_WIDE_MOVE_CYLINDER_UP_Y_POS_CHK:
		SetStep(SEQ_STEP_SFR_WIDE_MOVE_CYLINDER_UP_Y_POS);
		break;

	case SEQ_STEP_SFR_WIDE_MOVE_SFR_Y_POS:
	case SEQ_STEP_SFR_WIDE_MOVE_SFR_Y_POS_CHK:
		SetStep(SEQ_STEP_SFR_WIDE_MOVE_SFR_Y_POS);
		break;

	case SEQ_STEP_SFR_WIDE_MOVE_SFR_Z_POS:
	case SEQ_STEP_SFR_WIDE_MOVE_SFR_Z_POS_CHK:
		SetStep(SEQ_STEP_SFR_WIDE_MOVE_SFR_Z_POS);
		break;

	case SEQ_STEP_SFR_WIDE_SEND_INSPECTION_PREVIEW_RESTART:
	case SEQ_STEP_SFR_WIDE_SEND_INSPECTION_PREVIEW_START_CHK:
		SetStep(SEQ_STEP_SFR_WIDE_SEND_INSPECTION_PREVIEW_RESTART);
		break;

	case SEQ_STEP_SFR_WIDE_SEND_INSEPCTION_SFR:
	case SEQ_STEP_SFR_WIDE_SEND_INSEPCTION_SFR_WAIT_RECIVE:
		SetStep(SEQ_STEP_SFR_WIDE_SEND_INSEPCTION_SFR);
		break;

	case SEQ_STEP_SFR_WIDE_MOVE_Z_ZERO_1:
	case SEQ_STEP_SFR_WIDE_MOVE_Z_ZERO_1_CHK:
		SetStep(SEQ_STEP_SFR_WIDE_MOVE_Z_ZERO_1);
		break;

	case SEQ_STEP_SFR_WIDE_MOVE_BLEMISHI_Y_POS:
	case SEQ_STEP_SFR_WIDE_MOVE_BLEMISHI_Y_POS_CHK:
		SetStep(SEQ_STEP_SFR_WIDE_MOVE_BLEMISHI_Y_POS);
		break;

	case SEQ_STEP_SFR_WIDE_MOVE_BLEMISHI_Z_POS:
	case SEQ_STEP_SFR_WIDE_MOVE_BLEMISHI_Z_POS_CHK:
		SetStep(SEQ_STEP_SFR_WIDE_MOVE_BLEMISHI_Z_POS);
		break;

	case SEQ_STEP_SFR_WIDE_SEND_INSEPCTION_BLEMISHI:
	case SEQ_STEP_SFR_WIDE_SEND_INSEPCTION_BLEMISHI_WAIT_RECIVE:
		SetStep(SEQ_STEP_SFR_WIDE_SEND_INSEPCTION_BLEMISHI);
		break;

	case SEQ_STEP_SFR_WIDE_MOVE_Z_ZERO_3:
	case SEQ_STEP_SFR_WIDE_MOVE_Z_ZERO_3_CHK:
		SetStep(SEQ_STEP_SFR_WIDE_MOVE_Z_ZERO_3);
		break;

	case SEQ_STEP_SFR_WIDE_MOVE_Y_WAIT_POS:
	case SEQ_STEP_SFR_WIDE_MOVE_Y_WAIT_POS_CHK:
		SetStep(SEQ_STEP_SFR_WIDE_MOVE_Y_WAIT_POS);
		break;

	case SEQ_STEP_SFR_WIDE_MOVE_Z_WAIT_POS:
	case SEQ_STEP_SFR_WIDE_MOVE_Z_WAIT_POS_CHK:
		SetStep(SEQ_STEP_SFR_WIDE_MOVE_Z_WAIT_POS);
		break;

	case SEQ_STEP_SFR_WIDE_MOVE_CAMERA_FIX_CYLINDER_OFF:
	case SEQ_STEP_SFR_WIDE_MOVE_CAMERA_FIX_CYLINDER_OFF_CHK:
		SetStep(SEQ_STEP_SFR_WIDE_MOVE_CAMERA_FIX_CYLINDER_OFF);
		break;

	case SEQ_STEP_SFR_WIDE_MOVE_CAMERA_CYLINDER_DOWN:
	case SEQ_STEP_SFR_WIDE_MOVE_CAMERA_CYLINDER_DOWN_CHK:		
		SetStep(SEQ_STEP_SFR_WIDE_MOVE_CAMERA_CYLINDER_DOWN);
		break;

	case SEQ_STEP_SFR_WIDE_MOVE_SOCKET_CYLINDER_OUT:
	case SEQ_STEP_SFR_WIDE_MOVE_SOCKET_CYLINDER_OUT_CHK:
		SetStep(SEQ_STEP_SFR_WIDE_MOVE_SOCKET_CYLINDER_OUT);
		break;

	case SEQ_STEP_SFR_WIDE_UNLOAD_MOVE_SOCKET_CYLINDER_UP:
	case SEQ_STEP_SFR_WIDE_UNLOAD_MOVE_SOCKET_CYLINDER_UP_CHK:
		SetStep(SEQ_STEP_SFR_WIDE_UNLOAD_MOVE_SOCKET_CYLINDER_UP);
		break;
	
	default:
		break;
	}
}

bool CSeqOqcMulti::ReadyToStart()
{
	auto bRet = false;

	bRet = CIO::IO_I_DetectCamera();
	return bRet;
}

bool CSeqOqcMulti::IsStep_Ready()
{
	return GetStep() == SEQ_STEP_SFR_WIDE_READY;
}

bool CSeqOqcMulti::IsStep_WaitForPushSwitch()
{
	return GetStep() == SEQ_STEP_SFR_WIDE_WAIT_STOP_SWITCH_PUSH;
}

bool CSeqOqcMulti::IsStep_UnderMoveY()
{
	return GetStep() < SEQ_STEP_SFR_WIDE_MOVE_SFR_Y_POS;
}

bool CSeqOqcMulti::IsStep_BeforePushStopSwitch()
{
	return GetStep() < SEQ_STEP_SFR_WIDE_WAIT_STOP_SWITCH_PUSH;
}

bool CSeqOqcMulti::IsStep_NotStartupModeWait()
{
	return GetStep() != SEQ_STEP_SFR_WIDE_STARTUPMODE_ACTION_WAIT;
}

enMCErrCode CSeqOqcMulti::CheckPowerVoltage()
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

void CSeqOqcMulti::SetPowerVoltage()
{
	float fVolt = 0.0;
	m_stInspectionInfo.fPowerVoltage = 0.0f;

	Sleep(1500);
	CDevice::POWERSUPPLY_MeasureVolt(fVolt);
	m_stInspectionInfo.SetPowerVoltage(fVolt);
};

float CSeqOqcMulti::GetPowerVoltage()
{
	return m_stInspectionInfo.GetPowerVoltage();
};
#endif