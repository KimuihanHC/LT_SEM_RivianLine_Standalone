#pragma once
#include "afxcmn.h"

// CDlgLog 대화 상자

class CDlgLog : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgLog)

public:
	CDlgLog(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgLog();

// 대화 상자 데이터입니다.

	enum { IDD = IDD_DLG_LOG };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	int		m_nWidth;
	int		m_nLogMode;

private:
	CFont	m_font_Default;

public:
	void ListClear();
	void ListPrint(LPCTSTR lpszText);

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listLog;
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void SetWidth(int nWidth);
	afx_msg void SetMode(int nMode = 0);

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

};
