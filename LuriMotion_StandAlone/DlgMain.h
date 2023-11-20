#pragma once
#include "pch.h"
#include "resource.h"	
#include "VGBase/VGStatic.h"
#include "DlgLog.h"
#include <Psapi.h>

// CDlgMain 대화 상자
#define MAIN_TIMER_TOWERLAMP					100
#define MAIN_TIMER_MOTOR_STATUS					200
#define MAIN_TIMER_POWERSUPPLY					300

class CDlgMain : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgMain)

private:
	static CDlgMain *m_pInstance;


	HANDLE	m_hOpenTester;
	LONG	m_lCount;

public:
	static CDlgMain *Get_Instance(CWnd *pParent = NULL);
	void Delete_Instance();

public:
	CDlgMain(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgMain();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_MAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void InitUIEqpType();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void InitComponent_Main();
	afx_msg void ChangeUI_Language();
	afx_msg void ChangeDectectedUI_Status();
	afx_msg void Read_PowerSupply();
	afx_msg void RunStatus_Monitoring();

	BOOL ShowError	(__in enMCErrCode err);

	CDlgLog	 m_MainLog;
	CDlgLog	 m_CommLog;
	CDlgLog	 m_BarcodeLog;
	CDlgLog	 m_SequenceLog;

public:
	// Log
	void Write_Main_Log(CString strLog);
	void Write_Comm_Log(CString strLog);
	void Write_Sequence_Log(CString strLog);
	void Write_Barcode_Log(CString strLog);

public:
	// Timer
	BOOL	m_bMainStatus[MOTOR_MAX_COUNT];
	void	GetCurrentPosition();
	void	ReadMotorStatus_Ajin();
	void	ReadMotorStatus_Fastech();
	BOOL	IsCameraExistOrSocektClosed();

	// Proc Find
	//BOOL	getStateProcess(CString name);
	void	UpdateSemcoVer();
	BOOL	IsFindProcess(CString szExt, CString szPath);
	BOOL	bSelectCam;		//TRUE = 180, FALSE = 83
	int		nSelAngle;		//0 = H, 1 = V

	LRESULT SetMainUIStatusChange_Home(WPARAM wParam, LPARAM lParam);
	LRESULT SetMainUIStatusChange_Limit(WPARAM wParam, LPARAM lParam);
	LRESULT SetMainUIStatusChange_AxisAlarm(WPARAM wParam, LPARAM lParam);
	LRESULT SetMainUIStatusChange_InMotion(WPARAM wParam, LPARAM lParam);

	void HideStartUp();
	void ShowStartUp();
	void Startup_Start			(__in UINT nIdx, __in int nCapture = 0);
	void Startup_End			(__in UINT nIdx, __in bool bResult);
	void SetEnable_MoveCapture	(__in bool enable);

	BOOL m_bMotorHome_Done[MOTOR_MAX_COUNT];
	BOOL m_bMotorHome_Done_Old[MOTOR_MAX_COUNT];

	BOOL m_bMotorLimit_P[MOTOR_MAX_COUNT];
	BOOL m_bMotorLimit_M[MOTOR_MAX_COUNT];

	BOOL m_bMotorAlarm[MOTOR_MAX_COUNT];
	BOOL m_bMotorInMotion[MOTOR_MAX_COUNT];

public:
	//UI
	CVGStatic			m_stMainMotor_Title[7];						// 메인화면 모터 상태 UI의 title
	CVGStatic			m_stMainMotor_Number[6];					// 메인화면 모터 상태 UI의 모터넘버
	CVGStatic			m_StMainMotor_Name[6];						// 메인화면 모터 상태 UI의 모터이름
	CVGStatic			m_stMainMotor_Pos[6];						// 메인화면 모터 상태 UI의 현재위치
	CVGStatic			m_stMainMotor_Home[6];						// 메인화면 모터 상태 UI의 HOME 상태
	CVGStatic			m_stMainMotor_Limit_P[6];					// 메인화면 모터 상태 UI의 + 리밋상태
	CVGStatic			m_stMainMotor_Limit_M[6];					// 메인화면 모터 상태 UI의 - 리밋상태
	CVGStatic			m_stMainMotor_Error[6];						// 메인화면 모터 상태 UI의 에러 상태
	CVGStatic			m_stMainMotor_InMotion[6];					// 메인화면 모터 상태 UI의 동작중 확인 상태


	CVGStatic			m_stMainComm_Status[2];						// 메인화면 통신연결 상태 확인.
	CVGStatic			m_stMainComm_LanSerial[5][2];				// Lan/Serial 통신 연결상태 확인.
	CVGStatic			m_stMainComm_IO_Input_Status[3][2];			// 앞장비와 IO 통신 상태 확인
	CVGStatic			m_stMainComm_IO_Output_Status[3][2];		// 뒷장비와 IO 통신 상태 확인


	CVGStatic			m_stMainYield_Title;						// Yield 타이틀
	CVGStatic			m_stMainYield_Title_Sub[4][2];				// Yield Sub 타이틀 및 데이터 
	CVGStatic			m_btnMainYield_Reset;						// Yield Reset button
	
	CVGStatic			m_btnMainStartUpMode[8];						// 시업점검 button

	CVGStatic			m_stMainBarcode_Title;						// Barcode_ 타이틀
	CVGStatic			m_stMainBarcode_Title2;						// Tester Comm
	CVGStatic			m_stMainBarcode_Title3;						// Tester Comm

	CVGStatic			m_stPowerSupplyInfo[2];						// Power Supply Volt, Current;

	CVGStatic			m_stMainUI_ZoneView[3];
	CVGStatic			m_stMainUI_MovingView[4];
	CVGStatic			m_stMainUI_StepView[19];

	CVGStatic			m_stFN300Light_Title;						// FN300 Light
	CVGStatic			m_stFN300Light_Status[2];					// FN300 Light

	CMFCButton			m_btnMainFN300Light[2];						// FN300 Light

	CEdit				m_edtFileName[4];
	CVGStatic			m_sttFileProc[4];

	CVGStatic			m_stFileName[4];
	CVGStatic			m_stComponent_Title;
	CVGStatic			m_stComponent_subTitle[2];
	CVGStatic			m_stComponent_Num[4];

protected:
	CFont				m_font_Normal;

public:
	//--------------------------------------------------------------------------------------
	// Tower Lamp
	//afx_msg void SetTowerLamp(int nMode);
	int		m_nCurrentMode;
	int		m_nLastMode;
	BOOL	m_bIsTower_Blink;
	int		m_nBtnUiMode;

	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedBtnYieldReset();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();


public:
	void MchState_View();

	BOOL	m_bMchState_View_1;
	BOOL	m_bMchState_View_2;
	BOOL	m_bMchState_View_3;
	BOOL    m_bMchState_View_4;
	BOOL	m_bMchState_View_5;

	BOOL	m_bChangeUIReplaceFlag;
	int		m_nPowerSupplyReadMode;

	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton10();
	afx_msg void OnBnClickedButton11();
	afx_msg void OnBnClickedButton12();
	afx_msg void ChangeDayProduct();
	afx_msg void OnStnDblclickStcLogMain();
	afx_msg void OnStnDblclickStcLogComm();
	afx_msg void OnStnDblclickStcLogSeq();
	afx_msg void OnBnClickedButtonClearlog1();
	afx_msg void OnBnClickedButtonClearlog2();
	afx_msg void OnBnClickedButtonClearlog3();
	afx_msg void OnBnClickedBtnStartupMode1();
	afx_msg void OnBnClickedBtnStartupMode2();
	afx_msg void OnBnClickedBtnStartupMode3();
	afx_msg void OnBnClickedButtonFn300_L1();
	afx_msg void OnBnClickedButtonFn300_L2();
	//afx_msg void OnBnClickedBtnProcFind();
	afx_msg void OnBnClickedBtnDoorLock();
	afx_msg void OnBnClickedBtnStartupMode4();
	afx_msg void OnBnClickedBtnStartupMode5();
	afx_msg void OnBnClickedBtnStartupMode6();
	afx_msg void OnBnClickedBtnStartupMode7();
	afx_msg void OnBnClickedBtnStartupMode8();
};
