// DlgLanguage.cpp: 구현 파일
//

#include "pch.h"
#include "LuriMotion_OQA_SFR_Wide.h"
#include "DlgLanguage.h"
#include "afxdialogex.h"


// CDlgLanguage 대화 상자

IMPLEMENT_DYNAMIC(CDlgLanguage, CDialogEx)

CDlgLanguage::CDlgLanguage(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_LANGUAGE_2, pParent)
{
	m_nChangeLanguage = -1;
}

CDlgLanguage::~CDlgLanguage()
{
}

void CDlgLanguage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STC_TITLE_LANG, m_stTitle);
	
	DDX_Control(pDX, IDC_BTN_SELECT_LANGUAGE_1, m_btnSelectLanguage[0]);
	DDX_Control(pDX, IDC_BTN_SELECT_LANGUAGE_2, m_btnSelectLanguage[1]);
	DDX_Control(pDX, IDC_BTN_SELECT_LANGUAGE_3, m_btnSelectLanguage[2]);
	DDX_Control(pDX, IDC_BTN_SELECT_LANGUAGE_4, m_btnSelectLanguage[3]);

	DDX_Control(pDX, IDC_BTN_OK, m_btnOk);
	
}


BEGIN_MESSAGE_MAP(CDlgLanguage, CDialogEx)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BTN_OK, &CDlgLanguage::OnBnClickedBtnOk)
	ON_BN_CLICKED(IDC_BTN_SELECT_LANGUAGE_1, &CDlgLanguage::OnBnClickedBtnSelectLanguage1)
	ON_BN_CLICKED(IDC_BTN_SELECT_LANGUAGE_2, &CDlgLanguage::OnBnClickedBtnSelectLanguage2)
	ON_BN_CLICKED(IDC_BTN_SELECT_LANGUAGE_3, &CDlgLanguage::OnBnClickedBtnSelectLanguage3)
	ON_BN_CLICKED(IDC_BTN_SELECT_LANGUAGE_4, &CDlgLanguage::OnBnClickedBtnSelectLanguage4)
END_MESSAGE_MAP()


// CDlgLanguage 메시지 처리기
BOOL CDlgLanguage::PreTranslateMessage(MSG* pMsg)
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
int CDlgLanguage::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	//InitComponent_MessageBox();

	return 0;
}
BOOL CDlgLanguage::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CRect rect;
	GetClientRect(rect);
	//pDC->FillSolidRect(rect, RGB(105, 186, 235));
	pDC->FillSolidRect(rect, RGB(83, 169, 155));
	//pDC->FillSolidRect(rect, RGB(227, 255, 0));		// Lime
	return TRUE;
}
BOOL CDlgLanguage::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	InitComponnet_Language();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgLanguage::InitComponnet_Language()
{
	EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();

	m_stTitle.SetStaticStyle(CVGStatic::StaticStyle_GroupHeader);
	m_stTitle.SetColorStyle(CVGStatic::ColorStyle_Black);
	m_stTitle.SetFont_Gdip(L"Arial", 12.0F);

	if( stBasic.nLanguage == 0 )
		m_stTitle.SetText(_T("언어를 선택하세요"));
	else
		m_stTitle.SetText(_T("Select Language"));


	for (int i = 0; i < 4; i++)
	{
		m_btnSelectLanguage[i].SetStaticStyle(CVGStatic::StaticStyle_GroupHeader);

		if( i == 0 )
		{		
			if (stBasic.nLanguage == 0)
				m_btnSelectLanguage[i].SetColorStyle(CVGStatic::ColorStyle_Green);
			else
				m_btnSelectLanguage[i].SetColorStyle(CVGStatic::ColorStyle_Black);
		}
		else if (i == 1)
		{
			if (stBasic.nLanguage == 1)
				m_btnSelectLanguage[i].SetColorStyle(CVGStatic::ColorStyle_Green);
			else
				m_btnSelectLanguage[i].SetColorStyle(CVGStatic::ColorStyle_Black);
		}
		//else if (i == 2)
		//{
		//	if (stBasic.nLanguage == 2)
		//		m_btnSelectLanguage[i].SetColorStyle(CVGStatic::ColorStyle_Green);
		//	else
		//		m_btnSelectLanguage[i].SetColorStyle(CVGStatic::ColorStyle_Black);
		//}

		m_btnSelectLanguage[i].SetFont_Gdip(L"Arial", 12.0F);
		if (stBasic.nLanguage == 0)
			m_btnSelectLanguage[i].SetText(m_szSelect_Language_Kor[i]);
		else
			m_btnSelectLanguage[i].SetText(m_szSelect_Language_Eng[i]);
	}

	m_btnOk.SetStaticStyle(CVGStatic::StaticStyle_Title);
	m_btnOk.SetColorStyle(CVGStatic::ColorStyle_Black);
	m_btnOk.SetFont_Gdip(L"Arial", 12.0F);

	if (stBasic.nLanguage == 0)
		m_btnOk.SetText(_T("확인"));
	else
		m_btnOk.SetText(_T("OK"));
	

	m_nLanguageNum = stBasic.nLanguage;
}

int CDlgLanguage::GetChangeLanguage()
{
	return m_nChangeLanguage;
}


void CDlgLanguage::OnBnClickedBtnOk()
{
	EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();

	stBasic.nLanguage = m_nLanguageNum;

	DATAMANAGER->SetEqData(stBasic);

	m_nChangeLanguage = m_nLanguageNum;

	EndDialog(IDOK);
}


void CDlgLanguage::OnBnClickedBtnSelectLanguage1()
{
	m_nLanguageNum = 0;
	ChangeUi_Language();
}
void CDlgLanguage::OnBnClickedBtnSelectLanguage2()
{
	m_nLanguageNum = 1;
	ChangeUi_Language();
}
void CDlgLanguage::OnBnClickedBtnSelectLanguage3()
{
	return;
	m_nLanguageNum = 2;
	ChangeUi_Language();
}
void CDlgLanguage::OnBnClickedBtnSelectLanguage4()
{
	return;
	m_nLanguageNum = 3;
	ChangeUi_Language();
}

void CDlgLanguage::ChangeUi_Language()
{
	EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();

	for (int i = 0; i < 2; i++)
	{
		if (m_nLanguageNum == i)
		{
			m_btnSelectLanguage[i].SetStaticStyle(CVGStatic::StaticStyle_GroupHeader);
			m_btnSelectLanguage[i].SetColorStyle(CVGStatic::ColorStyle_Green);
			m_btnSelectLanguage[i].SetFont_Gdip(L"Arial", 12.0F);

			if( stBasic.nLanguage == 0 )
				m_btnSelectLanguage[i].SetText(m_szSelect_Language_Kor[i]);
			else if (stBasic.nLanguage == 1 )
				m_btnSelectLanguage[i].SetText(m_szSelect_Language_Eng[i]);
		}
		else
		{
			m_btnSelectLanguage[i].SetStaticStyle(CVGStatic::StaticStyle_GroupHeader);
			m_btnSelectLanguage[i].SetColorStyle(CVGStatic::ColorStyle_Black);
			m_btnSelectLanguage[i].SetFont_Gdip(L"Arial", 12.0F);

			if (stBasic.nLanguage == 0)
				m_btnSelectLanguage[i].SetText(m_szSelect_Language_Kor[i]);
			else if (stBasic.nLanguage == 1)
				m_btnSelectLanguage[i].SetText(m_szSelect_Language_Eng[i]);

		}
	}
}