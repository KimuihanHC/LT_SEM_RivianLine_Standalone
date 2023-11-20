#pragma once
#include "pch.h"
#include "resource.h"	
#include "VGbase/VGStatic.h"

// CDlgIo 대화 상자

class CDlgIo : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgIo)

public:
	CDlgIo(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgIo();

// 대화 상자 데이터입니다.

	enum { IDD = IDD_DLG_DIGITAL_IO };


protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

	afx_msg void OnTimer(UINT_PTR nIDEvent);

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void InitComponent_DigitalIO();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL OnInitDialog();

public:

	afx_msg void OnChkOutClick(UINT nID);	
	
public:
	// UI Init
	CVGStatic	m_stIo_Title[2];				// INPUT,OUTPUT Static
	CVGStatic	m_stIo_InputNum[32];			// Input Number Ui
	CVGStatic	m_stIo_InPutName[32];			// Input Name UI
	CVGStatic	m_stIo_OuputNum[32];			// Output Number UI
	CVGStatic	m_stIo_OuputName[32];			// Output Name UI
	CMFCButton  m_btnIo_OutputActive[32];		// Output Test Button UI

	CMFCButton	m_btn_IoPageMove[4];				// Input Output Back front 이동 버튼.

private:
	int			m_nSel_DIO_Input;							// 현재 몇번째 Input IO view인지 설정용 
	int			m_nSel_DIO_Input_Old;						// TImer에서 사용하기 위한 Flag

	int			m_nSel_DIO_Output;					// 현재 몇번째 Output IO view인지 설정용 
	int			m_nSel_DIO_Output_Old;				// Timer에서 사용하기 위한 Flag
	int			m_nOutputBtn_Status[1][32];			// Output Io 버튼이 CButton 속성이라 현재 상태 저장용 변수를 가지고 있어야 한다.

	DWORD		m_dwInput_Old[3];
	DWORD		m_dwOutput_Old[3];

	
public:
	afx_msg void OnBnClickedIoBack1();
	afx_msg void OnBnClickedIoBack2();
	afx_msg void OnBnClickedIoBack3();
	afx_msg void OnBnClickedIoBack4();


	afx_msg void Set_Disp_Input(void);
	afx_msg void Set_Disp_Output(void);

	afx_msg void ChangeUI_Language();
};
