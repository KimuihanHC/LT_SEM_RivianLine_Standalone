#pragma once

#include "VGbase/VGStatic.h"

// CDlgMaintence 대화 상자
class CDlgMaintence : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgMaintence)

public:
	CDlgMaintence(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgMaintence();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_MAINTENCE };

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnMaintCylOn();
	afx_msg void OnBnClickedBtnMaintCylOff();
	afx_msg void OnBnClickedBtnMaintCylOn2();
	afx_msg void OnBnClickedBtnMaintCylOff2();
	afx_msg void OnBnClickedBtnMaintCylOn3();
	afx_msg void OnBnClickedBtnMaintCylOff3();
	afx_msg void OnBnClickedBtnMaintCylOn4();
	afx_msg void OnBnClickedBtnMaintCylOff4();
	afx_msg void OnBnClickedBtnMaintAutoLoad();
	afx_msg void OnBnClickedBtnMaintAutoUnload();
	virtual BOOL OnInitDialog();
	void InitUIComponent();

	void InitUI_EqpType();

private:
	CVGStatic m_stTitle[2];
	CVGStatic m_stManual[4];

};
