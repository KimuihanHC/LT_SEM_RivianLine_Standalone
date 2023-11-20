//*****************************************************************************
// Filename	: 	TorqueDriver.cpp
// Created	:	2018/03/09
// Modified	:	2018/03/09
//
// Author	:	KHO
//	
// Purpose	:	
//*****************************************************************************
#include "StdAfx.h"
#include "TorqueDriver.h"
#include "CommonFunction.h"
#include "ErrorCode.h"

using namespace PR_ErrorCode;

//=============================================================================
// 생성자
//=============================================================================
CTorqueDriver::CTorqueDriver()
{
 	SetDelimeter(Torque_Sehan_STX, Torque_Sehan_ETX, Torque_Sen_Protocol_Length, Torque_Rew_Protocol_Length);
}

//=============================================================================
// 소멸자
//=============================================================================
CTorqueDriver::~CTorqueDriver()
{
	TRACE(_T("<<< Start ~CTorqueDriver >>> \n"));



	TRACE(_T("<<< End ~CTorqueDriver >>> \n"));
}

//=============================================================================
// Method		: CTorqueDriver::OnFilterRecvData
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
LRESULT CTorqueDriver::OnFilterRecvData(const char* szACK, DWORD dwAckSize)
{
	if (NULL == szACK)
	{
		ASSERT(_T("NULL == szACK"));
		m_dwErrCode = ERROR_INVALID_PARAMETER;
		TRACE(_T("FilterAckData() : %d\n"), __LINE__);
		return FALSE;
	}

	ASSERT(0 != dwAckSize);

	DWORD_PTR	dwQueueSize		= 0;
	DWORD		dwDataLength	= 0;
	INT_PTR		iFindETXIndex	= 0;

	//ResetAckBuffer();
	m_dwACKBufSize = 0;

	//-----------------------------------------------------
	// Data가 들어오면 우선 큐에 집어 넣는다?
	//-----------------------------------------------------
	m_SerQueue.Push(szACK, dwAckSize);

	//-----------------------------------------------------
	// LF 를 찾는다.
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
		// LF 위치가 프로토콜 마지막
		dwDataLength = (DWORD)iFindETXIndex + 1;
		m_SerQueue.PopData(m_szACKBuf, (DWORD)iFindETXIndex + 1);
		break;
	}

	if (iFindETXIndex <= 0) //LF 를 못찾으면 데이터가 중간에 나뉘어 들어온걸로 판단함
	{
		// LF 까지 검색한 데이터 사이즈가 프로토콜 사이즈보다 넘어가면 다음 데이터가 들어오면 처리
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

	m_dwACKBufSize = dwDataLength;
	m_szACKBuf[m_dwACKBufSize] = 0x00;
	OnRecvProtocol(m_szACKBuf, m_dwACKBufSize);

	// 이벤트 핸들이 설정되어있다면 이벤트를 발생시킨다.
	if (NULL != m_hEvent_ACK)
	{
		if (Cmd_Rev_Param == m_stRecvProtocol.CMD)
		{
			SetEvent(m_hEvent_ACK);
		}
	}

	//-----------------------------------------------------
	// 큐 비우기
	//-----------------------------------------------------
	m_SerQueue.Empty();
	m_dwErrCode = NO_ERROR;

	TRACE(_T("FilterAckData : Complete Ack Protocol\n"));
	if (NULL != m_hOwnerWnd)
	{
		if (Cmd_Rev_Monitor == m_stRecvProtocol.CMD)
		{
			::SendNotifyMessage(m_hOwnerWnd, m_WM_Ack, (WPARAM)m_szACKBuf, (LPARAM)m_dwACKBufSize);
		}
	}

	return TRUE;
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
void CTorqueDriver::OnRecvProtocol(const char* szACK, DWORD dwAckSize)
{
	if (dwAckSize <= Torque_Rew_Protocol_Length)
	{
		m_stRecvProtocol.SetRecvProtocol(szACK, dwAckSize);
	}
}

//=============================================================================
// Method		: Send_VersionCheck
// Access		: public  
// Returns		: BOOL
// Qualifier	:
// Last Update	: 2018/3/6 - 10:45
// Desc.		:
//=============================================================================
BOOL CTorqueDriver::Send_VersionCheck()
{
	TRACE(_T("CTorqueDriver::Send_VersionCheck \n"));

	if (!IsOpen())
		return FALSE;

	if (WAIT_OBJECT_0 != LockSemaphore()) //-----------------------------------
		return FALSE;

	BOOL bReturn = FALSE;

	if (TransferStart(g_szToqueDriver_Version, 7))
	{
		if (WaitACK(m_dwAckWaitTime))
		{
			m_dwErrCode = NO_ERROR;
			bReturn = TRUE;
			TRACE(_T("CMD : Torque Driver Send_VersionCheck ACK OK\n"));
		}
		else
		{
			m_dwErrCode = ERR_SERIAL_ACK;
			bReturn = FALSE;
			TRACE(_T("CMD : Torque Driver Send_VersionCheck ACK ERROR\n"));
		}
	}
	else
	{
		m_dwErrCode = ERR_SERIAL_TRANSFER;
		bReturn = FALSE;
	}

	UnlockSemaphore(); //------------------------------------------------------

	return bReturn;
}
