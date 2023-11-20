#pragma once
#include "pch.h"
#include "resource.h"	
#include "VGbase/VGStatic.h"
#include "DlgLanguage.h"
#include "DlgAdmin.h"
#include "DlgTowerLamp.h"

// CDlgTitle 대화 상자

class CDlgTitle : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgTitle)

public:
	CDlgTitle(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgTitle();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_TITLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg  int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void InitComponent_Title();

public:
	CVGStatic	m_stTitleMchName;
	CVGStatic	m_stTitleMchName_Recipe;
	CVGStatic	m_stTitleView[6];
	CVGStatic	m_btnTitleToolbar[4];
	

	CString		m_strTimer;
	CString		m_strDate;
	CTime		m_t;
	BOOL		m_bDateFlag;		// 맨 처음에 Date를 읽어오는게 느려서 한번은 똑같이 진행하기 위해 만든 Flag;

	afx_msg void OnBnClickedBtnTitle1();
	afx_msg void OnBnClickedBtnTitle2();
	afx_msg void OnBnClickedBtnTitle3();

	//----------------------------------
	// 타워램프
	RUN_MODE	m_Mode_Old;
	UINT		m_bBlinkMode[3];			// 순서대로 .. 1:RED 2:Yellow 3:GREEN
	UINT		m_bBlinkBk[3];				// 순서대로 .. 1:RED 2:Yellow 3:GREEN
	UINT		m_bBlinkState[3];			// 깜박이는 상태 보여주기.

	//----------------------------------
	// 프로그램 꺼졌다 켜졋을 경우 자동으로 Recipe 변경하게 만들자.
	BOOL		m_bProgramStartFlag;
	//========================================
	BOOL        m_bRunTitle;
	
};
