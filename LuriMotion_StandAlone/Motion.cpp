
#include "pch.h"
#include "Motion.h"

CMotion::CMotion()
{

}

CMotion::~CMotion()
{

}

void CMotion::MO_AllAxis_Reset()
{
	for (auto i = 0; i < MOTOR_MAX_COUNT; i++)
	{
		FAS_MO->InitHomeOK(i);
	}
}

bool CMotion::MoveAxis(__in int nAxis, __in double dbPos, __in double dbSpeed)
{
	return FAS_MO->SingleAxisAbsMove(nAxis, dbPos, dbSpeed);
}

enMCErrCode CMotion::MO_ManualMove(__in int nAxis, __in double dbPos)
{
	auto err = MCEC_OK;

	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();

	auto axis = nAxis;
	double p = 0.0;
	double s = stMotion.nMotorSpeed[axis][2];

	switch (nAxis)
	{
	case SOCKET_Y:
		p = dbPos * PULSE_PER_UNIT_Y;
		break;

	case SOCKET_Z:
		p = dbPos * PULSE_PER_UNIT_Z;
		break;
	}

	if(MoveAxis(axis, p, s) == false)
		err = MCEC_Motion_Move_Failed;
	
	return err;
}

enMCErrCode CMotion::MO_MoveY_Load()
{
	auto err = MCEC_OK;

	// z축 zero position 일경우에만 이동가능
	if (MO_CheckZ_Zero() == false)
	{
		err = MCEC_MotionZ_Not_Zero;
		return err;
	}

	TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();
	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();

	double p = (double)stTeaching.nSocketY_Wait_Pos;
	double s = (double)stMotion.nMotorSpeed[0][2];

	if (MoveAxis(SOCKET_Y, p, s) == false)
		err = MCEC_Motion_Move_Failed;
	
	return err;
}

enMCErrCode CMotion::MO_MoveY_Blemish()
{
	auto err = MCEC_OK;

	// z축 zero position 일경우에만 이동가능
	if (MO_CheckZ_Zero() == false)
	{
		err = MCEC_MotionZ_Not_Zero;
		return err;
	}

	TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();
	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();

	double p = (double)stTeaching.nSocketY_Blemishi_Pos;
	double s = (double)stMotion.nMotorSpeed[0][2];

	if (MoveAxis(SOCKET_Y, p, s) == false)
		err = MCEC_Motion_Move_Failed;
	
	return err;
}

enMCErrCode CMotion::MO_MoveY_SFR()
{
	auto err = MCEC_OK;

	// z축 zero position 일경우에만 이동가능
	if (MO_CheckZ_Zero() == false)
	{
		err = MCEC_MotionZ_Not_Zero;
		return err;
	}
	
	TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();
	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();

	double p = (double)stTeaching.nSocketY_SFR_Pos;
	double s = (double)stMotion.nMotorSpeed[0][2];

	if (MoveAxis(SOCKET_Y, p, s) == false)
		err = MCEC_Motion_Move_Failed;
	
	return err;
}

enMCErrCode CMotion::MO_MoveY_Intrinsic(__in int nIndex)
{
	auto err = MCEC_OK;

	// z축 zero position 일경우에만 이동가능
	if (MO_CheckZ_Zero() == false)
	{
		err = MCEC_MotionZ_Not_Zero;
		return err;
	}

	TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();
	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();

	double p = (double)stTeaching.nSocketY_Intrinsic_Pos[nIndex-1];
	double s = (double)stMotion.nMotorSpeed[0][2];

	if (MoveAxis(SOCKET_Y, p, s) == false)
		err = MCEC_Motion_Move_Failed;

	return err;
}

enMCErrCode CMotion::MO_MoveZ_Load()
{
	auto err = MCEC_OK;
	   	
	if (MO_CheckY_Load() == false)
	{
		err = MCEC_MotionY_PositionErr;
		return err;
	}
	   
	TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();
	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();

	double p = (double)stTeaching.nSocketZ_Wait_Pos;
	double s = (double)stMotion.nMotorSpeed[SOCKET_Z][2];

	if (MoveAxis(SOCKET_Z, p, s) == false)
		err = MCEC_Motion_Move_Failed;

	return err;
}

enMCErrCode CMotion::MO_MoveZ_Blemish()
{
	auto err = MCEC_OK;

	if (MO_CheckY_Blemish() == false)
	{
		err = MCEC_MotionY_PositionErr;
		return err;
	}

	TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();
	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();

	double p = (double)stTeaching.nSocketZ_Blemishi_Pos;
	double s = (double)stMotion.nMotorSpeed[SOCKET_Z][2];

	if (MoveAxis(SOCKET_Z, p, s) == false)
		err = MCEC_Motion_Move_Failed;

	return err;
}

enMCErrCode CMotion::MO_MoveZ_SFR()
{
	auto err = MCEC_OK;

	if (MO_CheckY_SFR() == false)
	{
		err = MCEC_MotionY_PositionErr;
		return err;
	}
	
	TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();
	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();

	double p = (double)stTeaching.nSocketZ_SFR_180_Pos;
	double s = (double)stMotion.nMotorSpeed[SOCKET_Z][2];

	if (MoveAxis(SOCKET_Z, p, s) == false)
		err = MCEC_Motion_Move_Failed;

	return err;
}

enMCErrCode CMotion::MO_MoveZ_Zero()
{
	auto err = MCEC_OK;

	TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();
	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();

	double p = (double)0.0f;
	double s = (double)stMotion.nMotorSpeed[SOCKET_Z][2];

	if (MoveAxis(SOCKET_Z, p, s) == false)
		err = MCEC_Motion_Move_Failed;

	return err;
}

bool CMotion::MO_CheckY_Load()
{
	TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();
	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();

	if (CMotion::MO_CheckY_Moving())
		return false;

	return abs(FAS_MO->GetAxisCurrnetActualPos(SOCKET_Y) - stTeaching.nSocketY_Wait_Pos / PULSE_PER_UNIT_Y) < 1;
}

bool CMotion::MO_CheckY_Blemish()
{
	TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();
	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();

	if (CMotion::MO_CheckY_Moving())
		return false;

	return abs(FAS_MO->GetAxisCurrnetActualPos(SOCKET_Y) - stTeaching.nSocketY_Blemishi_Pos / PULSE_PER_UNIT_Y) < 1;
}

bool CMotion::MO_CheckY_SFR()
{
	TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();
	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();

	if (CMotion::MO_CheckY_Moving())
		return false;

	return abs(FAS_MO->GetAxisCurrnetActualPos(SOCKET_Y) - stTeaching.nSocketY_SFR_Pos / PULSE_PER_UNIT_Y) < 1;
}

bool CMotion::MO_CheckY_Intrinsic(__in int nIndex)
{
	TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();
	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();

	if (CMotion::MO_CheckY_Moving())
		return false;

	return abs(FAS_MO->GetAxisCurrnetActualPos(SOCKET_Y) - stTeaching.nSocketY_Intrinsic_Pos[nIndex-1] / PULSE_PER_UNIT_Y) < 1;
}

bool CMotion::MO_CheckZ_Load()
{
	TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();
	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();

	if (CMotion::MO_CheckZ_Moving())
		return false;

	return abs(FAS_MO->GetAxisCurrnetActualPos(SOCKET_Z) - stTeaching.nSocketZ_Wait_Pos / PULSE_PER_UNIT_Z) < 1;
}

bool CMotion::MO_CheckZ_Blemish()
{
	if (GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL)
	{
		TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();
		MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();

		if (CMotion::MO_CheckZ_Moving())
			return false;

		return abs(FAS_MO->GetAxisCurrnetActualPos(SOCKET_Z) - stTeaching.nSocketZ_Blemishi_Pos / PULSE_PER_UNIT_Z) < 1;
	}
	else
	{
		return true;
	}
}

bool CMotion::MO_CheckZ_SFR()
{
	if (GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL)
	{
		TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();
		MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();

		if (CMotion::MO_CheckZ_Moving())
			return false;

		return abs(FAS_MO->GetAxisCurrnetActualPos(SOCKET_Z) - stTeaching.nSocketZ_SFR_180_Pos / PULSE_PER_UNIT_Z) < 1;
	}
	else
	{
		return true;
	}
	}

bool CMotion::MO_CheckZ_Zero()
{
	TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();
	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();

	if (GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL)
	{
		if (CMotion::MO_CheckZ_Moving())
			return false;

		return FAS_MO->GetAxisCurrnetActualPos(SOCKET_Z) < 0 + 1;
	}
	else
	{
		return true;
	}
}

bool CMotion::MO_CheckY_Moving()
{
	return FAS_MO->IsMotioning(SOCKET_Y);
}

bool CMotion::MO_CheckZ_Moving()
{
	return FAS_MO->IsMotioning(SOCKET_Z);
}

int CMotion::MO_GetPositionY(__out double& dbpos)
{
	return 0;
}

int CMotion::MO_GetPositionZ(__out double& dbpos)
{
	return 0;
}
