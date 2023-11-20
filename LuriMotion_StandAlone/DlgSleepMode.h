#pragma once
#include "pch.h"
#include "resource.h"	
#include "VGBase/VGStatic.h"

// CDlgSleepMode 대화 상자

class CDlgSleepMode : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSleepMode)

public:
	CDlgSleepMode(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgSleepMode();

	// 대화 상자 데이터입니다.

	enum { IDD = IDD_DLG_SLEEP_MODE };


protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void InitComponnet_SleepMode();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();


public:
	CVGStatic	m_stSleepTitle;
	CVGStatic	m_btnOk;
	afx_msg void OnBnClickedButton1();
};
