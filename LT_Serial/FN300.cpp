//*****************************************************************************
// Filename	: 	FN300.cpp
// Created	:	2016/05/09
// Modified	:	2016/05/09
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************

#include "StdAfx.h"
#include "FN300.h"
#include "ErrorCode.h"

using namespace PR_ErrorCode;

//=============================================================================
// 생성자
//=============================================================================
CFN300::CFN300()
{
	SetDelimeter(FN300_Recieve, FN300_LF, 1, FN300_ProtoLength);
}

//=============================================================================
// 소멸자
//=============================================================================
CFN300::~CFN300()
{
	TRACE(_T("<<< Start ~CFN300 >>> \n"));



	TRACE(_T("<<< End ~CFN300 >>> \n"));
}

//=============================================================================
// Method		: CFN300::OnFilterRecvData
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
LRESULT CFN300::OnFilterRecvData(const char* szACK, DWORD dwAckSize)
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
		//if (CMD_OverCurrent != m_stRecvProtocol.CMD)
		//{
			SetEvent(m_hEvent_ACK);
		//}
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
			//if (CMD_OverCurrent == m_stRecvProtocol.CMD)
			//{
				//m_stBnCtrl_Read = m_stRecvProtocol.Button;
			//	::SendNotifyMessage(m_hOwnerWnd, m_WM_Ack, (WPARAM)m_szACKBuf, (LPARAM)m_dwACKBufSize);
			//}
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
void CFN300::OnRecvProtocol(const char* szACK, DWORD dwAckSize)
{
	m_stRecvProtocol.SetRecvProtocol(szACK, dwAckSize);
}

void CFN300::ResetProtocol()
{

}

LRESULT CFN300::Send_ChannelData(__in UINT nCH, __in BYTE xData)
{
	TRACE(_T("CFN300::Send_ChannelData \n"));
	if (!IsOpen())
		return SRC_Err_NotOpen;

	if (WAIT_OBJECT_0 != LockSemaphore()) //-----------------------------------
		return SRC_Err_Semaphore;

	LRESULT	bReturn = SRC_OK;

	m_stProtocol.MakeProtocol_ChannelData(nCH, xData);

	if (TransferStart((const char*)&m_stProtocol, FN300_ProtoLength))
	{
		if (WaitACK(m_dwAckWaitTime))
		{
			TRACE(_T("Send_ChannelData ACK OK\n"));

			// Board Number
			if(m_stRecvProtocol.bResult == TRUE)
				bReturn = SRC_OK;
			else
				bReturn = SRC_Err_Unknown;
		}
		else
		{
			bReturn = SRC_Err_Ack_Timeout;
			TRACE(_T("Send_ChannelData ACK ERROR\n"));
		}
	}
	else
	{
		bReturn = SRC_Err_Transfer;
	}

	UnlockSemaphore(); //------------------------------------------------------

	return bReturn;

}

LRESULT CFN300::Send_ChannelOnOff(__in UINT nCH, __in BOOL bOnOff)
{
	TRACE(_T("CFN300::Send_BoardCheck \n"));
	if (!IsOpen())
		return SRC_Err_NotOpen;

	if (WAIT_OBJECT_0 != LockSemaphore()) //-----------------------------------
		return SRC_Err_Semaphore;

	LRESULT	bReturn = SRC_OK;

	m_stProtocol.MakeProtocol_ChannelOnOff(nCH, bOnOff);

	if (TransferStart((const char*)&m_stProtocol, FN300_ProtoLength))
	{
		if (WaitACK(m_dwAckWaitTime))
		{
			TRACE(_T("Send_ChannelOnOff ACK OK\n"));

			// Board Number
			if (m_stRecvProtocol.bResult == TRUE)
				bReturn = SRC_OK;
			else
				bReturn = SRC_Err_Unknown;
		}
		else
		{
			bReturn = SRC_Err_Ack_Timeout;
			TRACE(_T("Send_ChannelOnOff ERROR\n"));
		}
	}
	else
	{
		bReturn = SRC_Err_Transfer;
	}

	UnlockSemaphore(); //------------------------------------------------------

	return bReturn;
}

LRESULT CFN300::Send_AllChannelOnOff(__in BOOL bOnOff)
{
	TRACE(_T("CFN300::Send_BoardCheck \n"));
	if (!IsOpen())
		return SRC_Err_NotOpen;

	if (WAIT_OBJECT_0 != LockSemaphore()) //-----------------------------------
		return SRC_Err_Semaphore;

	LRESULT	bReturn = SRC_OK;

	BOOL bTransfer = FALSE;

	if (bOnOff)
	{
		bTransfer = TransferStart(g_szChannelOn, FN300_ProtoLength);
	}
	else
	{
		bTransfer = TransferStart(g_szChannelOff, FN300_ProtoLength);
	}

	if (bTransfer)
	{
		if (WaitACK(m_dwAckWaitTime))
		{
			TRACE(_T("Send_ChannelOnOff ACK OK\n"));

			// Board Number
			if (m_stRecvProtocol.bResult == TRUE)
				bReturn = SRC_OK;
			else
				bReturn = SRC_Err_Unknown;
		}
		else
		{
			bReturn = SRC_Err_Ack_Timeout;
			TRACE(_T("Send_ChannelOnOff ERROR\n"));
		}
	}
	else
	{
		bReturn = SRC_Err_Transfer;
	}

	UnlockSemaphore(); //------------------------------------------------------

	return bReturn;
}
