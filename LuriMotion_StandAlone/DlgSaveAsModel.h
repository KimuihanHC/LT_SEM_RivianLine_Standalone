#pragma once
#include "pch.h"
#include "resource.h"	
#include "VGBase/VGStatic.h"

// CDlgSaveAsModel 대화 상자

class CDlgSaveAsModel : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSaveAsModel)

public:
	CDlgSaveAsModel(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgSaveAsModel();

// 대화 상자 데이터입니다.

	enum { IDD = IDD_DLG_SAVEAS_MODEL };


protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);


	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void InitComponnet_SaveAs();

	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL OnInitDialog();

protected:
	CFont			m_font_Large;
	CFont			m_font_Default;

public:

	CVGStatic		m_stTitle;
	CEdit			m_edModelName;
	CString			m_strModelName;

	CVGStatic		m_btnOk;
	CVGStatic		m_btnCancel;

public:
	afx_msg BOOL	SaveAsModel(CString strName);

	afx_msg void	InitComponent_SaveAs_EditBox();
	afx_msg void OnBnClickedOk();
};
