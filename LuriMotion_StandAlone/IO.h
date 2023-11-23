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

	static void IO_O_Muting(__in bool bOn);

	static bool IO_I_Socket_State_PCB_Gripper_On();
	static bool IO_I_Socket_State_PCB_Gripper_Off();
	static bool IO_I_Socket_State_PCB_Gripper_Up();
	static bool IO_I_Socket_State_PCB_Gripper_Down();
	static bool IO_I_CoverPush_Gripper_Up();
	static bool IO_I_CoverPush_Gripper_Down();
	static bool IO_I_Flim_Gripper_For(__in int nSelect);
	static bool IO_I_Flim_Gripper_Back(__in int nSelect);
	static bool IO_I_Flim_Gripper_On(__in int nSelect);
	static bool IO_I_Flim_Gripper_Off(__in int nSelect);
	static bool IO_I_Flim_Gripper_Cover_On(__in int nSelect);
	static bool IO_I_Flim_Gripper_Cover_Off(__in int nSelect);
	static bool IO_I_Flim_Gripper_Film_Box_Dtc(__in int nSelect);
	static bool IO_I_Socket_State_Film_Box_Dtc(__in int nSelect);
	static bool IO_I_Socket_State_Work_Zone_Dtc();
	static bool IO_I_Socket_State_PCB_Dtc();

	static bool IO_I_IZN10E_Ion(__in int nSelect);
	static bool IO_I_IZN10E_Error(__in int nSelect);
	static bool IO_I_IZN10E_Maintence(__in int nSelect);

	static bool IO_O_Socket_State_PCB_Gripper_On(bool bCheckStatus = true);
	static bool IO_O_Socket_State_PCB_Gripper_Off(bool bCheckStatus = true);
	static bool IO_O_Socket_State_PCB_Gripper_Up(bool bCheckStatus = true);
	static bool IO_O_Socket_State_PCB_Gripper_Down(bool bCheckStatus = true);
	static bool IO_O_CoverPush_Gripper_Up(bool bCheckStatus = true);
	static bool IO_O_CoverPush_Gripper_Down(bool bCheckStatus = true);
	static bool IO_O_Flim_Gripper_For(__in int nSelect, bool bCheckStatus = true);
	static bool IO_O_Flim_Gripper_Back(__in int nSelect, bool bCheckStatus = true);
	static bool IO_O_Flim_Gripper_On(__in int nSelect, bool bCheckStatus = true);
	static bool IO_O_Flim_Gripper_Off(__in int nSelect, bool bCheckStatus = true);
	static bool IO_O_Flim_Gripper_Cover_On(__in int nSelect, bool bCheckStatus = true);
	static bool IO_O_Flim_Gripper_Cover_Off(__in int nSelect, bool bCheckStatus = true);
	static void IO_O_Flim_Gripper_Air_Blow(__in int nSelect, __in bool bOn);

	static void IO_O_IZN10E_Power(__in int nSelect, __in bool bOn);
	static void IO_O_IZN10E_Ion(__in int nSelect, __in bool bOn);
	static void IO_O_IZN10E_Reset(__in int nSelect, __in bool bOn);

	static bool IO_Door_All_Lock	(__in bool bLock);

	bool IO_Door_Front1_Lock		(__in bool bLock);
	bool IO_Door_Rear1_Lock			(__in bool bLock);
	bool IO_Door_Rear2_Lock			(__in bool bLock);
	bool IO_Door_Rear3_Lock			(__in bool bLock);

protected:
};

