//*****************************************************************************
// Filename	: 	ODA_PT.cpp
// Created	:	2016/05/09
// Modified	:	2016/05/09
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************

#include "StdAfx.h"
#include "ODA_PT.h"
#include "ErrorCode.h"

using namespace PR_ErrorCode;

//=============================================================================
// 생성자
//=============================================================================
CODA_PT::CODA_PT()
{
	//SetDelimeter(LightBrd_STX, LightBrd_ETX, LightBrd_ProtoLength, LightBrd_ProtoLength);
	SetDelimeter(NULL, ODA_PT_ETX, 1, ODA_PT_MaxLength);

	m_fMinCurrent	= 0.0f;
	m_fMaxCurrent	= 3000.0f;
	m_fUnitCurrent	= (m_fMaxCurrent - m_fMinCurrent) / 1024.0f;
}

//=============================================================================
// 소멸자
//=============================================================================
CODA_PT::~CODA_PT()
{
	//TRACE(_T("<<< Start ~CODA_PT >>> \n"));

	

	//TRACE(_T("<<< End ~CODA_PT >>> \n"));
}


//=============================================================================
// Method		: CODA_PT::OnFilterRecvData
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
LRESULT CODA_PT::OnFilterRecvData(const char* szACK, DWORD dwAckSize)
{
	//return CSerialCom_Base::OnFilterRecvData(szACK, dwAckSize);

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
// 	if (NULL != m_hOwnerWnd)
// 	{
// 		::SendNotifyMessage(m_hOwnerWnd, m_WM_Ack, (WPARAM)m_szACKBuf, (LPARAM)m_dwACKBufSize);
// 	}

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
// Last Update	: 2016/5/9 - 23:28
// Desc.		:
//=============================================================================
void CODA_PT::OnRecvProtocol(const char* szACK, DWORD dwAckSize)
{
	m_stRecvProtocol.SetRecvProtocol(szACK, dwAckSize);
}

//=============================================================================
// Method		: ResetProtocol
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2017/10/7 - 18:17
// Desc.		:
//=============================================================================
void CODA_PT::ResetProtocol()
{

}

//=============================================================================
// Method		: SetMinMaxVoltage
// Access		: public  
// Returns		: void
// Parameter	: __in FLOAT fMinVolt
// Parameter	: __in FLOAT fMaxVolt
// Qualifier	:
// Last Update	: 2016/10/11 - 20:50
// Desc.		:
//=============================================================================
void CODA_PT::SetMinMaxVoltage(__in FLOAT fMinVolt, __in FLOAT fMaxVolt)
{
	m_fMinCurrent	= fMinVolt;
	m_fMaxCurrent	= fMaxVolt;

	m_fUnitCurrent	= (m_fMaxCurrent - m_fMinCurrent) / 1024.0f;
}

//=============================================================================
// Method		: Send_PortCheck
// Access		: public  
// Returns		: LRESULT
// Parameter	: __out BYTE & byPortNo
// Qualifier	:
// Last Update	: 2016/5/9 - 23:28
// Desc.		:
//=============================================================================
LRESULT CODA_PT::Send_PortCheck()
{
	CString szVersion;

	return Send_Version_Status(szVersion);
}

//=============================================================================
// Method		: Send_Apply
// Access		: public  
// Returns		: LRESULT
// Parameter	: __in float fVoltage
// Parameter	: __in float fCurrent
// Qualifier	:
// Last Update	: 2017/11/7 - 21:28
// Desc.		:
//=============================================================================
LRESULT CODA_PT::Send_Apply(__in float fVoltage, __in float fCurrent)
{
	TRACE(_T("CODA_PT::Send_Apply \n"));

	if (!IsOpen())
		return SRC_Err_NotOpen;

	if (WAIT_OBJECT_0 != LockSemaphore()) //-----------------------------------
		return SRC_Err_Semaphore;

	LRESULT	bReturn = SRC_OK;

	m_stProtocol.MakeProtocol_Apply(fVoltage, fCurrent);

	if (TransferStart(m_stProtocol.Protocol.GetBuffer(0), m_stProtocol.Protocol.GetLength()))
	{
		bReturn = SRC_OK;
		TRACE(_T("CMD : CODA_PT::Send_Apply Transfer OK\n"));
	}
	else
	{
		bReturn = SRC_Err_Transfer;
	}

	UnlockSemaphore(); //------------------------------------------------------

	return bReturn;
}

LRESULT CODA_PT::Send_Apply(__in float fVoltage)
{
	TRACE(_T("CODA_PT::Send_Apply \n"));

	if (!IsOpen())
		return SRC_Err_NotOpen;

	if (WAIT_OBJECT_0 != LockSemaphore()) //-----------------------------------
		return SRC_Err_Semaphore;

	LRESULT	bReturn = SRC_OK;

	m_stProtocol.MakeProtocol_Apply(fVoltage);

	if (TransferStart(m_stProtocol.Protocol.GetBuffer(0), m_stProtocol.Protocol.GetLength()))
	{
		bReturn = SRC_OK;
		TRACE(_T("CMD : CODA_PT::Send_Apply Transfer OK\n"));
	}
	else
	{
		bReturn = SRC_Err_Transfer;
	}

	UnlockSemaphore(); //------------------------------------------------------

	return bReturn;
}

//=============================================================================
// Method		: Send_Apply_Status
// Access		: public  
// Returns		: LRESULT
// Parameter	: __out float & fOutVoltage
// Parameter	: __out float & fOutCurrent
// Qualifier	:
// Last Update	: 2017/11/7 - 21:34
// Desc.		:
//=============================================================================
LRESULT CODA_PT::Send_Apply_Status(__out float& fOutVoltage, __out float& fOutCurrent)
{
	TRACE(_T("CODA_PT::Send_Apply_Status \n"));

	if (!IsOpen())
		return SRC_Err_NotOpen;

	if (WAIT_OBJECT_0 != LockSemaphore()) //-----------------------------------
		return SRC_Err_Semaphore;

	LRESULT	bReturn = SRC_OK;

	m_stProtocol.MakeProtocol(ODA_PT::CMD_APPLy_Req);
	m_stRecvProtocol.CommandID = ODA_PT::CMD_APPLy_Req;

	if (TransferStart(m_stProtocol.Protocol.GetBuffer(0), m_stProtocol.Protocol.GetLength()))
	{
		if (WaitACK(m_dwAckWaitTime))
		{
			fOutVoltage = m_stRecvProtocol.fVoltage;
			fOutCurrent = m_stRecvProtocol.fCurrent;

			bReturn = SRC_OK;
			TRACE(_T("CMD : CODA_PT::Send_Apply_Status ACK OK\n"));
		}
		else
		{
			bReturn = SRC_Err_Ack_Timeout;
			TRACE(_T("CMD : CODA_PT::Send_Apply_Status ACK Timeout ERROR\n"));
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
// Method		: Send_Voltage
// Access		: public  
// Returns		: LRESULT
// Parameter	: __in float fVoltage
// Qualifier	:
// Last Update	: 2018/2/5 - 20:47
// Desc.		:
//=============================================================================
LRESULT CODA_PT::Send_Voltage(__in float fVoltage)
{
	TRACE(_T("CODA_PT::Send_Voltage \n"));

	if (!IsOpen())
		return SRC_Err_NotOpen;

	if (WAIT_OBJECT_0 != LockSemaphore()) //-----------------------------------
		return SRC_Err_Semaphore;

	LRESULT	bReturn = SRC_OK;

	m_stProtocol.MakeProtocol_Volt(fVoltage);

	if (TransferStart(m_stProtocol.Protocol.GetBuffer(0), m_stProtocol.Protocol.GetLength()))
	{
		bReturn = SRC_OK;
		TRACE(_T("CMD : CODA_PT::Send_Voltage Transfer OK\n"));
	}
	else
	{
		bReturn = SRC_Err_Transfer;
	}

	UnlockSemaphore(); //------------------------------------------------------

	return bReturn;
}

LRESULT CODA_PT::Send_Voltage_Req(__out float& fOutVoltage)
{
	TRACE(_T("CODA_PT::Send_Voltage_Req \n"));

	if (!IsOpen())
		return SRC_Err_NotOpen;

	if (WAIT_OBJECT_0 != LockSemaphore()) //-----------------------------------
		return SRC_Err_Semaphore;

	LRESULT	bReturn = SRC_OK;

	m_stProtocol.MakeProtocol(ODA_PT::CMD_VOLT_Req);
	m_stRecvProtocol.CommandID = ODA_PT::CMD_VOLT_Req;

	if (TransferStart(m_stProtocol.Protocol.GetBuffer(0), m_stProtocol.Protocol.GetLength()))
	{
		if (WaitACK(m_dwAckWaitTime))
		{
			fOutVoltage = m_stRecvProtocol.fVoltage;

			bReturn = SRC_OK;
			TRACE(_T("CMD : CODA_PT::Send_Voltage_Req ACK OK\n"));
		}
		else
		{
			bReturn = SRC_Err_Ack_Timeout;
			TRACE(_T("CMD : CODA_PT::Send_Voltage_Req ACK Timeout ERROR\n"));
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
// Method		: Send_Current
// Access		: public  
// Returns		: LRESULT
// Parameter	: __in float fCurrent
// Qualifier	:
// Last Update	: 2018/2/5 - 20:49
// Desc.		:
//=============================================================================
LRESULT CODA_PT::Send_Current(__in float fCurrent)
{
	TRACE(_T("CODA_PT::Send_Current \n"));

	if (!IsOpen())
		return SRC_Err_NotOpen;

	if (WAIT_OBJECT_0 != LockSemaphore()) //-----------------------------------
		return SRC_Err_Semaphore;

	LRESULT	bReturn = SRC_OK;

	m_stProtocol.MakeProtocol_Curr(fCurrent);

	if (TransferStart(m_stProtocol.Protocol.GetBuffer(0), m_stProtocol.Protocol.GetLength()))
	{
		bReturn = SRC_OK;
		TRACE(_T("CMD : CODA_PT::Send_Current Transfer OK\n"));
	}
	else
	{
		bReturn = SRC_Err_Transfer;
	}

	UnlockSemaphore(); //------------------------------------------------------

	return bReturn;
}

LRESULT CODA_PT::Send_Current_Req(__out float& fOutCurrent)
{
	TRACE(_T("CODA_PT::Send_Current_Req \n"));

	if (!IsOpen())
		return SRC_Err_NotOpen;

	if (WAIT_OBJECT_0 != LockSemaphore()) //-----------------------------------
		return SRC_Err_Semaphore;

	LRESULT	bReturn = SRC_OK;

	m_stProtocol.MakeProtocol(ODA_PT::CMD_CURR_Req);
	m_stRecvProtocol.CommandID = ODA_PT::CMD_CURR_Req;

	if (TransferStart(m_stProtocol.Protocol.GetBuffer(0), m_stProtocol.Protocol.GetLength()))
	{
		if (WaitACK(m_dwAckWaitTime))
		{
			fOutCurrent = m_stRecvProtocol.fCurrent;

			bReturn = SRC_OK;
			TRACE(_T("CMD : CODA_PT::Send_Current_Req ACK OK\n"));
		}
		else
		{
			bReturn = SRC_Err_Ack_Timeout;
			TRACE(_T("CMD : CODA_PT::Send_Current_Req ACK Timeout ERROR\n"));
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
// Method		: Send_Flow_Status
// Access		: public  
// Returns		: LRESULT
// Parameter	: __in enCV_CC nOutCVCC
// Qualifier	:
// Last Update	: 2017/11/7 - 21:36
// Desc.		:
//=============================================================================
LRESULT CODA_PT::Send_Flow_Status(__in enCV_CC nOutCVCC)
{
	TRACE(_T("CODA_PT::Send_Flow_Status \n"));

	if (!IsOpen())
		return SRC_Err_NotOpen;

	if (WAIT_OBJECT_0 != LockSemaphore()) //-----------------------------------
		return SRC_Err_Semaphore;

	LRESULT	bReturn = SRC_OK;

	m_stProtocol.MakeProtocol(ODA_PT::CMD_FLOW_Req);
	m_stRecvProtocol.CommandID = ODA_PT::CMD_FLOW_Req;

	if (TransferStart(m_stProtocol.Protocol.GetBuffer(0), m_stProtocol.Protocol.GetLength()))
	{
		if (WaitACK(m_dwAckWaitTime))
		{
			nOutCVCC = m_stRecvProtocol.nCVCCStatus;

			bReturn = SRC_OK;
			TRACE(_T("CMD : CODA_PT::Send_Flow_Status ACK OK\n"));
		}
		else
		{
			bReturn = SRC_Err_Ack_Timeout;
			TRACE(_T("CMD : CODA_PT::Send_Flow_Status ACK Timeout ERROR\n"));
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
// Method		: Send_Meas_ALL
// Access		: public  
// Returns		: LRESULT
// Parameter	: __out float & fOutVoltage
// Parameter	: __out float & fOutCurrent
// Qualifier	:
// Last Update	: 2017/11/8 - 13:44
// Desc.		:
//=============================================================================
LRESULT CODA_PT::Send_Meas_ALL(__out float& fOutVoltage, __out float& fOutCurrent)
{
	TRACE(_T("CODA_PT::Send_Meas_ALL \n"));

	if (!IsOpen())
		return SRC_Err_NotOpen;

	if (WAIT_OBJECT_0 != LockSemaphore()) //-----------------------------------
		return SRC_Err_Semaphore;

	LRESULT	bReturn = SRC_OK;

	m_stProtocol.MakeProtocol(ODA_PT::CMD_MEAS_ALL_Req);
	m_stRecvProtocol.CommandID = ODA_PT::CMD_MEAS_ALL_Req;

	if (TransferStart(m_stProtocol.Protocol.GetBuffer(0), m_stProtocol.Protocol.GetLength()))
	{
		if (WaitACK(m_dwAckWaitTime))
		{
			fOutVoltage = m_stRecvProtocol.fMeasVoltage;
			fOutCurrent = m_stRecvProtocol.fMeasCurrent;

			bReturn = SRC_OK;
			TRACE(_T("CMD : CODA_PT::Send_Meas_ALL ACK OK\n"));
		}
		else
		{
			bReturn = SRC_Err_Ack_Timeout;
			TRACE(_T("CMD : CODA_PT::Send_Meas_ALL ACK Timeout ERROR\n"));
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
// Method		: Send_Error_Status
// Access		: public  
// Returns		: LRESULT
// Parameter	: __out int & iOutErrCode
// Parameter	: __out CString & szOutErrorDesc
// Qualifier	:
// Last Update	: 2017/11/7 - 21:41
// Desc.		:
//=============================================================================
LRESULT CODA_PT::Send_Error_Status(__out int& iOutErrCode, __out CString& szOutErrorDesc)
{
	TRACE(_T("CODA_PT::Send_Error_Status \n"));

	if (!IsOpen())
		return SRC_Err_NotOpen;

	if (WAIT_OBJECT_0 != LockSemaphore()) //-----------------------------------
		return SRC_Err_Semaphore;

	LRESULT	bReturn = SRC_OK;

	m_stProtocol.MakeProtocol(ODA_PT::CMD_SYST_ERR_Req);
	m_stRecvProtocol.CommandID = ODA_PT::CMD_SYST_ERR_Req;

	if (TransferStart(m_stProtocol.Protocol.GetBuffer(0), m_stProtocol.Protocol.GetLength()))
	{
		if (WaitACK(m_dwAckWaitTime))
		{
			iOutErrCode		= m_stRecvProtocol.iErrorCode;
			szOutErrorDesc	= m_stRecvProtocol.ErrorDesc;

			bReturn = SRC_OK;
			TRACE(_T("CMD : CODA_PT::Send_Error_Status ACK OK\n"));
		}
		else
		{
			bReturn = SRC_Err_Ack_Timeout;
			TRACE(_T("CMD : CODA_PT::Send_Error_Status ACK Timeout ERROR\n"));
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
// Method		: Send_Version_Status
// Access		: public  
// Returns		: LRESULT
// Parameter	: __out CString & szOutVersion
// Qualifier	:
// Last Update	: 2017/11/7 - 21:42
// Desc.		:
//=============================================================================
LRESULT CODA_PT::Send_Version_Status(__out CString& szOutVersion)
{
	TRACE(_T("CODA_PT::Send_Version_Status \n"));

	if (!IsOpen())
		return SRC_Err_NotOpen;

	if (WAIT_OBJECT_0 != LockSemaphore()) //-----------------------------------
		return SRC_Err_Semaphore;

	LRESULT	bReturn = SRC_OK;

	m_stProtocol.MakeProtocol(ODA_PT::CMD_SYST_VERS_Req);
	m_stRecvProtocol.CommandID = ODA_PT::CMD_SYST_VERS_Req;

	if (TransferStart(m_stProtocol.Protocol.GetBuffer(0), m_stProtocol.Protocol.GetLength()))
	{
		if (WaitACK(m_dwAckWaitTime))
		{
			szOutVersion = m_stRecvProtocol.Version;

			bReturn = SRC_OK;
			TRACE(_T("CMD : CODA_PT::Send_Version_Status ACK OK\n"));
		}
		else
		{
			bReturn = SRC_Err_Ack_Timeout;
			TRACE(_T("CMD : CODA_PT::Send_Version_Status ACK Timeout ERROR\n"));
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
// Method		: Send_Output
// Access		: public  
// Returns		: LRESULT
// Parameter	: __in enSwitchOnOff nOutputOnOff
// Qualifier	:
// Last Update	: 2017/11/7 - 21:36
// Desc.		:
//=============================================================================
LRESULT CODA_PT::Send_Output(__in enSwitchOnOff nOutputOnOff)
{
	TRACE(_T("CODA_PT::Send_Output \n"));

	if (!IsOpen())
		return SRC_Err_NotOpen;

	if (WAIT_OBJECT_0 != LockSemaphore()) //-----------------------------------
		return SRC_Err_Semaphore;

	LRESULT	bReturn = SRC_OK;

	m_stProtocol.MakeProtocol_Output(nOutputOnOff);

	if (TransferStart(m_stProtocol.Protocol.GetBuffer(0), m_stProtocol.Protocol.GetLength()))
	{
		bReturn = SRC_OK;
		TRACE(_T("CMD : CODA_PT::Send_Output Transfer OK\n"));
	}
	else
	{
		bReturn = SRC_Err_Transfer;
	}

	UnlockSemaphore(); //------------------------------------------------------

	return bReturn;
}

//=============================================================================
// Method		: Send_Output_Status
// Access		: public  
// Returns		: LRESULT
// Parameter	: __out enSwitchOnOff & nOutputOnOff
// Qualifier	:
// Last Update	: 2017/11/7 - 21:37
// Desc.		:
//=============================================================================
LRESULT CODA_PT::Send_Output_Status(__out enSwitchOnOff& nOutputOnOff)
{
	TRACE(_T("CODA_PT::Send_Output_Status \n"));

	if (!IsOpen())
		return SRC_Err_NotOpen;

	if (WAIT_OBJECT_0 != LockSemaphore()) //-----------------------------------
		return SRC_Err_Semaphore;

	LRESULT	bReturn = SRC_OK;

	m_stProtocol.MakeProtocol(ODA_PT::CMD_SYST_OUTP_Req);
	m_stRecvProtocol.CommandID = ODA_PT::CMD_SYST_OUTP_Req;

	if (TransferStart(m_stProtocol.Protocol.GetBuffer(0), m_stProtocol.Protocol.GetLength()))
	{
		if (WaitACK(m_dwAckWaitTime))
		{
			nOutputOnOff = m_stRecvProtocol.nOut_OnOff;

			bReturn = SRC_OK;
			TRACE(_T("CMD : CODA_PT::Send_Output_Status ACK OK\n"));
		}
		else
		{
			bReturn = SRC_Err_Ack_Timeout;
			TRACE(_T("CMD : CODA_PT::Send_Output_Status ACK Timeout ERROR\n"));
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
// Method		: Send_KeyLock
// Access		: public  
// Returns		: LRESULT
// Parameter	: __in enSwitchOnOff nKeyLockOnOff
// Qualifier	:
// Last Update	: 2017/11/7 - 21:37
// Desc.		:
//=============================================================================
LRESULT CODA_PT::Send_KeyLock(__in enSwitchOnOff nKeyLockOnOff)
{
	TRACE(_T("CODA_PT::Send_KeyLock \n"));

	if (!IsOpen())
		return SRC_Err_NotOpen;

	if (WAIT_OBJECT_0 != LockSemaphore()) //-----------------------------------
		return SRC_Err_Semaphore;

	LRESULT	bReturn = SRC_OK;

	m_stProtocol.MakeProtocol_KeyLock(nKeyLockOnOff);

	if (TransferStart(m_stProtocol.Protocol.GetBuffer(0), m_stProtocol.Protocol.GetLength()))
	{
		bReturn = SRC_OK;
		TRACE(_T("CMD : CODA_PT::Send_KeyLock Transfer OK\n"));
	}
	else
	{
		bReturn = SRC_Err_Transfer;
	}

	UnlockSemaphore(); //------------------------------------------------------

	return bReturn;
}

//=============================================================================
// Method		: Send_KeyLock_Status
// Access		: public  
// Returns		: LRESULT
// Parameter	: __out enSwitchOnOff & nKeyLockOnOff
// Qualifier	:
// Last Update	: 2017/11/7 - 21:38
// Desc.		:
//=============================================================================
LRESULT CODA_PT::Send_KeyLock_Status(__out enSwitchOnOff& nKeyLockOnOff)
{
	TRACE(_T("CODA_PT::Send_KeyLock_Status \n"));

	if (!IsOpen())
		return SRC_Err_NotOpen;

	if (WAIT_OBJECT_0 != LockSemaphore()) //-----------------------------------
		return SRC_Err_Semaphore;

	LRESULT	bReturn = SRC_OK;

	m_stProtocol.MakeProtocol(ODA_PT::CMD_SYST_KEYL_Req);
	m_stRecvProtocol.CommandID = ODA_PT::CMD_SYST_KEYL_Req;

	if (TransferStart(m_stProtocol.Protocol.GetBuffer(0), m_stProtocol.Protocol.GetLength()))
	{
		if (WaitACK(m_dwAckWaitTime))
		{
			nKeyLockOnOff = m_stRecvProtocol.nKey_OnOff;

			bReturn = SRC_OK;
			TRACE(_T("CMD : CODA_PT::Send_KeyLock_Status ACK OK\n"));
		}
		else
		{
			bReturn = SRC_Err_Ack_Timeout;
			TRACE(_T("CMD : CODA_PT::Send_KeyLock_Status ACK Timeout ERROR\n"));
		}
	}
	else
	{
		bReturn = SRC_Err_Transfer;
	}

	UnlockSemaphore(); //------------------------------------------------------

	return bReturn;
}
