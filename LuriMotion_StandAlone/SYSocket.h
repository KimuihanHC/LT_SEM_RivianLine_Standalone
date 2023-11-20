#if !defined(AFX_SYSOCKET_H__0ECF3FE6_C149_45A1_827A_010F81F73C0C__INCLUDED_)
#define AFX_SYSOCKET_H__0ECF3FE6_C149_45A1_827A_010F81F73C0C__INCLUDED_

#include "pch.h"
#include "EzSock.h"	// Added by ClassView

#if _MSC_VER > 1000

#pragma once
#endif // _MSC_VER > 1000
// SYsocket.h : header file
//
#include "afxwin.h"

#include "DataManager.h"



//#define WM_MODBUS_MSG				(WM_USER+9000)

#define FLAG_SOCKET_RECEIVE			1
#define FLAG_SOCKET_CONNECT			2
#define FLAG_SOCKET_CLOSE			3
#define FLAG_SOCKET_ERROR			4

#define MODE_SOCKET_READ			1
#define MODE_SOCKET_STOP			2

#define SOCKET_READ_FC				0x03
#define SOCKET_READ_ERROR_FC		0x83
#define SOCKET_WRITE_FC				0x10
#define SOCKET_WRITE_ERROR_FC		0x90

#define SOCKET_DATA_MAX_SIZE		200

#define TIMER_RECIEVE				1
#define RECIEVE_WAIT_TIME			(int)1000
#define RECIEVE_DELAY_TIME			10

enum {
	eComMode_Ready, eComMode_PreviewStart, eComMode_Insp_SFR, eComMode_Insp_Distortion, eComMode_Insp_TEST, eComMode_Insp_PreviewStop
	, eComMode_Barcode, eComMode_Version, eComMode_SwInit, eComMode_FuseID, eComMode_Status, eComMode_MES_CONNECT, eComMode_SendMES, eComMode_MAX
};


class CSYSocket : public CWnd
{
public:
	CSYSocket(void);
	virtual ~CSYSocket(void);

// Generated message map functions
protected:
	//{{AFX_MSG(SYSocket)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	afx_msg void OnSockConnectMsg(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSockCloseMsg(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSockAcceptMsg(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSockRecvMsg(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

	afx_msg void OnTimer(UINT_PTR nIDEvent);

private:
	_u8 send_buf[SOCKET_DATA_MAX_SIZE];
	_u8 recv_buf[SOCKET_DATA_MAX_SIZE];

	int m_nRecvPos;
	int m_nRecieveSetTimerFlag;
	BOOL m_bRecieveCR_OK;
	BOOL m_server_accept;
	CEzSock m_socket;
	CEzSock m_socket_server;

	int m_nInitFlag;
	
	void send_msg_target(int flag, int code);


public:
	BOOL Create(CWnd* pParentWnd=NULL);

	void InitVariable(int id);
	void CloseSocket();
	void CreateSocket(DWORD server_ip, UINT server_port, int flag);
	void CreateSocket(DWORD server_ip, UINT server_port, CString strIP, int flag);

	BOOL		m_bRetrun_Recv;				// 데이터 Recv 시에 변환되는 Flag
	CString		m_strReturn_Log;			// Log에 남기기 위한 문자열
		
public:	
	int m_nId;
	int m_nMode;
	int m_nLogMode_Socket;

	CString m_strLastError;
	CString m_strRecvData;			//수신 된 Data
		
	BOOL m_bConnected;	
	BOOL m_bReturn;


	bool Send_Command(CString strMsg);

	void NoatepadDebug(__in LPCTSTR szDebugString);


public:
	// 수신후 종료
	bool Request_Recv(BYTE *byExt, BYTE *byData, int nLength);
	// 'S' 일경우 Return;

	// Request_Recv 관련 변수들.
	BOOL m_bFlag_Status;
	BOOL m_bFlag_Tester_Start;
	CString  m_strTester_ReturnFail;

	BOOL m_bFlag_Tester_Stop;

	BOOL m_bFlag_Tester_Result;
	CString m_strTester_Result;

	BOOL m_bFlag_Emg;
	BOOL m_bFlag_Origin;

	BOOL m_bFlag_Lot_Start;
	BOOL m_bFlag_Lot_Stop;
	BOOL m_bFlag_Lot_Info;

	BOOL m_bFlag_Move_Stage;				// 지정위치로 이동하라는 Flag
	BOOL m_bFlag_Move_Motor;				// 모터를 위치값만큼 이동하라는 Flag Single Mode에서 사용
	BOOL m_bFlag_Move_Motor_Abs;			// 모터를 위치값만큼 이동하라는 Flag Dual Mode에서 사용

	BOOL m_bFlag_ChartLight;
	BOOL m_bFlag_BlemishLight;


	int  m_nData_MoveStage_TeachingPosition;
	int  m_nData_MoveStage_TeachingPositionBK;

	int  m_nData_Motor[MOTOR_MAX_COUNT];
	int  m_nData_MoveDistance[MOTOR_MAX_COUNT];

	BOOL m_bLightChangeCommandRecv;
	int  m_nData_LightNum[LIGHT_CONTROLLER_TYPE_COUNT];
	int  m_nData_LightValue[LIGHT_CONTROLLER_TYPE_COUNT];

	BOOL	m_bVaibration_Run;
	int		m_nVaibration_Angle;
	int		m_nVaibration_Hz;
	BOOL	m_bVaibration_Stop;


	 BYTE szBuffer_Save[50];
	 int m_nLastEtxCnt;


public:
	// 이곳은 핸들러와 검사기의 통신에 사용되는 함수들		
	// 수신후 회신
	bool Send_Return_s(BYTE *byExt, BYTE byTesterNum);
	bool Send_Return(BYTE *byExt, BYTE byTesterNum);
	bool Send_Return_LongData(BYTE *byExt, BYTE *byData, int nDataLength, BYTE byTesterNum);


	// Reset
	void Reset_FlagData();
	void Reset_MotorMove_FlagData();
	void Reset_LightController_FlagData();
	

	// Response
	// 실제 사용하는 Light Contorller
	bool Response_LightController(int nTester);
	// 이동 완료후 Response.
	bool Response_StageMove(int nPos);
	bool Response_MotorMove_Rel(int nPos);
	bool Response_MotorMove_Abs(int nPos);
	bool Response_Vaibration(int nPos);
	bool Response_Vaibration_Stop(int nPos);

	bool m_bTest;


	// 송신 Request
	bool Send_Status(int nTesterNum);
	bool Send_Start(int nTesterNum, CString strBarcode);
	bool Send_Start_NoBarcode(int nTesterNum);
	bool Send_Stop(int nTesterNum);
	bool Send_Result(int nTesterNum);
	bool Send_EmgStop(int nTesterNum);
	bool Send_Origin();

	bool Send_LotStart(int nTesterNum);
	bool Send_LotStop(int nTesterNum);
	bool Send_LotInfo(CString strInfo);

	bool Send_StageMove(int nPos);
	bool Send_MotorMove_Rel(CString strMove);
	bool Send_MotorMove_Abs(CString strMove);

	// 이건 임의로 만든거 같은데..?
	bool Send_ChartLight(CString strLight);
	bool Send_BlemishLight(CString strLight);
		
	

	// 아래는 핸들러 to 핸들러 통신.
public:
	
	void ParsingData_HandlerToHandler(CString strData);

	bool Send_Handler_SampleInfo(int nHandlerNum);
	bool Send_Handler_MchStart(int nHandlerNum, CString strBarcode);
	bool Send_Handler_LivePing(int nHandlerNum);

	bool Return_Handler_SampleInfo(int nHandlerNum, int nState);
	bool Return_Handler_MchStart(int nHandlerNum, int nState);
	bool Return_Handler_LivePing(int nHandlerNum);

	// Sample:
	// @001,1,			// LIVE PING  무조건 2번째 파라가 1이 와야된다.
	// @002,Barcode,    // Barcode
	// @003,1,			// 1 : START, 0 : STOP
	// @004,1,			// 1 : SHUTTLE
	CString m_strReciveBarcode;
	BOOL	m_bConnection_Live;
	BOOL	m_bRecive_StartSignal;
	BOOL    m_bRecive_ShuttleMove_OK;



};
extern CSYSocket m_ctrlOthers[LAN_COMMUNICATION_COUNT];

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSOCKET_H__0ECF3FE6_C149_45A1_827A_010F81F73C0C__INCLUDED_)
