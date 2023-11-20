#pragma once
#include "pch.h"
#include "resource.h"	
#include "VGBase/VGStatic.h"

// CDlgTowerLamp 대화 상자

class CDlgTowerLamp : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgTowerLamp)

public:
	CDlgTowerLamp(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgTowerLamp();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_TOWERLAMP };

protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void InitComponnet_TowerLamp();

	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL OnInitDialog();

public:
	CVGStatic	m_stTitle;
	CVGStatic	m_stTitle_Sub_Lamp[4];
	CVGStatic	m_stTitle_Sub_Mode[6];

	CVGStatic	m_btnTowerLamp_Setting[6][4];
	int			m_nTowerLampSetting[6][4];		// 실제 동작 변수 저장용
public:
	afx_msg void ChangeButtonStatus(int nMode, int nColor);
	afx_msg void Load_TowerLamp();

	afx_msg void OnBtnClickedGreen_Run();
	afx_msg void OnBtnClickedYellow_Run();
	afx_msg void OnBtnClickedRed_Run();
	afx_msg void OnBtnClickedBuzzer_Run();

	afx_msg void OnBtnClickedGreen_Ready();
	afx_msg void OnBtnClickedYellow_Ready();
	afx_msg void OnBtnClickedRed_Ready();
	afx_msg void OnBtnClickedBuzzer_Ready();

	afx_msg void OnBtnClickedGreen_Error();
	afx_msg void OnBtnClickedYellow_Error();
	afx_msg void OnBtnClickedRed_Error();
	afx_msg void OnBtnClickedBuzzer_Error();

	afx_msg void OnBtnClickedGreen_Stop();
	afx_msg void OnBtnClickedYellow_Stop();
	afx_msg void OnBtnClickedRed_Stop();
	afx_msg void OnBtnClickedBuzzer_Stop();

	afx_msg void OnBtnClickedGreen_Call();
	afx_msg void OnBtnClickedYellow_Call();
	afx_msg void OnBtnClickedRed_Call();
	afx_msg void OnBtnClickedBuzzer_Call();

	afx_msg void OnBtnClickedGreen_Pause();
	afx_msg void OnBtnClickedYellow_Pause();
	afx_msg void OnBtnClickedRed_Pause();
	afx_msg void OnBtnClickedBuzzer_Pause();

	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
};
