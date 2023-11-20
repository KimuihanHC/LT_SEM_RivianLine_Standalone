           // HeaderBoard.cpp : 구현 파일입니다.
//

//#include "stdafx.h"
#include "pch.h"
#include "LightController_YouthTech.h"
#include "PCOMM/PCOMM.H"
#include "SerialParam.h"

#include "Common.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CLightController_Luri
CLightController_YouthTech m_ctrlLightController_Youth[LIGHT_CONTROLLER_LURITECH_BOARD_COUNT];

IMPLEMENT_DYNAMIC(CLightController_YouthTech, CWnd)

CLightController_YouthTech::CLightController_YouthTech()
{	
	//InitVariables();
}

CLightController_YouthTech::~CLightController_YouthTech()
{
	ClosePort();
}


BEGIN_MESSAGE_MAP(CLightController_YouthTech, CWnd)
	ON_MESSAGE(WM_LIGHT_CONTROL_YOUTHTECH_MSG, OnLightControlReceive)
END_MESSAGE_MAP()



// CLightController_Luri 메시지 처리기입니다.

BOOL CLightController_YouthTech::Create(CWnd* pParentWnd)
{
	CRect rect = CRect(0, 0, 0, 0); // 화면 영역 설정
	return CWnd::Create(NULL, NULL, WS_CHILD, rect, GetDesktopWindow(), 0);
}

void CLightController_YouthTech::InitVariables(int nID)
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

void CALLBACK CLightController_YouthTech::Control_LIGHT_CONTROL_IRQ0_YOUTHTECH(int port)
{	
	if(m_ctrlLightController_Youth[0].m_bConnected == TRUE)
	{
		if(m_ctrlLightController_Youth[0].m_hWnd != NULL)
		{
			::SendMessage(m_ctrlLightController_Youth[0].m_hWnd, m_ctrlLightController_Youth[0].m_MSG, m_ctrlLightController_Youth[0].m_nPort, 0);
		}
	}	
}

void CALLBACK CLightController_YouthTech::Control_LIGHT_CONTROL_IRQ1_YOUTHTECH(int port)
{	
	if(m_ctrlLightController_Youth[1].m_bConnected == TRUE)
	{
		if(m_ctrlLightController_Youth[1].m_hWnd != NULL)
		{
			::SendMessage(m_ctrlLightController_Youth[1].m_hWnd, m_ctrlLightController_Youth[1].m_MSG, m_ctrlLightController_Youth[1].m_nPort, 0);
		}
	}	
}

BOOL CLightController_YouthTech::OpenPort(int Port,int BaudRate, UINT Msg, int Parity, int DataBit, int StopBit, int nIdx)
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
				ret = sio_cnt_irq(Port, Control_LIGHT_CONTROL_IRQ0_YOUTHTECH, 1);
			}
			else if(nIdx == 1)
			{
				ret = sio_cnt_irq(Port, Control_LIGHT_CONTROL_IRQ1_YOUTHTECH, 1);
			}
			if(ret != SIO_OK)
			{
				return false;
			}
			
			m_nPort = Port;
			m_MSG = Msg;
			m_bConnected = TRUE;
		}
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}

void CLightController_YouthTech::ClosePort()
{
	if(m_bConnected == TRUE)
	{
		sio_close(m_nPort);

		m_bConnected = FALSE;
		m_nMode = LIGHT_CONTROL_READY;
	}	
}

BOOL CLightController_YouthTech::WritePort(int Port, char *szBuffer)
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

BOOL CLightController_YouthTech::ReadPort(int Port, char *szBuffer)
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

LRESULT CLightController_YouthTech::OnLightControlReceive(WPARAM nPort, LPARAM nLen)
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
			
		}
		//if(len > 0)

	}
	//if(ReadPort((int)wParam, buffer) == TRUE)

	return 0;
}

BOOL CLightController_YouthTech::WaitOK(int timeout, BOOL& kill)
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

BOOL CLightController_YouthTech::Send_Light_Command(char Ch, char cCommand, int nData)
{
	char chBuf[10];
	int nInputData;

	if ((cCommand == YOUTHTECH_CMD_SET_ONOFF) || (cCommand == YOUTHTECH_CMD_ACTIVE) || (cCommand == YOUTHTECH_CMD_ONOFF_SELECT))
	{
		nInputData = ((nData & 0x00FF) << 8);
	}
	else
	{
		nInputData = nData;
	}

	chBuf[0] = 0x4C;			// STX
	chBuf[1] = cCommand;		// Command
	chBuf[2] = Ch;				// Channel
	chBuf[3] = (char)((nInputData >> 8) & 0x00FF);		//	Data 앞 두자리
	chBuf[4] = (char)(nInputData & 0x00FF);				// data 뒷 두자리
	chBuf[5] = 0x00;			// BCC

	for (int i = 0; i < 5; i++)
	{
		chBuf[5] += chBuf[i];
	}

	CString strSendMsg = _T("");

	for (int i = 0; i < (int)LIGHT_CONTROL_BUFFER_MAX; i++)
	{
		strSendMsg += chBuf[i];
	}

	if (WritePort(m_nPort, chBuf) == FALSE)
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

}
