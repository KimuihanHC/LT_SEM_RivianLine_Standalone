// DlgTowerLamp.cpp: 구현 파일
//

#include "pch.h"
#include "LuriMotion_OQA_SFR_Wide.h"
#include "DlgTowerLamp.h"
#include "afxdialogex.h"


// CDlgTowerLamp 대화 상자

IMPLEMENT_DYNAMIC(CDlgTowerLamp, CDialogEx)

CDlgTowerLamp::CDlgTowerLamp(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_TOWERLAMP, pParent)
{

}

CDlgTowerLamp::~CDlgTowerLamp()
{
}

void CDlgTowerLamp::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STC_TITLE_TOWERLAMP_1, m_stTitle);

	DDX_Control(pDX, IDC_STC_TOWERLAMP_COLOR_1, m_stTitle_Sub_Lamp[0]);
	DDX_Control(pDX, IDC_STC_TOWERLAMP_COLOR_2, m_stTitle_Sub_Lamp[1]);
	DDX_Control(pDX, IDC_STC_TOWERLAMP_COLOR_3, m_stTitle_Sub_Lamp[2]);
	DDX_Control(pDX, IDC_STC_TOWERLAMP_COLOR_4, m_stTitle_Sub_Lamp[3]);


	DDX_Control(pDX, IDC_STC_TOWERLMAP_MODE_1, m_stTitle_Sub_Mode[0]);
	DDX_Control(pDX, IDC_STC_TOWERLMAP_MODE_2, m_stTitle_Sub_Mode[1]);
	DDX_Control(pDX, IDC_STC_TOWERLMAP_MODE_3, m_stTitle_Sub_Mode[2]);
	DDX_Control(pDX, IDC_STC_TOWERLMAP_MODE_4, m_stTitle_Sub_Mode[3]);
	DDX_Control(pDX, IDC_STC_TOWERLMAP_MODE_5, m_stTitle_Sub_Mode[4]);
	DDX_Control(pDX, IDC_STC_TOWERLMAP_MODE_6, m_stTitle_Sub_Mode[5]);

	DDX_Control(pDX, IDC_BTN_GREEN_1, m_btnTowerLamp_Setting[0][0]);
	DDX_Control(pDX, IDC_BTN_GREEN_2, m_btnTowerLamp_Setting[1][0]);
	DDX_Control(pDX, IDC_BTN_GREEN_3, m_btnTowerLamp_Setting[2][0]);
	DDX_Control(pDX, IDC_BTN_GREEN_4, m_btnTowerLamp_Setting[3][0]);
	DDX_Control(pDX, IDC_BTN_GREEN_5, m_btnTowerLamp_Setting[4][0]);
	DDX_Control(pDX, IDC_BTN_GREEN_6, m_btnTowerLamp_Setting[5][0]);

	DDX_Control(pDX, IDC_BTN_YELLOW_1, m_btnTowerLamp_Setting[0][1]);
	DDX_Control(pDX, IDC_BTN_YELLOW_2, m_btnTowerLamp_Setting[1][1]);
	DDX_Control(pDX, IDC_BTN_YELLOW_3, m_btnTowerLamp_Setting[2][1]);
	DDX_Control(pDX, IDC_BTN_YELLOW_4, m_btnTowerLamp_Setting[3][1]);
	DDX_Control(pDX, IDC_BTN_YELLOW_5, m_btnTowerLamp_Setting[4][1]);
	DDX_Control(pDX, IDC_BTN_YELLOW_6, m_btnTowerLamp_Setting[5][1]);

	DDX_Control(pDX, IDC_BTN_RED_1, m_btnTowerLamp_Setting[0][2]);
	DDX_Control(pDX, IDC_BTN_RED_2, m_btnTowerLamp_Setting[1][2]);
	DDX_Control(pDX, IDC_BTN_RED_3, m_btnTowerLamp_Setting[2][2]);
	DDX_Control(pDX, IDC_BTN_RED_4, m_btnTowerLamp_Setting[3][2]);
	DDX_Control(pDX, IDC_BTN_RED_5, m_btnTowerLamp_Setting[4][2]);
	DDX_Control(pDX, IDC_BTN_RED_6, m_btnTowerLamp_Setting[5][2]);

	DDX_Control(pDX, IDC_BTN_BUZZER_1, m_btnTowerLamp_Setting[0][3]);
	DDX_Control(pDX, IDC_BTN_BUZZER_2, m_btnTowerLamp_Setting[1][3]);
	DDX_Control(pDX, IDC_BTN_BUZZER_3, m_btnTowerLamp_Setting[2][3]);
	DDX_Control(pDX, IDC_BTN_BUZZER_4, m_btnTowerLamp_Setting[3][3]);
	DDX_Control(pDX, IDC_BTN_BUZZER_5, m_btnTowerLamp_Setting[4][3]);
	DDX_Control(pDX, IDC_BTN_BUZZER_6, m_btnTowerLamp_Setting[5][3]);

}


BEGIN_MESSAGE_MAP(CDlgTowerLamp, CDialogEx)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON5, &CDlgTowerLamp::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CDlgTowerLamp::OnBnClickedButton6)

	ON_BN_CLICKED(IDC_BTN_GREEN_1, &CDlgTowerLamp::OnBtnClickedGreen_Run)
	ON_BN_CLICKED(IDC_BTN_YELLOW_1, &CDlgTowerLamp::OnBtnClickedYellow_Run)
	ON_BN_CLICKED(IDC_BTN_RED_1, &CDlgTowerLamp::OnBtnClickedRed_Run)
	ON_BN_CLICKED(IDC_BTN_BUZZER_1, &CDlgTowerLamp::OnBtnClickedBuzzer_Run)

	ON_BN_CLICKED(IDC_BTN_GREEN_2, &CDlgTowerLamp::OnBtnClickedGreen_Stop)
	ON_BN_CLICKED(IDC_BTN_YELLOW_2, &CDlgTowerLamp::OnBtnClickedYellow_Stop)
	ON_BN_CLICKED(IDC_BTN_RED_2, &CDlgTowerLamp::OnBtnClickedRed_Stop)
	ON_BN_CLICKED(IDC_BTN_BUZZER_2, &CDlgTowerLamp::OnBtnClickedBuzzer_Stop)

	ON_BN_CLICKED(IDC_BTN_GREEN_3, &CDlgTowerLamp::OnBtnClickedGreen_Error)
	ON_BN_CLICKED(IDC_BTN_YELLOW_3, &CDlgTowerLamp::OnBtnClickedYellow_Error)
	ON_BN_CLICKED(IDC_BTN_RED_3, &CDlgTowerLamp::OnBtnClickedRed_Error)
	ON_BN_CLICKED(IDC_BTN_BUZZER_3, &CDlgTowerLamp::OnBtnClickedBuzzer_Error)

	ON_BN_CLICKED(IDC_BTN_GREEN_4, &CDlgTowerLamp::OnBtnClickedGreen_Ready)
	ON_BN_CLICKED(IDC_BTN_YELLOW_4, &CDlgTowerLamp::OnBtnClickedYellow_Ready)
	ON_BN_CLICKED(IDC_BTN_RED_4, &CDlgTowerLamp::OnBtnClickedRed_Ready)
	ON_BN_CLICKED(IDC_BTN_BUZZER_4, &CDlgTowerLamp::OnBtnClickedBuzzer_Ready)

	ON_BN_CLICKED(IDC_BTN_GREEN_5, &CDlgTowerLamp::OnBtnClickedGreen_Call)
	ON_BN_CLICKED(IDC_BTN_YELLOW_5, &CDlgTowerLamp::OnBtnClickedYellow_Call)
	ON_BN_CLICKED(IDC_BTN_RED_5, &CDlgTowerLamp::OnBtnClickedRed_Call)
	ON_BN_CLICKED(IDC_BTN_BUZZER_5, &CDlgTowerLamp::OnBtnClickedBuzzer_Call)

	ON_BN_CLICKED(IDC_BTN_GREEN_6, &CDlgTowerLamp::OnBtnClickedGreen_Pause)
	ON_BN_CLICKED(IDC_BTN_YELLOW_6, &CDlgTowerLamp::OnBtnClickedYellow_Pause)
	ON_BN_CLICKED(IDC_BTN_RED_6, &CDlgTowerLamp::OnBtnClickedRed_Pause)
	ON_BN_CLICKED(IDC_BTN_BUZZER_6, &CDlgTowerLamp::OnBtnClickedBuzzer_Pause)

END_MESSAGE_MAP()


// CDlgTowerLamp 메시지 처리기
BOOL CDlgTowerLamp::PreTranslateMessage(MSG* pMsg)
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
int CDlgTowerLamp::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	//InitComponent_MessageBox();

	return 0;
}

BOOL CDlgTowerLamp::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CRect rect;
	GetClientRect(rect);
	//pDC->FillSolidRect(rect, RGB(105, 186, 235));
	pDC->FillSolidRect(rect, RGB(83, 169, 155));
	//pDC->FillSolidRect(rect, RGB(227, 255, 0));		// Lime
	return TRUE;
}
BOOL CDlgTowerLamp::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	InitComponnet_TowerLamp();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
void CDlgTowerLamp::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (bShow)
	{
		Load_TowerLamp();
	}
}
void CDlgTowerLamp::Load_TowerLamp()
{
	EQ_TOWERLAMP	stTowerLamp = *DATAMANAGER->GetCurrentTowerLampData();

	for (int i = 0; i < (int)eModeMax; i++)
	{
		for (int j = 0; j < (int)eTowerMax; j++)
		{
			m_nTowerLampSetting[i][j] = stTowerLamp.nTowerLampSetting[i][j];
			ChangeButtonStatus(i, j);
		}
	}
}

void CDlgTowerLamp::InitComponnet_TowerLamp()
{

	m_stTitle.SetStaticStyle(CVGStatic::StaticStyle_GroupHeader);
	m_stTitle.SetColorStyle(CVGStatic::ColorStyle_Black);
	m_stTitle.SetFont_Gdip(L"Arial", 12.0F);
	m_stTitle.SetText(_T("Tower Lamp Setting"));

	for (int i = 0; i < 6; i++)
	{
		m_stTitle_Sub_Mode[i].SetStaticStyle(CVGStatic::StaticStyle_GroupHeader);
		m_stTitle_Sub_Mode[i].SetColorStyle(CVGStatic::ColorStyle_DeepDarkGray);
		m_stTitle_Sub_Mode[i].SetFont_Gdip(L"Arial", 10.0F);
		m_stTitle_Sub_Mode[i].SetText(m_szTowerLamp[i]);
	}

	for (int i = 0; i < 4; i++)
	{
		m_stTitle_Sub_Lamp[i].SetStaticStyle(CVGStatic::StaticStyle_GroupHeader);
		m_stTitle_Sub_Lamp[i].SetColorStyle(CVGStatic::ColorStyle_DeepDarkGray);

		m_stTitle_Sub_Lamp[i].SetFont_Gdip(L"Arial", 10.0F);
		m_stTitle_Sub_Lamp[i].SetText(m_szToweColor[i]);
	}

	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			m_btnTowerLamp_Setting[i][j].SetStaticStyle(CVGStatic::StaticStyle_Data);
			m_btnTowerLamp_Setting[i][j].SetText(_T("OFF"));
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// RUN MODE
void CDlgTowerLamp::OnBtnClickedGreen_Run()
{
	if (m_nTowerLampSetting[eModeRun][eTowerRed] == eTowerLamp_Off)
		m_nTowerLampSetting[eModeRun][eTowerRed] = eTowerLamp_On;
	else if (m_nTowerLampSetting[eModeRun][eTowerRed] == eTowerLamp_On)
		m_nTowerLampSetting[eModeRun][eTowerRed] = eTowerLamp_Blink;
	else if (m_nTowerLampSetting[eModeRun][eTowerRed] == eTowerLamp_Blink)
		m_nTowerLampSetting[eModeRun][eTowerRed] = eTowerLamp_Off;

	ChangeButtonStatus((int)eModeRun, (int)eTowerRed);

	
}
void CDlgTowerLamp::OnBtnClickedYellow_Run()
{
	if (m_nTowerLampSetting[eModeRun][eTowerYellow] == eTowerLamp_Off)
		m_nTowerLampSetting[eModeRun][eTowerYellow] = eTowerLamp_On;
	else if (m_nTowerLampSetting[eModeRun][eTowerYellow] == eTowerLamp_On)
		m_nTowerLampSetting[eModeRun][eTowerYellow] = eTowerLamp_Blink;
	else if (m_nTowerLampSetting[eModeRun][eTowerYellow] == eTowerLamp_Blink)
		m_nTowerLampSetting[eModeRun][eTowerYellow] = eTowerLamp_Off;

	ChangeButtonStatus((int)eModeRun, (int)eTowerYellow);
}

void CDlgTowerLamp::OnBtnClickedRed_Run()
{
	if (m_nTowerLampSetting[eModeRun][eTowerGreen] == eTowerLamp_Off)
		m_nTowerLampSetting[eModeRun][eTowerGreen] = eTowerLamp_On;
	else if (m_nTowerLampSetting[eModeRun][eTowerGreen] == eTowerLamp_On)
		m_nTowerLampSetting[eModeRun][eTowerGreen] = eTowerLamp_Blink;
	else if (m_nTowerLampSetting[eModeRun][eTowerGreen] == eTowerLamp_Blink)
		m_nTowerLampSetting[eModeRun][eTowerGreen] = eTowerLamp_Off;

	ChangeButtonStatus((int)eModeRun, (int)eTowerGreen);
}
void CDlgTowerLamp::OnBtnClickedBuzzer_Run()
{
	if (m_nTowerLampSetting[eModeRun][eTowerBuzzer] == eTowerLamp_Off)
		m_nTowerLampSetting[eModeRun][eTowerBuzzer] = eTowerLamp_On;
	else if (m_nTowerLampSetting[eModeRun][eTowerBuzzer] == eTowerLamp_On)
		m_nTowerLampSetting[eModeRun][eTowerBuzzer] = eTowerLamp_Off;

	ChangeButtonStatus((int)eModeRun, (int)eTowerBuzzer);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// READY MODE
void CDlgTowerLamp::OnBtnClickedGreen_Ready()
{
	if (m_nTowerLampSetting[eModeReady][eTowerRed] == eTowerLamp_Off)
		m_nTowerLampSetting[eModeReady][eTowerRed] = eTowerLamp_On;
	else if (m_nTowerLampSetting[eModeReady][eTowerRed] == eTowerLamp_On)
		m_nTowerLampSetting[eModeReady][eTowerRed] = eTowerLamp_Blink;
	else if (m_nTowerLampSetting[eModeReady][eTowerRed] == eTowerLamp_Blink)
		m_nTowerLampSetting[eModeReady][eTowerRed] = eTowerLamp_Off;

	ChangeButtonStatus((int)eModeReady, (int)eTowerRed);
}
void CDlgTowerLamp::OnBtnClickedYellow_Ready()
{
	if (m_nTowerLampSetting[eModeReady][eTowerYellow] == eTowerLamp_Off)
		m_nTowerLampSetting[eModeReady][eTowerYellow] = eTowerLamp_On;
	else if (m_nTowerLampSetting[eModeReady][eTowerYellow] == eTowerLamp_On)
		m_nTowerLampSetting[eModeReady][eTowerYellow] = eTowerLamp_Blink;
	else if (m_nTowerLampSetting[eModeReady][eTowerYellow] == eTowerLamp_Blink)
		m_nTowerLampSetting[eModeReady][eTowerYellow] = eTowerLamp_Off;

	ChangeButtonStatus((int)eModeReady, (int)eTowerYellow);
}
void CDlgTowerLamp::OnBtnClickedRed_Ready()
{
	if (m_nTowerLampSetting[eModeReady][eTowerGreen] == eTowerLamp_Off)
		m_nTowerLampSetting[eModeReady][eTowerGreen] = eTowerLamp_On;
	else if (m_nTowerLampSetting[eModeReady][eTowerGreen] == eTowerLamp_On)
		m_nTowerLampSetting[eModeReady][eTowerGreen] = eTowerLamp_Blink;
	else if (m_nTowerLampSetting[eModeReady][eTowerGreen] == eTowerLamp_Blink)
		m_nTowerLampSetting[eModeReady][eTowerGreen] = eTowerLamp_Off;

	ChangeButtonStatus((int)eModeReady, (int)eTowerGreen);
}
void CDlgTowerLamp::OnBtnClickedBuzzer_Ready()
{
	if (m_nTowerLampSetting[eModeReady][eTowerBuzzer] == eTowerLamp_Off)
		m_nTowerLampSetting[eModeReady][eTowerBuzzer] = eTowerLamp_On;
	else if (m_nTowerLampSetting[eModeReady][eTowerBuzzer] == eTowerLamp_On)
		m_nTowerLampSetting[eModeReady][eTowerBuzzer] = eTowerLamp_Off;

	ChangeButtonStatus((int)eModeReady, (int)eTowerBuzzer);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ERROR MODE
void CDlgTowerLamp::OnBtnClickedGreen_Error()
{
	if (m_nTowerLampSetting[eModeError][eTowerRed] == eTowerLamp_Off)
		m_nTowerLampSetting[eModeError][eTowerRed] = eTowerLamp_On;
	else if (m_nTowerLampSetting[eModeError][eTowerRed] == eTowerLamp_On)
		m_nTowerLampSetting[eModeError][eTowerRed] = eTowerLamp_Blink;
	else if (m_nTowerLampSetting[eModeError][eTowerRed] == eTowerLamp_Blink)
		m_nTowerLampSetting[eModeError][eTowerRed] = eTowerLamp_Off;

	ChangeButtonStatus((int)eModeError, (int)eTowerRed);

	
}
void CDlgTowerLamp::OnBtnClickedYellow_Error()
{
	if (m_nTowerLampSetting[eModeError][eTowerYellow] == eTowerLamp_Off)
		m_nTowerLampSetting[eModeError][eTowerYellow] = eTowerLamp_On;
	else if (m_nTowerLampSetting[eModeError][eTowerYellow] == eTowerLamp_On)
		m_nTowerLampSetting[eModeError][eTowerYellow] = eTowerLamp_Blink;
	else if (m_nTowerLampSetting[eModeError][eTowerYellow] == eTowerLamp_Blink)
		m_nTowerLampSetting[eModeError][eTowerYellow] = eTowerLamp_Off;

	ChangeButtonStatus((int)eModeError, (int)eTowerYellow);
}
void CDlgTowerLamp::OnBtnClickedRed_Error()
{
	if (m_nTowerLampSetting[eModeError][eTowerGreen] == eTowerLamp_Off)
		m_nTowerLampSetting[eModeError][eTowerGreen] = eTowerLamp_On;
	else if (m_nTowerLampSetting[eModeError][eTowerGreen] == eTowerLamp_On)
		m_nTowerLampSetting[eModeError][eTowerGreen] = eTowerLamp_Blink;
	else if (m_nTowerLampSetting[eModeError][eTowerGreen] == eTowerLamp_Blink)
		m_nTowerLampSetting[eModeError][eTowerGreen] = eTowerLamp_Off;

	ChangeButtonStatus((int)eModeError, (int)eTowerGreen);
}
void CDlgTowerLamp::OnBtnClickedBuzzer_Error()
{
	if (m_nTowerLampSetting[eModeError][eTowerBuzzer] == eTowerLamp_Off)
		m_nTowerLampSetting[eModeError][eTowerBuzzer] = eTowerLamp_On;
	else if (m_nTowerLampSetting[eModeError][eTowerBuzzer] == eTowerLamp_On)
		m_nTowerLampSetting[eModeError][eTowerBuzzer] = eTowerLamp_Off;

	ChangeButtonStatus((int)eModeError, (int)eTowerBuzzer);
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// STOP MODE
void CDlgTowerLamp::OnBtnClickedGreen_Stop()
{
	if (m_nTowerLampSetting[eModeStop][eTowerRed] == eTowerLamp_Off)
		m_nTowerLampSetting[eModeStop][eTowerRed] = eTowerLamp_On;
	else if (m_nTowerLampSetting[eModeStop][eTowerRed] == eTowerLamp_On)
		m_nTowerLampSetting[eModeStop][eTowerRed] = eTowerLamp_Blink;
	else if (m_nTowerLampSetting[eModeStop][eTowerRed] == eTowerLamp_Blink)
		m_nTowerLampSetting[eModeStop][eTowerRed] = eTowerLamp_Off;

	ChangeButtonStatus((int)eModeStop, (int)eTowerRed);

}
void CDlgTowerLamp::OnBtnClickedYellow_Stop()
{
	if (m_nTowerLampSetting[eModeStop][eTowerYellow] == eTowerLamp_Off)
		m_nTowerLampSetting[eModeStop][eTowerYellow] = eTowerLamp_On;
	else if (m_nTowerLampSetting[eModeStop][eTowerYellow] == eTowerLamp_On)
		m_nTowerLampSetting[eModeStop][eTowerYellow] = eTowerLamp_Blink;
	else if (m_nTowerLampSetting[eModeStop][eTowerYellow] == eTowerLamp_Blink)
		m_nTowerLampSetting[eModeStop][eTowerYellow] = eTowerLamp_Off;

	ChangeButtonStatus((int)eModeStop, (int)eTowerYellow);
}
void CDlgTowerLamp::OnBtnClickedRed_Stop()
{
	if (m_nTowerLampSetting[eModeStop][eTowerGreen] == eTowerLamp_Off)
		m_nTowerLampSetting[eModeStop][eTowerGreen] = eTowerLamp_On;
	else if (m_nTowerLampSetting[eModeStop][eTowerGreen] == eTowerLamp_On)
		m_nTowerLampSetting[eModeStop][eTowerGreen] = eTowerLamp_Blink;
	else if (m_nTowerLampSetting[eModeStop][eTowerGreen] == eTowerLamp_Blink)
		m_nTowerLampSetting[eModeStop][eTowerGreen] = eTowerLamp_Off;

	ChangeButtonStatus((int)eModeStop, (int)eTowerGreen);

}
void CDlgTowerLamp::OnBtnClickedBuzzer_Stop()
{
	if (m_nTowerLampSetting[eModeStop][eTowerBuzzer] == eTowerLamp_Off)
		m_nTowerLampSetting[eModeStop][eTowerBuzzer] = eTowerLamp_On;
	else if (m_nTowerLampSetting[eModeStop][eTowerBuzzer] == eTowerLamp_On)
		m_nTowerLampSetting[eModeStop][eTowerBuzzer] = eTowerLamp_Off;

	ChangeButtonStatus((int)eModeStop, (int)eTowerBuzzer);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CALL MODE
void CDlgTowerLamp::OnBtnClickedGreen_Call()
{
	if (m_nTowerLampSetting[eModeCall][eTowerRed] == eTowerLamp_Off)
		m_nTowerLampSetting[eModeCall][eTowerRed] = eTowerLamp_On;
	else if (m_nTowerLampSetting[eModeCall][eTowerRed] == eTowerLamp_On)
		m_nTowerLampSetting[eModeCall][eTowerRed] = eTowerLamp_Blink;
	else if (m_nTowerLampSetting[eModeCall][eTowerRed] == eTowerLamp_Blink)
		m_nTowerLampSetting[eModeCall][eTowerRed] = eTowerLamp_Off;

	ChangeButtonStatus((int)eModeCall, (int)eTowerRed);

}
void CDlgTowerLamp::OnBtnClickedYellow_Call()
{
	if (m_nTowerLampSetting[eModeCall][eTowerYellow] == eTowerLamp_Off)
		m_nTowerLampSetting[eModeCall][eTowerYellow] = eTowerLamp_On;
	else if (m_nTowerLampSetting[eModeCall][eTowerYellow] == eTowerLamp_On)
		m_nTowerLampSetting[eModeCall][eTowerYellow] = eTowerLamp_Blink;
	else if (m_nTowerLampSetting[eModeCall][eTowerYellow] == eTowerLamp_Blink)
		m_nTowerLampSetting[eModeCall][eTowerYellow] = eTowerLamp_Off;

	ChangeButtonStatus((int)eModeCall, (int)eTowerYellow);
}
void CDlgTowerLamp::OnBtnClickedRed_Call()
{
	if (m_nTowerLampSetting[eModeCall][eTowerGreen] == eTowerLamp_Off)
		m_nTowerLampSetting[eModeCall][eTowerGreen] = eTowerLamp_On;
	else if (m_nTowerLampSetting[eModeCall][eTowerGreen] == eTowerLamp_On)
		m_nTowerLampSetting[eModeCall][eTowerGreen] = eTowerLamp_Blink;
	else if (m_nTowerLampSetting[eModeCall][eTowerGreen] == eTowerLamp_Blink)
		m_nTowerLampSetting[eModeCall][eTowerGreen] = eTowerLamp_Off;

	ChangeButtonStatus((int)eModeCall, (int)eTowerGreen);
}
void CDlgTowerLamp::OnBtnClickedBuzzer_Call()
{
	if (m_nTowerLampSetting[eModeCall][eTowerBuzzer] == eTowerLamp_Off)
		m_nTowerLampSetting[eModeCall][eTowerBuzzer] = eTowerLamp_On;
	else if (m_nTowerLampSetting[eModeCall][eTowerBuzzer] == eTowerLamp_On)
		m_nTowerLampSetting[eModeCall][eTowerBuzzer] = eTowerLamp_Off;

	ChangeButtonStatus((int)eModeCall, (int)eTowerBuzzer);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PAUSE MODE
void CDlgTowerLamp::OnBtnClickedGreen_Pause()
{
	if (m_nTowerLampSetting[eModePause][eTowerRed] == eTowerLamp_Off)
		m_nTowerLampSetting[eModePause][eTowerRed] = eTowerLamp_On;
	else if (m_nTowerLampSetting[eModePause][eTowerRed] == eTowerLamp_On)
		m_nTowerLampSetting[eModePause][eTowerRed] = eTowerLamp_Blink;
	else if (m_nTowerLampSetting[eModePause][eTowerRed] == eTowerLamp_Blink)
		m_nTowerLampSetting[eModePause][eTowerRed] = eTowerLamp_Off;

	ChangeButtonStatus((int)eModePause, (int)eTowerRed);
	
}
void CDlgTowerLamp::OnBtnClickedYellow_Pause()
{
	if (m_nTowerLampSetting[eModePause][eTowerYellow] == eTowerLamp_Off)
		m_nTowerLampSetting[eModePause][eTowerYellow] = eTowerLamp_On;
	else if (m_nTowerLampSetting[eModePause][eTowerYellow] == eTowerLamp_On)
		m_nTowerLampSetting[eModePause][eTowerYellow] = eTowerLamp_Blink;
	else if (m_nTowerLampSetting[eModePause][eTowerYellow] == eTowerLamp_Blink)
		m_nTowerLampSetting[eModePause][eTowerYellow] = eTowerLamp_Off;

	ChangeButtonStatus((int)eModePause, (int)eTowerYellow);
}
void CDlgTowerLamp::OnBtnClickedRed_Pause()
{
	if (m_nTowerLampSetting[eModePause][eTowerGreen] == eTowerLamp_Off)
		m_nTowerLampSetting[eModePause][eTowerGreen] = eTowerLamp_On;
	else if (m_nTowerLampSetting[eModePause][eTowerGreen] == eTowerLamp_On)
		m_nTowerLampSetting[eModePause][eTowerGreen] = eTowerLamp_Blink;
	else if (m_nTowerLampSetting[eModePause][eTowerGreen] == eTowerLamp_Blink)
		m_nTowerLampSetting[eModePause][eTowerGreen] = eTowerLamp_Off;

	ChangeButtonStatus((int)eModePause, (int)eTowerGreen);
}
void CDlgTowerLamp::OnBtnClickedBuzzer_Pause()
{
	if (m_nTowerLampSetting[eModePause][eTowerBuzzer] == eTowerLamp_Off)
		m_nTowerLampSetting[eModePause][eTowerBuzzer] = eTowerLamp_On;
	else if (m_nTowerLampSetting[eModePause][eTowerBuzzer] == eTowerLamp_On)
		m_nTowerLampSetting[eModePause][eTowerBuzzer] = eTowerLamp_Off;

	ChangeButtonStatus((int)eModePause, (int)eTowerBuzzer);
}
void CDlgTowerLamp::ChangeButtonStatus(int nMode, int nColor)
{
	CString strTitle = _T("");

	if (m_nTowerLampSetting[nMode][nColor] == eTowerLamp_Off)
	{
		strTitle = _T("OFF");

		m_btnTowerLamp_Setting[nMode][nColor].SetStaticStyle(CVGStatic::StaticStyle_Data);

	}
	else if (m_nTowerLampSetting[nMode][nColor] == eTowerLamp_On)
	{
		strTitle = _T("ON");

		m_btnTowerLamp_Setting[nMode][nColor].SetStaticStyle(CVGStatic::StaticStyle_Default);
		m_btnTowerLamp_Setting[nMode][nColor].SetColorStyle(CVGStatic::ColorStyle_Green);
	}
	else if (m_nTowerLampSetting[nMode][nColor] == eTowerLamp_Blink)
	{
		strTitle = _T("BLINK");

		m_btnTowerLamp_Setting[nMode][nColor].SetStaticStyle(CVGStatic::StaticStyle_Default);
		m_btnTowerLamp_Setting[nMode][nColor].SetColorStyle(CVGStatic::ColorStyle_Navy);
	}

	m_btnTowerLamp_Setting[nMode][nColor].SetText(strTitle);
}

void CDlgTowerLamp::OnBnClickedButton5()
{
	CString strTemp = _T("");
	int		nTempData = 0;

	EQ_TOWERLAMP	stTowerLamp = *DATAMANAGER->GetCurrentTowerLampData();

	for (int i = 0; i < (int)eModeMax; i++)
	{
		for (int j = 0; j < (int)eTowerMax; j++)
		{
			stTowerLamp.nTowerLampSetting[i][j] = m_nTowerLampSetting[i][j];
		}
	}

	DATAMANAGER->SetstTower(stTowerLamp);
}


void CDlgTowerLamp::OnBnClickedButton6()
{
	CDialogEx::OnOK();
}
