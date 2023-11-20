#pragma once
#include "pch.h"
#include "resource.h"	
#include "VGBase/VGStatic.h"

// CDlgLotoMode 대화 상자

class CDlgLotoMode : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgLotoMode)

public:
	CDlgLotoMode(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgLotoMode();

// 대화 상자 데이터입니다.

	enum { IDD = IDD_DLG_LOTO_MODE };
	enum EStt { eSttPw1, eSttPw2 ,eStt_Max};
	enum EBtn { eBtnApply1, eBtnApply2, eBtnReturn, eBtn_Max };
	enum EEdt { eEdtPw1, eEdtPw2, eEdt_Max };

protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()


public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int	 OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();

	afx_msg void InitComponnet_LotoMode();

protected:
	CFont			m_font_Large;
	CFont			m_font_Default;
	CFont			m_font_Normal;

private:
	CMFCButton	m_Btn[eBtn_Max] ;
	CVGStatic	m_Stt[eStt_Max];
	CVGStatic	m_stLotoTitle;
	CEdit		m_Edt[eEdt_Max];

	CString		m_strPW[eEdt_Max];
	UINT		m_nFlickCount;

public:
	afx_msg void OnStnClickedLotoTitle1();
};
