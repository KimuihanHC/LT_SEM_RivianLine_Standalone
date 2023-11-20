#pragma once
#include "pch.h"
#include "resource.h"	
#include "VGbase/VGStatic.h"
#include "DlgMain.h"

// CDlgComponent 대화 상자

class CDlgComponent : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgComponent)

public:
	CDlgComponent(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgComponent();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_COMMUNICATION_SETTING };

protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void InitComponent_Component();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL OnInitDialog();

	void InitUI_EqpType();

public:
	// UI
	CVGStatic		m_stComponent_Title[7];					// lan & serial title 2개
	CVGStatic		m_stComponent_Lan_Sub[3];				// lan sub title 3개
	CVGStatic		m_stComponent_Lan_Ip[3];				// Ip static 3개
	CVGStatic		m_btnComponent_Lan_Server[2];			// Lan 통신 Server도 생성해야 되는지 확인하는 버튼.
	CVGStatic		m_btnComponent_Lan_Connect[3];			// Lan 통신 연결
	CVGStatic		m_btnComponent_Lan_DisConnect[3];		// Lan 통신 g해제

	CString			m_strComponent_Lan_Port[2];
	CEdit			m_edComponent_Lan_Port[2];

	CVGStatic		m_stComponent_Serial_Sub[6];
	CVGStatic		m_stComponent_Serial_Controller[5][3];	// Serial Comport 설정 UI
	CMFCButton		m_btnComponent_Serial_Connect[1];		// Serial 연결
	CMFCButton		m_btnComponent_Serial_DisConnect[1];	// Serial 해제
	CMFCButton		m_btnPower_Serial_Connect[1];		// Serial 연결
	CMFCButton		m_btnPower_Serial_DisConnect[1];	// Serial 해제


	CVGStatic		m_stComponent_Serial_InputData[9];		// Vol, Currnet 입력
	CVGStatic		m_stComponent_Serial_Channel[13];		// Channel
	CVGStatic		m_stComponent_Serial_1_Data[13];		// 1 조명 데이터
	CMFCButton		m_btnComponent_Serial_OnOff[13];		// OnOff btn
	CMFCButton		m_btnComponent_Serial_All[2];			// All btn

	CVGStatic		m_stComponent_Power_Volt;
	CVGStatic		m_stComponent_Power_Current;

	CEdit			m_edComponent_Power_Volt;
	CEdit			m_edComponent_Power_Current;

	// Tester
	CVGStatic		m_stComponent_Tester_No[MAX_TESTER_NUM];	
	CMFCEditBrowseCtrl	m_ebTesterPath[MAX_TESTER_NUM];
#if (GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL)
	CString	cTestN[8] = 
	{
		_T("180"),
		_T("180H"),
		_T("83H"),
		_T("83H"),
		_T("180V"),
		_T("180V"),
		_T("83V"),
		_T("83V") 
	};
#elif (GET_INSPECTOR == SYS_OQC_SFR_SINGLE_CL) || (GET_INSPECTOR == SYS_DISTORTION_30)
	CString	cTestN[8] =
	{
		_T("30"),
		_T("180H"),
		_T("83H"),
		_T("83H"),
		_T("180V"),
		_T("180V"),
		_T("83V"),
		_T("83V")
	};
#endif

public:
	// 컨트롤 연결
	CIPAddressCtrl	m_ipTester[2];
	CEdit			m_edPort[2];
	CVGStatic		m_btnSelectUse[2];

	CComboBox		m_cbLightController1_Port;
	CComboBox		m_cbLightController1_Baud;
	CComboBox		m_cbLightController1_DataBit;
	CComboBox		m_cbLightController1_Parity;
	CComboBox		m_cbLightController1_StopBit;

	CComboBox		m_cbLightController1_Light;

	CComboBox		m_cbPowerSupply1_Port;
	CComboBox		m_cbPowerSupply1_Baud;
	CComboBox		m_cbPowerSupply1_DataBit;
	CComboBox		m_cbPowerSupply1_Parity;
	CComboBox		m_cbPowerSupply1_StopBit;
	afx_msg void OnBnClickedBtnPowerConnect1();
	afx_msg void OnBnClickedBtnPowerDisconnect1();

	CComboBox		m_cbHandyBCR_Port;
	CComboBox		m_cbHandyBCR_Baud;
	CComboBox		m_cbHandyBCR_DataBit;
	CComboBox		m_cbHandyBCR_Parity;
	CComboBox		m_cbHandyBCR_StopBit;

	// 저장용 Data
	CString			m_strIpAddress[2];
	int				m_nTesterPort[2];
	DWORD			m_dwTesterIp[2];
	int				m_nHandlerServerMode[2];

	int				m_nSerial_1_Data[13];

	// FN300
	int				m_nLightSelected;

	afx_msg void ChangeUi_Lan();
	afx_msg void ChangeUi_Serial();
	afx_msg void ChangeUI_WaitList();
	afx_msg void ChangeUI_Tester();
	afx_msg void ChangeUI_Power();

	afx_msg void Load_ComponentDlg();

	afx_msg void OnBnClickedBtn1();

	afx_msg void SetComboBox_Serial(CComboBox *pBox, int Data);
	afx_msg void OnBnClickedBtnLanConnect1();
	afx_msg void OnBnClickedBtnDisconnect1();
	afx_msg void OnBnClickedBtnLanConnect2();
	afx_msg void OnBnClickedBtnDisconnect2();
	afx_msg void OnBnClickedBtnLanConnect3();
	afx_msg void OnBnClickedBtnDisconnect3();
	afx_msg void OnBnClickedBtnSerialConnect1();
	afx_msg void OnBnClickedBtnSerialDisconnect1();

	afx_msg void OnRangeInputData(UINT nID);

	// Language 선택
	afx_msg void ChangeUI_Language();

	// Client, Server Mode.
	afx_msg void OnBnClickedBtnConnectLanServerChk1();
	afx_msg void OnBnClickedBtnConnectLanServerChk2();
	afx_msg void OnRangeSerialOnoff(UINT nID);

	int m_ntestFlag;
	CListCtrl m_ListWaitTime;
	int m_nSelectColumn;
	afx_msg void OnNMDblclkListCommDelaylist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnSaveWiattimelist();
//	afx_msg void OnBnClickedBtnSerialConnect3();
//	afx_msg void OnBnClickedBtnSerialDisconnect3();
	afx_msg void OnCbnSelchangeComboSerial6();
	afx_msg void OnBnClickedButtonFnLightAllOn();
	afx_msg void OnBnClickedButtonFnLightAllOff();
	afx_msg void OnBnClickedBtnBcrConnect();
	afx_msg void OnBnClickedBtnBcrDisconnect();
	afx_msg void OnBnClickedBtnSerial1Onoff01();
};
