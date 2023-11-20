//*****************************************************************************
// Filename	: 	BCRCtrl.cpp
// Created	:	2016/5/9 - 23:33
// Modified	:	2016/5/9 - 23:33
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
#include "StdAfx.h"
#include "BCRCtrl.h"
#include "CommonFunction.h"
#include "ErrorCode.h"

using namespace PR_ErrorCode;

static const char* g_szReadBarcode = "+";
//16022H122116\r\n -> 바코드

//=============================================================================
// 생성자
//=============================================================================
CBCRCtrl::CBCRCtrl()
{
	SetDelimeter(NULL, 0x0D, 8, 24);
}

//=============================================================================
// 소멸자
//=============================================================================
CBCRCtrl::~CBCRCtrl()
{
	TRACE(_T("<<< Start ~CBCRCtrl >>> \n"));

	

	TRACE(_T("<<< End ~CBCRCtrl >>> \n"));
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
void CBCRCtrl::OnEvent(EEvent eEvent, EError eError)
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
LRESULT CBCRCtrl::OnFilterRecvData(const char* szACK, DWORD dwAckSize)
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
// Method		: Send_ReadBarcode
// Access		: public  
// Returns		: BOOL
// Parameter	: __out CString & szOutBarcode
// Qualifier	:
// Last Update	: 2016/5/10 - 13:16
// Desc.		:
//=============================================================================
BOOL CBCRCtrl::Send_ReadBarcode(__out CString& szOutBarcode)
{
	TRACE(_T("CBCRCtrl::Send_ReadBarcode \n"));
	if (!IsOpen())
		return FALSE;

	if (WAIT_OBJECT_0 != LockSemaphore()) //-----------------------------------
		return FALSE;

	BOOL	bReturn = FALSE;
	if (TransferStart(g_szReadBarcode, (UINT)strlen(g_szReadBarcode)))
	{
		if (WaitACK(m_dwAckWaitTime))
		{
			// Barcode
			szOutBarcode = m_szACKBuf;
			szOutBarcode.Remove('\r');
			szOutBarcode.Remove('\n');

			m_dwErrCode = NO_ERROR;
			bReturn = TRUE;
			TRACE(_T("Read Barcode ACK OK\n"));
		}
		else
		{
			m_dwErrCode = ERR_SERIAL_ACK;
			bReturn = FALSE;
			TRACE(_T("Read Barcode ACK ERROR\n"));
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
