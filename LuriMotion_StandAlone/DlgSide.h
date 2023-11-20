#pragma once
#include "pch.h"
#include "resource.h"	
#include "VGbase/VGStatic.h"
#include "DlgAxisHomeProgress.h"

// CDlgSide 대화 상자
#define TIMER_100_SWITCH_LAMPCHECK   100
#define TIMER_200_EMO_POWER_CHECK	 300
#define TIMER_300_CHANGE_MODE		 200

class CDlgSide : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSide)

public:
	CDlgSide(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgSide();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_SIDE };
	// [21.1018.9] Added
	enum enMenu
	{
		Menu_Auto = 0,
		Menu_Manual,
		Menu_Loto,
		Menu_Barcode,
		Menu_Inspect,
		Menu_DryRun,
		Menu_Null,
		Menu_Init,
		Menu_Max,
	};
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()

public:
	void InitUI_EqpType();
	afx_msg void InitComponent_Side();


public:
	CVGStatic	m_btnSideMenu[8];
	afx_msg void OnBnClickedBtnSideCommand1();
	afx_msg void OnBnClickedBtnSideCommand2();
	afx_msg void OnBnClickedBtnSideCommand3();
	afx_msg void OnBnClickedBtnSideCommand4();
	afx_msg void OnBnClickedBtnSideCommand5();
	afx_msg void OnBnClickedBtnSideCommand6();
	afx_msg void OnBnClickedBtnSideCommand7();
	afx_msg void OnBnClickedBtnSideCommand8();

	afx_msg void SetSIdeBarStartButton_LampOn();			// START 와 STOP은 서로 반대쪽은 꺼지게 만들자.
	afx_msg void SetSIdeBarStopButton_LampOn();				// START 와 STOP은 서로 반대쪽은 꺼지게 만들자.

	afx_msg void SetSIdeBarIntialButton_LampOn(int nOn);
	afx_msg void SetSIdeBarDryRunButton_LampOn(int nOn);

	void OnInitialize();

	void SetOperationMode(__in OperationMode mode);

	BOOL m_bFlagSampleInput_Block_Foced;
	BOOL m_bFlagSampleOutput_Block_Foced;
public:
	afx_msg void ChangeUI_Language();
	afx_msg BOOL Check_Start();
		
	BOOL	Check_Switch_Start();
	BOOL	Check_Switch_Stop();
	BOOL	Check_Switch_Init();
	__int64 m_tStartBtnPress;
	__int64 m_tStopBtnPress;
	__int64 m_tInitBtnPress;

	__int64 m_tTimeOutReadySignal;

	BOOL	m_MainAutoFlag;
	BOOL	m_MainManualFlag;
	BOOL	m_SideStopFlag;
	BOOL	m_bStartFlag;
	BOOL	m_bStopFlag;
	BOOL	m_bInitFlag;
	BOOL	m_bStartFlagIsActive;
	BOOL	m_bStopFlagIsActive;
	BOOL	m_bInitFlagIsActive;
	void	SetChangeMcgModeFlag(BOOL bState);

	BOOL	m_bFlagStartButtonLamp;
	
	__int64	m_tChangeModeWaitTime;
	BOOL	m_bChangeModeWaitTime;

	DWORD	m_dwTemp1;
	BOOL	m_bTemp1;
	// [21.1029.1]
	BOOL	m_bPowerStatus;
	BOOL	m_bEMOSignalCheck;

	void Lamp_EqpType	();
};


