// DlgBottom.cpp: 구현 파일
//

#include "pch.h"
#include "DlgBottom.h"
#include "LuriMotion_OQA_SFR_WideDlg.h"
#include "afxdialogex.h"


// CDlgBottom 대화 상자

IMPLEMENT_DYNAMIC(CDlgBottom, CDialogEx)

CDlgBottom::CDlgBottom(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_BOTTOM, pParent)
{
	m_nCurrentSelectDlgNum = 1;
}

CDlgBottom::~CDlgBottom()
{
}

void CDlgBottom::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BTN_TEST1, m_btnNavigationMenu[0]);
	DDX_Control(pDX, IDC_BTN_TEST2, m_btnNavigationMenu[1]);
	DDX_Control(pDX, IDC_BTN_TEST3, m_btnNavigationMenu[2]);
	DDX_Control(pDX, IDC_BTN_TEST4, m_btnNavigationMenu[3]);
	DDX_Control(pDX, IDC_BTN_TEST5, m_btnNavigationMenu[4]);
	DDX_Control(pDX, IDC_BTN_TEST6, m_btnNavigationMenu[5]);
	DDX_Control(pDX, IDC_BTN_TEST7, m_btnNavigationMenu[6]);
	DDX_Control(pDX, IDC_BTN_TEST8, m_btnNavigationMenu[7]);	
}

BEGIN_MESSAGE_MAP(CDlgBottom, CDialogEx)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_TEST1, &CDlgBottom::OnBnClickedBtnTest1)
	ON_BN_CLICKED(IDC_BTN_TEST2, &CDlgBottom::OnBnClickedBtnTest2)
	ON_BN_CLICKED(IDC_BTN_TEST3, &CDlgBottom::OnBnClickedBtnTest3)
	ON_BN_CLICKED(IDC_BTN_TEST4, &CDlgBottom::OnBnClickedBtnTest4)
	ON_BN_CLICKED(IDC_BTN_TEST5, &CDlgBottom::OnBnClickedBtnTest5)
	ON_BN_CLICKED(IDC_BTN_TEST6, &CDlgBottom::OnBnClickedBtnTest6)
	ON_BN_CLICKED(IDC_BTN_TEST7, &CDlgBottom::OnBnClickedBtnTest7)
	ON_BN_CLICKED(IDC_BTN_TEST8, &CDlgBottom::OnBnClickedBtnTest8)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

BOOL CDlgBottom::PreTranslateMessage(MSG* pMsg)
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
// CDlgBottom 메시지 처리기

void CDlgBottom::OnLButtonDown(UINT nFlags, CPoint point)
{
	// [21.1021.4]
	CLuriMotion_OQA_SFR_WideDlg *pParrentDlg = (CLuriMotion_OQA_SFR_WideDlg*)AfxGetApp()->GetMainWnd();
	pParrentDlg->SendMessage(MESSAGE_UI_WINDOW_TOP, NULL, NULL);

	CDialogEx::OnLButtonDown(nFlags, point);
}


BOOL CDlgBottom::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(135, 206, 235));		// SkyBlue
	//pDC->FillSolidRect(rect, RGB(112, 146, 190));

	return TRUE;
}

int CDlgBottom::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	

	return 0;
}
void CDlgBottom::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (bShow)
	{

	}
}
BOOL CDlgBottom::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitComponent_Bottom();
	ChangeButtonState(1);

	InitUIEqpType();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgBottom::InitComponent_Bottom()
{
	EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();

	int nGap = 15;
	CRect rt;
	CRect rt_Move;
	
	m_btnNavigationMenu[0].GetWindowRect(rt);

	int nStart_X = 0;
	int nStart_Y = 20;

	int nResult_X = 0;
	int nResult_Y = 0;

	int nWidth = rt.Width();
	for (int i = 0; i < 7; i++)
	{
		nResult_X = nStart_X + nGap + ( (nWidth *2) * i ) + (nGap*i);
		nResult_Y = nStart_Y;

		rt_Move.left = nResult_X + nGap;
		rt_Move.top = nResult_Y;
		rt_Move.right = nResult_X + (nWidth *2);
		rt_Move.bottom = nResult_Y + (rt.Height());
		
		m_btnNavigationMenu[i].MoveWindow(rt_Move, SWP_SHOWWINDOW);
	}

	rt_Move.left = 1724;
	rt_Move.top = nResult_Y;
	rt_Move.right = 1724 + (rt.Width() * 2);
	rt_Move.bottom = nResult_Y + (rt.Height());

	m_btnNavigationMenu[7].MoveWindow(rt_Move, SWP_SHOWWINDOW);
	
	for (int i = 0; i < 8; i++)
	{
		m_btnNavigationMenu[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_btnNavigationMenu[i].SetColorStyle(CVGStatic::ColorStyle_DeepDarkGray);
		m_btnNavigationMenu[i].SetFont_Gdip(L"Arial", 9.0F);

		if( stBasic.nLanguage == 0 )
			m_btnNavigationMenu[i].SetText(m_szBottomMenuKor[i]);
		else
			m_btnNavigationMenu[i].SetText(m_szBottomMenuEng[i]);
	}
}
 void CDlgBottom::ChangeButtonState(int nNumber)
{
	 if (nNumber < 1)
		 return;

	 EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();

	 for (int i = 0; i < 8; i++)
	 {
		 if (i == nNumber - 1)
		 {
			 m_btnNavigationMenu[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
			 m_btnNavigationMenu[i].SetColorStyle(CVGStatic::ColorStyle_Green);
			 m_btnNavigationMenu[i].SetFont_Gdip(L"Arial", 9.0F);
			 if (stBasic.nLanguage == 0)
				 m_btnNavigationMenu[i].SetText(m_szBottomMenuKor[i]);
			 else
				 m_btnNavigationMenu[i].SetText(m_szBottomMenuEng[i]);
		 }
		 else
		 {
			 m_btnNavigationMenu[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
			 m_btnNavigationMenu[i].SetColorStyle(CVGStatic::ColorStyle_DeepDarkGray);
			 m_btnNavigationMenu[i].SetFont_Gdip(L"Arial", 9.0F);
			 if (stBasic.nLanguage == 0)
				 m_btnNavigationMenu[i].SetText(m_szBottomMenuKor[i]);
			 else
				 m_btnNavigationMenu[i].SetText(m_szBottomMenuEng[i]);
		 }
	 }

	 InitUIEqpType();
}

void CDlgBottom::OnBnClickedBtnTest1()
{
	CLuriMotion_OQA_SFR_WideDlg *pParrentDlg = (CLuriMotion_OQA_SFR_WideDlg*)AfxGetApp()->GetMainWnd();

	pParrentDlg->ChangeMainUi(1);

	ChangeButtonState(1);

	m_nCurrentSelectDlgNum = 1;
}

// [21.1017.13] Modified 
void CDlgBottom::OnBnClickedBtnTest2()
{
	EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();
	//if (COMMON->GetPermissionGrade() < 1)
	//{
	//	if (stBasic.nLanguage == 0)
	//		COMMON->ShowMsgDlg_Domodal(_T("권한이 없습니다."), MODE_INFORMATION);
	//	else
	//		COMMON->ShowMsgDlg_Domodal(_T("You don't have permission."), MODE_INFORMATION);
	//	return;
	//}		

	CLuriMotion_OQA_SFR_WideDlg *pParrentDlg = (CLuriMotion_OQA_SFR_WideDlg*)AfxGetApp()->GetMainWnd();

	pParrentDlg->ChangeMainUi(2);	

	ChangeButtonState(2);

	m_nCurrentSelectDlgNum = 2;
}

// [21.1017.14] Modified 
void CDlgBottom::OnBnClickedBtnTest3()
{
	EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();
	if (COMMON->GetPermissionGrade() < LVL_MAINT)
	{
		if (stBasic.nLanguage == 0)
			COMMON->ShowMsgDlg_Domodal(_T("권한이 없습니다."), MODE_INFORMATION);
		else
			COMMON->ShowMsgDlg_Domodal(_T("You don't have permission."), MODE_INFORMATION);
		return;
	}
	CLuriMotion_OQA_SFR_WideDlg *pParrentDlg = (CLuriMotion_OQA_SFR_WideDlg*)AfxGetApp()->GetMainWnd();

	pParrentDlg->ChangeMainUi(3);

	ChangeButtonState(3);

	m_nCurrentSelectDlgNum = 3;
}

// [21.1017.15] Modified 
void CDlgBottom::OnBnClickedBtnTest4()
{
	EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();
	if (COMMON->GetPermissionGrade() < LVL_MAINT)
	{
		if (stBasic.nLanguage == 0)
			COMMON->ShowMsgDlg_Domodal(_T("권한이 없습니다."), MODE_INFORMATION);
		else
			COMMON->ShowMsgDlg_Domodal(_T("You don't have permission."), MODE_INFORMATION);
		return;
	}
	CLuriMotion_OQA_SFR_WideDlg *pParrentDlg = (CLuriMotion_OQA_SFR_WideDlg*)AfxGetApp()->GetMainWnd();

	pParrentDlg->ChangeMainUi(4);

	ChangeButtonState(4);

	m_nCurrentSelectDlgNum = 4;
}
void CDlgBottom::OnBnClickedBtnTest5()
{
	// Hide
// 	CLuriMotion_OQA_SFR_WideDlg *pParrentDlg = (CLuriMotion_OQA_SFR_WideDlg*)AfxGetApp()->GetMainWnd();
// 
// 	pParrentDlg->Hide_Program();

	EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();
	if (COMMON->GetPermissionGrade() < LVL_MAINT)
	{
		if (stBasic.nLanguage == 0)
			COMMON->ShowMsgDlg_Domodal(_T("권한이 없습니다."), MODE_INFORMATION);
		else
			COMMON->ShowMsgDlg_Domodal(_T("You don't have permission."), MODE_INFORMATION);
		return;
	}
	CLuriMotion_OQA_SFR_WideDlg *pParrentDlg = (CLuriMotion_OQA_SFR_WideDlg*)AfxGetApp()->GetMainWnd();

	pParrentDlg->ChangeMainUi(5);

	ChangeButtonState(5);

	m_nCurrentSelectDlgNum = 5;

	return;

	//if (COMMON->GetPermissionGrade() < 2)
	//{
	//	COMMON->ShowMsgDlg_Domodal(_T("권한이 없습니다."), MODE_INFORMATION);
	//	return;
	//}

	//CLuriMotion_OQA_SFR_NarrowDlg *pParrentDlg = (CLuriMotion_OQA_SFR_NarrowDlg*)AfxGetApp()->GetMainWnd();

	//pParrentDlg->ChangeMainUi(5);

	//ChangeButtonState(5);

	//m_nCurrentSelectDlgNum = 5;
}
void CDlgBottom::OnBnClickedBtnTest6()
{

	// Hide
	CLuriMotion_OQA_SFR_WideDlg *pParrentDlg = (CLuriMotion_OQA_SFR_WideDlg*)AfxGetApp()->GetMainWnd();

	pParrentDlg->Hide_Program();

	return;

	//ChangeButtonState(6);

	//m_nCurrentSelectDlgNum = 6;

	//Recipe_Load();

	//// 각각의 다이얼로그에서 레시피를 변경하면 UI가 변경되지 않는다.
	//// 레시피 변경시 UI를 같이 변경시키기 위해 데이터를 다시 로드 하도록 하자
	//CLuriMotion_OQA_SFR_WideDlg *pParrentDlg = (CLuriMotion_OQA_SFR_WideDlg*)AfxGetApp()->GetMainWnd();
	//pParrentDlg->RecipeChange_Ui_Refresh();
	//

}

void CDlgBottom::OnBnClickedBtnTest7()
{
	// Hide
	CLuriMotion_OQA_SFR_WideDlg *pParrentDlg = (CLuriMotion_OQA_SFR_WideDlg*)AfxGetApp()->GetMainWnd();

	pParrentDlg->Hide_Program();
}

void CDlgBottom::OnBnClickedBtnTest8()
{
	// PROGRAM EXIT
	CLuriMotion_OQA_SFR_WideDlg *pParrentDlg = (CLuriMotion_OQA_SFR_WideDlg*)AfxGetApp()->GetMainWnd();
	EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();
	CString szStr = _T("");

	if (stBasic.nLanguage == 0)
		szStr.Format(_T("프로그램을 종료하시겠습니까?"));
	else
		szStr.Format(_T("Do you want to Close the Program?"));

	// [21.1214.6]
	if (AfxMessageBox(szStr, MB_YESNO) == IDYES)
	{
		pParrentDlg->m_pDlgSide->KillTimer(TIMER_100_SWITCH_LAMPCHECK);
		pParrentDlg->m_pDlgSide->KillTimer(TIMER_200_EMO_POWER_CHECK);
		pParrentDlg->m_pDlgSide->KillTimer(TIMER_300_CHANGE_MODE);

		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_09_TOWERLAMP_BEEP1, FALSE);
		pParrentDlg->OnClose();
	}
}

void CDlgBottom::ChangeUi_Language()
{
	EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();

	for (int i = 0; i < 7; i++)
	{
		m_btnNavigationMenu[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_btnNavigationMenu[i].SetColorStyle(CVGStatic::ColorStyle_DeepDarkGray);
		m_btnNavigationMenu[i].SetFont_Gdip(L"Arial", 9.0F);

		if (stBasic.nLanguage == 0)
			m_btnNavigationMenu[i].SetText(m_szBottomMenuKor[i]);
		else
			m_btnNavigationMenu[i].SetText(m_szBottomMenuEng[i]);
	}

	ChangeButtonState(m_nCurrentSelectDlgNum);
}


void CDlgBottom::Recipe_Load()
{
	EQ_BASIC_PARAM	stEq = *DATAMANAGER->GetCurrentEqBasicData();
	if (COMMON->GetPermissionGrade() == 0)
	{
		if(stEq.nLanguage ==0)
			COMMON->ShowMsgDlg_Domodal(_T("Admin 권한이 필요합니다."), MODE_INFORMATION);
		else
			COMMON->ShowMsgDlg_Domodal(_T("Admin rights are required."), MODE_INFORMATION);
		return;
	}

	int nIndex = 0;
	int nLength = 0;
	CString strLog = _T("");
	CString strPath = _T("");
	CString strModelName = _T("");

	CFileDialog fileDlg(TRUE, L"*.Mod", L"*.Mod", OFN_READONLY,
		_T("Handler Model File (*.Mod)|*.Mod| All File (*.*)|*.*|"));

	strPath.Format(_T("%s"), _PROGRAM_RECIPE_DIR);
	fileDlg.m_ofn.lpstrInitialDir = strPath;

	if (fileDlg.DoModal() == IDOK)
	{
		//Model file 경로
		strPath = fileDlg.GetPathName();

		nLength = strPath.GetLength();
		strPath.Delete(nLength - 4, 4);

		//모델 명
		nLength = strPath.GetLength();
		nIndex = strPath.ReverseFind('\\');
		strModelName = strPath.Mid(nIndex + 1, nLength - nIndex - 1);
		strPath.Delete(nIndex, nLength - nIndex);

		// 변경하기전에 마지막으로 불러온 레시피를 저장하자.
		stEq.strLastRecipe = strModelName;
		DATAMANAGER->SetEqData(stEq);

		// UI 변경용 FLAG
		COMMON->m_bRecipeChange = TRUE;

		// 레시피 변경.
		DATAMANAGER->RecipeChange(strModelName);	
	}
}

void CDlgBottom::InitUIEqpType()
{
	auto EqpType = GET_INSPECTOR;

	switch (EqpType)
	{
	case SYS_DISTORTION_30:
// 		m_btnNavigationMenu[1].EnableWindow(FALSE);
// 		m_btnNavigationMenu[1].SetStaticStyle(CVGStatic::StaticStyle_Title);
// 		m_btnNavigationMenu[1].SetColorStyle(CVGBase_Static::ColorStyle_SlateGray);
// 		m_btnNavigationMenu[1].SetText(_T("-"));
		break;
	default:
		break;
	}
}