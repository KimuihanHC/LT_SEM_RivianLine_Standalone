
#include "pch.h"
#include "AjinBoard.h"
#include <stdio.h>

// CAjinMotion

IMPLEMENT_DYNAMIC(CAjinMotion, CWnd)
CAjinMotion::CAjinMotion()
{
	
	for (int i = 0; i < (int)MOTOR_MAX_COUNT; i++)
	{
		m_bReadError[i]		= FALSE;
		m_bReadHome[i]		= FALSE;
		m_bReadLimitP[i]	= FALSE;
		m_bReadLimitM[i]	= FALSE;
		m_bReadInPos[i]		= FALSE;
	}

	m_strLastError			= _T("");
	m_dwLastError			= 0;

	m_bConnected			= FALSE;
}

CAjinMotion::~CAjinMotion()
{

}


BEGIN_MESSAGE_MAP(CAjinMotion, CWnd)
//	ON_WM_TIMER()
END_MESSAGE_MAP()



// CAjinMotion 메시지 처리기입니다.

BOOL CAjinMotion::Create(CWnd* pParentWnd) 
{
	CRect rect = CRect(0, 0, 0, 0); // 화면 영역 설정
	return CWnd::Create(NULL, NULL, WS_CHILD, rect, GetDesktopWindow(), 0);	
}

void CAjinMotion::InitVariables()
{	

}

void CAjinMotion::InitRobotVariables()
{	

}



UINT CAjinMotion::Thread_MotionRead(LPVOID lpVoid)
{
	
	while (AJIN_MO->m_bThreadRun)
	{
		if (AJIN_MO->m_dwLastError != AXT_RT_SUCCESS)
			continue;

		//if (AJIN_MO->IsInvaildAxis() == TRUE)
		{
			AJIN_MO->IsServoAlarmOn();
			AJIN_MO->IsServoCurPos();
			AJIN_MO->IsMotionInpos();
			AJIN_MO->IsLimitChk();
			AJIN_MO->isServoHome();
		}
		//else
		//{
		//	AJIN_MO->m_strLastError.Format(_T("%u"), AJIN_MO->m_dwLastError);
		//	WRITE_MAIN_LOG(AJIN_MO->m_strLastError);
		//}

		Sleep(200);
	}

	AJIN_MO->m_bThreadRun = FALSE;
	AJIN_MO->m_pMotionThread = NULL;

	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

bool CAjinMotion::Init_Ajin()
{
	DWORD dwResult;

	dwResult = AxlOpen(AXL_DEFAULT_IRQNO);

	char szFilePath[] = "C:\\Exec_Rivian\\Mot\\1.mot";

	if(AxmMotLoadParaAll(szFilePath) != AXT_RT_SUCCESS)
	{
		return false;
	}
	
	m_bConnected = true;

	m_bThreadRun = TRUE;
	m_pMotionThread = AfxBeginThread(Thread_MotionRead, this);

	for (int i = 0; i < (int)MOTOR_MAX_COUNT; i++)
	{
		AJIN_MO->ServoOn((long)i);
	}

	return true;
}

bool CAjinMotion::IsInit_Ajin()
{
	if( m_bConnected != true ) 
		return false;

	DWORD dwResult;

	dwResult = AxlOpen(AXL_DEFAULT_IRQNO);

	if ( dwResult == AXT_RT_OPEN_ALREADY )
		return TRUE;

	return FALSE;
}

//========================================================================================
// Monitoring Func

bool CAjinMotion::IsInvaildAxis()
{
	DWORD dwResult = 0;

	for (int i = 0; i < (int)MOTOR_MAX_COUNT; i++)
	{
		dwResult = AxmInfoIsInvalidAxisNo((long)i);

		if (AXT_RT_SUCCESS != dwResult)
		{
			m_dwLastError = dwResult;
			return FALSE;
		}			

		dwResult = 0;
	}

	return TRUE;
}

bool CAjinMotion::IsServoAlarmOn()
{
	DWORD dwResult = 0;

	for (int i = 0; i < (int)MOTOR_MAX_COUNT; i++)
	{
		dwResult = AxmSignalReadServoAlarm( (long)i, &m_bReadError[i]);
	}

	return TRUE;
}


bool CAjinMotion::IsServoCurPos()
{
	DWORD dwResult = 0;

	for (int i = 0; i < (int)MOTOR_MAX_COUNT; i++)
	{
		dwResult = AxmStatusGetActPos((long)i, &m_dbPos[i]);
	}

	return TRUE;
}
bool CAjinMotion::IsMotionInpos()
{
	DWORD dwResult = 0;

	for (int i = 0; i < (int)MOTOR_MAX_COUNT; i++)
	{
		dwResult = AxmSignalReadInpos((long)i, &m_bReadInPos[i]);
	}

	return TRUE;
}
bool CAjinMotion::IsLimitChk()
{
	DWORD dwResult = 0;

	for (int i = 0; i < (int)MOTOR_MAX_COUNT; i++)
	{
		dwResult = AxmSignalReadLimit((long)i, &m_bReadLimitP[i], &m_bReadLimitM[i]);
	}

	return TRUE;
}
bool CAjinMotion::isServoHome()
{
	
	DWORD dwResult = 0;

	for (int i = 0; i < (int)MOTOR_MAX_COUNT; i++)
	{
		dwResult = AxmHomeReadSignal((long)i, &m_bReadHome[i]);
	}
	

	return TRUE;
}

// End
//========================================================================================
bool CAjinMotion::LoadFile_Mot()
{
	CString	temp;
	CFileDialog dlg(TRUE, _T("Mot"), NULL, OFN_HIDEREADONLY, _T("AXM File (*.mot)|*.mot|All Files (*.*)|*.*||"));

	if (dlg.DoModal() == IDOK)
	{		
		temp				= dlg.GetPathName();
		char *strFilename	= (LPSTR)(LPCTSTR)temp.GetBuffer(0);

		//++ 함수 실행 성공시 지정한 Mot파일의 설정값으로 모션축 설정이 일괄 변경됩니다.
		if (AxmMotLoadParaAll(strFilename) != AXT_RT_SUCCESS)
			return false;
		else
			return true;
	}
	
	return false;
}
bool CAjinMotion::SaveFile_Mot()
{
	CString temp;
	CFileDialog dlg(FALSE, _T("mot"), NULL, OFN_HIDEREADONLY, _T("AXM File (*.mot)|*.mot|All Files (*.*)|*.*||"));

	if(dlg.DoModal() == IDOK)
	{
		temp = dlg.GetPathName();
		char *strFilename = (LPSTR)(LPCTSTR)temp.GetBuffer(0);
		//++ 함수 실행 성공시 각각의 모션축 설정값으로 지정한 Mot파일을 생성 및 저장 됩니다.
		if (AXT_RT_SUCCESS != AxmMotSaveParaAll(strFilename))
			return false;

		temp.ReleaseBuffer();	
		return true;
	}

	return false;
}

bool CAjinMotion::Move_Home(long lAxis)
{
	DWORD	dwRet;
	CString strResult = _T("");

	dwRet = AxmHomeSetStart(lAxis);	
	if(dwRet != AXT_RT_SUCCESS)
	{
		CString strData;
		strResult.Format(_T("AxmHomeSetStart return error[Code:%04d]"), dwRet);
		return false;
	}

	return true;
}
bool CAjinMotion::HomeResult(long lAxis)
{
	CString strResult;	
	DWORD	dwRet;

	AxmHomeGetResult(lAxis, &dwRet);

	if (dwRet == HOME_SUCCESS)
		return TRUE;

	return FALSE;
}

BOOL CAjinMotion::GetHomeRate(long lAxis)
{
	DWORD dwRet;
	m_dwMain = 0;
	m_dwSub = 0;

	dwRet = AxmHomeGetRate(lAxis, &m_dwMain, &m_dwSub);
	if(dwRet == 100)
	{
		return TRUE;
	}

	return FALSE;
}

int  CAjinMotion::GetHomeRate_Gage(long lAxis)
{
	DWORD dwRet;
	m_dwMain = 0;
	m_dwSub = 0;

	dwRet = AxmHomeGetRate(lAxis, &m_dwMain, &m_dwSub);

	return (int)dwRet;
}

DWORD CAjinMotion::Get_HomeRate_Sub()
{
	return m_dwSub;
}

void CAjinMotion::Set_HomeRate_Sub_Zero()
{
	m_dwSub = 0;
}

bool CAjinMotion::MoveEStop(long lAxis)
{
	DWORD dwRet;

	dwRet = AxmMoveEStop(lAxis);

	if( dwRet != AXT_RT_SUCCESS )
	{
		return false;
	}

	return true;
}

bool CAjinMotion::MoveSStop(long lAxis)
{
	DWORD dwRet;

	dwRet = AxmMoveSStop(lAxis);

	if( dwRet != AXT_RT_SUCCESS )
	{
		return false;
	}

	return true;
}

void CAjinMotion::ServoOn(long lAxis)
{
	AxmSignalServoOn(lAxis, TRUE);
}

void CAjinMotion::ServoOff(long lAxis)
{
	AxmSignalServoOn(lAxis, FALSE);
}

bool CAjinMotion::IsServoOn(long lAxis)
{
	DWORD dwOnOff = 0;

	AxmSignalIsServoOn(lAxis, &dwOnOff);

	if( dwOnOff == 0 )
		return FALSE;
	
	return TRUE;
}

bool CAjinMotion::SetErrorReset(long lAxis)
{
	DWORD dwStatus = 0;

	// 서보 알람 상태를 읽고
	AxmSignalReadServoAlarm(lAxis, &dwStatus);
	if (FALSE == dwStatus)
		return TRUE;

	// 서보가 꺼져있으면 서보를 키자.
	AxmSignalIsServoOn(lAxis, &dwStatus);
	if( dwStatus == 0 )
		AxmSignalServoOn(lAxis, TRUE);

	//그후 에러처리
	dwStatus = AxmSignalServoAlarmReset(lAxis, ENABLE);

	Sleep(500);
	dwStatus = AxmSignalServoAlarmReset(lAxis, DISABLE);

	if (dwStatus != AXT_RT_SUCCESS)
		return FALSE;

	return TRUE;
}

void CAjinMotion::GetPulseAndUnit(long lAxis, double *dUnit, long *dPulse)
{
	AxmMotGetMoveUnitPerPulse(lAxis, dUnit, dPulse);
}

double CAjinMotion::GetActPos(long lAxis)
{
	double dPos = 0.0f;
	AxmStatusGetActPos(lAxis, &dPos);
	return (dPos);
}

double CAjinMotion::GetCmdPos(long lAxis)
{
	double dPos = 0.0;
	AxmStatusGetCmdPos(lAxis, &dPos);
	return (dPos);
}

//////////////////////////////////////////////////////////////////////////
// Else

bool CAjinMotion::IsMotion(long lAxis)
{
	DWORD	dwRet;
	AxmStatusReadInMotion(lAxis, &dwRet);
	return dwRet;
}

void CAjinMotion::PositionClear(long lAxis)
{
	AxmStatusSetPosMatch(lAxis, 0.0);
}

void CAjinMotion::SetMotionMode( long lAxis, int nMode )
{
	AxmMotSetAbsRelMode( lAxis, nMode );
}
bool CAjinMotion::MoveAxis_NonWait(long lAxis, double dPos, double dVel)
{
	CString strData;

	/*
	if( (int)lAxis == (int)AXIS_HANDLER_Y1_MAIN )
	{
		if( GetGantryModeEnable() == FALSE )
		{
			strData.Format(_T("Gantry Mode Is Not Enable \n"));
			return FALSE;
		}
	}
	*/
	double dVelocity = 0.0f;
	double dAcc = 0.0f;
	double dDec = 0.0f;

	/*
	MOTION_PARAM   stSpeed = *DATAMANAGER->GetCurrentMotionParam();

	if (dVel == 0)
	{
		dVelocity	= stSpeed.nMotorSpeed[(int)lAxis][eSpeedHigh];
		dAcc		= stSpeed.nMotorSpeed[(int)lAxis][eSpeedAcc];
		dDec		= stSpeed.nMotorSpeed[(int)lAxis][eSpeedDec];
	}
	else
	{
		dVelocity = dVel;
		dAcc = dVel * 5;
		dDec = dVel * 5;
	}
	dwRetCode = AxmMoveStartPos(lAxis, dPos, dVelocity, dAcc, dDec);

	if(dwRetCode != AXT_RT_SUCCESS)
	{
		strData.Format(_T("AxmMoveStartPos Return Error[Code:%04d]"), dwRetCode);
		
		return FALSE;
	}
	*/
	return TRUE;
}

bool CAjinMotion::MoveAxis_Wait(long lAxis, double dPos, double dVel)
{
	DWORD	dwRetCode;
	CString strData;
	dwRetCode = AxmMovePos(lAxis, dPos, dVel, dVel*5, dVel*5);

	if(dwRetCode != AXT_RT_SUCCESS)
	{
		strData.Format(_T("AxmMoveStartPos Return Error[Code:%04d]"), dwRetCode);
		return FALSE;
	}

	return TRUE;
}

bool CAjinMotion::MoveAxis_Velocity(long lAxis, double dVel)
{
	CString strData;

	double dAcc = dVel;
	if( dVel < 0 ) dAcc = dVel * -1;

	double dDec = 0.0f;

	/*
	MOTION_PARAM   stSpeed = *DATAMANAGER->GetCurrentMotionParam();
	dAcc = stSpeed.nMotorSpeed[(int)lAxis][eSpeedAcc];
	dDec = stSpeed.nMotorSpeed[(int)lAxis][eSpeedDec];

	dwRetCode = AxmMoveVel(lAxis, dVel, dAcc, dDec);

	if(dwRetCode != AXT_RT_SUCCESS)
	{
		strData.Format(_T("AxmMoveStartPos Return Error[Code:%04d]"), dwRetCode);

		return FALSE;
	}
	*/
	return TRUE;
}

bool CAjinMotion::MotorMultiAxis_NonWait(long lAxisCnt, long *lpAxisNo, double *dpPos, double *dpVel)
{
	/*
	CString strData;
	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();

	double* dpAcc = new double[lAxisCnt];
	double* dpDec = new double[lAxisCnt];

	for( int i=0; i<(int)lAxisCnt; i++)
	{
		dpAcc[i] = (double)stMotion.nMotorSpeed[lpAxisNo[i]][eSpeedAcc];
		dpDec[i] = (double)stMotion.nMotorSpeed[lpAxisNo[i]][eSpeedDec];
	}

	dwRetCode = AxmMoveStartMultiPos(lAxisCnt, lpAxisNo, dpPos, dpVel, dpAcc, dpDec);
	if (dwRetCode != AXT_RT_SUCCESS)
	{
		strData.Format(_T("AxmMoveStartMultiPos Return Error[Code:%04d]"), dwRetCode);
		return FALSE;
	}
	*/
	return TRUE;
}

bool CAjinMotion::GantryEnable(long lAxisMaster, long lAxisSlave)
{
	DWORD dwRet;

	dwRet = AxmGantrySetEnable(lAxisMaster, lAxisSlave, 0, 0, 0);
	if(dwRet != AXT_RT_SUCCESS)
	{
		return false;
	}

	return TRUE;
}

bool CAjinMotion::GantryDisEnable(long lAxisMaster, long lAxisSlave)
{
	DWORD dwRet;

	dwRet = AxmGantrySetDisable(lAxisMaster, lAxisSlave);
	if(dwRet != AXT_RT_SUCCESS)
	{
		return false;
	}

	return TRUE;
}

bool CAjinMotion::GetGantryModeEnable()
{
	DWORD dwRet;

	AxmGantryGetEnable((long)0, NULL, NULL, NULL, &dwRet);
	if(dwRet != ENABLE)
	{
		return false;
	}

	return TRUE;
}
