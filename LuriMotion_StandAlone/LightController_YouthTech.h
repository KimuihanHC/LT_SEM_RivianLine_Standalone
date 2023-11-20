#pragma once

#include "math.h"
#include "pch.h"
#include "DataManager.h"
#include "User_Define.h"

#define	WM_LIGHT_CONTROL_YOUTHTECH_MSG						WM_USER + 650 
#define LIGHT_CONTROL_YOUTHTECH_BUFFER_MAX					23
#define LIGHT_CONTROL_YOUTHTECH_BUFFER_MAX_RCV				100
#define LIGHT_CONTROL_YOUTHTECH_COMMAND_TIMEOUT				4000
#define LIGHT_CONTROL_YOUTHTECH_COMMAND_ALLDATA_TIMEOUT		4000
#define LIGHT_CONTROL_YOUTHTECH_ETX_MSG						0x40				//'@'

#define LIGHT_CONTROL_YOUTHTECH_READY						0
#define LIGHT_CONTROL_YOUTHTECH_DEFAULT_MODE				1



#define YOUTHTECH_CMD_GET_STATUS							0x41
#define YOUTHTECH_CMD_SET_ONOFF								0x42
#define YOUTHTECH_CMD_SET_TCP								0x43
#define YOUTHTECH_CMD_EV									0x44
#define YOUTHTECH_CMD_ACTIVE								0x45
#define YOUTHTECH_CMD_CALIBRATION							0x46
#define YOUTHTECH_CMD_SELECT_CH								0x47
#define YOUTHTECH_CMD_GET_STATUS2							0x48
#define YOUTHTECH_CMD_ONOFF_SELECT							0x49



class CLightController_YouthTech : public CWnd
{
	DECLARE_DYNAMIC(CLightController_YouthTech)

public:
	CLightController_YouthTech();
	virtual ~CLightController_YouthTech();
	

private:
	

	BOOL WritePort(int Port, char *szBuffer);
	BOOL WaitOK(int timeout, BOOL &kill);
		
	HANDLE m_hEvent_HeaderBoard;

	static void	CALLBACK Control_LIGHT_CONTROL_IRQ0_YOUTHTECH(int port);
	static void	CALLBACK Control_LIGHT_CONTROL_IRQ1_YOUTHTECH(int port);


protected:
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnLightControlReceive(WPARAM nPort, LPARAM nLen);

private:
	BOOL m_bHW;
	BOOL m_bSW;
	BOOL m_bConnected;
	
	int m_nPort;
	int	m_npos;
	int m_nMode;

	char packet[LIGHT_CONTROL_YOUTHTECH_BUFFER_MAX];
	char recv[LIGHT_CONTROL_YOUTHTECH_BUFFER_MAX_RCV];
	

public:
	int m_nId;

	CString m_strRecvData_LightController;
	CString m_strLastError_LightController;
	
	UINT m_MSG;

	BOOL Is_Connected() { return m_bConnected; }

	BOOL Create(CWnd* pParentWnd=NULL);
	void InitVariables(int nID);

	BOOL OpenPort(int Port, int BaudRate, UINT Msg,int Parity=0, int DataBit=1, int StopBit=0, int nIdx = 0);
	BOOL ReadPort(int Port, char *szBuffer);	
	
	void ClosePort();

	CString m_strRevData;
	CString m_strRecvData;

	LURI_LIGHT_RECV		m_stRecv;
	// LURITECHT BOARD
public:

	BOOL Send_Light_Command(char Ch, char cCommand, int nData);
};
extern CLightController_YouthTech m_ctrlLightController_Youth[LIGHT_CONTROLLER_LURITECH_BOARD_COUNT];


