           // HeaderBoard.cpp : 구현 파일입니다.
//

//#include "stdafx.h"
#include "pch.h"
#include "LightController_IPL_L100.h"
#include "PCOMM/PCOMM.H"
#include "SerialParam.h"

#include "Common.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CLightController_Luri
CLightController_L100 m_ctrlLight_L100[LIGHT_CONTROLLER_IPL_L100_COUNT];

IMPLEMENT_DYNAMIC(CLightController_L100, CWnd)

CLightController_L100::CLightController_L100()
{	
	//InitVariables();
}

CLightController_L100::~CLightController_L100()
{
	ClosePort();
}


BEGIN_MESSAGE_MAP(CLightController_L100, CWnd)
	ON_MESSAGE(WM_LIGHT_CONTROL_L100_MSG, OnLightControlReceive)
END_MESSAGE_MAP()



// CLightController_Luri 메시지 처리기입니다.

BOOL CLightController_L100::Create(CWnd* pParentWnd)
{
	CRect rect = CRect(0, 0, 0, 0); // 화면 영역 설정
	return CWnd::Create(NULL, NULL, WS_CHILD, rect, GetDesktopWindow(), 0);
}

void CLightController_L100::InitVariables(int nID)
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

void CALLBACK CLightController_L100::Control_LIGHT_CONTROL_IRQ0(int port)
{	
	if(m_ctrlLight_L100[0].m_bConnected == TRUE)
	{
		if(m_ctrlLight_L100[0].m_hWnd != NULL)
		{
			::SendMessage(m_ctrlLight_L100[0].m_hWnd, m_ctrlLight_L100[0].m_MSG, m_ctrlLight_L100[0].m_nPort, 0);
		}
	}	
}

void CALLBACK CLightController_L100::Control_LIGHT_CONTROL_IRQ1(int port)
{	
	if(m_ctrlLight_L100[1].m_bConnected == TRUE)
	{
		if(m_ctrlLight_L100[1].m_hWnd != NULL)
		{
			::SendMessage(m_ctrlLight_L100[1].m_hWnd, m_ctrlLight_L100[1].m_MSG, m_ctrlLight_L100[1].m_nPort, 0);
		}
	}	
}

BOOL CLightController_L100::OpenPort(int Port,int BaudRate, UINT Msg, int Parity, int DataBit, int StopBit, int nIdx)
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

void CLightController_L100::ClosePort()
{
	if(m_bConnected == TRUE)
	{
#ifdef SCARA_CONTROL_USE
		sio_close(m_nPort);
#endif
		m_bConnected = FALSE;
		m_nMode = LIGHT_CONTROL_L100_READY;
	}	
}

BOOL CLightController_L100::WritePort(int Port, char *szBuffer)
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

BOOL CLightController_L100::WritePort_Length(int Port, int nLength, char *szBuffer)
{
	CString strLog = _T("");
	CString strTemp = _T("");

	if (m_bConnected == TRUE)
	{
		int ret = 0, len = 0;
		CString str = _T("");

		len = nLength;

		// Flush any data within input and/or output buffer
		// 0 : Input Buffer , 1: Output Buffer , 2 : Input / Output Buffer;

		ret = sio_flush(Port, 2);
		if (ret != SIO_OK)
		{
			return FALSE;
		}

		ret = sio_write(Port, szBuffer, len);
		if (ret < 0)
		{
			return FALSE;
		}

		//Log
		for (int j = 0; j < len; j++)		
		{
			strTemp += szBuffer[j];
		}
		strLog.Format(_T("[LIGHT_CONTROL] TX : %s [ Scara : %d ]\n"), strTemp, m_nId + 1);
	}
	else
	{
		if (m_bConnected == FALSE)
		{
		}
		return FALSE;
	}

	return TRUE;

}

BOOL CLightController_L100::ReadPort(int Port, char *szBuffer)
{
	char buf[LIGHT_CONTROL_BUFFER_MAX_RCV];
		
	if(m_bConnected == TRUE)
	{
		int ret;
		ret = sio_read(Port, buf, LIGHT_CONTROL_L100_BUFFER_MAX_RCV);
		
		if(ret > 0)
		{
			if(ret > LIGHT_CONTROL_L100_BUFFER_MAX_RCV)
			{
				ret = LIGHT_CONTROL_L100_BUFFER_MAX_RCV;
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

LRESULT CLightController_L100::OnLightControlReceive(WPARAM nPort, LPARAM nLen)
{
	int len = 0;
	int nPos = 0;
	int bufferlen = 0;
	
	bool re = true;

	char buffer[LIGHT_CONTROL_L100_BUFFER_MAX_RCV];
	CString str = _T("");
	CString strLog = _T("");
	CString strTemp = _T("");
	CString strTemp_Command = _T("");

	////////////////////////////////////////////////////////////
	
	memset(buffer, 0x00, sizeof(char) * LIGHT_CONTROL_L100_BUFFER_MAX_RCV);

	COMMON->uSleep_NoReturn(20);  

	if(ReadPort((int)nPort, buffer) == TRUE)
	{
		for (int i = 0; i < (int)LIGHT_CONTROL_L100_BUFFER_MAX_RCV; i++)
		{
			if (buffer[i] == 0x03)
				break;
			else
				len++;
		}
		

	}
	return 0;
}

BOOL CLightController_L100::WaitOK(int timeout, BOOL& kill)
{
	DWORD time = GetTickCount();

	while (1)
	{
		if (kill == FALSE)
		{
			return FALSE;
		}
		if (WaitForSingleObject(m_hEvent_HeaderBoard, 5) == WAIT_OBJECT_0)
		{
			return TRUE;
		}
		if (GetTickCount() - time > (DWORD)timeout)
		{
			return FALSE;
		}
		Sleep(1);
		COMMON->DoEvents();
	}
	return FALSE;
}

BOOL CLightController_L100::SendLampValueChange(int nValue)
{
	ResetEvent(m_hEvent_HeaderBoard);

	char strValue[LIGHT_CONTROL_L100_BUFFER_MAX];
	//char strPoint[2];					//Point 최대 99개 까지 가능	
	CString strNum = _T("");
	CString strSendMsg = _T("");

	BOOL bKill = TRUE;

	/////////////////////////////////////////////////////////////////////////////////////

	memset(strValue, 0x00, sizeof(char) * LIGHT_CONTROL_L100_BUFFER_MAX);

	if (m_bConnected == TRUE)
	{

		if (m_nMode != LIGHT_CONTROL_L100_READY)
		{
			goto ABNORMAL_EXIT;
		}

		m_strRecvData_LightController = _T("");
		m_npos = 0;
		memset(recv, 0x00, sizeof(char)  * LIGHT_CONTROL_L100_BUFFER_MAX_RCV);
		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_L100_BUFFER_MAX);

		m_nMode = LIGHT_CONTROL_L100_DEFAULT_MODE;

		//Command
		strValue[0] = 0x5A;
		strValue[1] = 0x0E;
		strValue[2] = 0x01;
		strValue[3] = 0x02;
		strValue[4] = 0x00;

		int nSmall = nValue % 256;
		int nBig = nValue / 256;

		strValue[5] = 0x00 + nSmall;
		strValue[6] = 0x00 + nBig;

		strValue[7] = 0x00;
		strValue[8] = 0x00;

		strValue[9] = 0x00;
		strValue[10] = 0x00;

		strValue[11] = 0x00;
		strValue[12] = 0x00;

		strValue[13] = 0x03;

		for (int i = 0; i < 14; i++)
		{
			strSendMsg += strValue[i];
		}

		if (WritePort_Length(m_nPort, 14, strValue) == FALSE)
		{
			goto ABNORMAL_EXIT;
		}
		m_nMode = LIGHT_CONTROL_L100_READY;
		m_npos = 0;
		memset(recv, 0x00, sizeof(char)  * LIGHT_CONTROL_L100_BUFFER_MAX_RCV);
		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_L100_BUFFER_MAX);

		return TRUE;


ABNORMAL_EXIT:
	m_nMode = LIGHT_CONTROL_L100_READY;
	m_npos = 0;
	memset(recv, 0x00, sizeof(char)  * LIGHT_CONTROL_L100_BUFFER_MAX_RCV);
	memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_L100_BUFFER_MAX);
	return FALSE;

	}
	else
	{
		return FALSE;
	}

	return FALSE;
}

BOOL CLightController_L100::SendLampOn()
{
	ResetEvent(m_hEvent_HeaderBoard);

	char strValue[LIGHT_CONTROL_L100_BUFFER_MAX];
	//char strPoint[2];					//Point 최대 99개 까지 가능	
	CString strNum = _T("");
	CString strSendMsg = _T("");

	BOOL bKill = TRUE;

	/////////////////////////////////////////////////////////////////////////////////////

	memset(strValue, 0x00, sizeof(char) * LIGHT_CONTROL_L100_BUFFER_MAX);

	if (m_bConnected == TRUE)
	{

		if (m_nMode != LIGHT_CONTROL_L100_READY)
		{
			goto ABNORMAL_EXIT;
		}

		m_strRecvData_LightController = _T("");
		m_npos = 0;
		memset(recv, 0x00, sizeof(char)  * LIGHT_CONTROL_L100_BUFFER_MAX_RCV);
		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_L100_BUFFER_MAX);

		m_nMode = LIGHT_CONTROL_L100_DEFAULT_MODE;

		//Command
		strValue[0] = 0x5A;
		strValue[1] = 0x08;
		strValue[2] = 0x01;
		strValue[3] = 0x01;
		strValue[4] = 0x00;
		strValue[5] = 0x00;
		strValue[6] = 0x00;
		strValue[7] = 0x03;

		for (int i = 0; i < 8; i++)
		{
			strSendMsg += strValue[i];
		}

		if (WritePort_Length(m_nPort, 8, strValue) == FALSE)
		{
			goto ABNORMAL_EXIT;
		}

		m_nMode = LIGHT_CONTROL_L100_READY;
		m_npos = 0;
		memset(recv, 0x00, sizeof(char)  * LIGHT_CONTROL_L100_BUFFER_MAX_RCV);
		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_L100_BUFFER_MAX);

		return TRUE;


	ABNORMAL_EXIT:
		m_nMode = LIGHT_CONTROL_L100_READY;
		m_npos = 0;
		memset(recv, 0x00, sizeof(char)  * LIGHT_CONTROL_L100_BUFFER_MAX_RCV);
		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_L100_BUFFER_MAX);
		return FALSE;

	}
	else
	{
		return FALSE;
	}

	return FALSE;
}
BOOL CLightController_L100::SendLampOff()
{
	ResetEvent(m_hEvent_HeaderBoard);

	char strValue[LIGHT_CONTROL_L100_BUFFER_MAX];
	//char strPoint[2];					//Point 최대 99개 까지 가능	
	CString strNum = _T("");
	CString strSendMsg = _T("");

	BOOL bKill = TRUE;

	/////////////////////////////////////////////////////////////////////////////////////

	memset(strValue, 0x00, sizeof(char) * LIGHT_CONTROL_L100_BUFFER_MAX);

	if (m_bConnected == TRUE)
	{

		if (m_nMode != LIGHT_CONTROL_L100_READY)
		{
			goto ABNORMAL_EXIT;
		}

		m_strRecvData_LightController = _T("");
		m_npos = 0;
		memset(recv, 0x00, sizeof(char)  * LIGHT_CONTROL_L100_BUFFER_MAX_RCV);
		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_L100_BUFFER_MAX);

		m_nMode = LIGHT_CONTROL_L100_DEFAULT_MODE;

		//Command
		strValue[0] = 0x5A;
		strValue[1] = 0x08;
		strValue[2] = 0x01;
		strValue[3] = 0x01;
		strValue[4] = 0x00;
		strValue[5] = 0x0F;
		strValue[6] = 0x0F;
		strValue[7] = 0x03;

		for (int i = 0; i < 8; i++)
		{
			strSendMsg += strValue[i];
		}

		if (WritePort_Length(m_nPort, 8, strValue) == FALSE)
		{
			goto ABNORMAL_EXIT;
		}

		m_nMode = LIGHT_CONTROL_L100_READY;
		m_npos = 0;
		memset(recv, 0x00, sizeof(char)  * LIGHT_CONTROL_L100_BUFFER_MAX_RCV);
		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_L100_BUFFER_MAX);

		return TRUE;


	ABNORMAL_EXIT:
		m_nMode = LIGHT_CONTROL_L100_READY;
		m_npos = 0;
		memset(recv, 0x00, sizeof(char)  * LIGHT_CONTROL_L100_BUFFER_MAX_RCV);
		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_L100_BUFFER_MAX);
		return FALSE;

	}
	else
	{
		return FALSE;
	}

	return FALSE;
}