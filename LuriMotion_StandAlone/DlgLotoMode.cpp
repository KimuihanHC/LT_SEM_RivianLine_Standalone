// DlgLotoMode.cpp: 구현 파일
//

#include "pch.h"
#include "LuriMotion_OQA_SFR_Wide.h"
#include "DlgLotoMode.h"
#include "afxdialogex.h"

#define TIMER_TITLE_FLICK   100

// CDlgLotoMode 대화 상자

IMPLEMENT_DYNAMIC(CDlgLotoMode, CDialogEx)

CDlgLotoMode::CDlgLotoMode(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_LOTO_MODE, pParent)
{
	VERIFY(m_font_Large.CreateFont(
		16,							// nHeight
		0,							// nWidth
		0,							// nEscapement
		0,							// nOrientation
		FW_BOLD,					// nWeight
		FALSE,						// bItalic
		FALSE,						// bUnderline
		0,							// cStrikeOut
		ANSI_CHARSET,				// nCharSet
		OUT_DEFAULT_PRECIS,			// nOutPrecision
		CLIP_DEFAULT_PRECIS,		// nClipPrecision
		DEFAULT_QUALITY,			// nQuality
		VARIABLE_PITCH,				// nPitchAndFamily
		_T("Arial")));		// lpszFacename

	VERIFY(m_font_Default.CreateFont(
		24,							// nHeight
		0,							// nWidth
		0,							// nEscapement
		0,							// nOrientation
		FW_BOLD,					// nWeight
		FALSE,						// bItalic
		FALSE,						// bUnderline
		0,							// cStrikeOut
		ANSI_CHARSET,				// nCharSet
		OUT_DEFAULT_PRECIS,			// nOutPrecision
		CLIP_DEFAULT_PRECIS,		// nClipPrecision
		DEFAULT_QUALITY,			// nQuality
		VARIABLE_PITCH,				// nPitchAndFamily
		_T("Arial")));		// lpszFacename

	VERIFY(m_font_Normal.CreateFont(
		18,							// nHeight
		0,							// nWidth
		0,							// nEscapement
		0,							// nOrientation
		FW_NORMAL,					// nWeight
		FALSE,						// bItalic
		FALSE,						// bUnderline
		0,							// cStrikeOut
		ANSI_CHARSET,				// nCharSet
		OUT_DEFAULT_PRECIS,			// nOutPrecision
		CLIP_DEFAULT_PRECIS,		// nClipPrecision
		DEFAULT_QUALITY,			// nQuality
		VARIABLE_PITCH,				// nPitchAndFamily
		_T("Gothic")));			// lpszFacename
}

CDlgLotoMode::~CDlgLotoMode()
{
}

void CDlgLotoMode::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LOTO_TITLE_1, m_stLotoTitle);
	DDX_Control(pDX, IDC_LOTO_STATIC_1, m_Stt[eSttPw1]);
	DDX_Control(pDX, IDC_LOTO_STATIC_2, m_Stt[eSttPw2]);
	DDX_Control(pDX, IDC_LOTO_BUTTON_1, m_Btn[eBtnReturn]);
	DDX_Control(pDX, IDC_LOTO_BUTTON_2, m_Btn[eBtnApply1]);
	DDX_Control(pDX, IDC_LOTO_BUTTON_3, m_Btn[eBtnApply2]);
	DDX_Control(pDX, IDC_LOTO_EDIT_1, m_Edt[eEdtPw1]);
	DDX_Control(pDX, IDC_LOTO_EDIT_2, m_Edt[eEdtPw2]);

}


BEGIN_MESSAGE_MAP(CDlgLotoMode, CDialogEx)
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_LOTO_BUTTON_1, &CDlgLotoMode::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_LOTO_BUTTON_2, &CDlgLotoMode::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_LOTO_BUTTON_3, &CDlgLotoMode::OnBnClickedButton3)
END_MESSAGE_MAP()


// CDlgLanguage 메시지 처리기
BOOL CDlgLotoMode::PreTranslateMessage(MSG* pMsg)
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

int CDlgLotoMode::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	return 0;
}

BOOL CDlgLotoMode::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CRect rect;
	GetClientRect(rect);
	//pDC->FillSolidRect(rect, RGB(105, 186, 235));
	pDC->FillSolidRect(rect, RGB(255, 255, 255));
	//pDC->FillSolidRect(rect, RGB(227, 255, 0));		// Lime
	return TRUE;
}

BOOL CDlgLotoMode::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// 시작할 때
	m_Btn[eBtnApply2].ShowWindow(FALSE);

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgLotoMode::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (bShow)
	{
		InitComponnet_LotoMode();
		
	}
}

void CDlgLotoMode::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_nFlickCount++ < 5)
	{
		m_stLotoTitle.SetColorStyle(CVGStatic::ColorStyle_Orange);

	}
	else if (m_nFlickCount < 10)
	{
		m_stLotoTitle.SetColorStyle(CVGStatic::ColorStyle_Black);
	}
	else
	{
		m_nFlickCount = 0;
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CDlgLotoMode::InitComponnet_LotoMode()
{
	m_stLotoTitle.SetStaticStyle(CVGStatic::StaticStyle_GroupHeader);
	m_stLotoTitle.SetColorStyle(CVGStatic::ColorStyle_Black);
	m_stLotoTitle.SetFont_Gdip(L"Gothic", 38.0F);
	m_stLotoTitle.SetText(_T("LOTO Mode"));

	CString str;
	// Static
	for (int i = 0; i < eStt_Max; i++)
	{
		m_Stt[i].SetStaticStyle(CVGStatic::StaticStyle_GroupHeader);
		m_Stt[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_Stt[i].SetFont_Gdip(L"Gothic", 28.0F);
		if(i == 0) str.Format(_T("PW SET"));
		if (i == 1) str.Format(_T("INS PW"));
		m_Stt[i].SetText(str);
	}
	// Button
	for (int i = 0; i < eBtn_Max; i++)
	{
		m_Btn[i].SetFont(&m_font_Normal);
		if (i == eBtnApply1)			str.Format(_T("APPLY"));
		else if (i == eBtnApply2)		str.Format(_T("APPLY"));
		else if (i == eBtnReturn)		str.Format(_T("Return"));

		m_Btn[i].SetWindowText(str);
	}
	// Edit
	for (int i = 0; i < eEdt_Max; i++)
	{
		m_Edt[i].SetFont(&m_font_Normal);
		if (i == eEdtPw1)			str.Format(_T("12345678"));
		else if (i == eEdtPw2)		str.Format(_T("87654321"));

		m_Edt[i].SetWindowText(str);
	}
}

void CDlgLotoMode::OnBnClickedButton1()
{
	COMMON->SetLotoMode(FALSE);

	OnOK();
}

// LOTO 잠금시
void CDlgLotoMode::OnBnClickedButton2()
{
	int length;
	CString str1 = _T("__QPZM1234ZXCV1234QWER__");
	CString str2;
	m_Edt[eEdtPw1].GetWindowText(m_strPW[eEdtPw1]);
	m_Edt[eEdtPw2].GetWindowText(m_strPW[eEdtPw2]);

	if (m_strPW[eEdtPw1] == m_strPW[eEdtPw2])
	{
		m_Edt[eEdtPw2].EnableWindow(FALSE);
		m_Btn[eBtnApply1].ShowWindow(FALSE);
		m_Btn[eBtnApply2].ShowWindow(TRUE);
		m_Btn[eBtnReturn].EnableWindow(FALSE);
		// [21.1026.6]
		length = m_strPW[eEdtPw2].GetLength();
		str2 = str1.Mid(0,length);
		m_Edt[eEdtPw1].SetWindowText(str2);
		// [21.1018.3]
		COMMON->SetLotoMode(TRUE);
		SetTimer(TIMER_TITLE_FLICK, 200, NULL);
	}
}

// LOTO 해제시 / MasterKey : 33793379
void CDlgLotoMode::OnBnClickedButton3()
{
	time_t curTime, elpTime;
	bool bRtn1, bRtn2;
	m_Edt[eEdtPw1].GetWindowText(m_strPW[eEdtPw1]);
	m_Edt[eEdtPw2].GetWindowText(m_strPW[eEdtPw2]);

	bRtn1 = (m_strPW[eEdtPw1] == m_strPW[eEdtPw2] ? true : false);
	bRtn2 = (m_strPW[eEdtPw1] == _T("33793379") ? true : false);

	if (bRtn1 || bRtn2)
	{
		m_Edt[eEdtPw1].EnableWindow(FALSE);
		m_Btn[eBtnApply1].EnableWindow(FALSE);
		m_Btn[eBtnApply2].EnableWindow(FALSE);
		m_Btn[eBtnReturn].EnableWindow(TRUE);
		KillTimer(TIMER_TITLE_FLICK);

		// Sleep Mode 처리
		if (COMMON->m_SleepModeStep == SLEEP_ON)
		{
			curTime = GetTickCount64();
			elpTime = COMMON->m_tSleepElapsed;
			COMMON->m_tSleepStart = curTime - elpTime;
		}
	}
}
