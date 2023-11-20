           // HeaderBoard.cpp : 구현 파일입니다.
//

//#include "stdafx.h"
#include "pch.h"
#include "LightController_Luri.h"
#include "PCOMM/PCOMM.H"
#include "SerialParam.h"

#include "Common.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CLightController_Luri
CLightController_Luri m_ctrlLightController[LIGHT_CONTROLLER_LURITECH_BOARD_COUNT];

IMPLEMENT_DYNAMIC(CLightController_Luri, CWnd)

CLightController_Luri::CLightController_Luri()
{	
	//InitVariables();
}

CLightController_Luri::~CLightController_Luri()
{
	ClosePort();
}


BEGIN_MESSAGE_MAP(CLightController_Luri, CWnd)
	ON_MESSAGE(WM_LIGHT_CONTROL_MSG, OnLightControlReceive)
END_MESSAGE_MAP()



// CLightController_Luri 메시지 처리기입니다.

BOOL CLightController_Luri::Create(CWnd* pParentWnd) 
{
	CRect rect = CRect(0, 0, 0, 0); // 화면 영역 설정
	return CWnd::Create(NULL, NULL, WS_CHILD, rect, GetDesktopWindow(), 0);
}

void CLightController_Luri::InitVariables(int nID)
{
	m_nId = nID;

	m_bConnected = FALSE;
	m_bHW = FALSE;
	m_bSW = FALSE;

	m_npos = 0;
	m_nMode = LIGHT_CONTROL_READY;

	m_strRevData = _T("");

	memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_BUFFER_MAX);
	memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_BUFFER_MAX);

	m_hEvent_HeaderBoard = NULL;
	m_hEvent_HeaderBoard = ::CreateEvent(NULL, TRUE,FALSE, NULL);
	
}

void CALLBACK CLightController_Luri::Control_LIGHT_CONTROL_IRQ0(int port)
{	
	if(m_ctrlLightController[0].m_bConnected == TRUE)
	{
		if(m_ctrlLightController[0].m_hWnd != NULL)
		{
			::SendMessage(m_ctrlLightController[0].m_hWnd, m_ctrlLightController[0].m_MSG, m_ctrlLightController[0].m_nPort, 0);
		}
	}	
}

void CALLBACK CLightController_Luri::Control_LIGHT_CONTROL_IRQ1(int port)
{	
	if(m_ctrlLightController[1].m_bConnected == TRUE)
	{
		if(m_ctrlLightController[1].m_hWnd != NULL)
		{
			::SendMessage(m_ctrlLightController[1].m_hWnd, m_ctrlLightController[1].m_MSG, m_ctrlLightController[1].m_nPort, 0);
		}
	}	
}

BOOL CLightController_Luri::OpenPort(int Port,int BaudRate, UINT Msg, int Parity, int DataBit, int StopBit, int nIdx)
{
	if(m_bConnected == FALSE)
	{
#ifdef LIGHT_CONTROLLER_LURITECH_BOARD_USE

		if(IsWindow(this->m_hWnd) == TRUE)
		{
			int ret = 0, mode = 0;		

			ret = sio_open(Port);
			if(ret != SIO_OK)
			{
				return FALSE;
			}


			mode = ParityTable[Parity] | DataBitTable[DataBit] | StopBitTable[StopBit];
			ret = sio_ioctl(Port, BaudTable[BaudRate], mode);
			if(ret != SIO_OK)
			{
				return FALSE;
			}


			int	hw = m_bHW ? 3 : 0;	
			int	sw = m_bSW ? 12 : 0;   
			//if((ret = sio_flowctrl(Port, hw | sw)) != SIO_OK)
			if(ret = sio_flowctrl(Port, 0) != SIO_OK)				//H/W 사양에 맞게 Flow Control 제어 방식 변경 필요
			{
				return FALSE;
			}


			BOOL Dtr = TRUE , RTS = TRUE;						 
			if((ret = sio_DTR(Port, (Dtr ? 1 : 0))) != SIO_OK)
			{
				return FALSE;
			}


			if(!m_bHW)
			{
				if((ret = sio_RTS(Port, (RTS ? 1 : 0))) != SIO_OK)
				{
					return FALSE;
				}
			}
		

			ret = sio_flush(Port, 2);
			if(ret != SIO_OK)
			{
				return FALSE;
			}
		
			if(nIdx == 0)
			{
				ret = sio_cnt_irq(Port, Control_LIGHT_CONTROL_IRQ0, 1);
			}
			else if(nIdx == 1)
			{
				ret = sio_cnt_irq(Port, Control_LIGHT_CONTROL_IRQ1, 1);
			}
			if(ret != SIO_OK)
			{
				return false;
			}
			
			m_nPort = Port;
			m_MSG = Msg;
			m_bConnected = TRUE;
		}
#else
		m_nPort = Port;
		m_bConnected = TRUE;
#endif
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}

void CLightController_Luri::ClosePort()
{
	if(m_bConnected == TRUE)
	{
#ifdef SCARA_CONTROL_USE
		sio_close(m_nPort);
#endif
		m_bConnected = FALSE;
		m_nMode = LIGHT_CONTROL_READY;
	}	
}

BOOL CLightController_Luri::WritePort(int Port, char *szBuffer)
{	
	CString strLog = _T("");
	CString strTemp = _T("");

	if(m_bConnected == TRUE)
	{
		int ret = 0, len = 0;
		CString str = _T("");		
		
		len = (int)strlen(szBuffer) ;

		// Flush any data within input and/or output buffer
		// 0 : Input Buffer , 1: Output Buffer , 2 : Input / Output Buffer;

		ret = sio_flush(Port, 2);
		if(ret != SIO_OK)
		{
			return FALSE;
		}

		ret = sio_write(Port, szBuffer, len);
		if(ret < 0)
		{
			return FALSE;
		}

		
		//Log
		for(int j = 0; j < len - 1; j++)		//CR 제외
		{
			strTemp += szBuffer[j];
		}
		strLog.Format(_T("[LIGHT_CONTROL] TX : %s [ Scara : %d ]\n"), strTemp, m_nId + 1);
	}
	else
	{
		if(m_bConnected == FALSE) 
		{
		}	
		return FALSE;
	}

	return TRUE;
}

BOOL CLightController_Luri::ReadPort(int Port, char *szBuffer)
{
	char buf[LIGHT_CONTROL_BUFFER_MAX_RCV];
		
	if(m_bConnected == TRUE)
	{
		int ret;
		ret = sio_read(Port, buf, LIGHT_CONTROL_BUFFER_MAX_RCV);
		
		if(ret > 0)
		{
			if(ret > LIGHT_CONTROL_BUFFER_MAX_RCV)
			{
				ret = LIGHT_CONTROL_BUFFER_MAX_RCV;
			}

			memcpy(szBuffer, buf, sizeof(char) * ret);
		}
		else
		{
			return FALSE;
		}		
	}
	else
	{
		if(m_bConnected == FALSE) 
		{
		}
		return FALSE;
	}
    return TRUE;
}

LRESULT CLightController_Luri::OnLightControlReceive(WPARAM nPort, LPARAM nLen)
{
	int len = 0;
	int nPos = 0;
	int bufferlen = 0;
	
	bool re = true;

	char buffer[LIGHT_CONTROL_BUFFER_MAX_RCV];
	CString str = _T("");
	CString strLog = _T("");
	CString strTemp = _T("");
	CString strTemp_Command = _T("");

	////////////////////////////////////////////////////////////
	
	memset(buffer, 0x00, sizeof(char) * LIGHT_CONTROL_BUFFER_MAX_RCV);

	COMMON->uSleep_NoReturn(20);  

	if(ReadPort((int)nPort, buffer) == TRUE)
	{
		len = (int)strlen(buffer);
		
		if(len > 0)
		{
			for(int i = 0; i < len; i++)
			{
				recv[m_npos + i] = buffer[i];
			}
			m_npos += len;

			for(int j = 0; j < m_npos; j++)
			{
				str += recv[j];
			}
			

			strTemp_Command = str.GetAt(1);
			
			if(strTemp_Command == _T("U"))		// EtcSetVolt
			{
				// 탐과 동시에 정상처리.
				m_bRecv_EtcSetVolt = TRUE;

				m_strRevData.Format(strTemp_Command);
				str = _T("");
			}
			else if (strTemp_Command == _T("0"))	// BoardCheck
			{
				// 진입 후에 2번째 자리의 문자열을 봐야 한다.
				m_bRecv_PortCheck = FALSE;
				 
				strTemp = str.GetAt(2);
				
				if (strTemp == _T("1"))
					m_bRecv_PortCheck = TRUE;
			}
			else if (strTemp_Command == _T("W"))	// DetectVolt
			{
				strTemp = str.Mid(2, 4);
				m_stRecv.fVolt[0] = _tstof(strTemp);

				strTemp = str.Mid(6, 4);
				m_stRecv.fVolt[1] = _tstof(strTemp);

				strTemp = str.Mid(10, 4);
				m_stRecv.fVolt[2] = _tstof(strTemp);

				strTemp = str.Mid(14, 4);
				m_stRecv.fVolt[3] = _tstof(strTemp);

				strTemp = str.Mid(18, 4);
				m_stRecv.fVolt[4] = _tstof(strTemp);


				m_bRecv_DetectVolt = TRUE;

			}
			else if (strTemp_Command == _T("5"))	// EtcSEtCurrent
			{
				// 진입 후에 2번째 자리의 문자열을 봐야 한다.
				m_bRecv_PortCheck = FALSE;

				strTemp = str.GetAt(2);

				if (strTemp == _T("1"))
					m_bRecv_PortCheck = TRUE;
			}
			else if (strTemp_Command == _T("4"))	// DetectCurrent
			{
				strTemp = str.Mid(2, 4);
				m_stRecv.fCurrent[0] = _tstof(strTemp);

				strTemp = str.Mid(6, 4);
				m_stRecv.fCurrent[1] = _tstof(strTemp);

				strTemp = str.Mid(10, 4);
				m_stRecv.fCurrent[2] = _tstof(strTemp);

				strTemp = str.Mid(14, 4);
				m_stRecv.fCurrent[3] = _tstof(strTemp);

				strTemp = str.Mid(18, 4);
				m_stRecv.fCurrent[4] = _tstof(strTemp);


				m_bRecv_DetectCurrent = TRUE;
			}
			else if (strTemp_Command == _T("4"))	// ControlCheck
			{
				// Uart
				strTemp = str.GetAt(2);

				m_bRecv_ControlCheck_Auto = FALSE;

				if (strTemp == _T("1"))
					m_bRecv_ControlCheck_Auto = TRUE;


				// Dial
				strTemp = str.GetAt(3);

				m_bRecv_ControlCheck_Dial = FALSE;

				if (strTemp == _T("1"))
					m_bRecv_ControlCheck_Dial = TRUE;
			}


		}
		//if(len > 0)

	}
	//if(ReadPort((int)wParam, buffer) == TRUE)

	return 0;
}

BOOL CLightController_Luri::WaitOK(int timeout, BOOL& kill)
{
	DWORD time = GetTickCount();

	while(1)
	{
		if(kill == FALSE)
		{
			return FALSE;
		}
		if(WaitForSingleObject(m_hEvent_HeaderBoard, 5) == WAIT_OBJECT_0)
		{
			return TRUE;
		}
		if(GetTickCount() - time > (DWORD)timeout)
		{
			return FALSE;
		}
		Sleep(1);
		COMMON->DoEvents();
	}
	return FALSE;
}


void CLightController_Luri::SetMinMaxVoltage(float fMin, float fMax)
{
	m_fMinCurrent = fMin;
	m_fMaxCurrent = fMax;

	m_fUnitCurrent = (m_fMaxCurrent - m_fMinCurrent) / 1024.0f;
}
void CLightController_Luri::SetMinMaxCurrent(float fMin, float fMax)
{

}

BOOL CLightController_Luri::Send_PortCheck(BYTE byPortNo)
{
	ResetEvent(m_hEvent_HeaderBoard);

	char strValue[LIGHT_CONTROL_BUFFER_MAX];
	CString strSendMsg = _T("");

	BOOL bKill = TRUE;

	/////////////////////////////////////////////////////////////////////////////////////

	memset(strValue, 0x00, sizeof(char) * LIGHT_CONTROL_BUFFER_MAX);

	if (m_bConnected == TRUE)
	{
#ifdef LIGHT_CONTROLLER_LURITECH_BOARD_USE
		if (m_nMode != LIGHT_CONTROL_READY)
		{
			goto ABNORMAL_EXIT;
		}

		m_strRecvData = _T("");
		m_npos = 0;
		memset(recv, 0x00, sizeof(char)  * LIGHT_CONTROL_BUFFER_MAX);
		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_BUFFER_MAX);

		m_nMode = LIGHT_CONTROL_DEFAULT_MODE;

		//Command		
		for (int i = 0; i < 23; i++)
		{
			if( i == 0 )
				strValue[i] = '!';		// stx
			else if ( i == 22 )
				strValue[i] = '@';		// etx
			else
				strValue[i] = '0';		// etx

		}
		
		for (int i = 0; i < (int)LIGHT_CONTROL_BUFFER_MAX; i++)
		{
			strSendMsg += strValue[i];
		}

		if (WritePort(m_nPort, strValue) == FALSE)
		{
			goto ABNORMAL_EXIT;
		}

		
		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_BUFFER_MAX);

		return TRUE;

	ABNORMAL_EXIT:
		
		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_BUFFER_MAX);
		return FALSE;

#else	
		return TRUE;
#endif
	}
	else
	{
	}
	return FALSE;
}
BOOL CLightController_Luri::Send_EtcSetVolt(UINT nSlotNo, float fVolt)
{
	ResetEvent(m_hEvent_HeaderBoard);

	char strValue[LIGHT_CONTROL_BUFFER_MAX];
	char strInputVolt[LIGHT_CONTROL_BUFFER_MAX];
	CString strSendMsg = _T("");

	BOOL bKill = TRUE;

	/////////////////////////////////////////////////////////////////////////////////////

	memset(strValue, 0x00, sizeof(char) * LIGHT_CONTROL_BUFFER_MAX);
	memset(strInputVolt, 0x00, sizeof(char) * LIGHT_CONTROL_BUFFER_MAX);

	if (m_bConnected == TRUE)
	{
#ifdef LIGHT_CONTROLLER_LURITECH_BOARD_USE
		if (m_nMode != LIGHT_CONTROL_READY)
		{
			goto ABNORMAL_EXIT;
		}

		m_strRecvData = _T("");
		m_npos = 0;
		memset(recv, 0x00, sizeof(char)  * LIGHT_CONTROL_BUFFER_MAX);
		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_BUFFER_MAX);

		m_nMode = LIGHT_CONTROL_DEFAULT_MODE;

		//Command		
		strValue[0] = '!';		// stx
		strValue[1] = 'U';		// Command
		strValue[2] = nSlotNo + 0x30;	// slot
		
		sprintf_s(strInputVolt, 5, "%04.0f", fVolt * 100.0f);	// value
		for (int j = 0; j < 4; j++)
		{
			strValue[3 + j] = strInputVolt[j];
		}

		for (int i = 7; i < LIGHT_CONTROL_BUFFER_MAX; i++)
		{
			strValue[i] = '0';		// Zero
		}
		strValue[LIGHT_CONTROL_BUFFER_MAX-1] = '@';		// etx
		

		for (int i = 0; i < (int)LIGHT_CONTROL_BUFFER_MAX; i++)
		{
			strSendMsg += strValue[i];
		}

		if (WritePort(m_nPort, strValue) == FALSE)
		{
			goto ABNORMAL_EXIT;
		}

		m_nMode = LIGHT_CONTROL_READY;

		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_BUFFER_MAX);

		return TRUE;

	ABNORMAL_EXIT:

		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_BUFFER_MAX);
		return FALSE;

#else	
		return TRUE;
#endif
	}
	else
	{
	}
	return FALSE;
}

BOOL CLightController_Luri::SetVoltage_OnOff(bool bState)
{
	ResetEvent(m_hEvent_HeaderBoard);

	char strValue[LIGHT_CONTROL_BUFFER_MAX];
	char strInputVolt[LIGHT_CONTROL_BUFFER_MAX];
	CString strSendMsg = _T("");

	BOOL bKill = TRUE;

	/////////////////////////////////////////////////////////////////////////////////////

	memset(strValue, 0x00, sizeof(char) * LIGHT_CONTROL_BUFFER_MAX);
	memset(strInputVolt, 0x00, sizeof(char) * LIGHT_CONTROL_BUFFER_MAX);

	if (m_bConnected == TRUE)
	{
#ifdef LIGHT_CONTROLLER_LURITECH_BOARD_USE
		if (m_nMode != LIGHT_CONTROL_READY)
		{
			goto ABNORMAL_EXIT;
		}

		m_strRecvData = _T("");
		m_npos = 0;
		memset(recv, 0x00, sizeof(char)  * LIGHT_CONTROL_BUFFER_MAX);
		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_BUFFER_MAX);

		m_nMode = LIGHT_CONTROL_DEFAULT_MODE;

		//Command		
		strValue[0] = '!';		// stx
		strValue[1] = 'V';		// Command

		if (bState == TRUE)
			strValue[2] = '1';
		else
			strValue[2] = '0';

		for (int i = 3; i < LIGHT_CONTROL_BUFFER_MAX; i++)
		{
			strValue[i] = '0';		// Zero
		}
		strValue[LIGHT_CONTROL_BUFFER_MAX - 1] = '@';		// etx


		for (int i = 0; i < (int)LIGHT_CONTROL_BUFFER_MAX; i++)
		{
			strSendMsg += strValue[i];
		}

		if (WritePort(m_nPort, strValue) == FALSE)
		{
			goto ABNORMAL_EXIT;
		}


		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_BUFFER_MAX);

		return TRUE;

	ABNORMAL_EXIT:

		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_BUFFER_MAX);
		return FALSE;

#else	
		return TRUE;
#endif
	}
	else
	{
	}
	return FALSE;
}

BOOL CLightController_Luri::Send_DetectVolt(LURI_LIGHT_RECV& stRecv)
{
	ResetEvent(m_hEvent_HeaderBoard);

	char strValue[LIGHT_CONTROL_BUFFER_MAX];
	CString strSendMsg = _T("");

	BOOL bKill = TRUE;

	/////////////////////////////////////////////////////////////////////////////////////

	memset(strValue, 0x00, sizeof(char) * LIGHT_CONTROL_BUFFER_MAX);

	if (m_bConnected == TRUE)
	{
#ifdef LIGHT_CONTROLLER_LURITECH_BOARD_USE
		if (m_nMode != LIGHT_CONTROL_READY)
		{
			goto ABNORMAL_EXIT;
		}

		m_strRecvData = _T("");
		m_npos = 0;
		memset(recv, 0x00, sizeof(char)  * LIGHT_CONTROL_BUFFER_MAX);
		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_BUFFER_MAX);

		m_nMode = LIGHT_CONTROL_DEFAULT_MODE;

		//Command		
		for (int i = 0; i < 23; i++)
		{

			switch (i)
			{
			case 0:
				strValue[i] = '!';		// stx
				break;
			case 1:
				strValue[i] = 'W';		// Command
				break;
			case 22:
				strValue[i] = '@';		// etx
				break;
			default:
				strValue[i] = '0';		// Zero
				break;
			}
		}

		for (int i = 0; i < (int)LIGHT_CONTROL_BUFFER_MAX; i++)
		{
			strSendMsg += strValue[i];
		}

		if (WritePort(m_nPort, strValue) == FALSE)
		{
			goto ABNORMAL_EXIT;
		}

		m_nMode = LIGHT_CONTROL_READY;

		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_BUFFER_MAX);

		return TRUE;

	ABNORMAL_EXIT:

		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_BUFFER_MAX);
		return FALSE;

#else	
		return TRUE;
#endif
	}
	else
	{
	}
	return FALSE;
}
BOOL CLightController_Luri::Send_EtcSetCurrent(UINT nSlotNo, float fCurrent)
{
	ResetEvent(m_hEvent_HeaderBoard);
	char strInputVolt[LIGHT_CONTROL_BUFFER_MAX];
	char strValue[LIGHT_CONTROL_BUFFER_MAX];
	CString strSendMsg = _T("");

	BOOL bKill = TRUE;

	/////////////////////////////////////////////////////////////////////////////////////

	memset(strValue, 0x00, sizeof(char) * LIGHT_CONTROL_BUFFER_MAX);
	memset(strInputVolt, 0x00, sizeof(char) * LIGHT_CONTROL_BUFFER_MAX);

	if (m_bConnected == TRUE)
	{
#ifdef LIGHT_CONTROLLER_LURITECH_BOARD_USE
		if (m_nMode != LIGHT_CONTROL_READY)
		{
			goto ABNORMAL_EXIT;
		}

		m_strRecvData = _T("");
		m_npos = 0;
		memset(recv, 0x00, sizeof(char)  * LIGHT_CONTROL_BUFFER_MAX);
		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_BUFFER_MAX);

		m_nMode = LIGHT_CONTROL_DEFAULT_MODE;

		//Command
		strValue[0] = '!';
		strValue[1] = '5';

		strValue[2] = nSlotNo + 0x30;

		//Input Voltage
		sprintf_s(strInputVolt, 5, "%04.0f", fCurrent * 1.0f);
		for (int i = 0; i < 4; i++)
		{
			strValue[3 + i] = strInputVolt[i];
		}

		for (int i = 7; i < LIGHT_CONTROL_BUFFER_MAX; i++)
		{
			strValue[i] = '0';
		}
		strValue[LIGHT_CONTROL_BUFFER_MAX - 1] = '@';

		for (int i = 0; i < (int)LIGHT_CONTROL_BUFFER_MAX; i++)
		{
			strSendMsg += strValue[i];
		}

		if (WritePort(m_nPort, strValue) == FALSE)
		{
			goto ABNORMAL_EXIT;
		}

		m_nMode = LIGHT_CONTROL_READY;
		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_BUFFER_MAX);

		return TRUE;

	ABNORMAL_EXIT:

		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_BUFFER_MAX);
		return FALSE;

#else	
		return TRUE;
#endif
	}
	else
	{
	}
	return FALSE;
}
BOOL CLightController_Luri::Send_DetectCurrent(LURI_LIGHT_RECV& stRecv)
{
	ResetEvent(m_hEvent_HeaderBoard);

	char strValue[LIGHT_CONTROL_BUFFER_MAX];
	CString strSendMsg = _T("");

	BOOL bKill = TRUE;

	/////////////////////////////////////////////////////////////////////////////////////

	memset(strValue, 0x00, sizeof(char) * LIGHT_CONTROL_BUFFER_MAX);

	if (m_bConnected == TRUE)
	{
#ifdef LIGHT_CONTROLLER_LURITECH_BOARD_USE
		if (m_nMode != LIGHT_CONTROL_READY)
		{
			goto ABNORMAL_EXIT;
		}

		m_strRecvData = _T("");
		m_npos = 0;
		memset(recv, 0x00, sizeof(char)  * LIGHT_CONTROL_BUFFER_MAX);
		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_BUFFER_MAX);

		m_nMode = LIGHT_CONTROL_DEFAULT_MODE;

		//Command		
		for (int i = 0; i < 23; i++)
		{

			switch (i)
			{
			case 0:
				strValue[i] = '!';		// stx
				break;
			case 1:
				strValue[i] = '4';		// Command
				break;
			case 22:
				strValue[i] = '@';		// etx
				break;
			default:
				strValue[i] = '0';		// Zero
				break;
			}
		}

		for (int i = 0; i < (int)LIGHT_CONTROL_BUFFER_MAX; i++)
		{
			strSendMsg += strValue[i];
		}

		if (WritePort(m_nPort, strValue) == FALSE)
		{
			goto ABNORMAL_EXIT;
		}


		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_BUFFER_MAX);

		return TRUE;

	ABNORMAL_EXIT:

		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_BUFFER_MAX);
		return FALSE;

#else	
		return TRUE;
#endif
	}
	else
	{
	}
	return FALSE;
}
BOOL CLightController_Luri::Send_ControlCheck(BOOL& bAuto, BOOL& bManual)
{
	ResetEvent(m_hEvent_HeaderBoard);

	char strValue[LIGHT_CONTROL_BUFFER_MAX];
	CString strSendMsg = _T("");

	BOOL bKill = TRUE;

	/////////////////////////////////////////////////////////////////////////////////////

	memset(strValue, 0x00, sizeof(char) * LIGHT_CONTROL_BUFFER_MAX);

	if (m_bConnected == TRUE)
	{
#ifdef LIGHT_CONTROLLER_LURITECH_BOARD_USE
		if (m_nMode != LIGHT_CONTROL_READY)
		{
			goto ABNORMAL_EXIT;
		}

		m_strRecvData = _T("");
		m_npos = 0;
		memset(recv, 0x00, sizeof(char)  * LIGHT_CONTROL_BUFFER_MAX);
		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_BUFFER_MAX);

		m_nMode = LIGHT_CONTROL_DEFAULT_MODE;

		//Command		
		for (int i = 0; i < 23; i++)
		{

			switch (i)
			{
			case 0:
				strValue[i] = '!';		// stx
				break;
			case 1:
				strValue[i] = 'C';		// Command
				break;
			case 22:
				strValue[i] = '@';		// etx
				break;
			default:
				strValue[i] = '0';		// Zero
				break;
			}
		}

		for (int i = 0; i < (int)LIGHT_CONTROL_BUFFER_MAX; i++)
		{
			strSendMsg += strValue[i];
		}

		if (WritePort(m_nPort, strValue) == FALSE)
		{
			goto ABNORMAL_EXIT;
		}


		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_BUFFER_MAX);

		return TRUE;

	ABNORMAL_EXIT:

		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_BUFFER_MAX);
		return FALSE;

#else	
		return TRUE;
#endif
	}
	else
	{
	}
	return FALSE;
}
BOOL CLightController_Luri::Send_AmbientVoltOn_All(__in float fVolt)
{
	return Send_EtcSetVolt(Slot_All, fVolt);
}
BOOL CLightController_Luri::Send_AmbientVoltOff_All()
{
	return Send_EtcSetVolt(Slot_All, 0.0f);
}
BOOL CLightController_Luri::Send_AmbientCurrentOn_All(__in WORD wValue)
{
	return Send_EtcSetCurrent(Slot_All, wValue);
}
BOOL CLightController_Luri::Send_AmbientCurrentOff_All()
{
	return Send_EtcSetCurrent(Slot_All, 0.0f);
}