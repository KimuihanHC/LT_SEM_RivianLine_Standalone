
#include "pch.h"
#include "Device.h"

#include "LuriMotion_OQA_SFR_Wide.h"
#include "LuriMotion_OQA_SFR_WideDlg.h"

static HANDLE g_hEvent_ProgramExit;

CDevice::CDevice()
{

}

CDevice::~CDevice()
{

}

bool CDevice::ConnectDevicez()
{
	MOTION_PARAM	stSpeed = *DATAMANAGER->GetCurrentMotionParam();

	TESTER_COMM stTester = *DATAMANAGER->GetCurrentTesterCommData();

	g_hEvent_ProgramExit = CreateEvent(NULL, TRUE, FALSE, NULL);

	// Ajin board init
#if(GET_INSPECTOR == SYS_FILM_PEEL_OFF)
	AJIN_MO->Init_Ajin();
#endif
	AJIN_IO->Start_AjinIO();

	DWORD BaudInitList_Motor[] = { 9600, 19200, 38400, 57600, 115200 };

	// Fastech EziMotion.
	bool bRet = false;
	CString sz;

	int nPort;
	int nBaudRate;
	int nDataBit;
	int nStopBit;
	int nParityBit;

	FAS_MO->Disconnect_EziServo();

	//20231123
	//bRet = FAS_MO->Connect_EziServo(stSpeed.nPort[0], BaudInitList_Motor[stSpeed.nBaudRate[0]]);
	bRet = FAS_MO->Connect_EziServo(stSpeed.nPort[0], BaudInitList_Motor[0]);
	if (bRet == false)
		WRITE_MAIN_LOG(_T("EziServo_Connect Fail"));
	else
	{
		FAS_MO->Servo_Enable_Init();
		for (int i = 0; i < MOTOR_MAX_COUNT; i++)
			FAS_MO->Servo_Enable(i);
	}

	// Tester
	m_UdpTester[0].Connect_Tester(stTester.dwTesterIP[0], stTester.nTesterPort[0], stTester.strTesterConnect[0], 1);
	COMMON->m_bConnectTesterFlag[0] = TRUE;

	// ODA Q
	nPort		= stTester.nPort[1];
	nBaudRate	= 9600;// stTester.nBaudRate[1];
	nDataBit	= 8;// stTester.nDataBit[1];
	nStopBit	= 0;// stTester.nStopBit[1];
	nParityBit	= 0;// stTester.nParityBit[1];

	ODA_Q->SetSemaphore(_T("POWER_ODA"));
	ODA_Q->SetExitEvent(g_hEvent_ProgramExit);

	sz.Format(_T("\\\\.\\COM%d"), nPort);

	ODA_Q->Connect(sz, nBaudRate, nParityBit, nStopBit, nDataBit);

	if (ODA_Q->IsConnected())
	{
		PowerOn();
		PowerOff();
	}

	nPort = stTester.nPort[0];
	nBaudRate = stTester.nBaudRate[0];
	nDataBit = stTester.nDataBit[0];
	nStopBit = stTester.nStopBit[0];
	nParityBit = stTester.nParityBit[0];

#if (GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL)
	// FN 300
// 	if (m_ctrlLightController_FN1500[0].Is_Connected() == FALSE)
// 	{
// 		m_ctrlLightController_FN1500[0].Create();
// 		m_ctrlLightController_FN1500[0].Invalidate(0);
// 	}
// 
// 	m_ctrlLightController_FN1500[0].ClosePort();
// 	if (m_ctrlLightController_FN1500[0].OpenPort(nPort, nBaudRate, WM_LIGHT_CONTROL_FN300_MSG, nParityBit, nDataBit, nStopBit, 0) == FALSE)
// 	{
// 		WRITE_MAIN_LOG(_T("FN300 CONNECT FAIL"));
// 	}
// 	else
// 		WRITE_MAIN_LOG(_T("FN300 CONNECT SUCCESS"));

	nBaudRate = 9600;// stTester.nBaudRate[1];
	nDataBit = 8;// stTester.nDataBit[1];
	nStopBit = 0;// stTester.nStopBit[1];
	nParityBit = 0;// stTester.nParityBit[1];  

	FN300->SetSemaphore(_T("LED_FN300"));
	FN300->SetExitEvent(g_hEvent_ProgramExit);

	sz.Format(_T("\\\\.\\COM%d"), nPort);

	FN300->Connect(sz, nBaudRate, nParityBit, nStopBit, nDataBit);

	CString szTemp;
	if (FN300->IsConnected())
	{
		szTemp.Format(_T("Light Control CONNECT Success : %s, %d, %d, %d, %d"), sz, nBaudRate, nParityBit, nStopBit, nDataBit);
		WRITE_MAIN_LOG(szTemp);
	}
	else
	{
		szTemp.Format(_T("Light Control CONNECT Fail : %s, %d, %d, %d, %d"), sz, nBaudRate, nParityBit, nStopBit, nDataBit);
		WRITE_MAIN_LOG(szTemp);
	}
	
#elif(GET_INSPECTOR == SYS_DISTORTION_30)
	if (m_ctrlLightController_Youth_DPS[0].Is_Connected() == FALSE)
	{
		m_ctrlLightController_Youth_DPS[0].Create(nPort);
		m_ctrlLightController_Youth_DPS[0].Invalidate(0); 
	}

	m_ctrlLightController_Youth_DPS[0].ClosePort();
	if (m_ctrlLightController_Youth_DPS[0].OpenPort(nPort, nBaudRate, WM_LIGHT_CONTROL_YOUTHTECH_MSG, nParityBit, nDataBit, nStopBit, 0) == FALSE)
	{
		WRITE_MAIN_LOG(_T("Youth DPS CONNECT FAIL"));
	}
	else
		WRITE_MAIN_LOG(_T("Youth DPS CONNECT SUCCESS"));

	LightOn();
#endif

	CLuriMotion_OQA_SFR_WideDlg * pDlg = (CLuriMotion_OQA_SFR_WideDlg*)AfxGetApp()->GetMainWnd();
	g_HandyBCR.SetOwnerHwnd(pDlg->m_hWnd);
	g_HandyBCR.SetAckMsgID(UM_RECV_BARCODE);
	g_HandyBCR.Disconnect();

	nPort		= stTester.nPort[2];
	nBaudRate	= 9600;// stTester.nBaudRate[2];
	nDataBit	= 8;// stTester.nDataBit[2];
	nStopBit	= 0;// stTester.nStopBit[2];
	nParityBit	= 0;// stTester.nParityBit[2];

	sz.Format(_T("\\\\.\\COM%d"), nPort);
	
	g_HandyBCR.Connect(sz, nBaudRate, nParityBit, nStopBit, nDataBit);

	//if (g_HandyBCR.IsConnected())
	//	AfxMessageBox(_T("ok"));

	// [21.1214.5]
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_09_TOWERLAMP_BEEP1, FALSE);
#if(GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL) || (GET_INSPECTOR == SYS_OQC_SFR_SINGLE_CL) || (GET_INSPECTOR == SYS_DISTORTION_30)
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_0B_SLEEP_MODE_ON, FALSE);
#elif(GET_INSPECTOR == SYS_FILM_PEEL_OFF)
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_11_SLEEP_MODE_ON, FALSE);
#endif
#if(GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL)
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_10_DOORLOCK_ONOFF_FRONT, TRUE);
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_11_DOORLOCK_ONOFF_LEFT1, TRUE);
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_12_DOORLOCK_ONOFF_LEFT2, TRUE);
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_13_DOORLOCK_ONOFF_RIGHT1, TRUE);
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_14_DOORLOCK_ONOFF_RIGHT2, TRUE);
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_15_DOORLOCK_ONOFF_BACK1, TRUE);
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_16_DOORLOCK_ONOFF_BACK2, TRUE);
#elif(GET_INSPECTOR == SYS_OQC_SFR_SINGLE_CL)
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_10_DOORLOCK_ONOFF_FRONT, TRUE);
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_11_DOORLOCK_ONOFF_LEFT1, TRUE);
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_12_DOORLOCK_ONOFF_RIGHT1, TRUE);
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_13_DOORLOCK_ONOFF_BACK1, TRUE);
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_14_DOORLOCK_ONOFF_BACK2, TRUE);
#elif(GET_INSPECTOR == SYS_DISTORTION_30)
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_10_DOORLOCK_ONOFF_FRONT, TRUE);
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_11_DOORLOCK_ONOFF_LEFT1, TRUE);
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_12_DOORLOCK_ONOFF_LEFT2, TRUE);
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_13_DOORLOCK_ONOFF_RIGHT1, TRUE);
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_14_DOORLOCK_ONOFF_RIGHT2, TRUE);
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_15_DOORLOCK_ONOFF_BACK1, TRUE);
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_16_DOORLOCK_ONOFF_BACK2, TRUE);
#elif(GET_INSPECTOR == SYS_FILM_PEEL_OFF)
	AJIN_IO->SendOutputBit(DIO_INPUT_X0_16_REAR_DOORLOCK1_OPENCLOSE, TRUE);
	AJIN_IO->SendOutputBit(DIO_INPUT_X0_17_REAR_DOORLOCK2_OPENCLOSE, TRUE);
	AJIN_IO->SendOutputBit(DIO_INPUT_X0_18_REAR_DOORLOCK3_OPENCLOSE, TRUE);
#endif

	SEQUENCE->Start_Inlk();
	SEQUENCE->Start_MonitoringIO();
	return true;
}

bool CDevice::IsConnected_HandyBCR()
{
	return g_HandyBCR.IsConnected();
}

bool CDevice::IsConnected_Power()
{
	return ODA_Q->IsConnected();
}

bool CDevice::GetMeasure_Volt(__out float& value)
{
	float f = 0.0f;
	ODA_Q->Send_Meas_Voltage(f);
	value = f;
	return true;
}

bool CDevice::GetMeasure_Current(__out float& value)
{
	float f = 0.0f;
	ODA_Q->Send_Meas_Current(f);
	value = f;
	return true;
}

bool CDevice::ReconnectDeviez()
{
	DWORD BaudInitList_Motor[] = { 9600, 19200, 38400, 57600, 115200 };

	MOTION_PARAM	stSpeed = *DATAMANAGER->GetCurrentMotionParam();
	TESTER_COMM stTester = *DATAMANAGER->GetCurrentTesterCommData();

	int nPort;
	int nBaudRate;
	int nDataBit;
	int nStopBit;
	int nParityBit;

	FAS_MO->Disconnect_EziServo();

	Sleep(500);
	if (FAS_MO->Connect_EziServo(stSpeed.nPort[0], BaudInitList_Motor[stSpeed.nBaudRate[0]]) == false)
		WRITE_MAIN_LOG(_T("EziServo_Connect Fail"));
	else
	{
		FAS_MO->Servo_Enable_Init();
		for (int i = 0; i < MOTOR_MAX_COUNT; i++)
			FAS_MO->Servo_Enable(i);
	}
		
	// Tester
	m_UdpTester[0].Connect_Tester(stTester.dwTesterIP[0], stTester.nTesterPort[0], stTester.strTesterConnect[0], 1);
	COMMON->m_bConnectTesterFlag[0] = TRUE;
	
	// ODA Q
	nPort = stTester.nPort[1];
	nBaudRate = stTester.nBaudRate[1];
	nDataBit = stTester.nDataBit[1];
	nStopBit = stTester.nStopBit[1];
	nParityBit = stTester.nParityBit[1];

	ODA_Q->Disconnect();
	CString sz;

	sz.Format(_T("\\\\.\\COM%d"), nPort);

	ODA_Q->Connect(sz, nBaudRate, nParityBit, nStopBit, nDataBit);
	PowerReset();
	
	/*m_ctrlLight_ODA_Q[0].ClosePort();
	if (m_ctrlLight_ODA_Q[0].OpenPort(nPort, nBaudRate, WM_LIGHT_CONTROL_Q_MSG, nParityBit, nDataBit, nStopBit, 0) == FALSE)
	{
		WRITE_MAIN_LOG(_T("ODA Q CONNECT FAIL"));
	}
	else
		WRITE_MAIN_LOG(_T("ODA Q CONNECT SUCCESS"));*/

	// Light Control
	nPort = stTester.nPort[0];
	nBaudRate = stTester.nBaudRate[0];
	nDataBit = stTester.nDataBit[0];
	nStopBit = stTester.nStopBit[0];
	nParityBit = stTester.nParityBit[0];
	
#if (GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL)
	FN300->SetSemaphore(_T("LED_FN300"));
	FN300->SetExitEvent(g_hEvent_ProgramExit);

	if (nPort < 10)
		sz.Format(_T(".\\COM%d"), nPort);
	else
		sz.Format(_T("\\\\.\\COM%d"), nPort);

	FN300->Connect(sz, nBaudRate, nParityBit, nStopBit, nDataBit);

	// FN 300
// 	if (m_ctrlLightController_FN1500[0].Is_Connected() == FALSE)
// 	{
// 		m_ctrlLightController_FN1500[0].Create();
// 		m_ctrlLightController_FN1500[0].Invalidate(0);
// 	}
// 
// 	m_ctrlLightController_FN1500[0].ClosePort();
// 	if (m_ctrlLightController_FN1500[0].OpenPort(nPort, nBaudRate, WM_LIGHT_CONTROL_FN300_MSG, nParityBit, nDataBit, nStopBit, 0) == FALSE)
// 	{
// 		WRITE_MAIN_LOG(_T("FN300 CONNECT FAIL"));
// 	}
// 	else
// 		WRITE_MAIN_LOG(_T("FN300 CONNECT SUCCESS"));
	

#elif(GET_INSPECTOR == SYS_DISTORTION_30)
	/*if (m_ctrlLightController_Youth_DPS[0].m_hWnd == NULL)
	{
		m_ctrlLightController_Youth_DPS[0].Create(nPort);
		m_ctrlLightController_Youth_DPS[0].Invalidate(0);
	}*/

	m_ctrlLightController_Youth_DPS[0].ClosePort();
	if (m_ctrlLightController_Youth_DPS[0].OpenPort(nPort, nBaudRate, WM_LIGHT_CONTROL_YOUTHTECH_MSG, nParityBit, nDataBit, nStopBit, 0) == FALSE)
	{
		WRITE_MAIN_LOG(_T("Youth DPS CONNECT FAIL"));
	}
	else
		WRITE_MAIN_LOG(_T("Youth DPS CONNECT SUCCESS"));

	LightOn();
#endif

	// handy bcr
	nPort = stTester.nPort[2];
	nBaudRate = stTester.nBaudRate[2];
	nDataBit = stTester.nDataBit[2];
	nStopBit = stTester.nStopBit[2];
	nParityBit = stTester.nParityBit[2];

	g_HandyBCR.Disconnect();
//	CString sz;

	//if (nPort < 10)
	//{
	//	sz.Format(_T("COM%d"), nPort);
	//}
	//else
	//{
		sz.Format(_T("\\\\.\\COM%d"), nPort);
	//}
	g_HandyBCR.Connect(sz, nBaudRate, nParityBit, nStopBit, nDataBit);

	return true;
}

bool CDevice::DisconnectDevicez()
{
	g_HandyBCR.Disconnect();
	return true;
}

bool CDevice::PowerReset()
{
	Sleep(500);
	ODA_Q->Send_Output(Switch_OFF);
	Sleep(500);
	ODA_Q->Send_Output(Switch_ON);
	Sleep(500);

	return true;
}

bool CDevice::PowerOn()
{
	TESTER_COMM stTester = *DATAMANAGER->GetCurrentTesterCommData();

	auto volt = stTester.fVolt;
	auto current = stTester.nCurrent;

	if (ODA_Q->IsConnected() == false)
		return false;

	auto EqpType = GET_INSPECTOR;

	switch (EqpType)
	{
	case SYS_DISTORTION_30:
	{
		float fread = 0.0f;

		ODA_Q->Send_Output(Switch_ON);
		COMMON->m_bMainUiFlag_StepView[7] = TRUE;
		Sleep(200);

		for (auto i = 0; i < 3; i++)
		{
			ODA_Q->Send_Channel(1);
			Sleep(200);
			ODA_Q->Send_Voltage(volt);
			Sleep(100);

			ODA_Q->Send_Meas_Voltage(fread);

			if (abs(fread - volt) < 0.2)
			{
				break;
			}
			else
			{
				CString sz;
				sz.Format(_T("Power On [2] Volt fail : %.2f"), fread);
				WRITE_SEQ_LOG(sz);
			}
		}

	//	ODA_Q->Send_Channel(1);
		Sleep(100);
//		ODA_Q->Send_Voltage(volt);
		COMMON->m_bMainUiFlag_StepView[4] = TRUE;
		Sleep(100);

		ODA_Q->Send_Current(current);
		COMMON->m_bMainUiFlag_StepView[5] = TRUE;
		Sleep(100);

		CString sz;
		sz.Format(_T("Power On Volt : %.2f, Current : %d"), volt, current);
		WRITE_SEQ_LOG(sz);
	}
		break;

	case SYS_OQC_SFR_MULTI_CL:
	case SYS_OQC_SFR_SINGLE_CL:
	{
		float fread = 0.0f;

		ODA_Q->Send_Output(Switch_ON);
		COMMON->m_bMainUiFlag_StepView[7] = TRUE;
		Sleep(300);

		for (auto i = 0; i < 3; i++)
		{
			ODA_Q->Send_Channel(2);
			Sleep(200);
			ODA_Q->Send_Voltage(volt);
			Sleep(100);

			ODA_Q->Send_Meas_Voltage(fread);

			if (abs(fread - volt) < 0.2)
			{
				break;
			}
			else
			{
				CString sz;
				sz.Format(_T("Power On [2] Volt fail : %.2f"), fread);
				WRITE_SEQ_LOG(sz);
			}
		}

		Sleep(100);

		ODA_Q->Send_Current(current);
		Sleep(300);

		fread = 0.0;

		for (auto i = 0; i < 3; i++)
		{
			ODA_Q->Send_Channel(1);
			Sleep(200);
			ODA_Q->Send_Voltage(volt);
			Sleep(100);

			ODA_Q->Send_Meas_Voltage(fread);

			if (abs(fread - volt) < 0.2)
			{
				break;
			}
			else
			{
				CString sz;
				sz.Format(_T("Power On [1] Volt fail : %.2f"), fread);
				WRITE_SEQ_LOG(sz);
			}
		}

//		ODA_Q->Send_Channel(1);
//		Sleep(300);
//		ODA_Q->Send_Voltage(volt);
		COMMON->m_bMainUiFlag_StepView[4] = TRUE;
//		Sleep(200);

		ODA_Q->Send_Current(current);
		COMMON->m_bMainUiFlag_StepView[5] = TRUE;
		Sleep(200);

		CString sz;
		sz.Format(_T("Power On Volt : %.2f, Current : %d"), volt, current);
		WRITE_SEQ_LOG(sz);
	}
		break;
	}

	return true;
}

bool CDevice::PowerOff()
{
	if (ODA_Q->IsConnected() == false)
		return false;

	auto EqpType = GET_INSPECTOR;

	switch (EqpType)
	{
	case SYS_DISTORTION_30:
	{
		ODA_Q->Send_Channel(1);
		Sleep(100);
		ODA_Q->Send_Voltage(0);
	}
	break;

	case SYS_OQC_SFR_MULTI_CL:
	case SYS_OQC_SFR_SINGLE_CL:
	{
		ODA_Q->Send_Channel(2);
		Sleep(100);
		ODA_Q->Send_Voltage(0);
		Sleep(100);

		ODA_Q->Send_Channel(1);
		Sleep(100);
		ODA_Q->Send_Voltage(0);
		COMMON->m_bMainUiFlag_StepView[4] = TRUE;
		Sleep(100);
	}
	break;
	}

	CString sz;
	sz = _T("Power Off Volt : 0V");
	WRITE_SEQ_LOG(sz);
	return true;
}

BOOL CDevice::POWERSUPPLY_MeasureVolt(__out float & value)
{
	int powerSupplyIndex = 0;
	int channelIndex = 0;

	auto EqpType = GET_INSPECTOR;

	switch (EqpType)
	{
	case SYS_DISTORTION_30:
		channelIndex = 1;
		break;
	case SYS_OQC_SFR_MULTI_CL:
	case SYS_OQC_SFR_SINGLE_CL:
		channelIndex = 2;
		break;
	default:
		channelIndex = 1;
		break;
	}

	ODA_Q->Send_Channel(channelIndex);
	COMMON->DoEvent(100);

	ODA_Q->Send_Meas_Voltage(value);
	return TRUE;
}

bool CDevice::LightOn()
{
	int nCate = 0;
	int nValue = 0;
	int nRead = 0;
	
	TESTER_COMM		stTester = *DATAMANAGER->GetCurrentTesterCommData();
	
	auto EqpType = GET_INSPECTOR;

#if(GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL)
	CString sz;
	bool bCheck = false;

	for (int nChan = 0; nChan < 5; nChan++)
	{
		if (FN300->IsConnected())
		{
			for (auto retry = 0; retry < 3; retry++)
			{
				nValue = stTester.stLightData[0].nVoltage[nCate][nChan];
				FN300->Send_SetChannelData(nChan + 1, nValue);
				Sleep(300);
				FN300->Send_SetChannelOnOff(nChan + 1, TRUE);
				Sleep(300);
				FN300->Send_GetChannelData(nChan + 1, nRead);

				sz.Format(_T("Light Value Ch%d : %d, Read : %d"), nChan + 1, nValue, nRead);
				WRITE_SEQ_LOG(sz);

				if (nRead == nValue)
				{
					bCheck = true;
					break;
				}

				bCheck = false;

				sz.Format(_T("Light On Retry : %d"), retry + 1);
				WRITE_SEQ_LOG(sz);
			}	

			if (bCheck == false)
			{
				sz.Format(_T("Light On Error"));
				WRITE_SEQ_LOG(sz);
				break;
			}
		}
	}
#elif(GET_INSPECTOR == SYS_DISTORTION_30)
	CString sz;
	nValue = stTester.stLightData[0].nVoltage[0][0];
	m_ctrlLightController_Youth_DPS[0].LightOn(nValue);

	sz.Format(_T("Light Value Ch1 : %d"), nValue);
	WRITE_SEQ_LOG(sz);
#endif
	return true;
}

bool CDevice::LightOff()
{
	auto EqpType = GET_INSPECTOR;

	switch (EqpType)
	{
	case SYS_OQC_SFR_MULTI_CL:
	{
	}
	break;

	case SYS_DISTORTION_30:
		m_ctrlLightController_Youth_DPS[0].LightOff();
		break;

	default:
		break;
	}
	return true;
}

bool CDevice::SEMCO_SendReadyStart()
{
	bool bRet = true;

	if (m_UdpTester[0].m_bConnected == false)
		return false;

	bRet = m_UdpTester[0].Send_Tester_Communication(eComMode_Ready);

	return bRet;
}

bool CDevice::SEMCO_WaitReadyStartAck(__in UINT nWaitTime, __out UINT& nRetCode)
{
	bool bRet = false;

	if (m_UdpTester[0].m_bConnected == false)
		return false;

	clock_t startm = clock();

	do
	{
		if (m_UdpTester[0].m_bTesterFlag_Ready)
		{
			nRetCode = m_UdpTester[0].m_nTesterFlag_ReadyErrorCode;
			bRet = true;
			break;
		}

		COMMON->DoEvent(10);
	} while ((clock() - startm) < nWaitTime);

	return bRet;
}

bool CDevice::SEMCO_SendBcr(__in CString szBCR)
{	
	return m_UdpTester[0].Send_Barcode(szBCR);;
}

bool CDevice::SEMCO_WaitBarcodeAck(__in UINT nWaitTime, __out CString& szBCR)
{
	bool bRet = false;

	if (m_UdpTester[0].m_bConnected == false)
		return false;

	bool bAck = false;
	int nAckErrorCode = 0;

	clock_t startm = clock();

	do
	{
		bAck = m_UdpTester[0].m_bAckFlag_Barcode;

		if (bAck)
		{
			szBCR = m_UdpTester[0].m_strTesterQuery_Barcode;
			bRet = true;
			break;
		}

		COMMON->DoEvent(10);
	} while ((clock() - startm) < nWaitTime);
	return bRet;
}

bool CDevice::SEMCO_PrewviewStart()
{
	m_UdpTester[0].Reset_TesterFlag();
	m_UdpTester[0].Send_Tester_Communication(eComMode_PreviewStart);//START PREVIEW 1		
	return true;
}

bool CDevice::SEMCO_PrewviewStop()
{
	m_UdpTester[0].Send_Tester_Communication(eComMode_Insp_PreviewStop);
	return true;
}

bool CDevice::SEMCO_StartInspection()
{
	return true;
}

bool CDevice::SEMCO_SendCurrentVoltage()
{
	bool bRet = true;

	float volt, current;

	ODA_Q->Send_Channel(1);
	COMMON->DoEvent(50);

	for (auto retry = 0; retry < 10; retry++)
	{
		ODA_Q->Send_Meas_Voltage(volt);
		COMMON->DoEvent(50);

		if (volt > 0)
			break;
	}

	ODA_Q->Send_Meas_Current(current);
	for (auto retry = 0; retry < 10; retry++)
	{
		ODA_Q->Send_Meas_Current(current);

		if (current >= 0.07 && current <= 0.09)
			break;

		Sleep(200);
	}	

	m_UdpTester[0].Send_CurrentVoltage(current, volt);

	return bRet;
}

bool CDevice::SEMCO_WaitCurrentVoltageAck(__in UINT nWaitTime, __out CString& szCurrentVoltage)
{
	bool bRet = false;

	bool bAck = false;
	int nAckErrorCode = 0;

	clock_t startm = clock();

	do
	{
		bool bRet = false;
		bAck = m_UdpTester[0].m_bAckFlag_CurrentVoltage;

		if (bAck)
		{
			szCurrentVoltage = m_UdpTester[0].m_strTesterQuery_CurrentVoltage;
			bRet = true;
			break;
		}

		COMMON->DoEvent(10);
	}while ((clock() - startm) < nWaitTime);

	return true;
}