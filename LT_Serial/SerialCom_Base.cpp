//*****************************************************************************
// Filename	: 	SerialCom_Base.cpp
// Created	:	2016/5/9 - 22:47
// Modified	:	2016/5/9 - 22:47
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
#include "StdAfx.h"
#include "SerialCom_Base.h"
#include "CommonFunction.h"
#include <process.h>

using namespace PR_ErrorCode;

#define		RECON_WAIT_TIME		100

//=============================================================================
// 생성자
//=============================================================================
CSerialCom_Base::CSerialCom_Base()
{
	m_hOwnerWnd				= NULL;

	ZeroMemory(m_szReadBuf, MAX_SERIAL_BUFFER);
	m_dwReadedBufSize		= 0;
	ZeroMemory(m_szACKBuf, MAX_SERIAL_BUFFER);
	m_dwACKBufSize			= 0;

	m_dwAckWaitTime			= 2000;	// Milisecond
	m_hEvent_ACK			= NULL;
	m_hEvent_ThreadExit		= NULL;

	CreateAckEvent();

	m_WM_Ack				= NULL;

	m_dwErrCode				= NO_ERROR;

	m_dwSemaphoreWaitTime	= 10;
	m_hSemaphore			= NULL;
	//m_hSemaphore = CreateSemaphore(NULL, 1, 1, _T("SerialCom_Base"));

	m_nMinProtocolLength	= 1;
	m_nMaxProtocolLength	= 11;
	m_chSTX					= 0x21;
	m_chETX					= 0x40;
}

//=============================================================================
// 소멸자
//=============================================================================
CSerialCom_Base::~CSerialCom_Base()
{
	//TRACE(_T("<<< Start ~CSerialCom_Base >>> \n"));

	if (IsOpen())
		Disconnect();

	CloseAckEvent();

	if (NULL != m_hSemaphore)
		CloseHandle(m_hSemaphore);

	//TRACE(_T("<<< End ~CSerialCom_Base >>> \n"));
}

//=============================================================================
// Method		: OnEvent
// Access		: virtual protected  
// Returns		: void
// Parameter	: EEvent eEvent
// Parameter	: EError eError
// Qualifier	:
// Last Update	: 2016/5/9 - 22:47
// Desc.		:
//=============================================================================
void CSerialCom_Base::OnEvent(EEvent eEvent, EError eError)
{
	//if (eError)
	//	DisplayEvent(_T("An internal error occurred."));

	//if (eEvent & CSerial::EEventBreak)
	//	DisplayEvent(_T("Break detected on input."));

	//if (eEvent & CSerial::EEventError)
	//	DisplayEvent(_T("A line-status error occurred."));

	//if (eEvent & CSerial::EEventRcvEv)
	//	DisplayEvent(_T("Event character has been received."));

	//if (eEvent & CSerial::EEventRing)
	//	DisplayEvent(_T("Ring detected"));

	//if (eEvent & CSerial::EEventSend)
	//	DisplayEvent(_T("All data is send"));

	//if (eEvent & CSerial::EEventCTS)
	//	DisplayEventSetting(_T("CTS signal change"), _T("CTS"), GetCTS());

	//if (eEvent & CSerial::EEventDSR)
	//	DisplayEventSetting(_T("DSR signal change"), _T("DSR"), GetDSR());

	//if (eEvent & CSerial::EEventRLSD)
	//	DisplayEventSetting(_T("RLSD signal change"), _T("RLSD"), GetRLSD());

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

			TRACE(_T("Read Data : %d\n"), m_dwReadedBufSize);
		} while (m_dwReadedBufSize == nBufLen);


		TRACE(_T("Recv Data[%d] : %s\n"), m_dwReadedBufSize, HexToCString(m_szReadBuf, m_dwReadedBufSize).GetBuffer(0));

		if (0 < m_dwReadedBufSize)
			OnFilterRecvData(m_szReadBuf, m_dwReadedBufSize);
	}
}

//=============================================================================
// Method		: CSerialCom_Base::OnFilterRecvData
// Access		: protected 
// Returns		: LRESULT
//					 1	-> ACK 처리 성공
//					 0	-> 데이터가 중간에 짤렸음 다음 데이터 수신시 처리
//					-1	-> 쓰레기 데이터 처리
//					-2	-> 매개변수 오류
// Parameter	: const char * szACK	-> 수신된 ACK 데이터
// Parameter	: DWORD dwAckSize		-> 수신된 ACK 데이터 크기
// Qualifier	:
// Last Update	: 2016/5/9 - 22:47
// Desc.		: 
//=============================================================================
LRESULT CSerialCom_Base::OnFilterRecvData(const char* szACK, DWORD dwAckSize)
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

	INT_PTR		iFindSTXIndex = 0;
	INT_PTR		iFindETXIndex = 0;

	//ResetAckBuffer();
	m_dwACKBufSize = 0;

	//-----------------------------------------------------
	// Data가 들어오면 우선	큐에 집어 넣는다?
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
			// ETX 위치가 프로토콜 마지막
// 			if (iFindETXIndex < (INT_PTR)(m_nMaxProtocolLength - 1))
// 			{
// 				++iFindETXIndex; // ETX 찾은 위치 다음부터 찾는다.(무한 루프 방지)
// 				continue;
// 			}
// 			else
//			{
				dwDataLength = (DWORD)iFindETXIndex + 1;
				m_SerQueue.PopData(m_szACKBuf, (DWORD)iFindETXIndex + 1);
				break;
//			}
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

	//-----------------------------------------------------
	// 큐 비우기
	//-----------------------------------------------------
	m_SerQueue.Empty();

	// 이벤트 핸들이 설정되어있다면 이벤트를 발생시킨다.
	if (NULL != m_hEvent_ACK)
		SetEvent(m_hEvent_ACK);

	//TRACE(_T("FilterAckData : Complete Ack Protocol\n"));
// 	if (NULL != m_hOwnerWnd)
// 	{
// 		::SendNotifyMessage(m_hOwnerWnd, m_WM_Ack, (WPARAM)m_szACKBuf, (LPARAM)m_dwACKBufSize);
// 	}

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
// Last Update	: 2016/5/18 - 18:07
// Desc.		:
//=============================================================================
void CSerialCom_Base::OnRecvProtocol(const char* szACK, DWORD dwAckSize)
{
	
}

//=============================================================================
// Method		: CSerialCom_Base::LockSemaphore
// Access		: protected 
// Returns		: DWORD
// Qualifier	:
// Last Update	: 2015/11/28 - 16:35
// Desc.		: 
//=============================================================================
DWORD CSerialCom_Base::LockSemaphore()
{
	DWORD	dwResult = WaitForSingleObject(m_hSemaphore, m_dwSemaphoreWaitTime);
	if (WAIT_OBJECT_0 != dwResult)
	{
		TRACE(_T("------------** [CSerialCom_Base] Error : Semaphore is Alive **------------\n"));
		m_dwErrCode = ERR_SEMAPHORE;
	}
	else
	{
		//TRACE(_T("------------** [CSerialCom_Base] Enter Semaphore **------------\n"));
	}

	return dwResult;
}

//=============================================================================
// Method		: CSerialCom_Base::UnlockSemaphore
// Access		: protected 
// Returns		: BOOL
// Qualifier	:
// Last Update	: 2015/11/28 - 16:35
// Desc.		: 
//=============================================================================
BOOL CSerialCom_Base::UnlockSemaphore()
{
	BOOL bResult = ReleaseSemaphore(m_hSemaphore, 1, NULL);

	if (!bResult)
	{
		TRACE(_T("------------** [CSerialCom_Base] Error : ReleaseSemaphore() **------------\n"));
	}
	//TRACE(_T("------------** [CSerialCom_Base] ReleaseSemaphore() **------------\n"));

	return bResult;
}

//=============================================================================
// Method		: CSerialCom_Base::WaitSemaphore
// Access		: protected 
// Returns		: BOOL
// Parameter	: DWORD dwTimeout
// Qualifier	:
// Last Update	: 2015/11/28 - 16:35
// Desc.		: 
//=============================================================================
BOOL CSerialCom_Base::WaitSemaphore(DWORD dwTimeout)
{
	BOOL bResult = FALSE;
	DWORD	dwResult = WaitForSingleObject(m_hSemaphore, dwTimeout);

	switch (dwResult)
	{
	case WAIT_OBJECT_0:
		bResult = ReleaseSemaphore(m_hSemaphore, 1, NULL);
		break;

	case WAIT_TIMEOUT:
		bResult = FALSE;
		break;

	case WAIT_ABANDONED:
		bResult = FALSE;
		break;
	}

	return bResult;
}

//=============================================================================
// Method		: CSerialCom_Base::CreatePGEvent
// Access		: protected 
// Returns		: BOOL
// Qualifier	:
// Last Update	: 2015/11/28 - 16:35
// Desc.		: 
//=============================================================================
BOOL CSerialCom_Base::CreateAckEvent()
{
	m_hEvent_ACK = CreateEvent(NULL, FALSE, FALSE, NULL);

	return TRUE;
}

//=============================================================================
// Method		: CSerialCom_Base::ClosePGEvent
// Access		: protected 
// Returns		: BOOL
// Qualifier	:
// Last Update	: 2015/11/28 - 16:35
// Desc.		: 
//=============================================================================
BOOL CSerialCom_Base::CloseAckEvent()
{
	if (FALSE == CloseHandle(m_hEvent_ACK))
	{
		CString strText;
		strText.Format(_T("Close Event error: %d\n"), GetLastError());
		m_Log.LogMsg_Err(strText);
		return FALSE;
	}

	return TRUE;
}

//=============================================================================
// Method		: CSerialCom_Base::WaitACK
// Access		: protected 
// Returns		: BOOL
// Parameter	: UINT nCmd				-> 통신제어 커맨드
// Parameter	: DWORD dwWaitTime		-> ACK 대기 시간
// Qualifier	:
// Last Update	: 2015/11/28 - 16:35
// Desc.		: 
//=============================================================================
BOOL CSerialCom_Base::WaitACK(DWORD dwWaitTime)
{
	BOOL	bRet = FALSE;
	DWORD	dwBufSize = 0;
	BOOL	fDone = TRUE;
	DWORD	dwStartTick = GetTickCount();
	DWORD	dwChkTick = 0;
	DWORD	dwResult = 0;
	DWORD	dwCurrentTick = 0;

	HANDLE  hArray[2];
	hArray[0] = m_hEvent_ACK;			// Ack 이벤트가
	hArray[1] = m_hEvent_ThreadExit;	// 쓰레드 종료 이벤트

	while (fDone)
	{
		dwCurrentTick = GetTickCount();
		if (dwStartTick <= dwCurrentTick)
		{
			dwChkTick = dwWaitTime - (dwCurrentTick - dwStartTick);
		}
		else
		{
			dwChkTick = dwWaitTime - (0xFFFFFFFF - dwStartTick + dwCurrentTick);
		}

		dwResult = MsgWaitForMultipleObjects(2, hArray, FALSE, dwChkTick, QS_ALLINPUT);
		switch (dwResult)
		{
		case WAIT_OBJECT_0:		// 포트에 데이터 수신됨
			ResetEvent(m_hEvent_ACK);

			// 필러링된 데이터 사용
			if (0 < m_dwACKBufSize)
			{
				fDone = FALSE;
				bRet = TRUE;
			}
			else
			{
				continue;
			}

			break;

		case WAIT_OBJECT_0 + 1:	// 프로그램 종료 이벤트 처리
			fDone = FALSE;
			break;

		case WAIT_OBJECT_0 + 2:
			//TRACE (_T("WaitACK is Message Event\n"));
			DoEvents();
			break;

		case WAIT_TIMEOUT:
			TRACE(_T("WaitACK is TimeOut\n"));
			fDone = FALSE;

			m_dwErrCode = WAIT_TIMEOUT;
			bRet = FALSE;
			break;
		}

		if (false == IsOpen())
		{
			TRACE(_T("WaitACK is Disconnected\n"));
			fDone = FALSE;
			break;
		}
	}

	return bRet;
}

//=============================================================================
// Method		: CSerialCom_Base::ResetAckBuffer
// Access		: protected 
// Returns		: void
// Qualifier	:
// Last Update	: 2015/11/28 - 16:35
// Desc.		: 
//=============================================================================
void CSerialCom_Base::ResetAckBuffer()
{
	ZeroMemory(m_szACKBuf, MAX_SERIAL_BUFFER);
	m_dwACKBufSize = 0;

	m_SerQueue.Empty();
}

void CSerialCom_Base::SetDelimeter(__in char chSTX, __in char chETX, __in UINT_PTR nMinProtocolLength, __in UINT_PTR nMaxProtocolLength)
{
	m_nMinProtocolLength = nMinProtocolLength;
	m_nMaxProtocolLength = nMaxProtocolLength;
	m_chSTX = chSTX;
	m_chETX = chETX;
}


void CSerialCom_Base::SetProtocolLength(__in UINT_PTR nMinProtocolLength, __in UINT_PTR nMaxProtocolLength)
{
	m_nMinProtocolLength = nMinProtocolLength;
	m_nMaxProtocolLength = nMaxProtocolLength;
}

//=============================================================================
// Method		: SetSemaphore
// Access		: public  
// Returns		: void
// Parameter	: LPCTSTR lpName
// Qualifier	:
// Last Update	: 2015/12/28 - 0:07
// Desc.		:
//=============================================================================
void CSerialCom_Base::SetSemaphore(LPCTSTR lpName)
{
	m_hSemaphore = CreateSemaphore(NULL, 1, 1, lpName);
}

void CSerialCom_Base::ResetSemaphore()
{
	ReleaseSemaphore(m_hSemaphore, 1, NULL);
}

//=============================================================================
// Method		: CSerialCom_Base::SetLogMsgID
// Access		: public 
// Returns		: void
// Parameter	: UINT nWM_ID
// Parameter	: UINT nLogType
// Qualifier	:
// Last Update	: 2015/11/28 - 16:35
// Desc.		:
//=============================================================================
void CSerialCom_Base::SetLogMsgID(UINT nWM_ID, UINT nLogType /*= 0*/)
{
	m_Log.SetOwner(m_hOwnerWnd, nWM_ID);
	m_Log.SetLogType(nLogType);
	//m_WM_ID_LOG = nWM_ID;
	//m_nLogType = nLogType;
}

//=============================================================================
// Method		: CSerialCom_Base::SetAckMsgID
// Access		: public 
// Returns		: void
// Parameter	: UINT nWM_ID
// Qualifier	:
// Last Update	: 2015/11/28 - 16:35
// Desc.		: 
//=============================================================================
void CSerialCom_Base::SetAckMsgID(UINT nWM_ID)
{
	m_WM_Ack = nWM_ID;
}

//=============================================================================
// Method		: CSerialCom_Base::SetAckWaitTime
// Access		: public 
// Returns		: void
// Parameter	: DWORD dwMilisecond
// Qualifier	:
// Last Update	: 2015/11/28 - 16:35
// Desc.		: 
//=============================================================================
void CSerialCom_Base::SetAckWaitTime(DWORD dwMilisecond)
{
	m_dwAckWaitTime = dwMilisecond;
}

//=============================================================================
// Method		: SetupCommPort
// Access		: public  
// Returns		: HANDLE
// Parameter	: LPCWSTR inPort
// Parameter	: DWORD inBaudRate
// Parameter	: BYTE inParity
// Parameter	: BYTE inStopBits
// Parameter	: BYTE inByteSize
// Qualifier	:
// Last Update	: 2015/12/28 - 13:22
// Desc.		:
//=============================================================================
HANDLE CSerialCom_Base::Connect(LPCTSTR inPort, DWORD inBaudRate, BYTE inParity, BYTE inStopBits, BYTE inByteSize)
{
	m_szPort = inPort;

	if (ERROR_SUCCESS != CSerialWnd::Open(m_szPort, m_hOwnerWnd))
	{
		TRACE(_T("CSerialWnd::Open Error!!! \n"));
	}

	Setup((EBaudrate)inBaudRate, (EDataBits)inByteSize, (EParity)inParity, (EStopBits)inStopBits);
	//SetupHandshaking(CSerial::EHandshakeHardware);

	return m_hFile;
}

//=============================================================================
// Method		: BreakDownCommPort
// Access		: public  
// Returns		: BOOL
// Qualifier	:
// Last Update	: 2015/12/28 - 13:22
// Desc.		:
//=============================================================================
BOOL CSerialCom_Base::Disconnect()
{
	if (IsOpen())
	{
		if (ERROR_SUCCESS == Close())
		{
			m_dwErrCode = NO_ERROR;
			return TRUE;
		}
		else
		{
			m_Log.LogMsg_Err(_T("통신 포트 연결해제 실패!!"));
			m_dwErrCode = ERR_COMM_DISCONNECT;
			return FALSE;
		}
	}

	return FALSE;
}

//=============================================================================
// Method		: CSerialCom_Base::ReConnect
// Access		: public 
// Returns		: BOOL
// Qualifier	:
// Last Update	: 2015/11/28 - 16:35
// Desc.		: 
//=============================================================================
BOOL CSerialCom_Base::ReConnect()
{
	if (ERROR_SUCCESS == Close())
	{
		Delay(RECON_WAIT_TIME);

		if (ERROR_SUCCESS != CSerialWnd::Open(m_szPort, m_hOwnerWnd))
		{
			m_dwErrCode = NO_ERROR;
			return TRUE;
		}
		else
		{
			m_Log.LogMsg_Err(_T("연결 실패!!"));
			m_dwErrCode = ERR_COMM_CONNECT;
			return FALSE;
		}
	}
	else
	{
		m_dwErrCode = ERR_COMM_DISCONNECT;
		return FALSE;
	}

	m_dwErrCode = NO_ERROR;
	return TRUE;
}

//=============================================================================
// Method		: CSerialCom_Base::IsConnected
// Access		: public 
// Returns		: BOOL
// Qualifier	:
// Last Update	: 2015/11/28 - 16:35
// Desc.		: 
//=============================================================================
BOOL CSerialCom_Base::IsConnected()
{
	return IsOpen();
}

//=============================================================================
// Method		: CSerialCom_Base::TransferStart
// Access		: public 
// Returns		: BOOL
// Parameter	: const char * lpstrSendBuff
// Parameter	: UINT nBuffSize
// Qualifier	:
// Last Update	: 2015/11/28 - 16:35
// Desc.		: 
//=============================================================================
BOOL CSerialCom_Base::TransferStart(const char* lpstrSendBuff, UINT nBuffSize)
{
	__try
	{
		// 큐를 비운다.
		//m_SerQueue.Empty();

		ResetAckBuffer();

		m_Log.LogMsg_ConvNULL(lpstrSendBuff, nBuffSize);
		if (ERROR_SUCCESS == Write(lpstrSendBuff, nBuffSize))
		{
			m_dwErrCode = NO_ERROR;

			OnDebugMsgFile(lpstrSendBuff, nBuffSize);
			/*for (UINT _a = 0; _a < nBuffSize; _a++)
				TRACE(_T("[%d :%x ] \r\n"), _a, lpstrSendBuff[_a]);*/
			return TRUE;
		}
		else
		{
			TRACE(_T("Write Error\n"));
			m_dwErrCode = ERR_SERIAL_TRANSFER;
			return FALSE;
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		TRACE(_T("*** Exception Error : CSerialCom_Base::TransferStart ()"));
	}

	return TRUE;
}

//=============================================================================
// Method		: CSerialCom_Base::TransferProtocol
// Access		: public 
// Returns		: BOOL
// Parameter	: UINT nPal_Cmd
// Parameter	: const char * lpcBuffer
// Parameter	: UINT wBufferSize
// Qualifier	:
// Last Update	: 2015/11/28 - 16:35
// Desc.		: 프로토콜 전송하고 ACK 처리
//=============================================================================
BOOL CSerialCom_Base::TransferProtocol(const char* lpcBuffer, UINT wBufferSize, BOOL bWaitAck/* = FALSE*/)
{
	if (NULL == lpcBuffer)
	{
		m_Log.LogMsg_Err(_T("lpcBuffer is NULL"));
		m_dwErrCode = ERROR_INVALID_PARAMETER;
		return FALSE;
	}

	if (0 == wBufferSize)
	{
		m_Log.LogMsg_Err(_T("wBufferSize is Zero"));
		m_dwErrCode = ERROR_INVALID_PARAMETER;
		return FALSE;
	}

	//   	if (WAIT_OBJECT_0 != LockSemaphore())
	//   		return FALSE;

	// 널문자를 공백으로 바꾸어 출력
	//SendLogMsgConvNULL (lpcBuffer, wBufferSize);

	BOOL	bReturn = FALSE;
	if (TransferStart(lpcBuffer, wBufferSize))
	{
		// ACK 처리
		if (bWaitAck)
		{
			TRACE(_T("Wait ACK\n"));
			if (WaitACK(m_dwAckWaitTime))
			{
				m_dwErrCode = NO_ERROR;
				TRACE(_T("ACK OK\n"));
				bReturn = TRUE;
			}
			else
			{
				m_dwErrCode = ERR_SERIAL_ACK;
				TRACE(_T("ACK ERROR\n"));
				bReturn = FALSE;
			}
		}
		else
			bReturn = TRUE;
	}
	else
	{
		m_dwErrCode = ERR_SERIAL_TRANSFER;
		bReturn = FALSE;
	}

	//	UnlockSemaphore();

	return bReturn;
}

//=============================================================================
// Method		: CSerialCom_Base::GetAckData
// Access		: public 
// Returns		: DWORD_PTR
// Parameter	: char * lpcAck
// Qualifier	:
// Last Update	: 2015/11/28 - 16:35
// Desc.		: 필터링된 Ack 데이터를 반환
//=============================================================================
DWORD_PTR CSerialCom_Base::GetAckData(char* lpcAck)
{
	if (NULL != lpcAck)
	{
		memcpy(lpcAck, m_szACKBuf, m_dwACKBufSize);
		return m_dwACKBufSize;
	}

	return 0;
}

//=============================================================================
// Method		: WaitRecvAck
// Access		: public  
// Returns		: BOOL
// Qualifier	:
// Last Update	: 2016/2/3 - 14:37
// Desc.		:
//=============================================================================
BOOL CSerialCom_Base::WaitRecvAck()
{
	if (WaitACK(m_dwAckWaitTime))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


//************************************
// Method:    OnDebugMsgFile
// Date:	  2017/11/21(16:23:57)
// FullName:  CSerialCom_Base::OnDebugMsgFile
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: __in LPCSTR szMsg
// Parameter: __in DWORD nBuffSize
// Parameter: __in BOOL bSend
//************************************
void CSerialCom_Base::OnDebugMsgFile(__in LPCSTR szMsg, __in DWORD nBuffSize, __in BOOL bSend)
{
	CString sz;
	CString szTemp;
	CString szBuf;

	if (bSend)
		sz.Format(_T("Send Length[%d]"), nBuffSize);
	else
		sz.Format(_T("Ack Length[%d]"), nBuffSize);
	
	for (UINT _a = 0; _a < nBuffSize; _a++)
	{
		szTemp.Format(_T("%x "), szMsg[_a]);
		szBuf += szTemp;
	}

	CString szLog;

	for (UINT iIndex = 0; iIndex < nBuffSize; iIndex++)
	{
		if (NULL == szMsg[iIndex])
		{
			szLog.AppendChar(_T(' '));
			//lpszLogString[iIndex] = ' ';
		}
		else
		{
			szLog.AppendChar(szMsg[iIndex]);
			//lpszLogString[iIndex] = lpszLog[iIndex];
		}
	}
	szBuf += _T("\r\n");

	sz += szBuf;
	NoatepadDebug(sz);

	szLog += _T("\r\n");
	NoatepadDebug(szLog);
}
