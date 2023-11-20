// DlgSaveAsModel.cpp: 구현 파일
//

#include "pch.h"
#include "LuriMotion_OQA_SFR_Wide.h"
#include "DlgSaveAsModel.h"
#include "afxdialogex.h"


// CDlgSaveAsModel 대화 상자

IMPLEMENT_DYNAMIC(CDlgSaveAsModel, CDialogEx)

CDlgSaveAsModel::CDlgSaveAsModel(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_SAVEAS_MODEL, pParent)
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
}

CDlgSaveAsModel::~CDlgSaveAsModel()
{
}

void CDlgSaveAsModel::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STC_SAVEAS_TITLE, m_stTitle);

	DDX_Control(pDX, IDC_EDIT_SAVEAS_MODEL_NAME, m_edModelName);
	DDX_Text(pDX, IDC_EDIT_SAVEAS_MODEL_NAME, m_strModelName);

	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}


BEGIN_MESSAGE_MAP(CDlgSaveAsModel, CDialogEx)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDOK, &CDlgSaveAsModel::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgSaveAsModel 메시지 처리기
BOOL CDlgSaveAsModel::PreTranslateMessage(MSG* pMsg)
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
int CDlgSaveAsModel::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	//InitComponent_MessageBox();

	return 0;
}

BOOL CDlgSaveAsModel::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CRect rect;
	GetClientRect(rect);
	//pDC->FillSolidRect(rect, RGB(105, 186, 235));
	pDC->FillSolidRect(rect, RGB(83, 169, 155));
	//pDC->FillSolidRect(rect, RGB(227, 255, 0));		// Lime
	return TRUE;
}
BOOL CDlgSaveAsModel::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
void CDlgSaveAsModel::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (bShow)
	{
		InitComponnet_SaveAs();
		InitComponent_SaveAs_EditBox();
	}
}
void CDlgSaveAsModel::InitComponnet_SaveAs()
{
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();

	m_stTitle.SetStaticStyle(CVGStatic::StaticStyle_GroupHeader);
	m_stTitle.SetColorStyle(CVGStatic::ColorStyle_Black);
	m_stTitle.SetFont_Gdip(L"Arial", 12.0F);

	if( stEq.nLanguage == 0 )
		m_stTitle.SetText(_T("새로운 모델 이름을 입력하세요"));
	else
		m_stTitle.SetText(_T("Input Model Name (SaveAs)"));
	 
	m_edModelName.SetFont(&m_font_Default);

	m_btnOk.SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
	m_btnOk.SetColorStyle(CVGStatic::ColorStyle_Black);
	m_btnOk.SetFont_Gdip(L"Arial", 12.0F);
	m_btnOk.SetText(_T("OK"));

	m_btnCancel.SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
	m_btnCancel.SetColorStyle(CVGStatic::ColorStyle_Black);
	m_btnCancel.SetFont_Gdip(L"Arial", 12.0F);
	m_btnCancel.SetText(_T("Cancel"));
	
}

void CDlgSaveAsModel::InitComponent_SaveAs_EditBox()
{
	CClientDC dc(GetDlgItem(IDC_EDIT_SAVEAS_MODEL_NAME));
	CRect rt;

	GetDlgItem(IDC_EDIT_SAVEAS_MODEL_NAME)->GetClientRect(&rt);

	rt.left += 5;
	rt.right -= 5;

	rt.top += 10;
	rt.bottom -= 10;

	((CEdit*)GetDlgItem(IDC_EDIT_SAVEAS_MODEL_NAME))->SetRect(&rt);
	UpdateData(FALSE);
}

/*void CDlgSaveAsModel::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strTemp = _T("");
	GetDlgItemText(IDC_EDIT_SAVEAS_MODEL_NAME, strTemp);
	
	m_btnOk.SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
	m_btnOk.SetColorStyle(CVGStatic::ColorStyle_Green);
	m_btnOk.SetFont_Gdip(L"Arial", 12.0F);
	m_btnOk.SetText(_T("OK"));

	if (strTemp == _T(""))
	{
		COMMON->ShowMsgDlg_Domodal(_T("Empty Model Name."), MODE_INFORMATION);

		m_btnOk.SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_btnOk.SetColorStyle(CVGStatic::ColorStyle_Black);
		m_btnOk.SetFont_Gdip(L"Arial", 12.0F);
		m_btnOk.SetText(_T("OK"));

		return;
	}

	// 실제 저장하는 루트
	int nData = 0;
	int nIndex = 0;
	int nLength = 0;

	double dData = 0;

	CString strModelName = _T("");
	CString strSavePath = _T("");
	CString strFileDlgOpenPath = _T("");
	CString strFileCopyPath = _T("");
	CString strFileOriginPath = _T("");
	CString strSection = _T("");
	CString strKey = _T("");
	CString strData = _T("");
	CString strLog = _T("");
	CString strPath = _T("");

	// 폴더를 만들자.
	strFileDlgOpenPath.Format(_T("%s\\%s"), _PROGRAM_RECIPE_DIR, strTemp);
	CreateDirectory(strFileDlgOpenPath, NULL);

	CString strTestName = _T("");
	
	// 만든 폴더로 호출하자.
	CFileDialog fileDlg(FALSE, L"*.Mod", LPCTSTR(strTemp), OFN_READONLY,
		_T("Model FIle(*.Mod)|*.Mod| All File (*.*)|*.*|"));

	fileDlg.m_ofn.lpstrInitialDir = strFileDlgOpenPath;

	if (fileDlg.DoModal() == IDOK)
	{
		//Model file 경로
		strPath = fileDlg.GetPathName();

		nLength = strPath.GetLength();
		strPath.Delete(nLength - 4, 4);

		nLength = strPath.GetLength();
		nIndex = strPath.ReverseFind('\\');
		strModelName = strPath.Mid(nIndex + 1, nLength - nIndex - 1);

		strFileOriginPath.Format(_T("%s\\%s.Mod"), strFileDlgOpenPath, strTemp);
		strFileCopyPath.Format(_T("%s\\%s.Mod"), strFileDlgOpenPath, strTemp);

		if (CopyFile(strFileOriginPath, strPath, TRUE) == TRUE)
		{
			//strLog.Format(_T("[SAVE_AS] 동일한 모델 파일이 있습니다. [ Model Name : %s ]\n"), strModelName);
			COMMON->ShowMsgDlg_Domodal(_T("Same Recipe Name. Change Recipe Name"), MODE_INFORMATION);
			return;
		}
		else
		{
			TEACHING_PARAM	stTeach = *DATAMANAGER->GetCurrentTeachingData();
			MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();
			TESTER_COMM		stTester = *DATAMANAGER->GetCurrentTesterCommData();

			CString strTeach = _T("");
			CString strMotion = _T("");
			CString strTester = _T("");

			strTeach.Format(_T("%s\\%s.Mod"), strFileDlgOpenPath, strTemp);
			strMotion.Format(_T("%s\\MotionParam.ini"), strFileDlgOpenPath);
			strTester.Format(_T("%s\\TesterComm.ini"), strFileDlgOpenPath);

			DATAMANAGER->SetSaveAs_TeachingData(stTeach, strTeach);
			DATAMANAGER->SetSaveAs_MotionParam(stMotion, strMotion);
			DATAMANAGER->SetSaveAs_TesterCommParam(stTester, strTester);
		}


	}

	CDialogEx::OnOK();
}
*/
void CDlgSaveAsModel::OnBnClickedOk()
{
	// 실제 저장하는 루트
	int nData = 0;
	int nIndex = 0;
	int nLength = 0;

	double dData = 0;

	CString strModelName = _T("");
	CString strSavePath = _T("");
	CString strFileDlgOpenPath = _T("");
	CString strFileCopyPath = _T("");
	CString strFileOriginPath = _T("");
	CString strSection = _T("");
	CString strKey = _T("");
	CString strData = _T("");
	CString strLog = _T("");
	CString strPath = _T("");
	CString strTemp = _T("");

	strFileDlgOpenPath.Format(_T("%s\\%s"), _PROGRAM_RECIPE_DIR, strTemp);

	CFileDialog fileDlg(FALSE, L"*.Mod", L"*.Mod", OFN_READONLY,
		_T("Model FIle(*.Mod)|*.Mod| All File (*.*)|*.*|"));

	fileDlg.m_ofn.lpstrInitialDir = strFileDlgOpenPath;

	if (fileDlg.DoModal() == IDOK)
	{
		//Model file 경로
		strPath = fileDlg.GetPathName();

		nLength = strPath.GetLength();
		strPath.Delete(nLength - 4, 4);

		nLength = strPath.GetLength();
		nIndex = strPath.ReverseFind('\\');
		strModelName = strPath.Mid(nIndex + 1, nLength - nIndex - 1);

		strFileOriginPath.Format(_T("%s\\%s.Mod"), strFileDlgOpenPath, strModelName);
		strFileCopyPath.Format(_T("%s\\%s.Mod"), strFileDlgOpenPath, strModelName);

		if (CopyFile(strFileOriginPath, strPath, TRUE) == TRUE)
		{
			//strLog.Format(_T("[SAVE_AS] 동일한 모델 파일이 있습니다. [ Model Name : %s ]\n"), strModelName);
			COMMON->ShowMsgDlg_Domodal(_T("Same Recipe Name. Change Recipe Name"), MODE_INFORMATION);
			return;
		}
		else
		{
			TEACHING_PARAM	stTeach = *DATAMANAGER->GetCurrentTeachingData();
			MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();
			TESTER_COMM		stTester = *DATAMANAGER->GetCurrentTesterCommData();

			CString strTeach = _T("");
			CString strMotion = _T("");
			CString strTester = _T("");

			strTeach.Format(_T("%s%s\\%s.Mod"), strFileDlgOpenPath, strModelName, strModelName);
			strMotion.Format(_T("%s%s\\MotionParam.ini"), strFileDlgOpenPath, strModelName);
			strTester.Format(_T("%s%s\\TesterComm.ini"), strFileDlgOpenPath, strModelName);

			DATAMANAGER->SetSaveAs_TeachingData(stTeach, strTeach);
			DATAMANAGER->SetSaveAs_MotionParam(stMotion, strMotion);
			DATAMANAGER->SetSaveAs_TesterCommParam(stTester, strTester);

			COMMON->ShowMsgDlg_Domodal(_T("Recipe Save Compleate."), MODE_INFORMATION);
		}
	}

	CDialogEx::OnOK();
}
