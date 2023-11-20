// DlgComponent.cpp: 구현 파일
//

#include "pch.h"
#include "LuriMotion_OQA_SFR_Wide.h"
#include "DlgComponent.h"
#include "afxdialogex.h"
#include "DlgNumPad.h"

// CDlgComponent 대화 상자

IMPLEMENT_DYNAMIC(CDlgComponent, CDialogEx)

CDlgComponent::CDlgComponent(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_COMMUNICATION_SETTING, pParent)
{
	m_ntestFlag = 0;
	m_nLightSelected = 0;
}

CDlgComponent::~CDlgComponent()
{
}

void CDlgComponent::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STC_COMPONENT_TITLE_1, m_stComponent_Title[0]);
	DDX_Control(pDX, IDC_STC_COMPONENT_TITLE_2, m_stComponent_Title[1]);
	DDX_Control(pDX, IDC_STC_COMPONENT_TITLE_3, m_stComponent_Title[2]);
	DDX_Control(pDX, IDC_STC_COMPONENT_TITLE_4, m_stComponent_Title[3]);
	DDX_Control(pDX, IDC_STC_COMPONENT_TITLE_5, m_stComponent_Title[4]);
//	DDX_Control(pDX, IDC_STC_COMPONENT_TITLE_6, m_stComponent_Title[5]);
	DDX_Control(pDX, IDC_STC_COMPONENT_TITLE_8, m_stComponent_Title[5]);

	DDX_Control(pDX, IDC_STC_LAN_CONNECT_1, m_stComponent_Lan_Sub[0]);
	DDX_Control(pDX, IDC_STC_LAN_CONNECT_2, m_stComponent_Lan_Sub[1]);
	DDX_Control(pDX, IDC_STC_LAN_CONNECT_3, m_stComponent_Lan_Sub[2]);

	DDX_Control(pDX, IDC_STC_CONNECT_LAN_IP_1, m_stComponent_Lan_Ip[0]);
	DDX_Control(pDX, IDC_STC_CONNECT_LAN_IP_2, m_stComponent_Lan_Ip[1]);
	DDX_Control(pDX, IDC_STC_CONNECT_LAN_IP_3, m_stComponent_Lan_Ip[2]);

	DDX_Control(pDX, IDC_BTN_CONNECT_LAN_SERVER_CHK_1, m_btnComponent_Lan_Server[0]);
	DDX_Control(pDX, IDC_BTN_CONNECT_LAN_SERVER_CHK_2, m_btnComponent_Lan_Server[1]);

	DDX_Control(pDX, IDC_BTN_LAN_CONNECT_1, m_btnComponent_Lan_Connect[0]);
	DDX_Control(pDX, IDC_BTN_LAN_CONNECT_2, m_btnComponent_Lan_Connect[1]);
	DDX_Control(pDX, IDC_BTN_LAN_CONNECT_3, m_btnComponent_Lan_Connect[2]);

	DDX_Control(pDX, IDC_BTN_DISCONNECT_1, m_btnComponent_Lan_DisConnect[0]);
	DDX_Control(pDX, IDC_BTN_DISCONNECT_2, m_btnComponent_Lan_DisConnect[1]);
	DDX_Control(pDX, IDC_BTN_DISCONNECT_3, m_btnComponent_Lan_DisConnect[2]);

	DDX_Control(pDX, IDC_STC_SERIAL_CONNECT_TITLE_1, m_stComponent_Serial_Sub[0]);
	DDX_Control(pDX, IDC_STC_SERIAL_CONNECT_TITLE_2, m_stComponent_Serial_Sub[1]);
	DDX_Control(pDX, IDC_STC_SERIAL_CONNECT_TITLE_3, m_stComponent_Serial_Sub[2]);
	DDX_Control(pDX, IDC_STC_SERIAL_CONNECT_TITLE_7, m_stComponent_Serial_Sub[3]);
	
	DDX_Control(pDX, IDC_STC_SERIAL_CONNECT_01, m_stComponent_Serial_Controller[0][0]);
	DDX_Control(pDX, IDC_STC_SERIAL_CONNECT_02, m_stComponent_Serial_Controller[1][0]);
	DDX_Control(pDX, IDC_STC_SERIAL_CONNECT_03, m_stComponent_Serial_Controller[2][0]);
	DDX_Control(pDX, IDC_STC_SERIAL_CONNECT_04, m_stComponent_Serial_Controller[3][0]);
	DDX_Control(pDX, IDC_STC_SERIAL_CONNECT_05, m_stComponent_Serial_Controller[4][0]);

	DDX_Control(pDX, IDC_STC_SERIAL_CONNECT_16, m_stComponent_Serial_Controller[0][1]);
	DDX_Control(pDX, IDC_STC_SERIAL_CONNECT_17, m_stComponent_Serial_Controller[1][1]);
	DDX_Control(pDX, IDC_STC_SERIAL_CONNECT_18, m_stComponent_Serial_Controller[2][1]);
	DDX_Control(pDX, IDC_STC_SERIAL_CONNECT_19, m_stComponent_Serial_Controller[3][1]);
	DDX_Control(pDX, IDC_STC_SERIAL_CONNECT_20, m_stComponent_Serial_Controller[4][1]);

	DDX_Control(pDX, IDC_STC_SERIAL_CONNECT_21, m_stComponent_Serial_Controller[0][2]);
	DDX_Control(pDX, IDC_STC_SERIAL_CONNECT_22, m_stComponent_Serial_Controller[1][2]);
	DDX_Control(pDX, IDC_STC_SERIAL_CONNECT_23, m_stComponent_Serial_Controller[2][2]);
	DDX_Control(pDX, IDC_STC_SERIAL_CONNECT_24, m_stComponent_Serial_Controller[3][2]);
	DDX_Control(pDX, IDC_STC_SERIAL_CONNECT_25, m_stComponent_Serial_Controller[4][2]);

	DDX_Control(pDX, IDC_BTN_SERIAL_CONNECT_1, m_btnComponent_Serial_Connect[0]);
	DDX_Control(pDX, IDC_BTN_SERIAL_DISCONNECT_1, m_btnComponent_Serial_DisConnect[0]);
	DDX_Control(pDX, IDC_BTN_POWER_CONNECT_1, m_btnPower_Serial_Connect[0]);
	DDX_Control(pDX, IDC_BTN_POWER_DISCONNECT_1, m_btnPower_Serial_DisConnect[0]);

	DDX_Control(pDX, IDC_STC_SERIAL_LIGHT_CONTROLLER_1, m_stComponent_Serial_InputData[0]);
	DDX_Control(pDX, IDC_STC_SERIAL_LIGHT_CONTROLLER_2, m_stComponent_Serial_InputData[1]);
	DDX_Control(pDX, IDC_STC_SERIAL_LIGHT_CONTROLLER_3, m_stComponent_Serial_InputData[2]);
	DDX_Control(pDX, IDC_STC_SERIAL_LIGHT_CONTROLLER_4, m_stComponent_Serial_InputData[3]);

	DDX_Control(pDX, IDC_STC_SERIAL_1_CHANNEL_01, m_stComponent_Serial_Channel[0]);
	DDX_Control(pDX, IDC_STC_SERIAL_1_CHANNEL_02, m_stComponent_Serial_Channel[1]);
	DDX_Control(pDX, IDC_STC_SERIAL_1_CHANNEL_03, m_stComponent_Serial_Channel[2]);
	DDX_Control(pDX, IDC_STC_SERIAL_1_CHANNEL_04, m_stComponent_Serial_Channel[3]);
	DDX_Control(pDX, IDC_STC_SERIAL_1_CHANNEL_05, m_stComponent_Serial_Channel[4]);
	DDX_Control(pDX, IDC_STC_SERIAL_1_CHANNEL_06, m_stComponent_Serial_Channel[5]);
	DDX_Control(pDX, IDC_STC_SERIAL_1_CHANNEL_07, m_stComponent_Serial_Channel[6]);
	DDX_Control(pDX, IDC_STC_SERIAL_1_CHANNEL_08, m_stComponent_Serial_Channel[7]);
	DDX_Control(pDX, IDC_STC_SERIAL_1_CHANNEL_09, m_stComponent_Serial_Channel[8]);
	DDX_Control(pDX, IDC_STC_SERIAL_1_CHANNEL_10, m_stComponent_Serial_Channel[9]);
	DDX_Control(pDX, IDC_STC_SERIAL_1_CHANNEL_11, m_stComponent_Serial_Channel[10]);
	DDX_Control(pDX, IDC_STC_SERIAL_1_CHANNEL_12, m_stComponent_Serial_Channel[11]);
	DDX_Control(pDX, IDC_STC_SERIAL_1_CHANNEL_13, m_stComponent_Serial_Channel[12]);

	DDX_Control(pDX, IDC_STC_SERIAL_1_LIGHT_01, m_stComponent_Serial_1_Data[0]);
	DDX_Control(pDX, IDC_STC_SERIAL_1_LIGHT_02, m_stComponent_Serial_1_Data[1]);
	DDX_Control(pDX, IDC_STC_SERIAL_1_LIGHT_03, m_stComponent_Serial_1_Data[2]);
	DDX_Control(pDX, IDC_STC_SERIAL_1_LIGHT_04, m_stComponent_Serial_1_Data[3]);
	DDX_Control(pDX, IDC_STC_SERIAL_1_LIGHT_05, m_stComponent_Serial_1_Data[4]);
	DDX_Control(pDX, IDC_STC_SERIAL_1_LIGHT_06, m_stComponent_Serial_1_Data[5]);
	DDX_Control(pDX, IDC_STC_SERIAL_1_LIGHT_07, m_stComponent_Serial_1_Data[6]);
	DDX_Control(pDX, IDC_STC_SERIAL_1_LIGHT_08, m_stComponent_Serial_1_Data[7]);
	DDX_Control(pDX, IDC_STC_SERIAL_1_LIGHT_09, m_stComponent_Serial_1_Data[8]);
	DDX_Control(pDX, IDC_STC_SERIAL_1_LIGHT_10, m_stComponent_Serial_1_Data[9]);
	DDX_Control(pDX, IDC_STC_SERIAL_1_LIGHT_11, m_stComponent_Serial_1_Data[10]);
	DDX_Control(pDX, IDC_STC_SERIAL_1_LIGHT_12, m_stComponent_Serial_1_Data[11]);
	DDX_Control(pDX, IDC_STC_SERIAL_1_LIGHT_13, m_stComponent_Serial_1_Data[12]);

	DDX_Control(pDX, IDC_BTN_SERIAL_1_ONOFF_01, m_btnComponent_Serial_OnOff[0]);
	DDX_Control(pDX, IDC_BTN_SERIAL_1_ONOFF_02, m_btnComponent_Serial_OnOff[1]);
	DDX_Control(pDX, IDC_BTN_SERIAL_1_ONOFF_03, m_btnComponent_Serial_OnOff[2]);
	DDX_Control(pDX, IDC_BTN_SERIAL_1_ONOFF_04, m_btnComponent_Serial_OnOff[3]);
	DDX_Control(pDX, IDC_BTN_SERIAL_1_ONOFF_05, m_btnComponent_Serial_OnOff[4]);
	DDX_Control(pDX, IDC_BTN_SERIAL_1_ONOFF_06, m_btnComponent_Serial_OnOff[5]);
	DDX_Control(pDX, IDC_BTN_SERIAL_1_ONOFF_07, m_btnComponent_Serial_OnOff[6]);
	DDX_Control(pDX, IDC_BTN_SERIAL_1_ONOFF_08, m_btnComponent_Serial_OnOff[7]);
	DDX_Control(pDX, IDC_BTN_SERIAL_1_ONOFF_09, m_btnComponent_Serial_OnOff[8]);
	DDX_Control(pDX, IDC_BTN_SERIAL_1_ONOFF_10, m_btnComponent_Serial_OnOff[9]);
	DDX_Control(pDX, IDC_BTN_SERIAL_1_ONOFF_11, m_btnComponent_Serial_OnOff[10]);
	DDX_Control(pDX, IDC_BTN_SERIAL_1_ONOFF_12, m_btnComponent_Serial_OnOff[11]);
	DDX_Control(pDX, IDC_BTN_SERIAL_1_ONOFF_13, m_btnComponent_Serial_OnOff[12]);

	DDX_Control(pDX, IDC_BUTTON_FN_LIGHT_ALL_ON, m_btnComponent_Serial_All[0]);
	DDX_Control(pDX, IDC_BUTTON_FN_LIGHT_ALL_OFF, m_btnComponent_Serial_All[1]);

	DDX_Control(pDX, IDC_IPADDRESS_LAN_1, m_ipTester[0]);
	DDX_Control(pDX, IDC_IPADDRESS_LAN_2, m_ipTester[1]);

	DDX_Control(pDX, IDC_EDIT_LAN_PORT_INPUT_1, m_edComponent_Lan_Port[0]);
	DDX_Control(pDX, IDC_EDIT_LAN_PORT_INPUT_2, m_edComponent_Lan_Port[1]);

	DDX_Control(pDX, IDC_COMBO_SERIAL_1, m_cbLightController1_Port);
	DDX_Control(pDX, IDC_COMBO_SERIAL_2, m_cbLightController1_Baud);
	DDX_Control(pDX, IDC_COMBO_SERIAL_3, m_cbLightController1_DataBit);
	DDX_Control(pDX, IDC_COMBO_SERIAL_4, m_cbLightController1_Parity);
	DDX_Control(pDX, IDC_COMBO_SERIAL_5, m_cbLightController1_StopBit);
	DDX_Control(pDX, IDC_COMBO_SERIAL_6, m_cbLightController1_Light);

	DDX_Control(pDX, IDC_COMBO_POWER_1, m_cbPowerSupply1_Port);
	DDX_Control(pDX, IDC_COMBO_POWER_2, m_cbPowerSupply1_Baud);
	DDX_Control(pDX, IDC_COMBO_POWER_3, m_cbPowerSupply1_DataBit);
	DDX_Control(pDX, IDC_COMBO_POWER_4, m_cbPowerSupply1_Parity);
	DDX_Control(pDX, IDC_COMBO_POWER_5, m_cbPowerSupply1_StopBit);

	DDX_Control(pDX, IDC_COMBO_BCR_COMPORT, m_cbHandyBCR_Port);
	DDX_Control(pDX, IDC_COMBO_BCR_BAUDRATE, m_cbHandyBCR_Baud);
	DDX_Control(pDX, IDC_COMBO_BCR_DATABIT, m_cbHandyBCR_DataBit);
	DDX_Control(pDX, IDC_COMBO_BCR_PARITYBIT, m_cbHandyBCR_Parity);
	DDX_Control(pDX, IDC_COMBO_BCR_STOPBIT, m_cbHandyBCR_StopBit);

	DDX_Control(pDX, IDC_LIST_COMM_DELAYLIST, m_ListWaitTime);

	DDX_Control(pDX, IDC_STATIC_TESTER_NO_1, m_stComponent_Tester_No[0]);
	DDX_Control(pDX, IDC_STATIC_TESTER_NO_2, m_stComponent_Tester_No[1]);
	DDX_Control(pDX, IDC_STATIC_TESTER_NO_3, m_stComponent_Tester_No[2]);
	DDX_Control(pDX, IDC_STATIC_TESTER_NO_4, m_stComponent_Tester_No[3]);
	DDX_Control(pDX, IDC_STATIC_TESTER_NO_5, m_stComponent_Tester_No[4]);
	DDX_Control(pDX, IDC_STATIC_TESTER_NO_6, m_stComponent_Tester_No[5]);
	DDX_Control(pDX, IDC_STATIC_TESTER_NO_7, m_stComponent_Tester_No[6]);
	DDX_Control(pDX, IDC_STATIC_TESTER_NO_8, m_stComponent_Tester_No[7]);

	DDX_Control(pDX, IDC_BROWSE_TESTER_1, m_ebTesterPath[0]);
	DDX_Control(pDX, IDC_BROWSE_TESTER_2, m_ebTesterPath[1]);
	DDX_Control(pDX, IDC_BROWSE_TESTER_3, m_ebTesterPath[2]);
	DDX_Control(pDX, IDC_BROWSE_TESTER_4, m_ebTesterPath[3]);
	DDX_Control(pDX, IDC_BROWSE_TESTER_5, m_ebTesterPath[4]);
	DDX_Control(pDX, IDC_BROWSE_TESTER_6, m_ebTesterPath[5]);
	DDX_Control(pDX, IDC_BROWSE_TESTER_7, m_ebTesterPath[6]);
	DDX_Control(pDX, IDC_BROWSE_TESTER_8, m_ebTesterPath[7]);

	// 전압 & 전류
	DDX_Control(pDX, IDC_STC_POWER_VOLT,		m_stComponent_Power_Volt);
	DDX_Control(pDX, IDC_STC_POWER_CURRENT,		m_stComponent_Power_Current);
	DDX_Control(pDX, IDC_EDIT_POWER_VOLT,		m_edComponent_Power_Volt);
	DDX_Control(pDX, IDC_EDIT_POWER_CURRENT,	m_edComponent_Power_Current);
}

BEGIN_MESSAGE_MAP(CDlgComponent, CDialogEx)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_1, &CDlgComponent::OnBnClickedBtn1)
	ON_BN_CLICKED(IDC_BTN_LAN_CONNECT_1, &CDlgComponent::OnBnClickedBtnLanConnect1)
	ON_BN_CLICKED(IDC_BTN_DISCONNECT_1, &CDlgComponent::OnBnClickedBtnDisconnect1)
	ON_BN_CLICKED(IDC_BTN_LAN_CONNECT_2, &CDlgComponent::OnBnClickedBtnLanConnect2)
	ON_BN_CLICKED(IDC_BTN_DISCONNECT_2, &CDlgComponent::OnBnClickedBtnDisconnect2)
	ON_BN_CLICKED(IDC_BTN_LAN_CONNECT_3, &CDlgComponent::OnBnClickedBtnLanConnect3)
	ON_BN_CLICKED(IDC_BTN_DISCONNECT_3, &CDlgComponent::OnBnClickedBtnDisconnect3)
	ON_BN_CLICKED(IDC_BTN_SERIAL_CONNECT_1, &CDlgComponent::OnBnClickedBtnSerialConnect1)
	ON_BN_CLICKED(IDC_BTN_SERIAL_DISCONNECT_1, &CDlgComponent::OnBnClickedBtnSerialDisconnect1)
	ON_EN_KILLFOCUS(IDC_EDIT_WAITTIME, &CDlgComponent::OnBnClickedBtnSaveWiattimelist)
	
	ON_BN_CLICKED(IDC_BTN_CONNECT_LAN_SERVER_CHK_1, &CDlgComponent::OnBnClickedBtnConnectLanServerChk1)
	ON_BN_CLICKED(IDC_BTN_CONNECT_LAN_SERVER_CHK_2, &CDlgComponent::OnBnClickedBtnConnectLanServerChk2)

	ON_CONTROL_RANGE(BN_CLICKED, IDC_STC_SERIAL_1_LIGHT_01, IDC_STC_SERIAL_1_LIGHT_13, &CDlgComponent::OnRangeInputData)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_SERIAL_1_ONOFF_01, IDC_BTN_SERIAL_1_ONOFF_13, &CDlgComponent::OnRangeSerialOnoff)
	ON_NOTIFY(NM_CLICK, IDC_LIST_COMM_DELAYLIST, &CDlgComponent::OnNMDblclkListCommDelaylist)
	ON_CBN_SELCHANGE(IDC_COMBO_SERIAL_6, &CDlgComponent::OnCbnSelchangeComboSerial6)
	ON_BN_CLICKED(IDC_BUTTON_FN_LIGHT_ALL_ON, &CDlgComponent::OnBnClickedButtonFnLightAllOn)
	ON_BN_CLICKED(IDC_BUTTON_FN_LIGHT_ALL_OFF, &CDlgComponent::OnBnClickedButtonFnLightAllOff)
	ON_BN_CLICKED(IDC_BTN_POWER_CONNECT_1, &CDlgComponent::OnBnClickedBtnPowerConnect1)
	ON_BN_CLICKED(IDC_BTN_POWER_DISCONNECT_1, &CDlgComponent::OnBnClickedBtnPowerDisconnect1)
	ON_BN_CLICKED(IDC_BTN_BCR_CONNECT, &CDlgComponent::OnBnClickedBtnBcrConnect)
	ON_BN_CLICKED(IDC_BTN_BCR_DISCONNECT, &CDlgComponent::OnBnClickedBtnBcrDisconnect)
	ON_BN_CLICKED(IDC_BTN_SERIAL_1_ONOFF_01, &CDlgComponent::OnBnClickedBtnSerial1Onoff01)
END_MESSAGE_MAP()

BOOL CDlgComponent::PreTranslateMessage(MSG* pMsg)
{
	//ALT + F4 차단
	if (pMsg->message == WM_SYSKEYDOWN && pMsg->wParam == VK_F4)
	{
		return TRUE;
	}

	//ESC 차단
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		return TRUE;
	}

	//Enter 차단
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
// CDlgTitle 메시지 처리기

BOOL CDlgComponent::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(83, 169, 155));
	return TRUE;
}

// CDlgIo 메시지 처리기
int CDlgComponent::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}
void CDlgComponent::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (bShow)
	{
		BOOL bState = (m_UdpTester[0].m_bConnected == TRUE);
		EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();

		for (int i = 0; i < 1; i++)
		{
			m_btnComponent_Lan_Connect[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
			if(bState == TRUE) m_btnComponent_Lan_Connect[i].SetColorStyle(CVGStatic::ColorStyle_Green);
			else m_btnComponent_Lan_Connect[i].SetColorStyle(CVGStatic::ColorStyle_Black);
			m_btnComponent_Lan_Connect[i].SetFont_Gdip(L"Arial", 9.0F);

			if (stBasic.nLanguage == 0)
				m_btnComponent_Lan_Connect[i].SetText(m_szComponentConnect_Kor[0]);
			else
				m_btnComponent_Lan_Connect[i].SetText(m_szComponentConnect_Eng[0]);
		}
		for (int i = 0; i < 1; i++)
		{
			m_btnComponent_Lan_DisConnect[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
			if (bState == FALSE) m_btnComponent_Lan_DisConnect[i].SetColorStyle(CVGStatic::ColorStyle_Green);
			else m_btnComponent_Lan_DisConnect[i].SetColorStyle(CVGStatic::ColorStyle_Black);
			m_btnComponent_Lan_DisConnect[i].SetFont_Gdip(L"Arial", 9.0F);

			if (stBasic.nLanguage == 0)
				m_btnComponent_Lan_DisConnect[i].SetText(m_szComponentConnect_Kor[1]);
			else
				m_btnComponent_Lan_DisConnect[i].SetText(m_szComponentConnect_Eng[1]);
		}

		TESTER_COMM	stTester = *DATAMANAGER->GetCurrentTesterCommData();
		Load_ComponentDlg();
		SetDlgItemInt(IDC_EDIT_WAITTIME2, stTester.nSleepModeTimer);

		BOOL bEnable[2];
		BOOL bConnect;
		
#if (GET_INSPECTOR != SYS_DISTORTION_30)
	//	if (bConnect = m_ctrlLightController_FN1500[0].Is_Connected())
		if(bConnect = FN300->IsConnected())
#else
		if (bConnect = m_ctrlLightController_Youth_DPS[0].Is_Connected())
#endif
		{
			bEnable[0] = FALSE;
			bEnable[1] = TRUE;
		}
		else
		{
			bEnable[0] = TRUE;
			bEnable[1] = FALSE;
		}

		m_btnComponent_Serial_Connect[0].EnableWindow(bEnable[0]);
		m_btnComponent_Serial_DisConnect[0].EnableWindow(bEnable[1]);

		m_btnComponent_Serial_All[0].EnableWindow(bEnable[1]);
		m_btnComponent_Serial_All[1].EnableWindow(bEnable[1]);
	}
}
BOOL CDlgComponent::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	InitComponent_Component();

	//
	InitUI_EqpType();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgComponent::InitUI_EqpType()
{
	auto EqpType = GET_INSPECTOR;

	switch (EqpType)
	{
	case SYS_DISTORTION_30:
		GetDlgItem(IDC_STC_SERIAL_1_LIGHT_02)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_SERIAL_1_ONOFF_02)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_SERIAL_1_CHANNEL_03)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_SERIAL_1_LIGHT_03)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_SERIAL_1_ONOFF_03)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_SERIAL_1_CHANNEL_04)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_SERIAL_1_LIGHT_04)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_SERIAL_1_ONOFF_04)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_SERIAL_1_CHANNEL_05)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_SERIAL_1_LIGHT_05)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_SERIAL_1_ONOFF_05)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_SERIAL_1_CHANNEL_02)->ShowWindow(SW_HIDE);		
		break;

	case SYS_OQC_SFR_MULTI_CL:
		break;

	case SYS_OQC_SFR_SINGLE_CL:
		GetDlgItem(IDC_STC_SERIAL_1_LIGHT_02)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_SERIAL_1_ONOFF_02)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_SERIAL_1_CHANNEL_03)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_SERIAL_1_LIGHT_03)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_SERIAL_1_ONOFF_03)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_SERIAL_1_CHANNEL_04)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_SERIAL_1_LIGHT_04)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_SERIAL_1_ONOFF_04)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_SERIAL_1_CHANNEL_05)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_SERIAL_1_LIGHT_05)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_SERIAL_1_ONOFF_05)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_SERIAL_1_CHANNEL_02)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_STC_SERIAL_CONNECT_TITLE_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_SERIAL_LIGHT_CONTROLLER_1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_SERIAL_LIGHT_CONTROLLER_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_SERIAL_LIGHT_CONTROLLER_3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_SERIAL_1_CHANNEL_01)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_SERIAL_1_LIGHT_01)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_SERIAL_1_ONOFF_01)->ShowWindow(SW_HIDE);
		break;

	default:
		break;
	}
}

void CDlgComponent::InitComponent_Component()
{
	EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();
	
	CString strTemp = _T("");
	CRect rt;
	m_ListWaitTime.GetWindowRect(&rt);
	m_ListWaitTime.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_nSelectColumn = 0;
	for (int i = 0; i < 2; i++)
	{
		if (stBasic.nLanguage == 0)
			m_ListWaitTime.InsertColumn(i, m_szComponentWaitList_Kor[i], LVCFMT_CENTER, (int)(rt.Width()*0.495));
		else
			m_ListWaitTime.InsertColumn(i, m_szComponentWaitList_Eng[i], LVCFMT_CENTER, (int)(rt.Width()*0.495));
	}
	for (int i = 0; i < WAIT_COMM_MAX; i++)
	{
		if (stBasic.nLanguage == 0)
			m_ListWaitTime.InsertItem(i, m_szWaitTimeList_Kor[i]);
		else
			m_ListWaitTime.InsertItem(i, m_szWaitTimeList_Eng[i]);

	}
	for (int i = 0; i < 6; i++)
	{
		m_stComponent_Title[i].SetStaticStyle(CVGStatic::StaticStyle_Title);
		m_stComponent_Title[i].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_stComponent_Title[i].SetFont_Gdip(L"Arial", 9.0F);

		if( stBasic.nLanguage == 0)
			m_stComponent_Title[i].SetText(m_szComponentTitle_Kor[i]);
		else
			m_stComponent_Title[i].SetText(m_szComponentTitle_Eng[i]);
	}

	for (int i = 0; i < 3; i++)
	{
		m_stComponent_Lan_Sub[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stComponent_Lan_Sub[i].SetColorStyle(CVGStatic::ColorStyle_Turquois);
		m_stComponent_Lan_Sub[i].SetFont_Gdip(L"Arial", 9.0F);

		if (stBasic.nLanguage == 0)
			m_stComponent_Lan_Sub[i].SetText(m_szComponentLan_Sub_Kor[i]);
		else
			m_stComponent_Lan_Sub[i].SetText(m_szComponentLan_Sub_Eng[i]);
	}

	for (int i = 0; i < 3; i++)
	{
		m_stComponent_Lan_Ip[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stComponent_Lan_Ip[i].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_stComponent_Lan_Ip[i].SetFont_Gdip(L"Arial", 9.0F);
		m_stComponent_Lan_Ip[i].SetText(_T("IP"));
	}

	for (int i = 0; i < 2; i++)
	{
		m_btnComponent_Lan_Server[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_btnComponent_Lan_Server[i].SetColorStyle(CVGStatic::ColorStyle_Yellow);
		m_btnComponent_Lan_Server[i].SetFont_Gdip(L"Arial", 9.0F);
		m_btnComponent_Lan_Server[i].SetText(_T("Handler is Server"));
	}

	for (int i = 0; i < 3; i++)
	{
		m_btnComponent_Lan_Connect[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_btnComponent_Lan_Connect[i].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_btnComponent_Lan_Connect[i].SetFont_Gdip(L"Arial", 9.0F);

		if (stBasic.nLanguage == 0)
			m_btnComponent_Lan_Connect[i].SetText(m_szComponentConnect_Kor[0]);
		else
			m_btnComponent_Lan_Connect[i].SetText(m_szComponentConnect_Eng[0]);

		m_btnComponent_Lan_DisConnect[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_btnComponent_Lan_DisConnect[i].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_btnComponent_Lan_DisConnect[i].SetFont_Gdip(L"Arial", 9.0F);

		if (stBasic.nLanguage == 0)
			m_btnComponent_Lan_DisConnect[i].SetText(m_szComponentConnect_Kor[1]);
		else
			m_btnComponent_Lan_DisConnect[i].SetText(m_szComponentConnect_Eng[1]);
	}

	for (int i = 0; i < 6; i++)
	{
		m_stComponent_Serial_Sub[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stComponent_Serial_Sub[i].SetColorStyle(CVGStatic::ColorStyle_Turquois);
		m_stComponent_Serial_Sub[i].SetFont_Gdip(L"Arial", 9.0F);

		if (stBasic.nLanguage == 0)
			m_stComponent_Serial_Sub[i].SetText(m_szComponentSerialSub_Kor[i]);
		else
			m_stComponent_Serial_Sub[i].SetText(m_szComponentSerialSub_Eng[i]);
	}

	for (int i = 0; i < 5; i++)
	{
		m_stComponent_Serial_Controller[i][0].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stComponent_Serial_Controller[i][0].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_stComponent_Serial_Controller[i][0].SetFont_Gdip(L"Arial", 9.0F);
		m_stComponent_Serial_Controller[i][0].SetText(m_szComponentSerial_Param[i]);

		m_stComponent_Serial_Controller[i][1].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stComponent_Serial_Controller[i][1].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_stComponent_Serial_Controller[i][1].SetFont_Gdip(L"Arial", 9.0F);
		m_stComponent_Serial_Controller[i][1].SetText(m_szComponentSerial_Param[i]);

		m_stComponent_Serial_Controller[i][2].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stComponent_Serial_Controller[i][2].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_stComponent_Serial_Controller[i][2].SetFont_Gdip(L"Arial", 9.0F);
		m_stComponent_Serial_Controller[i][2].SetText(m_szComponentSerial_Param[i]);
	}
	
	for (int i = 0; i < 1; i++)
	{
		//m_btnComponent_Serial_Connect[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		//m_btnComponent_Serial_Connect[i].SetColorStyle(CVGStatic::ColorStyle_Black);
		//m_btnComponent_Serial_Connect[i].SetFont_Gdip(L"Arial", 9.0F);

		if (stBasic.nLanguage == 0)
			m_btnComponent_Serial_Connect[i].SetWindowText(m_szComponentConnect_Kor[0]);
		else
			m_btnComponent_Serial_Connect[i].SetWindowText(m_szComponentConnect_Eng[0]);

		//m_btnComponent_Serial_DisConnect[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		//m_btnComponent_Serial_DisConnect[i].SetColorStyle(CVGStatic::ColorStyle_Black);
		//m_btnComponent_Serial_DisConnect[i].SetFont_Gdip(L"Arial", 9.0F);

		if (stBasic.nLanguage == 0)
			m_btnComponent_Serial_DisConnect[i].SetWindowText(m_szComponentConnect_Kor[1]);
		else
			m_btnComponent_Serial_DisConnect[i].SetWindowText(m_szComponentConnect_Eng[1]);
	}

	/*for (int i = 0; i < 1; i++)
	{
		m_btnPower_Serial_Connect[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_btnPower_Serial_Connect[i].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_btnPower_Serial_Connect[i].SetFont_Gdip(L"Arial", 9.0F);

		if (stBasic.nLanguage == 0)
			m_btnPower_Serial_Connect[i].SetText(m_szComponentConnect_Kor[0]);
		else
			m_btnPower_Serial_Connect[i].SetText(m_szComponentConnect_Eng[0]);

		m_btnPower_Serial_DisConnect[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_btnPower_Serial_DisConnect[i].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_btnPower_Serial_DisConnect[i].SetFont_Gdip(L"Arial", 9.0F);

		if (stBasic.nLanguage == 0)
			m_btnPower_Serial_DisConnect[i].SetText(m_szComponentConnect_Kor[1]);
		else
			m_btnPower_Serial_DisConnect[i].SetText(m_szComponentConnect_Eng[1]);
	}*/

	for (int i = 0; i < 4; i++)
	{
		m_stComponent_Serial_InputData[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Alt);
		m_stComponent_Serial_InputData[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_stComponent_Serial_InputData[i].SetFont_Gdip(L"Arial", 9.0F);

		if( i % 4 == 0)
			m_stComponent_Serial_InputData[i].SetText(_T("Channel"));
		else if ( i % 4 == 1 )
			m_stComponent_Serial_InputData[i].SetText(_T("Light"));
		else if (i % 4 == 2)
			m_stComponent_Serial_InputData[i].SetText(_T("On/Off"));
		else
			m_stComponent_Serial_InputData[i].SetText(_T("Model"));
	}

	for (int i = 0; i < 13; i++)
	{
		strTemp.Format(_T("CH %d"), (i + 1));
		m_stComponent_Serial_Channel[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stComponent_Serial_Channel[i].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_stComponent_Serial_Channel[i].SetFont_Gdip(L"Arial", 9.0F);
		m_stComponent_Serial_Channel[i].SetText(strTemp);

		strTemp.Format(_T("0.0"));
		m_stComponent_Serial_1_Data[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stComponent_Serial_1_Data[i].SetColorStyle(CVGStatic::ColorStyle_White);
		m_stComponent_Serial_1_Data[i].SetFont_Gdip(L"Arial", 9.0F);
		m_stComponent_Serial_1_Data[i].SetText(strTemp);

		//m_btnComponent_Serial_OnOff[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		//m_btnComponent_Serial_OnOff[i].SetColorStyle(CVGStatic::ColorStyle_Black);
		//m_btnComponent_Serial_OnOff[i].SetFont_Gdip(L"Arial", 9.0F);
		//m_btnComponent_Serial_OnOff[i].SetText(_T("Off"));
	}
	// TESTER
	for (int i = 0; i < MAX_TESTER_NUM; i++)
	{
		strTemp.Format(_T("%s"), cTestN[i]);
		m_stComponent_Tester_No[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stComponent_Tester_No[i].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_stComponent_Tester_No[i].SetFont_Gdip(L"Arial", 9.0F);
		m_stComponent_Tester_No[i].SetText(strTemp);
	}

	// SERIAL
	TCHAR *BaudList[] = { _T("9600"),_T("19200"),_T("38400"),_T("57600"),_T("115200") };
	TCHAR *DataBitList[] = { _T("7"), _T("8") };
	TCHAR *ParityList[] = { _T("None"),	_T("Even"),	_T("Odd") };
	TCHAR *StopBitList[] = { _T("1"), _T("2") };

	// Port
	m_cbLightController1_Port.ResetContent();
	m_cbPowerSupply1_Port.ResetContent();

	COMMCONFIG lpCC;
	CString str, str1;
	unsigned long ccsize = sizeof(COMMCONFIG);
	int index = 0;
	BOOL bExistFlag = FALSE;


	for (int i = 1; i < 15; i++)
	{
		str = _T("COM");
		str1.Format(_T("%d"), i);
		str += str1;
		bExistFlag = FALSE;

		// 중요 포인트 return값이 TRUE이면 COMPORT존재
		bExistFlag = GetDefaultCommConfig(str, &lpCC, &ccsize);

		//if (bExistFlag)
		{
			index = m_cbLightController1_Port.AddString(str);
			m_cbLightController1_Port.SetItemData(index, i);

			index = m_cbPowerSupply1_Port.AddString(str);
			m_cbPowerSupply1_Port.SetItemData(index, i);

			index = m_cbHandyBCR_Port.AddString(str);
			m_cbHandyBCR_Port.SetItemData(index, i);
		}
	}

	//Baudrate
	m_cbLightController1_Baud.ResetContent();
	m_cbPowerSupply1_Baud.ResetContent();

	for (int i = 0; i < 5; i++)
	{
		str.Format(_T("%s"), BaudList[i]);
		index = m_cbLightController1_Baud.AddString(str);
		m_cbLightController1_Baud.SetItemData(index, i);

		index = m_cbPowerSupply1_Baud.AddString(str);
		m_cbPowerSupply1_Baud.SetItemData(index, i);

		index = m_cbHandyBCR_Baud.AddString(str);
		m_cbHandyBCR_Baud.SetItemData(index, i);
	}

	//Data bit
	m_cbLightController1_DataBit.ResetContent();
	m_cbPowerSupply1_DataBit.ResetContent();

	for (int i = 0; i < 2; i++)
	{
		str.Format(_T("%s"), DataBitList[i]);
		index = m_cbLightController1_DataBit.AddString(str);
		m_cbLightController1_DataBit.SetItemData(index, i);

		index = m_cbPowerSupply1_DataBit.AddString(str);
		m_cbPowerSupply1_DataBit.SetItemData(index, i);

		index = m_cbHandyBCR_DataBit.AddString(str);
		m_cbHandyBCR_DataBit.SetItemData(index, i);
	}

	//Parity bit
	m_cbLightController1_Parity.ResetContent();
	m_cbPowerSupply1_Parity.ResetContent();

	for (int i = 0; i < 3; i++)
	{
		str.Format(_T("%s"), ParityList[i]);
		index = m_cbLightController1_Parity.AddString(str);
		m_cbLightController1_Parity.SetItemData(index, i);

		index = m_cbPowerSupply1_Parity.AddString(str);
		m_cbPowerSupply1_Parity.SetItemData(index, i);

		index = m_cbHandyBCR_Parity.AddString(str);
		m_cbHandyBCR_Parity.SetItemData(index, i);
	}

	//Stop bit
	m_cbLightController1_StopBit.ResetContent();
	m_cbPowerSupply1_StopBit.ResetContent();

	for (int i = 0; i < 2; i++)
	{
		str.Format(_T("%s"), StopBitList[i]);
		index = m_cbLightController1_StopBit.AddString(str);
		m_cbLightController1_StopBit.SetItemData(index, i);

		index = m_cbPowerSupply1_StopBit.AddString(str);
		m_cbPowerSupply1_StopBit.SetItemData(index, i);

		index = m_cbHandyBCR_StopBit.AddString(str);
		m_cbHandyBCR_StopBit.SetItemData(index, i);
	}

	// 전압 & 전류
	m_stComponent_Power_Volt.SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
	m_stComponent_Power_Volt.SetColorStyle(CVGStatic::ColorStyle_Black);
	m_stComponent_Power_Volt.SetFont_Gdip(L"Arial", 9.0F);
	m_stComponent_Power_Volt.SetText(_T("Volt(V)"));

	m_stComponent_Power_Current.SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
	m_stComponent_Power_Current.SetColorStyle(CVGStatic::ColorStyle_Black);
	m_stComponent_Power_Current.SetFont_Gdip(L"Arial", 9.0F);
	m_stComponent_Power_Current.SetText(_T("Current(A)"));

	// Light 
	TCHAR *LightList[] = { _T("LENS_180"), _T("LENS_82.5") };

	m_cbLightController1_Light.ResetContent();

	for (int i = 0; i < 2; i++)
	{
		str.Format(_T("%s"), LightList[i]);
		index = m_cbLightController1_Light.AddString(str);
		m_cbLightController1_Light.SetItemData(index, i);
	}
}
void CDlgComponent::ChangeUI_Language()
{
	EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();

	CString strTemp = _T("");
	CRect rt;
	
	m_ListWaitTime.GetWindowRect(&rt);
	m_ListWaitTime.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_nSelectColumn = 0;
	for (int i = 0; i < 2; i++)
	{
		m_ListWaitTime.DeleteColumn(i);
		if (stBasic.nLanguage == 0)
			m_ListWaitTime.InsertColumn(i, m_szComponentWaitList_Kor[i], LVCFMT_CENTER, (int)(rt.Width()*0.495));
		else
			m_ListWaitTime.InsertColumn(i, m_szComponentWaitList_Eng[i], LVCFMT_CENTER, (int)(rt.Width()*0.495));
	}
	
	ChangeUI_WaitList();

	for (int i = 0; i < 6; i++)
	{
		m_stComponent_Title[i].SetStaticStyle(CVGStatic::StaticStyle_Title);
		m_stComponent_Title[i].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_stComponent_Title[i].SetFont_Gdip(L"Arial", 9.0F);

		if (stBasic.nLanguage == 0)
			m_stComponent_Title[i].SetText(m_szComponentTitle_Kor[i]);
		else
			m_stComponent_Title[i].SetText(m_szComponentTitle_Eng[i]);
	}

	for (int i = 0; i < 3; i++)
	{
		m_stComponent_Lan_Sub[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stComponent_Lan_Sub[i].SetColorStyle(CVGStatic::ColorStyle_Turquois);
		m_stComponent_Lan_Sub[i].SetFont_Gdip(L"Arial", 9.0F);

		if (stBasic.nLanguage == 0)
			m_stComponent_Lan_Sub[i].SetText(m_szComponentLan_Sub_Kor[i]);
		else
			m_stComponent_Lan_Sub[i].SetText(m_szComponentLan_Sub_Eng[i]);
	}

	for (int i = 0; i < 3; i++)
	{
		m_stComponent_Lan_Ip[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stComponent_Lan_Ip[i].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_stComponent_Lan_Ip[i].SetFont_Gdip(L"Arial", 9.0F);
		m_stComponent_Lan_Ip[i].SetText(_T("IP"));
	}

	for (int i = 0; i < 2; i++)
	{
		m_btnComponent_Lan_Server[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_btnComponent_Lan_Server[i].SetColorStyle(CVGStatic::ColorStyle_Yellow);
		m_btnComponent_Lan_Server[i].SetFont_Gdip(L"Arial", 9.0F);
		m_btnComponent_Lan_Server[i].SetText(_T("Handler is Server"));
	}

	for (int i = 0; i < 3; i++)
	{
		m_btnComponent_Lan_Connect[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_btnComponent_Lan_Connect[i].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_btnComponent_Lan_Connect[i].SetFont_Gdip(L"Arial", 9.0F);

		if (stBasic.nLanguage == 0)
			m_btnComponent_Lan_Connect[i].SetText(m_szComponentConnect_Kor[0]);
		else
			m_btnComponent_Lan_Connect[i].SetText(m_szComponentConnect_Eng[0]);

		m_btnComponent_Lan_DisConnect[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_btnComponent_Lan_DisConnect[i].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_btnComponent_Lan_DisConnect[i].SetFont_Gdip(L"Arial", 9.0F);

		if (stBasic.nLanguage == 0)
			m_btnComponent_Lan_DisConnect[i].SetText(m_szComponentConnect_Kor[1]);
		else
			m_btnComponent_Lan_DisConnect[i].SetText(m_szComponentConnect_Eng[1]); 
	}

	for (int i = 0; i < 6; i++)
	{
		m_stComponent_Serial_Sub[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stComponent_Serial_Sub[i].SetColorStyle(CVGStatic::ColorStyle_Turquois);
		m_stComponent_Serial_Sub[i].SetFont_Gdip(L"Arial", 9.0F);

		if (stBasic.nLanguage == 0)
			m_stComponent_Serial_Sub[i].SetText(m_szComponentSerialSub_Kor[i]);
		else
			m_stComponent_Serial_Sub[i].SetText(m_szComponentSerialSub_Eng[i]);
	}

	for (int i = 0; i < 5; i++)
	{
		m_stComponent_Serial_Controller[i][0].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stComponent_Serial_Controller[i][0].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_stComponent_Serial_Controller[i][0].SetFont_Gdip(L"Arial", 9.0F);
		m_stComponent_Serial_Controller[i][0].SetText(m_szComponentSerial_Param[i]);

		m_stComponent_Serial_Controller[i][1].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stComponent_Serial_Controller[i][1].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_stComponent_Serial_Controller[i][1].SetFont_Gdip(L"Arial", 9.0F);
		m_stComponent_Serial_Controller[i][1].SetText(m_szComponentSerial_Param[i]);

		m_stComponent_Serial_Controller[i][2].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stComponent_Serial_Controller[i][2].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_stComponent_Serial_Controller[i][2].SetFont_Gdip(L"Arial", 9.0F);
		m_stComponent_Serial_Controller[i][2].SetText(m_szComponentSerial_Param[i]);
	}

	for (int i = 0; i < 1; i++)
	{
		//m_btnComponent_Serial_Connect[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		//m_btnComponent_Serial_Connect[i].SetColorStyle(CVGStatic::ColorStyle_Black);
		//m_btnComponent_Serial_Connect[i].SetFont_Gdip(L"Arial", 9.0F);

		if (stBasic.nLanguage == 0)
			m_btnComponent_Serial_Connect[i].SetWindowText(m_szComponentConnect_Kor[0]);
		else
			m_btnComponent_Serial_Connect[i].SetWindowText(m_szComponentConnect_Eng[0]);

		//m_btnComponent_Serial_DisConnect[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		//m_btnComponent_Serial_DisConnect[i].SetColorStyle(CVGStatic::ColorStyle_Black);
		//m_btnComponent_Serial_DisConnect[i].SetFont_Gdip(L"Arial", 9.0F);

		if (stBasic.nLanguage == 0)
			m_btnComponent_Serial_DisConnect[i].SetWindowText(m_szComponentConnect_Kor[1]);
		else
			m_btnComponent_Serial_DisConnect[i].SetWindowText(m_szComponentConnect_Eng[1]);
	}

	/*for (int i = 0; i < 1; i++)
	{
		m_btnPower_Serial_Connect[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_btnPower_Serial_Connect[i].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_btnPower_Serial_Connect[i].SetFont_Gdip(L"Arial", 9.0F);

		if (stBasic.nLanguage == 0)
			m_btnPower_Serial_Connect[i].SetText(m_szComponentConnect_Kor[0]);
		else
			m_btnPower_Serial_Connect[i].SetText(m_szComponentConnect_Eng[0]);

		m_btnPower_Serial_DisConnect[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_btnPower_Serial_DisConnect[i].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_btnPower_Serial_DisConnect[i].SetFont_Gdip(L"Arial", 9.0F);

		if (stBasic.nLanguage == 0)
			m_btnPower_Serial_DisConnect[i].SetText(m_szComponentConnect_Kor[1]);
		else
			m_btnPower_Serial_DisConnect[i].SetText(m_szComponentConnect_Eng[1]);
	}*/

	for (int i = 0; i < 4; i++)
	{
		m_stComponent_Serial_InputData[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Alt);
		m_stComponent_Serial_InputData[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_stComponent_Serial_InputData[i].SetFont_Gdip(L"Arial", 9.0F);

		if (stBasic.nLanguage == 0)
		{
			m_stComponent_Serial_InputData[i].SetText(m_szComponentSerialInput_Kor[i]);
		}
		else
		{
			m_stComponent_Serial_InputData[i].SetText(m_szComponentSerialInput_Eng[i]);
		}
}

	m_ListWaitTime.DeleteAllItems();
	for (int i = 0; i < WAIT_COMM_MAX; i++)
	{
		if (stBasic.nLanguage == 0)
			m_ListWaitTime.InsertItem(i, m_szWaitTimeList_Kor[i]);
		else
			m_ListWaitTime.InsertItem(i, m_szWaitTimeList_Eng[i]);

	}
	ChangeUI_WaitList();
}

void CDlgComponent::Load_ComponentDlg()
{
	TESTER_COMM	stTester = *DATAMANAGER->GetCurrentTesterCommData();
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();

	for (int i = 0; i < 2; i++)
	{
		m_strIpAddress[i] = stTester.strTesterConnect[i];
		m_nTesterPort[i] = stTester.nTesterPort[i];
		m_dwTesterIp[i] = stTester.dwTesterIP[i];
		m_nHandlerServerMode[i] = stTester.nServerMode[i];
	}

	int nSelected;

	for (int i = 0; i < 13; i++)
	{
		//m_nSerial_1_Data[i] = stTester.stLightData[0].nVoltage[nSelected = stEq.nLightSelected][i];
		m_nSerial_1_Data[i] = stTester.stLightData[0].nVoltage[nSelected = m_nLightSelected][i];
	}

	ChangeUi_Lan();
	ChangeUi_Serial();
	ChangeUI_WaitList();
	ChangeUI_Tester();
	ChangeUI_Power();
}

void CDlgComponent::ChangeUI_Power()
{
	TESTER_COMM	stTester = *DATAMANAGER->GetCurrentTesterCommData();

	CString sz;

	sz.Format(_T("%.1f"), stTester.fVolt);
	m_edComponent_Power_Volt.SetWindowText(sz);

	sz.Format(_T("%d"), stTester.nCurrent);
	m_edComponent_Power_Current.SetWindowText(sz);
}

void CDlgComponent::ChangeUi_Lan()
{
	CString strTemp = _T("");
	CString strPart = _T("");
	BYTE byIp[4] = { 0x00, };
	TCHAR tchSep = _T('.');

	for (int i = 0; i < 2; i++)
	{
		strTemp.Format(_T("%s"), m_strIpAddress[i]);
		for (int j = 0; j < 4; j++)
		{
			AfxExtractSubString(strPart, strTemp, j, tchSep);
			byIp[j] = _ttoi(strPart);

		}
		m_ipTester[i].SetAddress(byIp[0], byIp[1], byIp[2], byIp[3]);
	}

	strTemp.Format(_T("%d"), m_nTesterPort[0]);
	SetDlgItemTextW(IDC_EDIT_LAN_PORT_INPUT_1, strTemp);

	strTemp.Format(_T("%d"), m_nTesterPort[1]);
	SetDlgItemTextW(IDC_EDIT_LAN_PORT_INPUT_2, strTemp);


	for (int i = 0; i < 2; i++)
	{
		if (m_nHandlerServerMode[i] == TRUE)
		{
			m_btnComponent_Lan_Server[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
			m_btnComponent_Lan_Server[i].SetColorStyle(CVGStatic::ColorStyle_Green);
			m_btnComponent_Lan_Server[i].SetFont_Gdip(L"Arial", 9.0F);
			m_btnComponent_Lan_Server[i].SetText(_T("SERVER MODE"));			
		}
		else
		{
			m_btnComponent_Lan_Server[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
			m_btnComponent_Lan_Server[i].SetColorStyle(CVGStatic::ColorStyle_Yellow);
			m_btnComponent_Lan_Server[i].SetFont_Gdip(L"Arial", 9.0F);
			m_btnComponent_Lan_Server[i].SetText(_T("CLIENT MODE"));
		}
	}
}
void CDlgComponent::ChangeUi_Serial()
{
	TESTER_COMM	stTester = *DATAMANAGER->GetCurrentTesterCommData();

	SetComboBox_Serial(&m_cbLightController1_Port, stTester.nPort[0]);
	SetComboBox_Serial(&m_cbLightController1_Baud, stTester.nBaudRate[0]);
	SetComboBox_Serial(&m_cbLightController1_DataBit, stTester.nDataBit[0]);
	SetComboBox_Serial(&m_cbLightController1_Parity, stTester.nParityBit[0]);
	SetComboBox_Serial(&m_cbLightController1_StopBit, stTester.nStopBit[0]);

	SetComboBox_Serial(&m_cbPowerSupply1_Port, stTester.nPort[1]);
	SetComboBox_Serial(&m_cbPowerSupply1_Baud, stTester.nBaudRate[1]);
	SetComboBox_Serial(&m_cbPowerSupply1_DataBit, stTester.nDataBit[1]);
	SetComboBox_Serial(&m_cbPowerSupply1_Parity, stTester.nParityBit[1]);
	SetComboBox_Serial(&m_cbPowerSupply1_StopBit, stTester.nStopBit[1]);

	SetComboBox_Serial(&m_cbHandyBCR_Port, stTester.nPort[2]);
	SetComboBox_Serial(&m_cbHandyBCR_Baud, stTester.nBaudRate[2]);
	SetComboBox_Serial(&m_cbHandyBCR_DataBit, stTester.nDataBit[2]);
	SetComboBox_Serial(&m_cbHandyBCR_Parity, stTester.nParityBit[2]);
	SetComboBox_Serial(&m_cbHandyBCR_StopBit, stTester.nStopBit[2]);

	m_cbLightController1_Light.SetCurSel(m_nLightSelected);

	CString strTemp = _T("");
	for (int i = 0; i < 13; i++)
	{
		strTemp.Format(_T("%d"), m_nSerial_1_Data[i] );
		SetDlgItemText(IDC_STC_SERIAL_1_LIGHT_01 + i, strTemp);
	}
}

void CDlgComponent::ChangeUI_WaitList()
{
	TESTER_COMM	stTester = *DATAMANAGER->GetCurrentTesterCommData();

	for (int i = 0; i < MAX_WAITCOMM_NUM; i++)
	{
		m_ListWaitTime.SetItem(i, 1, LVIF_TEXT, stTester.strWaitTimeLIst[i], NULL, NULL, NULL, NULL);
	}

}

void CDlgComponent::ChangeUI_Tester()
{
	TESTER_COMM	stTester = *DATAMANAGER->GetCurrentTesterCommData();

	CString strTemp = _T("");
	for (int i = 0; i < MAX_TESTER_NUM; i++)
	{
		strTemp.Format(_T("%s"), stTester.strSemcoVer[i]);
		//SetDlgItemText(IDC_EDIT_TESTER_NAME_1+i, strTemp);
		m_ebTesterPath[i].SetWindowText(strTemp);
	}
}

void CDlgComponent::OnBnClickedBtn1()
{
	// Save
	TESTER_COMM	stTester = *DATAMANAGER->GetCurrentTesterCommData();

	CString strIp = _T("");
	CString strPort = _T("");
	CString strTemp = _T("");
	TCHAR cName[MAX_PATH] = { 0, };
	TCHAR cExt[MAX_PATH] = { 0, };
	BYTE byIp[4] = { 0x00, };

	DWORD dwIp = 0;

	GetDlgItemText(IDC_EDIT_WAITTIME2, strTemp);

	stTester.nSleepModeTimer = _ttoi(strTemp);

	for (int i = 0; i < MAX_WAITCOMM_NUM; i++)
	{
		strTemp = m_ListWaitTime.GetItemText(i, 1);
		stTester.strWaitTimeLIst[i] = strTemp;
	}
	for (int i = 0; i < 2; i++)
	{
		
		m_ipTester[i].GetAddress(byIp[0], byIp[1], byIp[2], byIp[3]);
		strTemp.Format(_T("%d.%d.%d.%d"), byIp[0], byIp[1], byIp[2], byIp[3]);
		m_strIpAddress[i] = strTemp;

		stTester.strTesterConnect[i] = m_strIpAddress[i];

		GetDlgItemText(IDC_EDIT_LAN_PORT_INPUT_1 + i, strTemp);
		m_nTesterPort[i] = _ttoi(strTemp);

		stTester.nTesterPort[i] = m_nTesterPort[i];

		m_ipTester[i].GetAddress(dwIp);
		stTester.dwTesterIP[i] = dwIp;

		stTester.nServerMode[i] = m_nHandlerServerMode[i];		
	}

	// 전압 & 전류
	m_edComponent_Power_Volt.GetWindowText(strTemp);
	auto fVolt = _tstof(strTemp);

	if (fVolt < 5 || fVolt > 18)
	{
		AfxMessageBox(_T("Voltage Set Range Over : it can be Voltage Range 3v ~ 18v"));
		m_edComponent_Power_Volt.SetWindowTextW(_T("13.5"));
		return;
	}
	else
	{
		stTester.fVolt = _tstof(strTemp);
	}	

	m_edComponent_Power_Current.GetWindowText(strTemp);

	auto fCurrent = _tstof(strTemp);
	if (fCurrent < 1 || fCurrent > 3)
	{
		AfxMessageBox(_T("Current Set Range Over : it can be Current Range 1A ~ 3A"));
		m_edComponent_Power_Current.SetWindowTextW(_T("1"));
		return;
	}
	else
	{
		stTester.nCurrent = _ttoi(strTemp);
	}

	// LIGHT CONTROLER 1
	int index = 0;
	strTemp = _T("");
	int nTempData = 0;

	index = m_cbLightController1_Port.GetCurSel();
	stTester.nPort[0] = (int)m_cbLightController1_Port.GetItemData(index);
	index = m_cbLightController1_Baud.GetCurSel();
	stTester.nBaudRate[0] = (int)m_cbLightController1_Baud.GetItemData(index);
	index = m_cbLightController1_DataBit.GetCurSel();
	stTester.nDataBit[0] = (int)m_cbLightController1_DataBit.GetItemData(index);
	index = m_cbLightController1_Parity.GetCurSel();
	stTester.nParityBit[0] = (int)m_cbLightController1_Parity.GetItemData(index);
	index = m_cbLightController1_StopBit.GetCurSel();
	stTester.nStopBit[0] = (int)m_cbLightController1_StopBit.GetItemData(index);

	// 선택된 조명 모델 저장
	index = m_cbPowerSupply1_Port.GetCurSel();
	stTester.nPort[1] = (int)m_cbPowerSupply1_Port.GetItemData(index);
	index = m_cbPowerSupply1_Baud.GetCurSel();
	stTester.nBaudRate[1] = (int)m_cbPowerSupply1_Baud.GetItemData(index);
	index = m_cbPowerSupply1_DataBit.GetCurSel();
	stTester.nDataBit[1] = (int)m_cbPowerSupply1_DataBit.GetItemData(index);
	index = m_cbPowerSupply1_Parity.GetCurSel();
	stTester.nParityBit[1] = (int)m_cbPowerSupply1_Parity.GetItemData(index);
	index = m_cbPowerSupply1_StopBit.GetCurSel();
	stTester.nStopBit[1] = (int)m_cbPowerSupply1_StopBit.GetItemData(index);

	// HANDY BCR 
	index = m_cbHandyBCR_Port.GetCurSel();
	stTester.nPort[2] = (int)m_cbHandyBCR_Port.GetItemData(index);
	index = m_cbHandyBCR_Baud.GetCurSel();
	stTester.nBaudRate[2] = (int)m_cbHandyBCR_Baud.GetItemData(index);
	index = m_cbHandyBCR_DataBit.GetCurSel();
	stTester.nDataBit[2] = (int)m_cbHandyBCR_DataBit.GetItemData(index);
	index = m_cbHandyBCR_Parity.GetCurSel();
	stTester.nParityBit[2] = (int)m_cbHandyBCR_Parity.GetItemData(index);
	index = m_cbHandyBCR_StopBit.GetCurSel();
	stTester.nStopBit[2] = (int)m_cbHandyBCR_StopBit.GetItemData(index);	

	int nSelected;
	index = m_cbLightController1_Light.GetCurSel();  //modified 0928
	// 선택 조명 채널별 값 저장
	for (int i = 0; i < 13; i++)
	{
		nTempData = m_nSerial_1_Data[i] ;

		stTester.stLightData[0].nVoltage[nSelected= index][i] = nTempData;
	}
	// 테스터 이름
	for (int i = 0; i < MAX_TESTER_NUM; i++)
	{
		//GetDlgItemText(IDC_EDIT_TESTER_NAME_1+i, strTemp);
		m_ebTesterPath[i].GetWindowTextW(strTemp);
		stTester.strSemcoVer[i] = strTemp;
		_wsplitpath(strTemp, NULL, NULL, cName, cExt);
		strTemp.Format(_T("%s%s"), cName, cExt);
		stTester.strName[i] = strTemp;
	}

	DATAMANAGER->SetTesterCommData(stTester);

	AfxMessageBox(_T("Saving Completed"), MB_OK);
}


void CDlgComponent::SetComboBox_Serial(CComboBox *pBox, int Data)
{
	int count;
	count = pBox->GetCount();
	for (int i = 0; i < count; i++)
	{
		if (Data == pBox->GetItemData(i))
		{
			pBox->SetCurSel(i);
			break;
		}
	}
}

void CDlgComponent::OnBnClickedBtnLanConnect1()
{
	CString strTemp = _T("");

	TESTER_COMM stTester = *DATAMANAGER->GetCurrentTesterCommData();

	if (stTester.nUseMode[0] == TRUE)
	{
		m_ctrlOthers[0].CloseSocket();
		Sleep(100);
		//if (m_ctrlOthers[i].Connect(stTester.strTesterConnect_Single[i], stTester.nTesterPort_Single[i]) == FALSE)
		m_ctrlOthers[0].CreateSocket(stTester.dwTesterIP[0], stTester.nTesterPort[0], 1);


		if (COMMON->Wait_Time(2000, &m_ctrlOthers[0].m_bConnected) == FALSE)
		{
			COMMON->m_bConnectTesterFlag[0] = FALSE;

			strTemp.Format(_T("IP[%s],Port[%d] Connect Fail. "), stTester.strTesterConnect[0], stTester.nTesterPort[0]);
			//WRITE_COM_FILE(_COMM_LOG, strTemp);
			WRITE_COMM_LOG(strTemp);
		}
		else
		{
			COMMON->m_bConnectTesterFlag[0] = TRUE;

			strTemp.Format(_T("IP[%s],Port[%d] Connect Success."), stTester.strTesterConnect[0], stTester.nTesterPort[0]);
			//WRITE_COM_FILE(_COMM_LOG, strTemp);
			WRITE_COMM_LOG(strTemp);
		}
	}
}
void CDlgComponent::OnBnClickedBtnDisconnect1()
{
	CString strTemp = _T("");

	TESTER_COMM stTester = *DATAMANAGER->GetCurrentTesterCommData();

	int nCount = 0;


	if (stTester.nUseMode[0] == TRUE)
	{
		m_ctrlOthers[0].CloseSocket();

		strTemp.Format(_T("IP[%s],Port[%d] Disconnect"), stTester.strTesterConnect[0], stTester.nTesterPort[0]);
		//WRITE_COM_FILE(_COMM_LOG, strTemp);
		WRITE_COMM_LOG(strTemp);
	}
}


void CDlgComponent::OnBnClickedBtnLanConnect2()
{
	CString strTemp = _T("");

	TESTER_COMM stTester = *DATAMANAGER->GetCurrentTesterCommData();

	if (stTester.nUseMode[1] == TRUE)
	{
		m_ctrlOthers[1].CloseSocket();
		Sleep(100);
		//if (m_ctrlOthers[i].Connect(stTester.strTesterConnect_Single[i], stTester.nTesterPort_Single[i]) == FALSE)
		m_ctrlOthers[1].CreateSocket(stTester.dwTesterIP[1], stTester.nTesterPort[1], 1);

		if (COMMON->Wait_Time(2000, &m_ctrlOthers[1].m_bConnected) == FALSE)
		{
			COMMON->m_bConnectTesterFlag[1] = FALSE;

			strTemp.Format(_T("IP[%s],Port[%d] Connect Fail. "), stTester.strTesterConnect[1], stTester.nTesterPort[1]);
			//WRITE_COM_FILE(_COMM_LOG, strTemp);
			WRITE_COMM_LOG(strTemp);
		}
		else
		{
			COMMON->m_bConnectTesterFlag[1] = TRUE;

			strTemp.Format(_T("IP[%s],Port[%d] Connect Success."), stTester.strTesterConnect[1], stTester.nTesterPort[1]);
			//WRITE_COM_FILE(_COMM_LOG, strTemp);
			WRITE_COMM_LOG(strTemp);
		}
	}
}
void CDlgComponent::OnBnClickedBtnDisconnect2()
{
	CString strTemp = _T("");

	TESTER_COMM stTester = *DATAMANAGER->GetCurrentTesterCommData();

	if (stTester.nUseMode[1] == TRUE)
	{
		m_ctrlOthers[1].CloseSocket();

		strTemp.Format(_T("IP[%s],Port[%d] Disconnect"), stTester.strTesterConnect[1], stTester.nTesterPort[1]);
		//WRITE_COM_FILE(_COMM_LOG, strTemp);
		WRITE_COMM_LOG(strTemp);
	}
}

void CDlgComponent::OnBnClickedBtnLanConnect3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
void CDlgComponent::OnBnClickedBtnDisconnect3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CDlgComponent::OnBnClickedBtnSerialConnect1()
{
	TESTER_COMM stTester = *DATAMANAGER->GetCurrentTesterCommData();

	int nPort;
	int nBaudRate;
	int nDataBit;
	int nStopBit;
	int nParityBit;
	BOOL bEnable[2];

	CString sz;

	CString strLog = _T("");
	// Luritech Light Controller
	nPort = stTester.nPort[0];
	nBaudRate = stTester.nBaudRate[0];
	nDataBit = stTester.nDataBit[0];
	nStopBit = stTester.nStopBit[0];
	nParityBit = stTester.nParityBit[0];

#if (GET_INSPECTOR != SYS_DISTORTION_30)
// 	if (m_ctrlLightController_FN1500[0].m_hWnd == NULL)
// 	{
// 		m_ctrlLightController_FN1500[0].Create();
// 		m_ctrlLightController_FN1500[0].InitVariables(0);
// 	}
// 
// 	if (m_ctrlLightController_FN1500[0].OpenPort(nPort, nBaudRate, WM_LIGHT_CONTROL_FN300_MSG, nParityBit, nDataBit, nStopBit, 0))
// 	{
// 		strLog.Format(_T("[Progress] FN300 Light Connect Success."));
// 		WRITE_MAIN_LOG(strLog);
// 	}
// 	else
// 	{
// 		strLog.Format(_T("[Progress] FN300 Light Connect Fail."));
// 		WRITE_MAIN_LOG(strLog);
// 	}
// 	BOOL b;
// 	if (b = m_ctrlLightController_FN1500[0].Is_Connected())
// 	{
// 		bEnable[0] = FALSE;
// 		bEnable[1] = TRUE;
// 	}
// 	else
// 	{
// 		bEnable[0] = TRUE;
// 		bEnable[1] = FALSE;
// 	}

		sz.Format(_T("\\\\.\\COM%d"), nPort);

	if (FN300->Connect(sz, nBaudRate, nParityBit, nDataBit, nStopBit))
	{
		strLog.Format(_T("[Progress] FN300 Light Connect Success."));
		WRITE_MAIN_LOG(strLog);
	}
	else
	{
		strLog.Format(_T("[Progress] FN300 Light Connect Fail."));
		WRITE_MAIN_LOG(strLog);
	}

	BOOL b;
	if (b = FN300->IsConnected())
	{
		bEnable[0] = FALSE;
		bEnable[1] = TRUE;
	}
	else
	{
		bEnable[0] = TRUE;
		bEnable[1] = FALSE;
	}
#else
	if (m_ctrlLightController_Youth_DPS[0].m_hWnd == NULL)
	{
		m_ctrlLightController_Youth_DPS[0].Create(nPort);
		m_ctrlLightController_Youth_DPS[0].InitVariables(0);
	}

	if (m_ctrlLightController_Youth_DPS[0].OpenPort(nPort, nBaudRate, WM_LIGHT_CONTROL_YOUTHTECH_MSG, nParityBit, nDataBit, nStopBit, 0))
	{
		strLog.Format(_T("[Progress] Youth DPS Light Connect Success."));
		WRITE_MAIN_LOG(strLog);
	}
	else
	{
		strLog.Format(_T("[Progress] Youth DPS Light Connect Fail."));
		WRITE_MAIN_LOG(strLog);
	}
	BOOL b;
	if (b = m_ctrlLightController_Youth_DPS[0].Is_Connected())
	{
		bEnable[0] = FALSE;
		bEnable[1] = TRUE;
	}
	else
	{
		bEnable[0] = TRUE;
		bEnable[1] = FALSE;
	}
#endif

	m_btnComponent_Serial_Connect[0].EnableWindow(bEnable[0]);
	m_btnComponent_Serial_DisConnect[0].EnableWindow(bEnable[1]);
}
void CDlgComponent::OnBnClickedBtnSerialDisconnect1()
{
#if (GET_INSPECTOR != SYS_DISTORTION_30)
//	m_ctrlLightController_FN1500[0].ClosePort();
	FN300->Close();
#else
	m_ctrlLightController_Youth_DPS[0].ClosePort();
#endif

	BOOL bEnable[2];
	bEnable[0] = TRUE;
	bEnable[1] = FALSE;
	m_btnComponent_Serial_Connect[0].EnableWindow(bEnable[0]);
	m_btnComponent_Serial_DisConnect[0].EnableWindow(bEnable[1]);

}

void CDlgComponent::OnRangeInputData(UINT nID)
{
	int ID_Vol = nID - IDC_STC_SERIAL_1_LIGHT_01;

	int nNum = 0;
	CString strTemp = _T("");
	int nCh = ID_Vol % 13;

	CDlgNumPad Dlg;
	Dlg.SetValue(nNum = m_nSerial_1_Data[nCh]);

	if (Dlg.DoModal() == IDOK)
	{
		nNum = _ttoi(Dlg.GetCurrentNum());
		strTemp.Format(_T("%d"), nNum);

		m_nSerial_1_Data[nCh] = nNum;
		m_stComponent_Serial_1_Data[nCh].SetText(strTemp);
	}
}

void CDlgComponent::OnBnClickedBtnConnectLanServerChk1()
{
	if (m_nHandlerServerMode[0] == FALSE)
	{
		m_nHandlerServerMode[0] = TRUE;

		m_btnComponent_Lan_Server[0].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_btnComponent_Lan_Server[0].SetColorStyle(CVGStatic::ColorStyle_Green);
		m_btnComponent_Lan_Server[0].SetFont_Gdip(L"Arial", 9.0F);
		m_btnComponent_Lan_Server[0].SetText(_T("SERVER MODE"));
	}
	else
	{
		m_nHandlerServerMode[0] = FALSE;

		m_btnComponent_Lan_Server[0].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_btnComponent_Lan_Server[0].SetColorStyle(CVGStatic::ColorStyle_Yellow);
		m_btnComponent_Lan_Server[0].SetFont_Gdip(L"Arial", 9.0F);
		m_btnComponent_Lan_Server[0].SetText(_T("CLIENT MODE"));
	}
}


void CDlgComponent::OnBnClickedBtnConnectLanServerChk2()
{
	if (m_nHandlerServerMode[1] == FALSE)
	{
		m_nHandlerServerMode[1] = TRUE;

		m_btnComponent_Lan_Server[1].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_btnComponent_Lan_Server[1].SetColorStyle(CVGStatic::ColorStyle_Green);
		m_btnComponent_Lan_Server[1].SetFont_Gdip(L"Arial", 9.0F);
		m_btnComponent_Lan_Server[1].SetText(_T("SERVER MODE"));
	}
	else
	{
		m_nHandlerServerMode[1] = FALSE;

		m_btnComponent_Lan_Server[1].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_btnComponent_Lan_Server[1].SetColorStyle(CVGStatic::ColorStyle_Yellow);
		m_btnComponent_Lan_Server[1].SetFont_Gdip(L"Arial", 9.0F);
		m_btnComponent_Lan_Server[1].SetText(_T("CLIENT MODE"));
	}
}

// 개별 온오프
void CDlgComponent::OnRangeSerialOnoff(UINT nID)
{
	UINT nChanl, nValue;

	nChanl = nID - IDC_BTN_SERIAL_1_ONOFF_01;
	nValue = m_nSerial_1_Data[nChanl] ;
	// 온
	CString str;
	m_btnComponent_Serial_OnOff[nChanl].GetWindowText(str);
#if (GET_INSPECTOR != SYS_DISTORTION_30)
//	m_ctrlLightController_FN1500[0].Set_FN1500_C(nChanl+1, nValue);
	FN300->Send_SetChannelData(nChanl + 1, nValue);
#else
	m_ctrlLightController_Youth_DPS[0].LightOn(nValue);
#endif
}

void CDlgComponent::OnBnClickedButtonFnLightAllOn()
{
	LRESULT ret = SRC_OK;
#if (GET_INSPECTOR != SYS_DISTORTION_30)
	ret = FN300->Send_SetChannelOnOff(1, TRUE);
	ret = (ret == SRC_OK) ? FN300->Send_SetChannelOnOff(2, TRUE) : ret;
	ret = (ret == SRC_OK) ? FN300->Send_SetChannelOnOff(3, TRUE) : ret;
	ret = (ret == SRC_OK) ? FN300->Send_SetChannelOnOff(4, TRUE) : ret;
	ret = (ret == SRC_OK) ? FN300->Send_SetChannelOnOff(5, TRUE) : ret;
//	m_ctrlLightController_FN1500[0].Set_FN1500_All_On();
#endif
}

void CDlgComponent::OnBnClickedButtonFnLightAllOff()
{
	LRESULT ret = SRC_OK;

#if (GET_INSPECTOR != SYS_DISTORTION_30)
	ret = FN300->Send_SetChannelOnOff(1, FALSE);
	ret = (ret == SRC_OK) ? FN300->Send_SetChannelOnOff(2, FALSE) : ret;
	ret = (ret == SRC_OK) ? FN300->Send_SetChannelOnOff(3, FALSE) : ret;
	ret = (ret == SRC_OK) ? FN300->Send_SetChannelOnOff(4, FALSE) : ret;
	ret = (ret == SRC_OK) ? FN300->Send_SetChannelOnOff(5, FALSE) : ret;
//	m_ctrlLightController_FN1500[0].Set_FN1500_All_Off();
#endif
}

void CDlgComponent::OnNMDblclkListCommDelaylist(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*) pNMHDR;
	m_nSelectColumn = pNMListView->iItem;

	CString sIndexValue;
	sIndexValue = m_ListWaitTime.GetItemText(m_nSelectColumn, 1);
	GetDlgItem(IDC_EDIT_WAITTIME)->SetWindowTextW(sIndexValue);
	*pResult = 0;
}

void CDlgComponent::OnBnClickedBtnSaveWiattimelist()
{
	CString strTemp;
	GetDlgItemText(IDC_EDIT_WAITTIME, strTemp);
	m_ListWaitTime.SetItem(m_nSelectColumn, 1,LVIF_TEXT, strTemp,NULL, NULL, NULL, NULL);
}

//void CDlgComponent::OnBnClickedBtnSerialConnect3()
//{
//	TESTER_COMM stTester = *DATAMANAGER->GetCurrentTesterCommData();
//
//	int nPort;
//	int nBaudRate;
//	int nDataBit;
//	int nStopBit;
//	int nParityBit;
//
//
//	CString strLog = _T("");
//	// Luritech Light Controller
//
//	if (m_ctrlLight_PT[0].m_hWnd == NULL)
//	{
//		m_ctrlLight_PT[0].Create();
//		m_ctrlLight_PT[0].InitVariables(0);
//	}
//
//
//
//	nPort = stTester.nPort[2];
//	nBaudRate = stTester.nBaudRate[2];
//	nDataBit = stTester.nDataBit[2];
//	nStopBit = stTester.nStopBit[2];
//	nParityBit = stTester.nParityBit[2];
//
//
//	if (m_ctrlLight_PT[0].OpenPort(nPort, nBaudRate, WM_LIGHT_CONTROL_MSG, nParityBit, nDataBit, nStopBit, 0))
//	{
//		strLog.Format(_T("[Progress] L100 Light 3 Connect Success."));
//		m_btnComponent_Serial_Connect[2].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
//		m_btnComponent_Serial_Connect[2].SetColorStyle(CVGStatic::ColorStyle_Green);
//		m_btnComponent_Serial_DisConnect[2].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
//		m_btnComponent_Serial_DisConnect[2].SetColorStyle(CVGStatic::ColorStyle_Black);
//		WRITE_MAIN_LOG(strLog);
//	}
//	else
//	{
//		strLog.Format(_T("[Progress] L100 Light 3 Connect Fail."));
//		WRITE_MAIN_LOG(strLog);
//	}
//}


//void CDlgComponent::OnBnClickedBtnSerialDisconnect3()
//{
//	m_btnComponent_Serial_DisConnect[2].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
//	m_btnComponent_Serial_DisConnect[2].SetColorStyle(CVGStatic::ColorStyle_Green);
//	m_btnComponent_Serial_Connect[2].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
//	m_btnComponent_Serial_Connect[2].SetColorStyle(CVGStatic::ColorStyle_Black);
//	m_ctrlLight_PT[0].ClosePort();
//}


// 데이타 변경
void CDlgComponent::OnCbnSelchangeComboSerial6()
{
	TESTER_COMM		stTester	= *DATAMANAGER->GetCurrentTesterCommData();
	EQ_BASIC_PARAM	stEq		= *DATAMANAGER->GetCurrentEqBasicData();

	int nSelected;
	m_nLightSelected = m_cbLightController1_Light.GetCurSel();

	for (int i = 0; i < 13; i++)
	{
		//m_nSerial_1_Data[i] = stTester.stLightData[0].nVoltage[nSelected = stEq.nLightSelected][i];
		m_nSerial_1_Data[i] = stTester.stLightData[0].nVoltage[nSelected = m_nLightSelected][i];
	}

	CString strTemp = _T("");
	for (int i = 0; i < 13; i++)
	{
		strTemp.Format(_T("%d"), m_nSerial_1_Data[i]);
		SetDlgItemText(IDC_STC_SERIAL_1_LIGHT_01 + i, strTemp);
	}
}



void CDlgComponent::OnBnClickedBtnPowerConnect1()
{
	TESTER_COMM stTester = *DATAMANAGER->GetCurrentTesterCommData();
	EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();

	int nPort;
	int nBaudRate;
	int nDataBit;
	int nStopBit;
	int nParityBit;

	if (COMMON->GetPermissionGrade() < 1)
	{
		if (stBasic.nLanguage == 0)
			WRITE_MAIN_LOG(_T("권한이 없습니다."));
		else
			WRITE_MAIN_LOG(_T("You don't have Permision."));

		return;
	}

	CString strLog = _T("");

	nPort = stTester.nPort[1];
	nBaudRate = stTester.nBaudRate[1];
	nDataBit = stTester.nDataBit[1];
	nStopBit = stTester.nStopBit[1];
	nParityBit = stTester.nParityBit[1];

	ODA_Q->Disconnect();
	CString sz;

	if (nPort < 10)
	{
		sz.Format(_T("COM%d"), nPort);
	}
	else
	{
		sz.Format(_T("\\\\.\\COM%d"), nPort);
	}

	ODA_Q->Connect(sz, nBaudRate, nParityBit, nStopBit, nDataBit);

	/*if (m_ctrlLight_ODA_Q[0].Is_Connected())
		m_ctrlLight_ODA_Q[0].ClosePort();

	nPort = stTester.nPort[1];
	nBaudRate = stTester.nBaudRate[1];
	nDataBit = stTester.nDataBit[1];
	nStopBit = stTester.nStopBit[1];
	nParityBit = stTester.nParityBit[1];

	if (m_ctrlLight_ODA_Q[0].OpenPort(nPort, nBaudRate, WM_LIGHT_CONTROL_Q_MSG, nParityBit, nDataBit, nStopBit, 0))
	{
		strLog.Format(_T("[Progress] Power Supply Connect Success."));
		m_btnPower_Serial_Connect[0].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_btnPower_Serial_Connect[0].SetColorStyle(CVGStatic::ColorStyle_Green);
		m_btnPower_Serial_DisConnect[0].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_btnPower_Serial_DisConnect[0].SetColorStyle(CVGStatic::ColorStyle_Black);
		WRITE_MAIN_LOG(strLog);
	}
	else
	{
		strLog.Format(_T("[Progress] Power Supply Connect Fail."));
		WRITE_MAIN_LOG(strLog);
	}*/
}

void CDlgComponent::OnBnClickedBtnPowerDisconnect1()
{
	/*m_btnPower_Serial_DisConnect[0].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
	m_btnPower_Serial_DisConnect[0].SetColorStyle(CVGStatic::ColorStyle_Green);
	m_btnPower_Serial_Connect[0].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
	m_btnPower_Serial_Connect[0].SetColorStyle(CVGStatic::ColorStyle_Black);*/
//	m_ctrlLight_ODA_Q[0].ClosePort();
//	ODA_Q->Disconnect();
	ODA_Q->Disconnect();
}

void CDlgComponent::OnBnClickedBtnBcrConnect()
{
	
}

void CDlgComponent::OnBnClickedBtnBcrDisconnect()
{
	g_HandyBCR.Disconnect();
}

void CDlgComponent::OnBnClickedBtnSerial1Onoff01()
{
	// TODO: Add your control notification handler code here
}
