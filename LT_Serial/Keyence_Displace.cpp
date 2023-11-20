//*****************************************************************************
// Filename	: 	Keyence_Displace.cpp
// Created	:	2016/5/9 - 23:33
// Modified	:	2016/5/9 - 23:33
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
#include "StdAfx.h"
#include "Keyence_Displace.h"
#include "CommonFunction.h"
#include "ErrorCode.h"

using namespace PR_ErrorCode;

static const char* g_szReadBarcode = "LON"; // LOFF
//16022H122116\r\n -> 바코드

//=============================================================================
// 생성자
//=============================================================================
CKeyence_Displace::CKeyence_Displace()
{
	SetDelimeter(Keyence_STX, Keyence_ETX, 1, Keyence_ProtoLength);
}

//=============================================================================
// 소멸자
//=============================================================================
CKeyence_Displace::~CKeyence_Displace()
{
	TRACE(_T("<<< Start ~CKeyence_Displace >>> \n"));

	

	TRACE(_T("<<< End ~CKeyence_Displace >>> \n"));
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
void CKeyence_Displace::OnEvent(EEvent eEvent, EError eError)
{
	if (eEvent & CSerial::EEventRecv)
	{
		// Create a clean buffer
		ZeroMemory(m_szReadBuf, MAX_SERIAL_BUFFER);
		m_dwReadedBufSize = 0;

		const int nBufLen = sizeof(m_szReadBuf) - 1;

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
LRESULT CKeyence_Displace::OnFilterRecvData(const char* szACK, DWORD dwAckSize)
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
			//if (CMD_OverCurrent == m_stRecvProtocol.CMD)
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
// Last Update	: 2017/10/30 - 16:34
// Desc.		:
//=============================================================================
void CKeyence_Displace::OnRecvProtocol(const char* szACK, DWORD dwAckSize)
{
	m_stRecvProtocol.SetRecvProtocol(szACK, dwAckSize);
}


LRESULT CKeyence_Displace::Send_OutputData(__out double& dOutData)
{
	TRACE(_T("CKeyence_Displace::Send_Cal_Result_Can \n"));

	if (!IsOpen())
		return SRC_Err_NotOpen;

	if (WAIT_OBJECT_0 != LockSemaphore()) //-----------------------------------
		return SRC_Err_Semaphore;

	LRESULT	bReturn = SRC_OK;

	BYTE Data[5] = {0x4d, 0x30, 0x2c, 0x30, 0x0d};

	if (TransferStart((const char *)Data, 5))
	{
		if (WaitACK(m_dwAckWaitTime))
		{
			dOutData = m_stRecvProtocol.dVal;

			bReturn = SRC_OK;
		}
		else
		{
			bReturn = SRC_Err_Ack_Timeout;
			TRACE(_T("CMD : CKeyence_Displace::Send_VersionCheck ACK Timeout ERROR\n"));
		}
	}
	else
	{
		bReturn = SRC_Err_Transfer;
	}

	UnlockSemaphore(); //------------------------------------------------------

	return bReturn;
}
