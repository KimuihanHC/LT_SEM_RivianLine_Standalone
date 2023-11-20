           // HeaderBoard.cpp : 구현 파일입니다.
//

#include "pch.h"
#include "LightController_ODA_Q.h"
#include "PCOMM/PCOMM.H"
#include "SerialParam.h"

#include "Common.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CLightController_ODA_PT
CLightController_ODA_Q m_ctrlLight_ODA_Q[LIGHT_CONTROLLER_PT_BOARD_COUNT];

IMPLEMENT_DYNAMIC(CLightController_ODA_Q, CWnd)

CLightController_ODA_Q::CLightController_ODA_Q()
{	
	m_nRecvType = -1;
}

CLightController_ODA_Q::~CLightController_ODA_Q()
{
	ClosePort();
}


BEGIN_MESSAGE_MAP(CLightController_ODA_Q, CWnd)
	ON_MESSAGE(WM_LIGHT_CONTROL_Q_MSG, OnLightControlReceive)
END_MESSAGE_MAP()



// CLightController_ODA_PT 메시지 처리기입니다.

BOOL CLightController_ODA_Q::Create(CWnd* pParentWnd)
{
	CRect rect = CRect(0, 0, 0, 0); // 화면 영역 설정
	return CWnd::Create(NULL, NULL, WS_CHILD, rect, GetDesktopWindow(), 0);
}

void CLightController_ODA_Q::InitVariables(int nID)
{
	m_nId = nID;

	m_bConnected = FALSE;
	m_bHW = FALSE;
	m_bSW = FALSE;

	m_npos = 0;
	m_nMode = LIGHT_CONTROL_Q_READY;

	m_strRevData = _T("");

	memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);
	memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);

	m_hEvent_HeaderBoard = NULL;
	m_hEvent_HeaderBoard = ::CreateEvent(NULL, TRUE,FALSE, NULL);

	m_nReadMode = 0;
	
}

void CALLBACK CLightController_ODA_Q::Control_LIGHT_CONTROL_Q_IRQ0(int port)
{	
	if(m_ctrlLight_ODA_Q[0].m_bConnected == TRUE)
	{
		if(m_ctrlLight_ODA_Q[0].m_hWnd != NULL)
		{
			::SendMessage(m_ctrlLight_ODA_Q[0].m_hWnd, m_ctrlLight_ODA_Q[0].m_MSG, m_ctrlLight_ODA_Q[0].m_nPort, 0);
		}
	}	
}

void CALLBACK CLightController_ODA_Q::Control_LIGHT_CONTROL_Q_IRQ1(int port)
{	
	if(m_ctrlLight_ODA_Q[1].m_bConnected == TRUE)
	{
		if(m_ctrlLight_ODA_Q[1].m_hWnd != NULL)
		{
			::SendMessage(m_ctrlLight_ODA_Q[1].m_hWnd, m_ctrlLight_ODA_Q[1].m_MSG, m_ctrlLight_ODA_Q[1].m_nPort, 0);
		}
	}	
}

BOOL CLightController_ODA_Q::OpenPort(int Port,int BaudRate, UINT Msg, int Parity, int DataBit, int StopBit, int nIdx)
{
	if(m_bConnected == FALSE)
	{


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
				ret = sio_cnt_irq(Port, Control_LIGHT_CONTROL_Q_IRQ0, 1);
			}
			else if(nIdx == 1)
			{
				ret = sio_cnt_irq(Port, Control_LIGHT_CONTROL_Q_IRQ1, 1);
			}
			if(ret != SIO_OK)
			{
				return false;
			}
			
			m_nPort = Port;
			m_MSG = Msg;
			m_bConnected = TRUE;
			m_nRecvType = 0;
		}
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}

void CLightController_ODA_Q::ClosePort()
{
	if(m_bConnected == TRUE)
	{
//#ifdef SCARA_CONTROL_USE
		sio_close(m_nPort);
//#endif
		m_bConnected = FALSE;
		m_nMode = LIGHT_CONTROL_Q_READY;
	}	
}

BOOL CLightController_ODA_Q::WritePort(int Port, char *szBuffer)
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

BOOL CLightController_ODA_Q::ReadPort(int Port, char *szBuffer)
{
	char buf[LIGHT_CONTROL_Q_BUFFER_MAX_RCV];
		
	if(m_bConnected == TRUE)
	{
		int ret;
		ret = sio_read(Port, buf, LIGHT_CONTROL_Q_BUFFER_MAX_RCV);
		
		if(ret > 0)
		{
			if(ret > LIGHT_CONTROL_Q_BUFFER_MAX_RCV)
			{
				ret = LIGHT_CONTROL_Q_BUFFER_MAX_RCV;
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

LRESULT CLightController_ODA_Q::OnLightControlReceive(WPARAM nPort, LPARAM nLen)
{
	int len = 0;
	int nPos = 0;
	int bufferlen = 0;
	
	bool re = true;

	char buffer[LIGHT_CONTROL_Q_BUFFER_MAX_RCV];
	CString str = _T("");
	CString strLog = _T("");
	CString strTemp = _T("");
	CString strTemp_Command = _T("");

	////////////////////////////////////////////////////////////
	
	memset(buffer, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX_RCV);

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

			for(int j = 0; j < m_npos-1; j++)
			{
				str += recv[j];
			}
			
			if (m_nReadMode == 0)
			{
				SetVoltValue(_ttof(str));
			}
			else if (m_nReadMode == 1)
			{
				SetCurrentValue(_ttof(str));
			}

		}
		//if(len > 0)

	}
	//if(ReadPort((int)wParam, buffer) == TRUE)

	return 0;
}

BOOL CLightController_ODA_Q::WaitOK(int timeout, BOOL& kill)
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

BOOL CLightController_ODA_Q::Write_OFF()
{
	ResetEvent(m_hEvent_HeaderBoard);

	char strValue[LIGHT_CONTROL_Q_BUFFER_MAX];
	char strInputVolt[LIGHT_CONTROL_Q_BUFFER_MAX];
	CString strSendMsg = _T("");

	BOOL bKill = TRUE;

	/////////////////////////////////////////////////////////////////////////////////////

	memset(strValue, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);
	memset(strInputVolt, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);

	if (m_bConnected == TRUE)
	{
		CString strTemp = _T("");
		if (m_nMode != LIGHT_CONTROL_Q_READY)
		{
			goto ABNORMAL_EXIT;
		}

		m_strRecvData = _T("");
		m_npos = 0;
		memset(recv, 0x00, sizeof(char)  * LIGHT_CONTROL_BUFFER_MAX);
		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_BUFFER_MAX);

		m_nMode = LIGHT_CONTROL_Q_DEFAULT_MODE;

		//Command		
		strValue[0] = 'O';		// stx
		strValue[1] = 'u';		// Command
		strValue[2] = 't';
		strValue[3] = 'p';
		strValue[4] = 0x20;
		strValue[5] = 'O';
		strValue[6] = 'F';
		strValue[7] = 'F';
		strValue[8] = LF;

		for (int i = 0; i < (int)LIGHT_CONTROL_Q_BUFFER_MAX; i++)
		{
			strSendMsg += strValue[i];
		}

		if (WritePort(m_nPort, strValue) == FALSE)
		{
			goto ABNORMAL_EXIT;
		}

		m_nMode = LIGHT_CONTROL_READY;

		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);

		return TRUE;

	ABNORMAL_EXIT:

		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);
		return FALSE;

	}
	else
	{
	}
	return FALSE;
}
BOOL CLightController_ODA_Q::Write_Channel(int nChannel)
{
	ResetEvent(m_hEvent_HeaderBoard);

	char strValue[LIGHT_CONTROL_Q_BUFFER_MAX];
	char strInputVolt[LIGHT_CONTROL_Q_BUFFER_MAX];
	CString strSendMsg = _T("");

	BOOL bKill = TRUE;

	/////////////////////////////////////////////////////////////////////////////////////

	memset(strValue, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);
	memset(strInputVolt, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);

	if (m_bConnected == TRUE)
	{
		CString strTemp = _T("");
		if (m_nMode != LIGHT_CONTROL_Q_READY)
		{
			goto ABNORMAL_EXIT;
		}

		m_strRecvData = _T("");
		m_npos = 0;
		memset(recv, 0x00, sizeof(char)  * LIGHT_CONTROL_BUFFER_MAX);
		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_BUFFER_MAX);

		m_nMode = LIGHT_CONTROL_Q_DEFAULT_MODE;

		//Command		
		strValue[0] = 'I';		// stx
		strValue[1] = 'n';		// Command
		strValue[2] = 's';
		strValue[3] = 't';
		strValue[4] = 0x20;
		strValue[5] = 'O';
		strValue[6] = 'U';
		strValue[7] = 'T';
		strValue[8] = 'P';
		strValue[9] = '1';
		strValue[10] = LF;

		for (int i = 0; i < (int)LIGHT_CONTROL_Q_BUFFER_MAX; i++)
		{
			strSendMsg += strValue[i];
		}

		if (WritePort(m_nPort, strValue) == FALSE)
		{
			goto ABNORMAL_EXIT;
		}

		m_nMode = LIGHT_CONTROL_READY;

		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);

		return TRUE;

	ABNORMAL_EXIT:

		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);
		return FALSE;

	}
	else
	{
	}
	return FALSE;
}

BOOL CLightController_ODA_Q::Wrtie_ON()
{
	ResetEvent(m_hEvent_HeaderBoard);

	char strValue[LIGHT_CONTROL_Q_BUFFER_MAX];
	char strInputVolt[LIGHT_CONTROL_Q_BUFFER_MAX];
	CString strSendMsg = _T("");

	BOOL bKill = TRUE;

	/////////////////////////////////////////////////////////////////////////////////////

	memset(strValue, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);
	memset(strInputVolt, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);

	if (m_bConnected == TRUE)
	{
		CString strTemp = _T("");
		if (m_nMode != LIGHT_CONTROL_Q_READY)
		{
			goto ABNORMAL_EXIT;
		}

		m_strRecvData = _T("");
		m_npos = 0;
		memset(recv, 0x00, sizeof(char)  * LIGHT_CONTROL_BUFFER_MAX);
		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_BUFFER_MAX);

		m_nMode = LIGHT_CONTROL_Q_DEFAULT_MODE;

		strValue[0] = 'O';		// stx
		strValue[1] = 'u';		// Command
		strValue[2] = 't';
		strValue[3] = 'p';
		strValue[4] = 0x20;
		strValue[5] = 'O';
		strValue[6] = 'N';
		strValue[7] = LF;
		for (int i = 0; i < (int)LIGHT_CONTROL_Q_BUFFER_MAX; i++)
		{
			strSendMsg += strValue[i];
		}

		if (WritePort(m_nPort, strValue) == FALSE)
		{
			goto ABNORMAL_EXIT;
		}

		m_nMode = LIGHT_CONTROL_READY;

		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);

		return TRUE;

	ABNORMAL_EXIT:

		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);
		return FALSE;

	}
	else
	{
	}
	return FALSE;
}

BOOL CLightController_ODA_Q::Read_Current()
{
	ResetEvent(m_hEvent_HeaderBoard);

	char strValue[LIGHT_CONTROL_Q_BUFFER_MAX];
	char strInputVolt[LIGHT_CONTROL_Q_BUFFER_MAX];
	CString strSendMsg = _T("");

	BOOL bKill = TRUE;

	/////////////////////////////////////////////////////////////////////////////////////

	memset(strValue, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);
	memset(strInputVolt, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);

	if (m_bConnected == TRUE)
	{
		m_nReadMode = 1;

		CString strTemp = _T("");
		if (m_nMode != LIGHT_CONTROL_Q_READY)
		{
			goto ABNORMAL_EXIT;
		}

		m_strRecvData = _T("");
		m_npos = 0;
		memset(recv, 0x00, sizeof(char)  * LIGHT_CONTROL_BUFFER_MAX);
		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_BUFFER_MAX);

		m_nMode = LIGHT_CONTROL_Q_DEFAULT_MODE;

		//Command		
		strValue[0] = 'C';		// stx
		strValue[1] = 'u';		// Command
		strValue[2] = 'r';
		strValue[3] = 'r';
		strValue[4] = '?';
		strValue[5] = LF;

		for (int i = 0; i < (int)LIGHT_CONTROL_Q_BUFFER_MAX; i++)
		{
			strSendMsg += strValue[i];
		}

		if (WritePort(m_nPort, strValue) == FALSE)
		{
			goto ABNORMAL_EXIT;
		}

		m_nMode = LIGHT_CONTROL_READY;

		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);

		return TRUE;

	ABNORMAL_EXIT:

		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);
		return FALSE;

	}
	else
	{
	}
	return FALSE;

}

BOOL CLightController_ODA_Q::Read_Volt()
{
	ResetEvent(m_hEvent_HeaderBoard);

	char strValue[LIGHT_CONTROL_Q_BUFFER_MAX];
	char strInputVolt[LIGHT_CONTROL_Q_BUFFER_MAX];
	CString strSendMsg = _T("");

	BOOL bKill = TRUE;

	/////////////////////////////////////////////////////////////////////////////////////

	memset(strValue, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);
	memset(strInputVolt, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);

	if (m_bConnected == TRUE)
	{
		m_nReadMode = 0;

		CString strTemp = _T("");
		if (m_nMode != LIGHT_CONTROL_Q_READY)
		{
			goto ABNORMAL_EXIT;
		}

		m_strRecvData = _T("");
		m_npos = 0;
		memset(recv, 0x00, sizeof(char)  * LIGHT_CONTROL_BUFFER_MAX);
		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_BUFFER_MAX);

		m_nMode = LIGHT_CONTROL_Q_DEFAULT_MODE;

		//Command		
		strValue[0] = 'v';		// stx
		strValue[1] = 'o';		// Command
		strValue[2] = 'l';
		strValue[3] = 't';
		strValue[4] = '?';
		strValue[5] = LF;

		for (int i = 0; i < (int)LIGHT_CONTROL_Q_BUFFER_MAX; i++)
		{
			strSendMsg += strValue[i];
		}

		if (WritePort(m_nPort, strValue) == FALSE)
		{
			goto ABNORMAL_EXIT;
		}

		m_nMode = LIGHT_CONTROL_READY;

		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);

		return TRUE;

	ABNORMAL_EXIT:

		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);
		return FALSE;

	}
	else
	{
	}
	return FALSE;
}

void CLightController_ODA_Q::SetCurrentValue(double dVal)
{
	m_dGetCurrent = dVal;
}

void CLightController_ODA_Q::SetVoltValue(double dVal)
{
	m_dGetVolt = dVal;
}

double CLightController_ODA_Q::GetCurrentValue()
{
	return m_dGetCurrent;
}

double CLightController_ODA_Q::GetVoltValue()
{
	return m_dGetVolt;
}

BOOL CLightController_ODA_Q::Write_Volt(double dVolt)
{
	ResetEvent(m_hEvent_HeaderBoard);

	char strValue[LIGHT_CONTROL_Q_BUFFER_MAX];
	char strInputVolt[LIGHT_CONTROL_Q_BUFFER_MAX];
	CString strSendMsg = _T("");

	BOOL bKill = TRUE;

	/////////////////////////////////////////////////////////////////////////////////////

	memset(strValue, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);
	memset(strInputVolt, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);

	if (m_bConnected == TRUE)
	{
		CString strTemp = _T("");
		int nTemp = 0;
		if (m_nMode != LIGHT_CONTROL_Q_READY)
		{
			goto ABNORMAL_EXIT;
		}

		m_strRecvData = _T("");
		m_npos = 0;
		memset(recv, 0x00, sizeof(char)  * LIGHT_CONTROL_BUFFER_MAX);
		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_BUFFER_MAX);

		m_nMode = LIGHT_CONTROL_Q_DEFAULT_MODE;

		//Command		
		strValue[0] = 'V';		// stx
		strValue[1] = 'o';		// Command
		strValue[2] = 'l';
		strValue[3] = 't';
		strValue[4] = 0x20;
		strValue[5] = '1';
		strValue[6] = '2';
		strValue[7] = LF;

		for (int i = 0; i < (int)LIGHT_CONTROL_Q_BUFFER_MAX; i++)
		{
			strSendMsg += strValue[i];
		}

		if (WritePort(m_nPort, strValue) == FALSE)
		{
			goto ABNORMAL_EXIT;
		}

		m_nMode = LIGHT_CONTROL_READY;

		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);

		return TRUE;

	ABNORMAL_EXIT:

		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);
		return FALSE;

	}
	else
	{
	}
	return FALSE;
}
BOOL CLightController_ODA_Q::Write_Current(double dCur)
{
	ResetEvent(m_hEvent_HeaderBoard);

	char strValue[LIGHT_CONTROL_Q_BUFFER_MAX];
	char strInputVolt[LIGHT_CONTROL_Q_BUFFER_MAX];
	CString strSendMsg = _T("");

	BOOL bKill = TRUE;

	/////////////////////////////////////////////////////////////////////////////////////

	memset(strValue, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);
	memset(strInputVolt, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);

	if (m_bConnected == TRUE)
	{
		CString strTemp = _T("");
		if (m_nMode != LIGHT_CONTROL_Q_READY)
		{
			goto ABNORMAL_EXIT;
		}

		m_strRecvData = _T("");
		m_npos = 0;
		memset(recv, 0x00, sizeof(char)  * LIGHT_CONTROL_BUFFER_MAX);
		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_BUFFER_MAX);

		m_nMode = LIGHT_CONTROL_Q_DEFAULT_MODE;

		//Command		
		strValue[0] = 'C';		// stx
		strValue[1] = 'u';		// Command
		strValue[2] = 'r';
		strValue[3] = 'r';
		strValue[4] = 0x20;
		strValue[5] = '1';
		strValue[6] = LF;

		for (int i = 0; i < (int)LIGHT_CONTROL_Q_BUFFER_MAX; i++)
		{
			strSendMsg += strValue[i];
		}

		if (WritePort(m_nPort, strValue) == FALSE)
		{
			goto ABNORMAL_EXIT;
		}

		m_nMode = LIGHT_CONTROL_READY;

		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);

		return TRUE;

	ABNORMAL_EXIT:

		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);
		return FALSE;

	}
	else
	{
	}
	return FALSE;
}
