#pragma once
#include "pch.h"
#include "resource.h"	
#include "VGBase/VGStatic.h"


// CDlgMessageBox 대화 상자

class CDlgMessageBox_Instance : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgMessageBox_Instance)
private:
	static CDlgMessageBox_Instance *m_pInstance;

public:
	static CDlgMessageBox_Instance *Get_Instance(CWnd *pParent = NULL);
	void Delete_Instance();

public:
	CDlgMessageBox_Instance( CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgMessageBox_Instance();

// 대화 상자 데이터입니다.

	enum { IDD = IDD_DLG_MESSAGEBOX_INSTANCE };


protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void InitComponent_MessageBox();

	afx_msg void ChangeComponent_MessageBox();
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

public:
	CString			m_strError;
	int				m_nMode;

	CVGStatic		m_stTitle;

	int				m_nReturn;
	void SetMsg(CString str, int nMode);

	afx_msg void OnBnClickedBtn1();
	afx_msg void OnBnClickedBtn2();
	afx_msg void OnBnClickedBtn3();
	afx_msg void OnBnClickedCancel();

	virtual void	OnCancel();

	DWORD		m_dwResultTimer;
	BOOL		m_bTouched;
	BOOL		m_bTest_comp;
};
