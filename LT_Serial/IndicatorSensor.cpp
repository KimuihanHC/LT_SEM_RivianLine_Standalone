//*****************************************************************************
// Filename	: 	IndicatorSensor.c
// Created	:	2017/1/2 - 17:57
// Modified	:	2017/1/2 - 17:57
//
// Author	:	KHO
//	
// Purpose	:	
//*****************************************************************************
#include "StdAfx.h"
#include "IndicatorSensor.h"
#include "CommonFunction.h"
#include "ErrorCode.h"

using namespace PR_ErrorCode;

//=============================================================================
// 생성자
//=============================================================================
CIndicatorSensor::CIndicatorSensor()
{
	SetDelimeter(NULL, IndicatorSensor::SEN_Protocol_LF, IndicatorSensor::SEN_Protocol_Length, IndicatorSensor::REV_Protocol_Length);
}

//=============================================================================
// 소멸자
//=============================================================================
CIndicatorSensor::~CIndicatorSensor()
{
	TRACE(_T("<<< Start ~CIndicatorSensor >>> \n"));

	TRACE(_T("<<< End ~CIndicatorSensor >>> \n"));
}

//=============================================================================
// Method		: OnEvent
// Access		: virtual protected  
// Returns		: void
// Parameter	: EEvent eEvent
// Parameter	: EError eError
// Qualifier	:
// Last Update	: 2015/12/28 - 13:22
// Desc.		:
//=============================================================================
void CIndicatorSensor::OnEvent(EEvent eEvent, EError eError)
{
	if (eEvent & CSerial::EEventRecv)
	{
		// Create a clean buffer
		ZeroMemory(m_szReadBuf, MAX_SERIAL_BUFFER);
		m_dwReadedBufSize = 0;

		const int nBufLen = sizeof(m_szReadBuf)-1;

		// Obtain the data from the serial port
		do
		{
			Read(m_szReadBuf, nBufLen, &m_dwReadedBufSize);
			m_szReadBuf[m_dwReadedBufSize] = '\0';

		} while (m_dwReadedBufSize == nBufLen);


		TRACE(_T("Recv Data[%d] : %s\n"), m_dwReadedBufSize, HexToCString(m_szReadBuf, m_dwReadedBufSize));

		if (0 < m_dwReadedBufSize)
			OnFilterRecvData(m_szReadBuf, m_dwReadedBufSize);
	}
}

//=============================================================================
// Method		: OnFilterRecvData
// Access		: virtual protected  
// Returns		: LRESULT
// Parameter	: const char * szACK
// Parameter	: DWORD dwAckSize
// Qualifier	:
// Last Update	: 2016/5/10 - 13:16
// Desc.		:
//=============================================================================
LRESULT CIndicatorSensor::OnFilterRecvData(const char* szACK, DWORD dwAckSize)
{
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
	INT_PTR		iFindETXIndex = 0;

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

	OnRecvProtocol(m_szACKBuf, m_dwACKBufSize);

	// 이벤트 핸들이 설정되어있다면 이벤트를 발생시킨다.
	if (NULL != m_hEvent_ACK)
		SetEvent(m_hEvent_ACK);

	TRACE(_T("FilterAckData : Complete Ack Protocol\n"));
	if (NULL != m_hOwnerWnd)
	{
		::SendNotifyMessage(m_hOwnerWnd, m_WM_Ack, (WPARAM)m_szACKBuf, (LPARAM)m_dwACKBufSize);
	}

	//-----------------------------------------------------
	// 큐 비우기
	//-----------------------------------------------------
	m_SerQueue.Empty();
	m_dwErrCode = NO_ERROR;

	return TRUE;
}

//=============================================================================
// Method		: OnRecvProtocol
// Access		: virtual protected  
// Returns		: void
// Parameter	: const char * szACK
// Parameter	: DWORD dwAckSize
// Qualifier	:
// Last Update	: 2017/1/10 - 14:32
// Desc.		:
//=============================================================================
void CIndicatorSensor::OnRecvProtocol(const char* szACK, DWORD dwAckSize)
{
	if (dwAckSize <= sizeof(IndicatorSensor::ST_Protocol_Response))
	{
		memcpy(&m_stRecvProtocol, szACK, dwAckSize);
	}
}

//=============================================================================
// Method		: Send_SetZero
// Access		: public  
// Returns		: BOOL
// Qualifier	:
// Last Update	: 2017/1/12 - 15:28
// Desc.		:
//=============================================================================
BOOL CIndicatorSensor::Send_SetZero()
{
	TRACE(_T("CIndicatorSensor::Send_PortCheck \n"));
	if (!IsOpen())
		return FALSE;

	if (WAIT_OBJECT_0 != LockSemaphore()) //-----------------------------------
		return FALSE;

	BOOL		bReturn = FALSE;
	CStringA	Protocol;

	m_stSendProtocol.Command = IndicatorSensor::Cmd_Zero_Data;

	if (TransferStart((char*)&m_stSendProtocol, IndicatorSensor::SEN_Protocol_Length))
	{
		if (WaitACK(m_dwAckWaitTime))
		{
			m_dwErrCode = NO_ERROR;
			bReturn = TRUE;
			TRACE(_T("Send_PortCheck ACK OK\n"));
		}
		else
		{
			m_dwErrCode = ERR_SERIAL_ACK;
			bReturn = FALSE;
			TRACE(_T("Send_PortCheck ACK ERROR\n"));
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

//=============================================================================
// Method		: Send_PortCheck
// Access		: public  
// Returns		: BOOL
// Parameter	: __out float & fValue
// Qualifier	:
// Last Update	: 2017/1/10 - 15:58
// Desc.		:
//=============================================================================
BOOL CIndicatorSensor::Send_PortCheck(__out CStringA& strData)
{
	TRACE(_T("CIndicatorSensor::Send_PortCheck \n"));
	if (!IsOpen())
		return FALSE;

	if (WAIT_OBJECT_0 != LockSemaphore()) //-----------------------------------
		return FALSE;

	BOOL		bReturn = FALSE;
	CStringA	Protocol;

	m_stSendProtocol.Command = IndicatorSensor::Cmd_Real_Data;

	if (TransferStart((char*)&m_stSendProtocol, IndicatorSensor::SEN_Protocol_Length))
	{
		if (WaitACK(m_dwAckWaitTime))
		{
			for (UINT nIdx = 0; nIdx < 2; nIdx++)
				Protocol.AppendChar(m_stRecvProtocol.TYPE[nIdx]);

			strData = Protocol;
			m_dwErrCode = NO_ERROR;
			bReturn = TRUE;
			TRACE(_T("Send_PortCheck ACK OK\n"));
		}
		else
		{
			m_dwErrCode = ERR_SERIAL_ACK;
			bReturn = FALSE;
			TRACE(_T("Send_PortCheck ACK ERROR\n"));
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

//=============================================================================
// Method		: Send_ReadIndicatorData
// Access		: public  
// Returns		: BOOL
// Parameter	: __in BYTE & byCommand
// Qualifier	:
// Last Update	: 2017/1/2 - 18:04
// Desc.		:
//=============================================================================
BOOL CIndicatorSensor::Send_ReadIndicatorData(__in BYTE byCommand, __out float& fValue)
{
	TRACE(_T("CIndicatorSensor::Send_ReadIndicatorData \n"));
	if (!IsOpen())
		return FALSE;

	if (WAIT_OBJECT_0 != LockSemaphore()) //-----------------------------------
		return FALSE;

	BOOL		bReturn = FALSE;
	CStringA	Protocol;
	CString		strTempData;

	m_stSendProtocol.Command = (char)byCommand;

	if (TransferStart((char*)&m_stSendProtocol, IndicatorSensor::SEN_Protocol_Length))
	{
		if (WaitACK(m_dwAckWaitTime))
		{
			for (UINT nIdx = 0; nIdx < 8; nIdx++)
				Protocol.AppendChar(m_stRecvProtocol.Data[nIdx]);
			
			fValue = (float)atof(Protocol);

			m_dwErrCode = NO_ERROR;
			bReturn = TRUE;
			TRACE(_T("Send_ReadIndicatorData ACK OK\n"));
		}
		else
		{
			m_dwErrCode = ERR_SERIAL_ACK;
			bReturn = FALSE;
			TRACE(_T("Send_ReadIndicatorData ACK ERROR\n"));
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
