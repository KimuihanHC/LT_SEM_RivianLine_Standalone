// DlgLog.cpp: 구현 파일
//

#include "pch.h"
#include "LuriMotion_OQA_SFR_Wide.h"
#include "DlgLog.h"
#include "afxdialogex.h"


// CDlgLog 대화 상자

IMPLEMENT_DYNAMIC(CDlgLog, CDialogEx)

CDlgLog::CDlgLog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_LOG, pParent)
{
	m_nLogMode = 0;

	VERIFY(m_font_Default.CreateFont(
		16,							// nHeight
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
		_T("Arial")));		// lpszFacename
}

CDlgLog::~CDlgLog()
{
}

void CDlgLog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_LOG, m_listLog);
}


BEGIN_MESSAGE_MAP(CDlgLog, CDialogEx)
	ON_WM_SIZE()
END_MESSAGE_MAP()



BOOL CDlgLog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_listLog.DeleteColumn(0);
	m_listLog.SetFont(&m_font_Default);

	CString strTitle = _T("");

	if (m_nLogMode == 0)
		strTitle = _T("Main Log");
	else if (m_nLogMode == 1)
		strTitle = _T("Sequence Log");
	else if (m_nLogMode == 2)
		strTitle = _T("Comm Log");
	else if (m_nLogMode == 3 )
		strTitle = _T("Barcode Log");

	m_listLog.InsertColumn(0, strTitle, LVCFMT_LEFT, m_nWidth, 0);

	ListClear();
	ListPrint(_T("Program Start..."));
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgLog::ListClear()
{
	m_listLog.DeleteAllItems();

	m_listLog.SetBkColor(RGB(255, 230, 230));
	m_listLog.SetTextBkColor(RGB(255, 230, 230));
	// 리스트 컨트롤의 행 선택,Flat,라인.. 스타일
	m_listLog.SetExtendedStyle(LVS_EX_FLATSB | LVS_EX_GRIDLINES);
}

void CDlgLog::ListPrint(LPCTSTR lpszText)
{
	CString str;
	CString str_Time;
	CTime t = CTime::GetCurrentTime();
	str_Time.Format(_T("[%02d:%02d:%02d]"), t.GetHour(), t.GetMinute(), t.GetSecond());

	int aaa = m_listLog.GetItemCount();

	str.Format(_T("%s %s"), str_Time,lpszText);
	m_listLog.InsertItem(m_listLog.GetItemCount(), str);
	m_listLog.EnsureVisible(aaa, FALSE);

	if (m_nLogMode == 2)
	{
		str_Time += _COMM_LOG;
		WRITE_COM_FILE(str_Time, lpszText);
	}		
	else if (m_nLogMode == 1)
	{
		str_Time += _SEQ_LOG;
		WRITE_SEQ_FILE(str_Time, lpszText);
	}		
	else if (m_nLogMode == 0)
	{
		str_Time += _SYSTEM_LOG;
		WRITE_SYSTEM_FILE(str_Time, lpszText);
	}
	else if (m_nLogMode == 3)
	{
		str_Time += _BARCODE_LOG;
		WRITE_BARCODE_FILE(str_Time, lpszText);
	}
}

void CDlgLog::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if (m_listLog.GetSafeHwnd())
		m_listLog.MoveWindow(0, 0, cx, cy);
}

void CDlgLog::SetWidth(int nWidth)
{
	m_nWidth = nWidth;
}

void CDlgLog::SetMode(int nMode)
{
	m_nLogMode = nMode;
}