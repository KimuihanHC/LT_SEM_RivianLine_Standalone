// DlgMaintence.cpp: 구현 파일
//

#include "pch.h"
#include "DlgMaintence.h"
#include "afxdialogex.h"

#include "LuriMotion_OQA_SFR_Wide.h"
#include "LuriMotion_OQA_SFR_WideDlg.h"

#include "IO.h"
#include "System_Define.h"

// CDlgMaintence 대화 상자

LPCTSTR g_szItem[] =
{
	_T("MANUAL"),
	_T("AUTO"),
};

LPCTSTR g_szManualItem[] =
{
	_T("CAMERA FIX --- ON/OFF"),
	_T("PROBE --- ON/OFF"),
	_T("SOCKET --- UP/DN"),
	_T("SOCKET --- IN/OUT"),
};

IMPLEMENT_DYNAMIC(CDlgMaintence, CDialogEx)

CDlgMaintence::CDlgMaintence(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_MAINTENCE, pParent)
{

}

CDlgMaintence::~CDlgMaintence()
{
}

void CDlgMaintence::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STC_MAINT_TITLE1, m_stTitle[0]);
	DDX_Control(pDX, IDC_STC_MAINT_TITLE2, m_stTitle[1]);

	DDX_Control(pDX, IDC_STC_MAINT_CYL1, m_stManual[0]);
	DDX_Control(pDX, IDC_STC_MAINT_CYL2, m_stManual[1]);
	DDX_Control(pDX, IDC_STC_MAINT_CYL3, m_stManual[2]);
	DDX_Control(pDX, IDC_STC_MAINT_CYL4, m_stManual[3]);
}


BEGIN_MESSAGE_MAP(CDlgMaintence, CDialogEx)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BTN_MAINT_CYL_ON, &CDlgMaintence::OnBnClickedBtnMaintCylOn)
	ON_BN_CLICKED(IDC_BTN_MAINT_CYL_OFF, &CDlgMaintence::OnBnClickedBtnMaintCylOff)
	ON_BN_CLICKED(IDC_BTN_MAINT_CYL_ON2, &CDlgMaintence::OnBnClickedBtnMaintCylOn2)
	ON_BN_CLICKED(IDC_BTN_MAINT_CYL_OFF2, &CDlgMaintence::OnBnClickedBtnMaintCylOff2)
	ON_BN_CLICKED(IDC_BTN_MAINT_CYL_ON3, &CDlgMaintence::OnBnClickedBtnMaintCylOn3)
	ON_BN_CLICKED(IDC_BTN_MAINT_CYL_OFF3, &CDlgMaintence::OnBnClickedBtnMaintCylOff3)
	ON_BN_CLICKED(IDC_BTN_MAINT_CYL_ON4, &CDlgMaintence::OnBnClickedBtnMaintCylOn4)
	ON_BN_CLICKED(IDC_BTN_MAINT_CYL_OFF4, &CDlgMaintence::OnBnClickedBtnMaintCylOff4)
	ON_BN_CLICKED(IDC_BTN_MAINT_AUTO_LOAD, &CDlgMaintence::OnBnClickedBtnMaintAutoLoad)
	ON_BN_CLICKED(IDC_BTN_MAINT_AUTO_UNLOAD, &CDlgMaintence::OnBnClickedBtnMaintAutoUnload)
END_MESSAGE_MAP()

// CDlgMaintence 메시지 처리기
BOOL CDlgMaintence::PreTranslateMessage(MSG* pMsg)
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

BOOL CDlgMaintence::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(83, 169, 155));
	return TRUE;
}

// CDlgIo 메시지 처리기
int CDlgMaintence::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

// camera fix on
void CDlgMaintence::OnBnClickedBtnMaintCylOn()
{
	CIO::IO_O_Camera_Fix_Cylinder_On(false);
}

// camera fix off
void CDlgMaintence::OnBnClickedBtnMaintCylOff()
{
	CIO::IO_O_Camera_Fix_Cylinder_Off(false);
}

// probe pin on
void CDlgMaintence::OnBnClickedBtnMaintCylOn2()
{
	CIO::IO_O_ProbePin_Up(false);
}

// probe pin off
void CDlgMaintence::OnBnClickedBtnMaintCylOff2()
{
	CIO::IO_O_ProbePin_Down(false);
}

// socket up
void CDlgMaintence::OnBnClickedBtnMaintCylOn3()
{
	CIO::IO_O_Socket_Up(false);
}

// socket dn
void CDlgMaintence::OnBnClickedBtnMaintCylOff3()
{
	CIO::IO_O_Socket_Down(false);
}

// socket in
void CDlgMaintence::OnBnClickedBtnMaintCylOn4()
{
	CIO::IO_O_Socket_In(false);
}

// socket out
void CDlgMaintence::OnBnClickedBtnMaintCylOff4()
{
	CIO::IO_O_Socket_Out(false);
}

void CDlgMaintence::OnBnClickedBtnMaintAutoLoad()
{
	auto err = MCEC_OK;
	err = (enMCErrCode)SEQUENCE->Maintence(enMaintFunId::Load);

	if(err != MCEC_OK)
		SHOW_ERROR(err);
}

void CDlgMaintence::OnBnClickedBtnMaintAutoUnload()
{
	auto err = MCEC_OK;
	err = (enMCErrCode)SEQUENCE->Maintence(enMaintFunId::Unload);

	if (err != MCEC_OK)
		SHOW_ERROR(err);
}

BOOL CDlgMaintence::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitUIComponent();

	InitUI_EqpType();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgMaintence::InitUIComponent()
{
	auto cnt= 0;
	for (auto &ref : m_stTitle)
	{
		ref.SetStaticStyle(CVGStatic::StaticStyle_Data);
		ref.SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		ref.SetFont_Gdip(L"Arial", 9.0F);
		ref.SetText(g_szItem[cnt++]);
	}

	cnt = 0;
	for (auto &ref : m_stManual)
	{
		ref.SetStaticStyle(CVGStatic::StaticStyle_Data);
		ref.SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		ref.SetFont_Gdip(L"Arial", 9.0F);
		ref.SetText(g_szManualItem[cnt++]);
	}
}

void CDlgMaintence::InitUI_EqpType()
{
	auto EqpType = GET_INSPECTOR;

	switch (EqpType)
	{
	case SYS_DISTORTION_30:
		GetDlgItem(IDC_STC_MAINT_CYL3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_MAINT_CYL4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_MAINT_CYL_ON3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_MAINT_CYL_OFF3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_MAINT_CYL_ON4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_MAINT_CYL_OFF4)->ShowWindow(SW_HIDE);
		break;

	case SYS_OQC_SFR_MULTI_CL:
		break;

	case SYS_OQC_SFR_SINGLE_CL:
		break;
	default:
		break;
	}
}
