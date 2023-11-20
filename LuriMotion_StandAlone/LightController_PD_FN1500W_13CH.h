#pragma once

#include "math.h"
#include "pch.h"
#include "DataManager.h"
#include "User_Define.h"

#define	WM_LIGHT_CONTROL_FN1500_MSG					WM_USER + 600 
#define LIGHT_CONTROL_FN1500_BUFFER_MAX				23
#define LIGHT_CONTROL_FN1500_BUFFER_MAX_RCV			100
#define LIGHT_CONTROL_FN1500_COMMAND_TIMEOUT			4000
#define LIGHT_CONTROL_FN1500_COMMAND_ALLDATA_TIMEOUT	4000
#define LIGHT_CONTROL_FN1500_ETX_MSG					0x40				//'@'

#define LIGHT_CONTROL_FN1500_READY						0
#define LIGHT_CONTROL_FN1500_DEFAULT_MODE				1


class CLightController_FN1500 : public CWnd
{
	DECLARE_DYNAMIC(CLightController_FN1500)

public:
	CLightController_FN1500();
	virtual ~CLightController_FN1500();
	

private:
	

	BOOL WritePort(int Port, char *szBuffer);
	BOOL WaitOK(int timeout, BOOL &kill);
		
	HANDLE m_hEvent_HeaderBoard;

	static void	CALLBACK Control_LIGHT_CONTROL_IRQ0(int port);
	static void	CALLBACK Control_LIGHT_CONTROL_IRQ1(int port);	


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

	char packet[LIGHT_CONTROL_FN1500_BUFFER_MAX];
	char recv[LIGHT_CONTROL_FN1500_BUFFER_MAX];
	

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
	
	float m_fMinCurrent;
	float m_fMaxCurrent;
	float m_fUnitCurrent;

	void SetMinMaxVoltage(float fMin, float fMax);
	void SetMinMaxCurrent(float fMin, float fMax);

	BOOL Set_FN1500_All_On();
	BOOL Set_FN1500_All_Off();
	BOOL Set_FN1500_Channel_On(int channel);
	BOOL Set_FN1500_Channel_Off(int channel);


	BOOL Set_FN1500_C(int nChannel, int fVolt);


};
extern CLightController_FN1500 m_ctrlLightController_FN1500[2];


