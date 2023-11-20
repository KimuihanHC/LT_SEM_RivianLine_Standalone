#pragma once

#include "Def_CompileOption.h"
#include "Def_Sequence.h"

class CIO
{
public:
	CIO();
	~CIO();

private:
	static void DoEvents					();
	static void DoEvents					(DWORD dwMiliSeconds);

	static bool WaitStatus					(__in int OnPort, __in int OffPort);
	static bool WaitStatus					(__in int Port, __in bool Status);

public:
	bool IO_I_WaitStatus					(__in int idx, __in bool bStatus, __in DWORD dwMilisecond);
	static bool IO_I_AREA_SENSOR			();
	static bool IO_I_EMO					();
	static bool IO_I_PowerOff				();
	static bool IO_I_INIT					();
	static bool IO_I_MainAirPress			();
	static bool IO_I_SleepMode				();
	static void IO_O_SleepMode				(__in bool bOn);
	static void IO_O_Buzzer					(__in bool bOn);
	static void IO_O_RESET					(__in bool bOn);
	static void IO_O_INIT					(__in bool bOn);

	static bool IO_I_Socket_In();
	static bool IO_I_Socket_Out();
	static bool IO_I_Socket_Up();
	static bool IO_I_Socket_Down();
	static bool IO_I_Camera_Fix_Cylinder_On();
	static bool IO_I_Camera_Fix_Cylinder_Off();
	static bool IO_I_ProbePin_Up();
	static bool IO_I_ProbePin_Down();
	static bool IO_I_DetectCamera();
	static bool IO_I_SocketCoverOpen();
	static bool IO_O_Socket_In(bool bCheckStatus = true);
	static bool IO_O_Socket_Out(bool bCheckStatus = true);
	static bool IO_O_Socket_Up(bool bCheckStatus = true);
	static bool IO_O_Socket_Down(bool bCheckStatus = true);
	static bool IO_O_Camera_Fix_Cylinder_On(bool bCheckStatus = true);
	static bool IO_O_Camera_Fix_Cylinder_Off(bool bCheckStatus = true);
	static bool IO_O_ProbePin_Up(bool bCheckStatus = true);
	static bool IO_O_ProbePin_Down(bool bCheckStatus = true);
	static bool MoveSocketToSaftyZone();

	static bool IO_Door_All_Lock	(__in bool bLock);
	bool IO_Door_Front1_Lock		(__in bool bLock);
	bool IO_Door_Rear1_Lock			(__in bool bLock);
	bool IO_Door_Rear2_Lock			(__in bool bLock);

protected:
};

