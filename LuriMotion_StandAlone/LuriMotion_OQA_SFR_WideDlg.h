
// LuriMotionUIDlg.h: 헤더 파일
#pragma once
#include "DlgMain.h"
#include "DlgSide.h"
#include "DlgTitle.h"
#include "DlgBottom.h"
#include "DlgMotorTeach.h"
#include "DlgIo.h"
#include "DlgComponent.h"
#include "DlgMessageBox.h"
#include "DlgMessageBox_Instance.h"
#include "DlgErrorSolution.h"
#include "DlgMaintence.h"

#include <afxwin.h>
#include "VGbase/VGStatic.h"

// CLuriMotionUIDlg 대화 상자
class CLuriMotion_OQA_SFR_WideDlg : public CDialogEx
{
// 생성입니다.
public:
	CLuriMotion_OQA_SFR_WideDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LuriMotion_OQA_SFR_Wide_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


	afx_msg LRESULT OnRecvBarcode(WPARAM wParam, LPARAM lParam);

	// 구현입니다.
protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	

	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	CCriticalSection m_critSec;
	// CVG 사용하기 위한 GDI 초기화
	Gdiplus::GdiplusStartupInput m_gdiplusStartupInput;
	ULONG_PTR m_gdiplusToken;

public:
	CDlgMain*					m_pDlgMain;
	CDlgSide*					m_pDlgSide;
	CDlgBottom*					m_pDlgBtm;
	CDlgTitle*					m_pDlgTitle;
	
	CDlgMotorTeach*				m_pDlgTeach;
	CDlgIo*						m_pDlgIO;
	CDlgComponent*				m_pDlgComponent;
	CDlgErrorSolution*			m_pDlgErrSolution;
	CDlgMaintence*				m_pDlgMaintence;

	CDlgMessageBox_Instance*		m_pMsgBox_Instance;

	afx_msg void CreateDlg();
	afx_msg void Dlg_MovePosition();

	afx_msg void OnClose();

	afx_msg void ChangeMainUi(int nPart);
	afx_msg void RecipeChange_Ui_Refresh();

public:
	CDlgMessageBox	m_DlgMessage;
	afx_msg BOOL RunMessageDlg_Domodal(CString strInfo, int nMode);

	afx_msg void RunMessageDlg_Instance(CString strInfo, int nMode);
	afx_msg void RunMessageDlg_Instance_Show();
	bool m_bMessageBlock;

public:
	void ChangeUi_ByLanguage();
	void ChangeUi_ByLevel();

public:
	afx_msg BOOL Connect_Tester();
	//afx_msg BOOL Connect_PowerSupply();

public:
	afx_msg void Hide_Program();

	LRESULT OnMessageWindow2Top	(WPARAM wParam, LPARAM lParam);

	void InitProgram();
	void InitDevice();
	void OnInitialize();

	bool FindModel(__in CString szBCR);
public:
	BOOL	m_bStartCompleate;

};
