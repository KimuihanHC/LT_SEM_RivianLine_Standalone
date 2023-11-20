#pragma once
#include "pch.h"
#include "resource.h"	
#include "VGbase/VGStatic.h"

// CDlgNumPad 대화 상자

class CDlgNumPad : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgNumPad)

public:
	CDlgNumPad(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgNumPad();

// 대화 상자 데이터입니다.

	enum { IDD = IDD_DLG_NUMPAD };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);


private:
	CVGStatic	m_stNumPos[12];
	
	int			m_nCursorPos;
	CString		m_strPanelBit[12];
	int			m_nNumLength;
	double		m_dCalValue;
	double		m_dMaxLimitValue;
	double		m_dMinLimitValue;

	CButton	m_ctrlBtnOk;
	CButton	m_ctrlBtnCancel;
	CButton	m_ctrlBtnClear;

public:
	void InitComponent_NumPad();

	CString GetCurrentNum();

	double GetValue();
	void SetValue		(double dValue);
	void SetValue		(int nValue);
	void SetLimitValue	(double dMinValue, double dMaxValue);

	void DisplayNum();
	void SetCursorColor(int nNo, Gdiplus::Color clrBack);
	void AddBit(CString strBit);
	void BackspaceBit();
	void DeleteBit();

	afx_msg void ClickOk();
	afx_msg void ClickClose();

	afx_msg void OnBnClickedBtnNum0();
	afx_msg void OnBnClickedBtnNum1();
	afx_msg void OnBnClickedBtnNum2();
	afx_msg void OnBnClickedBtnNum3();
	afx_msg void OnBnClickedBtnNum4();
	afx_msg void OnBnClickedBtnNum5();
	afx_msg void OnBnClickedBtnNum6();
	afx_msg void OnBnClickedBtnNum7();
	afx_msg void OnBnClickedBtnNum8();
	afx_msg void OnBnClickedBtnNum9();

	afx_msg void OnBnClickedBtnBack();
	afx_msg void OnBnClickedBtnDel();
	afx_msg void OnBnClickedBtnCursorLeft();
	afx_msg void OnBnClickedBtnCursorRight();
	afx_msg void OnBnClickedBtnDot();
	afx_msg void OnBnClickedBtnSign();
	afx_msg void OnBnClickedBtnClear();
	afx_msg void OnBnClickedBtnCancel();
	afx_msg void OnBnClickedBtnOk();

};
