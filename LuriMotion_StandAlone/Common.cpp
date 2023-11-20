#include <math.h>

#include "pch.h"
#include "Common.h"
#include "DlgMain.h"
#include "LuriMotion_OQA_SFR_Wide.h"
#include "LuriMotion_OQA_SFR_WideDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNAMIC(CCommon, CWnd)

CCommon::CCommon(void)
{
	m_bThreadShowMsg = FALSE;
	m_closeEvent = NULL;

	m_nMsg_NoCloseFlag = 0;

	m_nTestFlag = 0;
	m_strid = _T("OPER");
	m_nGrade = 0;

	m_bFlagSampleInput_Block_Foced = FALSE;
	m_bFlagSampleOutput_Block_Foced = FALSE;

	m_bRecipeChange = FALSE;
	COMMON->SetEMODisable(FALSE);

	for( int i=0; i<5; i++)
	{
		m_bConnectTesterFlag[i] = FALSE;
	}

	for (int i = 0; i < 10; i++)
	{
		m_bMotorOrginFlag[i] = FALSE;
	}

	m_bOriginPopUpCheck = false;
	m_bFirstInit = false;
	m_bEMO_All_InitFlag = false;

	m_bFlag_StopSwitchPushed = FALSE;
	COMMON->m_bFlag_AreaStop_CancelMode = FALSE;

	COMMON->SetClearCounts();
	COMMON->SetReplaceFlag(FALSE);
	// [21.1018.2]
	m_bLotoMode = FALSE;
	COMMON->SetMOAllInitNeedFlag(TRUE);
}

CCommon::~CCommon(void)
{
}

BEGIN_MESSAGE_MAP(CCommon, CWnd)
	ON_WM_TIMER()
END_MESSAGE_MAP()


BOOL CCommon::ShowMsgDlg_Domodal(CString strInfo, int nMode)
{
	// [21.1208.2] Logging
	WRITE_MAIN_LOG(strInfo);
	CLuriMotion_OQA_SFR_WideDlg *pParrentDlg = (CLuriMotion_OQA_SFR_WideDlg*)AfxGetApp()->GetMainWnd();
	// [22.0117.2]
	pParrentDlg->m_pDlgMain->EnableWindow(FALSE);
	pParrentDlg->m_pDlgSide->EnableWindow(FALSE);
	pParrentDlg->m_pDlgBtm->EnableWindow(FALSE);
	pParrentDlg->m_pDlgTitle->EnableWindow(FALSE);
	pParrentDlg->m_pDlgTeach->EnableWindow(FALSE);
	pParrentDlg->m_pDlgIO->EnableWindow(FALSE);

	if (pParrentDlg->RunMessageDlg_Domodal(strInfo, nMode) == TRUE)
	{
		pParrentDlg->m_pDlgMain->EnableWindow(TRUE);
		pParrentDlg->m_pDlgSide->EnableWindow(TRUE);
		pParrentDlg->m_pDlgBtm->EnableWindow(TRUE);
		pParrentDlg->m_pDlgTitle->EnableWindow(TRUE);
		pParrentDlg->m_pDlgTeach->EnableWindow(TRUE);
		pParrentDlg->m_pDlgIO->EnableWindow(TRUE);
		return TRUE;
	}
	pParrentDlg->m_pDlgMain->EnableWindow(TRUE);
	pParrentDlg->m_pDlgSide->EnableWindow(TRUE);
	pParrentDlg->m_pDlgBtm->EnableWindow(TRUE);
	pParrentDlg->m_pDlgTitle->EnableWindow(TRUE);
	pParrentDlg->m_pDlgTeach->EnableWindow(TRUE);
	pParrentDlg->m_pDlgIO->EnableWindow(TRUE);
	return FALSE;
}


void CCommon::ShowMsg_Thread(CString strMsg, int nMode, bool bTesterAlarm)
{
	m_bThreadShowMsg = TRUE;
	m_ParamMsg.strMsg = strMsg;
	m_ParamMsg.nMode = nMode;
	m_ParamMsg.bTesterAlarm = bTesterAlarm;

	//m_ctrlOthers[0].Send_Stop(0);

	//if (AJIN_IO->GetOutputIoBit(DIO_OUTPUT_Y0_09_TOWERLAMP_BEEP) == FALSE)
	//		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_09_TOWERLAMP_BEEP, TRUE);

	AfxBeginThread(Thread_ShowMsg, &m_ParamMsg, THREAD_PRIORITY_HIGHEST);
}
UINT CCommon::Thread_ShowMsg(LPVOID lpParam)
{
	THREAD_PARAM *pParam = (THREAD_PARAM *)lpParam;

	COMMON->Show_MsgBox(pParam->strMsg, pParam->nMode);
	COMMON->m_bThreadShowMsg = FALSE;

	return 0;
}

INT_PTR CCommon::Show_MsgBox(CString strMsg, int nMode)
{
	CLuriMotion_OQA_SFR_WideDlg *pParrentDlg = (CLuriMotion_OQA_SFR_WideDlg*)AfxGetApp()->GetMainWnd();
	CDlgMessageBox_Instance *pUserMsgDlg = CDlgMessageBox_Instance::Get_Instance();
	
	int nSuppored = 0;
	LARGE_INTEGER llFreq;

	nSuppored = QueryPerformanceFrequency(&llFreq);
	QueryPerformanceCounter(&m_llErrorStartTime_DoModal);

	pUserMsgDlg->SetMsg(strMsg, nMode);
	// [22.0117.3]
	pParrentDlg->m_pDlgMain->EnableWindow(FALSE);
	pParrentDlg->m_pDlgSide->EnableWindow(FALSE);
	pParrentDlg->m_pDlgBtm->EnableWindow(FALSE);
	pParrentDlg->m_pDlgTitle->EnableWindow(FALSE);
	pParrentDlg->m_pDlgTeach->EnableWindow(FALSE);
	pParrentDlg->m_pDlgIO->EnableWindow(FALSE);

	switch (nMode)
	{
	case MODE_QUESTION:
		break;

	case MODE_ERROR:	//검사 시퀀스 중 Time Out Error 발생 시
		Close_MsgBox();

		m_nTestFlag = 1;
		break;

	case MODE_INFORMATION:	//Sequence 중 사용자에게 정보 표시를 위한 메세지일 경우에는  MSGDLG_CLOSE_TIME 만큰 표시 후 닫는다
		
		m_nTestFlag = 2;
		break;
	}
	
	m_closeEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	pUserMsgDlg->ShowWindow(SW_SHOW);
	// [22.0401.2]
	//pUserMsgDlg->BringWindowToTop();

	::WaitForSingleObject(m_closeEvent, INFINITE);
	CloseHandle(m_closeEvent);
	m_closeEvent = NULL;
	// [22.0117.4]
	pParrentDlg->m_pDlgMain->EnableWindow(TRUE);
	pParrentDlg->m_pDlgSide->EnableWindow(TRUE);
	pParrentDlg->m_pDlgBtm->EnableWindow(TRUE);
	pParrentDlg->m_pDlgTitle->EnableWindow(TRUE);
	pParrentDlg->m_pDlgTeach->EnableWindow(TRUE);
	pParrentDlg->m_pDlgIO->EnableWindow(TRUE);

	INT_PTR nReturn = 0;
	nReturn = pUserMsgDlg->m_nReturn;

	if (m_nMsgTimerOpenFlag == 1)
	{
		m_nMsgTimerOpenFlag = 0;
		KillTimer(100);
	}

	switch (nMode)
	{
	case MODE_QUESTION:
		break;

	case MODE_ERROR:				//검사 시퀀스 중 Time Out Error 발생 시
		break;

	case MODE_INFORMATION:
		break;

	}
	return TRUE;
}
void CCommon::Close_MsgBox(void)
{
	
	CDlgMessageBox_Instance *pUserMsgDlg = CDlgMessageBox_Instance::Get_Instance();

	//HWND hWnd = ::FindWindow(NULL, _T("User Message Box"));	
	//::SendMessage(hWnd, WM_CLOSE, NULL, NULL);

	pUserMsgDlg->ShowWindow(SW_HIDE);
	::SetEvent(m_closeEvent);

}

//====================================================================================
BOOL CCommon::Wait_Time(double msec, BOOL *pKill)
{
	LARGE_INTEGER start, end;
	double delay = 0;

	LARGE_INTEGER   freq;
	int bSuppored = QueryPerformanceFrequency(&freq);	//tick을 구함.

	QueryPerformanceCounter(&start);
	while (1)
	{
		QueryPerformanceCounter(&end);
		delay = (double)(end.QuadPart - start.QuadPart) / freq.QuadPart * 1000;
		if (delay > msec)
		{
			return  FALSE;
		}
		if (*pKill == TRUE)
		{
			return TRUE;
		}
		Sleep(1);
		DoEvents();
	}

	return FALSE;
}

void CCommon::DoEvent(int cnt)
{
	MSG msg;
	for (int i = 0; i < cnt; i++)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// If it's a quit message, we're out of here.
		//	if (msg.message == WM_QUIT) 	return 1; 
			// Otherwise, dispatch the message.
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} // End of PeekMessage while loop.
		Sleep(1);
	}
}



bool CCommon::DoEvents(void)
{
	MSG msg;
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		if (msg.message == 0)
		{
			return false;
		}
	}

	return true;
}

void CCommon::uSleep_NoReturn(double msec)
{
	LARGE_INTEGER start, end;
	double delay = 0;

	LARGE_INTEGER freq;
	int bSuppored = QueryPerformanceFrequency(&freq);	//tick을 구함.

	QueryPerformanceCounter(&start);

	while (1)
	{
		QueryPerformanceCounter(&end);
		delay = (double)(end.QuadPart - start.QuadPart) / freq.QuadPart * 1000;
		if (delay > msec)
		{
			return;
		}
		Sleep(1);
		DoEvents();
	}

	return;
}



//=====================================================================================================
// 
void CCommon::TESTRUN()
{
	
}

void CCommon::OnTimer(UINT_PTR nIDEvent)
{
	CWnd::OnTimer(nIDEvent);

	switch (nIDEvent)
	{
	case 1:
	{
	}
	break;

	case 100:
	{
		KillTimer(100);
		m_nMsgTimerOpenFlag = 0;
		Close_MsgBox();
	}
	break;

	}
}


void CCommon::Write_Main_Log(CString strLog)
{
	CDlgMain *pMain = CDlgMain::Get_Instance();

	pMain->Write_Main_Log(strLog);
}

void CCommon::Write_Comm_Log(CString strLog)
{
	CDlgMain *pMain = CDlgMain::Get_Instance();
	pMain->Write_Comm_Log(strLog);
}
void CCommon::Write_Barcode_Log(CString strLog)
{
	CDlgMain *pMain = CDlgMain::Get_Instance();
	pMain->Write_Barcode_Log(strLog);
}
void CCommon::Write_Sequence_Log(CString strLog)
{
	CDlgMain *pMain = CDlgMain::Get_Instance();
	pMain->Write_Sequence_Log(strLog);	
}

//-------------------------------------------------
// Permission
void	CCommon::SetPermissionID(CString strID)
{
	m_strid = strID;
}
void	CCommon::SetPermissionGrade(int nGrade)
{
	m_nGrade = nGrade;
}

CString CCommon::GetPermissionId()
{
	return m_strid;
}
int	CCommon::GetPermissionGrade()
{
	return m_nGrade;
}

//-------------------------------------------------
// Loto [21.1018.6] Add
void	CCommon::SetLotoMode(BOOL b)
{
	COMMON->m_bLotoMode = b;
}

BOOL CCommon::GetLotoMode()
{
	return COMMON->m_bLotoMode;
}

BOOL CCommon::Check_EMO()
{
	if (AJIN_IO->m_bInitDIO == FALSE)
		return FALSE;

	if (FALSE == AJIN_IO->GetInputIoBit(DIO_INPUT_X0_04_EMO))
	{	
		EQ_BASIC_PARAM	stEq = *DATAMANAGER->GetCurrentEqBasicData();
		SEQUENCE->Stop_Run();
		if (FAS_MO->IsConnected() == true) FAS_MO->Disconnect_EziServo();

		//SEQUENCE->m_bRunMode = eError;
		COMMON->SetMOAllInitNeedFlag(TRUE);
		//COMMON->SetTowerLampStatus(eModeError);
		COMMON->SetEqStatus(eError);

		if(stEq.nLanguage ==0)
			COMMON->ShowMsgDlg_Domodal(_T("[Auto] EMO버튼이 눌렸습니다. \nEMO 해제후 Reset 버튼을 눌러주십시오."), MODE_ERROR);
		else
			COMMON->ShowMsgDlg_Domodal(_T("[Auto] The EMO button has been pressed. \nPlease press the Reset button after releasing the EMO."), MODE_ERROR);
		bEMOResetFlag = TRUE;
		return TRUE;
	}
	else if (TRUE == bEMOResetFlag)
	{
		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_09_TOWERLAMP_BEEP1, FALSE);
		bEMOResetFlag = FALSE;
	}
	return TRUE;
}



//-------------------------------------------------
// Side Button Function
BOOL	CCommon::Check_Mch_Start()
{
	CString strMsg;
	// 1. 통신 연결이 되었는가?
	if (COMMON->GetDryRunModeFlag() == FALSE)
	{
		if (COMMON->m_bConnectTesterFlag[0] == FALSE)
		{
			strMsg.Format(_T("Tester 0 is Not Connected.\n"));
			Write_Main_Log(strMsg);
			return FALSE;
		}
		if (COMMON->m_bConnectTesterFlag[1] == FALSE)
		{
			strMsg.Format(_T("Tester 1 is Not Connected.\n"));
			Write_Main_Log(strMsg);
			return FALSE;
		}
	}

#	// 2. 모터가 초기화가 되어있는가?
	BOOL bTemp = FALSE;
	for (int i = 0; i < (int)MOTOR_MAX_COUNT; i++)
	{
		bTemp = FAS_MO->IsHomeOK(i);
		
		if (bTemp  == FALSE)
		{
			strMsg.Format(_T("%d Motor is Not Initialize.\n"), i);
			Write_Main_Log(strMsg);
			return FALSE;
		}
			
	}
	
	// 3. 모터는 정상인가?
	// 4. Door Lock 은 닫혀있는가?
	if (TRUE != GetInterlock_DoorCheck())
		return FALSE;

	//소켓 실린더 체크 추가 필요
	/*
	// 5. 카메라는 제대로 올려 있는가?
	if (AJIN_IO->GetInputIoBit(DIO_INPUT_X0_08_MANUALSOCEKT_OPENCLOSE) == FALSE)
	{
		strMsg.Format(_T("Camera is Not Exist.\n"));
		Write_Main_Log(strMsg);
		return FALSE;
	}
	*/
	return TRUE;
}
BOOL	CCommon::Mch_Pause()
{
	return TRUE;
}
BOOL	CCommon::Mch_Stop()
{
	return TRUE;
}

BOOL CCommon::Check_Socket()
{
	//if (FAS_MO->IsMotioning(SOCKET_Y) != FALSE)
	//	return TRUE;
	//if (FAS_MO->IsMotioning(SOCKET_Z) != FALSE)
	//	return TRUE;

	//TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();
	//if (FAS_MO->GetAxisCurrnetActualPos(SOCKET_Y) < stTeaching.nSocketY_Wait_Pos + 1)
	//	return TRUE;

#ifndef _DRY_RUN_MODULE_NONE
	#if (GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL) || (GET_INSPECTOR == SYS_OQC_SFR_SINGLE_CL)
		if (TRUE != AJIN_IO->GetInputIoBit(DIO_INPUT_X0_08_CAMERA_DETECT_SENSOR) && SEQUENCE->GetRunMode() == eRunning)
	#else
		//if ((TRUE != AJIN_IO->GetInputIoBit(DIO_INPUT_X0_08_MANUALSOCEKT_OPENCLOSE1) || (TRUE != AJIN_IO->GetInputIoBit(DIO_INPUT_X0_09_MANUALSOCEKT_OPENCLOSE2)))
		//	&& SEQUENCE->GetRunMode() == eRunning)
		if (false)
	#endif
		{
			EQ_BASIC_PARAM	stEq = *DATAMANAGER->GetCurrentEqBasicData();
			if (stEq.nLanguage == 0)
				COMMON->ShowMsgDlg_Domodal(_T("카메라가 감지되지않습니다. 자동작업을 종료합니다."), MODE_INFORMATION);
			else
				COMMON->ShowMsgDlg_Domodal(_T("Camera is not detected. End the automatic operation."), MODE_INFORMATION);
			return FALSE;
		}
#endif
	return TRUE;
}

BOOL	CCommon::Input_Block()
{
	// 앞장비에서 자재 들어오는것 막기.
	// Output. Io.
	return TRUE;
}
BOOL	CCommon::Output_Block()
{
	// 자재가 뒷 장비로 넘어가는것 막기.
	return TRUE;
}
BOOL	CCommon::Check_Mch_DryRun()
{
	// 드라이런 모드 설정.
	return TRUE;
}

void CCommon::SetOriginPopUp()
{
	m_bOriginPopUpCheck = true;
}

void CCommon::ResetOriginPopUp()
{
	m_bOriginPopUpCheck = false;
}

bool CCommon::GetOriginPopUp()
{
	return m_bOriginPopUpCheck;
}

void CCommon::SetFirstInit()
{
	m_bFirstInit = true;
}

bool CCommon::GetFirstInit()
{
	return m_bFirstInit;
}

BOOL CCommon::GetEMOAllInitFlag()
{
	return m_bEMO_All_InitFlag;
}

void CCommon::SetEMOAllInitFlag(BOOL bState)
{
	m_bEMO_All_InitFlag = bState;
}
//-------------------------------------------------
// Convayor 선택 FUnction
BOOL	CCommon::Convayor1_Sensor_Select(int nPos)
{
	return TRUE;
}

BOOL	CCommon::Convayor2_Sensor_Select(int nPos)
{
	return TRUE;
}

BOOL	CCommon::Convayor3_Sensor_Select(int nPos)
{
	return TRUE;
}

BOOL	CCommon::Convayor1_Sensor_Select_Clear()
{
	return TRUE;
}

BOOL	CCommon::Convayor2_Sensor_Select_Clear()
{
	return TRUE;

}

BOOL	CCommon::Convayor3_Sensor_Select_Clear()
{
	return TRUE;
}

void	CCommon::SetErrorLog_Write(CString strNumber)
{
	ERROR_LIST		stErr = *DATAMANAGER->GetCurrentErrorList();

	stErr.nErrorCount++;
	stErr.vtErrorNum.push_back(strNumber);

	DATAMANAGER->SetErrorListData(stErr);
}
BOOL CCommon::GetDryRunModeFlag()
{
	return m_bFlagDryRunMode;
}
void CCommon::SetDryRunModeFlag(BOOL bState)
{
	m_bFlagDryRunMode = bState;
}
BOOL CCommon::GetFlagStartUpMode()
{
	return m_bFlagStartUpMode;
}
void CCommon::SetFlagStartUpMode(BOOL bState)
{
	m_bFlagStartUpMode = bState;
}
int CCommon::GetFlagStartUpModeAction()
{
	return COMMON->m_nFlagStartUpModeAction;
}
void CCommon::SetFlagStartUpModeAction(int nAction)
{
	COMMON->m_nFlagStartUpModeAction = nAction;
}

CString CCommon::GetErrorString(__in int errcode)
{
	return GetErrorName_EachLanguage(errcode);
}

CString CCommon::GetErrorName_EachLanguage(int nErrorNum)
{
	ERROR_LIST		stErr = *DATAMANAGER->GetCurrentErrorList();
	EQ_BASIC_PARAM	stEq = *DATAMANAGER->GetCurrentEqBasicData();

	CString strErrName = _T("");
	CString strErrTemp = _T("");

	int nNumber = 0;
	int nPart = 0;
	int nSub = 0;

	nPart = nErrorNum / 100;
	nSub = nErrorNum % 100;

	if (nPart == 1)
		nNumber = nSub;
	else if (nPart == 2)
		nNumber = stErr.nErrorNameCount_100 + nSub;
	else if (nPart == 3)
		nNumber = stErr.nErrorNameCount_100 + stErr.nErrorNameCount_200 + nSub;

	if (stEq.nLanguage == 0)
	{
		if (nNumber < stErr.vtErr_Kor.size())
			strErrTemp = stErr.vtErr_Kor.at(nNumber);
		strErrName.Format(_T("%s"), strErrTemp);
	}
	else if (stEq.nLanguage == 1)
	{
		if (nNumber < stErr.vtErr_Eng.size())
			strErrTemp = stErr.vtErr_Eng.at(nNumber);
		strErrName.Format(_T("%s"), strErrTemp);
	}
	else if (stEq.nLanguage == 2)
	{
		if (nNumber < stErr.vtErr_Viet.size())
			strErrTemp = stErr.vtErr_Viet.at(nNumber);
		strErrName.Format(_T("%s"), strErrTemp);
	}

	return strErrName;
}


BOOL	CCommon::GetInterlock_DoorCheck()
{
	// 근접센서는 문이 닫혀있으면 FALSE
	// 도어락은 OUTPUT에서 On을 해야  TRUE가 뜬다.
	if (COMMON->GetPermissionGrade() != 0)
		return TRUE;

	EQ_BASIC_PARAM	stEq = *DATAMANAGER->GetCurrentEqBasicData();

	BOOL bRet_Front_Near = TRUE;
	BOOL bRet_Left1_Near = TRUE;
	BOOL bRet_Left2_Near = TRUE;
	BOOL bRet_Right1_Near = TRUE;
	BOOL bRet_Right2_Near = TRUE;
	BOOL bRet_Back1_Near = TRUE;
	BOOL bRet_Back2_Near = TRUE;

#if(GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL || GET_INSPECTOR == SYS_DISTORTION_30)
	bRet_Front_Near = AJIN_IO->GetInputIoBit(DIO_INPUT_X0_10_FRONT_DOORLOCK_OPENCLOSE);
	bRet_Left1_Near = AJIN_IO->GetInputIoBit(DIO_INPUT_X0_11_LEFT_DOORLOCK1_OPENCLOSE);
	bRet_Left2_Near = AJIN_IO->GetInputIoBit(DIO_INPUT_X0_12_LEFT_DOORLOCK2_OPENCLOSE);
	bRet_Right1_Near = AJIN_IO->GetInputIoBit(DIO_INPUT_X0_13_RIGHT_DOORLOCK1_OPENCLOSE);
	bRet_Right2_Near = AJIN_IO->GetInputIoBit(DIO_INPUT_X0_14_RIGHT_DOORLOCK2_OPENCLOSE);
	bRet_Back1_Near = AJIN_IO->GetInputIoBit(DIO_INPUT_X0_15_BACK_DOORLOCK1_OPENCLOSE);
	bRet_Back2_Near = AJIN_IO->GetInputIoBit(DIO_INPUT_X0_16_BACK_DOORLOCK2_OPENCLOSE);
#elif(GET_INSPECTOR == SYS_OQC_SFR_SINGLE_CL)
	bRet_Front_Near = AJIN_IO->GetInputIoBit(DIO_INPUT_X0_10_FRONT_DOORLOCK_OPENCLOSE);
	bRet_Left1_Near = AJIN_IO->GetInputIoBit(DIO_INPUT_X0_11_LEFT_DOORLOCK1_OPENCLOSE);
	bRet_Left2_Near = FALSE;
	bRet_Right1_Near = AJIN_IO->GetInputIoBit(DIO_INPUT_X0_12_RIGHT_DOORLOCK1_OPENCLOSE);
	bRet_Right2_Near = FALSE;
	bRet_Back1_Near = AJIN_IO->GetInputIoBit(DIO_INPUT_X0_13_BACK_DOORLOCK1_OPENCLOSE);
	bRet_Back2_Near = AJIN_IO->GetInputIoBit(DIO_INPUT_X0_14_BACK_DOORLOCK2_OPENCLOSE);
#endif

	CString strErrName = _T("");

	//if (bRet_Front_Near == TRUE)
	//{
	//	//도어(전면) 센서가 감지되지 않습니다. 문을 닫아주세요.
	//	strErrName = GetErrorName_EachLanguage(1000);
	//	if(stEq.nLanguage ==0)
	//		WRITE_MAIN_LOG(_T("도어(전면) 센서가 감지되지 않습니다.문을 닫아주세요\n"));
	//	else
	//		WRITE_MAIN_LOG(_T("The door (front) sensor is not detected.Please close the door.\n"));

	//	SetErrorLog_Write(_T("1000"));
	//}
	//if (bRet_Left1_Near == TRUE)
	//{
	//	//도어(좌측면) 센서가 감지되지 않습니다. 문을 닫아주세요.
	//	strErrName = GetErrorName_EachLanguage(1001);
	//	if (stEq.nLanguage == 0)
	//		WRITE_MAIN_LOG(_T("도어(좌측1) 센서가 감지되지 않습니다.문을 닫아주세요\n"));
	//	else
	//		WRITE_MAIN_LOG(_T("The door (Left1) sensor is not detected.Please close the door.\n"));

	//	SetErrorLog_Write(_T("1001"));
	//}
	//if (bRet_Left2_Near == TRUE)
	//{
	//	//도어(좌측면) 센서가 감지되지 않습니다. 문을 닫아주세요.
	//	strErrName = GetErrorName_EachLanguage(1001);
	//	if (stEq.nLanguage == 0)
	//		WRITE_MAIN_LOG(_T("도어(좌측2) 센서가 감지되지 않습니다.문을 닫아주세요\n"));
	//	else
	//		WRITE_MAIN_LOG(_T("The door (Left2) sensor is not detected.Please close the door.\n"));

	//	SetErrorLog_Write(_T("1001"));
	//}
	//if (bRet_Right1_Near == TRUE)
	//{
	//	//도어(좌측면) 센서가 감지되지 않습니다. 문을 닫아주세요.
	//	strErrName = GetErrorName_EachLanguage(1001);
	//	if (stEq.nLanguage == 0)
	//		WRITE_MAIN_LOG(_T("도어(우측1) 센서가 감지되지 않습니다.문을 닫아주세요\n"));
	//	else
	//		WRITE_MAIN_LOG(_T("The door (right1) sensor is not detected.Please close the door.\n"));

	//	SetErrorLog_Write(_T("1001"));
	//}
	//if (bRet_Right2_Near == TRUE)
	//{
	//	//도어(좌측면) 센서가 감지되지 않습니다. 문을 닫아주세요.
	//	strErrName = GetErrorName_EachLanguage(1001);
	//	if (stEq.nLanguage == 0)
	//		WRITE_MAIN_LOG(_T("도어(우측2) 센서가 감지되지 않습니다.문을 닫아주세요\n"));
	//	else
	//		WRITE_MAIN_LOG(_T("The door (right2) sensor is not detected.Please close the door.\n"));

	//	SetErrorLog_Write(_T("1001"));
	//}
	//if (bRet_Back1_Near == TRUE)
	//{
	//	//도어(후면1) 센서가 감지되지 않습니다. 문을 닫아주세요.
	//	strErrName = GetErrorName_EachLanguage(1003);
	//	if (stEq.nLanguage == 0)
	//		WRITE_MAIN_LOG(_T("도어(후면1) 센서가 감지되지 않습니다.문을 닫아주세요\n"));
	//	else
	//		WRITE_MAIN_LOG(_T("The door (Back1) sensor is not detected.Please close the door.\n"));

	//	SetErrorLog_Write(_T("1003"));
	//}
	//if (bRet_Back2_Near == TRUE)
	//{
	//	//도어(후면2) 센서가 감지되지 않습니다. 문을 닫아주세요.
	//	strErrName = GetErrorName_EachLanguage(1004);
	//	if (stEq.nLanguage == 0)
	//		WRITE_MAIN_LOG(_T("도어(후면2) 센서가 감지되지 않습니다.문을 닫아주세요\n"));
	//	else
	//		WRITE_MAIN_LOG(_T("The door (back2) sensor is not detected.Please close the door.\n"));
	//	SetErrorLog_Write(_T("1004"));
	//}

	if ((bRet_Front_Near == FALSE && bRet_Left1_Near == FALSE && bRet_Left2_Near == FALSE && bRet_Right1_Near == FALSE && bRet_Right2_Near == FALSE && bRet_Back1_Near == FALSE && bRet_Back2_Near == FALSE))
		return TRUE;

	return FALSE;
}

enMCErrCode	CCommon::GetInterlock_AreaSensor()
{
	if (SEQUENCE->GetRunMode() == eStandby)
		return MCEC_OK;

	BOOL bRet1 = TRUE;
	EQ_BASIC_PARAM	stEq = *DATAMANAGER->GetCurrentEqBasicData();
	CString strErrName = _T("");

	bRet1 = AJIN_IO->GetInputIoBit(DIO_INPUT_X0_06_AREASENSOR1);

	if (bRet1 == FALSE)
	{
		//if (COMMON->GetFlagStartUpMode() == TRUE)
		//{
		//	FAS_MO->MoveStop(SOCKET_Y);
		//	FAS_MO->MoveStop(SOCKET_Z);
		//	COMMON->SetEqStatus(eError);
		//	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_09_TOWERLAMP_BEEP1, TRUE);

		//	/*if (stEq.nLanguage == 0)
		//		strErrName.Format(_T("Area 센서 감지되었습니다. 대기위치로 이동합니다."));
		//	else
		//		strErrName.Format(_T("Area sensor detected. Go to the standby position."));

		//	if (COMMON->ShowMsgDlg_Domodal(strErrName, MODE_INFORMATION) == IDOK)
		//	{*/
		//		AJIN_IO->ChangeBtnStatus(DIO_OUTPUT_Y0_02_STOP_SWITCH_LAMP, 0, TRUE);
		//		COMMON->SetAreaStop_CancelPush(TRUE);
		//		SEQUENCE->Pause_Run();
		//	//}
		//	SEQUENCE->Reset_SeqFlags();
		//	COMMON->SetFlagStartUpModeAction(0);

		//	return MCEC_AreaSensorDetect;
		//}

		FAS_MO->MoveStop(SOCKET_Z);
		FAS_MO->MoveStop(SOCKET_Y);

		COMMON->SetEqStatus(eError);
		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_09_TOWERLAMP_BEEP1, TRUE);

		//if (stEq.nLanguage == 0)
		//	strErrName.Format(_T("Area 센서 감지되었습니다. 장비를 정지시킵니다."));
		//else
		//	strErrName.Format(_T("Area sensor detected. Stop the equipment."));
		//if (COMMON->ShowMsgDlg_Domodal(strErrName, MODE_NO_CLOSE) == IDOK) // "OK 선택"
		//{
			if (COMMON->GetAreaStop_CancelPush() != TRUE)
			{
				AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_09_TOWERLAMP_BEEP1, FALSE);
				AJIN_IO->ChangeBtnStatus(DIO_OUTPUT_Y0_01_START_SWITCH_LAMP, 0, TRUE); // 반짝
				COMMON->SetAreaStop_OkPush(TRUE);
			}
			else
			{
				AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_09_TOWERLAMP_BEEP1, FALSE);
				AJIN_IO->ChangeBtnStatus(DIO_OUTPUT_Y0_02_STOP_SWITCH_LAMP, 0, TRUE);
				COMMON->SetAreaStop_CancelPush(TRUE);
			}

			COMMON->SetEqStatus(eStop);
			return MCEC_AreaSensorDetect;
		//}
		//else // "CANCEL 선택"
		//{		
		//	if (COMMON->GetAreaStop_OkPush() != TRUE)
		//	{
		//		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_09_TOWERLAMP_BEEP1, FALSE);
		//		AJIN_IO->ChangeBtnStatus(DIO_OUTPUT_Y0_02_STOP_SWITCH_LAMP, 0, TRUE); // 반짝
		//		COMMON->SetAreaStop_CancelPush(TRUE);
		//	}
		//	else
		//	{
		//		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_09_TOWERLAMP_BEEP1, FALSE);
		//		AJIN_IO->ChangeBtnStatus(DIO_OUTPUT_Y0_01_START_SWITCH_LAMP, 0, TRUE);
		//		COMMON->SetAreaStop_OkPush(TRUE);
		//	}
		//	COMMON->SetEqStatus(eStop);
		//	return MCEC_AreaSensorDetect;
		//}
	}
	else
		return MCEC_OK;
}

BOOL	CCommon::GetInterlock_CameraDetected()
{
	BOOL bRet = TRUE;
	CString strErrName = _T("");

#if (GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL) || (GET_INSPECTOR == SYS_OQC_SFR_SINGLE_CL)
	bRet = AJIN_IO->GetInputIoBit(DIO_INPUT_X0_08_CAMERA_DETECT_SENSOR);

	/*if (bRet == FALSE)
	{
		strErrName = GetErrorName_EachLanguage(2005);
		WRITE_MAIN_LOG(strErrName);
	}*/

#endif
	if (bRet == TRUE)
		return TRUE;

	return FALSE;
}

BOOL CCommon::GetInterlock_SocketClose()
{
	BOOL bRet = TRUE;
	BOOL bRet2 = TRUE;
	CString strErrName = _T("");

#if (GET_INSPECTOR == SYS_DISTORTION_30)
	bRet = AJIN_IO->GetInputIoBit(DIO_INPUT_X0_08_MANUALSOCEKT_OPENCLOSE1);
	bRet2 = AJIN_IO->GetInputIoBit(DIO_INPUT_X0_09_MANUALSOCEKT_OPENCLOSE2);

	return true;

	if (bRet == FALSE || bRet2 == FALSE)
	{
		/*strErrName = GetErrorName_EachLanguage(5);
		WRITE_MAIN_LOG(strErrName);*/
		return FALSE;
	}
#endif
	return TRUE;
}

BOOL	CCommon::GetInterlock_SocketInAndDown()
{
	BOOL bRet = TRUE;
	CString strErrName = _T("");

#if (GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL) || (GET_INSPECTOR == SYS_OQC_SFR_SINGLE_CL)
	bRet = AJIN_IO->GetInputIoBit(DIO_INPUT_X0_1C_SOCKET_IN);
	bRet &= AJIN_IO->GetInputIoBit(DIO_INPUT_X0_1F_SOCKET_DOWN);

	if (bRet == FALSE)
	{
		/*strErrName = GetErrorName_EachLanguage(2006);
		WRITE_MAIN_LOG(strErrName);*/
	}
#endif

	return bRet;
}

BOOL	CCommon::GetInterlock_SocketIn()
{
	BOOL bRet = TRUE;
	CString strErrName = _T("");

#if (GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL) || (GET_INSPECTOR == SYS_OQC_SFR_SINGLE_CL)
	bRet = AJIN_IO->GetInputIoBit(DIO_INPUT_X0_1C_SOCKET_IN);

	if (bRet == FALSE)
	{
		/*strErrName = GetErrorName_EachLanguage(2006);
		WRITE_MAIN_LOG(strErrName);*/
	}
#endif

	return bRet;
}

void	CCommon::SetDoorLock(enDoorLIst DoorId, BOOL action)
{
	LONG io;

#if(GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL)
	switch (DoorId)
	{
	case EDoor_Front:
		io = DIO_OUTPUT_Y0_10_DOORLOCK_ONOFF_FRONT;
		break;
	case EDoor_Left_Front:
		io = DIO_OUTPUT_Y0_11_DOORLOCK_ONOFF_LEFT1;
		break;
	case EDoor_Left_Rear:
		io = DIO_OUTPUT_Y0_12_DOORLOCK_ONOFF_LEFT2;
		break;
	case EDoor_Right_Front:
		io = DIO_OUTPUT_Y0_13_DOORLOCK_ONOFF_RIGHT1;
		break;
	case EDoor_Right_Rear:
		io = DIO_OUTPUT_Y0_14_DOORLOCK_ONOFF_RIGHT2;
		break;
	case EDoor_Back_Left:
		io = DIO_OUTPUT_Y0_15_DOORLOCK_ONOFF_BACK1;
		break;
	case EDoor_Back_Right:
		io = DIO_OUTPUT_Y0_16_DOORLOCK_ONOFF_BACK2;
		break;
	}
#elif(GET_INSPECTOR == SYS_OQC_SFR_SINGLE_CL)
	switch (DoorId)
	{
	case EDoor_Front:
		io = DIO_OUTPUT_Y0_10_DOORLOCK_ONOFF_FRONT;
		break;
	case EDoor_Left:
		io = DIO_OUTPUT_Y0_11_DOORLOCK_ONOFF_LEFT1;
		break;
	case EDoor_Right:
		io = DIO_OUTPUT_Y0_12_DOORLOCK_ONOFF_RIGHT1;
		break;
	case EDoor_Back_Left:
		io = DIO_OUTPUT_Y0_13_DOORLOCK_ONOFF_BACK1;
		break;
	case EDoor_Back_Right:
		io = DIO_OUTPUT_Y0_14_DOORLOCK_ONOFF_BACK2;
		break;
	}
#elif(GET_INSPECTOR == SYS_DISTORTION_30)
	switch (DoorId)
	{
	case EDoor_Front:
		io = DIO_OUTPUT_Y0_10_DOORLOCK_ONOFF_FRONT;
		break;
	case EDoor_Left_Front:
		io = DIO_OUTPUT_Y0_11_DOORLOCK_ONOFF_LEFT1;
		break;
	case EDoor_Left_Rear:
		io = DIO_OUTPUT_Y0_12_DOORLOCK_ONOFF_LEFT2;
		break;
	case EDoor_Right_Front:
		io = DIO_OUTPUT_Y0_13_DOORLOCK_ONOFF_RIGHT1;
		break;
	case EDoor_Right_Rear:
		io = DIO_OUTPUT_Y0_14_DOORLOCK_ONOFF_RIGHT2;
		break;
	case EDoor_Back_Left:
		io = DIO_OUTPUT_Y0_15_DOORLOCK_ONOFF_BACK1;
		break;
	case EDoor_Back_Right:
		io = DIO_OUTPUT_Y0_16_DOORLOCK_ONOFF_BACK2;
		break;
	}
#endif
	AJIN_IO->SendOutputBit(io, action);
}

BOOL	CCommon::GetDoorClose(enDoorLIst DoorId)
{
	LONG io;
	BOOL ret;

#if(GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL)
	switch (DoorId)
	{
	case EDoor_Front:
		io = DIO_INPUT_X0_10_FRONT_DOORLOCK_OPENCLOSE;
		break;
	case EDoor_Left_Front:
		io = DIO_INPUT_X0_11_LEFT_DOORLOCK1_OPENCLOSE;
		break;
	case EDoor_Left_Rear:
		io = DIO_INPUT_X0_12_LEFT_DOORLOCK2_OPENCLOSE;
		break;
	case EDoor_Right_Front:
		io = DIO_INPUT_X0_13_RIGHT_DOORLOCK1_OPENCLOSE;
		break;
	case EDoor_Right_Rear:
		io = DIO_INPUT_X0_14_RIGHT_DOORLOCK2_OPENCLOSE;
		break;
	case EDoor_Back_Left:
		io = DIO_INPUT_X0_15_BACK_DOORLOCK1_OPENCLOSE;
		break;
	case EDoor_Back_Right:
		io = DIO_INPUT_X0_16_BACK_DOORLOCK2_OPENCLOSE;
		break;
	}
#elif(GET_INSPECTOR == SYS_OQC_SFR_SINGLE_CL)
	switch (DoorId)
	{
	case EDoor_Front:
		io = DIO_INPUT_X0_10_FRONT_DOORLOCK_OPENCLOSE;
		break;
	case EDoor_Left:
		io = DIO_INPUT_X0_11_LEFT_DOORLOCK1_OPENCLOSE;
		break;
	case EDoor_Right:
		io = DIO_INPUT_X0_12_RIGHT_DOORLOCK1_OPENCLOSE;
		break;
	case EDoor_Back_Left:
		io = DIO_INPUT_X0_13_BACK_DOORLOCK1_OPENCLOSE;
		break;
	case EDoor_Back_Right:
		io = DIO_INPUT_X0_14_BACK_DOORLOCK2_OPENCLOSE;
		break;
	}
#elif(GET_INSPECTOR == SYS_DISTORTION_30)
	switch (DoorId)
	{
	case EDoor_Front:
		io = DIO_INPUT_X0_10_FRONT_DOORLOCK_OPENCLOSE;
		break;
	case EDoor_Left_Front:
		io = DIO_INPUT_X0_11_LEFT_DOORLOCK1_OPENCLOSE;
		break;
	case EDoor_Left_Rear:
		io = DIO_INPUT_X0_12_LEFT_DOORLOCK2_OPENCLOSE;
		break;
	case EDoor_Right_Front:
		io = DIO_INPUT_X0_13_RIGHT_DOORLOCK1_OPENCLOSE;
		break;
	case EDoor_Right_Rear:
		io = DIO_INPUT_X0_14_RIGHT_DOORLOCK2_OPENCLOSE;
		break;
	case EDoor_Back_Left:
		io = DIO_INPUT_X0_15_BACK_DOORLOCK1_OPENCLOSE;
		break;
	case EDoor_Back_Right:
		io = DIO_INPUT_X0_16_BACK_DOORLOCK2_OPENCLOSE;
		break;
	}
#endif

	ret = (AJIN_IO->GetInputIoBit(io) == FALSE ? TRUE : FALSE);
	return(ret);
}

void CCommon::SetOperationMode(__in OperationMode mode)
{
	m_OperationMode = mode;
}

OperationMode CCommon::GetOperationMode()
{
	return m_OperationMode;
}

void	CCommon::SetEqStatus(RUN_MODE rm)
{
	SEQUENCE->SetRunMode(rm);
}

void	CCommon::SetSleepStarted(time_t time)
{
	m_tSleepStart = time;
}

time_t CCommon::GetSleepStarted()
{
	return m_tSleepStart;
}

void	CCommon::SetSleepElapsed(time_t time)
{
	m_tSleepElapsed = time;
}

time_t CCommon::GetSleepElapsed()
{
	return m_tSleepElapsed;
}

void CCommon::SetSleepModeStep(int nMode)
{
	COMMON->m_SleepModeStep = nMode;
}

int CCommon::GetSleepModeStep()
{
	return COMMON->m_SleepModeStep;
}

void CCommon::SetStopSwitchPushed(BOOL bFlag)
{
	m_bFlag_StopSwitchPushed = bFlag;
}
BOOL CCommon::GetStopSwitchPushed()
{
	return m_bFlag_StopSwitchPushed;
}

void CCommon::SetAreaStop_CancelPush(BOOL bFlag)
{
	COMMON->m_bFlag_AreaStop_CancelMode = bFlag;
}
BOOL CCommon::GetAreaStop_CancelPush()
{
	return COMMON->m_bFlag_AreaStop_CancelMode;
}
void CCommon::SetAreaStop_OkPush(BOOL bFlag)
{
	m_bFlag_AreaStop_OkMode = bFlag;
}
BOOL CCommon::GetAreaStop_OkPush()
{
	return m_bFlag_AreaStop_OkMode;
}


void CCommon::SetProbePinCylinder_UpDown(BOOL bFlag)
{
	if (bFlag == _CYLINDER_UP)
	{
		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_0E_PROBEPIN_CYLINDER_UP, TRUE);
		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_0F_PROBEPIN_CYLINDER_DOWN, FALSE);
	}
	else if (bFlag == _CYLINDER_DOWN)
	{
		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_0E_PROBEPIN_CYLINDER_UP, FALSE);
		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_0F_PROBEPIN_CYLINDER_DOWN, TRUE);
	}
}


void CCommon::SetMainUiFlag_CurrentPos(int nPos)
{
	for (int i = 0; i < 3; i++)
	{
		if (nPos == i)
			m_bMainUiFlag_CurrentAction[i] = TRUE;
		else
			m_bMainUiFlag_CurrentAction[i] = FALSE;
	}
}
BOOL CCommon::GetMainUiFlag_CurrentPos(int nPos)
{
	return m_bMainUiFlag_CurrentAction[nPos];
}

void	CCommon::SetForcedStop_1Sec(BOOL bFlg)
{
	m_bForcedStop_Under_1Sec = bFlg;
}
BOOL	CCommon::GetForcedStop_1Sec()
{
	return m_bForcedStop_Under_1Sec;
}

void	CCommon::SetAccpet_ReciveBarcode(BOOL bFlag)
{
	m_bAccept_ReciveBarcode = bFlag;
}
BOOL	CCommon::GetAccpet_ReciveBarcode()
{
	return m_bAccept_ReciveBarcode;
}

void	CCommon::SetForced_SideBtn_StopOn(BOOL bFlag)
{
	m_bForced_SIdeBtn_Stop_On = bFlag;
}
BOOL	CCommon::GetFOrced_SideBtn_StopOn()
{
	return m_bForced_SIdeBtn_Stop_On;
}

void CCommon::SetClearCounts()
{
	COMMON->m_nTotalCnt = 0;
	COMMON->m_nOkCnt = 0;
	COMMON->m_nNGCnt = 0;
}

void CCommon::SetTotalCnt()
{
	COMMON->m_nTotalCnt++;
}

void CCommon::SetOKCnt()
{
	COMMON->m_nOkCnt++;
}

void CCommon::SetNGCnt()
{
	COMMON->m_nNGCnt++;
}

void CCommon::SetReplaceFlag(BOOL bState)
{
	COMMON->m_bReplaceFlag = bState;
}

int CCommon::GetTotalCnt()
{
	return m_nTotalCnt;
}

int CCommon::GetOKCnt()
{
	return m_nOkCnt;
}

int CCommon::GetNGCnt()
{
	return m_nNGCnt;
}

BOOL CCommon::GetReplaceFlag()
{
	return m_bReplaceFlag;
}

BOOL CCommon::GetMOAllInitNeedFlag()
{
	return m_bEMOAllInitNeedFlag;
}

void CCommon::SetMOAllInitNeedFlag(BOOL bState)
{
	m_bEMOAllInitNeedFlag = bState;
}

void CCommon::SetEMODisable(BOOL bState)
{
	COMMON->m_bEMODisableFlag = bState;
}

BOOL CCommon::GetEMODisable()
{
	return COMMON->m_bEMODisableFlag;
}
