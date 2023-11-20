#pragma once
#include "pch.h"
#include "resource.h"	
#include "VGbase/VGStatic.h"

// CDlgErrorSolution 대화 상자

class CDlgErrorSolution : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgErrorSolution)

public:
	CDlgErrorSolution(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgErrorSolution();

// 대화 상자 데이터입니다.

	enum { IDD = IDD_DLG_ERROR_SOL };


protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void InitComponent_ErrSolution();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL OnInitDialog();

	afx_msg void SetComboBox_Date(CComboBox *pBox, int Data);
public:
	CVGStatic		m_stErrorTitle[3];
	CVGStatic		m_stErrorSubTitle[5];

	CVGStatic		m_stErrorCnt[10];
	CVGStatic		m_StErrorNumber[10];
	CVGStatic		m_StErrorName[10];
	CVGStatic		m_btnErrorSolution[10];

	CVGStatic		m_stListPageMove[2];
	CVGStatic		m_btnListPartSort[3];

	CVGStatic		m_stDate[3];
	CComboBox		m_cbDate[3];

	CListCtrl		m_List_ErrorList_Past;

public:
	afx_msg void ChangeUI_Language();
	afx_msg void InitList();

	void	ShowErrorList();
	int		m_nCurrentPageCount;

	afx_msg void OnbtnClickedSolution_0();
	afx_msg void OnbtnClickedSolution_1();
	afx_msg void OnbtnClickedSolution_2();
	afx_msg void OnbtnClickedSolution_3();
	afx_msg void OnbtnClickedSolution_4();
	afx_msg void OnbtnClickedSolution_5();
	afx_msg void OnbtnClickedSolution_6();
	afx_msg void OnbtnClickedSolution_7();
	afx_msg void OnbtnClickedSolution_8();
	afx_msg void OnbtnClickedSolution_9();

	afx_msg void ChangeUi_Solution(int nErrNum);
	afx_msg void ChangeUi_Solution_BtnUi(int nBtnNum);
	BOOL	m_bSolutionBtnFlag[10];
	afx_msg void OnStnClickedStcErrorSolView1();
	afx_msg void OnStnClickedStcErrorSolView2();

	afx_msg void OnBnClickedBtnErrorSolPart1();
	afx_msg void OnBnClickedBtnErrorSolPart2();
	afx_msg void OnBnClickedBtnErrorSolPart3();
	
	int		m_nPartErrorNum_1[100];
	int		m_nPartErrorCnt_1;

	int		m_nPartErrorNum_2[100];
	int		m_nPartErrorCnt_2;

	int		m_nPartErrorNum_3[100];
	int		m_nPartErrorCnt_3;
};
