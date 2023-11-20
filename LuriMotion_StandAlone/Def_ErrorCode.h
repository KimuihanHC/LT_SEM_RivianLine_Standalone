#pragma once

typedef enum
{
	MCEC_OK = 0,
	MCEC_Unknown = 100,
	MCEC_AreaSensorDetect,
	MCEC_ForceStop,

	// 설비 유틸 io 와 관련된 에러코드
	MCEC_Door_Open,		// door
	MCEC_Door_OpenFront1,
	MCEC_Door_OpenFront2,
	MCEC_Door_OpenRear1,
	MCEC_Door_OpenRear2,		

	//---------------------------- 에러 발생후 원점을 잡아야만 재시작 할수 있다 //-----------------
	MCEC_EMO,			// emo
	MCEC_AirPressure,	// air
	MCEC_PowerOff,

	// 모션
	MCEC_Motion_Move_Failed = 200,
	MCEC_MotionZ_Not_Zero,
	MCEC_MotionY_PositionErr,
	MCEC_MotionZ_MovingTimeout,
	MCEC_MotionY_MovingTimeout,

	// io
	MCEC_Io_SocketCoverOpen,
	MCEC_Io_ProbePinUp_Timeout,
	MCEC_Io_ProbePinDn_Timeout,

	MCEC_Io_CameraFix_On_Timeout,
	MCEC_Io_CameraFix_Off_Timeout,

	MCEC_Io_Socket_Up_Timeout,
	MCEC_Io_Socket_Down_Timeout,

	MCEC_Io_Socket_In_Timeout,
	MCEC_Io_Socket_Out_Timeout,
	MCEC_Io_CameraNotDetect,
	MCEC_SetPowerVoltage_Fail,
	MCEC_MaxNum,
}enMCErrCode;

// static LPCTSTR g_szMachineErrKor[] = 
// {
// 	_T("성공"),															//MCEC_OK = 0,
// 	NULL
// };
// 
static LPCTSTR g_szMachineErrEng[] =
{
	_T("Success"),										//MCEC_OK = 0,
	_T("Unknown error"),								//MCEC_Unknown,
	_T("Stop"),											//MCEC_ForceStop

	//// 설비 유틸 io 와 관련된 에러코드
	_T("Door Open"),									//MCEC_Door_Open,		// door
	_T("Front1 Door Open"),								//MCEC_Door_OpenFront1,
	_T("Front2 Door Open"),								//MCEC_Door_OpenFront2,
	_T("Rear1 Door Open"),								//MCEC_Door_OpenRear1,
	_T("Rear2 Door Open	"),								//MCEC_Door_OpenRear2,
	
	////---------------------------- 에러 발생후 원점을 잡아야만 재시작 할수 있다 //-----------------
	_T("Equipment EMO"),						//MCEC_EMO,			// emo
	_T("Equipment Main AIR Off"),				//MCEC_AirPressure,	// air
	_T("Equipment Power Off"),
	
	//// 모션
	_T("Motion Move Failed"),
	_T("MotionZ is not Zero"),			//MCEC_MotionZ_Not_Zero
	_T("MotionY is Position error"),	//MCEC_MotionY_PositionErr

	NULL
};

