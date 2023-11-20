#pragma once

#include "pch.h"
#include "afxwin.h"
#include "DataManager.h"

class CUDPSocket: public CWnd
{
public:
	CUDPSocket(void);
	virtual ~CUDPSocket(void);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	static UINT	Thread_UDPRead(LPVOID lpVoid);

	DECLARE_MESSAGE_MAP()

	
	CWinThread* pUDP_RecvThread;


public:
	void CloseSocket();
	BOOL Create(CWnd* pParentWnd = NULL);

	BOOL m_bThreadRun;

	afx_msg void Connect_Tester(DWORD server_ip, UINT server_port, CString strIP, int flag);
	afx_msg BOOL Thread_Start();

	SOCKET		m_clientSock;
	SOCKADDR_IN	m_ClientAddr;
	SOCKADDR_IN	m_FromServer;

public:
	int m_nId;
	void InitVariable(int id);
	bool m_bConnected;

	// 여기는 삼성전기 TESTER PROGRAM 통신
	void ParsingData_HandlerToTester(CString strData);

	bool Send_Tester_Communication(int nMode, int nTestIdx  = 0);
	bool Send_Flare_CLL_Communication(int nCondition, int nLL);
	void Reset_ReadyFlag();
	void Reset_TesterFlag();

	BOOL			m_bConnectCheckFunc;
	DWORD			m_dwTimeOut;

	BOOL			m_bTesterFlag_Ready;
	BOOL			m_bTesterFlag_PreviewStart;
	BOOL			m_bTesterFlag_Inspection_TEST;		// 이물검사
	BOOL			m_bTesterFlag_Inspection_SFR;		// SFR 검사
	BOOL			m_bTesterFlag_Distortion[10];		// Distortion 검사
	BOOL			m_bTesterFlag_PreviewStop;

	BOOL			m_bAckFlag_Barcode;					// 바코드
	BOOL			m_bAckFlag_CurrentVoltage;				// 전류 전압값

	int				m_nTesterFlag_ReadyErrorCode;
	int				m_bTesterFlag_PreviewStartErrorCode;
	int				m_bTesterFlag_Inspection_SFRErrorCode;		// SFR 검사
	int				m_nTesterFlag_Inspection_DistortionErrorCode[10];		// Distortion 검사
	int				m_bTesterFlag_Inspection_TESTErrorCode;		// 이물검사
	int				m_bTesterFlag_PreviewStopErrorCode;

	CString			m_strTesterQuery_Barcode;
	CString			m_strTesterQuery_Version;
	BOOL			m_strTesterQuery_SWInital;
	CString			m_strTesterQuery_FuseID;
	CString			m_strTesterQuery_Status;
	BOOL			m_strTesterQuery_MES_Connect;
	CString			m_strTesterQuery_MesData;
	CString			m_strTesterQuery_CurrentVoltage;

	BOOL			m_bFlareGhostFlag_Recive[2][10];
	
	BOOL			ForcedLive_Ready();
	bool			Send_Barcode(__in CString szBCR);
	bool			Send_CurrentVoltage(__in float current, __in float voltage);
	bool			SendData(__in CString szSend);
};

extern CUDPSocket m_UdpTester[LAN_COMMUNICATION_COUNT];