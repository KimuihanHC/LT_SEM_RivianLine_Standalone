// DlgAdmin.cpp: 구현 파일
//

#include "pch.h"
#include "LuriMotion_OQA_SFR_Wide.h"
#include "DlgAdmin.h"
#include "afxdialogex.h"

#define  MSK_OPER1		0x01
#define  MSK_MAINT1		0x02
#define  MSK_ADMIN1		0x04
#define  MSK_OPER2		0x08
#define  MSK_MAINT2		0x10
#define  MSK_ADMIN2		0x20


WCHAR *lvlList[] = {_T("OPERATOR"),_T("MAINTENANCE"),_T("ADMINISTRATOR") };
LVITEM		lvItem;
// CDlgAdmin 대화 상자

IMPLEMENT_DYNAMIC(CDlgAdmin, CDialogEx)

CDlgAdmin::CDlgAdmin(int nMode, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_ADMIN, pParent)
{
	m_nSelectAcessMode = nMode;

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

	VERIFY(m_font_List.CreateFont(
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
		_T("Cambria")));			// lpszFacename

	m_nSavePermission = -1;
}

CDlgAdmin::~CDlgAdmin()
{
}

void CDlgAdmin::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STC_TITLE_LANG, m_stTitle);
	DDX_Control(pDX, IDC_STC_TITLE_LANG2, m_stTitle_Add);

	DDX_Control(pDX, IDC_BTN_MODE_1, m_btnUserMode[0]);
	DDX_Control(pDX, IDC_BTN_MODE_2, m_btnUserMode[1]);
	DDX_Control(pDX, IDC_BTN_MODE_3, m_btnUserMode[2]);
	DDX_Control(pDX, IDC_BTN_MODE_4, m_btnUserMode[3]);
	DDX_Control(pDX, IDC_BTN_MODE_5, m_btnUserMode[4]);
	DDX_Control(pDX, IDC_BTN_MODE_6, m_btnUserMode[5]);

	DDX_Control(pDX, IDC_STC_SUB_TITLE_1, m_stTitle_Sub[0]);
	DDX_Control(pDX, IDC_STC_SUB_TITLE_2, m_stTitle_Sub[1]);
	DDX_Control(pDX, IDC_STC_SUB_TITLE_3, m_stTitle_Sub[2]);
	DDX_Control(pDX, IDC_STC_SUB_TITLE_4, m_stTitle_Sub[3]);

	DDX_Control(pDX, IDC_EDIT_ADMIN_ID, m_edId);
	DDX_Control(pDX, IDC_EDIT_ADMIN_PW, m_edPw);

	DDX_Control(pDX, IDC_EDIT_ADMIN_INPUT_ID, m_edId_add);
	DDX_Control(pDX, IDC_EDIT_ADMIN_INPUT_PW, m_edPw_add);
	
	DDX_Control(pDX, IDC_BTN_OK1, m_btnChangeOK[0]);
	DDX_Control(pDX, IDC_BTN_OK2, m_btnChangeOK[1]);
	DDX_Control(pDX, IDC_BTN_OK3, m_btnChangeOK[2]);
	DDX_Control(pDX, IDC_BTN_OK4, m_btnChangeOK[3]);

	DDX_Control(pDX, IDC_LIST_VIEW, m_ListView);

}


BEGIN_MESSAGE_MAP(CDlgAdmin, CDialogEx)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BTN_MODE_1, &CDlgAdmin::OnBnClickedBtnMode1)
	ON_BN_CLICKED(IDC_BTN_MODE_2, &CDlgAdmin::OnBnClickedBtnMode2)
	ON_BN_CLICKED(IDC_BTN_MODE_3, &CDlgAdmin::OnBnClickedBtnMode3)
	ON_BN_CLICKED(IDC_BTN_MODE_4, &CDlgAdmin::OnBnClickedBtnMode4)
	ON_BN_CLICKED(IDC_BTN_MODE_5, &CDlgAdmin::OnBnClickedBtnMode5)
	ON_BN_CLICKED(IDC_BTN_MODE_6, &CDlgAdmin::OnBnClickedBtnMode6)
	ON_BN_CLICKED(IDC_BTN_OK1, &CDlgAdmin::OnBnClickedBtnOk1)
	ON_BN_CLICKED(IDC_BTN_OK2, &CDlgAdmin::OnBnClickedBtnOk2)
	ON_BN_CLICKED(IDC_BTN_OK3, &CDlgAdmin::OnBnClickedBtnOk3)
	ON_BN_CLICKED(IDC_BTN_OK4, &CDlgAdmin::OnBnClickedBtnOk4)
	ON_NOTIFY(NM_CLICK, IDC_LIST_VIEW, &CDlgAdmin::OnNMClickedListView)
END_MESSAGE_MAP()

// CDlgLanguage 메시지 처리기
BOOL CDlgAdmin::PreTranslateMessage(MSG* pMsg)
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
int CDlgAdmin::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	//InitComponent_MessageBox();

	return 0;
}

BOOL CDlgAdmin::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CRect rect;
	GetClientRect(rect);
	//pDC->FillSolidRect(rect, RGB(105, 186, 235));
	//pDC->FillSolidRect(rect, RGB(83, 169, 155));
	pDC->FillSolidRect(rect, RGB(112, 128, 144));		// Slate Gray
	return TRUE;
}
BOOL CDlgAdmin::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitView_List();
	int lvl = m_nSelectAcessMode;
	DisplayByLevel(lvl);
	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
void CDlgAdmin::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (bShow)
	{
		int lvl = m_nSelectAcessMode;
		InitComponnet_Admin(lvl);
		Disable_AddPermission(lvl);
	}
}


void CDlgAdmin::Disable_AddPermission(int nLevel)
{
	if (nLevel == LVL_OPER || nLevel == LVL_MAINT)
	{
		m_stTitle_Add.ShowWindow(SW_HIDE);
		m_btnUserMode[3].ShowWindow(SW_HIDE);
		m_btnUserMode[4].ShowWindow(SW_HIDE);
		m_btnUserMode[5].ShowWindow(SW_HIDE);
		m_stTitle_Sub[2].ShowWindow(SW_HIDE);
		m_stTitle_Sub[3].ShowWindow(SW_HIDE);

		m_edId_add.ShowWindow(SW_HIDE);
		m_edPw_add.ShowWindow(SW_HIDE);

		m_btnChangeOK[2].ShowWindow(SW_HIDE);
		m_btnChangeOK[3].ShowWindow(SW_HIDE);
	}
	else if (nLevel == LVL_ADMIN)
	{
		m_stTitle_Add.ShowWindow(SW_SHOW);
		m_btnUserMode[3].ShowWindow(SW_SHOW);
		m_btnUserMode[4].ShowWindow(SW_SHOW);
		m_btnUserMode[5].ShowWindow(SW_SHOW);
		m_stTitle_Sub[2].ShowWindow(SW_SHOW);
		m_stTitle_Sub[3].ShowWindow(SW_SHOW);

		m_edId_add.ShowWindow(SW_SHOW);
		m_edPw_add.ShowWindow(SW_SHOW);

		m_btnChangeOK[2].ShowWindow(SW_SHOW);
		m_btnChangeOK[3].ShowWindow(SW_SHOW);
	}
	
}

void CDlgAdmin::InitComponnet_Admin(int nLevel)
{
	EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();

	m_stTitle.SetStaticStyle(CVGStatic::StaticStyle_GroupHeader);
	m_stTitle.SetColorStyle(CVGStatic::ColorStyle_SlateGray);
	m_stTitle.SetFont_Gdip(L"Arial", 12.0F);
	if (stBasic.nLanguage == 0)
		m_stTitle.SetText(_T("권한 계정을 설정하세요."));
	else
		m_stTitle.SetText(_T("Select a permission account."));

	m_stTitle_Add.SetStaticStyle(CVGStatic::StaticStyle_GroupHeader);
	m_stTitle_Add.SetColorStyle(CVGStatic::ColorStyle_SlateGray);
	m_stTitle_Add.SetFont_Gdip(L"Arial", 12.0F);
	if (stBasic.nLanguage == 0)
		m_stTitle_Add.SetText(_T("추가할 권한 계정을 설정하세요"));
	else
		m_stTitle_Add.SetText(_T("Set up a permission account to add."));

	int nTemp = 0;
	// level 버튼
	switch (nLevel)
	{
	case LVL_OPER:
		DisplayBtnLevel(MSK_OPER1 | MSK_OPER2, stBasic.nLanguage);
		break;
	case LVL_MAINT:
		DisplayBtnLevel(MSK_MAINT1 | MSK_MAINT2, stBasic.nLanguage);
		break;
	case LVL_ADMIN:
		DisplayBtnLevel(MSK_ADMIN1 | MSK_ADMIN2, stBasic.nLanguage);
		break;
	}
	// Ok 버튼
	DisplayBtnOk(stBasic.nLanguage);

	// 타이틀
	DisplayTitle(stBasic.nLanguage);

	// 에딧트
	m_edId.SetFont(&m_font_Default);
	m_edPw.SetFont(&m_font_Default);
	m_edId_add.SetFont(&m_font_Default);
	m_edPw_add.SetFont(&m_font_Default);



}
void CDlgAdmin::DisplayBtnLevel(int nMask, int nLanguage)
{
	for (int i = 0; i < MAX_ITEM; i++)
	{
		// COLOR
		if ((nMask & (MSK_OPER1 << i)) != 0)
		{
			m_btnUserMode[i].SetStaticStyle(CVGStatic::StaticStyle_Title);
			m_btnUserMode[i].SetColorStyle(CVGStatic::ColorStyle_Green);
		}
		else
		{
			m_btnUserMode[i].SetStaticStyle(CVGStatic::StaticStyle_Title);
			m_btnUserMode[i].SetColorStyle(CVGStatic::ColorStyle_Default);
		}
		m_btnUserMode[i].SetFont_Gdip(L"Arial", 12.0F);
	}

	// TEXT
	if (nLanguage == 0)
	{
		m_btnUserMode[BTN_OPER1].SetText(m_szSelect_Admin_Kor[0]);
		m_btnUserMode[BTN_MAINT1].SetText(m_szSelect_Admin_Kor[1]);
		m_btnUserMode[BTN_ADMIN1].SetText(m_szSelect_Admin_Kor[2]);

		m_btnUserMode[BTN_OPER2].SetText(m_szSelect_Admin_Kor[0]);
		m_btnUserMode[BTN_MAINT2].SetText(m_szSelect_Admin_Kor[1]);
		m_btnUserMode[BTN_ADMIN2].SetText(m_szSelect_Admin_Kor[2]);
	}
	else
	{
		m_btnUserMode[BTN_OPER1].SetText(m_szSelect_Admin_Eng[0]);
		m_btnUserMode[BTN_MAINT1].SetText(m_szSelect_Admin_Eng[1]);
		m_btnUserMode[BTN_ADMIN1].SetText(m_szSelect_Admin_Eng[2]);

		m_btnUserMode[BTN_OPER2].SetText(m_szSelect_Admin_Eng[0]);
		m_btnUserMode[BTN_MAINT2].SetText(m_szSelect_Admin_Eng[1]);
		m_btnUserMode[BTN_ADMIN2].SetText(m_szSelect_Admin_Eng[2]);
	}

}

void CDlgAdmin::DisplayBtnOk(int nLanguage)
{
	// color
	for (int i = 0; i < MAX_EXE; i++)
	{
		m_btnChangeOK[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Alt);
		m_btnChangeOK[i].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_btnChangeOK[i].SetFont_Gdip(L"Arial", 12.0F);
	}
	// text
	if (nLanguage == 0)
	{
		m_btnChangeOK[BTN_APPLY].SetText(_T("확인"));
		m_btnChangeOK[BTN_QUIT].SetText(_T("종료"));
		m_btnChangeOK[BTN_ADD].SetText(_T("추가"));
		m_btnChangeOK[BTN_DEL].SetText(_T("삭제"));
	}
	else
	{
		m_btnChangeOK[BTN_APPLY].SetText(_T("OK"));
		m_btnChangeOK[BTN_QUIT].SetText(_T("Quit"));
		m_btnChangeOK[BTN_ADD].SetText(_T("Add"));
		m_btnChangeOK[BTN_DEL].SetText(_T("Delete"));
	}
}

void CDlgAdmin::DisplayTitle(int nLanguage)
{
	// color
	for (int i = 0; i < MAX_TITLE; i++)
	{
		m_stTitle_Sub[i].SetStaticStyle(CVGStatic::StaticStyle_Title);
		m_stTitle_Sub[i].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_stTitle_Sub[i].SetFont_Gdip(L"Arial", 12.0F);
	}
	// text
	if (nLanguage == 0)
	{
		m_stTitle_Sub[TITLE_ID1].SetText(_T("이름"));
		m_stTitle_Sub[TITLE_PW1].SetText(_T("비번"));
		m_stTitle_Sub[TITLE_ID2].SetText(_T("이름"));
		m_stTitle_Sub[TITLE_PW2].SetText(_T("비번"));
	}
	else
	{
		m_stTitle_Sub[TITLE_ID1].SetText(_T("ID"));
		m_stTitle_Sub[TITLE_PW1].SetText(_T("PW"));
		m_stTitle_Sub[TITLE_ID2].SetText(_T("ID"));
		m_stTitle_Sub[TITLE_PW2].SetText(_T("PW"));
	}
}


void CDlgAdmin::InitView_List()
{
	double width[3];
	CRect rct;

	m_ListView.GetClientRect(&rct);
	width[0] = rct.Width() * 0.1;
	width[1] = rct.Width() * 0.4;
	width[2] = rct.Width() * 0.5;

	LV_COLUMN lvColumn[3];
	lvColumn[0].mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvColumn[0].fmt = LVCFMT_LEFT;
	lvColumn[0].cx = (int)width[0];
	lvColumn[0].iSubItem = 0;
	lvColumn[0].pszText = _T("NO");

	lvColumn[1].mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvColumn[1].fmt = LVCFMT_LEFT;
	lvColumn[1].cx = (int)width[1];
	lvColumn[1].iSubItem = 0;
	lvColumn[1].pszText = _T("LEVEL");

	lvColumn[2].mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvColumn[2].fmt = LVCFMT_LEFT;
	lvColumn[2].cx = (int)width[2];
	lvColumn[2].iSubItem = 0;
	lvColumn[2].pszText = _T("NAME");

	m_ListView.InsertColumn(0, &lvColumn[0]);
	m_ListView.InsertColumn(1, &lvColumn[1]);
	m_ListView.InsertColumn(2, &lvColumn[2]);

	//m_ListView.InsertColumn(0, _T("NO"), LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM| LVCFMT_CENTER, (int)width[0]);
	//m_ListView.InsertColumn(1, _T("LEVEL"), LVCFMT_CENTER, (int)width[1]);
	//m_ListView.InsertColumn(2, _T("NAME"), LVCFMT_CENTER, (int)width[2]);
	m_ListView.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_ListView.SetFont(&m_font_List);

}

void CDlgAdmin::DisplayByLevel(int  nLevel)
{
	EQ_BASIC_PARAM	stEq = *DATAMANAGER->GetCurrentEqBasicData();
	EQ_USER_DATA	stEud;
	EQ_LEVEL_DATA	stLvl;
	std::vector<EQ_LEVEL_DATA> vtList;

	int idx = 0;
	for (int i = 0; i < MAX_USER_NUM; i++)
	{
		stEud = stEq.stUserData[i];

		if (stEud.nPermission == nLevel && stEud.strUserID != _T(""))
		{
			idx++;
			stLvl.strNo.Format(_T("%02d"), idx);
			stLvl.strLevel = lvlList[stEud.nPermission];
			stLvl.strUserID = stEud.strUserID;

			vtList.push_back(stLvl);
		}
	}
	m_ListView.DeleteAllItems();

	for (int i = 0; i < idx; i++) {

		m_ListView.InsertItem(LVIF_TEXT | LVIF_STATE, i, vtList[i].strNo, (i % 2) ? LVIS_SELECTED : 0, LVIS_SELECTED, 0, 0);
		m_ListView.SetItemText(i, 1, vtList[i].strLevel);
		m_ListView.SetItemText(i, 2, vtList[i].strUserID);
	}
}
// CDlgAdmin 메시지 처리기

BOOL CDlgAdmin::CheckIDandPassword(__out int & level)
{
	EQ_BASIC_PARAM	stEq = *DATAMANAGER->GetCurrentEqBasicData();
	EQ_USER_DATA	stEud;

	CString strPassword = _T("");
	CString strUserID = _T("");

	BOOL bRet = FALSE;


	GetDlgItemTextW(IDC_EDIT_ADMIN_PW, strPassword);
	GetDlgItemTextW(IDC_EDIT_ADMIN_ID, strUserID);

	if (strUserID == _T(""))
		bRet = FALSE;
	else
	{
		for (int i = 0; i < MAX_USER_NUM; i++)
		{
			stEud = stEq.stUserData[i];

			if (stEud.strUserID == _T(""))
				continue;

			//if (bRet == TRUE)
			//	continue;

			if (strUserID == stEud.strUserID)
			{
				if (strPassword == stEud.strPassword)
				{
					level = stEud.nPermission;
					bRet = TRUE;
				}
				else
					bRet = FALSE;
			}
		}
	}

	if (bRet == TRUE)
	{
		COMMON->SetPermissionID(strUserID);
		COMMON->SetPermissionGrade(level);
		//COMMON->SetPermissionGrade(m_nSelectAcessMode);

		m_bChangeFlag = TRUE;
	}
		

	SetDlgItemText(IDC_EDIT_ADMIN_PW, _T(""));
	SetDlgItemText(IDC_EDIT_ADMIN_ID, _T(""));

	return bRet;
}

BOOL  CDlgAdmin::GetPassword(__in CString str_id, __out CString & str_pw) const
{
	EQ_BASIC_PARAM	stEq = *DATAMANAGER->GetCurrentEqBasicData();
	EQ_USER_DATA	stEud;

	BOOL bRet = FALSE;
	str_pw = _T("");

	if (str_id == _T(""))
		bRet = FALSE;
	else
	{
		for (int i = 0; i < MAX_USER_NUM; i++)
		{
			stEud = stEq.stUserData[i];

			if (stEud.strUserID == _T(""))
				continue;

			if (str_id == stEud.strUserID)
			{
				if (_T("") != stEud.strPassword)
				{
					str_pw = stEud.strPassword;
					bRet = TRUE;
					break;
				}
				else
					bRet = FALSE;
			}
		}
	}

	return bRet;
}

BOOL CDlgAdmin::GetChangFlag()
{
	return m_bChangeFlag;
}

/*
   SAVE / DELETE
   1. 동일 ID 생성 금지
   2. 동일 ID 이면, PW 변경
**/
BOOL CDlgAdmin::Save_Delete_Pemission(int nMode)
{
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();

	CString strPassword = _T("");
	CString strUserID = _T("");
	bool	bFind = false;
	if (nMode == 0 && m_nSavePermission == -1)
	{
		COMMON->ShowMsg_Thread(_T("Select Permission."), MODE_INFORMATION);
		return FALSE;
	}

	//if (m_nSavePermission == 0)
	{
		GetDlgItemText(IDC_EDIT_ADMIN_INPUT_PW, strPassword);
		GetDlgItemText(IDC_EDIT_ADMIN_INPUT_ID, strUserID);

		if (strPassword == _T("") || strUserID == _T(""))
		{
			COMMON->ShowMsg_Thread(_T("Input ID & Password."), MODE_INFORMATION);
			return FALSE;
		}
	}
	//----------------------------------------------------------
	// nMode == 0  계정 저장.
	if (nMode == 0)
	{
		int nTemp = -1;

		if (bFind == false)
		{
			for (int i = 0; i < MAX_USER_NUM; i++)
			{
				if (stEq.stUserData[i].strUserID == strUserID)
				{
					if (stEq.stUserData[i].nPermission == m_nSavePermission)
					{
						nTemp = i;
						bFind = true;
					}
					else
					{
						nTemp = -2;
					}
					break;
				}
			}
			if (nTemp == -2)
			{
				COMMON->ShowMsg_Thread(_T("There is same ID already"), MODE_INFORMATION);
				return FALSE;
			}
		}

		if (bFind == false)
		{
			for (int i = 0; i < MAX_USER_NUM; i++)
			{
			 if (stEq.stUserData[i].strUserID == _T(""))
				{
					nTemp = i;
					break;
				}
			}
			if (nTemp == -1)
			{
				COMMON->ShowMsg_Thread(_T("The number of ID exceeds the maximum."), MODE_INFORMATION);
				return FALSE;
			}
		}

		GetDlgItemText(IDC_EDIT_ADMIN_INPUT_PW, strPassword);
		GetDlgItemText(IDC_EDIT_ADMIN_INPUT_ID, strUserID);

		stEq.stUserData[nTemp].strUserID = strUserID;
		stEq.stUserData[nTemp].strPassword = strPassword;
		stEq.stUserData[nTemp].nPermission = m_nSavePermission;

		DATAMANAGER->SetEqData(stEq);

		COMMON->ShowMsg_Thread(_T("Save Compleate."), MODE_INFORMATION);

		SetDlgItemText(IDC_EDIT_ADMIN_INPUT_PW, _T(""));
		SetDlgItemText(IDC_EDIT_ADMIN_INPUT_ID, _T(""));

		m_btnUserMode[0].SetColorStyle(CVGStatic::ColorStyle_Default);
		m_btnUserMode[1].SetColorStyle(CVGStatic::ColorStyle_Default);
		m_btnUserMode[2].SetColorStyle(CVGStatic::ColorStyle_Default);

		m_btnUserMode[m_nSavePermission].SetColorStyle(CVGStatic::ColorStyle_Green);

		DisplayByLevel(m_nSavePermission);

		return TRUE;
	}
	else   // Delete
	{
		GetDlgItemText(IDC_EDIT_ADMIN_INPUT_PW, strPassword);
		GetDlgItemText(IDC_EDIT_ADMIN_INPUT_ID, strUserID);

		int nTemp = -1;

		for (int i = 0; i < MAX_USER_NUM; i++)
		{
			if (stEq.stUserData[i].strUserID == strUserID)
			{
				if (stEq.stUserData[i].strUserID == _T("Luri"))
					nTemp = -2;
				else
					nTemp = i;
				break;
			}
		}
		if (nTemp == -1)
		{
			COMMON->ShowMsg_Thread(_T("No ID & Password"), MODE_INFORMATION);
			return FALSE;
		}
		else if (nTemp == -2)
		{
			COMMON->ShowMsg_Thread(_T("you can't remove {Luri}."), MODE_INFORMATION);
			return FALSE;
		}

		stEq.stUserData[nTemp].strUserID = _T("");
		stEq.stUserData[nTemp].strPassword = _T("");
		stEq.stUserData[nTemp].nPermission = 0;

		DATAMANAGER->SetEqData(stEq);


		SetDlgItemText(IDC_EDIT_ADMIN_INPUT_PW, _T(""));
		SetDlgItemText(IDC_EDIT_ADMIN_INPUT_ID, _T(""));

		m_btnUserMode[0].SetColorStyle(CVGStatic::ColorStyle_Default);
		m_btnUserMode[1].SetColorStyle(CVGStatic::ColorStyle_Default);
		m_btnUserMode[2].SetColorStyle(CVGStatic::ColorStyle_Default);

		m_btnUserMode[m_nSavePermission].SetColorStyle(CVGStatic::ColorStyle_Green);

		DisplayByLevel(m_nSavePermission);
		
		return TRUE;
	}


}

void CDlgAdmin::OnNMClickedListView(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ListboxToEditbox();

	*pResult = 0;
}

int CDlgAdmin::ListboxToEditbox()
{
	int idx;
	CString str_id, str_pw;

	POSITION pos = m_ListView.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		idx = m_ListView.GetNextSelectedItem(pos);
		str_id = m_ListView.GetItemText(idx, 2);
		SetDlgItemTextW(IDC_EDIT_ADMIN_ID, str_id);
		SetDlgItemTextW(IDC_EDIT_ADMIN_INPUT_ID, str_id);

		GetPassword(str_id, str_pw);
		SetDlgItemTextW(IDC_EDIT_ADMIN_INPUT_PW, str_pw);
	}

	return 0;
}


void CDlgAdmin::OnBnClickedBtnMode1()
{
	EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();

	m_edPw.EnableWindow(TRUE);
	m_edId.EnableWindow(TRUE);

	for (int i = 0; i < MAX_ITEM; i++)
	{
		if (i == BTN_OPER1 || i == BTN_OPER2)
			m_btnUserMode[i].SetColorStyle(CVGStatic::ColorStyle_Green);
		else
			m_btnUserMode[i].SetColorStyle(CVGStatic::ColorStyle_Default);
	}
	m_nSavePermission = LVL_OPER;
	DisplayByLevel(LVL_OPER);
}


void CDlgAdmin::OnBnClickedBtnMode2()
{
	EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();

	m_edPw.EnableWindow(TRUE);
	m_edId.EnableWindow(TRUE);

	for (int i = 0; i < MAX_ITEM; i++)
	{
		if (i == BTN_MAINT1 || i == BTN_MAINT2)
			m_btnUserMode[i].SetColorStyle(CVGStatic::ColorStyle_Green);
		else
			m_btnUserMode[i].SetColorStyle(CVGStatic::ColorStyle_Default);
	}
	m_nSavePermission = LVL_MAINT;
	DisplayByLevel(LVL_MAINT);
}

void CDlgAdmin::OnBnClickedBtnMode3()
{
	EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();

	m_edPw.EnableWindow(TRUE);
	m_edId.EnableWindow(TRUE);

	for (int i = 0; i < MAX_ITEM; i++)
	{
		if (i == BTN_ADMIN1 || i == BTN_ADMIN2)
			m_btnUserMode[i].SetColorStyle(CVGStatic::ColorStyle_Green);
		else
			m_btnUserMode[i].SetColorStyle(CVGStatic::ColorStyle_Default);
	}
	m_nSavePermission = LVL_ADMIN;
	DisplayByLevel(LVL_ADMIN);
}


void CDlgAdmin::OnBnClickedBtnMode4()
{
	EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();

	m_btnUserMode[3].SetStaticStyle(CVGStatic::StaticStyle_Title);
	m_btnUserMode[3].SetColorStyle(CVGStatic::ColorStyle_Green);
	m_btnUserMode[3].SetFont_Gdip(L"Arial", 12);

	m_btnUserMode[4].SetStaticStyle(CVGStatic::StaticStyle_Title);
	m_btnUserMode[4].SetColorStyle(CVGStatic::ColorStyle_Default);
	m_btnUserMode[4].SetFont_Gdip(L"Arial", 12);

	m_btnUserMode[5].SetStaticStyle(CVGStatic::StaticStyle_Title);
	m_btnUserMode[5].SetColorStyle(CVGStatic::ColorStyle_Default);
	m_btnUserMode[5].SetFont_Gdip(L"Arial", 12);

	m_nSavePermission = LVL_OPER;
}


void CDlgAdmin::OnBnClickedBtnMode5()
{
	EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();

	m_btnUserMode[3].SetStaticStyle(CVGStatic::StaticStyle_Title);
	m_btnUserMode[3].SetColorStyle(CVGStatic::ColorStyle_Default);
	m_btnUserMode[3].SetFont_Gdip(L"Arial", 12);

	m_btnUserMode[4].SetStaticStyle(CVGStatic::StaticStyle_Title);
	m_btnUserMode[4].SetColorStyle(CVGStatic::ColorStyle_Green);
	m_btnUserMode[4].SetFont_Gdip(L"Arial", 12);

	m_btnUserMode[5].SetStaticStyle(CVGStatic::StaticStyle_Title);
	m_btnUserMode[5].SetColorStyle(CVGStatic::ColorStyle_Default);
	m_btnUserMode[5].SetFont_Gdip(L"Arial", 12);

	m_nSavePermission = LVL_MAINT;
}

void CDlgAdmin::OnBnClickedBtnMode6()
{
	EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();

	m_btnUserMode[3].SetStaticStyle(CVGStatic::StaticStyle_Title);
	m_btnUserMode[3].SetColorStyle(CVGStatic::ColorStyle_Default);
	m_btnUserMode[3].SetFont_Gdip(L"Arial", 12);

	m_btnUserMode[4].SetStaticStyle(CVGStatic::StaticStyle_Title);
	m_btnUserMode[4].SetColorStyle(CVGStatic::ColorStyle_Default);
	m_btnUserMode[4].SetFont_Gdip(L"Arial", 12);

	m_btnUserMode[5].SetStaticStyle(CVGStatic::StaticStyle_Title);
	m_btnUserMode[5].SetColorStyle(CVGStatic::ColorStyle_Green);
	m_btnUserMode[5].SetFont_Gdip(L"Arial", 12);

	m_nSavePermission = LVL_ADMIN;
}

void CDlgAdmin::OnBnClickedBtnOk1()
{
	int nLevel;
	if (CheckIDandPassword(nLevel) == TRUE)
	{
		Disable_AddPermission(nLevel);
		m_nSelectAcessMode = nLevel;
		// CLOSE
		if (nLevel == LVL_OPER || nLevel == LVL_MAINT)
			CDialogEx::OnOK();
	}
	else
	{
		COMMON->ShowMsg_Thread(_T("Wrong ID & Password"), MODE_INFORMATION);
	}
}

void CDlgAdmin::OnBnClickedBtnOk2()
{
	CDialogEx::OnOK();
}

void CDlgAdmin::OnBnClickedBtnOk3()
{
	Save_Delete_Pemission(0);
}


void CDlgAdmin::OnBnClickedBtnOk4()
{
	Save_Delete_Pemission(1);
}




