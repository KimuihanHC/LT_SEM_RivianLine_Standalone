//*****************************************************************************
// Filename	: 	PCBCamBrd.cpp
// Created	:	2016/05/09
// Modified	:	2016/05/09
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************

#include "StdAfx.h"
#include "PCBCamBrd.h"
#include "ErrorCode.h"

using namespace PR_ErrorCode;

//=============================================================================
// 생성자
//=============================================================================
CPCBCamBrd::CPCBCamBrd()
{
	SetDelimeter(CamBrd_STX, CamBrd_ETX, CamBrd_ProtoLength, CamBrd_ProtoLength);
}

//=============================================================================
// 소멸자
//=============================================================================
CPCBCamBrd::~CPCBCamBrd()
{
	TRACE(_T("<<< Start ~CPCBCamBrd >>> \n"));



	TRACE(_T("<<< End ~CPCBCamBrd >>> \n"));
}

//=============================================================================
// Method		: CPCBCamBrd::OnFilterRecvData
// Access		: protected 
// Returns		: LRESULT
//					 1	-> ACK 처리 성공
//					 0	-> 데이터가 중간에 짤렸음 다음 데이터 수신시 처리
//					-1	-> 쓰레기 데이터 처리
//					-2	-> 매개변수 오류
// Parameter	: const char * szACK	-> 수신된 ACK 데이터
// Parameter	: DWORD dwAckSize		-> 수신된 ACK 데이터 크기
// Qualifier	:
// Last Update	: 2015/11/28 - 16:35
// Desc.		: 
//=============================================================================
LRESULT CPCBCamBrd::OnFilterRecvData(const char* szACK, DWORD dwAckSize)
{
	//LRESULT lReturn = CSerialCom_Base::OnFilterRecvData(szACK, dwAckSize);
	LRESULT lReturn = TRUE;

	if (NULL == szACK)
	{
		ASSERT(_T("NULL == szACK"));
		m_dwErrCode = ERROR_INVALID_PARAMETER;
		TRACE(_T("FilterAckData() : %d\n"), __LINE__);
		return FALSE;
	}

	ASSERT(0 != dwAckSize);

	DWORD_PTR	dwQueueSize = 0;
	DWORD		dwDataLength = 0;

	INT_PTR		iFindSTXIndex = 0;
	INT_PTR		iFindETXIndex = 0;

	//ResetAckBuffer();
	m_dwACKBufSize = 0;

	//-----------------------------------------------------
	// Data가 들어오면 우선 큐에 집어 넣는다?
	//-----------------------------------------------------
	m_SerQueue.Push(szACK, dwAckSize);

	//-----------------------------------------------------
	// STX를 찾는다.
	//-----------------------------------------------------
	if (0 <= (iFindSTXIndex = m_SerQueue.EraseUntilFindDelimiter(m_chSTX)))
	{
		//-----------------------------------------------------
		// ETX를 찾는다.
		//-----------------------------------------------------
		dwQueueSize = m_SerQueue.GetSize();

		// 프로토콜 사이즈보다 작으면 데이터가 들어오길 기다린다.
		if (dwQueueSize < (DWORD)m_nMinProtocolLength)
		{
			TRACE(_T("FilterAckData() : %d\n"), __LINE__);
			return FALSE;
		}

		iFindETXIndex = 0;
		while (0 < (iFindETXIndex = m_SerQueue.FindDelimiter(m_chETX, iFindETXIndex)))
		{
			dwDataLength = (DWORD)iFindETXIndex + 1;
			m_SerQueue.PopData(m_szACKBuf, (DWORD)iFindETXIndex + 1);
			break;
		}

		if (iFindETXIndex <= 0) // STX를 찾고 ETX를 못찾으면 데이터가 중간에 나뉘어 들어온걸로 판단함
		{
			// STX ~> ETX 까지 검색한 데이터 사이즈가 프로토콜 사이즈보다 넘어가면 다음 데이터가 들어오면 처리
			if (dwQueueSize < (DWORD)m_nMaxProtocolLength)
			{
				//TRACE(_T("FilterAckData() : %d\n"), __LINE__);
				return FALSE;
			}
			else // 아니면 에러 처리
			{
				m_SerQueue.Empty();
				TRACE(_T("FilterAckData() : %d\n"), __LINE__);
				return FALSE;
			}
		}
	}
	else // STX가 없으면 쓰레기 데이터로 간주하고 처리	
	{
		// 오류 : 쓰레기 데이터
		m_SerQueue.Empty();
		TRACE(_T("FilterAckData() : %d\n"), __LINE__);
		return FALSE;
	}

	m_dwACKBufSize = dwDataLength;
	m_szACKBuf[m_dwACKBufSize] = 0x00;
	OnRecvProtocol(m_szACKBuf, m_dwACKBufSize);

	// 이벤트 핸들이 설정되어있다면 이벤트를 발생시킨다.
	if (NULL != m_hEvent_ACK)
	{
		//if ((CMD_Button != m_stRecvProtocol.CMD) && (CMD_FailCount != m_stRecvProtocol.CMD))
		if (CMD_OverCurrent != m_stRecvProtocol.CMD)
		{
			SetEvent(m_hEvent_ACK);
		}
	}

	TRACE(_T("FilterAckData : Complete Ack Protocol\n"));

	//-----------------------------------------------------
	// 큐 비우기
	//-----------------------------------------------------
	m_SerQueue.Empty();
	m_dwErrCode = NO_ERROR;

	lReturn = TRUE;
	if (TRUE == lReturn)
	{
		//TRACE(_T("OnFilterRecvData : Complete Ack Protocol\n"));
		if (NULL != m_hOwnerWnd)
		{
			if (CMD_OverCurrent == m_stRecvProtocol.CMD)
			{
				//m_stBnCtrl_Read = m_stRecvProtocol.Button;
				::SendNotifyMessage(m_hOwnerWnd, m_WM_Ack, (WPARAM)m_szACKBuf, (LPARAM)m_dwACKBufSize);
			}
		}
	}

	return lReturn;
}

//=============================================================================
// Method		: OnRecvProtocol
// Access		: virtual protected  
// Returns		: void
// Parameter	: const char * szACK
// Parameter	: DWORD dwAckSize
// Qualifier	:
// Last Update	: 2016/5/9 - 23:27
// Desc.		:
//=============================================================================
void CPCBCamBrd::OnRecvProtocol(const char* szACK, DWORD dwAckSize)
{
	m_stRecvProtocol.SetRecvProtocol(szACK, dwAckSize);
}

void CPCBCamBrd::ResetProtocol()
{

}

//=============================================================================
// Method		: Send_BoardCheck
// Access		: public  
// Returns		: LRESULT
// Parameter	: __out BYTE & byBrdNo : 1, 2, 3, 4, 5, 6
// Qualifier	:
// Last Update	: 2016/5/9 - 23:27
// Desc.		:
//=============================================================================
LRESULT CPCBCamBrd::Send_BoardCheck(__out BYTE& byBrdNo)
{
	TRACE(_T("CPCBCamBrd::Send_BoardCheck \n"));
	if (!IsOpen())
		return SRC_Err_NotOpen;

	if (WAIT_OBJECT_0 != LockSemaphore()) //-----------------------------------
		return SRC_Err_Semaphore;

	LRESULT	bReturn = SRC_OK;

	if (TransferStart(g_szCamBrd_BoardCheck, CamBrd_ProtoLength))
	{
		if (WaitACK(3000/*m_dwAckWaitTime*/))
		{
			// Board Number
			byBrdNo = m_stRecvProtocol.Data[0] - 0x30;

			bReturn = SRC_OK;
			TRACE(_T("CMD : 카메라 보드 Board Check ACK OK\n"));
		}
		else
		{
			bReturn = SRC_Err_Ack_Timeout;
			TRACE(_T("CMD : 카메라 보드 Board Check ACK ERROR\n"));
		}
	}
	else
	{
		bReturn = SRC_Err_Transfer;
	}

	UnlockSemaphore(); //------------------------------------------------------

	return bReturn;
}

//=============================================================================
// Method		: Send_SetVolt
// Access		: public  
// Returns		: LRESULT
// Parameter	: __in BOOL bOnOfff
// Qualifier	:
// Last Update	: 2017/9/4 - 13:23
// Desc.		:
//=============================================================================
LRESULT CPCBCamBrd::Send_SetVolt(__in BOOL bOnOfff)
{
	TRACE(_T("CPCBCamBrd::Send_SetVolt \n"));
	if (!IsOpen())
		return SRC_Err_NotOpen;

	if (WAIT_OBJECT_0 != LockSemaphore()) //-----------------------------------
		return SRC_Err_Semaphore;

	LRESULT	bReturn = SRC_OK;
		
	m_stProtocol.MakeProtocol_SetVolt(bOnOfff);

	if (TransferStart((char*)&m_stProtocol, CamBrd_ProtoLength))
	{
		if (WaitACK(3000/*m_dwAckWaitTime*/))
		{
			bReturn = SRC_OK;
			TRACE(_T("CMD : 카메라 보드 Set Volt ACK OK\n"));
		}
		else
		{
			bReturn = SRC_Err_Ack_Timeout;
			TRACE(_T("CMD : 카메라 보드 Set Volt ACK ERROR\n"));
		}
	}
	else
	{
		bReturn = SRC_Err_Transfer;
	}

	UnlockSemaphore(); //------------------------------------------------------

	return bReturn;
}

//=============================================================================
// Method		: Send_GetCurrent
// Access		: public  
// Returns		: LRESULT
// Parameter	: __out ST_CamBrdCurrent & stCurrent
// Qualifier	:
// Last Update	: 2017/9/4 - 13:30
// Desc.		:
//=============================================================================
LRESULT CPCBCamBrd::Send_GetCurrent(__out ST_CamBrdCurrent& stCurrent)
{
	TRACE(_T("CPCBCamBrd::Send_GetCurrent \n"));

	if (!IsOpen())
		return SRC_Err_NotOpen;

	if (WAIT_OBJECT_0 != LockSemaphore()) //-----------------------------------
		return SRC_Err_Semaphore;

	LRESULT	bReturn = SRC_OK;

	if (TransferStart(g_szCamBrd_GetInCurrent, CamBrd_ProtoLength))
	{
		if (WaitACK(3000/*m_dwAckWaitTime*/))
		{
			// Board Number
			for (UINT nCh = 0; nCh < CurrentMax; nCh++)
				stCurrent.fOutCurrent[nCh] = m_stRecvProtocol.fCurrent[nCh];

			bReturn = SRC_OK;
			TRACE(_T("CMD : 카메라 보드 Get Current ACK OK\n"));
		}
		else
		{
			bReturn = SRC_Err_Ack_Timeout;
			TRACE(_T("CMD : 카메라 보드 Get Current ACK ERROR\n"));
		}
	}
	else
	{
		bReturn = SRC_Err_Transfer;
	}

	UnlockSemaphore(); //------------------------------------------------------

	return bReturn;
}


//=============================================================================
// Method		: Send_CheckOverCurrent
// Access		: public  
// Returns		: LRESULT
// Parameter	: __out BOOL & bOverCurrent
// Qualifier	:
// Last Update	: 2018/2/12 - 19:43
// Desc.		:
//=============================================================================
LRESULT CPCBCamBrd::Send_CheckOverCurrent(__out BOOL& bOverCurrent)
{
	TRACE(_T("CPCBCamBrd::Send_CheckOverCurrent \n"));
	if (!IsOpen())
		return SRC_Err_NotOpen;

	if (WAIT_OBJECT_0 != LockSemaphore()) //-----------------------------------
		return SRC_Err_Semaphore;

	LRESULT	bReturn = SRC_OK;

	if (TransferStart(g_szCamBrd_CheckOverCurrent, CamBrd_ProtoLength))
	{
		if (WaitACK(3000/*m_dwAckWaitTime*/))
		{
			// Board Number
			bOverCurrent = m_stRecvProtocol.nOverCurrent;

			bReturn = SRC_OK;
			TRACE(_T("CMD : 카메라 보드 Send_CheckOverCurrent ACK OK\n"));
		}
		else
		{
			bReturn = SRC_Err_Ack_Timeout;
			TRACE(_T("CMD : 카메라 보드 Send_CheckOverCurrent ACK ERROR\n"));
		}
	}
	else
	{
		bReturn = SRC_Err_Transfer;
	}

	UnlockSemaphore(); //------------------------------------------------------

	return bReturn;
}


//=============================================================================
// Method		: Send_LEDDutyControl
// Access		: public  
// Returns		: LRESULT
// Parameter	: __in double dLEDRMSCycle
// Qualifier	:
// Last Update	: 2017/9/4 - 13:30
// Desc.		:
//=============================================================================
LRESULT CPCBCamBrd::Send_LEDDutyControl(__in double dLEDRMSCycle)
{
	TRACE(_T("CLGE_LightBrd::Send_EtcSetCurrent \n"));

	if (!IsOpen())
		return SRC_Err_NotOpen;

	if (WAIT_OBJECT_0 != LockSemaphore()) //-----------------------------------
		return SRC_Err_Semaphore;

	LRESULT	bReturn = SRC_OK;

	m_stProtocol.MakeProtocol_EtcSetLEDDutyControl(dLEDRMSCycle);

	if (TransferStart((char*)&m_stProtocol, CamBrd_ProtoLength))
	{
		if (WaitACK(3000/*m_dwAckWaitTime*/))
		{	
			bReturn = SRC_OK;
			TRACE(_T("CMD : 보드 Etc Set LEDDutyControl ACK OK\n"));
		}
		else
		{
			bReturn = SRC_Err_Ack_Timeout;
			TRACE(_T("CMD : 보드 Etc Set LEDDutyControl ACK Timeout ERROR\n"));
		}
	}
	else
	{
		bReturn = SRC_Err_Transfer;
	}

	UnlockSemaphore(); //------------------------------------------------------

	return bReturn;
}

//=============================================================================
// Method		: Send_LEDViselOnOff
// Access		: public  
// Returns		: LRESULT
// Parameter	: __in BOOL bOnOfff
// Qualifier	:
// Last Update	: 2018/8/11 - 15:47
// Desc.		:
//=============================================================================
LRESULT CPCBCamBrd::Send_LEDViselOnOff(__in BOOL bOnOfff)
{
	TRACE(_T("CPCBCamBrd::Send_LEDViselOnOff \n"));
	if (!IsOpen())
		return SRC_Err_NotOpen;

	if (WAIT_OBJECT_0 != LockSemaphore()) //-----------------------------------
		return SRC_Err_Semaphore;

	LRESULT	bReturn = SRC_OK;

	m_stProtocol.MakeProtocol_SetLED(bOnOfff);

	if (TransferStart((char*)&m_stProtocol, CamBrd_ProtoLength))
	{
		if (WaitACK(3000/*m_dwAckWaitTime*/))
		{
			bReturn = SRC_OK;
			TRACE(_T("CMD : 카메라 보드 Set LED ACK OK\n"));
		}
		else
		{
			bReturn = SRC_Err_Ack_Timeout;
			TRACE(_T("CMD : 카메라 보드 Set LED ACK ERROR\n"));
		}
	}
	else
	{
		bReturn = SRC_Err_Transfer;
	}

	UnlockSemaphore(); //------------------------------------------------------

	return bReturn;
}