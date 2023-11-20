#pragma once
#include "pch.h"
#include "resource.h"	
#include "VGBase/VGStatic.h"

// CDlgAdmin 대화 상자

class CDlgAdmin : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgAdmin)

public:
	CDlgAdmin(int nMode, CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgAdmin();

// 대화 상자 데이터입니다.

	enum { IDD = IDD_DLG_ADMIN };

	enum { BTN_OPER1, BTN_MAINT1, BTN_ADMIN1, BTN_OPER2, BTN_MAINT2, BTN_ADMIN2, MAX_ITEM};
	enum { BTN_APPLY, BTN_QUIT, BTN_ADD, BTN_DEL , MAX_EXE};
	enum { TITLE_ID1, TITLE_PW1, TITLE_ID2, TITLE_PW2, MAX_TITLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);


	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMClickedListView(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnMode1();
	afx_msg void OnBnClickedBtnMode2();
	afx_msg void OnBnClickedBtnMode3();
	afx_msg void OnBnClickedBtnMode4();
	afx_msg void OnBnClickedBtnMode5();
	afx_msg void OnBnClickedBtnMode6();
	afx_msg void OnBnClickedBtnOk1();
	afx_msg void OnBnClickedBtnOk2();
	afx_msg void OnBnClickedBtnOk3();
	afx_msg void OnBnClickedBtnOk4();

	void InitComponnet_Admin	(int nLevel);
	void InitView_List			();
	void DisplayByLevel			(int nLevel);
	void Disable_AddPermission	(int nLevel);
	void DisplayBtnLevel		(int nMask , int nLanguage);
	void DisplayBtnOk			(int nLanguage);
	void DisplayTitle			(int nLanguage);

protected:
	CFont			m_font_Large;
	CFont			m_font_Default;
	CFont			m_font_List;

	CString			m_strAdminPw;

public:
	CVGStatic	m_stTitle;
	CVGStatic	m_stTitle_Add;
	CVGStatic	m_stTitle_Sub[MAX_TITLE];
	CVGStatic	m_btnUserMode[MAX_ITEM];
	CVGStatic	m_btnChangeOK[MAX_EXE];

	CEdit		m_edPw;
	CEdit		m_edId;
	CEdit		m_edPw_add;
	CEdit		m_edId_add;

	CListCtrl	m_ListView;

	int		m_nSelectAcessMode;

	int		m_nSavePermission;
	BOOL	m_bChangeFlag;						// title DLg 에서 사용할 flag;

	BOOL CheckIDandPassword		(__out int & nLevel);					// 운영,관리자 권한 설정에 필요한 함수
	BOOL GetPassword			(__in CString id, __out CString & pw) const;

	BOOL Save_Delete_Pemission(int nMode);
	int  ListboxToEditbox();



	afx_msg BOOL GetChangFlag();
};
