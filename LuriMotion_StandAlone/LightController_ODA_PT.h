#pragma once

#include "math.h"
#include "pch.h"
#include "DataManager.h"
#include "User_Define.h"

#define	WM_LIGHT_CONTROL_PT_MSG					WM_USER + 630 
#define LIGHT_CONTROL_PT_BUFFER_MAX				23
#define LIGHT_CONTROL_PT_BUFFER_MAX_RCV			100
#define LIGHT_CONTROL_PT_COMMAND_TIMEOUT			4000
#define LIGHT_CONTROL_PT_COMMAND_ALLDATA_TIMEOUT	4000
#define LIGHT_CONTROL_PT_ETX_MSG					0x40				//'@'

#define LIGHT_CONTROL_PT_READY						0
#define LIGHT_CONTROL_PT_DEFAULT_MODE				1


class CLightController_ODA_PT : public CWnd
{
	DECLARE_DYNAMIC(CLightController_ODA_PT)

public:
	CLightController_ODA_PT();
	virtual ~CLightController_ODA_PT();
	

private:
	

	BOOL WritePort(int Port, char *szBuffer);
	BOOL WaitOK(int timeout, BOOL &kill);
		
	HANDLE m_hEvent_HeaderBoard;

	static void	CALLBACK Control_LIGHT_CONTROL_PT_IRQ0(int port);
	static void	CALLBACK Control_LIGHT_CONTROL_PT_IRQ1(int port);


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

	char packet[LIGHT_CONTROL_PT_BUFFER_MAX];
	char recv[LIGHT_CONTROL_PT_BUFFER_MAX_RCV];
	

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

	// 포트 체크
	int			m_nRecvType;

	BOOL		Send_PortCheck();

	BOOL		Send_Apply(__in float fVoltage, __in float fCurrent);
	BOOL		Send_Apply(__in float fVoltage);
	BOOL		Send_Apply_Status(__out float& fOutVoltage, __out float& fOutCurrent);
	BOOL		Send_Voltage(__in float fVoltage);
	BOOL		Send_Voltage_Req(__out float& fOutVoltage);
	BOOL		Send_Current(__in float fCurrent);
	BOOL		Send_Current_Req(__out float& fOutCurrent);
	
	BOOL		Send_Flow_Status(__in enCV_CC nOutCVCC);
	
	BOOL		Send_Meas_ALL(__out float& fOutVoltage, __out float& fOutCurrent);
	BOOL		Send_Error_Status(__out int& iOutErrCode, __out CString& szOutErrorDesc);
	BOOL		Send_Version_Status(__out CString& szOutVersion);
	BOOL		Send_Output(__in enSwitchOnOff nOutputOnOff);
	BOOL		Send_Output_Status(__out enSwitchOnOff& nOutputOnOff);
	BOOL		Send_KeyLock(__in enSwitchOnOff nKeyLockOnOff);
	BOOL		Send_KeyLock_Status(__out enSwitchOnOff& nKeyLockOnOff);
};
extern CLightController_ODA_PT m_ctrlLight_PT[LIGHT_CONTROLLER_PT_BOARD_COUNT];


