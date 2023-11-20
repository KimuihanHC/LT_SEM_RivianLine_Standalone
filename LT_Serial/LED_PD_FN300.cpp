#include "stdafx.h"
#include "LED_PD_FN300.h"

CLED_PD_FN300* CLED_PD_FN300::GetInstance()
{
	static CLED_PD_FN300 g_FN300;

	return &g_FN300;
}

CLED_PD_FN300::CLED_PD_FN300()
{
	SetDelimeter(LED_ACK_STX, LED_ACK_ETX, LedPD_FN300_ProtoLength, LedPD_FN300_ProtoLength);
}

CLED_PD_FN300::~CLED_PD_FN300()
{

}

LRESULT CLED_PD_FN300::OnFilterRecvData(const char* szACK, DWORD dwAckSize)
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
		SetEvent(m_hEvent_ACK);
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
			//m_stBnCtrl_Read = m_stRecvProtocol.Button;
			::SendNotifyMessage(m_hOwnerWnd, m_WM_Ack, (WPARAM)m_szACKBuf, (LPARAM)m_dwACKBufSize);
		}
	}

	return lReturn;
}

void CLED_PD_FN300::OnRecvProtocol(const char* szACK, DWORD dwAckSize)
{
	m_stRecvProtocol.SetRecvProtocol(szACK, dwAckSize);
}

void CLED_PD_FN300::ResetProtocol()
{

}

BOOL CLED_PD_FN300::SendManuallyData(const char* lpBuffer, DWORD dwCount)
{
	BOOL  bRet = FALSE;
	// UDP
	CString sz;

	char szTemp[MAX_PATH] = { 0, };

	memcpy(&szTemp, lpBuffer, sizeof(char) + dwCount);

	szTemp[dwCount] = 0x0a;

	sz = _T("[send]");
	for (auto i = 0; i < dwCount + 1; i++)
		sz += szTemp[i];

	TRACE(sz + _T("\r\n"));

	/*sz.Trim();
	m_Log.AddLog(sz);*/

	TransferStart(szTemp, dwCount + 1);

	return bRet;
}


LRESULT CLED_PD_FN300::Send_SetChannelData(__in int ch, __in int data)
{
	if (!IsOpen())
	{
//		m_Logger.AddLog(_T("Open Fail : %d"), 2);
		return SRC_Err_NotOpen;
	}

	if (WAIT_OBJECT_0 != LockSemaphore()) //-----------------------------------
		return SRC_Err_Semaphore;

	LRESULT	bReturn = SRC_OK;

	m_stProtocol.MakeProtocol_SetChannelData(ch, data);

	if (TransferStart((char*)&m_stProtocol, LedPD_FN300_ProtoLength))
	{
		if (WaitACK(m_dwAckWaitTime))
		{
			bReturn = SRC_OK;
		}
		else
		{
			bReturn = SRC_Err_Ack_Timeout;
		}
	}
	else
	{
		bReturn = SRC_Err_Transfer;
	}

	UnlockSemaphore(); //------------------------------------------------------
	return bReturn;
}

LRESULT CLED_PD_FN300::Send_SetChannelOnOff(__in int ch, __in BOOL bOn)
{
	if (!IsOpen())
		return SRC_Err_NotOpen;

	if (WAIT_OBJECT_0 != LockSemaphore()) //-----------------------------------
		return SRC_Err_Semaphore;

	LRESULT	bReturn = SRC_OK;

	m_stProtocol.MakeProtocol_SetChannelOnOff(ch, bOn);

	if (TransferStart((char*)&m_stProtocol, LedPD_FN300_ProtoLength))
	{
		if (WaitACK(m_dwAckWaitTime))
		{
			bReturn = SRC_OK;
		}
		else
		{
			bReturn = SRC_Err_Ack_Timeout;
		}
	}
	else
	{
		bReturn = SRC_Err_Transfer;
	}
	UnlockSemaphore(); //------------------------------------------------------
	return bReturn;
}

LRESULT CLED_PD_FN300::Send_GetChannelData(__in int ch, __out int& data)
{
	if (!IsOpen())
		return SRC_Err_NotOpen;

	if (WAIT_OBJECT_0 != LockSemaphore()) //-----------------------------------
		return SRC_Err_Semaphore;

	LRESULT	bReturn = SRC_OK;

	m_stProtocol.MakeProtocol_GetChannelData(ch);

	if (TransferStart((char*)&m_stProtocol, LedPD_FN300_ProtoLength))
	{
		if (WaitACK(m_dwAckWaitTime))
		{
			CString sz(m_stRecvProtocol.Data);
			data = _wcstoui64(sz, NULL, 16);
			bReturn = SRC_OK;
		}
		else
		{
			bReturn = SRC_Err_Ack_Timeout;
		}
	}
	else
	{
		bReturn = SRC_Err_Transfer;
	}

	UnlockSemaphore(); //------------------------------------------------------
	return bReturn;
}

LRESULT CLED_PD_FN300::Send_GetChannelOnOff(__in int ch, __out BOOL& bOn)
{
	if (!IsOpen())
		return SRC_Err_NotOpen;

	if (WAIT_OBJECT_0 != LockSemaphore()) //-----------------------------------
		return SRC_Err_Semaphore;

	LRESULT	bReturn = SRC_OK;

	m_stProtocol.MakeProtocol_GetChannelOnOff(ch);

	if (TransferStart((char*)&m_stProtocol, LedPD_FN300_ProtoLength))
	{
		if (WaitACK(m_dwAckWaitTime))
		{
			if (m_stRecvProtocol.Data == "ON")
				bOn = TRUE;
			else
				bOn = FALSE;

			bReturn = SRC_OK;
		}
		else
		{
			bReturn = SRC_Err_Ack_Timeout;
		}
	}
	else
	{
		bReturn = SRC_Err_Transfer;
	}

	UnlockSemaphore(); //------------------------------------------------------
	return bReturn;
}

LRESULT CLED_PD_FN300::Send_GetChannelCheck(__in int ch, __out BOOL& bError)
{
	if (!IsOpen())
		return SRC_Err_NotOpen;

	if (WAIT_OBJECT_0 != LockSemaphore()) //-----------------------------------
		return SRC_Err_Semaphore;

	LRESULT	bReturn = SRC_OK;

	m_stProtocol.MakeProtocol_GetChannelCheck(ch);

	if (TransferStart((char*)&m_stProtocol, LedPD_FN300_ProtoLength))
	{
		if (WaitACK(m_dwAckWaitTime))
		{
			if (m_stRecvProtocol.Data == "ER")
				bError = TRUE;
			else
				bError = FALSE;

			bReturn = SRC_OK;
		}
		else
		{
			bReturn = SRC_Err_Ack_Timeout;
		}
	}
	else
	{
		bReturn = SRC_Err_Transfer;
	}


	UnlockSemaphore(); //------------------------------------------------------
	return bReturn;
}
