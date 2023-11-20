#pragma once

#include "pch.h"
#include "afxwin.h"
#include "resource.h"	

#include "User_Define.h"
#include "System_Define.h"

#include "Def_ErrorCode.h"

class CCommon : public CWnd, public CSington<CCommon>
{
	DECLARE_DYNAMIC(CCommon)

public:
	CCommon(void);
	virtual ~CCommon(void);

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:

	HANDLE				m_closeEvent;

	THREAD_PARAM		m_ParamMsg;
	BOOL				m_bThreadShowMsg;
	int					m_nMsg_NoCloseFlag;

	int					m_nMsgTimerOpenFlag;

	static	UINT	Thread_ShowMsg(LPVOID lpParam);
	afx_msg void	ShowMsg_Thread(CString strMsg, int nMode, bool bTesterAlarm = false);

	afx_msg INT_PTR	Show_MsgBox(CString strMsg, int nMode);			//User Message Box Open	
	afx_msg void	Close_MsgBox(void);


	// dialog Msgbox
	LARGE_INTEGER m_llErrorStartTime_DoModal;
	LARGE_INTEGER m_llErrorEndTime_DoModal;
	LARGE_INTEGER m_llErrorStartTime_Thread;
	LARGE_INTEGER m_llErrorEndTime_Thread;

public:
	//=================================================
	// dlg

	BOOL ShowMsgDlg_Domodal(CString strInfo, int nMode);
	//=================================================
	// Wait
	BOOL Wait_Time(double msec, BOOL *pKill);
	void DoEvent(int cnt);

	bool DoEvents(void);

	void uSleep_NoReturn(double msec);

	int	m_nTestFlag;
	void TESTRUN();

public:
	//=================================================
	// COMMON 에서 쓰는것도 만들어보자
	void Write_Main_Log(CString strLog);
	void Write_Comm_Log(CString strLog);
	void Write_Barcode_Log(CString strLog);
	void Write_Sequence_Log(CString strLog);

	BOOL	m_bTesterConnectFlag[5];

public:
	//=================================================
	// 관리자 권한 으로 프로그램 기능을 막자.
	CString m_strid;
	int		m_nGrade;

	CString GetPermissionId();
	int		GetPermissionGrade();
	
	void	SetPermissionID( CString strID );
	void	SetPermissionGrade( int nGrade);


public:
	//=================================================
	// 장비 START / STOP / PAUSE 등등 SIDE PANNEL 기능
	BOOL	Check_EMO();
	BOOL	Check_Mch_Start();
	BOOL	Mch_Pause();
	BOOL	Mch_Stop();
	BOOL	Check_Socket();

	BOOL	bEMOResetFlag;
	bool	bInitMotorFlag[10];

	BOOL	m_bMotorOrginFlag[10];

	BOOL	Input_Block();
	BOOL	Output_Block();
	BOOL	Check_Mch_DryRun();

	bool	m_bOriginPopUpCheck;
	void	SetOriginPopUp();
	void	ResetOriginPopUp();
	bool	GetOriginPopUp();

	bool	m_bFirstInit;
	void	SetFirstInit();
	bool	GetFirstInit();

	BOOL	m_bEMO_All_InitFlag;
	BOOL	GetEMOAllInitFlag();
	void	SetEMOAllInitFlag(BOOL bState);

public:
	//=================================================
	// Convayor Sensor 활성화 선택.  ( OQA에서 사용안함 ) Inline용
	BOOL	Convayor1_Sensor_Select(int nPos);
	BOOL	Convayor2_Sensor_Select(int nPos);
	BOOL	Convayor3_Sensor_Select(int nPos);

	BOOL	Convayor1_Sensor_Select_Clear();
	BOOL	Convayor2_Sensor_Select_Clear();
	BOOL	Convayor3_Sensor_Select_Clear();

	//=================================================
	// Convayor Sensor 활성화 선택.
	BOOL	m_bFlagSampleInput_Block_Foced;
	BOOL	m_bFlagSampleOutput_Block_Foced;

	//=================================================
	// Dryrun Mode 활성화 선택
	BOOL	m_bFlagDryRunMode;
	BOOL GetDryRunModeFlag();
	void SetDryRunModeFlag(BOOL bState);
	BOOL	m_bRecipeChange;

	//시업점검 모드
	BOOL m_bFlagStartUpMode;
	int	m_nFlagStartUpModeAction;
	BOOL GetFlagStartUpMode();
	void SetFlagStartUpMode(BOOL bState);
	int GetFlagStartUpModeAction();
	void SetFlagStartUpModeAction(int nAction);

	CString GetErrorString(__in int errcode);

	//================================================
	// 에러명 언어별로 가져오는 함수.
	CString GetErrorName_EachLanguage(int nErrorNum);

	//================================================
	// 에러발생시 에러로그 작성하는 함수.
	void	SetErrorLog_Write(CString strNumber);

	//================================================
	// Interlock
	BOOL		GetInterlock_DoorCheck();
	enMCErrCode	GetInterlock_AreaSensor();
	BOOL		GetInterlock_CameraDetected();
	BOOL		GetInterlock_SocketInAndDown();
	BOOL		GetInterlock_SocketIn();
	BOOL		GetInterlock_SocketClose();

	void	SetDoorLock		(enDoorLIst doorId, BOOL action);
	BOOL	GetDoorClose	(enDoorLIst doorid);
	//================================================
	// Tester Connect Flag.
	BOOL	m_bConnectTesterFlag[LAN_COMMUNICATION_COUNT];

private:
	//Loto Mode // [21.1018.1]
	BOOL m_bLotoMode;

public:
	//================================================
	// TOWERLAMp COMMAND
	//void	SetTowerLampStatus(int nMode);
	void	SetEqStatus(RUN_MODE rm);

	//Sleep Mode
	time_t m_tSleepStart;
	// [21.1018.8] Added
	time_t m_tSleepElapsed;
	int m_SleepModeStep;
	BOOL bUseSleepMode;

	void	SetSleepModeStep(int nMode);
	int		GetSleepModeStep();
	void	SetSleepStarted(time_t time);
	time_t  GetSleepStarted();
	void	SetSleepElapsed(time_t time);
	time_t  GetSleepElapsed();

	//Loto Mode
	void SetLotoMode(BOOL b);
	BOOL GetLotoMode();
	//================================================
	// Stop Switch CHeck
	void SetStopSwitchPushed(BOOL bFlag);
	BOOL GetStopSwitchPushed();
	BOOL m_bFlag_StopSwitchPushed;

	BOOL m_bFlag_AreaStop_CancelMode;
	void SetAreaStop_CancelPush(BOOL bFlag);
	BOOL GetAreaStop_CancelPush();

	BOOL m_bFlag_AreaStop_OkMode;
	void SetAreaStop_OkPush(BOOL bFlag);
	BOOL GetAreaStop_OkPush();
	//================================================
	// ProbePin CYlinder
	void SetProbePinCylinder_UpDown(BOOL bFlag);


	//메인 Flow UI관련
	void SetMainUiFlag_CurrentPos(int nPos);
	BOOL GetMainUiFlag_CurrentPos(int nPos);
	BOOL m_nMainUiFlag_Refresh;
	BOOL m_bStopFlag_MissStart;
	BOOL m_bMainUiFlag_CurrentAction_Old[3];
	BOOL m_bMainUiFlag_StepView_Old[19];
	BOOL m_bMainUiFlag_MoveSocket_Old[4];
	int  m_nMainUiFlag_InspectResult[2];  
	
	BOOL m_bMainUiFlag_CurrentAction[3];
	BOOL m_bMainUiFlag_StepView[19];
	BOOL m_bMainUiFlag_MoveSocket[4];
	int  m_nMainUiFlag_InspectResult_Old[2];
	//JYS 210930
	// 스타트 스위치 누르고 1초 미만이면 강제 정지 시키는 코드에 쓰는 변수들
	BOOL	m_bForcedStop_Under_1Sec;
	void	SetForcedStop_1Sec(BOOL bFlg);
	BOOL	GetForcedStop_1Sec();
	DWORD	m_dwForcedStop_Under_1Sec;

	// 바코드 입력시 동작하게 할지 말지 선택하는 변수
	BOOL	m_bAccept_ReciveBarcode;
	void	SetAccpet_ReciveBarcode(BOOL bFlag);
	BOOL	GetAccpet_ReciveBarcode();

	// 강제로 Stop 스위치 활성화 시키는 변수
	BOOL	m_bForced_SIdeBtn_Stop_On;
	void	SetForced_SideBtn_StopOn(BOOL bFlag);
	BOOL	GetFOrced_SideBtn_StopOn();

	//수율
	int m_nTotalCnt;
	int m_nOkCnt;
	int m_nNGCnt;
	BOOL m_bReplaceFlag;

	void SetClearCounts();
	
	void SetTotalCnt();
	void SetOKCnt();
	void SetNGCnt();
	void SetReplaceFlag(BOOL bState);
	
	int GetTotalCnt();
	int GetOKCnt();
	int GetNGCnt();
	BOOL GetReplaceFlag();

	BOOL m_bEMOAllInitNeedFlag;
	BOOL GetMOAllInitNeedFlag();
	void SetMOAllInitNeedFlag(BOOL bState);

	BOOL m_bEMODisableFlag;
	void SetEMODisable(BOOL bState);
	BOOL GetEMODisable();

	// operation mode
	void SetOperationMode(__in OperationMode mode);
	OperationMode GetOperationMode();

private:
	// operation mode
	OperationMode	m_OperationMode;
};