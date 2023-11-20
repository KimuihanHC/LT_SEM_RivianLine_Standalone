#pragma once

#include "math.h"
#include "pch.h"
#include "DataManager.h"
#include "User_Define.h"

#define	WM_LIGHT_CONTROL_YOUTHTECH_MSG						WM_USER + 700
#define LIGHT_CONTROL_YOUTHTECH_DPS_BUFFER_MAX					23
#define LIGHT_CONTROL_YOUTHTECH_DPS_BUFFER_MAX_RCV				100

class CLightController_YouthTech_DPS : public CWnd
{
	DECLARE_DYNAMIC(CLightController_YouthTech_DPS)

public:
	CLightController_YouthTech_DPS();
	virtual ~CLightController_YouthTech_DPS();

	bool LightOn(int value, char CH = '1');
	bool LightOff(char CH = '1');

private:
	BOOL WritePort(int Port, char *szBuffer);
	BOOL WaitOK(int timeout, BOOL &kill);

	HANDLE m_hEvent_HeaderBoard;

	static void	CALLBACK Control_LIGHT_CONTROL_IRQ0_YOUTHTECH_DPS(int port);
	static void	CALLBACK Control_LIGHT_CONTROL_IRQ1_YOUTHTECH_DPS(int port);
	static void	CALLBACK Control_LIGHT_CONTROL_IRQ2_YOUTHTECH_DPS(int port);

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

	char packet[LIGHT_CONTROL_YOUTHTECH_DPS_BUFFER_MAX];
	char recv[LIGHT_CONTROL_YOUTHTECH_DPS_BUFFER_MAX_RCV];


public:
	int m_nId;

	CString m_strRecvData_LightController;
	CString m_strLastError_LightController;

	UINT m_MSG;

	BOOL Is_Connected() { return m_bConnected; }

	BOOL Create(int index, CWnd* pParentWnd = NULL);
	void InitVariables(int nID);

	BOOL OpenPort(int Port, int BaudRate, UINT Msg, int Parity = 0, int DataBit = 1, int StopBit = 0, int nIdx = 0);
	BOOL ReadPort(int Port, char *szBuffer);

	void ClosePort();

	CString m_strRevData;
	CString m_strRecvData;

	LURI_LIGHT_RECV		m_stRecv;
};
extern CLightController_YouthTech_DPS m_ctrlLightController_Youth_DPS[LIGHT_CONTROLLER_LURITECH_BOARD_COUNT];


