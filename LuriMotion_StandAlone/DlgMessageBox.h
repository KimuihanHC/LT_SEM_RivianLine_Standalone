#pragma once
#include "pch.h"
#include "resource.h"	
#include "VGBase/VGStatic.h"

#define BUTTON_OK						0x1
#define BUTTON_CANCEL					0x2
#define BUTTON_BUZZER_OFF				0x4
#define BUTTON_ALL						0x7

// CDlgMessageBox 대화 상자

class CDlgMessageBox : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgMessageBox)

public:
	CDlgMessageBox(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgMessageBox();

	// 대화 상자 데이터입니다.

	enum { IDD = IDD_DLG_MESSAGEBOX };

	enum EBtn { eBtnOk, eBtnCancel, eBtnBuzOff, eBtn_Max };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedBtn1();
	afx_msg void OnBnClickedBtn2();
	afx_msg void OnBnClickedBtn3();

private:
	CVGStatic		m_stTitle;
	CButton			m_ctrlButton[eBtn_Max];
	CString			m_strMessage;
	INT				m_nMode;
	INT				m_nButton;

public:
	void InitComponent_MessageBox();
	void ChangeComponent_MessageBox();
	void SetMsg(CString str, int nMode);


	afx_msg void OnBnClickedBtn4();
};
