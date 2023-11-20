// DlgSleepMode.cpp: 구현 파일
//

#include "pch.h"
#include "LuriMotion_OQA_SFR_Wide.h"
#include "DlgSleepMode.h"
#include "afxdialogex.h"


// CDlgSleepMode 대화 상자

IMPLEMENT_DYNAMIC(CDlgSleepMode, CDialogEx)

CDlgSleepMode::CDlgSleepMode(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_SLEEP_MODE, pParent)
{

}

CDlgSleepMode::~CDlgSleepMode()
{
}

void CDlgSleepMode::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STC_SLEEP_TITLE, m_stSleepTitle);
	DDX_Control(pDX, IDC_BUTTON1, m_btnOk);
}


BEGIN_MESSAGE_MAP(CDlgSleepMode, CDialogEx)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgSleepMode::OnBnClickedButton1)
END_MESSAGE_MAP()


// CDlgLanguage 메시지 처리기
BOOL CDlgSleepMode::PreTranslateMessage(MSG* pMsg)
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
int CDlgSleepMode::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	//InitComponnet_SleepMode();

	return 0;
}

BOOL CDlgSleepMode::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CRect rect;
	GetClientRect(rect);
	//pDC->FillSolidRect(rect, RGB(105, 186, 235));
	// pDC->FillSolidRect(rect, RGB(255, 255, 255));
	//pDC->FillSolidRect(rect, RGB(227, 255, 0));		// Lime
	return TRUE;
}
BOOL CDlgSleepMode::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
void CDlgSleepMode::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (bShow)
	{
		InitComponnet_SleepMode();
		//SetTimer(56, rand() % 10000, NULL);

	}
}

void CDlgSleepMode::OnTimer(UINT_PTR nIDEvent)
{
	KillTimer(56);
	EndDialog(IDOK);
	CDialogEx::OnTimer(nIDEvent);
}

void CDlgSleepMode::InitComponnet_SleepMode()
{
	m_stSleepTitle.SetStaticStyle(CVGStatic::StaticStyle_GroupHeader);
	m_stSleepTitle.SetColorStyle(CVGStatic::ColorStyle_Black);
	m_stSleepTitle.SetFont_Gdip(L"Arial", 38.0F);
	m_stSleepTitle.SetText(_T("Sleep Mode"));

	m_btnOk.SetStaticStyle(CVGStatic::StaticStyle_Title);
	m_btnOk.SetColorStyle(CVGStatic::ColorStyle_Black);
	m_btnOk.SetFont_Gdip(L"Arial", 38.0F);
	m_btnOk.SetText(_T("Close"));
}


void CDlgSleepMode::OnBnClickedButton1()
{
	OnOK();
}
