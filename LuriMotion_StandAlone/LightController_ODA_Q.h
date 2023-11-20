#pragma once

#include "math.h"
#include "pch.h"
#include "DataManager.h"
#include "User_Define.h"

#define	WM_LIGHT_CONTROL_Q_MSG					WM_USER + 640 
#define LIGHT_CONTROL_Q_BUFFER_MAX				23
#define LIGHT_CONTROL_Q_BUFFER_MAX_RCV			100
#define LIGHT_CONTROL_Q_COMMAND_TIMEOUT			4000
#define LIGHT_CONTROL_Q_COMMAND_ALLDATA_TIMEOUT	4000
#define LIGHT_CONTROL_Q_ETX_MSG					0x40				//'@'

#define LIGHT_CONTROL_Q_READY						0
#define LIGHT_CONTROL_Q_DEFAULT_MODE				1


class CLightController_ODA_Q : public CWnd
{
	DECLARE_DYNAMIC(CLightController_ODA_Q)

public:
	CLightController_ODA_Q();
	virtual ~CLightController_ODA_Q();
	

private:
	

	BOOL WritePort(int Port, char *szBuffer);
	BOOL WaitOK(int timeout, BOOL &kill);
		
	HANDLE m_hEvent_HeaderBoard;

	static void	CALLBACK Control_LIGHT_CONTROL_Q_IRQ0(int port);
	static void	CALLBACK Control_LIGHT_CONTROL_Q_IRQ1(int port);


protected:
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnLightControlReceive(WPARAM nPort, LPARAM nLen);

private:
	BOOL m_bHW;
	BOOL m_bSW;
	
	
	int m_nPort;
	int	m_npos;
	int m_nMode;

	char packet[LIGHT_CONTROL_Q_BUFFER_MAX];
	char recv[LIGHT_CONTROL_Q_BUFFER_MAX_RCV];
	

public:
	int m_nId;
	BOOL m_bConnected;
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

	double m_dGetCurrent;
	double m_dGetVolt;
	int			m_nRecvType;

	BOOL Write_OFF();
	BOOL Write_Channel(int nChannel);
	BOOL Write_Volt(double dVolt);
	BOOL Write_Current(double dCur);
	BOOL Wrtie_ON();

	BOOL Read_Current();
	BOOL Read_Volt();
	int m_nReadMode;

	void SetCurrentValue(double dVal);
	void SetVoltValue(double dVal);

	double GetCurrentValue();
	double GetVoltValue();

};
extern CLightController_ODA_Q m_ctrlLight_ODA_Q[LIGHT_CONTROLLER_PT_BOARD_COUNT];


