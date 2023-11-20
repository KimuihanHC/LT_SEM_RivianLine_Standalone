
#include "pch.h"
#include "Socket_Udp.h"
#pragma comment (lib,"ws2_32.lib")

CUDPSocket m_UdpTester[LAN_COMMUNICATION_COUNT];


UINT CUDPSocket::Thread_UDPRead(LPVOID pVoid)
{
	DWORD dwWFSObj = 0;
	CUDPSocket *dlg = (CUDPSocket *)pVoid;

	int Recv_Size;

	BYTE Buffer[884] = { 0, };

	int nSize = sizeof(dlg->m_ClientAddr);
	int sendSize = 0;

	while (1)
	{
		// While  사용에는 Sleep이 무조건 잠깐은 필요하다.
		Sleep(20);

		Recv_Size = recvfrom(dlg->m_clientSock, reinterpret_cast<char*>(Buffer), 1024, 0, (struct sockaddr*) &dlg->m_ClientAddr, &nSize);

		// [21.1023.1] Modified , 연결됨
		if (Recv_Size >= 0)
		{
			dlg->m_bConnected = TRUE;
		}


		// -1이면 에러 
		if (Recv_Size >= 0)
		{
			// RECV 성공 시 MSG 처리.. 
			CString strTemp, strMsg;
			for (int i = 0; i < Recv_Size; i++)
			{
				strTemp.Format(_T("%c"), Buffer[i]);
				strMsg += strTemp;
			}
			COMMON->Write_Comm_Log(strMsg);

			dlg->ParsingData_HandlerToTester(strMsg);

			strMsg = _T("");
			CString str;
			str.Format(_T("%s"), Buffer);			
		}
	}

	closesocket(dlg->m_clientSock); //소켓 닫기

	return 0;

}

CUDPSocket::CUDPSocket(void)
{
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			m_bFlareGhostFlag_Recive[i][j] = FALSE;
		}
	}

	auto cnt = _countof(m_bTesterFlag_Distortion);

	for (auto i = 0; i < cnt; i++)
	{
		m_bTesterFlag_Distortion[i] = FALSE;
		m_nTesterFlag_Inspection_DistortionErrorCode[i] = 0;
	}	

	m_bConnectCheckFunc = FALSE;
}

CUDPSocket::~CUDPSocket(void)
{
	CloseSocket();
}

void CUDPSocket::InitVariable(int id)
{
	m_nId = id;
}

BEGIN_MESSAGE_MAP(CUDPSocket, CWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

BOOL CUDPSocket::Create(CWnd* pParentWnd)
{
	CRect rect = CRect(0, 0, 0, 0); // 화면 영역 설정
	return CWnd::Create(NULL, _T("SYSocket"), WS_CHILD, rect, GetDesktopWindow(), 1);
}

int CUDPSocket::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	

	return 0;
}

void CUDPSocket::CloseSocket()
{
		m_bConnected = FALSE;
}
BOOL CUDPSocket::Thread_Start()
{
	if (m_bThreadRun == FALSE)
	{
		m_bThreadRun = TRUE;
		pUDP_RecvThread = AfxBeginThread(Thread_UDPRead, this);
	}
	else
	{
		if (m_bThreadRun)
		{
			m_bThreadRun = FALSE;
			WaitForSingleObject(pUDP_RecvThread->m_hThread, INFINITE);
			return FALSE;
		}
	}

	return TRUE;
}
void CUDPSocket::Connect_Tester(DWORD server_ip, UINT server_port, CString strIP, int flag)
{
	CString strTemp = _T("$H0190000000000000#");

	int nLen = 0;

	for (int i = 0; i < strTemp.GetLength(); i++)
	{
		if ((strTemp.GetAt(i) <= 0) || (strTemp.GetAt(i) > 127))
			nLen += 2;
		else
			nLen += 1;
	}

	char* Buffer = new char[nLen + 1];

	size_t CharactersConverted = 0;

	// 유니코드 -> 아스키코드 (멀티바이트 -> 싱글바이트)
	wcstombs_s(&CharactersConverted, Buffer, nLen + 1, strTemp, _TRUNCATE);

	WSADATA wsdata;
	WSAStartup(MAKEWORD(2, 2), &wsdata);

	m_clientSock = socket(AF_INET, SOCK_DGRAM, 0);
	ZeroMemory(&m_ClientAddr, sizeof(m_ClientAddr));
	ZeroMemory(&m_FromServer, sizeof(m_FromServer));
	

	m_ClientAddr.sin_family = AF_INET;
	m_ClientAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	m_ClientAddr.sin_port = htons(6565);

	//m_ClientAddr = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);


	int retval = sendto(m_clientSock, reinterpret_cast<char*>(Buffer), nLen, 0, (SOCKADDR*)&m_ClientAddr, sizeof(m_ClientAddr));

	if (retval < 0 )
		COMMON->Write_Comm_Log(_T("SEND FAIL [READY]"));

	delete[] Buffer;


	pUDP_RecvThread = AfxBeginThread(Thread_UDPRead, this);

	// JYS 210923 TEST2.
	m_bConnectCheckFunc = TRUE;
	m_dwTimeOut = GetTickCount();
	// JYS 210923 TEST2.
}

bool CUDPSocket::Send_Flare_CLL_Communication(int nCondition, int nLL)
{
	CString strTemp = _T("");
	int	nLength = 0;

	strTemp.Format(_T("$H018000000%d%02d0000#"), nCondition, nLL);


	int nLen = 0;

	for (int i = 0; i < strTemp.GetLength(); i++)
	{
		if ((strTemp.GetAt(i) <= 0) || (strTemp.GetAt(i) > 127))
			nLen += 2;
		else
			nLen += 1;
	}

	char* Buffer = new char[nLen + 1];

	size_t CharactersConverted = 0;

	// 유니코드 -> 아스키코드 (멀티바이트 -> 싱글바이트)
	wcstombs_s(&CharactersConverted, Buffer, nLen + 1, strTemp, _TRUNCATE);

	int retval = sendto(m_clientSock, reinterpret_cast<char*>(Buffer), nLen, 0, (SOCKADDR*)&m_ClientAddr, sizeof(m_ClientAddr));

	COMMON->Write_Comm_Log(strTemp);

	delete[] Buffer;

	return TRUE;
}


bool CUDPSocket::Send_Tester_Communication(int nMode, int nTestIdx)
{
	CString strTemp = _T("");
	int	nLength = 0;

	switch (nMode)
	{
	case eComMode_Ready:
		strTemp = _T("$H0100000000000000#");
		break;
	case eComMode_PreviewStart:
		strTemp = _T("$H0110000000000000#");
		break;
	case eComMode_Insp_SFR:
		strTemp = _T("$H0130000000000000#");
		break;
	case eComMode_Insp_Distortion:
		strTemp.Format(_T("$H0170000000%02d0000#"), nTestIdx * 1);
		break;
	case eComMode_Insp_TEST:
		strTemp = _T("$H0160000000000000#");
		break;
	case eComMode_Insp_PreviewStop:
		strTemp = _T("$H0190000000000000#");
		break;
	case eComMode_Barcode:
		strTemp = _T("$H_Query[BARCDOE]#");
		break;
	case eComMode_Version:
		strTemp = _T("$H_Query[Version]#");
		break;
	case eComMode_SwInit:
		strTemp = _T("$H_Query[SWInitialized]#");
		break;
	case eComMode_FuseID:
		strTemp = _T("$H_Query[Fuse-ID]#");
		break;
	case eComMode_Status:
		strTemp = _T("$H_Query[Status]#");
		break;
	case eComMode_MES_CONNECT:
		strTemp = _T("$H_Query[MES_CONNECT]#");
		break;
	case eComMode_SendMES:
		strTemp = _T("$H_Query[SendMesDATA]#");
		break;
	}

	int nLen = 0;

	for (int i = 0; i < strTemp.GetLength(); i++)
	{
		if ((strTemp.GetAt(i) <= 0) || (strTemp.GetAt(i) > 127))
			nLen += 2;
		else
			nLen += 1;
	}

	char* Buffer = new char[nLen + 1];

	size_t CharactersConverted = 0;

	// 유니코드 -> 아스키코드 (멀티바이트 -> 싱글바이트)
	wcstombs_s(&CharactersConverted, Buffer, nLen + 1, strTemp, _TRUNCATE);

	int retval = sendto(m_clientSock, reinterpret_cast<char*>(Buffer), nLen, 0, (SOCKADDR*)&m_ClientAddr, sizeof(m_ClientAddr));
	
	COMMON->Write_Comm_Log(strTemp);

	delete[] Buffer;

	return TRUE;
}

void CUDPSocket::ParsingData_HandlerToTester(CString strData)
{
	// Sample:
	// 1 $T01100000000000#
	// 2 $T_ANSWER[BARCODE=ssss]#

	/*if( COMMON->GetAccpet_ReciveBarcode() == FALSE )
		return;*/

	CString strLogTemp = _T("");
	strLogTemp.Format(_T("[Recv][Tester] %s"), m_nId, strData);
	COMMON->Write_Comm_Log(strLogTemp);

	CString strCommand = _T("");
	CString strRecData = _T("");
	CString strCLLNumber = _T("");
	CString strNgCode = _T("");

	if (strData.GetAt(1) != 'T')
	{
		strLogTemp.Format(_T("[Recv][Tester] %c"), strData.GetAt(1));
		COMMON->Write_Comm_Log(strLogTemp);
		return;
	}

	if (strData.GetAt(2) == '0')
	{
		// 숫자 Command 일경우.
		strCommand = strData.Mid(2, 4);
		strRecData = strData.Mid(5, 8);
		strCLLNumber = strData.Mid(12, 2);
		strNgCode = strData.Mid(14, 4);

		int nCll = _ttoi(strCLLNumber);

		if (strCommand == _T("0100"))
		{
			m_bTesterFlag_Ready = TRUE;

			m_nTesterFlag_ReadyErrorCode = _ttoi(strNgCode);
		}
		else if (strCommand == _T("0110"))
		{
			m_bTesterFlag_PreviewStart = TRUE;
			m_bTesterFlag_PreviewStartErrorCode =_ttoi(strNgCode);
		}
		else if (strCommand == _T("0130"))
		{
			m_bTesterFlag_Inspection_SFR = TRUE;
			m_bTesterFlag_Inspection_SFRErrorCode =_ttoi(strNgCode);
		}
		else if (strCommand == _T("0160"))
		{
			m_bTesterFlag_Inspection_TEST = TRUE;
			m_bTesterFlag_Inspection_TESTErrorCode =_ttoi(strNgCode);
		}
		else if (strCommand == _T("0170"))
		{
			//if (nCll == 10)
			//{
			//	m_bTesterFlag_Distortion[nCll] = TRUE;
			//	m_nTesterFlag_Inspection_DistortionErrorCode[nCll] = _ttoi(strNgCode);
			//}
			//else
			//{
				m_bTesterFlag_Distortion[nCll] = TRUE;
				m_nTesterFlag_Inspection_DistortionErrorCode[nCll] = _ttoi(strNgCode);
//			}
		}
		else if (strCommand == _T("0190"))
		{
			m_bTesterFlag_PreviewStop = TRUE;
			m_bTesterFlag_PreviewStopErrorCode = _ttoi(strNgCode);
		}
	}
	else if (strData.GetAt(2) == '_')
	{
		int nLength = strData.GetLength();
		int nParsingLenght = 0;
		int nStartPos = 0;
		CString strMsg = _T("");
		CString strTemp = _T("");
		CString strPart = _T("");
		CString strResult = _T("");

		for (int i = 0; i < nLength; i++)
		{
			if (strData.GetAt(i) == '[')
				nStartPos = i;
		}

		for (int i = nStartPos + 1; i < nLength - 2; i++)
		{
			strTemp.Format(_T("%c"), strData.GetAt(i));
			strMsg += strTemp;
		}

		strPart = strMsg.Left(3);

		if (strPart == _T("BAR"))
		{
			m_bAckFlag_Barcode = true;
			strMsg = strMsg + _T("=");
			AfxExtractSubString(m_strTesterQuery_Barcode, strMsg, 1, '=');
		}
		else if (strPart == _T("Ver"))
		{
			m_strTesterQuery_Version = strMsg;
		}
		else if (strPart == _T("SWI"))
		{
			strMsg = strMsg + _T("=");
			AfxExtractSubString(strResult, strMsg, 1, '=');

			if (strResult == _T("TRUE"))
				m_strTesterQuery_SWInital = TRUE;
			else
				m_strTesterQuery_SWInital = FALSE;
		}
		else if (strPart == _T("Fus"))
		{
			strMsg = strMsg + _T("=");
			AfxExtractSubString(m_strTesterQuery_FuseID, strMsg, 1, '=');
		}
		else if (strPart == _T("Wor"))
		{
			m_strTesterQuery_Version = strMsg;
		}
		else if (strPart == _T("MES"))
		{
			if (strMsg.GetAt(4) == '_')
			{
				strMsg = strMsg + _T("=");
				AfxExtractSubString(strResult, strMsg, 1, '=');

				if (strResult == _T("TRUE"))
					m_strTesterQuery_MES_Connect = TRUE;
				else
					m_strTesterQuery_MES_Connect = FALSE;
			}
			else if (strMsg.GetAt(4) == '=')
			{
				strMsg = strMsg + _T("=");
				AfxExtractSubString(m_strTesterQuery_MesData, strMsg, 1, '=');
			}
		}
		else if (strPart == _T("Cur"))
		{
			m_bAckFlag_CurrentVoltage = true;
			strMsg = strMsg + _T("=");
			AfxExtractSubString(m_strTesterQuery_CurrentVoltage, strMsg, 1, '=');
		}
	}
}

void CUDPSocket::Reset_ReadyFlag()
{
	m_bTesterFlag_Ready = FALSE;
}

void CUDPSocket::Reset_TesterFlag()
{
	m_bAckFlag_Barcode = FALSE;
	m_bTesterFlag_PreviewStart = FALSE;
	m_bTesterFlag_Inspection_SFR = FALSE;
	m_bTesterFlag_Inspection_TEST = FALSE;
	m_bTesterFlag_PreviewStop = FALSE;
	m_bAckFlag_CurrentVoltage = FALSE;

	m_strTesterQuery_Barcode = _T("");
	m_strTesterQuery_Version = _T("");
	m_strTesterQuery_SWInital = FALSE;
	m_strTesterQuery_FuseID = _T("");
	m_strTesterQuery_Status = _T("");
	m_strTesterQuery_MES_Connect = FALSE;
	m_strTesterQuery_MesData = _T("");
	m_strTesterQuery_CurrentVoltage = _T("");

	m_nTesterFlag_ReadyErrorCode = FALSE;
	m_bTesterFlag_PreviewStartErrorCode = FALSE;
	m_bTesterFlag_Inspection_SFRErrorCode = FALSE;
	m_bTesterFlag_Inspection_TESTErrorCode = FALSE;
	m_bTesterFlag_PreviewStopErrorCode = FALSE;

	for (int i = 0; i < 10; i++)
	{
		m_bTesterFlag_Distortion[i] = FALSE;
		m_nTesterFlag_Inspection_DistortionErrorCode[i] = 0;
	}
}

BOOL CUDPSocket::ForcedLive_Ready()
{
	m_bTesterFlag_Ready = TRUE;

	return TRUE;
}

bool CUDPSocket::Send_Barcode(__in CString szBCR)
{
	CString strTemp = _T("");
	int	nLength = 0;

	strTemp.Format(_T("$H_Query[BARCODE=%s]#"), szBCR);
	return SendData(strTemp);
}

bool CUDPSocket::Send_CurrentVoltage(__in float current, __in float voltage)
{
	CString strTemp = _T("");
	int	nLength = 0;
	strTemp.Format(_T("$H_Query[Current_Voltage=%f_%f]#"), voltage, current);
	return SendData(strTemp);
}

bool CUDPSocket::SendData(__in CString szSend)
{
	int nLen = 0;

	for (int i = 0; i < szSend.GetLength(); i++)
	{
		if ((szSend.GetAt(i) <= 0) || (szSend.GetAt(i) > 127))
			nLen += 2;
		else
			nLen += 1;
	}

	char* Buffer = new char[nLen + 1];

	size_t CharactersConverted = 0;

	// 유니코드 -> 아스키코드 (멀티바이트 -> 싱글바이트)
	wcstombs_s(&CharactersConverted, Buffer, nLen + 1, szSend, _TRUNCATE);

	int retval = sendto(m_clientSock, reinterpret_cast<char*>(Buffer), nLen, 0, (SOCKADDR*)&m_ClientAddr, sizeof(m_ClientAddr));

	CString sz;
	sz.Format(_T("Send To Tester[%d] : %s"), m_nId, szSend);
	COMMON->Write_Main_Log(sz);

	delete[] Buffer;

	return TRUE;
}