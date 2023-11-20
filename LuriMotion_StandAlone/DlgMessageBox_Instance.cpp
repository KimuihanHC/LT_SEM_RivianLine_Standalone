// DlgMessageBox.cpp: 구현 파일
//

#include "pch.h"
#include "LuriMotion_OQA_SFR_Wide.h"
#include "DlgMessageBox_Instance.h"
#include "afxdialogex.h"


// CDlgMessageBox 대화 상자

IMPLEMENT_DYNAMIC(CDlgMessageBox_Instance, CDialogEx)
CDlgMessageBox_Instance *CDlgMessageBox_Instance::m_pInstance = NULL;

CDlgMessageBox_Instance *CDlgMessageBox_Instance::Get_Instance(CWnd *pParent)
{
	if (!m_pInstance) {
		m_pInstance = new CDlgMessageBox_Instance(pParent);
		if (!m_pInstance->m_hWnd) {
			m_pInstance->Create(IDD_DLG_MESSAGEBOX_INSTANCE, pParent);
		}
	}
	return m_pInstance;
}

void CDlgMessageBox_Instance::Delete_Instance()
{
	if (m_pInstance->m_hWnd) m_pInstance->DestroyWindow();
	if (m_pInstance) delete m_pInstance;
	m_pInstance = NULL;
}
CDlgMessageBox_Instance::CDlgMessageBox_Instance( CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_MESSAGEBOX_INSTANCE, pParent)
{
	m_strError = _T("");
	m_nMode = -1;


	m_bTouched = FALSE;
	m_bTest_comp = FALSE;
}

CDlgMessageBox_Instance::~CDlgMessageBox_Instance()
{
}

void CDlgMessageBox_Instance::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STC_TEST_1, m_stTitle);
	
}


BEGIN_MESSAGE_MAP(CDlgMessageBox_Instance, CDialogEx)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_1, &CDlgMessageBox_Instance::OnBnClickedBtn1)
	ON_BN_CLICKED(IDC_BTN_2, &CDlgMessageBox_Instance::OnBnClickedBtn2)
	ON_BN_CLICKED(IDC_BTN_3, &CDlgMessageBox_Instance::OnBnClickedBtn3)
	ON_BN_CLICKED(IDCANCEL, &CDlgMessageBox_Instance::OnBnClickedCancel)

END_MESSAGE_MAP()

BOOL CDlgMessageBox_Instance::PreTranslateMessage(MSG* pMsg)
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
int CDlgMessageBox_Instance::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}
// CDlgMessageBox 메시지 처리기
BOOL CDlgMessageBox_Instance::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(70, 70, 70));		// Lime
	return TRUE;
}
BOOL CDlgMessageBox_Instance::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgMessageBox_Instance::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (bShow == TRUE)
	{
		if (m_nMode == MODE_NO_CLOSE)
		{
			// [22.0401.1]
			//SetWindowPos(this, 0, 0, 1920, 1080, SWP_NOZORDER);
			SetWindowPos(&CWnd::wndTopMost, 0, 0, 1920, 1080, SWP_SHOWWINDOW);
			SetTimer(100, 50, NULL);	
			m_bTouched = FALSE;
		}	
		ChangeComponent_MessageBox();
	}
	else
	{
		m_bTouched = FALSE;
		m_bTest_comp = FALSE;
		m_nMode = -1;
		::SetEvent(COMMON->m_closeEvent);
	}
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

// CDlgMain 메시지 처리기
void CDlgMessageBox_Instance::InitComponent_MessageBox()
{
	CString strTemp = _T("");

	m_stTitle.SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
	m_stTitle.SetColorStyle(CVGStatic::ColorStyle_Yellow);
	m_stTitle.SetFont_Gdip(L"Arial", 35.0F);
	m_stTitle.SetText(m_strError);
}
void CDlgMessageBox_Instance::ChangeComponent_MessageBox()
{
	CString strTemp = _T("");

	if (m_nMode == (int)MODE_INFORMATION)
	{
		m_stTitle.SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stTitle.SetColorStyle(CVGStatic::ColorStyle_Orange);
		m_stTitle.SetFont_Gdip(L"Arial", 35.0F);
		m_stTitle.SetText(m_strError);
	}
	else if (m_nMode == (int)MODE_QUESTION)
	{
		m_stTitle.SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stTitle.SetColorStyle(CVGStatic::ColorStyle_Orange);
		m_stTitle.SetFont_Gdip(L"Arial", 35.0F);
		m_stTitle.SetText(m_strError);
	}
	else
	{
		m_stTitle.SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stTitle.SetColorStyle(CVGStatic::ColorStyle_Navy);
		m_stTitle.SetFont_Gdip(L"Arial", 35.0F);
		m_stTitle.SetText(m_strError);		
	}
}

void CDlgMessageBox_Instance::SetMsg(CString str, int nMode)
{
	m_strError = str;
	m_nMode = nMode;
}

void CDlgMessageBox_Instance::OnBnClickedBtn1()
{
	m_nReturn = 1;
	::SetEvent(COMMON->m_closeEvent);

	EndDialog(IDOK);
}
void CDlgMessageBox_Instance::OnBnClickedBtn2()
{
	m_nReturn = 0;
	::SetEvent(COMMON->m_closeEvent);

	EndDialog(IDCANCEL);
}
void CDlgMessageBox_Instance::OnBnClickedBtn3()
{
	//AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_09_TOWERLAMP_BEEP, _OFF);
}

void CDlgMessageBox_Instance::OnBnClickedCancel()
{
	m_nReturn = 0;
	::SetEvent(COMMON->m_closeEvent);
	CDialogEx::OnCancel();
}
void CDlgMessageBox_Instance::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	switch (nIDEvent)
	{
	case 100:
	{
		if (AJIN_IO->GetInputIoBit(DIO_INPUT_X0_02_STOP) == TRUE)
		{
//			if (m_bTouched == FALSE)
//			{
//				m_dwResultTimer = GetTickCount();
//				m_bTouched = TRUE;
//				m_bTest_comp = FALSE;
//			}
////			else if ((GetTickCount() - m_dwResultTimer) > TIMEOUT_DELAY_200 && m_bTest_comp == FALSE)
//			else if (m_bTest_comp == FALSE)
//			{
				SEQUENCE->SetStopswitchMode(TRUE);
				
				m_bTest_comp = TRUE;
				m_nMode = -1;

				// [22.0118.2]
				::SetEvent(COMMON->m_closeEvent);

				EndDialog(IDOK);
//			}
		}
	}
	break;
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CDlgMessageBox_Instance::OnCancel()
{
	m_nReturn = 0;
	::SetEvent(COMMON->m_closeEvent);

	CDialogEx::OnCancel();
}