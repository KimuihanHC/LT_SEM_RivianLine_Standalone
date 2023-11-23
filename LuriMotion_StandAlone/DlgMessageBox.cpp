// DlgMessageBox.cpp: 구현 파일
//

#include "pch.h"
#include "LuriMotion_OQA_SFR_Wide.h"
#include "DlgMessageBox.h"
#include "afxdialogex.h"

#include "IO.h"

// CDlgMessageBox 대화 상자

IMPLEMENT_DYNAMIC(CDlgMessageBox, CDialogEx)

CDlgMessageBox::CDlgMessageBox(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_MESSAGEBOX, pParent)
{
	m_strMessage = _T("");
	m_nMode = -1;
}

CDlgMessageBox::~CDlgMessageBox()
{
}

void CDlgMessageBox::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STC_TEST_1, m_stTitle);
	DDX_Control(pDX, IDC_BTN_1, m_ctrlButton[eBtnOk]);
	DDX_Control(pDX, IDC_BTN_2, m_ctrlButton[eBtnCancel]);
	DDX_Control(pDX, IDC_BTN_3, m_ctrlButton[eBtnBuzOff]);

}


BEGIN_MESSAGE_MAP(CDlgMessageBox, CDialogEx)
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BTN_1, &CDlgMessageBox::OnBnClickedBtn1)
	ON_BN_CLICKED(IDC_BTN_2, &CDlgMessageBox::OnBnClickedBtn2)
	ON_BN_CLICKED(IDC_BTN_3, &CDlgMessageBox::OnBnClickedBtn3)
	ON_BN_CLICKED(IDC_BTN_4, &CDlgMessageBox::OnBnClickedBtn4)
END_MESSAGE_MAP()

BOOL CDlgMessageBox::PreTranslateMessage(MSG* pMsg)
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
int CDlgMessageBox::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	//InitComponent_MessageBox();

	return 0;
}
// CDlgMessageBox 메시지 처리기
BOOL CDlgMessageBox::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CRect rect;
	GetClientRect(rect);
	//pDC->FillSolidRect(rect, RGB(105, 186, 235));
	//pDC->FillSolidRect(rect, RGB(83, 169, 155));
	pDC->FillSolidRect(rect, RGB(70, 70, 70));		// Lime
	return TRUE;
}
BOOL CDlgMessageBox::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	InitComponent_MessageBox();

	ChangeComponent_MessageBox();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgMessageBox::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (bShow == TRUE)
	{
		//SetTimer(55, rand() % 5000, NULL);
	}
	else
	{
	}
}

void CDlgMessageBox::OnTimer(UINT_PTR nIDEvent)
{
	KillTimer(55);
	EndDialog(IDOK);
	CDialogEx::OnTimer(nIDEvent);
}
// CDlgMessageBox 메시지 처리기
void CDlgMessageBox::InitComponent_MessageBox()
{
	CString strTemp = _T("");

	m_stTitle.SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
	//m_stTitle.SetColorStyle(CVGStatic::ColorStyle_Yellow);
	m_stTitle.SetFont_Gdip(L"Arial", 12.0F);
}
void CDlgMessageBox::ChangeComponent_MessageBox()
{
	CString strTemp = _T("");
	if (m_nMode == (int)MODE_QUESTION)
	{
		SetWindowText(_T("QUESTION"));
		m_stTitle.SetColorStyle(CVGStatic::ColorStyle_White);
		m_stTitle.SetText(m_strMessage);
		m_nButton = BUTTON_OK | BUTTON_CANCEL;
	}
	else if (m_nMode == (int)MODE_ERROR)
	{
		SetWindowText(_T("ERROR"));
		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_09_TOWERLAMP_BEEP1, TRUE);
		m_stTitle.SetColorStyle(CVGStatic::ColorStyle_Orange);
		m_stTitle.SetText(m_strMessage);
		m_nButton = BUTTON_OK | BUTTON_BUZZER_OFF;
	}
	else if (m_nMode == (int)MODE_INFORMATION)
	{
		SetWindowText(_T("INFORMATION"));
		m_stTitle.SetColorStyle(CVGStatic::ColorStyle_SlateGray);
		m_stTitle.SetText(m_strMessage);
		m_nButton = BUTTON_OK;
	}
	else
	{
		m_stTitle.SetColorStyle(CVGStatic::ColorStyle_Navy);
		m_stTitle.SetText(m_strMessage);
		m_nButton = BUTTON_ALL;
	}
	// 버튼숨김
	for (int i = 0; i < eBtn_Max; i++)
	{
		if ((BUTTON_OK & (m_nButton >> i)) == BUTTON_OK)
			m_ctrlButton[i].ShowWindow(TRUE);
		else
			m_ctrlButton[i].ShowWindow(FALSE);
	}
}



void CDlgMessageBox::SetMsg(CString str, int nMode)
{
	m_strMessage = str;
	m_nMode = nMode;

	//ChangeComponent_MessageBox();
}

void CDlgMessageBox::OnBnClickedBtn1()
{
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_09_TOWERLAMP_BEEP1, FALSE);
#if(GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL) || (GET_INSPECTOR == SYS_OQC_SFR_SINGLE_CL) || (GET_INSPECTOR == SYS_DISTORTION_30)
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_0B_SLEEP_MODE_ON, FALSE);
#elif(GET_INSPECTOR == SYS_FILM_PEEL_OFF)
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_11_SLEEP_MODE_ON, FALSE);
#endif
	COMMON->SetSleepStarted(GetTickCount64());
	EndDialog(IDOK);
}
void CDlgMessageBox::OnBnClickedBtn2()
{
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_09_TOWERLAMP_BEEP1, FALSE);
#if(GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL) || (GET_INSPECTOR == SYS_OQC_SFR_SINGLE_CL) || (GET_INSPECTOR == SYS_DISTORTION_30)
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_0B_SLEEP_MODE_ON, FALSE);
#elif(GET_INSPECTOR == SYS_FILM_PEEL_OFF)
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_11_SLEEP_MODE_ON, FALSE);
#endif
	COMMON->SetSleepStarted(GetTickCount64());
	EndDialog(IDCANCEL);
}
void CDlgMessageBox::OnBnClickedBtn3()
{
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_09_TOWERLAMP_BEEP1, FALSE);
#if(GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL) || (GET_INSPECTOR == SYS_OQC_SFR_SINGLE_CL) || (GET_INSPECTOR == SYS_DISTORTION_30)
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_0B_SLEEP_MODE_ON, FALSE);
#elif(GET_INSPECTOR == SYS_FILM_PEEL_OFF)
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_11_SLEEP_MODE_ON, FALSE);
#endif
	COMMON->SetSleepStarted(GetTickCount64());
}

void CDlgMessageBox::OnBnClickedBtn4()
{
	CIO::IO_Door_All_Lock(true);
	COMMON->SetSleepStarted(GetTickCount64());
}
