#pragma once
#include "pch.h"
#include "VGbase/VGStatic.h"

// CDlgAxisHomeProgress 대화 상자

class CDlgAxisHomeProgress : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgAxisHomeProgress)

public:
	CDlgAxisHomeProgress( CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgAxisHomeProgress();

// 대화 상자 데이터입니다.

	enum { IDD = IDD_DLG_MOTOR_HOME_RATE };


protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void InitComponent_AxisMoveHome(BYTE nAxis, int nAxis_Z1 = -1, int nAxis_Z2  = -1, int nAxis_Z3 = -1 ); //총 모터수, Z축 번호1, Z축 번호2, Z축 번호3

public:
	CProgressCtrl m_AxisHomeProgress;
	CVGStatic	  m_stTitle;

	BOOL		  m_bMoveHome;
	int			  m_nInitRate;
	int			  m_nAxisNum;

	BOOL		  m_bHomeFlag[MOTOR_MAX_COUNT];
	int			  m_nTemp;

	//모터초기화
	__int64 m_InitialTime;
	int m_nZAxisNum[3];
	int m_nAxisY;
	int m_HomeCompliate;
	afx_msg void OnBnClickedButtonMotorOriginall();
	afx_msg void OnBnClickedButtonMotorStop();
	afx_msg void OnBnClickedButtonMotorOriginY();
	afx_msg void OnBnClickedButtonMotorOriginZ();

	BOOL m_bFlag_InitStop;
	CString m_strInitial;

	// 개별축 초기화 사용 변수들
	int nManualInitial_Axis;
	int m_nEachStep;
	DWORD m_dwEachStep_Init;
	afx_msg void OnBnClickedButtonMotorStop2();

	// 전체축 초기화 사용 변수들
	int m_nAllAxis_InitStep;
	__int64 m_dwAllAxis_Init;
	BOOL m_bAllAxis_CheckFlag;
};
