           // HeaderBoard.cpp : 구현 파일입니다.
//

//#include "stdafx.h"
#include "pch.h"
#include "LightController_PD_FN300W_13CH.h"
#include "PCOMM/PCOMM.H"
#include "SerialParam.h"

#include "Common.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CLightController_Luri
CLightController_FN300 m_ctrlLight_FN300[2];

IMPLEMENT_DYNAMIC(CLightController_FN300, CWnd)

CLightController_FN300::CLightController_FN300()
{	
	//InitVariables();
}

CLightController_FN300::~CLightController_FN300()
{
	ClosePort();
}


BEGIN_MESSAGE_MAP(CLightController_FN300, CWnd)
	ON_MESSAGE(WM_LIGHT_CONTROL_FN300_MSG, OnLightControlReceive)
END_MESSAGE_MAP()



// CLightController_Luri 메시지 처리기입니다.

BOOL CLightController_FN300::Create(CWnd* pParentWnd)
{
	CRect rect = CRect(0, 0, 0, 0); // 화면 영역 설정
	return CWnd::Create(NULL, NULL, WS_CHILD, rect, GetDesktopWindow(), 0);
}

void CLightController_FN300::InitVariables(int nID)
{
	m_nId = nID;

	m_bConnected = FALSE;
	m_bHW = FALSE;
	m_bSW = FALSE;

	m_npos = 0;
	m_nMode = LIGHT_CONTROL_FN300_READY;

	m_strRevData = _T("");

	memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_FN300_BUFFER_MAX);
	memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_FN300_BUFFER_MAX);

	m_hEvent_HeaderBoard = NULL;
	m_hEvent_HeaderBoard = ::CreateEvent(NULL, TRUE,FALSE, NULL);	
}

void CALLBACK CLightController_FN300::Control_LIGHT_CONTROL_IRQ0(int port)
{	
	if(m_ctrlLight_FN300[0].m_bConnected == TRUE)
	{
		if(m_ctrlLight_FN300[0].m_hWnd != NULL)
		{
			::SendMessage(m_ctrlLight_FN300[0].m_hWnd, m_ctrlLight_FN300[0].m_MSG, m_ctrlLight_FN300[0].m_nPort, 0);
		}
	}	
}

void CALLBACK CLightController_FN300::Control_LIGHT_CONTROL_IRQ1(int port)
{	
	if(m_ctrlLight_FN300[1].m_bConnected == TRUE)
	{
		if(m_ctrlLight_FN300[1].m_hWnd != NULL)
		{
			::SendMessage(m_ctrlLight_FN300[1].m_hWnd, m_ctrlLight_FN300[1].m_MSG, m_ctrlLight_FN300[1].m_nPort, 0);
		}
	}	
}

BOOL CLightController_FN300::OpenPort(int Port,int BaudRate, UINT Msg, int Parity, int DataBit, int StopBit, int nIdx)
{
	if(m_bConnected == FALSE)
	{
#ifdef LIGHT_CONTROLLER_FN300_BOARD_USE

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

void CLightController_FN300::ClosePort()
{
	if(m_bConnected == TRUE)
	{
#ifdef LIGHT_CONTROLLER_FN300_BOARD_USE
		sio_close(m_nPort);
#endif
		m_bConnected = FALSE;
		m_nMode = LIGHT_CONTROL_FN300_READY;
	}	
}

BOOL CLightController_FN300::WritePort(int Port, char *szBuffer)
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

BOOL CLightController_FN300::ReadPort(int Port, char *szBuffer)
{
	char buf[LIGHT_CONTROL_FN300_BUFFER_MAX_RCV];
		
	if(m_bConnected == TRUE)
	{
		int ret;
		ret = sio_read(Port, buf, LIGHT_CONTROL_FN300_BUFFER_MAX_RCV);
		
		if(ret > 0)
		{
			if(ret > LIGHT_CONTROL_FN300_BUFFER_MAX_RCV)
			{
				ret = LIGHT_CONTROL_FN300_BUFFER_MAX_RCV;
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

LRESULT CLightController_FN300::OnLightControlReceive(WPARAM nPort, LPARAM nLen)
{
	int len = 0;
	int nPos = 0;
	int bufferlen = 0;
	
	bool re = true;

	char buffer[LIGHT_CONTROL_FN300_BUFFER_MAX_RCV];
	CString str = _T("");
	CString strLog = _T("");
	CString strTemp = _T("");
	CString strTemp_Command = _T("");

	////////////////////////////////////////////////////////////
	
	memset(buffer, 0x00, sizeof(char) * LIGHT_CONTROL_FN300_BUFFER_MAX_RCV);

	COMMON->uSleep_NoReturn(20);  

	if(ReadPort((int)nPort, buffer) == TRUE)
	{


	}
	//if(ReadPort((int)wParam, buffer) == TRUE)

	return 0;
}

BOOL CLightController_FN300::WaitOK(int timeout, BOOL& kill)
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


void CLightController_FN300::SetMinMaxVoltage(float fMin, float fMax)
{
	m_fMinCurrent = fMin;
	m_fMaxCurrent = fMax;

	m_fUnitCurrent = (m_fMaxCurrent - m_fMinCurrent) / 1024.0f;
}
void CLightController_FN300::SetMinMaxCurrent(float fMin, float fMax)
{

}
BOOL CLightController_FN300::Set_FN300_All_On()
{
	ResetEvent(m_hEvent_HeaderBoard);

	char strValue[LIGHT_CONTROL_FN300_BUFFER_MAX];
	char strInputVolt[LIGHT_CONTROL_FN300_BUFFER_MAX];
	CString strSendMsg = _T("");

	BOOL bKill = TRUE;

	/////////////////////////////////////////////////////////////////////////////////////

	memset(strValue, 0x00, sizeof(char) * LIGHT_CONTROL_FN300_BUFFER_MAX);
	memset(strInputVolt, 0x00, sizeof(char) * LIGHT_CONTROL_FN300_BUFFER_MAX);

	if (m_bConnected == TRUE)
	{
#ifdef LIGHT_CONTROLLER_FN300_BOARD_USE
		if (m_nMode != LIGHT_CONTROL_FN300_READY)
		{
			goto ABNORMAL_EXIT;
		}

		m_strRecvData = _T("");
		m_npos = 0;
		memset(recv, 0x00, sizeof(char)  * LIGHT_CONTROL_FN300_BUFFER_MAX);
		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_FN300_BUFFER_MAX);

		m_nMode = LIGHT_CONTROL_FN300_DEFAULT_MODE;

		//Command		
		//strValue[0] = 'H';		// Command
		//strValue[1] = 'T';		// Channel
		//strValue[2] = 'O';
		//strValue[3] = 'N';
		//strValue[4] = CR;
		//strValue[5] = LF;

		for (int i = 0; i < (int)LIGHT_CONTROL_FN300_BUFFER_MAX; i++)
		{
			strSendMsg += strValue[i];
		}

		if (WritePort(m_nPort, strValue) == FALSE)
		{
			goto ABNORMAL_EXIT;
		}


		for (int i = 10; i < 14; i++) // 사용하지 않은 광원을 오픈 할 경우 error 가 발생하여 비프음이 발생함 전체 on 후에 사용하지 않는 체널을 off 하도록 변경 
		{
			strSendMsg += strValue[i];
		
			//Command		
			strValue[0] = 'H';		// Command
			if (i > 0 && i < 10)
				strValue[1] = 0x31 + (i - 1);		// Channel
			else if (i >= 10 && i < 16)
				strValue[1] = 0x41 + (i - 10);		// Channel


			strValue[2] = 'O';
			strValue[3] = 'F';
			strValue[4] = CR;
			strValue[5] = LF;

			for (int i = 0; i < (int)LIGHT_CONTROL_FN300_BUFFER_MAX; i++)
			{
				strSendMsg += strValue[i];
			}
			Sleep(100);
			if (WritePort(m_nPort, strValue) == FALSE)
			{
				goto ABNORMAL_EXIT;
			}
		}



		m_nMode = LIGHT_CONTROL_FN300_READY;

		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_FN300_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_FN300_BUFFER_MAX);

		return TRUE;

	ABNORMAL_EXIT:

		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_FN300_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_FN300_BUFFER_MAX);
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
BOOL CLightController_FN300::Set_FN300_All_Off()
{
	ResetEvent(m_hEvent_HeaderBoard);

	char strValue[LIGHT_CONTROL_FN300_BUFFER_MAX];
	char strInputVolt[LIGHT_CONTROL_FN300_BUFFER_MAX];
	CString strSendMsg = _T("");

	BOOL bKill = TRUE;

	/////////////////////////////////////////////////////////////////////////////////////

	memset(strValue, 0x00, sizeof(char) * LIGHT_CONTROL_FN300_BUFFER_MAX);
	memset(strInputVolt, 0x00, sizeof(char) * LIGHT_CONTROL_FN300_BUFFER_MAX);

	if (m_bConnected == TRUE)
	{
#ifdef LIGHT_CONTROLLER_FN300_BOARD_USE
		if (m_nMode != LIGHT_CONTROL_FN300_READY)
		{
			goto ABNORMAL_EXIT;
		}

		m_strRecvData = _T("");
		m_npos = 0;
		memset(recv, 0x00, sizeof(char)  * LIGHT_CONTROL_FN300_BUFFER_MAX);
		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_FN300_BUFFER_MAX);

		m_nMode = LIGHT_CONTROL_FN300_DEFAULT_MODE;

		//Command		
		//strValue[0] = 'H';		// Command
		//strValue[1] = 'T';		// Channel
		//strValue[2] = 'O';
		//strValue[3] = 'F';
		//strValue[4] = CR;
		//strValue[5] = LF;

		for (int i = 0; i < (int)LIGHT_CONTROL_FN300_BUFFER_MAX; i++)
		{
			strSendMsg += strValue[i];
		}

		if (WritePort(m_nPort, strValue) == FALSE)
		{
			goto ABNORMAL_EXIT;
		}

		m_nMode = LIGHT_CONTROL_FN300_READY;

		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_FN300_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_FN300_BUFFER_MAX);

		return TRUE;

	ABNORMAL_EXIT:

		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_FN300_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_FN300_BUFFER_MAX);
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

BOOL CLightController_FN300::Set_FN300_C(int nChannel, int nVolt)
{
	ResetEvent(m_hEvent_HeaderBoard);

	char strValue[LIGHT_CONTROL_FN300_BUFFER_MAX];
	char strInputVolt[LIGHT_CONTROL_FN300_BUFFER_MAX];
	CString strSendMsg = _T("");

	BOOL bKill = TRUE;

	/////////////////////////////////////////////////////////////////////////////////////

	memset(strValue, 0x00, sizeof(char) * LIGHT_CONTROL_FN300_BUFFER_MAX);
	memset(strInputVolt, 0x00, sizeof(char) * LIGHT_CONTROL_FN300_BUFFER_MAX);

	if (m_bConnected == TRUE && nChannel != -1)
	{
#ifdef LIGHT_CONTROLLER_FN300_BOARD_USE
		if (m_nMode != LIGHT_CONTROL_FN300_READY)
		{
			goto ABNORMAL_EXIT;
		}

		m_strRecvData = _T("");
		m_npos = 0;
		memset(recv, 0x00, sizeof(char)  * LIGHT_CONTROL_FN300_BUFFER_MAX);
		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_FN300_BUFFER_MAX);

		m_nMode = LIGHT_CONTROL_FN300_DEFAULT_MODE;

		// 이쪽 조명은 0~255. 혹시라도 잘못 들어온 값이 있다면
		// 여기서 정리해주자.
		if (nVolt > 255)
			nVolt = 255;

		int nSmall = nVolt % 16;
		int nBig = nVolt / 16;

		//Command		
		strValue[0] = 'C';		// Command
		if( nChannel > 0 && nChannel < 10 )
			strValue[1] = 0x31 + (nChannel-1);		// Channel
		else if(nChannel >= 10 && nChannel < 16 )
			strValue[1] = 0x41 + ( nChannel-10);		// Channel
		else
		{
			if (nChannel == 0)
				strValue[1] = 'T';
			else
				return FALSE;
		}

		if (nBig < 10)
			strValue[2] = 0x30 + nBig;
		else
			strValue[2] = 0x41 + (nBig - 10);
			
		if (nSmall < 10)
			strValue[3] = 0x30 + nSmall;
		else
			strValue[3] = 0x41 + (nSmall-10);

		strValue[4] = CR;
		strValue[5] = LF;

		for (int i = 0; i < (int)LIGHT_CONTROL_FN300_BUFFER_MAX; i++)
		{
			strSendMsg += strValue[i];
		}

		if (WritePort(m_nPort, strValue) == FALSE)
		{
			goto ABNORMAL_EXIT;
		}

		m_nMode = LIGHT_CONTROL_FN300_READY;

		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_FN300_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_FN300_BUFFER_MAX);

		return TRUE;

	ABNORMAL_EXIT:

		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_FN300_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_FN300_BUFFER_MAX);
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