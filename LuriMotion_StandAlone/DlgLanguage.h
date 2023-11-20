#pragma once
#include "pch.h"
#include "resource.h"	
#include "VGBase/VGStatic.h"

// CDlgLanguage 대화 상자

class CDlgLanguage : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgLanguage)

public:
	CDlgLanguage(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgLanguage();

// 대화 상자 데이터입니다.

	enum { IDD = IDD_DLG_LANGUAGE_2};


protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void InitComponnet_Language();

	virtual BOOL OnInitDialog();

public:
	CVGStatic		m_stTitle;
	CVGStatic		m_btnSelectLanguage[4];

	CVGStatic		m_btnOk;

	int				m_nLanguageNum;

	int				m_nChangeLanguage;
	afx_msg int  GetChangeLanguage();

	afx_msg void OnBnClickedBtnOk();
	afx_msg void OnBnClickedBtnSelectLanguage1();
	afx_msg void OnBnClickedBtnSelectLanguage2();
	afx_msg void OnBnClickedBtnSelectLanguage3();
	afx_msg void OnBnClickedBtnSelectLanguage4();

	afx_msg void ChangeUi_Language();
};
