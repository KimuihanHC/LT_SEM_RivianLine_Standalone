//*****************************************************************************
// Filename	: 	LGE_LightBrd.cpp
// Created	:	2016/05/09
// Modified	:	2016/05/09
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************

#include "StdAfx.h"
#include "LGE_LightBrd.h"
#include "ErrorCode.h"

using namespace PR_ErrorCode;

//=============================================================================
// 생성자
//=============================================================================
CLGE_LightBrd::CLGE_LightBrd()
{
	SetDelimeter(LightBrd_STX, LightBrd_ETX, LightBrd_ProtoLength, LightBrd_ProtoLength);

	m_fMinCurrent	= 0.0f;
	m_fMaxCurrent	= 3000.0f;
	m_fUnitCurrent	= (m_fMaxCurrent - m_fMinCurrent) / 1024.0f;
}

//=============================================================================
// 소멸자
//=============================================================================
CLGE_LightBrd::~CLGE_LightBrd()
{
	//TRACE(_T("<<< Start ~CLGE_LightBrd >>> \n"));

	

	//TRACE(_T("<<< End ~CLGE_LightBrd >>> \n"));
}


//=============================================================================
// Method		: CLGE_LightBrd::OnFilterRecvData
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
LRESULT CLGE_LightBrd::OnFilterRecvData(const char* szACK, DWORD dwAckSize)
{
	return CSerialCom_Base::OnFilterRecvData(szACK, dwAckSize);
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
void CLGE_LightBrd::OnRecvProtocol(const char* szACK, DWORD dwAckSize)
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
void CLGE_LightBrd::ResetProtocol()
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
void CLGE_LightBrd::SetMinMaxVoltage(__in FLOAT fMinVolt, __in FLOAT fMaxVolt)
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
LRESULT CLGE_LightBrd::Send_PortCheck(__out BYTE& byPortNo)
{
	TRACE(_T("CLGE_LightBrd::Send_PortCheck \n"));

	if (!IsOpen())
		return SRC_Err_NotOpen;

	if (WAIT_OBJECT_0 != LockSemaphore()) //-----------------------------------
		return SRC_Err_Semaphore;

	LRESULT	bReturn = SRC_OK;

	if (TransferStart(g_szLightBrd_PortCheck, LightBrd_ProtoLength))
	{
		if (WaitACK(m_dwAckWaitTime))
		{
			byPortNo = m_stRecvProtocol.Data[0] - 0x30;

			bReturn = SRC_OK;
			TRACE(_T("CMD : 광원 보드 Port Check ACK OK\n"));
		}
		else
		{
			bReturn = SRC_Err_Ack_Timeout;
			TRACE(_T("CMD : 광원 보드 Port Check ACK Timeout ERROR\n"));
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
// Method		: Send_EtcSetVolt
// Access		: public  
// Returns		: LRESULT
// Parameter	: __in UINT nSlotNo
// Parameter	: __in float fVolt
// Qualifier	:
// Last Update	: 2017/9/20 - 18:55
// Desc.		:
//=============================================================================
LRESULT CLGE_LightBrd::Send_EtcSetVolt(__in UINT nSlotNo, __in float fVolt)
{
	TRACE(_T("CLGE_LightBrd::Send_OutputVolt \n"));
	
	if (!IsOpen())
		return SRC_Err_NotOpen;

	if (WAIT_OBJECT_0 != LockSemaphore()) //-----------------------------------
		return SRC_Err_Semaphore;

	LRESULT	bReturn = SRC_OK;

	m_stProtocol.MakeProtocol_EtcSetVolt(nSlotNo, fVolt);

	if (TransferStart((char*)&m_stProtocol, LightBrd_ProtoLength))
	{
		if (WaitACK(m_dwAckWaitTime))
		{
			if (m_stRecvProtocol.bAck)
			{
				bReturn = SRC_OK;
				TRACE(_T("CMD : 광원 보드 Etc Set Volt ACK OK\n"));
			}
			else
			{
				bReturn = SRC_Err_Ack_RecvFail;
				TRACE(_T("CMD : 광원 보드 Etc Set Volt ACK Fail Recv\n"));
			}
		}
		else
		{
			bReturn = SRC_Err_Ack_Timeout;
			TRACE(_T("CMD : 광원 보드 Etc Set Volt ACK Timeout ERROR\n"));
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
// Method		: Send_DetectVolt
// Access		: public  
// Returns		: LRESULT
// Parameter	: __out ST_SlotInfo & stOutSlotVolt
// Qualifier	:
// Last Update	: 2017/9/20 - 18:55
// Desc.		:
//=============================================================================
LRESULT CLGE_LightBrd::Send_DetectVolt(__out ST_SlotInfo& stOutSlotVolt)
{
	TRACE(_T("CLGE_LightBrd::Send_DetectVolt \n"));
	if (!IsOpen())
		return SRC_Err_NotOpen;

	if (WAIT_OBJECT_0 != LockSemaphore()) //-----------------------------------
		return SRC_Err_Semaphore;

	LRESULT	bReturn = SRC_OK;

	if (TransferStart(g_szLightBrd_DetectVolt, LightBrd_ProtoLength))
	{
		if (WaitACK(m_dwAckWaitTime))
		{
			memcpy(&stOutSlotVolt, &m_stRecvProtocol.RecvedSlotVolt, sizeof(ST_SlotInfo));

			bReturn = SRC_OK;
			TRACE(_T("CMD : 광원 보드 Detect Volt All ACK OK\n"));
		}
		else
		{
			bReturn = SRC_Err_Ack_Timeout;
			TRACE(_T("CMD : 광원 보드 Detect Volt All ACK Timeout ERROR\n"));
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
// Method		: Send_EtcSetCurrent
// Access		: public  
// Returns		: LRESULT
// Parameter	: __in UINT nSlotNo
// Parameter	: __in float fCurrent
// Qualifier	:
// Last Update	: 2017/9/20 - 17:27
// Desc.		:
//=============================================================================
LRESULT CLGE_LightBrd::Send_EtcSetCurrent(__in UINT nSlotNo, __in float fCurrent)
{
	TRACE(_T("CLGE_LightBrd::Send_EtcSetCurrent \n"));

	if (!IsOpen())
		return SRC_Err_NotOpen;

	if (WAIT_OBJECT_0 != LockSemaphore()) //-----------------------------------
		return SRC_Err_Semaphore;

	LRESULT	bReturn = SRC_OK;

	m_stProtocol.MakeProtocol_EtcSetCurrent(nSlotNo, fCurrent);

	if (TransferStart((char*)&m_stProtocol, LightBrd_ProtoLength))
	{
		if (WaitACK(m_dwAckWaitTime))
		{
			if (TRUE == m_stRecvProtocol.bAck)
			{
				bReturn = SRC_OK;
				TRACE(_T("CMD : 광원 보드 Etc Set Current ACK OK\n"));
			}
			else
			{
				bReturn = SRC_Err_Ack_RecvFail;
				TRACE(_T("CMD : 광원 보드 Etc Set Current ACK Fail Recv\n"));
			}
		}
		else
		{
			bReturn = SRC_Err_Ack_Timeout;
			TRACE(_T("CMD : 광원 보드 Etc Set Current ACK Timeout ERROR\n"));
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
// Method		: Send_DetectCurrent
// Access		: public  
// Returns		: LRESULT
// Parameter	: __out ST_SlotInfo & stOutSlotVolt
// Qualifier	:
// Last Update	: 2017/9/20 - 17:27
// Desc.		:
//=============================================================================
LRESULT CLGE_LightBrd::Send_DetectCurrent(__out ST_SlotInfo& stOutSlotVolt)
{
	TRACE(_T("CLGE_LightBrd::Send_DetectCurrent \n"));
	
	if (!IsOpen())
		return SRC_Err_NotOpen;

	if (WAIT_OBJECT_0 != LockSemaphore()) //-----------------------------------
		return SRC_Err_Semaphore;

	LRESULT	bReturn = SRC_OK;

	if (TransferStart(g_szLightBrd_DetectCurrent, LightBrd_ProtoLength))
	{
		if (WaitACK(m_dwAckWaitTime))
		{
			memcpy(&stOutSlotVolt, &m_stRecvProtocol.RecvedSlotVolt, sizeof(ST_SlotInfo));

			bReturn = SRC_OK;
			TRACE(_T("CMD : 광원 보드 Detect Current ACK OK\n"));
		}
		else
		{
			bReturn = SRC_Err_Ack_Timeout;
			TRACE(_T("CMD : 광원 보드 Detect Current ACK Timeout ERROR\n"));
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
// Method		: Send_ControlCheck
// Access		: public  
// Returns		: LRESULT
// Parameter	: __out BOOL & bAuto
// Parameter	: __out BOOL & bManual
// Qualifier	:
// Last Update	: 2017/9/20 - 17:30
// Desc.		:
//=============================================================================
LRESULT CLGE_LightBrd::Send_ControlCheck(__out BOOL& bAuto, __out BOOL& bManual)
{
	TRACE(_T("CLGE_LightBrd::Send_ControlCheck \n"));

	if (!IsOpen())
		return SRC_Err_NotOpen;

	if (WAIT_OBJECT_0 != LockSemaphore()) //-----------------------------------
		return SRC_Err_Semaphore;

	LRESULT	bReturn = SRC_OK;

	if (TransferStart(g_szLightBrd_ControlCheck, LightBrd_ProtoLength))
	{
		if (WaitACK(m_dwAckWaitTime))
		{
			bAuto	= m_stRecvProtocol.bAutomatic;
			bManual = m_stRecvProtocol.bManual;

			bReturn = SRC_OK;
			TRACE(_T("CMD : 광원 보드 Control Check ACK OK\n"));
		}
		else
		{
			bReturn = SRC_Err_Ack_Timeout;
			TRACE(_T("CMD : 광원 보드 Control Check ACK Timeout ERROR\n"));
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
// Method		: Send_AmbientVoltOn_All
// Access		: public  
// Returns		: LRESULT
// Parameter	: __in float fVolt
// Qualifier	:
// Last Update	: 2017/10/7 - 18:27
// Desc.		:
//=============================================================================
LRESULT CLGE_LightBrd::Send_AmbientVoltOn_All(__in float fVolt)
{
	return Send_EtcSetVolt(Slot_All, fVolt);
}

//=============================================================================
// Method		: Send_AmbientVoltOff_All
// Access		: public  
// Returns		: LRESULT
// Qualifier	:
// Last Update	: 2017/10/7 - 18:26
// Desc.		:
//=============================================================================
LRESULT CLGE_LightBrd::Send_AmbientVoltOff_All()
{
	return Send_EtcSetVolt(Slot_All, 0.0f);
}

//=============================================================================
// Method		: Send_AmbientLightOn_All
// Access		: public  
// Returns		: BOOL
// Parameter	: __in WORD wValue
// Qualifier	:
// Last Update	: 2016/10/20 - 13:40
// Desc.		:
//=============================================================================
LRESULT CLGE_LightBrd::Send_AmbientCurrentOn_All(__in WORD wValue)
{
	//FLOAT fCurrent = m_fMinCurrent + (m_fUnitCurrent * wValue);

	return Send_EtcSetCurrent(Slot_All, wValue);
}

//=============================================================================
// Method		: Send_AmbientLightOff_All
// Access		: public  
// Returns		: LRESULT
// Qualifier	:
// Last Update	: 2017/9/20 - 17:38
// Desc.		:
//=============================================================================
LRESULT CLGE_LightBrd::Send_AmbientCurrentOff_All()
{
	return Send_EtcSetCurrent(Slot_All, 0.0f);
}

