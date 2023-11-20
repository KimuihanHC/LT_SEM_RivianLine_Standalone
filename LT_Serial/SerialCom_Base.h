//*****************************************************************************
// Filename	: 	SerialCom_Base.h
// Created	:	2016/5/9 - 19:50
// Modified	:	2016/5/9 - 19:50
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
#ifndef SerialCom_Base_h__
#define SerialCom_Base_h__

#pragma once

#include "SerialMFC.h"
#include "OldSerQueue.h"
#include "ErrorCode.h"
#include "WLog.h"

#define MAX_SERIAL_BUFFER	1024

// Serial 통신 결과 코드
typedef enum enSerialResultCode
{
	SRC_Err_Unknown,
	SRC_OK,
	SRC_Err_NotOpen,
	SRC_Err_Semaphore,
	SRC_Err_Transfer,
	SRC_Err_Parameter,
	SRC_Err_Ack,
	SRC_Err_Ack_Timeout,
	SRC_Err_Ack_RecvFail,
	SRC_Err_Ack_Protocol,
	SRC_Err_Protocol,
	SRC_Err_Protocol_CheckSum,
}enSerialResultCode;

//-----------------------------------------------------------------------------
// CSerialCom_Base
//-----------------------------------------------------------------------------
class CSerialCom_Base : public CSerialMFC
{
public:
	CSerialCom_Base();
	virtual ~CSerialCom_Base();

	//---------------------------------------------------------
	// 재정의
	//---------------------------------------------------------
protected:
	virtual void	OnEvent					(EEvent eEvent, EError eError);
	virtual LRESULT	OnFilterRecvData		(const char* szACK, DWORD dwAckSize);
	virtual void	OnRecvProtocol			(const char* szACK, DWORD dwAckSize);
	
	HWND			m_hOwnerWnd;		// 이 클래스를 호출한 윈도우 핸들	
	UINT			m_WM_Ack;			// 수신된 데이터 전송용 윈도우 메세지

	//-----------------------------------------------------
	// 상위 윈도우로 Log 메세지 보내기 위한 함수
	//-----------------------------------------------------
	CWLog			m_Log;

	//-----------------------------------------------------
	// 에러 처리 / 동기화 처리
	//-----------------------------------------------------
	DWORD			m_dwErrCode;

	HANDLE			m_hSemaphore;
	DWORD			m_dwSemaphoreWaitTime;

	DWORD			LockSemaphore			();
	BOOL			UnlockSemaphore			();
	BOOL			WaitSemaphore			(DWORD dwTimeout = 2000);

	//-----------------------------------------------------
	// Event 처리
	//-----------------------------------------------------
	HANDLE			m_hEvent_ACK;			// 시리얼 포트의 ACK 수신 이벤트
	HANDLE			m_hEvent_ThreadExit;	// 외부 신호에 의한 쓰레드 종료 이벤트
	BOOL			CreateAckEvent			();
	BOOL			CloseAckEvent			();

	//-----------------------------------------------------
	// 통신 모듈
	//-----------------------------------------------------
	DWORD			m_dwAckWaitTime;		// 커맨드 전송후 ACK 대기 시간
	BOOL			WaitACK					(DWORD dwWaitTime = 1000);

// 	ST_LightBrdProtocol		m_stProtocol;
// 	ST_LightBrdRecvProtocol	m_stRecvProtocol;

	//-----------------------------------------------------
	// ACK Data 필터링
	//-----------------------------------------------------
	char			m_szReadBuf[MAX_SERIAL_BUFFER];	// 수신된 데이터를 저장하는 버퍼
	DWORD			m_dwReadedBufSize;				// 수신된 데이터의 크기

	char			m_szACKBuf[MAX_SERIAL_BUFFER];	// 수신된 데이터를 파싱하여 프로토콜에 맞춘 데이터
	DWORD			m_dwACKBufSize;					// ACK 데이터의 크기

	void			ResetAckBuffer();				// ACK 데이터 초기화

	COldSerQueue	m_SerQueue;

	CString			m_szPort;

	UINT_PTR		m_nMinProtocolLength;
	UINT_PTR		m_nMaxProtocolLength;
	char			m_chSTX;
	char			m_chETX;
	void			SetDelimeter			(__in char chSTX, __in char chETX, __in UINT_PTR nMinProtocolLength, __in UINT_PTR nMaxProtocolLength);
	void			SetProtocolLength		(__in UINT_PTR nMinProtocolLength, __in UINT_PTR nMaxProtocolLength);

public:

	void			 SetOwnerHwnd			(HWND hOwnerWnd)
	{
		m_hOwnerWnd = hOwnerWnd;
	};

	void			SetExitEvent			(HANDLE hEvent)
	{
		m_hEvent_ThreadExit = hEvent;
	};

	void			SetSemaphore			(LPCTSTR lpName);
	void			ResetSemaphore			();

	void			SetLogMsgID				(UINT nWM_ID, UINT nLogType = 0);
	void			SetAckMsgID				(UINT nWM_ID);
	void			SetAckWaitTime			(DWORD dwMilisecond);
	
	//-----------------------------------------------------
	// 연결/해제
	//-----------------------------------------------------
	HANDLE			Connect					(LPCTSTR inPort, DWORD inBaudRate, BYTE inParity, BYTE inStopBits, BYTE inByteSize);
	BOOL			Disconnect				();
	BOOL			ReConnect				();
	BOOL			IsConnected				();

	BOOL			TransferStart			(const char* lpstrSendBuff, UINT nBuffSize);
	BOOL			TransferProtocol		(const char* lpcBuffer, UINT wBufferSize, BOOL bWaitAck = FALSE);

	DWORD_PTR		GetAckData				(char* lpcAck);
	DWORD_PTR		GetLastErrorCode		()	
	{ 
		return m_dwErrCode; 
	};

// 	ST_LightBrdRecvProtocol	GetAckProtocol()
// 	{
// 		return m_stRecvProtocol;
// 	};

	BOOL			WaitRecvAck				();
	void			OnDebugMsgFile			(__in LPCSTR szMsg, __in DWORD nBuffSize, __in BOOL bSend = TRUE);
};

#endif // SerialCom_Base_h__
