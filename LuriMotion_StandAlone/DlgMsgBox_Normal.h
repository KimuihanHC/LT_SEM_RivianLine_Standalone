#pragma once
#include "pch.h"
#include "resource.h"	
#include "VGbase/VGStatic.h"

// CDlgMsgBox_Normal 대화 상자

class CDlgMsgBox_Normal : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgMsgBox_Normal)

public:
	CDlgMsgBox_Normal(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgMsgBox_Normal();

// 대화 상자 데이터입니다.

	enum { IDD = IDD_DLG_MSGBOX_NORMAL };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()

private:
	CString		m_strMsg;
	int			m_nMsgMode;

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButton1();

	void InitComponent_MSgBoxDlg(int nType, CString strMsg);
	CVGStatic		m_stMsg;

	void SetMsg(CString str, int mode);
};
