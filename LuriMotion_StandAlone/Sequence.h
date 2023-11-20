#pragma once

#include "SeqBase.h"
#include "User_Define.h"

#include "Def_ErrorCode.h"

#include "Def_Structure.h"

#include "Ui.h"

typedef struct
{
	LPVOID		pOwner;
	UINT		nPort;
	UINT		nArg;
}stThreadParam;

class CSequence : public CWnd, public CSeqBase, public CUi
{
public:
	CSequence();
	~CSequence();

	enum ETHREAD_ID
	{
		THREAD_RUN = 0,
		THREAD_MANUAL,
		THREAD_INITIAL,
		THREAD_INTLK,
		THREAD_IO,
		THREAD_MAX
	};

protected:
	BOOL		m_bThreadRun;
	CWinThread	*m_pThreadRun;
	BOOL		m_bThreadRunStop;
	CWinThread	*m_pThreadRunStop;
	static UINT	Thread_Run(LPVOID lpParam);
	static UINT	Thread_RunStop(LPVOID lpParam);

	enMCErrCode Equipment_EMO();
	enMCErrCode Equipment_PowerOff();
	enMCErrCode Equipment_MainAir();
	enMCErrCode Equipment_Door();
	enMCErrCode Equipment_Initialize();
	enMCErrCode Equipment_AreaSensor();

public:
//	BOOL	m_bSeqError[10];					// PART 별로 사용하기 위해 변수 10개 입력

protected:
	int		m_nPreviewRetry;
	int		m_nErrorNumber[10];					// 에러번호를 저장하고 솔루션을 따로 불러오게 하자.

	CString	m_strMsg;
	DWORD	m_dwTimer_StandAlone;
	DWORD	m_dwTimer_Preview;

	enMCErrCode	Run_SFR_Inspection();

	int		m_nStep_SFR_MoveWaitPos;
	DWORD	m_dwTimer_SFR_MoveWaitPos;
	BOOL	Run_SFR_MoveWaitPos();
	
	void OnDetectIoSingal(__in int port, __in bool bOn);

public:
	BOOL		m_bThread[THREAD_MAX];
	CWinThread	*m_pThread[THREAD_MAX];

	bool Begin_Thread(ETHREAD_ID tid);
	void End_Thread(ETHREAD_ID tid);

	bool Begin_RunThread();
	void End_RunThread();	
//	bool Begin_RunThreadStop();
	void End_RunThreadStop();
	   
	void Start_Inlk();
	void Start_MonitoringIO();

	RUN_MODE	m_bRunMode;
	void Start_Run();
	void Stop_Run();
	void Stop_Run_nonNormal();
	void Pause_Run();

	// JYS 210923 TEST3.
	void AutoMode_Run();
	void ManualMode_Run();	

	void Emergency_Run();
	void Init_Compleate();
	void Reset_SeqFlags();

	RUN_MODE	GetRunMode();
	void		SetRunMode(RUN_MODE rm);

	BOOL Run_Replay();
	BOOL Run_IO();
	BOOL Run_Equipment();

	BOOL Get_ThreadRun(__in ETHREAD_ID Thd) { return m_bThread[Thd]; }

public:
	// [21.1202.1]
	bool m_bProgramFirstStart;
	enEMOStep	m_enEMOStep;

public:
	BOOL	m_bFlagProcess_NormalFinish;			// 시퀀스에서 장비를 정지 시키는데 혹시라도 다시 시작할때 정상 시퀀스가 아닐수도 있어서 추가. TEST 210716
	BOOL    m_bFlagProcess_AreaStopFinish;
	BOOL    m_bFlagProcess_AReaStopLampnSide;
	void	SetAreaStopLampnSideFlag(BOOL bState);
	BOOL	GetAreaStopLampnSideFlag();
	BOOL	m_bStopSwitchPushed;					// 삼성전기에서 제공해준시퀀스에 사용 스탑버튼을 눌러야 SFR 검사이후 빠져나오게 한다.
	void	SetStopswitchMode(BOOL bMode);
	BOOL    GetStopSwtichMode();
	
	void		CameraInit();//james test

	void		Start_StartupCheck				(__in int nIdx, __in int nCaptureIdx = 0);
	void		Stop_StartupCheck				();

	virtual void		Initialize_Startup			(__in UINT nIdx, __in int nCapture = 0) = 0;
	virtual void		Finalize_Startup			(__in enMCErrCode err, __in int nIdx, __in int nCapture = 0) = 0;

	bool				 IsTesting_StartupCheck		();
	inline			bool IsThread_StartupCheck		()
	{
		return m_bThreadStartupCheck;	
	};

	UINT	Run_StartupCheck					();
public:
	void	SetSFR_Inspection_Step();
	void	Clear_MainUiFlag();

	virtual UINT    Maintence					(__in enMaintFunId id) = 0;

	virtual UINT	StartOperation_Startup		() = 0;
	virtual UINT	StartOperation_Run			() = 0;
	virtual void	SetInspectionStep			() = 0;
	virtual void	RestartStep					() = 0;

	virtual bool	IsStep_UnderMoveY			() = 0;
	virtual bool	IsStep_BeforePushStopSwitch	() = 0;
	virtual bool	IsStep_NotStartupModeWait	() = 0;

	virtual void	ResetStepFlag				() = 0;

	virtual bool	ReadyToStart				() = 0;

protected:
	static UINT Thread_MonitoringIo(LPVOID lpParam);
	static UINT Thread_Inlk(LPVOID lpVoid);

	static UINT Thread_StartupCheck(__in LPVOID lParam);
	
public:
	DWORD	m_dwInspection_CompleateTime;
	__int64  m_tAlarmTimer;

	bool m_bFirstIo;
	bool m_bOldIoInput[100];

	// 시업점검 모드
	HANDLE					m_hThreadStartupCheck;
	bool					m_bThreadStartupCheck;

	ST_InspectionInfo		m_stInspectionInfo;
};