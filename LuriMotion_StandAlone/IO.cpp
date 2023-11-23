#include "pch.h"
#include "IO.h"


#define WaitForDetectTimeout 4000

static bool g_bBlinkResetBtn;
static bool g_bBlinkInitializeBtn;

CIO::CIO()
{
	g_bBlinkResetBtn = false;
	g_bBlinkInitializeBtn = false;
}

CIO::~CIO()
{
}

void CIO::DoEvents()
{
	MSG msg;

	// window message         
	while (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void CIO::DoEvents(DWORD dwMiliSeconds)
{
	clock_t start_tm = clock();
	do
	{
		DoEvents();
		::Sleep(1);
	} while ((DWORD)(clock() - start_tm) < dwMiliSeconds);
}

bool CIO::WaitStatus(__in int OnPort, __in int OffPort)
{
	bool bRet = false;

	clock_t startm = clock();

	do
	{
		if (AJIN_IO->GetInputIoBit(OnPort) == TRUE 
			&& AJIN_IO->GetInputIoBit(OffPort) == FALSE)
		{
			bRet = true;
			break;
		}

		DoEvents(1);
	} while ((clock() - startm) < WaitForDetectTimeout);


	return bRet;
}

bool CIO::WaitStatus(__in int Port, __in bool Status)
{
	bool bRet = false;

	clock_t startm = clock();

	do
	{
		if (AJIN_IO->GetInputIoBit(Port) == (BOOL)Status)
		{
			bRet = true;
			break;
		}

		DoEvents(1);
	} while ((clock() - startm) < WaitForDetectTimeout);


	return bRet;
}

bool CIO::IO_I_WaitStatus(__in int idx, __in bool bStatus, __in DWORD dwMilisecond)
{
	bool bRet = false;

	clock_t startm = clock();

	do
	{
		if (AJIN_IO->GetInputIoBit(idx) == (BOOL)bStatus)
		{
			bRet = true;
			break;
		}

		DoEvents(10);
	} while ((clock() - startm) < dwMilisecond);

	return bRet;
}

bool CIO::IO_I_AREA_SENSOR()
{
	return AJIN_IO->GetInputIoBit(DIO_INPUT_X0_06_AREASENSOR1);
}

bool CIO::IO_I_EMO()
{
	return AJIN_IO->GetInputIoBit(DIO_INPUT_X0_04_EMO);
}

bool CIO::IO_I_PowerOff()
{
	return AJIN_IO->GetInputIoBit(DIO_INPUT_X0_00_POWER);
}

bool CIO::IO_I_INIT()
{
	return AJIN_IO->GetInputIoBit(DIO_INPUT_X0_03_INIT);
}

bool CIO::IO_I_MainAirPress()
{
	return AJIN_IO->GetInputIoBit(DIO_INPUT_X0_05_MAINAIR);
}

bool CIO::IO_I_SleepMode()
{
#if(GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL) || (GET_INSPECTOR == SYS_OQC_SFR_SINGLE_CL) || (GET_INSPECTOR == SYS_DISTORTION_30)
	return AJIN_IO->GetInputIoBit(DIO_INPUT_X0_0B_SLEEPMODE_ON_SENSOR);
#elif(GET_INSPECTOR == SYS_FILM_PEEL_OFF)
	return AJIN_IO->GetInputIoBit(DIO_INPUT_X0_11_SLEEPMODE_ON_SENSOR);
#endif
}

void CIO::IO_O_SleepMode(__in bool bOn)
{
#if(GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL) || (GET_INSPECTOR == SYS_OQC_SFR_SINGLE_CL) || (GET_INSPECTOR == SYS_DISTORTION_30)
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_0B_SLEEP_MODE_ON, bOn);
#elif(GET_INSPECTOR == SYS_FILM_PEEL_OFF)
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_11_SLEEP_MODE_ON, bOn);
#endif
}

void CIO::IO_O_Buzzer(__in bool bOn)
{
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_09_TOWERLAMP_BEEP1, bOn);
}

void CIO::IO_O_RESET(__in bool bOn)
{
	if (bOn)
	{
		if (!g_bBlinkResetBtn)
		{
			AJIN_IO->BlinkStart(DIO_OUTPUT_Y0_04_RESET_SWITCH_LAMP, TRUE, 500, 500);
			g_bBlinkResetBtn = true;
		}
	}
	else
	{
		g_bBlinkResetBtn = false;
		AJIN_IO->BlinkStop(DIO_OUTPUT_Y0_04_RESET_SWITCH_LAMP, FALSE);
	}
}

void CIO::IO_O_INIT(__in bool bOn)
{
#if(GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL) || (GET_INSPECTOR == SYS_OQC_SFR_SINGLE_CL) || (GET_INSPECTOR == SYS_FILM_PEEL_OFF)	
	if (bOn)
	{
		if (!g_bBlinkInitializeBtn)
		{
			AJIN_IO->BlinkStart(DIO_OUTPUT_Y0_03_INIT_SWITCH_LAMP, TRUE, 500, 500);
			g_bBlinkInitializeBtn = true;
		}
	}
	else
	{
		g_bBlinkInitializeBtn = false;
		AJIN_IO->BlinkStop(DIO_OUTPUT_Y0_03_INIT_SWITCH_LAMP, FALSE);
	}
#endif
}

bool CIO::IO_I_Socket_In()
{
#if (GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL) || (GET_INSPECTOR == SYS_OQC_SFR_SINGLE_CL)
	return (AJIN_IO->GetInputIoBit(DIO_INPUT_X0_1C_SOCKET_IN) == TRUE
		&& AJIN_IO->GetInputIoBit(DIO_INPUT_X0_1D_SOCKET_OUT) == FALSE);
	return true;
#endif
}

bool CIO::IO_I_Socket_Out()
{
#if (GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL) || (GET_INSPECTOR == SYS_OQC_SFR_SINGLE_CL)
	return (AJIN_IO->GetInputIoBit(DIO_INPUT_X0_1C_SOCKET_IN) == FALSE
		&& AJIN_IO->GetInputIoBit(DIO_INPUT_X0_1D_SOCKET_OUT) == TRUE);
#else
	return true;
#endif
}

bool CIO::IO_I_Socket_Up()
{
#if (GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL) || (GET_INSPECTOR == SYS_OQC_SFR_SINGLE_CL)
	return (AJIN_IO->GetInputIoBit(DIO_INPUT_X0_1E_SOCKET_UP) == TRUE
		&& AJIN_IO->GetInputIoBit(DIO_INPUT_X0_1F_SOCKET_DOWN) == FALSE);
#else
	return true;
#endif
}

bool CIO::IO_I_Socket_Down()
{
#if (GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL) || (GET_INSPECTOR == SYS_OQC_SFR_SINGLE_CL)
	return (AJIN_IO->GetInputIoBit(DIO_INPUT_X0_1E_SOCKET_UP) == FALSE
		&& AJIN_IO->GetInputIoBit(DIO_INPUT_X0_1F_SOCKET_DOWN) == TRUE);
#else
	return true;
#endif
}

bool CIO::IO_I_Camera_Fix_Cylinder_On()
{
#if(GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL) || (GET_INSPECTOR == SYS_OQC_SFR_SINGLE_CL) || (GET_INSPECTOR == SYS_DISTORTION_30)
	if (GET_INSPECTOR == SYS_DISTORTION_30) 
	{
		Sleep(500);
		return true;
	}
	else
	{
		return (AJIN_IO->GetInputIoBit(DIO_INPUT_X0_0C_CAMERA_FIX_CYL_ON) == TRUE
			&& AJIN_IO->GetInputIoBit(DIO_INPUT_X0_0D_CAMERA_FIX_CYL_OFF) == FALSE);
	}
#elif(GET_INSPECTOR == SYS_FILM_PEEL_OFF)
	return true;
#endif
}

bool CIO::IO_I_Camera_Fix_Cylinder_Off()
{
#if(GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL) || (GET_INSPECTOR == SYS_OQC_SFR_SINGLE_CL) || (GET_INSPECTOR == SYS_DISTORTION_30)
	return (AJIN_IO->GetInputIoBit(DIO_INPUT_X0_0C_CAMERA_FIX_CYL_ON) == FALSE
		&& AJIN_IO->GetInputIoBit(DIO_INPUT_X0_0D_CAMERA_FIX_CYL_OFF) == TRUE);
#elif(GET_INSPECTOR == SYS_FILM_PEEL_OFF)
	return true;
#endif
}

bool CIO::IO_I_ProbePin_Up()
{
#if(GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL) || (GET_INSPECTOR == SYS_OQC_SFR_SINGLE_CL) || (GET_INSPECTOR == SYS_DISTORTION_30)
	return (AJIN_IO->GetInputIoBit(DIO_INPUT_X0_0E_PROBEPIN_UP) == TRUE
		&& AJIN_IO->GetInputIoBit(DIO_INPUT_X0_0F_PROBEPIN_DOWN) == FALSE);
#elif(GET_INSPECTOR == SYS_FILM_PEEL_OFF)
	return true;
#endif
}

bool CIO::IO_I_ProbePin_Down()
{
#if(GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL) || (GET_INSPECTOR == SYS_OQC_SFR_SINGLE_CL) || (GET_INSPECTOR == SYS_DISTORTION_30)
	return (AJIN_IO->GetInputIoBit(DIO_INPUT_X0_0E_PROBEPIN_UP) == FALSE
		&& AJIN_IO->GetInputIoBit(DIO_INPUT_X0_0F_PROBEPIN_DOWN) == TRUE);
#elif(GET_INSPECTOR == SYS_FILM_PEEL_OFF)
	return true;
#endif
}

bool CIO::IO_I_DetectCamera()
{
	auto bRet = false;

#if(GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL)
	bRet = AJIN_IO->GetInputIoBit(DIO_INPUT_X0_08_CAMERA_DETECT_SENSOR);
#elif(GET_INSPECTOR == SYS_OQC_SFR_SINGLE_CL)
	bRet = AJIN_IO->GetInputIoBit(DIO_INPUT_X0_08_CAMERA_DETECT_SENSOR);
#elif(GET_INSPECTOR == SYS_DISTORTION_30)
	bRet = true;
#elif(GET_INSPECTOR == SYS_FILM_PEEL_OFF)
	bRet = true;
#endif

	return bRet;
}

bool CIO::IO_I_SocketCoverOpen()
{
	auto bRet = false;

#if(GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL)

#elif(GET_INSPECTOR == SYS_OQC_SFR_SINGLE_CL)
	bRet = AJIN_IO->GetInputIoBit(DIO_INPUT_X0_1B_SOCKET_COVER_OPEN);
#elif(GET_INSPECTOR == SYS_DISTORTION_30)
	if (AJIN_IO->GetInputIoBit(DIO_INPUT_X0_08_MANUALSOCEKT_OPENCLOSE1) && AJIN_IO->GetInputIoBit(DIO_INPUT_X0_09_MANUALSOCEKT_OPENCLOSE2))
		bRet = false;
	else
		bRet = true;
#elif(GET_INSPECTOR == SYS_FILM_PEEL_OFF)
	bRet = true;
#endif

	return bRet;
}

bool CIO::IO_O_Socket_In(bool bCheckStatus)
{
#if (GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL) || (GET_INSPECTOR == SYS_OQC_SFR_SINGLE_CL)
	auto bRet = false;

	if (IO_I_Socket_Up())
		return bRet;

	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_1D_SOCKET_OUT, FALSE);
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_1C_SOCKET_IN, TRUE);

	auto bOn = DIO_INPUT_X0_1C_SOCKET_IN;
	auto bOff = DIO_INPUT_X0_1D_SOCKET_OUT;

	if (bCheckStatus == true)
	{
		bRet = WaitStatus(bOn, bOff);
		Sleep(500);
	}
	else
	{
		bRet = true;
	}
	return bRet;
#else
	return true;
#endif
}

bool CIO::IO_O_Socket_Out(bool bCheckStatus)
{
#if (GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL) || (GET_INSPECTOR == SYS_OQC_SFR_SINGLE_CL)
	auto bRet = false;

	if (IO_I_Socket_Up())
		return bRet;

	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_1C_SOCKET_IN, FALSE);
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_1D_SOCKET_OUT, TRUE);

	auto bOn = DIO_INPUT_X0_1D_SOCKET_OUT;
	auto bOff = DIO_INPUT_X0_1C_SOCKET_IN;

	if (bCheckStatus == true)
	{
		bRet = WaitStatus(bOn, bOff);
		Sleep(500);
	}
	else
	{
		bRet = true;
	}
	return bRet;
#else
	return true;
#endif
}

bool CIO::IO_O_Socket_Up(bool bCheckStatus)
{
#if (GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL) || (GET_INSPECTOR == SYS_OQC_SFR_SINGLE_CL)
	auto bRet = false;

	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_1F_SOCKET_DOWN, FALSE);
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_1E_SOCKET_UP, TRUE);

	auto bOn = DIO_INPUT_X0_1E_SOCKET_UP;
	auto bOff = DIO_INPUT_X0_1F_SOCKET_DOWN;

	if (bCheckStatus == true)
	{
		bRet = WaitStatus(bOn, bOff);
		Sleep(500);
	}
	else
	{
		bRet = true;
	}
	return bRet;
#else
	return true;
#endif
}

bool CIO::IO_O_Socket_Down(bool bCheckStatus)
{
#if (GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL) || (GET_INSPECTOR == SYS_OQC_SFR_SINGLE_CL)
	auto bRet = false;

	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_1E_SOCKET_UP, FALSE);
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_1F_SOCKET_DOWN, TRUE);

	auto bOn = DIO_INPUT_X0_1F_SOCKET_DOWN;
	auto bOff = DIO_INPUT_X0_1E_SOCKET_UP;

	if (bCheckStatus == true)
	{
		bRet = WaitStatus(bOn, bOff);
		Sleep(500);
	}
	else
	{
		bRet = true;
	}
	return bRet;
#else
	return true;
#endif
}

bool CIO::IO_O_Camera_Fix_Cylinder_On(bool bCheckStatus)
{
	auto bRet = false;
#if(GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL) || (GET_INSPECTOR == SYS_OQC_SFR_SINGLE_CL) || (GET_INSPECTOR == SYS_DISTORTION_30)
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_0D_CAMERA_FIX_CYL_OFF, FALSE);
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_0C_CAMERA_FIX_CYL_ON, TRUE);

	auto bOn = DIO_INPUT_X0_0C_CAMERA_FIX_CYL_ON;
	auto bOff = DIO_INPUT_X0_0D_CAMERA_FIX_CYL_OFF;

	if (bCheckStatus == true)
	{
		if (GET_INSPECTOR == SYS_DISTORTION_30)
		{
			bRet = true;
			Sleep(500);
		}
		else
		{
			bRet = WaitStatus(bOn, bOff);
			Sleep(500);
		}		
	}
	else
	{
		bRet = true;
	}
#elif(GET_INSPECTOR == SYS_FILM_PEEL_OFF)
	bRet = true;
#endif
	return bRet;
}

bool CIO::IO_O_Camera_Fix_Cylinder_Off(bool bCheckStatus)
{
	auto bRet = false;
#if(GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL) || (GET_INSPECTOR == SYS_OQC_SFR_SINGLE_CL) || (GET_INSPECTOR == SYS_DISTORTION_30)
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_0C_CAMERA_FIX_CYL_ON, FALSE);
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_0D_CAMERA_FIX_CYL_OFF, TRUE);

	auto bOn = DIO_INPUT_X0_0D_CAMERA_FIX_CYL_OFF;
	auto bOff = DIO_INPUT_X0_0C_CAMERA_FIX_CYL_ON;

	if (bCheckStatus == true)
	{
		bRet = WaitStatus(bOn, bOff);
		Sleep(500);
	}
	else
	{
		bRet = true;
	}
#elif(GET_INSPECTOR == SYS_FILM_PEEL_OFF)
	bRet = true;
#endif
	return bRet;
}

bool CIO::IO_O_ProbePin_Up(bool bCheckStatus)
{
	auto bRet = false;
#if(GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL) || (GET_INSPECTOR == SYS_OQC_SFR_SINGLE_CL) || (GET_INSPECTOR == SYS_DISTORTION_30)
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_0F_PROBEPIN_CYLINDER_DOWN, FALSE);
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_0E_PROBEPIN_CYLINDER_UP, TRUE);

	auto bOn = DIO_INPUT_X0_0E_PROBEPIN_UP;
	auto bOff = DIO_INPUT_X0_0F_PROBEPIN_DOWN;

	if (bCheckStatus == true)
	{
		bRet = WaitStatus(bOn, bOff);
		Sleep(500);
	}
	else
	{
		bRet = true;
	}
#elif(GET_INSPECTOR == SYS_FILM_PEEL_OFF)
	bRet = true;
#endif
	return bRet;
}

bool CIO::IO_O_ProbePin_Down(bool bCheckStatus)
{
	auto bRet = false;
#if(GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL) || (GET_INSPECTOR == SYS_OQC_SFR_SINGLE_CL) || (GET_INSPECTOR == SYS_DISTORTION_30)
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_0E_PROBEPIN_CYLINDER_UP, FALSE);
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_0F_PROBEPIN_CYLINDER_DOWN, TRUE);

	auto bOn = DIO_INPUT_X0_0F_PROBEPIN_DOWN;
	auto bOff = DIO_INPUT_X0_0E_PROBEPIN_UP;

	if (bCheckStatus == true)
	{
		bRet = WaitStatus(bOn, bOff);
		Sleep(500);
	}
	else
	{
		bRet = true;
	}
#elif(GET_INSPECTOR == SYS_FILM_PEEL_OFF)
	bRet = true;
#endif
	return bRet;
}

bool CIO::MoveSocketToSaftyZone()
{
	auto bRet = false;
	bRet = IO_O_ProbePin_Down();
	bRet &= IO_O_Socket_Down();
	bRet &= IO_O_Socket_In();
#if (GET_INSPECTOR == SYS_FILM_PEEL_OFF)
	bRet &= IO_O_Socket_State_PCB_Gripper_Off();
	bRet &= IO_O_Socket_State_PCB_Gripper_Down();
	bRet &= IO_O_CoverPush_Gripper_Up();
	for (int i = 0; i < MAX_SIDE; i++) {
		bRet &= IO_O_Flim_Gripper_Back(i);
		bRet &= IO_O_Flim_Gripper_Off(i);
		bRet &= IO_O_Flim_Gripper_Cover_Off(i);
	}
	
#endif
	return bRet;
}

void CIO::IO_O_Muting(__in bool bOn)
{
#if (GET_INSPECTOR == SYS_FILM_PEEL_OFF)
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_09_TOWERLAMP_BEEP1, bOn);
#endif
}

bool CIO::IO_I_Socket_State_PCB_Gripper_On()
{
#if (GET_INSPECTOR == SYS_FILM_PEEL_OFF)
	return (AJIN_IO->GetInputIoBit(DIO_INPUT_X1_18_SOCKET_STAGE_UNIT_PCB_GRIPPER_CYLINDER_CLOSE_ON) == TRUE
		&& AJIN_IO->GetInputIoBit(DIO_INPUT_X1_19_SOCKET_STAGE_UNIT_PCB_GRIPPER_CYLINDER_OPEN_OFF) == FALSE);
	return true;
#endif
}

bool CIO::IO_I_Socket_State_PCB_Gripper_Off()
{
#if (GET_INSPECTOR == SYS_FILM_PEEL_OFF)
	return (AJIN_IO->GetInputIoBit(DIO_INPUT_X1_18_SOCKET_STAGE_UNIT_PCB_GRIPPER_CYLINDER_CLOSE_ON) == FALSE
		&& AJIN_IO->GetInputIoBit(DIO_INPUT_X1_19_SOCKET_STAGE_UNIT_PCB_GRIPPER_CYLINDER_OPEN_OFF) == TRUE);
#else
	return true;
#endif
}

bool CIO::IO_I_Socket_State_PCB_Gripper_Up()
{
#if (GET_INSPECTOR == SYS_FILM_PEEL_OFF)
	return (AJIN_IO->GetInputIoBit(DIO_INPUT_X1_20_SOCKET_STAGE_UNIT_PCB_GRIPPER_CYLINDER_UP) == TRUE
		&& AJIN_IO->GetInputIoBit(DIO_INPUT_X1_21_SOCKET_STAGE_UNIT_PCB_GRIPPER_CYLINDER_DOWN) == FALSE);
#else
	return true;
#endif
}

bool CIO::IO_I_Socket_State_PCB_Gripper_Down()
{
#if (GET_INSPECTOR == SYS_FILM_PEEL_OFF)
	return (AJIN_IO->GetInputIoBit(DIO_INPUT_X1_20_SOCKET_STAGE_UNIT_PCB_GRIPPER_CYLINDER_UP) == FALSE
		&& AJIN_IO->GetInputIoBit(DIO_INPUT_X1_21_SOCKET_STAGE_UNIT_PCB_GRIPPER_CYLINDER_DOWN) == TRUE);
#else
	return true;
#endif
}

bool CIO::IO_I_CoverPush_Gripper_Up()
{
#if (GET_INSPECTOR == SYS_FILM_PEEL_OFF)
	return (AJIN_IO->GetInputIoBit(DIO_INPUT_X1_30_LENS_COVER_PUSH_UNIT_UPDOWN_GRIPPER_CYLINDER_UP) == TRUE
		&& AJIN_IO->GetInputIoBit(DIO_INPUT_X1_31_LENS_COVER_PUSH_UNIT_UPDOWN_GRIPPER_CYLINDER_DOWN) == FALSE);
#else
	return true;
#endif
}

bool CIO::IO_I_CoverPush_Gripper_Down()
{
#if (GET_INSPECTOR == SYS_FILM_PEEL_OFF)
	return (AJIN_IO->GetInputIoBit(DIO_INPUT_X1_30_LENS_COVER_PUSH_UNIT_UPDOWN_GRIPPER_CYLINDER_UP) == FALSE
		&& AJIN_IO->GetInputIoBit(DIO_INPUT_X1_31_LENS_COVER_PUSH_UNIT_UPDOWN_GRIPPER_CYLINDER_DOWN) == TRUE);
#else
	return true;
#endif
}

bool CIO::IO_I_Flim_Gripper_For(__in int nSelect)
{
#if (GET_INSPECTOR == SYS_FILM_PEEL_OFF)
	if (nSelect == LEFT_SIDE) {
		return (AJIN_IO->GetInputIoBit(DIO_INPUT_X1_06_GRIPPER_UNIT_GRIPPER_FRONT_CYLINDER_LEFT) == TRUE
			&& AJIN_IO->GetInputIoBit(DIO_INPUT_X1_07_GRIPPER_UNIT_GRIPPER_REAR_CYLINDER_LEFT) == FALSE);
	}
	if (nSelect == RIGHT_SIDE) {
		return (AJIN_IO->GetInputIoBit(DIO_INPUT_X1_08_GRIPPER_UNIT_GRIPPER_FRONT_CYLINDER_RIGHT) == TRUE
			&& AJIN_IO->GetInputIoBit(DIO_INPUT_X1_09_GRIPPER_UNIT_GRIPPER_REAR_CYLINDER_RIGHT) == FALSE);
	}
#else
	return true;
#endif
}

bool CIO::IO_I_Flim_Gripper_Back(__in int nSelect)
{
#if (GET_INSPECTOR == SYS_FILM_PEEL_OFF)
	if (nSelect == LEFT_SIDE) {
		return (AJIN_IO->GetInputIoBit(DIO_INPUT_X1_06_GRIPPER_UNIT_GRIPPER_FRONT_CYLINDER_LEFT) == FALSE
			&& AJIN_IO->GetInputIoBit(DIO_INPUT_X1_07_GRIPPER_UNIT_GRIPPER_REAR_CYLINDER_LEFT) == TRUE);
	}
	if (nSelect == RIGHT_SIDE) {
		return (AJIN_IO->GetInputIoBit(DIO_INPUT_X1_08_GRIPPER_UNIT_GRIPPER_FRONT_CYLINDER_RIGHT) == FALSE
			&& AJIN_IO->GetInputIoBit(DIO_INPUT_X1_09_GRIPPER_UNIT_GRIPPER_REAR_CYLINDER_RIGHT) == TRUE);
	}
#else
	return true;
#endif
}

bool CIO::IO_I_Flim_Gripper_On(__in int nSelect)
{
#if (GET_INSPECTOR == SYS_FILM_PEEL_OFF)
	if (nSelect == LEFT_SIDE) {
		return (AJIN_IO->GetInputIoBit(DIO_INPUT_X1_00_GRIPPER_UNIT_FILM_GRIPPER_CYLINDER_ON_LEFT) == TRUE
			&& AJIN_IO->GetInputIoBit(DIO_INPUT_X1_01_GRIPPER_UNIT_FILM_GRIPPER_CYLINDER_OFF_LEFT) == FALSE);
	}
	if (nSelect == RIGHT_SIDE) {
		return (AJIN_IO->GetInputIoBit(DIO_INPUT_X1_02_GRIPPER_UNIT_FILM_GRIPPER_CYLINDER_ON_RIGHT) == TRUE
			&& AJIN_IO->GetInputIoBit(DIO_INPUT_X1_03_GRIPPER_UNIT_FILM_GRIPPER_CYLINDER_OFF_RIGHT) == FALSE);
	}
#else
	return true;
#endif
}

bool CIO::IO_I_Flim_Gripper_Off(__in int nSelect)
{
#if (GET_INSPECTOR == SYS_FILM_PEEL_OFF)
	if (nSelect == LEFT_SIDE) {
		return (AJIN_IO->GetInputIoBit(DIO_INPUT_X1_06_GRIPPER_UNIT_GRIPPER_FRONT_CYLINDER_LEFT) == FALSE
			&& AJIN_IO->GetInputIoBit(DIO_INPUT_X1_07_GRIPPER_UNIT_GRIPPER_REAR_CYLINDER_LEFT) == TRUE);
	}
	if (nSelect == RIGHT_SIDE) {
		return (AJIN_IO->GetInputIoBit(DIO_INPUT_X1_08_GRIPPER_UNIT_GRIPPER_FRONT_CYLINDER_RIGHT) == FALSE
			&& AJIN_IO->GetInputIoBit(DIO_INPUT_X1_09_GRIPPER_UNIT_GRIPPER_REAR_CYLINDER_RIGHT) == TRUE);
	}
#else
	return true;
#endif
}

bool CIO::IO_I_Flim_Gripper_Cover_On(__in int nSelect)
{
#if (GET_INSPECTOR == SYS_FILM_PEEL_OFF)
	if (nSelect == LEFT_SIDE) {
		return (AJIN_IO->GetInputIoBit(DIO_INPUT_X1_12_GRIPPER_UNIT_COVER_GRIPPER_CYLINDER_ON_LEFT) == TRUE
			&& AJIN_IO->GetInputIoBit(DIO_INPUT_X1_13_GRIPPER_UNIT_COVER_GRIPPER_CYLINDER_OFF_LEFT) == FALSE);
	}
	if (nSelect == RIGHT_SIDE) {
		return (AJIN_IO->GetInputIoBit(DIO_INPUT_X1_14_GRIPPER_UNIT_COVER_GRIPPER_CYLINDER_ON_RIGHT) == TRUE
			&& AJIN_IO->GetInputIoBit(DIO_INPUT_X1_15_GRIPPER_UNIT_COVER_GRIPPER_CYLINDER_OFF_RIGHT) == FALSE);
	}
#else
	return true;
#endif
}

bool CIO::IO_I_Flim_Gripper_Cover_Off(__in int nSelect)
{
#if (GET_INSPECTOR == SYS_FILM_PEEL_OFF)
	if (nSelect == LEFT_SIDE) {
		return (AJIN_IO->GetInputIoBit(DIO_INPUT_X1_12_GRIPPER_UNIT_COVER_GRIPPER_CYLINDER_ON_LEFT) == FALSE
			&& AJIN_IO->GetInputIoBit(DIO_INPUT_X1_13_GRIPPER_UNIT_COVER_GRIPPER_CYLINDER_OFF_LEFT) == TRUE);
	}
	if (nSelect == RIGHT_SIDE) {
		return (AJIN_IO->GetInputIoBit(DIO_INPUT_X1_14_GRIPPER_UNIT_COVER_GRIPPER_CYLINDER_ON_RIGHT) == FALSE
			&& AJIN_IO->GetInputIoBit(DIO_INPUT_X1_15_GRIPPER_UNIT_COVER_GRIPPER_CYLINDER_OFF_RIGHT) == TRUE);
	}
#else
	return true;
#endif
}

bool CIO::IO_I_Flim_Gripper_Film_Box_Dtc(__in int nSelect)
{
#if (GET_INSPECTOR == SYS_FILM_PEEL_OFF)
	if (nSelect == LEFT_SIDE) {
		return AJIN_IO->GetInputIoBit(DIO_INPUT_X0_22_GRIPPER_UNIT_FILM_BOX_DETECT_SENSOR_LEFT);
	}
	if (nSelect == RIGHT_SIDE) {
		return AJIN_IO->GetInputIoBit(DIO_INPUT_X0_23_GRIPPER_UNIT_FILM_BOX_DETECT_SENSOR_RIGHT);
	}
#else
	return true;
#endif
}

bool CIO::IO_I_Socket_State_Film_Box_Dtc(__in int nSelect)
{
#if (GET_INSPECTOR == SYS_FILM_PEEL_OFF)
	if (nSelect == LEFT_SIDE) {
		return AJIN_IO->GetInputIoBit(DIO_INPUT_X0_26_SOCKET_STAGE_UNIT_PCB_FILM_DETECTOR_SENSOR_LEFT);
	}
	if (nSelect == RIGHT_SIDE) {
		return AJIN_IO->GetInputIoBit(DIO_INPUT_X0_27_SOCKET_STAGE_UNIT_PCB_FILM_DETECTOR_SENSOR_RIGHT);
	}
#else
	return true;
#endif
}

bool CIO::IO_I_Socket_State_Work_Zone_Dtc()
{
#if (GET_INSPECTOR == SYS_FILM_PEEL_OFF)
	return AJIN_IO->GetInputIoBit(DIO_INPUT_X0_29_SOCKET_STAGE_UNIT_WORK_ZONE_AREA_SENSOR) ;
#else
	return true;
#endif
}

bool CIO::IO_I_Socket_State_PCB_Dtc()
{
#if (GET_INSPECTOR == SYS_FILM_PEEL_OFF)
	return AJIN_IO->GetInputIoBit(DIO_INPUT_X0_31_SOCKET_STAGE_UNIT_PCB_DETECTOR_SENSOR);
#else
	return true;
#endif
}

bool CIO::IO_I_IZN10E_Ion(__in int nSelect)
{
#if (GET_INSPECTOR == SYS_FILM_PEEL_OFF)
	if (nSelect == LEFT_SIDE) {
		return AJIN_IO->GetInputIoBit(DIO_INPUT_X1_22_IONIZER_UNIT_ION_CHECK_LEFT);
	}
	if (nSelect == RIGHT_SIDE) {
		return AJIN_IO->GetInputIoBit(DIO_INPUT_X1_26_IONIZER_UNIT_ION_CHECK_RIGHT);
	}
#else
	return true;
#endif
}

bool CIO::IO_I_IZN10E_Error(__in int nSelect)
{
#if (GET_INSPECTOR == SYS_FILM_PEEL_OFF)
	if (nSelect == LEFT_SIDE) {
		return AJIN_IO->GetInputIoBit(DIO_INPUT_X1_23_IONIZER_UNIT_ERROR_CHECK_LEFT);
	}
	if (nSelect == RIGHT_SIDE) {
		return AJIN_IO->GetInputIoBit(DIO_INPUT_X1_27_IONIZER_UNIT_ERROR_CHECK_RIGHT);
	}
#else
	return true;
#endif
}

bool CIO::IO_I_IZN10E_Maintence(__in int nSelect)
{
#if (GET_INSPECTOR == SYS_FILM_PEEL_OFF)
	if (nSelect == LEFT_SIDE) {
		return AJIN_IO->GetInputIoBit(DIO_INPUT_X1_24_IONIZER_UNIT_MAINTENANCE_CHECK_LEFT);
	}
	if (nSelect == RIGHT_SIDE) {
		return AJIN_IO->GetInputIoBit(DIO_INPUT_X1_28_IONIZER_UNIT_MAINTENANCE_CHECK_RIGHT);
	}
#else
	return true;
#endif
}
bool CIO::IO_O_Socket_State_PCB_Gripper_On(bool bCheckStatus)
{
#if (GET_INSPECTOR == SYS_FILM_PEEL_OFF)
	auto bRet = false;

	//if (IO_I_Socket_State_PCB_Gripper_On())
	//	return bRet;

	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_19_SOCKET_STAGE_UNIT_PCB_GRIPPER_CYLINDER_OPEN_OFF, FALSE);
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_18_SOCKET_STAGE_UNIT_PCB_GRIPPER_CYLINDER_CLOSE_ON, TRUE);

	auto bOn = DIO_INPUT_X1_18_SOCKET_STAGE_UNIT_PCB_GRIPPER_CYLINDER_CLOSE_ON;
	auto bOff = DIO_INPUT_X1_19_SOCKET_STAGE_UNIT_PCB_GRIPPER_CYLINDER_OPEN_OFF;

	if (bCheckStatus == true)
	{
		bRet = WaitStatus(bOn, bOff);
		Sleep(500);
	}
	else
	{
		bRet = true;
	}
	return bRet;
#else
	return true;
#endif
}

bool CIO::IO_O_Socket_State_PCB_Gripper_Off(bool bCheckStatus)
{
#if (GET_INSPECTOR == SYS_FILM_PEEL_OFF)
	auto bRet = false;

	//if (IO_I_Socket_State_PCB_Gripper_Off())
	//	return bRet;

	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_18_SOCKET_STAGE_UNIT_PCB_GRIPPER_CYLINDER_CLOSE_ON, FALSE);
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_19_SOCKET_STAGE_UNIT_PCB_GRIPPER_CYLINDER_OPEN_OFF, TRUE);

	auto bOn = DIO_INPUT_X1_19_SOCKET_STAGE_UNIT_PCB_GRIPPER_CYLINDER_OPEN_OFF;
	auto bOff = DIO_INPUT_X1_18_SOCKET_STAGE_UNIT_PCB_GRIPPER_CYLINDER_CLOSE_ON;

	if (bCheckStatus == true)
	{
		bRet = WaitStatus(bOn, bOff);
		Sleep(500);
	}
	else
	{
		bRet = true;
	}
	return bRet;
#else
	return true;
#endif
}

bool CIO::IO_O_Socket_State_PCB_Gripper_Up(bool bCheckStatus)
{
#if (GET_INSPECTOR == SYS_FILM_PEEL_OFF)
	auto bRet = false;

	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_21_SOCKET_STAGE_UNIT_PCB_GRIPPER_CYLINDER_DOWN, FALSE);
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_20_SOCKET_STAGE_UNIT_PCB_GRIPPER_CYLINDER_UP, TRUE);

	auto bOn = DIO_INPUT_X1_20_SOCKET_STAGE_UNIT_PCB_GRIPPER_CYLINDER_UP;
	auto bOff = DIO_INPUT_X1_21_SOCKET_STAGE_UNIT_PCB_GRIPPER_CYLINDER_DOWN;

	if (bCheckStatus == true)
	{
		bRet = WaitStatus(bOn, bOff);
		Sleep(500);
	}
	else
	{
		bRet = true;
	}
	return bRet;
#else
	return true;
#endif
}

bool CIO::IO_O_Socket_State_PCB_Gripper_Down(bool bCheckStatus)
{
#if (GET_INSPECTOR == SYS_FILM_PEEL_OFF)
	auto bRet = false;

	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_20_SOCKET_STAGE_UNIT_PCB_GRIPPER_CYLINDER_UP, FALSE);
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_21_SOCKET_STAGE_UNIT_PCB_GRIPPER_CYLINDER_DOWN, TRUE);

	auto bOn = DIO_INPUT_X1_21_SOCKET_STAGE_UNIT_PCB_GRIPPER_CYLINDER_DOWN;
	auto bOff = DIO_INPUT_X1_20_SOCKET_STAGE_UNIT_PCB_GRIPPER_CYLINDER_UP;

	if (bCheckStatus == true)
	{
		bRet = WaitStatus(bOn, bOff);
		Sleep(500);
	}
	else
	{
		bRet = true;
	}
	return bRet;
#else
	return true;
#endif
}


bool CIO::IO_O_CoverPush_Gripper_Up(bool bCheckStatus)
{
	auto bRet = false;
#if (GET_INSPECTOR == SYS_FILM_PEEL_OFF)
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_31_LENS_COVER_PUSH_UNIT_UPDOWN_GRIPPER_CYLINDER_DOWN, FALSE);
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_30_LENS_COVER_PUSH_UNIT_UPDOWN_GRIPPER_CYLINDER_UP, TRUE);

	auto bOn = DIO_INPUT_X1_30_LENS_COVER_PUSH_UNIT_UPDOWN_GRIPPER_CYLINDER_UP;
	auto bOff = DIO_INPUT_X1_31_LENS_COVER_PUSH_UNIT_UPDOWN_GRIPPER_CYLINDER_DOWN;

	if (bCheckStatus == true)
	{
		bRet = WaitStatus(bOn, bOff);
		Sleep(500);
	}
	else
	{
		bRet = true;
	}
#else
	return true;
#endif
	return bRet;
}

bool CIO::IO_O_CoverPush_Gripper_Down(bool bCheckStatus)
{
	auto bRet = false;
#if (GET_INSPECTOR == SYS_FILM_PEEL_OFF)
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_30_LENS_COVER_PUSH_UNIT_UPDOWN_GRIPPER_CYLINDER_UP, FALSE);
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_31_LENS_COVER_PUSH_UNIT_UPDOWN_GRIPPER_CYLINDER_DOWN, TRUE);

	auto bOn = DIO_INPUT_X1_31_LENS_COVER_PUSH_UNIT_UPDOWN_GRIPPER_CYLINDER_DOWN;
	auto bOff = DIO_INPUT_X1_30_LENS_COVER_PUSH_UNIT_UPDOWN_GRIPPER_CYLINDER_UP;

	if (bCheckStatus == true)
	{
		bRet = WaitStatus(bOn, bOff);
		Sleep(500);
	}
	else
	{
		bRet = true;
	}
#else
	return true;
#endif
	return bRet;
}

bool CIO::IO_O_Flim_Gripper_For(__in int nSelect, bool bCheckStatus)
{
	auto bRet = false;
	auto bOn = 0;
	auto bOff = 0;
#if (GET_INSPECTOR == SYS_FILM_PEEL_OFF)
	if (nSelect == LEFT_SIDE) {
		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_06_GRIPPER_UNIT_GRIPPER_FRONT_CYLINDER_LEFT, TRUE);
		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_07_GRIPPER_UNIT_GRIPPER_REAR_CYLINDER_LEFT, FALSE);

		bOn = DIO_INPUT_X1_06_GRIPPER_UNIT_GRIPPER_FRONT_CYLINDER_LEFT;
		bOff = DIO_INPUT_X1_07_GRIPPER_UNIT_GRIPPER_REAR_CYLINDER_LEFT;
	}
	if (nSelect == RIGHT_SIDE) {
		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_08_GRIPPER_UNIT_GRIPPER_FRONT_CYLINDER_RIGHT, TRUE);
		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_09_GRIPPER_UNIT_GRIPPER_REAR_CYLINDER_RIGHT, FALSE);

		bOn = DIO_INPUT_X1_08_GRIPPER_UNIT_GRIPPER_FRONT_CYLINDER_RIGHT;
		bOff = DIO_INPUT_X1_09_GRIPPER_UNIT_GRIPPER_REAR_CYLINDER_RIGHT;
	}

	if (bCheckStatus == true)
	{
		bRet = WaitStatus(bOn, bOff);
		Sleep(500);
	}
	else
	{
		bRet = true;
	}
#else
	return true;
#endif
	return bRet;
}

bool CIO::IO_O_Flim_Gripper_Back(__in int nSelect, bool bCheckStatus)
{
	auto bRet = false;
	auto bOn  = 0;
	auto bOff = 0;
#if (GET_INSPECTOR == SYS_FILM_PEEL_OFF)
	if (nSelect == LEFT_SIDE) {
		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_06_GRIPPER_UNIT_GRIPPER_FRONT_CYLINDER_LEFT, FALSE);
		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_07_GRIPPER_UNIT_GRIPPER_REAR_CYLINDER_LEFT, TRUE);

		bOn = DIO_INPUT_X1_07_GRIPPER_UNIT_GRIPPER_REAR_CYLINDER_LEFT;
		bOff = DIO_INPUT_X1_06_GRIPPER_UNIT_GRIPPER_FRONT_CYLINDER_LEFT;
	}
	if (nSelect == RIGHT_SIDE) {
		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_08_GRIPPER_UNIT_GRIPPER_FRONT_CYLINDER_RIGHT, FALSE);
		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_09_GRIPPER_UNIT_GRIPPER_REAR_CYLINDER_RIGHT, TRUE);

		bOn = DIO_INPUT_X1_09_GRIPPER_UNIT_GRIPPER_REAR_CYLINDER_RIGHT;
		bOff = DIO_INPUT_X1_08_GRIPPER_UNIT_GRIPPER_FRONT_CYLINDER_RIGHT;
	}

	if (bCheckStatus == true)
	{
		bRet = WaitStatus(bOn, bOff);
		Sleep(500);
	}
	else
	{
		bRet = true;
	}
#else
	return true;
#endif
	return bRet;
}

bool CIO::IO_O_Flim_Gripper_On(__in int nSelect, bool bCheckStatus)
{
	auto bRet = false;
	auto bOn = 0;
	auto bOff = 0;
#if (GET_INSPECTOR == SYS_FILM_PEEL_OFF)
	if (nSelect == LEFT_SIDE) {
		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_00_GRIPPER_UNIT_FILM_GRIPPER_CYLINDER_ON_LEFT, TRUE);
		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_01_GRIPPER_UNIT_FILM_GRIPPER_CYLINDER_OFF_LEFT, FALSE);

		bOn = DIO_INPUT_X1_00_GRIPPER_UNIT_FILM_GRIPPER_CYLINDER_ON_LEFT;
		bOff = DIO_INPUT_X1_01_GRIPPER_UNIT_FILM_GRIPPER_CYLINDER_OFF_LEFT;
	}
	if (nSelect == RIGHT_SIDE) {
		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_02_GRIPPER_UNIT_FILM_GRIPPER_CYLINDER_ON_RIGHT, TRUE);
		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_03_GRIPPER_UNIT_FILM_GRIPPER_CYLINDER_OFF_RIGHT, FALSE);

		bOn = DIO_INPUT_X1_02_GRIPPER_UNIT_FILM_GRIPPER_CYLINDER_ON_RIGHT;
		bOff = DIO_INPUT_X1_03_GRIPPER_UNIT_FILM_GRIPPER_CYLINDER_OFF_RIGHT;
	}

	if (bCheckStatus == true)
	{
		bRet = WaitStatus(bOn, bOff);
		Sleep(500);
	}
	else
	{
		bRet = true;
	}
#else
	return true;
#endif
	return bRet;
}

bool CIO::IO_O_Flim_Gripper_Off(__in int nSelect, bool bCheckStatus)
{
	auto bRet = false;
	auto bOn = 0;
	auto bOff = 0;
#if (GET_INSPECTOR == SYS_FILM_PEEL_OFF)
	if (nSelect == LEFT_SIDE) {
		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_00_GRIPPER_UNIT_FILM_GRIPPER_CYLINDER_ON_LEFT, FALSE);
		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_01_GRIPPER_UNIT_FILM_GRIPPER_CYLINDER_OFF_LEFT, TRUE);

		bOn = DIO_INPUT_X1_01_GRIPPER_UNIT_FILM_GRIPPER_CYLINDER_OFF_LEFT;
		bOff = DIO_INPUT_X1_00_GRIPPER_UNIT_FILM_GRIPPER_CYLINDER_ON_LEFT;
	}
	if (nSelect == RIGHT_SIDE) {
		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_02_GRIPPER_UNIT_FILM_GRIPPER_CYLINDER_ON_RIGHT, FALSE);
		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_03_GRIPPER_UNIT_FILM_GRIPPER_CYLINDER_OFF_RIGHT, TRUE);

		bOn = DIO_INPUT_X1_03_GRIPPER_UNIT_FILM_GRIPPER_CYLINDER_OFF_RIGHT;
		bOff = DIO_INPUT_X1_02_GRIPPER_UNIT_FILM_GRIPPER_CYLINDER_ON_RIGHT;
	}

	if (bCheckStatus == true)
	{
		bRet = WaitStatus(bOn, bOff);
		Sleep(500);
	}
	else
	{
		bRet = true;
	}
#else
	return true;
#endif
	return bRet;
}

bool CIO::IO_O_Flim_Gripper_Cover_On(__in int nSelect, bool bCheckStatus)
{
	auto bRet = false;
	auto bOn = 0;
	auto bOff = 0;
#if (GET_INSPECTOR == SYS_FILM_PEEL_OFF)
	if (nSelect == LEFT_SIDE) {
		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_12_GRIPPER_UNIT_COVER_GRIPPER_CYLINDER_ON_LEFT, TRUE);
		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_13_GRIPPER_UNIT_COVER_GRIPPER_CYLINDER_OFF_LEFT, FALSE);

		bOn = DIO_INPUT_X1_12_GRIPPER_UNIT_COVER_GRIPPER_CYLINDER_ON_LEFT;
		bOff = DIO_INPUT_X1_13_GRIPPER_UNIT_COVER_GRIPPER_CYLINDER_OFF_LEFT;
	}
	if (nSelect == RIGHT_SIDE) {
		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_14_GRIPPER_UNIT_COVER_GRIPPER_CYLINDER_ON_RIGHT, TRUE);
		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_15_GRIPPER_UNIT_COVER_GRIPPER_CYLINDER_OFF_RIGHT, FALSE);

		bOn = DIO_INPUT_X1_14_GRIPPER_UNIT_COVER_GRIPPER_CYLINDER_ON_RIGHT;
		bOff = DIO_INPUT_X1_15_GRIPPER_UNIT_COVER_GRIPPER_CYLINDER_OFF_RIGHT;
	}

	if (bCheckStatus == true)
	{
		bRet = WaitStatus(bOn, bOff);
		Sleep(500);
	}
	else
	{
		bRet = true;
	}
#else
	return true;
#endif
	return bRet;
}

bool CIO::IO_O_Flim_Gripper_Cover_Off(__in int nSelect, bool bCheckStatus)
{
	auto bRet = false;
	auto bOn = 0;
	auto bOff = 0;
#if (GET_INSPECTOR == SYS_FILM_PEEL_OFF)
	if (nSelect == LEFT_SIDE) {
		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_12_GRIPPER_UNIT_COVER_GRIPPER_CYLINDER_ON_LEFT, FALSE);
		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_13_GRIPPER_UNIT_COVER_GRIPPER_CYLINDER_OFF_LEFT, TRUE);

		bOn = DIO_INPUT_X1_13_GRIPPER_UNIT_COVER_GRIPPER_CYLINDER_OFF_LEFT;
		bOff = DIO_INPUT_X1_12_GRIPPER_UNIT_COVER_GRIPPER_CYLINDER_ON_LEFT;
	}
	if (nSelect == RIGHT_SIDE) {
		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_14_GRIPPER_UNIT_COVER_GRIPPER_CYLINDER_ON_RIGHT, FALSE);
		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_15_GRIPPER_UNIT_COVER_GRIPPER_CYLINDER_OFF_RIGHT, TRUE);

		bOn = DIO_INPUT_X1_15_GRIPPER_UNIT_COVER_GRIPPER_CYLINDER_OFF_RIGHT;
		bOff = DIO_INPUT_X1_14_GRIPPER_UNIT_COVER_GRIPPER_CYLINDER_ON_RIGHT;
	}

	if (bCheckStatus == true)
	{
		bRet = WaitStatus(bOn, bOff);
		Sleep(500);
	}
	else
	{
		bRet = true;
	}
#else
	return true;
#endif
	return bRet;
}

void CIO::IO_O_Flim_Gripper_Air_Blow(__in int nSelect, __in bool bOn)
{
#if (GET_INSPECTOR == SYS_FILM_PEEL_OFF)
	if (nSelect == LEFT_SIDE) {
		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_22_GRIPPER_UNIT_AIR_BLOW_UNIT_LEFT, bOn);
	}
	if (nSelect == RIGHT_SIDE) {
		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_22_GRIPPER_UNIT_AIR_BLOW_UNIT_LEFT, bOn);
	}
#endif
}

void CIO::IO_O_IZN10E_Power(__in int nSelect, __in bool bOn)
{
#if (GET_INSPECTOR == SYS_FILM_PEEL_OFF)
	if (nSelect == LEFT_SIDE) {
		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_22_IONIZER_UNIT_POWER_ONOFF_LEFT, bOn);
	}
	if (nSelect == RIGHT_SIDE) {
		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_26_IONIZER_UNIT_POWER_ONOFF_RIGHT, bOn);
	}
#endif
}

void CIO::IO_O_IZN10E_Ion(__in int nSelect, __in bool bOn)
{
#if (GET_INSPECTOR == SYS_FILM_PEEL_OFF)
	if (nSelect == LEFT_SIDE) {
		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_23_IONIZER_UNIT_ION_ONOFF_LEFT, bOn);
	}
	if (nSelect == RIGHT_SIDE) {
		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_27_IONIZER_UNIT_ION_ONOFF_RIGHT, bOn);
	}
#endif
}

void CIO::IO_O_IZN10E_Reset(__in int nSelect, __in bool bOn)
{
#if (GET_INSPECTOR == SYS_FILM_PEEL_OFF)
	if (nSelect == LEFT_SIDE) {
		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_24_IONIZER_UNIT_RESET_LEFT, bOn);
	}
	if (nSelect == RIGHT_SIDE) {
		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_28_IONIZER_UNIT_RESET_RIGHT, bOn);
	}
#endif
}

// DOOR ALL LOCK
bool CIO::IO_Door_All_Lock(__in bool bLock)
{
	//enMCErrCode err = enMCErrCode::MCEC_OK;
	bool bOK = true;
#if (GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL)
	AJIN_IO->SendOutputBit(DIO_INPUT_X0_10_FRONT_DOORLOCK_OPENCLOSE, TRUE);
	AJIN_IO->SendOutputBit(DIO_INPUT_X0_11_LEFT_DOORLOCK1_OPENCLOSE, TRUE);
	AJIN_IO->SendOutputBit(DIO_INPUT_X0_12_LEFT_DOORLOCK2_OPENCLOSE, TRUE);
	AJIN_IO->SendOutputBit(DIO_INPUT_X0_13_RIGHT_DOORLOCK1_OPENCLOSE, TRUE);
	AJIN_IO->SendOutputBit(DIO_INPUT_X0_14_RIGHT_DOORLOCK2_OPENCLOSE, TRUE);
	AJIN_IO->SendOutputBit(DIO_INPUT_X0_15_BACK_DOORLOCK1_OPENCLOSE, TRUE);
	AJIN_IO->SendOutputBit(DIO_INPUT_X0_16_BACK_DOORLOCK2_OPENCLOSE, TRUE);
#elif(GET_INSPECTOR == SYS_OQC_SFR_SINGLE_CL)
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_10_DOORLOCK_ONOFF_FRONT, TRUE);
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_11_DOORLOCK_ONOFF_LEFT1, TRUE);
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_12_DOORLOCK_ONOFF_RIGHT1, TRUE);
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_13_DOORLOCK_ONOFF_BACK1, TRUE);
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_14_DOORLOCK_ONOFF_BACK2, TRUE);
#elif(GET_INSPECTOR == SYS_DISTORTION_30)
	AJIN_IO->SendOutputBit(DIO_INPUT_X0_10_FRONT_DOORLOCK_OPENCLOSE, TRUE);
	AJIN_IO->SendOutputBit(DIO_INPUT_X0_11_LEFT_DOORLOCK1_OPENCLOSE, TRUE);
	AJIN_IO->SendOutputBit(DIO_INPUT_X0_12_LEFT_DOORLOCK2_OPENCLOSE, TRUE);
	AJIN_IO->SendOutputBit(DIO_INPUT_X0_13_RIGHT_DOORLOCK1_OPENCLOSE, TRUE);
	AJIN_IO->SendOutputBit(DIO_INPUT_X0_14_RIGHT_DOORLOCK2_OPENCLOSE, TRUE);
	AJIN_IO->SendOutputBit(DIO_INPUT_X0_15_BACK_DOORLOCK1_OPENCLOSE, TRUE);
	AJIN_IO->SendOutputBit(DIO_INPUT_X0_16_BACK_DOORLOCK2_OPENCLOSE, TRUE);
#elif(GET_INSPECTOR == SYS_FILM_PEEL_OFF)
	AJIN_IO->SendOutputBit(DIO_INPUT_X0_16_REAR_DOORLOCK1_OPENCLOSE, TRUE);
	AJIN_IO->SendOutputBit(DIO_INPUT_X0_17_REAR_DOORLOCK2_OPENCLOSE, TRUE);
	AJIN_IO->SendOutputBit(DIO_INPUT_X0_18_REAR_DOORLOCK3_OPENCLOSE, TRUE);
#endif


	//if (IO_Door_Front1_Lock(bLock))
	//{
	//	bOK = false;
	//	goto TEST_END;
	//}		

	///*
	//if (IO_Door_Front2_Lock(bLock))
	//{
	//	bOK = false;
	//	goto TEST_END;
	//}
	//*/
	//
	//if (IO_Door_Rear1_Lock(bLock))
	//{
	//	bOK = false;
	//	goto TEST_END;
	//}
	//
	//if (IO_Door_Rear2_Lock(bLock))
	//{
	//	bOK = false;
	//	goto TEST_END;
	//}	

//TEST_END:	
	return bOK;
}

// DOOR FRONT1 LOCK

bool CIO::IO_Door_Front1_Lock(__in bool bLock)
{
	auto bRet = false;
#if(GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL) || (GET_INSPECTOR == SYS_OQC_SFR_SINGLE_CL) || (GET_INSPECTOR == SYS_DISTORTION_30)
	auto Out = 0;
	auto InOpenClose = 0;
	auto inLock = 0;

	Out = DIO_OUTPUT_Y0_10_DOORLOCK_ONOFF_FRONT;
	AJIN_IO->SendOutputBit(Out, bLock);

	// door 상태 확인
	InOpenClose = DIO_INPUT_X0_10_FRONT_DOORLOCK_OPENCLOSE;
	
	bRet = WaitStatus(InOpenClose, bLock);
#elif(GET_INSPECTOR == SYS_FILM_PEEL_OFF)
	bRet = true;
#endif
	return bRet;
}

// DOOR REAR1 LOCK
bool CIO::IO_Door_Rear1_Lock(__in bool bLock)
{
	auto bRet = false;

	auto Out = 0;
	auto InOpenClose = 0;
	auto inLock = 0;

#if(GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL)
	Out = DIO_OUTPUT_Y0_15_DOORLOCK_ONOFF_BACK1;
	AJIN_IO->SendOutputBit(Out, bLock);

	// door 상태 확인 
	InOpenClose = DIO_INPUT_X0_15_BACK_DOORLOCK1_OPENCLOSE;
#elif(GET_INSPECTOR == SYS_OQC_SFR_SINGLE_CL)
	Out = DIO_OUTPUT_Y0_13_DOORLOCK_ONOFF_BACK1;
	AJIN_IO->SendOutputBit(Out, bLock);

	// door 상태 확인 
	InOpenClose = DIO_INPUT_X0_13_BACK_DOORLOCK1_OPENCLOSE;
#elif(GET_INSPECTOR == SYS_FILM_PEEL_OFF)
	Out = DIO_OUTPUT_Y0_16_REAR_DOORLOCK1_OPENCLOSE;
	AJIN_IO->SendOutputBit(Out, bLock);

	// door 상태 확인 
	InOpenClose = DIO_INPUT_X0_16_REAR_DOORLOCK1_OPENCLOSE;
#endif
	
	bRet = WaitStatus(InOpenClose, bLock);
	return bRet;
}

// DOOR REAR2 LOCK
bool CIO::IO_Door_Rear2_Lock(__in bool bLock)
{
	auto bRet = false;

	auto Out = 0;
	auto InOpenClose = 0;
	auto inLock = 0;

#if(GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL)
	Out = DIO_OUTPUT_Y0_16_DOORLOCK_ONOFF_BACK2;
	AJIN_IO->SendOutputBit(Out, bLock);

	// door 상태 확인 
	InOpenClose = DIO_INPUT_X0_16_BACK_DOORLOCK2_OPENCLOSE;
#elif(GET_INSPECTOR == SYS_OQC_SFR_SINGLE_CL)
	Out = DIO_OUTPUT_Y0_14_DOORLOCK_ONOFF_BACK2;
	AJIN_IO->SendOutputBit(Out, bLock);

	// door 상태 확인 
	InOpenClose = DIO_INPUT_X0_14_BACK_DOORLOCK2_OPENCLOSE;
#elif(GET_INSPECTOR == SYS_FILM_PEEL_OFF)
	Out = DIO_OUTPUT_Y0_17_REAR_DOORLOCK2_OPENCLOSE;
	AJIN_IO->SendOutputBit(Out, bLock);

	// door 상태 확인 
	InOpenClose = DIO_INPUT_X0_17_REAR_DOORLOCK2_OPENCLOSE;
#endif

	bRet = WaitStatus(InOpenClose, bLock);
	return bRet;
}

bool CIO::IO_Door_Rear3_Lock(__in bool bLock)
{
	auto bRet = false;

	auto Out = 0;
	auto InOpenClose = 0;
	auto inLock = 0;

#if(GET_INSPECTOR == SYS_FILM_PEEL_OFF)
	Out = DIO_OUTPUT_Y0_18_REAR_DOORLOCK3_OPENCLOSE;
	AJIN_IO->SendOutputBit(Out, bLock);

	// door 상태 확인 
	InOpenClose = DIO_INPUT_X0_18_REAR_DOORLOCK3_OPENCLOSE;
#endif

	bRet = WaitStatus(InOpenClose, bLock);
	return bRet;
}
