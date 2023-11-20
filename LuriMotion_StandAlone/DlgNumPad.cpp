// DlgNumPad.cpp: 구현 파일
//

#include "pch.h"
#include "LuriMotion_OQA_SFR_Wide.h"
#include "LuriMotion_OQA_SFR_WideDlg.h"
#include "DlgNumPad.h"
#include "afxdialogex.h"

#define DEFAULT_BACK_COLOR Gdiplus::Color::White
#define CURSOR_BACK_COLOR Gdiplus::Color::Yellow

// CDlgNumPad 대화 상자

IMPLEMENT_DYNAMIC(CDlgNumPad, CDialogEx)

CDlgNumPad::CDlgNumPad(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_NUMPAD, pParent)
{
}

CDlgNumPad::~CDlgNumPad()
{
}

void CDlgNumPad::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STC_KEYPAD_NUM_POS_00, m_stNumPos[0]);
	DDX_Control(pDX, IDC_STC_KEYPAD_NUM_POS_01, m_stNumPos[1]);
	DDX_Control(pDX, IDC_STC_KEYPAD_NUM_POS_02, m_stNumPos[2]);
	DDX_Control(pDX, IDC_STC_KEYPAD_NUM_POS_03, m_stNumPos[3]);
	DDX_Control(pDX, IDC_STC_KEYPAD_NUM_POS_04, m_stNumPos[4]);
	DDX_Control(pDX, IDC_STC_KEYPAD_NUM_POS_05, m_stNumPos[5]);
	DDX_Control(pDX, IDC_STC_KEYPAD_NUM_POS_06, m_stNumPos[6]);
	DDX_Control(pDX, IDC_STC_KEYPAD_NUM_POS_07, m_stNumPos[7]);
	DDX_Control(pDX, IDC_STC_KEYPAD_NUM_POS_08, m_stNumPos[8]);
	DDX_Control(pDX, IDC_STC_KEYPAD_NUM_POS_09, m_stNumPos[9]);
	DDX_Control(pDX, IDC_STC_KEYPAD_NUM_POS_10, m_stNumPos[10]);
	DDX_Control(pDX, IDC_STC_KEYPAD_NUM_POS_11, m_stNumPos[11]);

	DDX_Control(pDX, IDC_BTN_KEYPAD_OK, m_ctrlBtnOk);
	DDX_Control(pDX, IDC_BTN_KEYPAD_CANCEL, m_ctrlBtnCancel);
	DDX_Control(pDX, IDC_BTN_KEYPAD_CLEAR, m_ctrlBtnClear);
}


BEGIN_MESSAGE_MAP(CDlgNumPad, CDialogEx)
	//ON_WM_ERASEBKGND()

	ON_BN_CLICKED(IDC_BTN_KEYPAD_00, &CDlgNumPad::OnBnClickedBtnNum0)
	ON_BN_CLICKED(IDC_BTN_KEYPAD_01, &CDlgNumPad::OnBnClickedBtnNum1)
	ON_BN_CLICKED(IDC_BTN_KEYPAD_02, &CDlgNumPad::OnBnClickedBtnNum2)
	ON_BN_CLICKED(IDC_BTN_KEYPAD_03, &CDlgNumPad::OnBnClickedBtnNum3)
	ON_BN_CLICKED(IDC_BTN_KEYPAD_04, &CDlgNumPad::OnBnClickedBtnNum4)
	ON_BN_CLICKED(IDC_BTN_KEYPAD_05, &CDlgNumPad::OnBnClickedBtnNum5)
	ON_BN_CLICKED(IDC_BTN_KEYPAD_06, &CDlgNumPad::OnBnClickedBtnNum6)
	ON_BN_CLICKED(IDC_BTN_KEYPAD_07, &CDlgNumPad::OnBnClickedBtnNum7)
	ON_BN_CLICKED(IDC_BTN_KEYPAD_08, &CDlgNumPad::OnBnClickedBtnNum8)
	ON_BN_CLICKED(IDC_BTN_KEYPAD_09, &CDlgNumPad::OnBnClickedBtnNum9)

	ON_BN_CLICKED(IDC_BTN_KEYPAD_BACK, &CDlgNumPad::OnBnClickedBtnBack)
	ON_BN_CLICKED(IDC_BTN_KEYPAD_DEL, &CDlgNumPad::OnBnClickedBtnDel)
	ON_BN_CLICKED(IDC_BTN_KEYPAD_CURSOR_BACK, &CDlgNumPad::OnBnClickedBtnCursorLeft)
	ON_BN_CLICKED(IDC_BTN_KEYPAD_CURSOR_FORW, &CDlgNumPad::OnBnClickedBtnCursorRight)

	ON_BN_CLICKED(IDC_BTN_KEYPAD_DOT, &CDlgNumPad::OnBnClickedBtnDot)
	ON_BN_CLICKED(IDC_BTN_KEYPAD_SIGN, &CDlgNumPad::OnBnClickedBtnSign)
	ON_BN_CLICKED(IDC_BTN_KEYPAD_CLEAR, &CDlgNumPad::OnBnClickedBtnClear)
	ON_BN_CLICKED(IDC_BTN_KEYPAD_CANCEL, &CDlgNumPad::OnBnClickedBtnCancel)
	ON_BN_CLICKED(IDC_BTN_KEYPAD_OK, &CDlgNumPad::OnBnClickedBtnOk)

END_MESSAGE_MAP()

BOOL CDlgNumPad::PreTranslateMessage(MSG* pMsg)
{
	//ALT + F4 차단
	if (pMsg->message == WM_SYSKEYDOWN && pMsg->wParam == VK_F4)
	{
		return TRUE;
	}

	////ESC 차단
	//if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	//{
	//	return TRUE;
	//}

	////Enter 차단
	//if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	//{
	//	return TRUE;
	//}

	if (pMsg->message == WM_KEYUP)
	{
		switch (pMsg->wParam)
		{
		case '0':	case VK_NUMPAD0: OnBnClickedBtnNum0();	break;
		case '1':	case VK_NUMPAD1: OnBnClickedBtnNum1();	break;
		case '2':	case VK_NUMPAD2: OnBnClickedBtnNum2();	break;
		case '3':	case VK_NUMPAD3: OnBnClickedBtnNum3();	break;
		case '4':	case VK_NUMPAD4: OnBnClickedBtnNum4();	break;
		case '5':	case VK_NUMPAD5: OnBnClickedBtnNum5();	break;
		case '6':	case VK_NUMPAD6: OnBnClickedBtnNum6();	break;;
		case '7':	case VK_NUMPAD7: OnBnClickedBtnNum7();	break;
		case '8':	case VK_NUMPAD8: OnBnClickedBtnNum8();	break;
		case '9':	case VK_NUMPAD9: OnBnClickedBtnNum9();	break;
		case VK_RETURN:
			OnBnClickedBtnOk();
			break;
		case VK_ESCAPE:
			OnBnClickedBtnCancel();
			break;
		case VK_LEFT:
			OnBnClickedBtnCursorLeft();
			break;
		case VK_RIGHT:
			OnBnClickedBtnCursorRight();
			break;
		case VK_DELETE:
			OnBnClickedBtnDel();
			break;
			break;
		case VK_BACK:
			OnBnClickedBtnBack();
			break;
		case VK_OEM_PERIOD:	case VK_DECIMAL: 
			OnBnClickedBtnDot();
			break;
		}
	}
	UpdateData(FALSE);

	return CDialogEx::PreTranslateMessage(pMsg);
}

BOOL CDlgNumPad::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(227, 255, 0));		// Lime
	return TRUE;
}

// CDlgNumPad 메시지 처리기
BOOL CDlgNumPad::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	InitComponent_NumPad();

	m_nCursorPos = 0;
	DisplayNum();
	SetCursorColor(m_nCursorPos, CURSOR_BACK_COLOR);

	CLuriMotion_OQA_SFR_WideDlg *pParrentDlg = (CLuriMotion_OQA_SFR_WideDlg*)AfxGetApp()->GetMainWnd();

	m_ctrlBtnOk.SetFocus();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgNumPad::InitComponent_NumPad()
{
	for (int i = 0; i < 12; i++)
	{
		m_stNumPos[i].SetStaticStyle(CVGStatic::StaticStyle_Data);
		m_stNumPos[i].SetColorStyle(CVGStatic::ColorStyle_White);
		m_stNumPos[i].SetFont_Gdip(L"Arial", 15.0F);
		//m_stNumPos[i].SetText(m_szKeyPadNujm_Param[i]);
	}
}

void CDlgNumPad::SetValue(double dValue)
{
	int i;
	CString strBuffer;

	//Data를 초기화 한다.
	m_dCalValue = dValue;
	for (i = 0; i < 12; i++)
		m_strPanelBit[i].Format(_T(""));
	// 부호를 얻어낸다.
	if (dValue == 0.0) return;

	if (dValue < 0.0)
	{
		dValue = fabs(dValue);
		m_strPanelBit[11] = "-";
	}

	strBuffer.Format(_T("%.2lf"), dValue);
	m_nNumLength = strBuffer.GetLength();
	for (i = 0; i < m_nNumLength; i++)
	{
		m_strPanelBit[i + 1].Format(_T("%c"), strBuffer.GetAt(m_nNumLength - i - 1));
	}
}

void CDlgNumPad::SetValue(int nValue)
{
	int i;
	CString strBuffer;

	//Data를 초기화 한다.
	m_dCalValue = nValue;
	for (i = 0; i < 12; i++)
		m_strPanelBit[i].Format(_T(""));
	// 부호를 얻어낸다.
	if (nValue == 0) return;

	if (nValue < 0)
	{
		nValue = abs(nValue);
		m_strPanelBit[11] = "-";
	}

	strBuffer.Format(_T("%d"), nValue);
	m_nNumLength = strBuffer.GetLength();
	for (i = 0; i < m_nNumLength; i++)
	{
		m_strPanelBit[i + 1].Format(_T("%c"), strBuffer.GetAt(m_nNumLength - i - 1));
	}
}

double CDlgNumPad::GetValue()
{
	return m_dCalValue;
}

CString CDlgNumPad::GetCurrentNum()
{
	CString str;
	str.Format(_T("%.3f"), m_dCalValue);
	return str;
}

void CDlgNumPad::AddBit(CString strBit)
{
	CString strBuf;
	// *******************************************
	// Cursor가 0위치에 있으면 Tail에 추가 
	// *******************************************
	if (m_nCursorPos == 0)
	{
		if (m_nNumLength < 10)
			m_nNumLength++;
		else
			return;
		for (int i = 9; i > 0; i--)
			m_strPanelBit[i + 1] = m_strPanelBit[i];

		m_strPanelBit[1] = strBit;
	}
	// *******************************************
	// Cursor가 0위치에 없으면 중간에 Insert한다.
	// *******************************************
	else
	{
		m_strPanelBit[m_nCursorPos] = strBit;
		SetCursorColor(m_nCursorPos, DEFAULT_BACK_COLOR);
		m_nCursorPos--;
		SetCursorColor(m_nCursorPos, CURSOR_BACK_COLOR);
	}
	DisplayNum();
}

void CDlgNumPad::DeleteBit()
{
	if (m_nCursorPos == 0 || m_nNumLength == 0)
		return;
	else
	{
		for (int i = m_nCursorPos; i <= m_nNumLength; i++)
			m_strPanelBit[i - 1] = m_strPanelBit[i];

	}
	m_strPanelBit[m_nNumLength] = "";
	OnBnClickedBtnCursorRight();
	m_nNumLength--;
	DisplayNum();
}

void CDlgNumPad::BackspaceBit()
{
	if (m_nCursorPos >= m_nNumLength)//m_nCursorPos == 0 || 
		return;
	else
	{
		for (int i = m_nCursorPos; i < m_nNumLength; i++)
			m_strPanelBit[i] = m_strPanelBit[i + 1];

	}
	m_strPanelBit[m_nNumLength] = "";
	m_nNumLength--;
	DisplayNum();
}

void CDlgNumPad::SetCursorColor(int nNo, Gdiplus::Color clrBack)
{
	m_stNumPos[nNo].SetBackColor(clrBack);
}

void CDlgNumPad::DisplayNum()
{
	for (int i = 0; i < 12; i++)
		m_stNumPos[i].SetWindowText(m_strPanelBit[i]);
}
void CDlgNumPad::ClickOk()
{
	CDialogEx::OnOK();
}

void CDlgNumPad::ClickClose()
{
	::PostMessage(GetSafeHwnd(), WM_CLOSE, NULL, NULL);
}


void CDlgNumPad::OnBnClickedBtnNum0()
{
	CString strBit;
	strBit.Format(_T("0"));
	AddBit(strBit);
}

void CDlgNumPad::OnBnClickedBtnNum1()
{
	CString strBit;
	strBit.Format(_T("1"));
	AddBit(strBit);
}

void CDlgNumPad::OnBnClickedBtnNum2()
{
	CString strBit;
	strBit.Format(_T("2"));
	AddBit(strBit);
}

void CDlgNumPad::OnBnClickedBtnNum3()
{
	CString strBit;
	strBit.Format(_T("3"));
	AddBit(strBit);
}

void CDlgNumPad::OnBnClickedBtnNum4()
{
	CString strBit;
	strBit.Format(_T("4"));
	AddBit(strBit);
}

void CDlgNumPad::OnBnClickedBtnNum5()
{
	CString strBit;
	strBit.Format(_T("5"));
	AddBit(strBit);
}

void CDlgNumPad::OnBnClickedBtnNum6()
{
	CString strBit;
	strBit.Format(_T("6"));
	AddBit(strBit);
}

void CDlgNumPad::OnBnClickedBtnNum7()
{
	CString strBit;
	strBit.Format(_T("7"));
	AddBit(strBit);
}

void CDlgNumPad::OnBnClickedBtnNum8()
{
	CString strBit;
	strBit.Format(_T("8"));
	AddBit(strBit);
}

void CDlgNumPad::OnBnClickedBtnNum9()
{
	CString strBit;
	strBit.Format(_T("9"));
	AddBit(strBit);
}


void CDlgNumPad::OnBnClickedBtnBack()
{
	BackspaceBit();

}

void CDlgNumPad::OnBnClickedBtnDel()
{
	DeleteBit();

}
void CDlgNumPad::OnBnClickedBtnCursorLeft()
{
	if (m_nCursorPos < m_nNumLength)
	{
		SetCursorColor(m_nCursorPos, DEFAULT_BACK_COLOR);
		m_nCursorPos++;
		SetCursorColor(m_nCursorPos, CURSOR_BACK_COLOR);
	}
}
void CDlgNumPad::OnBnClickedBtnCursorRight()
{
	if (m_nCursorPos >= 1)
	{
		SetCursorColor(m_nCursorPos, DEFAULT_BACK_COLOR);
		m_nCursorPos--;
		SetCursorColor(m_nCursorPos, CURSOR_BACK_COLOR);
	}
}
void CDlgNumPad::OnBnClickedBtnDot()
{
	CString strBit;
	BOOL bFind = FALSE;
	strBit.Format(_T("."));
	// 앞쪽에 .이 있으면 수행하지 않는다.
	for (int i = m_nNumLength; i > m_nCursorPos; i--)
	{
		if (m_strPanelBit[i] == strBit)
		{
			bFind = TRUE;
			break;
		}
	}
	if (bFind == FALSE)
		AddBit(strBit);
}
void CDlgNumPad::OnBnClickedBtnSign()
{
	if (_tcscmp(m_strPanelBit[11], _T("-")) == 0)
		m_strPanelBit[11] = "+";
	else
		m_strPanelBit[11] = "-";
	DisplayNum();
}
void CDlgNumPad::OnBnClickedBtnClear()
{
	for (int i = 0; i < 12; i++)
		m_strPanelBit[i].Format(_T(""));
	m_nNumLength = 0;
	DisplayNum();
	SetCursorColor(m_nCursorPos, DEFAULT_BACK_COLOR);
	m_nCursorPos = 0;
	SetCursorColor(m_nCursorPos, CURSOR_BACK_COLOR);
}
void CDlgNumPad::OnBnClickedBtnCancel()
{
	CDialog::OnOK();
}
void CDlgNumPad::OnBnClickedBtnOk()
{
	//**************************************
	// 화면 Data를 숫자로 변환시킨다.
	//**************************************
	int nSign = 1, nNum;
	double dMultiple = 10.0;
	double dValue;

	dValue = 0.0;
	if (m_strPanelBit[11] == "-")
		nSign = -1;
	else
		nSign = 1;

	// ***************************
	// 소수점 이상을 계산한다.
	// ***************************
	int i = 0;
	for (i = m_nNumLength; i > 0; i--)
	{
		// 소숫점이면 빠져 나간다.
		if (m_strPanelBit[i] == ".")
		{
			i--;
			break;
		}
		nNum = m_strPanelBit[i].GetAt(0) - '0';
		dValue *= dMultiple;
		dValue += nNum;
	}
	// ***************************
	// 소수점 이상을 계산한다.
	// ***************************
	dMultiple = 0.1;
	for (; i > 0; i--)
	{
		nNum = m_strPanelBit[i].GetAt(0) - '0';
		dValue += nNum * dMultiple;
		dMultiple *= 0.1;
	}

	// 부호를 계산한다.
	dValue *= nSign;
	//	if(dValue > m_MaxLimitValue)
	m_dCalValue = dValue;
	CDialog::OnOK();
}


