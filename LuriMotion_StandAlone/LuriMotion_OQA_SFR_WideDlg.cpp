
// LuriMotionUIDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "LuriMotion_OQA_SFR_Wide.h"
#include "LuriMotion_OQA_SFR_WideDlg.h"
#include "afxdialogex.h"

#include "IO.h"
#include "Device.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// CLuriMotionUIDlg 대화 상자
CLuriMotion_OQA_SFR_WideDlg::CLuriMotion_OQA_SFR_WideDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LuriMotion_OQA_SFR_Wide_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pDlgMain				= NULL;;
	m_pDlgSide				= NULL;;
	m_pDlgTitle				= NULL;;
	m_pDlgBtm				= NULL;
	
	m_pDlgTeach				= NULL;
	m_pDlgIO				= NULL;
	m_pDlgComponent			= NULL;
	m_pDlgMaintence			= NULL;

	m_bStartCompleate		= FALSE;
	m_bMessageBlock = false;
}	
void CLuriMotion_OQA_SFR_WideDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CLuriMotion_OQA_SFR_WideDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(MESSAGE_UI_WINDOW_TOP, OnMessageWindow2Top)
	ON_MESSAGE(UM_RECV_BARCODE, OnRecvBarcode)
END_MESSAGE_MAP()

LRESULT CLuriMotion_OQA_SFR_WideDlg::OnRecvBarcode(WPARAM wParam, LPARAM lParam)
{
	CStringA szTemp = (char*)wParam;
	DWORD dwLength = (DWORD)lParam;

 	if (SEQUENCE->GetRunMode() != eStandby)
 		return FALSE;

	szTemp.Remove('\r');
	szTemp.Remove('\n');

//	szTemp = _T("000000000001");

	USES_CONVERSION;
	CString szBarcode = A2T(szTemp.GetBuffer());
	szTemp.ReleaseBuffer();

	CString sz;
	sz.Format(_T("Recv BARCODE : %s"), szBarcode);
	WRITE_SEQ_LOG(sz);

	// 모델체크
	bool bFindModel = true;

	switch (GET_INSPECTOR)
	{
	case SYS_DISTORTION_30:
		bFindModel = FindModel(szBarcode);
		break;
	default:
		break;
	}	 

	if (bFindModel)
	{
		m_UdpTester[0].m_bTesterFlag_Ready = FALSE;

		// ready 전송
		CDevice::SEMCO_SendReadyStart();

		// ready ack 확인
		UINT nRetCode = 0;
		if (CDevice::SEMCO_WaitReadyStartAck(3000, nRetCode) == false)
		{
			sz.Format(_T("Recv Ready Ack : %d"), nRetCode);
			WRITE_SEQ_LOG(sz);
			return 0;
		}

		// 전송
		CDevice::SEMCO_SendBcr(szBarcode);

		// bcr ACK 확인 
		CString szBCR;

		if (CDevice::SEMCO_WaitBarcodeAck(1000, szBCR) == false)
		{
			sz.Format(_T("Recv Ready Ack : %d"), nRetCode);
			WRITE_SEQ_LOG(sz);
			return 0;
		}

		//
		if (szBarcode == szBCR)
		{
			sz.Format(_T("Send BCR : %s == Recv BCR : %s"), szBarcode, szBCR);
			m_UdpTester[0].m_bTesterFlag_Ready = TRUE;

			AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_01_START_SWITCH_LAMP, TRUE);
			Sleep(500);
			AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_01_START_SWITCH_LAMP, FALSE);
		}
		else
		{
			sz.Format(_T("Send BCR : %s != Recv BCR : %s"), szBarcode, szBCR);
			m_UdpTester[0].m_bTesterFlag_Ready = FALSE;
		}
	}
	else
	{
		// 등록되지 않은 바코드 정보일경우 
		CString msg;
		msg.Format(_T("[Error] Camera Barcode  \r\n\r\n Please Scan Camera Barcode again \r\n [Input : %s]"), szBarcode);
		RunMessageDlg_Domodal(msg, 1);
		WRITE_SEQ_LOG(msg);
	}	

	WRITE_SEQ_LOG(sz);
	return TRUE;
}

BOOL CLuriMotion_OQA_SFR_WideDlg::PreTranslateMessage(MSG* pMsg)
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

// CLuriMotionUIDlg 메시지 처리기

BOOL CLuriMotion_OQA_SFR_WideDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	Gdiplus::GdiplusStartup(&m_gdiplusToken, &m_gdiplusStartupInput, NULL);

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	ShowWindow(SW_SHOWMAXIMIZED);

	InitProgram();

	CreateDlg();
	Dlg_MovePosition();

	InitDevice();

	m_bStartCompleate = TRUE;

//	OnRecvBarcode(0, 0);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

/*BOOL CLuriMotion_OQA_SFR_WideDlg::Connect_PowerSupply()
{
	if (m_ctrlLight_ODA_Q[0].m_bConnected == FALSE)
	{
		m_ctrlLight_ODA_Q[0].Create();
		m_ctrlLight_ODA_Q[0].Invalidate(0);
	}

	if (m_ctrlLight_ODA_Q[0].OpenPort(6, 0, WM_LIGHT_CONTROL_Q_MSG, 0, 1, 0, 0) == FALSE)
	{
		WRITE_MAIN_LOG(_T("FAIL CONNECT"));
		return FALSE;
	}
	else
		WRITE_MAIN_LOG(_T("POWER SUPPLY CONNECT SUCCESS"));

	return TRUE;
}*/

BOOL CLuriMotion_OQA_SFR_WideDlg::Connect_Tester()
{
	CString strTemp = _T("");

	TESTER_COMM stTester = *DATAMANAGER->GetCurrentTesterCommData();
	m_UdpTester[0].Connect_Tester(stTester.dwTesterIP[0], stTester.nTesterPort[0], stTester.strTesterConnect[0], 1);

	COMMON->m_bConnectTesterFlag[0] = TRUE;
	return FALSE;
}

void CLuriMotion_OQA_SFR_WideDlg::CreateDlg()
{
	m_pDlgMain = CDlgMain::Get_Instance(this);
	
	//m_pDlgMain = new CDlgMain;
	//m_pDlgMain->Create(CDlgMain::IDD, 0);
	m_pDlgMain->ShowWindow(SW_SHOW);

	m_pDlgSide = new CDlgSide;
	m_pDlgSide->Create(CDlgSide::IDD, 0);
	m_pDlgSide->ShowWindow(SW_SHOW);

	m_pDlgBtm = new CDlgBottom;
	m_pDlgBtm->Create(CDlgBottom::IDD, 0);
	m_pDlgBtm->ShowWindow(SW_SHOW);

	m_pDlgTitle = new CDlgTitle;
	m_pDlgTitle->Create(CDlgTitle::IDD, 0);
	m_pDlgTitle->ShowWindow(SW_SHOW);

	m_pDlgTeach = new CDlgMotorTeach;
	m_pDlgTeach->Create(CDlgMotorTeach::IDD, 0);
	m_pDlgTeach->ShowWindow(SW_HIDE);

	m_pDlgIO = new CDlgIo;
	m_pDlgIO->Create(CDlgIo::IDD, 0);
	m_pDlgIO->ShowWindow(SW_HIDE);

	m_pDlgErrSolution = new CDlgErrorSolution;
	m_pDlgErrSolution->Create(CDlgErrorSolution::IDD, 0);
	m_pDlgErrSolution->ShowWindow(SW_HIDE);	

	m_pDlgComponent = new CDlgComponent;
	m_pDlgComponent->Create(CDlgComponent::IDD, 0);
	m_pDlgComponent->ShowWindow(SW_HIDE);

	m_pDlgMaintence = new CDlgMaintence;
	m_pDlgMaintence->Create(CDlgMaintence::IDD, 0);
	m_pDlgMaintence->ShowWindow(SW_HIDE);

	m_pMsgBox_Instance = CDlgMessageBox_Instance::Get_Instance(this);
}
void CLuriMotion_OQA_SFR_WideDlg::Dlg_MovePosition()
{
	CRect rt;

	// Title.
	rt.left = 0;
	rt.top = 0;
	rt.right = 1920;
	rt.bottom = 150;
	m_pDlgTitle->MoveWindow(rt, TRUE);

	// Main 먼저.
	rt.left = 0;
	rt.top = 150;
	rt.right = 1700;
	rt.bottom = 940;
	m_pDlgMain->MoveWindow(rt, TRUE);

	rt.left = 0;
	rt.top = 150;
	rt.right = 1701;
	rt.bottom = 940;
	m_pDlgTeach->MoveWindow(rt, TRUE);

	rt.left = 0;
	rt.top = 150;
	rt.right = 1701;
	rt.bottom = 940;
	m_pDlgIO->MoveWindow(rt, TRUE);

	rt.left = 0;
	rt.top = 150;
	rt.right = 1701;
	rt.bottom = 940;
	m_pDlgComponent->MoveWindow(rt, TRUE);
	
	rt.left = 0;
	rt.top = 150;
	rt.right = 1701;
	rt.bottom = 940;
	m_pDlgErrSolution->MoveWindow(rt, TRUE);

	rt.left = 0;
	rt.top = 150;
	rt.right = 1701;
	rt.bottom = 940;
	m_pDlgMaintence->MoveWindow(rt, TRUE);
	

	// Side 
	rt.left = 1700;
	rt.top = 150;
	rt.right = 1920;
	rt.bottom = 940;
	m_pDlgSide->MoveWindow(rt, TRUE);

	//Btm
	rt.left = 0;
	rt.top = 940;
	rt.right = 1920;
	rt.bottom = 1080;
	m_pDlgBtm->MoveWindow(rt, TRUE);
	
	Invalidate();
}

void CLuriMotion_OQA_SFR_WideDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CLuriMotion_OQA_SFR_WideDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CLuriMotion_OQA_SFR_WideDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CLuriMotion_OQA_SFR_WideDlg::OnClose()
{
	m_ctrlLightController_Youth_DPS[0].LightOff();
	SEQUENCE->End_RunThread();

	PostQuitMessage(WM_QUIT);

	HANDLE hHandle = AfxGetMainWnd()->m_hWnd;
	DWORD dwID;
	GetWindowThreadProcessId((HWND)hHandle, &dwID);

	hHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwID);
	TerminateProcess(hHandle, 0);
	CloseHandle(hHandle);
}

void CLuriMotion_OQA_SFR_WideDlg::ChangeMainUi(int nPart)
{
	m_pMsgBox_Instance->ShowWindow(SW_HIDE);

	switch ( nPart )
	{
	case 1: // main
		m_pDlgIO->ShowWindow(SW_HIDE);
		m_pDlgTeach->ShowWindow(SW_HIDE);
		m_pDlgComponent->ShowWindow(SW_HIDE);
		m_pDlgErrSolution->ShowWindow(SW_HIDE);
		m_pDlgMaintence->ShowWindow(SW_HIDE);

		m_pDlgMain->ShowWindow(SW_SHOW);
		break;
	case 2: // teach
		m_pDlgMain->ShowWindow(SW_HIDE);
		m_pDlgIO->ShowWindow(SW_HIDE);	
		m_pDlgComponent->ShowWindow(SW_HIDE);
		m_pDlgErrSolution->ShowWindow(SW_HIDE);
		m_pDlgMaintence->ShowWindow(SW_HIDE);

		m_pDlgTeach->ShowWindow(SW_SHOW);
		break;
	case 3:	// maint
		m_pDlgMain->ShowWindow(SW_HIDE);
		m_pDlgIO->ShowWindow(SW_HIDE);
		m_pDlgComponent->ShowWindow(SW_HIDE);
		m_pDlgErrSolution->ShowWindow(SW_HIDE);
		m_pDlgTeach->ShowWindow(SW_HIDE);

		m_pDlgMaintence->ShowWindow(SW_SHOW);
		break;
	case 4: // io
		m_pDlgTeach->ShowWindow(SW_HIDE);
		m_pDlgMain->ShowWindow(SW_HIDE);
		m_pDlgComponent->ShowWindow(SW_HIDE);
		m_pDlgErrSolution->ShowWindow(SW_HIDE);
		m_pDlgMaintence->ShowWindow(SW_HIDE);

		m_pDlgIO->ShowWindow(SW_SHOW);
		break;
	case 5: // component
		m_pDlgIO->ShowWindow(SW_HIDE);
		m_pDlgTeach->ShowWindow(SW_HIDE);
		m_pDlgMain->ShowWindow(SW_HIDE);
		m_pDlgErrSolution->ShowWindow(SW_HIDE);
		m_pDlgMaintence->ShowWindow(SW_HIDE);

		m_pDlgComponent->ShowWindow(SW_SHOW);
		break;
	case 6:
		m_pDlgIO->ShowWindow(SW_HIDE);
		m_pDlgTeach->ShowWindow(SW_HIDE);
		m_pDlgMain->ShowWindow(SW_HIDE);
		m_pDlgComponent->ShowWindow(SW_HIDE);
		m_pDlgMaintence->ShowWindow(SW_HIDE);

		m_pDlgErrSolution->ShowWindow(SW_SHOW);
		break;
	case 7:
		break;
	}
}
void CLuriMotion_OQA_SFR_WideDlg::RecipeChange_Ui_Refresh()
{
	m_pDlgTeach->Load_TeachingData();

	m_pDlgComponent->Load_ComponentDlg();
}

// [21.1017.10] Modified , 이름변경
void CLuriMotion_OQA_SFR_WideDlg::ChangeUi_ByLanguage()
{
	m_pDlgBtm->ChangeUi_Language();
	m_pDlgMain->ChangeUI_Language();
	m_pDlgIO->ChangeUI_Language();
	m_pDlgTeach->ChangeUI_Language();
	m_pDlgComponent->ChangeUI_Language();
	m_pDlgSide->ChangeUI_Language();
	m_pDlgErrSolution->ChangeUI_Language();
}

// [21.1017.11] Added
void CLuriMotion_OQA_SFR_WideDlg::ChangeUi_ByLevel()
{
	//m_pDlgBtm->ChangeUi_Language();
	//m_pDlgMain->ChangeUI_Language();
	//m_pDlgIO->ChangeUI_Language();
	m_pDlgTeach->ChangeUI_Level();
	//m_pDlgComponent->ChangeUI_Language();
	//m_pDlgSide->ChangeUI_Language();
	//m_pDlgErrSolution->ChangeUI_Language();
}

// [21.1030.1]
BOOL CLuriMotion_OQA_SFR_WideDlg::RunMessageDlg_Domodal(CString strInfo, int nMode)
{
	CCriticalSection cs;
	
	m_critSec.Lock();
	if (m_bMessageBlock == true)
		return TRUE;
	m_DlgMessage.SetMsg(strInfo, nMode);
	m_bMessageBlock = true;
	if (m_DlgMessage.DoModal() == IDOK)
	{
		m_critSec.Unlock();
		m_bMessageBlock = false;
		return TRUE;
	}
	m_critSec.Unlock();
	m_bMessageBlock = false;
	return FALSE;
}

void CLuriMotion_OQA_SFR_WideDlg::RunMessageDlg_Instance(CString strInfo, int nMode)
{
	m_pMsgBox_Instance->SetMsg(strInfo, nMode);
}
void CLuriMotion_OQA_SFR_WideDlg::RunMessageDlg_Instance_Show()
{
	m_pMsgBox_Instance->ShowWindow(SW_SHOW);
}

void CLuriMotion_OQA_SFR_WideDlg::Hide_Program()
{
	ShowWindow(SW_SHOWMINIMIZED);
}	

// [21.1021.7]
LRESULT CLuriMotion_OQA_SFR_WideDlg::OnMessageWindow2Top(WPARAM wParam, LPARAM lParam)
{
	BringWindowToTop();
	return TRUE;
}

void CLuriMotion_OQA_SFR_WideDlg::InitProgram()
{
	SEQUENCE->m_bProgramFirstStart = true;
	SEQUENCE->m_enEMOStep = enEMOStep::EMO_1st;
}

void CLuriMotion_OQA_SFR_WideDlg::InitDevice()
{
	CDevice::ConnectDevicez();

	auto EqpType = GET_INSPECTOR;

	switch (EqpType)
	{
	case SYS_DISTORTION_30:
		CIO::IO_O_ProbePin_Down();
		CIO::IO_O_Camera_Fix_Cylinder_Off();
		break;

	case SYS_OQC_SFR_MULTI_CL:
		break;

	case SYS_OQC_SFR_SINGLE_CL:
		break;
	}
}

void CLuriMotion_OQA_SFR_WideDlg::OnInitialize()
{
	m_pDlgSide->OnInitialize();
}

bool CLuriMotion_OQA_SFR_WideDlg::FindModel(__in CString szBCR)
{
	bool bRet = false;

	TEACHING_PARAM stTeach = *DATAMANAGER->GetCurrentTeachingData();

	auto cnt = stTeach.vModel_8M_30.size();
	for (auto i = 0; i < cnt; i++)
	{
		auto str = stTeach.vModel_8M_30[i];

		if (szBCR.Find(str) != -1)
		{
			bRet = true;
			break;
		}
	}

	return bRet;
}