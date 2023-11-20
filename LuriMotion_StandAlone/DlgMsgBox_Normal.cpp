// DlgMsgBox_Normal.cpp: 구현 파일
//

#include "pch.h"
#include "LuriMotion_OQA_SFR_Wide.h"
#include "DlgMsgBox_Normal.h"
#include "afxdialogex.h"


// CDlgMsgBox_Normal 대화 상자

IMPLEMENT_DYNAMIC(CDlgMsgBox_Normal, CDialogEx)

CDlgMsgBox_Normal::CDlgMsgBox_Normal(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_MSGBOX_NORMAL, pParent)
{

}

CDlgMsgBox_Normal::~CDlgMsgBox_Normal()
{
}

void CDlgMsgBox_Normal::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STC_MSGBOX_TITLE, m_stMsg);
}


BEGIN_MESSAGE_MAP(CDlgMsgBox_Normal, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_MSGBOX_OK, &CDlgMsgBox_Normal::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_MSGBOX_NG, &CDlgMsgBox_Normal::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_MSGBOX_BEEP, &CDlgMsgBox_Normal::OnBnClickedButton1)
END_MESSAGE_MAP()


// CDlgMsgBox_Normal 메시지 처리기
BOOL CDlgMsgBox_Normal::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

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

	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgMsgBox_Normal::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	EndDialog(IDOK);
}


void CDlgMsgBox_Normal::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	EndDialog(IDCANCEL);
}


void CDlgMsgBox_Normal::OnBnClickedButton1()
{
	//AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_09_TOWERLAMP_BEEP, _OFF);
}

void CDlgMsgBox_Normal::InitComponent_MSgBoxDlg(int nType, CString strMsg)
{
	switch (nType)
	{
	case MODE_QUESTION:
		m_stMsg.SetStaticStyle(CVGStatic::StaticStyle_GroupHeader);
		m_stMsg.SetColorStyle(CVGStatic::ColorStyle_Navy);
		m_stMsg.SetFont_Gdip(L"Arial", 15.0F);
		m_stMsg.SetText(m_strMsg);
		break;
	case MODE_ERROR:
		m_stMsg.SetStaticStyle(CVGStatic::StaticStyle_GroupHeader);
		m_stMsg.SetColorStyle(CVGStatic::ColorStyle_Red);
		m_stMsg.SetFont_Gdip(L"Arial", 15.0F);
		m_stMsg.SetText(m_strMsg);
		break;
	case MODE_INFORMATION:
		m_stMsg.SetStaticStyle(CVGStatic::StaticStyle_GroupHeader);
		m_stMsg.SetColorStyle(CVGStatic::ColorStyle_Yellow);
		m_stMsg.SetFont_Gdip(L"Arial", 15.0F);
		m_stMsg.SetText(m_strMsg);
		break;
	default:
		break;
	}

	m_stMsg.SetFont_Gdip(L"Arial", 15.0F);
	m_stMsg.SetText(strMsg);
}


void CDlgMsgBox_Normal::SetMsg(CString str, int mode)
{
	m_strMsg = str;
	m_nMsgMode = mode;

	InitComponent_MSgBoxDlg(m_nMsgMode, m_strMsg);
}