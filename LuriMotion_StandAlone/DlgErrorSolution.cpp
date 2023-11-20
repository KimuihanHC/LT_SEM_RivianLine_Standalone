// DlgErrorSolution.cpp: 구현 파일
//

#include "pch.h"
#include "LuriMotion_OQA_SFR_Wide.h"
#include "DlgErrorSolution.h"
#include "afxdialogex.h"


// CDlgErrorSolution 대화 상자

IMPLEMENT_DYNAMIC(CDlgErrorSolution, CDialogEx)

CDlgErrorSolution::CDlgErrorSolution(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_ERROR_SOL, pParent)
{
	m_nCurrentPageCount = 0;
	for(int i=0; i<10; i++)
	{
		m_bSolutionBtnFlag[i] = FALSE;
	}

	m_nPartErrorCnt_1 = 0;
	m_nPartErrorCnt_2 = 0;
	m_nPartErrorCnt_3 = 0;
}

CDlgErrorSolution::~CDlgErrorSolution()
{
}

void CDlgErrorSolution::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STC_SOLUTION_TITLE, m_stErrorTitle[0]);
	DDX_Control(pDX, IDC_STC_SOLUTION_TITLE2, m_stErrorTitle[1]);
	DDX_Control(pDX, IDC_STC_SOLUTION_TITLE3, m_stErrorTitle[2]);
	

	DDX_Control(pDX, IDC_STC_ERROR_SOL_SUB_TITLE_1, m_stErrorSubTitle[0]);
	DDX_Control(pDX, IDC_STC_ERROR_SOL_SUB_TITLE_2, m_stErrorSubTitle[1]);
	DDX_Control(pDX, IDC_STC_ERROR_SOL_SUB_TITLE_3, m_stErrorSubTitle[2]);
	DDX_Control(pDX, IDC_STC_ERROR_SOL_SUB_TITLE_4, m_stErrorSubTitle[3]);
	DDX_Control(pDX, IDC_STC_ERROR_SOL_SUB_TITLE_5, m_stErrorSubTitle[4]);

	DDX_Control(pDX, IDC_STC_ERROR_SOL_COUNT_01, m_stErrorCnt[0]);
	DDX_Control(pDX, IDC_STC_ERROR_SOL_COUNT_02, m_stErrorCnt[1]);
	DDX_Control(pDX, IDC_STC_ERROR_SOL_COUNT_03, m_stErrorCnt[2]);
	DDX_Control(pDX, IDC_STC_ERROR_SOL_COUNT_04, m_stErrorCnt[3]);
	DDX_Control(pDX, IDC_STC_ERROR_SOL_COUNT_05, m_stErrorCnt[4]);
	DDX_Control(pDX, IDC_STC_ERROR_SOL_COUNT_06, m_stErrorCnt[5]);
	DDX_Control(pDX, IDC_STC_ERROR_SOL_COUNT_07, m_stErrorCnt[6]);
	DDX_Control(pDX, IDC_STC_ERROR_SOL_COUNT_08, m_stErrorCnt[7]);
	DDX_Control(pDX, IDC_STC_ERROR_SOL_COUNT_09, m_stErrorCnt[8]);
	DDX_Control(pDX, IDC_STC_ERROR_SOL_COUNT_10, m_stErrorCnt[9]);

	DDX_Control(pDX, IDC_STC_ERROR_SOL_ERRNUM_01, m_StErrorNumber[0]);
	DDX_Control(pDX, IDC_STC_ERROR_SOL_ERRNUM_02, m_StErrorNumber[1]);
	DDX_Control(pDX, IDC_STC_ERROR_SOL_ERRNUM_03, m_StErrorNumber[2]);
	DDX_Control(pDX, IDC_STC_ERROR_SOL_ERRNUM_04, m_StErrorNumber[3]);
	DDX_Control(pDX, IDC_STC_ERROR_SOL_ERRNUM_05, m_StErrorNumber[4]);
	DDX_Control(pDX, IDC_STC_ERROR_SOL_ERRNUM_06, m_StErrorNumber[5]);
	DDX_Control(pDX, IDC_STC_ERROR_SOL_ERRNUM_07, m_StErrorNumber[6]);
	DDX_Control(pDX, IDC_STC_ERROR_SOL_ERRNUM_08, m_StErrorNumber[7]);
	DDX_Control(pDX, IDC_STC_ERROR_SOL_ERRNUM_09, m_StErrorNumber[8]);
	DDX_Control(pDX, IDC_STC_ERROR_SOL_ERRNUM_10, m_StErrorNumber[9]);

	DDX_Control(pDX, IDC_STC_ERROR_SOL_NAME_01, m_StErrorName[0]);
	DDX_Control(pDX, IDC_STC_ERROR_SOL_NAME_02, m_StErrorName[1]);
	DDX_Control(pDX, IDC_STC_ERROR_SOL_NAME_03, m_StErrorName[2]);
	DDX_Control(pDX, IDC_STC_ERROR_SOL_NAME_04, m_StErrorName[3]);
	DDX_Control(pDX, IDC_STC_ERROR_SOL_NAME_05, m_StErrorName[4]);
	DDX_Control(pDX, IDC_STC_ERROR_SOL_NAME_06, m_StErrorName[5]);
	DDX_Control(pDX, IDC_STC_ERROR_SOL_NAME_07, m_StErrorName[6]);
	DDX_Control(pDX, IDC_STC_ERROR_SOL_NAME_08, m_StErrorName[7]);
	DDX_Control(pDX, IDC_STC_ERROR_SOL_NAME_09, m_StErrorName[8]);
	DDX_Control(pDX, IDC_STC_ERROR_SOL_NAME_10, m_StErrorName[9]);

	DDX_Control(pDX, IDC_BTN_ERROR_SOL_SELECT_01, m_btnErrorSolution[0]);
	DDX_Control(pDX, IDC_BTN_ERROR_SOL_SELECT_02, m_btnErrorSolution[1]);
	DDX_Control(pDX, IDC_BTN_ERROR_SOL_SELECT_03, m_btnErrorSolution[2]);
	DDX_Control(pDX, IDC_BTN_ERROR_SOL_SELECT_04, m_btnErrorSolution[3]);
	DDX_Control(pDX, IDC_BTN_ERROR_SOL_SELECT_05, m_btnErrorSolution[4]);
	DDX_Control(pDX, IDC_BTN_ERROR_SOL_SELECT_06, m_btnErrorSolution[5]);
	DDX_Control(pDX, IDC_BTN_ERROR_SOL_SELECT_07, m_btnErrorSolution[6]);
	DDX_Control(pDX, IDC_BTN_ERROR_SOL_SELECT_08, m_btnErrorSolution[7]);
	DDX_Control(pDX, IDC_BTN_ERROR_SOL_SELECT_09, m_btnErrorSolution[8]);
	DDX_Control(pDX, IDC_BTN_ERROR_SOL_SELECT_10, m_btnErrorSolution[9]);
	
	
	DDX_Control(pDX, IDC_BTN_ERROR_SOL_PART_1, m_btnListPartSort[0]);
	DDX_Control(pDX, IDC_BTN_ERROR_SOL_PART_2, m_btnListPartSort[1]);
	DDX_Control(pDX, IDC_BTN_ERROR_SOL_PART_3, m_btnListPartSort[2]);

	DDX_Control(pDX, IDC_STC_ERROR_SOL_VIEW_1, m_stListPageMove[0]);
	DDX_Control(pDX, IDC_STC_ERROR_SOL_VIEW_2, m_stListPageMove[1]);

	DDX_Control(pDX, IDC_CB_DATE_SELECT_1, m_cbDate[0]);
	DDX_Control(pDX, IDC_CB_DATE_SELECT_2, m_cbDate[1]);
	DDX_Control(pDX, IDC_CB_DATE_SELECT_3, m_cbDate[2]);
	
	DDX_Control(pDX, IDC_LIST2, m_List_ErrorList_Past);
}


BEGIN_MESSAGE_MAP(CDlgErrorSolution, CDialogEx)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_ERROR_SOL_SELECT_01, &CDlgErrorSolution::OnbtnClickedSolution_0)
	ON_BN_CLICKED(IDC_BTN_ERROR_SOL_SELECT_02, &CDlgErrorSolution::OnbtnClickedSolution_1)
	ON_BN_CLICKED(IDC_BTN_ERROR_SOL_SELECT_03, &CDlgErrorSolution::OnbtnClickedSolution_2)
	ON_BN_CLICKED(IDC_BTN_ERROR_SOL_SELECT_04, &CDlgErrorSolution::OnbtnClickedSolution_3)
	ON_BN_CLICKED(IDC_BTN_ERROR_SOL_SELECT_05, &CDlgErrorSolution::OnbtnClickedSolution_4)
	ON_BN_CLICKED(IDC_BTN_ERROR_SOL_SELECT_06, &CDlgErrorSolution::OnbtnClickedSolution_5)
	ON_BN_CLICKED(IDC_BTN_ERROR_SOL_SELECT_07, &CDlgErrorSolution::OnbtnClickedSolution_6)
	ON_BN_CLICKED(IDC_BTN_ERROR_SOL_SELECT_08, &CDlgErrorSolution::OnbtnClickedSolution_7)
	ON_BN_CLICKED(IDC_BTN_ERROR_SOL_SELECT_09, &CDlgErrorSolution::OnbtnClickedSolution_8)
	ON_BN_CLICKED(IDC_BTN_ERROR_SOL_SELECT_10, &CDlgErrorSolution::OnbtnClickedSolution_9)

	ON_STN_CLICKED(IDC_STC_ERROR_SOL_VIEW_1, &CDlgErrorSolution::OnStnClickedStcErrorSolView1)
	ON_STN_CLICKED(IDC_STC_ERROR_SOL_VIEW_2, &CDlgErrorSolution::OnStnClickedStcErrorSolView2)
	ON_BN_CLICKED(IDC_BTN_ERROR_SOL_PART_1, &CDlgErrorSolution::OnBnClickedBtnErrorSolPart1)
	ON_BN_CLICKED(IDC_BTN_ERROR_SOL_PART_2, &CDlgErrorSolution::OnBnClickedBtnErrorSolPart2)
	ON_BN_CLICKED(IDC_BTN_ERROR_SOL_PART_3, &CDlgErrorSolution::OnBnClickedBtnErrorSolPart3)
END_MESSAGE_MAP()


// CDlgErrorSolution 메시지 처리기
BOOL CDlgErrorSolution::PreTranslateMessage(MSG* pMsg)
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

BOOL CDlgErrorSolution::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CRect rect;
	GetClientRect(rect);
	//pDC->FillSolidRect(rect, RGB(255, 255, 255));		// 
	pDC->FillSolidRect(rect, RGB(83, 169, 155));
	//pDC->FillSolidRect(rect, RGB(227, 255, 0));		// Lime
	return TRUE;
}

// CDlgIo 메시지 처리기
int CDlgErrorSolution::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.


	return 0;
}
void CDlgErrorSolution::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (bShow)
	{
		ShowErrorList();
		InitList();
	}
	else
	{
	}
}

BOOL CDlgErrorSolution::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitComponent_ErrSolution();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
void CDlgErrorSolution::InitList()
{
	int nCount = 0;
	CString strTemp = _T("");

	CRect rt;

	m_List_ErrorList_Past.GetClientRect(&rt);

	m_List_ErrorList_Past.InsertColumn(0, _T("Idx"), LVCFMT_CENTER, 50);
	m_List_ErrorList_Past.InsertColumn(1, _T("Num"), LVCFMT_LEFT, rt.Width() - 50 - 500);
	m_List_ErrorList_Past.InsertColumn(2, _T("Name"), LVCFMT_LEFT, rt.Width()- 15);
}

void CDlgErrorSolution::InitComponent_ErrSolution()
{
	CString strTemp = _T("");
	EQ_BASIC_PARAM	stEq = *DATAMANAGER->GetCurrentEqBasicData();
	ERROR_LIST		stErr = *DATAMANAGER->GetCurrentErrorList();

	for (int i = 0; i < 3; i++)
	{
		m_stErrorTitle[i].SetStaticStyle(CVGStatic::StaticStyle_GroupHeader);
		m_stErrorTitle[i].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_stErrorTitle[i].SetFont_Gdip(L"Arial", 9.0F);

		if (stEq.nLanguage == 0)
			m_stErrorTitle[i].SetText(m_szErrList_Title_Kor[i]);
		else if ( stEq.nLanguage == 1)
			m_stErrorTitle[i].SetText(m_szErrList_Title_Eng[i]);
	}

	for (int i = 0; i < 5; i++)
	{
		m_stErrorSubTitle[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stErrorSubTitle[i].SetColorStyle(CVGStatic::ColorStyle_Navy);
		m_stErrorSubTitle[i].SetFont_Gdip(L"Arial", 9.0F);

		if (stEq.nLanguage == 0)
			m_stErrorSubTitle[i].SetText(m_szErrList_TitleSub_Kor[i]);
		else if (stEq.nLanguage == 1)
			m_stErrorSubTitle[i].SetText(m_szErrList_TitleSub_Eng[i]);
	}

	for (int i = 0; i < 10; i++)
	{
		strTemp.Format(_T("%d"), i);
		m_stErrorCnt[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stErrorCnt[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_stErrorCnt[i].SetFont_Gdip(L"Arial", 9.0F);

		m_stErrorCnt[i].SetText(strTemp);
	}

	for (int i = 0; i < 10; i++)
	{
		if( stErr.nErrorCount > i )
			strTemp.Format(_T("%d"), stErr.vtErrorNum.at(i));
		else
			strTemp.Format(_T(""));

		m_StErrorNumber[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_StErrorNumber[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_StErrorNumber[i].SetFont_Gdip(L"Arial", 9.0F);

		m_StErrorNumber[i].SetText(strTemp);
	}

	for (int i = 0; i < 10; i++)
	{
		if (stErr.nErrorCount > i)
			strTemp.Format(_T("%d"), stErr.vtErrorNum.at(i));
		else
			strTemp.Format(_T(""));

		m_StErrorName[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_StErrorName[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_StErrorName[i].SetFont_Gdip(L"Arial", 9.0F);

		m_StErrorName[i].SetText(strTemp);
	}
	
	for (int i = 0; i < 10; i++)
	{
		m_btnErrorSolution[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_btnErrorSolution[i].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_btnErrorSolution[i].SetFont_Gdip(L"Arial", 9.0F);
		m_btnErrorSolution[i].SetText(_T("SOLUTION"));
	}

	for (int i = 0; i < 2; i++)
	{
		m_stListPageMove[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stListPageMove[i].SetColorStyle(CVGStatic::ColorStyle_Orange);
		m_stListPageMove[i].SetFont_Gdip(L"Arial", 9.0F);

		if( i == 0)
			m_stListPageMove[i].SetText(_T("Before"));
		else
			m_stListPageMove[i].SetText(_T("Next"));
	}

	for (int i = 0; i < 3; i++)
	{
		m_btnListPartSort[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_btnListPartSort[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_btnListPartSort[i].SetFont_Gdip(L"Arial", 9.0F);

		if( i == 0)
			m_btnListPartSort[i].SetText(_T("Convayor Part"));
		else if ( i == 1)
			m_btnListPartSort[i].SetText(_T("Socket Part"));
		else if (i == 2)
			m_btnListPartSort[i].SetText(_T("Communication Part"));
	}

	CTime t = CTime::GetCurrentTime();
	int nYear = t.GetYear();
	int nMonth = t.GetMonth();
	int nDay = t.GetDay();

	int nTemp = 2021;
	int index = 0;

	for (int i = 0; i < 10; i++)
	{
		strTemp.Format(_T("%d"), nYear+i);
		index = m_cbDate[0].AddString(strTemp);
		m_cbDate[0].SetItemData(index, i);
	}

	for (int i = 0; i < 12; i++)
	{
		strTemp.Format(_T("%d"), i+1);
		index = m_cbDate[1].AddString(strTemp);

		m_cbDate[1].SetItemData(index, i);
	}

	for (int i = 0; i < 31; i++)
	{
		strTemp.Format(_T("%d"), i + 1);
		index = m_cbDate[2].AddString(strTemp);

		m_cbDate[2].SetItemData(index, i);
	}

	SetComboBox_Date(&m_cbDate[0], nYear-nTemp);
	SetComboBox_Date(&m_cbDate[1], nMonth);
	SetComboBox_Date(&m_cbDate[2], nDay);
}

void CDlgErrorSolution::SetComboBox_Date(CComboBox *pBox, int Data)
{
	int count;
	count = pBox->GetCount();
	for (int i = 0; i < count; i++)
	{
		if (Data == pBox->GetItemData(i))
		{
			pBox->SetCurSel(i);
			break;
		}
	}
}

void CDlgErrorSolution::ChangeUI_Language()
{
	ShowErrorList();
}

void CDlgErrorSolution::ShowErrorList()
{
	ERROR_LIST		stError = *DATAMANAGER->GetCurrentErrorList();
	EQ_BASIC_PARAM	stEq = *DATAMANAGER->GetCurrentEqBasicData();

	// Error Dlg 화면이 켜지면 무조건 맨 처음에러리스트로 돌아온다.
	m_nCurrentPageCount = 0;

	int nErrCount = stError.nErrorCount;

	// 에러가 0 개면 화면에 뿌릴게 없다..
	if (nErrCount == 0)
		return;

	// 맨처음화면인 관계로 맨앞 10개의 에러만 꺼내오자.
	CString strTemp = _T("");
	for (int i = 0; i < 10; i++)
	{
		if (stError.nErrorCount <= i)
			continue;

		strTemp = stError.vtErrorNum.at(i);			// 여기서는 에러번호를 가져온다.
		m_StErrorNumber[i].SetText(strTemp);
	}

	// 에러번호를 모두 가져왔으면 에러명도 적어주자.
	int nErrNum = 0;
	strTemp = _T("");
	for (int i = 0; i < 10; i++)
	{
		m_StErrorNumber[i].GetText(strTemp);

		if (strTemp == _T(""))
			return;

		nErrNum = _ttoi(strTemp);

		if (nErrNum < 1000)
			return;

		// 나머지 숫자만큼이 벡터의 위치다...
		nErrNum = nErrNum % 1000;

		if (stEq.nLanguage == 0)
		{
			strTemp = stError.vtErr_Kor.at(nErrNum);
			m_StErrorName[i].SetText(strTemp);
		}
		else if (stEq.nLanguage == 1)
		{
			strTemp = stError.vtErr_Eng.at(nErrNum);
			m_StErrorName[i].SetText(strTemp);
		}
		else if (stEq.nLanguage == 2)
		{
			strTemp = stError.vtErr_Viet.at(nErrNum);
			m_StErrorName[i].SetText(strTemp);
		}
	}
}

void CDlgErrorSolution::ChangeUi_Solution(int nErrNum)
{
	//ERROR_LIST		stError = *DATAMANAGER->GetCurrentErrorList();
	//EQ_BASIC_PARAM	stEq = *DATAMANAGER->GetCurrentEqBasicData();

	//int nPartNum = 0;
	//int nSubNum = 0;
	//int nPickNum = 0;
	//CString strTemp = _T("");

	//nPartNum = nErrNum / 1000;
	//nSubNum = nErrNum % 1000;

	//if (nPartNum == 1)
	//	nPartNum = nSubNum;
	//else if (nPartNum == 2)
	//	nPartNum = stError.nErrorNameCount_1000 + nSubNum;
	//else if ( nPartNum == 3 )
	//	nPartNum = stError.nErrorNameCount_1000 + stError.nErrorNameCount_2000 + nSubNum;



	//// 출력
	//strTemp.Format(_T("%d"), nErrNum);

	//m_stErrorSubTitle[3].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
	//m_stErrorSubTitle[3].SetColorStyle(CVGStatic::ColorStyle_Yellow);
	//m_stErrorSubTitle[3].SetFont_Gdip(L"Arial", 9.0F);
	//m_stErrorSubTitle[3].SetText(strTemp);

	//m_stErrorSubTitle[4].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
	//m_stErrorSubTitle[4].SetColorStyle(CVGStatic::ColorStyle_Yellow);
	//m_stErrorSubTitle[4].SetFont_Gdip(L"Arial", 9.0F);

	//if (stEq.nLanguage == 0)
	//{
	//	strTemp = stError.vtSol_Kor.at(nPartNum);
	//	m_stErrorSubTitle[4].SetText(strTemp);
	//}
	//else if (stEq.nLanguage == 1)
	//{
	//	strTemp = stError.vtSol_Eng.at(nPartNum);
	//	m_stErrorSubTitle[4].SetText(strTemp);
	//}
	//else if (stEq.nLanguage == 2)
	//{
	//	strTemp = stError.vtSol_Viet.at(nPartNum);
	//	m_stErrorSubTitle[4].SetText(strTemp);
	//}
}

void CDlgErrorSolution::ChangeUi_Solution_BtnUi(int nBtnNum)
{
	if (nBtnNum == 100)
	{
		for (int i = 0; i < 10; i++)
		{
			m_bSolutionBtnFlag[i] = FALSE;

			m_btnErrorSolution[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
			m_btnErrorSolution[i].SetColorStyle(CVGStatic::ColorStyle_Black);
			m_btnErrorSolution[i].SetFont_Gdip(L"Arial", 9.0F);
			m_btnErrorSolution[i].SetText(_T("SOLUTION"));
		}

	}
	else
	{
		for (int i = 0; i < 10; i++)
		{
			if (i == nBtnNum)
			{
				if (m_bSolutionBtnFlag[nBtnNum] == FALSE)
				{
					m_bSolutionBtnFlag[nBtnNum] = TRUE;

					m_btnErrorSolution[nBtnNum].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
					m_btnErrorSolution[nBtnNum].SetColorStyle(CVGStatic::ColorStyle_Green);
					m_btnErrorSolution[nBtnNum].SetFont_Gdip(L"Arial", 9.0F);
					m_btnErrorSolution[nBtnNum].SetText(_T("ON"));
				}
				else
				{
					m_bSolutionBtnFlag[nBtnNum] = FALSE;

					m_btnErrorSolution[nBtnNum].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
					m_btnErrorSolution[nBtnNum].SetColorStyle(CVGStatic::ColorStyle_Black);
					m_btnErrorSolution[nBtnNum].SetFont_Gdip(L"Arial", 9.0F);
					m_btnErrorSolution[nBtnNum].SetText(_T("SOLUTION"));
				}

			}
			else
			{
				m_bSolutionBtnFlag[i] = FALSE;

				m_btnErrorSolution[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
				m_btnErrorSolution[i].SetColorStyle(CVGStatic::ColorStyle_Black);
				m_btnErrorSolution[i].SetFont_Gdip(L"Arial", 9.0F);
				m_btnErrorSolution[i].SetText(_T("SOLUTION"));
			}
		}
	}
}
void CDlgErrorSolution::OnbtnClickedSolution_0()
{
	CString strTemp = _T("");
	int nTemp = 0;
	
	m_StErrorNumber[0].GetText(strTemp);

	if (strTemp == _T(""))
	{
		COMMON->ShowMsgDlg_Domodal(_T("잘못 선택하셨습니다. 에러번호가 있는 버튼을 눌러주세요."), MODE_INFORMATION);
		return;
	}

	nTemp = _ttoi(strTemp);
	ChangeUi_Solution(nTemp);

	ChangeUi_Solution_BtnUi(0);
	
}
void CDlgErrorSolution::OnbtnClickedSolution_1()
{
	CString strTemp = _T("");
	int nTemp = 0;

	m_StErrorNumber[1].GetText(strTemp);

	if (strTemp == _T(""))
	{
		COMMON->ShowMsgDlg_Domodal(_T("잘못 선택하셨습니다. 에러번호가 있는 버튼을 눌러주세요."), MODE_INFORMATION);
		return;
	}

	nTemp = _ttoi(strTemp);
	ChangeUi_Solution(nTemp);
	ChangeUi_Solution_BtnUi(1);
}
void CDlgErrorSolution::OnbtnClickedSolution_2()
{
	CString strTemp = _T("");
	int nTemp = 0;

	m_StErrorNumber[2].GetText(strTemp);

	if (strTemp == _T(""))
	{
		COMMON->ShowMsgDlg_Domodal(_T("잘못 선택하셨습니다. 에러번호가 있는 버튼을 눌러주세요."), MODE_INFORMATION);
		return;
	}

	nTemp = _ttoi(strTemp);
	ChangeUi_Solution(nTemp);
	ChangeUi_Solution_BtnUi(2);
}
void CDlgErrorSolution::OnbtnClickedSolution_3()
{
	CString strTemp = _T("");
	int nTemp = 0;

	m_StErrorNumber[3].GetText(strTemp);

	if (strTemp == _T(""))
	{
		COMMON->ShowMsgDlg_Domodal(_T("잘못 선택하셨습니다. 에러번호가 있는 버튼을 눌러주세요."), MODE_INFORMATION);
		return;
	}

	nTemp = _ttoi(strTemp);
	ChangeUi_Solution(nTemp);
	ChangeUi_Solution_BtnUi(3);
}
void CDlgErrorSolution::OnbtnClickedSolution_4()
{
	CString strTemp = _T("");
	int nTemp = 0;

	m_StErrorNumber[4].GetText(strTemp);

	if (strTemp == _T(""))
	{
		COMMON->ShowMsgDlg_Domodal(_T("잘못 선택하셨습니다. 에러번호가 있는 버튼을 눌러주세요."), MODE_INFORMATION);
		return;
	}

	nTemp = _ttoi(strTemp);
	ChangeUi_Solution(nTemp);
	ChangeUi_Solution_BtnUi(4);
}
void CDlgErrorSolution::OnbtnClickedSolution_5()
{
	CString strTemp = _T("");
	int nTemp = 0;

	m_StErrorNumber[5].GetText(strTemp);

	if (strTemp == _T(""))
	{
		COMMON->ShowMsgDlg_Domodal(_T("잘못 선택하셨습니다. 에러번호가 있는 버튼을 눌러주세요."), MODE_INFORMATION);
		return;
	}

	nTemp = _ttoi(strTemp);
	ChangeUi_Solution(nTemp);
	ChangeUi_Solution_BtnUi(5);
}
void CDlgErrorSolution::OnbtnClickedSolution_6()
{
	CString strTemp = _T("");
	int nTemp = 0;

	m_StErrorNumber[6].GetText(strTemp);

	if (strTemp == _T(""))
	{
		COMMON->ShowMsgDlg_Domodal(_T("잘못 선택하셨습니다. 에러번호가 있는 버튼을 눌러주세요."), MODE_INFORMATION);
		return;
	}

	nTemp = _ttoi(strTemp);
	ChangeUi_Solution(nTemp);
	ChangeUi_Solution_BtnUi(6);
}
void CDlgErrorSolution::OnbtnClickedSolution_7()
{
		CString strTemp = _T("");
	int nTemp = 0;
	
	m_StErrorNumber[7].GetText(strTemp);

	if (strTemp == _T(""))
	{
		COMMON->ShowMsgDlg_Domodal(_T("잘못 선택하셨습니다. 에러번호가 있는 버튼을 눌러주세요."), MODE_INFORMATION);
		return;
	}

	nTemp = _ttoi(strTemp);
	ChangeUi_Solution(nTemp);
	ChangeUi_Solution_BtnUi(7);
}
void CDlgErrorSolution::OnbtnClickedSolution_8()
{
	CString strTemp = _T("");
	int nTemp = 0;

	m_StErrorNumber[8].GetText(strTemp);

	if (strTemp == _T(""))
	{
		COMMON->ShowMsgDlg_Domodal(_T("잘못 선택하셨습니다. 에러번호가 있는 버튼을 눌러주세요."), MODE_INFORMATION);
		return;
	}

	nTemp = _ttoi(strTemp);
	ChangeUi_Solution(nTemp);
	ChangeUi_Solution_BtnUi(8);
}
void CDlgErrorSolution::OnbtnClickedSolution_9()
{
	CString strTemp = _T("");
	int nTemp = 0;

	m_StErrorNumber[9].GetText(strTemp);

	if (strTemp == _T(""))
	{
		COMMON->ShowMsgDlg_Domodal(_T("잘못 선택하셨습니다. 에러번호가 있는 버튼을 눌러주세요."), MODE_INFORMATION);
		return;
	}

	nTemp = _ttoi(strTemp);
	ChangeUi_Solution(nTemp);
	ChangeUi_Solution_BtnUi(9);
}

void CDlgErrorSolution::OnStnClickedStcErrorSolView1()
{
	//ERROR_LIST		stError = *DATAMANAGER->GetCurrentErrorList();
	//EQ_BASIC_PARAM	stEq = *DATAMANAGER->GetCurrentEqBasicData();

	//if (m_nCurrentPageCount < 1)
	//	return;

	//if( stError.nErrorCount > 10 )
	//	m_nCurrentPageCount--;

	//ChangeUi_Solution_BtnUi(100);			// 100은 FLAG 전체 FLASE 만들기 ( 버튼)

	//int nStart = 0;
	//int nEnd = 0;

	//nStart	= m_nCurrentPageCount * 10;
	//nEnd	= (m_nCurrentPageCount + 1) * 10;

	//// 맨처음화면인 관계로 맨앞 10개의 에러만 꺼내오자.
	//CString strTemp = _T("");
	//int nStep = 0;

	//for (int i = nStart; i < nEnd; i++)
	//{
	//	if (stError.nErrorCount <= i)
	//	{
	//		m_StErrorNumber[nStep].SetText(_T(""));
	//		m_StErrorName[nStep].SetText(_T(""));
	//		nStep++;
	//		continue;
	//	}

	//	strTemp = stError.vtErrorNum.at(i);			// 여기서는 에러번호를 가져온다.
	//	m_StErrorNumber[nStep].SetText(strTemp);
	//	nStep++;
	//}

	//// 에러번호를 모두 가져왔으면 에러명도 적어주자.
	//int nErrNum = 0;
	//strTemp = _T("");

	//int nPartNum = 0;
	//int nSubNum = 0;
	//int nPickNum = 0;

	//for (int i = 0; i < 10; i++)
	//{
	//	m_StErrorNumber[i].GetText(strTemp);

	//	if (strTemp == _T(""))
	//		return;

	//	nErrNum = _ttoi(strTemp);

	//	if (nErrNum < 1000)
	//		return;

	//	//에러번호에 따른 vector 번호 찾기
	//	nPartNum = nErrNum / 1000;
	//	nSubNum = nErrNum % 1000;

	//	if (nPartNum == 1)
	//		nPartNum = nSubNum;
	//	else if (nPartNum == 2)
	//		nPartNum = stError.nErrorNameCount_1000 + nSubNum;
	//	else if (nPartNum == 3)
	//		nPartNum = stError.nErrorNameCount_1000 + stError.nErrorNameCount_2000 + nSubNum;


	//	if (stEq.nLanguage == 0)
	//	{
	//		strTemp = stError.vtErr_Kor.at(nPartNum);
	//		m_StErrorName[i].SetText(strTemp);
	//	}
	//	else if (stEq.nLanguage == 1)
	//	{
	//		strTemp = stError.vtErr_Eng.at(nPartNum);
	//		m_StErrorName[i].SetText(strTemp);
	//	}
	//	else if (stEq.nLanguage == 2)
	//	{
	//		strTemp = stError.vtErr_Viet.at(nPartNum);
	//		m_StErrorName[i].SetText(strTemp);
	//	}
	//}
}


void CDlgErrorSolution::OnStnClickedStcErrorSolView2()
{
	//ERROR_LIST		stError = *DATAMANAGER->GetCurrentErrorList();
	//EQ_BASIC_PARAM	stEq = *DATAMANAGER->GetCurrentEqBasicData();

	//if ( (stError.nErrorCount/10) <=  m_nCurrentPageCount )
	//	return;
	//
	//m_nCurrentPageCount++;
	//ChangeUi_Solution_BtnUi(100);			// 100은 FLAG 전체 FLASE 만들기 ( 버튼)

	//int nStart = 0;
	//int nEnd = 0;

	//nStart = m_nCurrentPageCount * 10;
	//nEnd = (m_nCurrentPageCount + 1) * 10;

	//// 맨처음화면인 관계로 맨앞 10개의 에러만 꺼내오자.
	//CString strTemp = _T("");
	//int nStep = 0;
	//for (int i = nStart; i < nEnd; i++)
	//{
	//	if (stError.nErrorCount <= i)
	//	{
	//		m_StErrorNumber[nStep].SetText(_T(""));
	//		m_StErrorName[nStep].SetText(_T(""));
	//		nStep++;
	//		continue;
	//	}			

	//	strTemp = stError.vtErrorNum.at(i);			// 여기서는 에러번호를 가져온다.
	//	m_StErrorNumber[nStep].SetText(strTemp);
	//	nStep++;
	//}

	//// 에러번호를 모두 가져왔으면 에러명도 적어주자.
	//int nErrNum = 0;
	//strTemp = _T("");

	//int nPartNum = 0;
	//int nSubNum = 0;
	//int nPickNum = 0;

	//for (int i = 0; i < 10; i++)
	//{
	//	m_StErrorNumber[i].GetText(strTemp);

	//	if (strTemp == _T(""))
	//		return;

	//	nErrNum = _ttoi(strTemp);

	//	if (nErrNum < 1000)
	//		return;

	//	//에러번호에 따른 vector 번호 찾기
	//	nPartNum = nErrNum / 1000;
	//	nSubNum = nErrNum % 1000;

	//	if (nPartNum == 1)
	//		nPartNum = nSubNum;
	//	else if (nPartNum == 2)
	//		nPartNum = stError.nErrorNameCount_1000 + nSubNum;
	//	else if (nPartNum == 3)
	//		nPartNum = stError.nErrorNameCount_1000 + stError.nErrorNameCount_2000 + nSubNum;


	//	if (stEq.nLanguage == 0)
	//	{
	//		strTemp = stError.vtErr_Kor.at(nPartNum);
	//		m_StErrorName[i].SetText(strTemp);
	//	}
	//	else if (stEq.nLanguage == 1)
	//	{
	//		strTemp = stError.vtErr_Eng.at(nPartNum);
	//		m_StErrorName[i].SetText(strTemp);
	//	}
	//	else if (stEq.nLanguage == 2)
	//	{
	//		strTemp = stError.vtErr_Viet.at(nPartNum);
	//		m_StErrorName[i].SetText(strTemp);
	//	}
	//}
}


void CDlgErrorSolution::OnBnClickedBtnErrorSolPart1()
{
	ERROR_LIST		stError = *DATAMANAGER->GetCurrentErrorList();
	EQ_BASIC_PARAM	stEq = *DATAMANAGER->GetCurrentEqBasicData();
	
	if (stError.nErrorCount < 1)
		return;

	m_nCurrentPageCount = 0;

	__int64 nVectorCnt = stError.vtErrorNum.size();

	CString strTemp = _T("");
	for (int i = 0; i < nVectorCnt; i++)
	{
		strTemp = stError.vtErrorNum.at(i);
		if (_ttoi(strTemp) >= 1000 && _ttoi(strTemp) < 2000)
		{
			m_nPartErrorNum_1[m_nPartErrorCnt_1] = _ttoi(strTemp);
			m_nPartErrorCnt_1++;
		}
	}


	int nStart = 0;
	int nEnd = 0;

	nStart = m_nCurrentPageCount * 10;
	nEnd = (m_nCurrentPageCount + 1) * 10;

	// 에러번호
	int nStep = 0;
	for (int i = nStart; i < nEnd; i++)
	{
		if (stError.nErrorCount <= i)
		{
			m_StErrorNumber[nStep].SetText(_T(""));
			m_StErrorName[nStep].SetText(_T(""));
			nStep++;
			continue;
		}

		strTemp.Format(_T("%d"), m_nPartErrorNum_1[i]);		// 여기서는 에러번호를 가져온다.
		m_StErrorNumber[nStep].SetText(strTemp);
		nStep++;
	}

	// 에러명
	int nErrNum = 0;
	strTemp = _T("");

	int nSubNum = 0;

	for (int i = 0; i < 10; i++)
	{
		m_StErrorNumber[i].GetText(strTemp);

		if (strTemp == _T(""))
			return;

		nErrNum = _ttoi(strTemp);

		if (nErrNum < 1000)
			return;

		//에러번호에 따른 vector 번호 찾기
		nSubNum = nErrNum % 1000;

		if (stEq.nLanguage == 0)
		{
			strTemp = stError.vtErr_Kor.at(nSubNum);
			m_StErrorName[i].SetText(strTemp);
		}
		else if (stEq.nLanguage == 1)
		{
			strTemp = stError.vtErr_Eng.at(nSubNum);
			m_StErrorName[i].SetText(strTemp);
		}
		else if (stEq.nLanguage == 2)
		{
			strTemp = stError.vtErr_Viet.at(nSubNum);
			m_StErrorName[i].SetText(strTemp);
		}
	}
}


void CDlgErrorSolution::OnBnClickedBtnErrorSolPart2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CDlgErrorSolution::OnBnClickedBtnErrorSolPart3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
