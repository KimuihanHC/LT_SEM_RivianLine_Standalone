// DlgIo.cpp: 구현 파일
//

#include "pch.h"
#include "LuriMotion_OQA_SFR_Wide.h"
#include "DlgIo.h"
#include "afxdialogex.h"


// CDlgIo 대화 상자
#define TIMER_IO_STATUS		2

IMPLEMENT_DYNAMIC(CDlgIo, CDialogEx)

CDlgIo::CDlgIo(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_DIGITAL_IO, pParent)
{
	m_nSel_DIO_Input = 0;
	m_nSel_DIO_Input_Old = 0;
	m_nSel_DIO_Output = 0;
	m_nSel_DIO_Output_Old = 0;

	for (int i = 0; i < 32; i++)
	{
			m_nOutputBtn_Status[0][i] = 0;
	}
	for (int i = 0; i < 3; i++)
	{
		m_dwInput_Old[i] = { 0, };
		m_dwOutput_Old[i] = { 0, };
	}
}

CDlgIo::~CDlgIo()
{
}

void CDlgIo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STC_IO_MAIN_TITLE_1, m_stIo_Title[0]);
	DDX_Control(pDX, IDC_STC_IO_MAIN_TITLE_2, m_stIo_Title[1]);

	DDX_Control(pDX, IDC_STC_INPUT_IO_NUMBER_01, m_stIo_InputNum[0]);
	DDX_Control(pDX, IDC_STC_INPUT_IO_NUMBER_02, m_stIo_InputNum[1]);
	DDX_Control(pDX, IDC_STC_INPUT_IO_NUMBER_03, m_stIo_InputNum[2]);
	DDX_Control(pDX, IDC_STC_INPUT_IO_NUMBER_04, m_stIo_InputNum[3]);
	DDX_Control(pDX, IDC_STC_INPUT_IO_NUMBER_05, m_stIo_InputNum[4]);
	DDX_Control(pDX, IDC_STC_INPUT_IO_NUMBER_06, m_stIo_InputNum[5]);
	DDX_Control(pDX, IDC_STC_INPUT_IO_NUMBER_07, m_stIo_InputNum[6]);
	DDX_Control(pDX, IDC_STC_INPUT_IO_NUMBER_08, m_stIo_InputNum[7]);
	DDX_Control(pDX, IDC_STC_INPUT_IO_NUMBER_09, m_stIo_InputNum[8]);
	DDX_Control(pDX, IDC_STC_INPUT_IO_NUMBER_10, m_stIo_InputNum[9]);
	DDX_Control(pDX, IDC_STC_INPUT_IO_NUMBER_11, m_stIo_InputNum[10]);
	DDX_Control(pDX, IDC_STC_INPUT_IO_NUMBER_12, m_stIo_InputNum[11]);
	DDX_Control(pDX, IDC_STC_INPUT_IO_NUMBER_13, m_stIo_InputNum[12]);
	DDX_Control(pDX, IDC_STC_INPUT_IO_NUMBER_14, m_stIo_InputNum[13]);
	DDX_Control(pDX, IDC_STC_INPUT_IO_NUMBER_15, m_stIo_InputNum[14]);
	DDX_Control(pDX, IDC_STC_INPUT_IO_NUMBER_16, m_stIo_InputNum[15]);
	DDX_Control(pDX, IDC_STC_INPUT_IO_NUMBER_17, m_stIo_InputNum[16]);
	DDX_Control(pDX, IDC_STC_INPUT_IO_NUMBER_18, m_stIo_InputNum[17]);
	DDX_Control(pDX, IDC_STC_INPUT_IO_NUMBER_19, m_stIo_InputNum[18]);
	DDX_Control(pDX, IDC_STC_INPUT_IO_NUMBER_20, m_stIo_InputNum[19]);
	DDX_Control(pDX, IDC_STC_INPUT_IO_NUMBER_21, m_stIo_InputNum[20]);
	DDX_Control(pDX, IDC_STC_INPUT_IO_NUMBER_22, m_stIo_InputNum[21]);
	DDX_Control(pDX, IDC_STC_INPUT_IO_NUMBER_23, m_stIo_InputNum[22]);
	DDX_Control(pDX, IDC_STC_INPUT_IO_NUMBER_24, m_stIo_InputNum[23]);
	DDX_Control(pDX, IDC_STC_INPUT_IO_NUMBER_25, m_stIo_InputNum[24]);
	DDX_Control(pDX, IDC_STC_INPUT_IO_NUMBER_26, m_stIo_InputNum[25]);
	DDX_Control(pDX, IDC_STC_INPUT_IO_NUMBER_27, m_stIo_InputNum[26]);
	DDX_Control(pDX, IDC_STC_INPUT_IO_NUMBER_28, m_stIo_InputNum[27]);
	DDX_Control(pDX, IDC_STC_INPUT_IO_NUMBER_29, m_stIo_InputNum[28]);
	DDX_Control(pDX, IDC_STC_INPUT_IO_NUMBER_30, m_stIo_InputNum[29]);
	DDX_Control(pDX, IDC_STC_INPUT_IO_NUMBER_31, m_stIo_InputNum[30]);
	DDX_Control(pDX, IDC_STC_INPUT_IO_NUMBER_32, m_stIo_InputNum[31]);

	DDX_Control(pDX, IDC_STC_INPUT_NAME_01, m_stIo_InPutName[0]);
	DDX_Control(pDX, IDC_STC_INPUT_NAME_02, m_stIo_InPutName[1]);
	DDX_Control(pDX, IDC_STC_INPUT_NAME_03, m_stIo_InPutName[2]);
	DDX_Control(pDX, IDC_STC_INPUT_NAME_04, m_stIo_InPutName[3]);
	DDX_Control(pDX, IDC_STC_INPUT_NAME_05, m_stIo_InPutName[4]);
	DDX_Control(pDX, IDC_STC_INPUT_NAME_06, m_stIo_InPutName[5]);
	DDX_Control(pDX, IDC_STC_INPUT_NAME_07, m_stIo_InPutName[6]);
	DDX_Control(pDX, IDC_STC_INPUT_NAME_08, m_stIo_InPutName[7]);
	DDX_Control(pDX, IDC_STC_INPUT_NAME_09, m_stIo_InPutName[8]);
	DDX_Control(pDX, IDC_STC_INPUT_NAME_10, m_stIo_InPutName[9]);
	DDX_Control(pDX, IDC_STC_INPUT_NAME_11, m_stIo_InPutName[10]);
	DDX_Control(pDX, IDC_STC_INPUT_NAME_12, m_stIo_InPutName[11]);
	DDX_Control(pDX, IDC_STC_INPUT_NAME_13, m_stIo_InPutName[12]);
	DDX_Control(pDX, IDC_STC_INPUT_NAME_14, m_stIo_InPutName[13]);
	DDX_Control(pDX, IDC_STC_INPUT_NAME_15, m_stIo_InPutName[14]);
	DDX_Control(pDX, IDC_STC_INPUT_NAME_16, m_stIo_InPutName[15]);
	DDX_Control(pDX, IDC_STC_INPUT_NAME_17, m_stIo_InPutName[16]);
	DDX_Control(pDX, IDC_STC_INPUT_NAME_18, m_stIo_InPutName[17]);
	DDX_Control(pDX, IDC_STC_INPUT_NAME_19, m_stIo_InPutName[18]);
	DDX_Control(pDX, IDC_STC_INPUT_NAME_20, m_stIo_InPutName[19]);
	DDX_Control(pDX, IDC_STC_INPUT_NAME_21, m_stIo_InPutName[20]);
	DDX_Control(pDX, IDC_STC_INPUT_NAME_22, m_stIo_InPutName[21]);
	DDX_Control(pDX, IDC_STC_INPUT_NAME_23, m_stIo_InPutName[22]);
	DDX_Control(pDX, IDC_STC_INPUT_NAME_24, m_stIo_InPutName[23]);
	DDX_Control(pDX, IDC_STC_INPUT_NAME_25, m_stIo_InPutName[24]);
	DDX_Control(pDX, IDC_STC_INPUT_NAME_26, m_stIo_InPutName[25]);
	DDX_Control(pDX, IDC_STC_INPUT_NAME_27, m_stIo_InPutName[26]);
	DDX_Control(pDX, IDC_STC_INPUT_NAME_28, m_stIo_InPutName[27]);
	DDX_Control(pDX, IDC_STC_INPUT_NAME_29, m_stIo_InPutName[28]);
	DDX_Control(pDX, IDC_STC_INPUT_NAME_30, m_stIo_InPutName[29]);
	DDX_Control(pDX, IDC_STC_INPUT_NAME_31, m_stIo_InPutName[30]);
	DDX_Control(pDX, IDC_STC_INPUT_NAME_32, m_stIo_InPutName[31]);


	DDX_Control(pDX, IDC_STC_OUTPUT_IO_01, m_stIo_OuputNum[0]);
	DDX_Control(pDX, IDC_STC_OUTPUT_IO_02, m_stIo_OuputNum[1]);
	DDX_Control(pDX, IDC_STC_OUTPUT_IO_03, m_stIo_OuputNum[2]);
	DDX_Control(pDX, IDC_STC_OUTPUT_IO_04, m_stIo_OuputNum[3]);
	DDX_Control(pDX, IDC_STC_OUTPUT_IO_05, m_stIo_OuputNum[4]);
	DDX_Control(pDX, IDC_STC_OUTPUT_IO_06, m_stIo_OuputNum[5]);
	DDX_Control(pDX, IDC_STC_OUTPUT_IO_07, m_stIo_OuputNum[6]);
	DDX_Control(pDX, IDC_STC_OUTPUT_IO_08, m_stIo_OuputNum[7]);
	DDX_Control(pDX, IDC_STC_OUTPUT_IO_09, m_stIo_OuputNum[8]);
	DDX_Control(pDX, IDC_STC_OUTPUT_IO_10, m_stIo_OuputNum[9]);
	DDX_Control(pDX, IDC_STC_OUTPUT_IO_11, m_stIo_OuputNum[10]);
	DDX_Control(pDX, IDC_STC_OUTPUT_IO_12, m_stIo_OuputNum[11]);
	DDX_Control(pDX, IDC_STC_OUTPUT_IO_13, m_stIo_OuputNum[12]);
	DDX_Control(pDX, IDC_STC_OUTPUT_IO_14, m_stIo_OuputNum[13]);
	DDX_Control(pDX, IDC_STC_OUTPUT_IO_15, m_stIo_OuputNum[14]);
	DDX_Control(pDX, IDC_STC_OUTPUT_IO_16, m_stIo_OuputNum[15]);
	DDX_Control(pDX, IDC_STC_OUTPUT_IO_17, m_stIo_OuputNum[16]);
	DDX_Control(pDX, IDC_STC_OUTPUT_IO_18, m_stIo_OuputNum[17]);
	DDX_Control(pDX, IDC_STC_OUTPUT_IO_19, m_stIo_OuputNum[18]);
	DDX_Control(pDX, IDC_STC_OUTPUT_IO_20, m_stIo_OuputNum[19]);
	DDX_Control(pDX, IDC_STC_OUTPUT_IO_21, m_stIo_OuputNum[20]);
	DDX_Control(pDX, IDC_STC_OUTPUT_IO_22, m_stIo_OuputNum[21]);
	DDX_Control(pDX, IDC_STC_OUTPUT_IO_23, m_stIo_OuputNum[22]);
	DDX_Control(pDX, IDC_STC_OUTPUT_IO_24, m_stIo_OuputNum[23]);
	DDX_Control(pDX, IDC_STC_OUTPUT_IO_25, m_stIo_OuputNum[24]);
	DDX_Control(pDX, IDC_STC_OUTPUT_IO_26, m_stIo_OuputNum[25]);
	DDX_Control(pDX, IDC_STC_OUTPUT_IO_27, m_stIo_OuputNum[26]);
	DDX_Control(pDX, IDC_STC_OUTPUT_IO_28, m_stIo_OuputNum[27]);
	DDX_Control(pDX, IDC_STC_OUTPUT_IO_29, m_stIo_OuputNum[28]);
	DDX_Control(pDX, IDC_STC_OUTPUT_IO_30, m_stIo_OuputNum[29]);
	DDX_Control(pDX, IDC_STC_OUTPUT_IO_31, m_stIo_OuputNum[30]);
	DDX_Control(pDX, IDC_STC_OUTPUT_IO_32, m_stIo_OuputNum[31]);

	DDX_Control(pDX, IDC_STC_OUTPUT_NAME_01, m_stIo_OuputName[0]);
	DDX_Control(pDX, IDC_STC_OUTPUT_NAME_02, m_stIo_OuputName[1]);
	DDX_Control(pDX, IDC_STC_OUTPUT_NAME_03, m_stIo_OuputName[2]);
	DDX_Control(pDX, IDC_STC_OUTPUT_NAME_04, m_stIo_OuputName[3]);
	DDX_Control(pDX, IDC_STC_OUTPUT_NAME_05, m_stIo_OuputName[4]);
	DDX_Control(pDX, IDC_STC_OUTPUT_NAME_06, m_stIo_OuputName[5]);
	DDX_Control(pDX, IDC_STC_OUTPUT_NAME_07, m_stIo_OuputName[6]);
	DDX_Control(pDX, IDC_STC_OUTPUT_NAME_08, m_stIo_OuputName[7]);
	DDX_Control(pDX, IDC_STC_OUTPUT_NAME_09, m_stIo_OuputName[8]);
	DDX_Control(pDX, IDC_STC_OUTPUT_NAME_10, m_stIo_OuputName[9]);
	DDX_Control(pDX, IDC_STC_OUTPUT_NAME_11, m_stIo_OuputName[10]);
	DDX_Control(pDX, IDC_STC_OUTPUT_NAME_12, m_stIo_OuputName[11]);
	DDX_Control(pDX, IDC_STC_OUTPUT_NAME_13, m_stIo_OuputName[12]);
	DDX_Control(pDX, IDC_STC_OUTPUT_NAME_14, m_stIo_OuputName[13]);
	DDX_Control(pDX, IDC_STC_OUTPUT_NAME_15, m_stIo_OuputName[14]);
	DDX_Control(pDX, IDC_STC_OUTPUT_NAME_16, m_stIo_OuputName[15]);
	DDX_Control(pDX, IDC_STC_OUTPUT_NAME_17, m_stIo_OuputName[16]);
	DDX_Control(pDX, IDC_STC_OUTPUT_NAME_18, m_stIo_OuputName[17]);
	DDX_Control(pDX, IDC_STC_OUTPUT_NAME_19, m_stIo_OuputName[18]);
	DDX_Control(pDX, IDC_STC_OUTPUT_NAME_20, m_stIo_OuputName[19]);
	DDX_Control(pDX, IDC_STC_OUTPUT_NAME_21, m_stIo_OuputName[20]);
	DDX_Control(pDX, IDC_STC_OUTPUT_NAME_22, m_stIo_OuputName[21]);
	DDX_Control(pDX, IDC_STC_OUTPUT_NAME_23, m_stIo_OuputName[22]);
	DDX_Control(pDX, IDC_STC_OUTPUT_NAME_24, m_stIo_OuputName[23]);
	DDX_Control(pDX, IDC_STC_OUTPUT_NAME_25, m_stIo_OuputName[24]);
	DDX_Control(pDX, IDC_STC_OUTPUT_NAME_26, m_stIo_OuputName[25]);
	DDX_Control(pDX, IDC_STC_OUTPUT_NAME_27, m_stIo_OuputName[26]);
	DDX_Control(pDX, IDC_STC_OUTPUT_NAME_28, m_stIo_OuputName[27]);
	DDX_Control(pDX, IDC_STC_OUTPUT_NAME_29, m_stIo_OuputName[28]);
	DDX_Control(pDX, IDC_STC_OUTPUT_NAME_30, m_stIo_OuputName[29]);
	DDX_Control(pDX, IDC_STC_OUTPUT_NAME_31, m_stIo_OuputName[30]);
	DDX_Control(pDX, IDC_STC_OUTPUT_NAME_32, m_stIo_OuputName[31]);

	DDX_Control(pDX, IDC_BTN_OUTPUT_ONOFF_01, m_btnIo_OutputActive[0]);
	DDX_Control(pDX, IDC_BTN_OUTPUT_ONOFF_02, m_btnIo_OutputActive[1]);
	DDX_Control(pDX, IDC_BTN_OUTPUT_ONOFF_03, m_btnIo_OutputActive[2]);
	DDX_Control(pDX, IDC_BTN_OUTPUT_ONOFF_04, m_btnIo_OutputActive[3]);
	DDX_Control(pDX, IDC_BTN_OUTPUT_ONOFF_05, m_btnIo_OutputActive[4]);
	DDX_Control(pDX, IDC_BTN_OUTPUT_ONOFF_06, m_btnIo_OutputActive[5]);
	DDX_Control(pDX, IDC_BTN_OUTPUT_ONOFF_07, m_btnIo_OutputActive[6]);
	DDX_Control(pDX, IDC_BTN_OUTPUT_ONOFF_08, m_btnIo_OutputActive[7]);
	DDX_Control(pDX, IDC_BTN_OUTPUT_ONOFF_09, m_btnIo_OutputActive[8]);
	DDX_Control(pDX, IDC_BTN_OUTPUT_ONOFF_10, m_btnIo_OutputActive[9]);
	DDX_Control(pDX, IDC_BTN_OUTPUT_ONOFF_11, m_btnIo_OutputActive[10]);
	DDX_Control(pDX, IDC_BTN_OUTPUT_ONOFF_12, m_btnIo_OutputActive[11]);
	DDX_Control(pDX, IDC_BTN_OUTPUT_ONOFF_13, m_btnIo_OutputActive[12]);
	DDX_Control(pDX, IDC_BTN_OUTPUT_ONOFF_14, m_btnIo_OutputActive[13]);
	DDX_Control(pDX, IDC_BTN_OUTPUT_ONOFF_15, m_btnIo_OutputActive[14]);
	DDX_Control(pDX, IDC_BTN_OUTPUT_ONOFF_16, m_btnIo_OutputActive[15]);
	DDX_Control(pDX, IDC_BTN_OUTPUT_ONOFF_17, m_btnIo_OutputActive[16]);
	DDX_Control(pDX, IDC_BTN_OUTPUT_ONOFF_18, m_btnIo_OutputActive[17]);
	DDX_Control(pDX, IDC_BTN_OUTPUT_ONOFF_19, m_btnIo_OutputActive[18]);
	DDX_Control(pDX, IDC_BTN_OUTPUT_ONOFF_20, m_btnIo_OutputActive[19]);
	DDX_Control(pDX, IDC_BTN_OUTPUT_ONOFF_21, m_btnIo_OutputActive[20]);
	DDX_Control(pDX, IDC_BTN_OUTPUT_ONOFF_22, m_btnIo_OutputActive[21]);
	DDX_Control(pDX, IDC_BTN_OUTPUT_ONOFF_23, m_btnIo_OutputActive[22]);
	DDX_Control(pDX, IDC_BTN_OUTPUT_ONOFF_24, m_btnIo_OutputActive[23]);
	DDX_Control(pDX, IDC_BTN_OUTPUT_ONOFF_25, m_btnIo_OutputActive[24]);
	DDX_Control(pDX, IDC_BTN_OUTPUT_ONOFF_26, m_btnIo_OutputActive[25]);
	DDX_Control(pDX, IDC_BTN_OUTPUT_ONOFF_27, m_btnIo_OutputActive[26]);
	DDX_Control(pDX, IDC_BTN_OUTPUT_ONOFF_28, m_btnIo_OutputActive[27]);
	DDX_Control(pDX, IDC_BTN_OUTPUT_ONOFF_29, m_btnIo_OutputActive[28]);
	DDX_Control(pDX, IDC_BTN_OUTPUT_ONOFF_30, m_btnIo_OutputActive[29]);
	DDX_Control(pDX, IDC_BTN_OUTPUT_ONOFF_31, m_btnIo_OutputActive[30]);
	DDX_Control(pDX, IDC_BTN_OUTPUT_ONOFF_32, m_btnIo_OutputActive[31]);

	DDX_Control(pDX, IDC_BUTTON_IO_BACK_1, m_btn_IoPageMove[0]);
	DDX_Control(pDX, IDC_BUTTON_IO_BACK_2, m_btn_IoPageMove[1]);
	DDX_Control(pDX, IDC_BUTTON_IO_BACK_3, m_btn_IoPageMove[2]);
	DDX_Control(pDX, IDC_BUTTON_IO_BACK_4, m_btn_IoPageMove[3]);
	
}


BEGIN_MESSAGE_MAP(CDlgIo, CDialogEx)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()

	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_OUTPUT_ONOFF_01, IDC_BTN_OUTPUT_ONOFF_32, &CDlgIo::OnChkOutClick)

	ON_BN_CLICKED(IDC_BUTTON_IO_BACK_1, &CDlgIo::OnBnClickedIoBack1)
	ON_BN_CLICKED(IDC_BUTTON_IO_BACK_2, &CDlgIo::OnBnClickedIoBack2)
	ON_BN_CLICKED(IDC_BUTTON_IO_BACK_3, &CDlgIo::OnBnClickedIoBack3)
	ON_BN_CLICKED(IDC_BUTTON_IO_BACK_4, &CDlgIo::OnBnClickedIoBack4)
END_MESSAGE_MAP()
BOOL CDlgIo::PreTranslateMessage(MSG* pMsg)
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

BOOL CDlgIo::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CRect rect;
	GetClientRect(rect);
	//pDC->FillSolidRect(rect, RGB(83, 169, 155));
	pDC->FillSolidRect(rect, RGB(84, 169, 169));
	return TRUE;
}

// CDlgIo 메시지 처리기
int CDlgIo::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.


	return 0;
}
void CDlgIo::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (bShow)
	{
		SetTimer(TIMER_IO_STATUS, 200, NULL);
	}
	else
	{
		KillTimer(TIMER_IO_STATUS);
	}
}

BOOL CDlgIo::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitComponent_DigitalIO();

	CString strTemp = _T("");
	strTemp.Format(_T("Input IO #%d"), m_nSel_DIO_Input + 1);
	m_stIo_Title[0].SetText(strTemp);
	strTemp.Format(_T("Output IO #%d"), m_nSel_DIO_Output + 1);
	m_stIo_Title[1].SetText(strTemp);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgIo::InitComponent_DigitalIO()
{
	CString strTemp = _T("");
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();

	for (int i = 0; i < 2; i++)
	{
		m_stIo_Title[i].SetStaticStyle(CVGStatic::StaticStyle_Title);
		m_stIo_Title[i].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_stIo_Title[i].SetFont_Gdip(L"Arial", 15.0F);

		if( i == 0 )
			m_stIo_Title[i].SetText(_T("INPUT IO"));
		else
			m_stIo_Title[i].SetText(_T("OUTPUT IO"));
	}


	// INPUT 숫자.
	int nTemp = 1;
	for (int i = 0; i < 32; i++)
	{
		strTemp.Format(_T("X %d%02d"), (m_nSel_DIO_Input/**2*/)+nTemp, i);

		m_stIo_InputNum[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_stIo_InputNum[i].SetFont_Gdip(L"Arial", 9.0F);
		m_stIo_InputNum[i].SetText(strTemp);
	}

	// INPUT IO 이름
	for (int i = 0; i < 32; i++)
	{
		m_stIo_InPutName[i].SetColorStyle(CVGStatic::ColorStyle_SlateGray);
		m_stIo_InPutName[i].SetFont_Gdip(L"Arial", 10);
		m_stIo_InPutName[i].SetTextAlignment(Gdiplus::StringAlignmentNear);

		if( stEq.nLanguage == 0 )
			m_stIo_InPutName[i].SetText(m_szDio_Input_Kor[i]);
		else
			m_stIo_InPutName[i].SetText(m_szDio_Input_Eng[i]);
	}

	//OUTPUT 번호
	nTemp = 1;
	for (int i = 0; i < 32; i++)
	{
		strTemp.Format(_T("Y %d%02d"), (m_nSel_DIO_Output /** 2*/)+nTemp, i);

		m_stIo_OuputNum[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_stIo_OuputNum[i].SetFont_Gdip(L"Arial", 9.0F);
		m_stIo_OuputNum[i].SetText(strTemp);
	}

	// OUTPUT IO 이름
	for (int i = 0; i < 32; i++)
	{
		m_stIo_OuputName[i].SetColorStyle(CVGStatic::ColorStyle_SlateGray);
		m_stIo_OuputName[i].SetFont_Gdip(L"Arial", 10);
		m_stIo_OuputName[i].SetTextAlignment(Gdiplus::StringAlignmentNear);
		m_stIo_OuputName[i].SetBorderColor(Gdiplus::Color::DarkGray);

		if (stEq.nLanguage == 0)
			m_stIo_OuputName[i].SetText(m_szDio_Output_Kor[i]);
		else
			m_stIo_OuputName[i].SetText(m_szDio_Output_Eng[i]);
	}

	// OUTPUT 버튼
	for (int i = 0; i < 32; i++)
	{
		//m_btnIo_OutputActive[i].SetStaticStyle(CVGStatic::StaticStyle_GroupHeader);
		//m_btnIo_OutputActive[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		//m_btnIo_OutputActive[i].SetFont_Gdip(L"Arial", 9.0F);

		//m_btnIo_OutputActive[i].SetText(_T("O"));
	}

	for (int i = 0; i < 4; i++)
	{
#if(GET_INSPECTOR == SYS_FILM_PEEL_OFF)
		m_btn_IoPageMove[i].ShowWindow(TRUE);
#endif
		m_btn_IoPageMove[i].SetTextColor(RGB(255,0,0));
		if( i % 2 == 0 )
			m_btn_IoPageMove[i].SetWindowText(_T("PREV"));
		else
			m_btn_IoPageMove[i].SetWindowText(_T("NEXT"));
	}
	
}

void CDlgIo::ChangeUI_Language()
{
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();

	for (int i = 0; i < 32; i++)
	{
		m_stIo_InPutName[i].SetFont_Gdip(L"Arial", 10);
		if (stEq.nLanguage == 0)
			m_stIo_InPutName[i].SetText(m_szDio_Input_Kor[i]);
		else
			m_stIo_InPutName[i].SetText(m_szDio_Input_Eng[i]);


		m_stIo_OuputName[i].SetFont_Gdip(L"Arial", 10);

		if (stEq.nLanguage == 0)
			m_stIo_OuputName[i].SetText(m_szDio_Output_Kor[i]);
		else
			m_stIo_OuputName[i].SetText(m_szDio_Output_Eng[i]);
	}
}


void CDlgIo::OnChkOutClick(UINT nID)
{
	int ID = nID - IDC_BTN_OUTPUT_ONOFF_01;

	if (m_nOutputBtn_Status[m_nSel_DIO_Output][ID] == 0)
	{
		AJIN_IO->SendOutputBit(ID + (m_nSel_DIO_Output * 32), TRUE);
		m_nOutputBtn_Status[m_nSel_DIO_Output][ID] = 1;

	}
	else
	{
		AJIN_IO->SendOutputBit(ID + (m_nSel_DIO_Output * 32), FALSE);

		m_nOutputBtn_Status[m_nSel_DIO_Output][ID] = 0;
	}
}



void CDlgIo::OnBnClickedIoBack1()
{
	if (m_nSel_DIO_Input <= 0)
		return;

	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();

	m_nSel_DIO_Input_Old = m_nSel_DIO_Input;

	m_nSel_DIO_Input--;

	CString strTemp = _T("");
	int nTemp = 1;

	for (int i = 0; i < 32; i++)
	{
		//if ((i / 16) > 0) nTemp = 1;

		strTemp.Format(_T("X %d%02d"), (/*2 **/ m_nSel_DIO_Input)+nTemp, i);

		m_stIo_InputNum[i].SetText(strTemp);
	}

	for (int i = 0; i < 32; i++)
	{
	
		if (stEq.nLanguage == 0)
			m_stIo_InPutName[i].SetText(m_szDio_Input_Kor[i + (m_nSel_DIO_Input * 32)]);
		else
			m_stIo_InPutName[i].SetText(m_szDio_Input_Eng[i + (m_nSel_DIO_Input * 32)]);
	}

	strTemp.Format(_T("Input IO #%d"), m_nSel_DIO_Input + 1);
	//m_stInputIO_Title.SetStaticStyle(CVGStatic::StaticStyle_Title);
	//m_stInputIO_Title.SetColorStyle(CVGStatic::ColorStyle_Sky);
	//m_stIo_Title[0].SetFont_Gdip(L"Arial", 16.0F);
	m_stIo_Title[0].SetText(strTemp);

}
void CDlgIo::OnBnClickedIoBack2()
{
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();

	int nTemp1 = 0;
	int nTemp2 = 0;
	
	nTemp1 = (int)INPUT_IO_MAX_COUNT / 32;
	nTemp2 = (int)INPUT_IO_MAX_COUNT % 32;
	
	if (m_nSel_DIO_Input >= (nTemp1 - 1))
		return;

	BOOL bResult = FALSE;
	if (nTemp1 > 0 && nTemp2 > 0)
		bResult = TRUE;
	
	if (nTemp1 > 1 && nTemp2 == 0)
		bResult = TRUE;

	if (bResult == FALSE)
		return;

	m_nSel_DIO_Input_Old = m_nSel_DIO_Input;

	m_nSel_DIO_Input++;

	CString strTemp = _T("");
	int nTemp = 1;

	for (int i = 0; i < 32; i++)
	{
		//if ((i / 16) > 0) nTemp = 1;

		strTemp.Format(_T("X %d%02d"), (/*2 **/ m_nSel_DIO_Input) + nTemp, i);

		m_stIo_InputNum[i].SetText(strTemp);
	}

	for (int i = 0; i < 32; i++)
	{

		if (stEq.nLanguage == 0)
			m_stIo_InPutName[i].SetText(m_szDio_Input_Kor[i + (m_nSel_DIO_Input * 32)]);
		else
			m_stIo_InPutName[i].SetText(m_szDio_Input_Eng[i + (m_nSel_DIO_Input * 32)]);
	}

	strTemp.Format(_T("Input IO #%d"), m_nSel_DIO_Input + 1);
	//m_stInputIO_Title.SetStaticStyle(CVGStatic::StaticStyle_Title);
	//m_stInputIO_Title.SetColorStyle(CVGStatic::ColorStyle_Sky);
	//m_stIo_Title[0].SetFont_Gdip(L"Arial", 16.0F);
	m_stIo_Title[0].SetText(strTemp);
}
void CDlgIo::OnBnClickedIoBack3()
{
	if (m_nSel_DIO_Output <= 0)
		return;

	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();

	m_nSel_DIO_Output_Old = m_nSel_DIO_Output;

	m_nSel_DIO_Output--;

	CString strTemp = _T("");
	int nTemp = 1;

	for (int i = 0; i < 32; i++)
	{
		//if ((i / 16) > 0) nTemp = 1;

		strTemp.Format(_T("Y %d%02d"), (/*2 **/ m_nSel_DIO_Output) + nTemp, i);

		m_stIo_OuputNum[i].SetText(strTemp);
	}

	for (int i = 0; i < 32; i++)
	{
		if (stEq.nLanguage == 0)
			m_stIo_OuputName[i].SetText(m_szDio_Output_Kor[i + (m_nSel_DIO_Output * 32)]);
		else
			m_stIo_OuputName[i].SetText(m_szDio_Output_Eng[i + (m_nSel_DIO_Output * 32)]);
	}

	strTemp.Format(_T("Output IO #%d"), m_nSel_DIO_Output + 1);
	//m_stInputIO_Title.SetStaticStyle(CVGStatic::StaticStyle_Title);
	//m_stInputIO_Title.SetColorStyle(CVGStatic::ColorStyle_Sky);
	//m_stIo_Title[1].SetFont_Gdip(L"Arial", 16.0F);
	m_stIo_Title[1].SetText(strTemp);
}
void CDlgIo::OnBnClickedIoBack4()
{
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();

	int nTemp1 = 0;
	int nTemp2 = 0;

	nTemp1 = (int)OUTPUT_IO_MAX_COUNT / 32;
	nTemp2 = (int)OUTPUT_IO_MAX_COUNT % 32;

	if (m_nSel_DIO_Output >= (nTemp1-1))
		return;

	BOOL bResult = FALSE;
	if (nTemp1 > 0 && nTemp2 > 0)
		bResult = TRUE;
	
	if (nTemp1 > 1 && nTemp2 == 0)
		bResult = TRUE;


	if (bResult == FALSE)
		return;

	m_nSel_DIO_Output_Old = m_nSel_DIO_Output;

	m_nSel_DIO_Output++;

	CString strTemp = _T("");
	int nTemp = 1;

	for (int i = 0; i < 32; i++)
	{
		//if ((i / 16) > 0) nTemp = 1;

		strTemp.Format(_T("Y %d%02d"), (/*2 **/ m_nSel_DIO_Output) + nTemp, i);

		m_stIo_OuputNum[i].SetText(strTemp);
	}

	for (int i = 0; i < 32; i++)
	{	
		if (stEq.nLanguage == 0)
			m_stIo_OuputName[i].SetText(m_szDio_Output_Kor[i + (m_nSel_DIO_Output * 32)]);
		else
			m_stIo_OuputName[i].SetText(m_szDio_Output_Eng[i + (m_nSel_DIO_Output * 32)]);
	}

	strTemp.Format(_T("Output IO #%d"), m_nSel_DIO_Output + 1);
	//m_stInputIO_Title.SetStaticStyle(CVGStatic::StaticStyle_Title);
	//m_stInputIO_Title.SetColorStyle(CVGStatic::ColorStyle_Sky);
	//m_stIo_Title[1].SetFont_Gdip(L"Arial", 16.0F);
	m_stIo_Title[1].SetText(strTemp);
}


void CDlgIo::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnTimer(nIDEvent);

	switch (nIDEvent)
	{
	case TIMER_IO_STATUS:
	{
		Set_Disp_Input();				//Input I/O 값 표시
		Set_Disp_Output();
	}
	break;

	}
}

void CDlgIo::Set_Disp_Input()
{
	if ((m_dwInput_Old[m_nSel_DIO_Input] != AJIN_IO->m_dwInputBuf[m_nSel_DIO_Input]) || (m_nSel_DIO_Input != m_nSel_DIO_Input_Old))
	{
		m_dwInput_Old[m_nSel_DIO_Input] = AJIN_IO->m_dwInputBuf[m_nSel_DIO_Input];

		for (int i = 0; i < 32; i++)
		{
			if ((m_dwInput_Old[m_nSel_DIO_Input] >> i) & 0x01)
			{
				m_stIo_InPutName[i].SetBackColor(Gdiplus::Color::Green);
				//m_stIo_InPutName[i].SetColorStyle(CVGStatic::ColorStyle_Green);
				//m_stIo_InputNum[i].SetColorStyle(CVGStatic::ColorStyle_Navy);
			}
			else
			{
				m_stIo_InPutName[i].SetBackColor(Gdiplus::Color::SlateGray);
				//m_stIo_InPutName[i].SetColorStyle(CVGStatic::ColorStyle_SlateGray);
				//m_stIo_InputNum[i].SetColorStyle(CVGStatic::ColorStyle_Black);
			}
		}
	}
}

void CDlgIo::Set_Disp_Output()
{
	if ((m_dwOutput_Old[m_nSel_DIO_Output] != AJIN_IO->m_dwOutputBuf[m_nSel_DIO_Output]) || (m_nSel_DIO_Output != m_nSel_DIO_Output_Old))
	{
		m_dwOutput_Old[m_nSel_DIO_Output] = AJIN_IO->m_dwOutputBuf[m_nSel_DIO_Output];

		for (int i = 0; i < 32; i++)
		{
			if ((m_dwOutput_Old[m_nSel_DIO_Output] >> i) & 0x01)
			{
				m_stIo_OuputName[i].SetBackColor(Gdiplus::Color::Red);
				//m_stIo_OuputName[i].SetColorStyle(CVGStatic::ColorStyle_Red);
				//m_stIo_OuputNum[i].SetColorStyle(CVGStatic::ColorStyle_Navy);
				//m_stIo_OuputNum[i].SetColorStyle(CVGStatic::ColorStyle_Navy);

				if (m_nOutputBtn_Status[m_nSel_DIO_Output][i] == 0)
				{
					m_nOutputBtn_Status[m_nSel_DIO_Output][i] = 1;

				//	m_btnIo_OutputActive[i].SetColorStyle(CVGStatic::ColorStyle_Navy);
				//	m_btnIo_OutputActive[i].SetFont_Gdip(L"Arial", 9.0F);
				//	m_btnIo_OutputActive[i].SetText(_T("ON"));
				}
			}
			else
			{
				m_stIo_OuputName[i].SetBackColor(Gdiplus::Color::SlateGray);
				//m_stIo_OuputName[i].SetColorStyle(CVGStatic::ColorStyle_SlateGray);
				//m_stIo_OuputNum[i].SetColorStyle(CVGStatic::ColorStyle_Black);
				//m_stIo_OuputNum[i].SetColorStyle(CVGStatic::ColorStyle_Black);

				if (m_nOutputBtn_Status[m_nSel_DIO_Output_Old][i] == 1)
				{
					m_nOutputBtn_Status[m_nSel_DIO_Output_Old][i] = 0;

				//	m_btnIo_OutputActive[i].SetColorStyle(CVGStatic::ColorStyle_Black);
				//	m_btnIo_OutputActive[i].SetFont_Gdip(L"Arial", 9.0F);
				//	m_btnIo_OutputActive[i].SetText(_T("OFF"));
				}
			}
		}
	}
}