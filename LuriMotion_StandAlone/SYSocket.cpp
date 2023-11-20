#include "pch.h"
#include "SYSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CSYSocket

CSYSocket m_ctrlOthers[LAN_COMMUNICATION_COUNT];


CSYSocket::CSYSocket(void)
{
	InitVariable(0);
}

CSYSocket::~CSYSocket(void)
{
	CloseSocket();
}

BEGIN_MESSAGE_MAP(CSYSocket, CWnd)
	//{{AFX_MSG_MAP(CSYSocket)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SOCK_RECV_MSG,(LRESULT(AFX_MSG_CALL CWnd::*)(WPARAM, LPARAM))OnSockRecvMsg)
	ON_MESSAGE(WM_SOCK_CONNECT_MSG,(LRESULT(AFX_MSG_CALL CWnd::*)(WPARAM, LPARAM)) OnSockConnectMsg)
	ON_MESSAGE(WM_SOCK_CLOSE_MSG,(LRESULT(AFX_MSG_CALL CWnd::*)(WPARAM, LPARAM)) OnSockCloseMsg)
	ON_MESSAGE(WM_SOCK_ACCEPT_MSG,(LRESULT(AFX_MSG_CALL CWnd::*)(WPARAM, LPARAM)) OnSockAcceptMsg)
	ON_WM_TIMER()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSYSocket message handlers

BOOL CSYSocket::Create(CWnd* pParentWnd) 
{
	CRect rect = CRect(0, 0, 0, 0); // 화면 영역 설정
	return CWnd::Create(NULL, _T("SYSocket"), WS_CHILD, rect, GetDesktopWindow(), 1);
}

void CSYSocket::InitVariable(int id)
{	
	m_nId					= id;

	m_nRecvPos				= 0;	
	m_nLogMode_Socket		= 0;
	m_nMode					= 0;
	m_nRecieveSetTimerFlag	= 0;
	m_nInitFlag				= 0;

	m_bRecieveCR_OK			= FALSE;	
	m_bConnected			= FALSE;	
	m_bReturn				= FALSE;
	
	m_strLastError		= _T("");
	m_strRecvData		= _T("");

	m_nLastEtxCnt = 0;

	memset(send_buf, 0x00, SOCKET_DATA_MAX_SIZE);
	memset(recv_buf, 0x00, SOCKET_DATA_MAX_SIZE);

	::ZeroMemory(szBuffer_Save, sizeof(szBuffer_Save));

}

int CSYSocket::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	m_socket.m_parent_wnd = GetSafeHwnd();
	m_socket_server.m_parent_wnd = GetSafeHwnd();
	return 0;
}

void CSYSocket::OnSockAcceptMsg(WPARAM wParam, LPARAM lParam)
{
	SOCKADDR_IN saddr;
	int nErrorCode = (int)lParam;
	CEzSock *sock = (CEzSock*)wParam;

	if(nErrorCode)
	{
		send_msg_target(FLAG_SOCKET_ERROR, nErrorCode);
	}
	else
	{
		if(m_server_accept == FALSE)
		{
#ifdef SOCKET_USE
			if(!sock->Accept(m_socket, (SOCKADDR *)&saddr) )
			{
				send_msg_target(FLAG_SOCKET_ERROR, sock->GetLastError());
			}
			else
			{
				m_server_accept = TRUE;
				send_msg_target(FLAG_SOCKET_CONNECT, nErrorCode);
			}
#else
			m_server_accept = TRUE;
			send_msg_target(FLAG_SOCKET_CONNECT, nErrorCode);
#endif
		}
		else
		{
#ifdef SOCKET_USE
			CEzSock temp_socket;
			if(sock->Accept(temp_socket, (SOCKADDR*)&saddr) )
			{
				temp_socket.Close();
			}
#endif
		}
	}
}

void CSYSocket::OnSockCloseMsg(WPARAM wParam, LPARAM lParam)
{
	int nErrorCode = (int)lParam;
	send_msg_target(FLAG_SOCKET_CLOSE, nErrorCode);
}

void CSYSocket::OnSockConnectMsg(WPARAM wParam, LPARAM lParam)
{
	int nErrorCode = (int)lParam;
	send_msg_target(FLAG_SOCKET_CONNECT, nErrorCode);
}

void CSYSocket::OnSockRecvMsg(WPARAM wParam, LPARAM lParam)
{
	CEzSock *sock = (CEzSock*)wParam;

	int recv_len = 0;
	BYTE szBuffer[50];
	::ZeroMemory(szBuffer, sizeof(szBuffer));
	

	int nEtxCheck = 0;
	CString strTemp = _T("");
	CString strMsg = _T("");
	CString strTemp2 = _T("");
	CString strExt = _T("");
	CString strData = _T("");
	CString strLog = _T("");

	CString strRecv_R = _T("");
	CString strRecv_S[5] = { _T(""), };
	int     nExtCount = 0;

	BYTE szType;
	BYTE szTesterNum;
	BYTE szTesterNum_small;
	BYTE szExtCode[3];
	BYTE szData[1024];

	int nRecvLenth;
	int nDatalenth;

	int nEtxSlotNum[10] = { 0, };
	int nMessageLength = 0;

	recv_len = sock->Receive(szBuffer, SOCKET_DATA_MAX_SIZE, 0);

	if (recv_len != SOCKET_ERROR)
	{
		if (szBuffer[recv_len - 1] == 0x03)
		{
			nEtxCheck = 1;
		}

		//수신 데이터 Log 남김
		for (int i = 0; i < recv_len; i++)
		{
			if (nEtxCheck == 0)
			{
				szBuffer_Save[i] = szBuffer[i];

				m_nLastEtxCnt = i;
			}
			else
			{
				if (m_nLastEtxCnt != 0)
				{
					szBuffer_Save[m_nLastEtxCnt + i + 1] = szBuffer[i];
					nMessageLength = m_nLastEtxCnt + recv_len + 1;
				}
			}

			if (szBuffer[i] == NULL)
				break;

			strTemp.Format(_T("%c"), szBuffer[i]);
			strMsg += strTemp;
		}

		// Etx 확인.
		if (nEtxCheck == 0)
		{
			memcpy(recv_buf + m_nRecvPos, szBuffer, sizeof(char)* recv_len);
			m_nRecvPos += recv_len;

			if (m_nRecieveSetTimerFlag == 0)
			{
				m_nRecieveSetTimerFlag = 1;
				SetTimer(TIMER_RECIEVE, 100, NULL);
			}
			return;
		}
		else
		{
			if (m_nRecieveSetTimerFlag == 1)
			{
				m_bRecieveCR_OK = TRUE;
				m_nRecieveSetTimerFlag = 0;
			}
		}

		// 통신이 끊어져 들어온경우 저장된 버퍼를 원래 버퍼로 넣어준다... 
		// 이유 : 아래 코드 다 고치기 귀찮아서.
		for (int i = 0; i < 50; i++)
		{
			szBuffer[i] = szBuffer_Save[i];
		}

		//수신 데이터 변수 저장 - 수신 된 Parameter 사용 및 기본 Log 저장 위함
		for (int i = 0; i < nMessageLength; i++)
		{                                                                                                                                                  
			strTemp.Format(_T("%c"), szBuffer[i]);
			//m_strRecvData += strTemp;
			strMsg += strTemp;
		}

		for (int i = 0; i < strMsg.GetLength(); i++)
		{
			if (strMsg.GetAt(i) == 0x03)
			{
				nExtCount++;
				nEtxSlotNum[nExtCount] = i;
			}
		}

		if (nExtCount >= 1)
		{
			for (int i = 0; i < nExtCount; i++)
			{
				AfxExtractSubString(strRecv_S[i], strMsg, i, 0x03);
			}			
		}

		for (int i = 0; i < nExtCount; i++)
		{
			// Recive Data Save Flag
			m_strReturn_Log = strRecv_S[i];
			m_bRetrun_Recv = TRUE;

			// Recive Data Parsing
			szType				= strRecv_S[i].GetAt(1);
			
			// 핸들러 to 핸들러 에서만 사용.
			if (szType == '@')
			{
				ParsingData_HandlerToHandler(strRecv_S[i]);
				return;
			}

			szTesterNum_small	= strRecv_S[i].GetAt(2);
			szTesterNum			= strRecv_S[i].GetAt(3);
			szExtCode[0]		= strRecv_S[i].GetAt(4);
			szExtCode[1]		= strRecv_S[i].GetAt(5);
			szExtCode[2]		= strRecv_S[i].GetAt(6);
			nRecvLenth			= m_strReturn_Log.GetLength() + 1;

			if (szType == 'S') // 내가 S, Recv s
				nDatalenth = nRecvLenth - 8;					// 순수 데이터 길이만 찾기 위해 8을 뺏다. 8인 이유ㅡ STX(1), 통신코드(1), 송신자(1),수신자(1),확장코드(3), ETX(1)
			if (szType == 'R') // 상대가 R, 내가 r
				nDatalenth = nRecvLenth - 10;

			if (szType == 'r') // 상대가 R, 내가 r
				break;

			// Data Cstring 으로 변환.
			if (szType == 'S')
			{
				for (int j = 0; j < nDatalenth; j++)
				{
					szData[j] = strRecv_S[i].GetAt(7+j);

					strTemp2.Format(_T("%c"), strRecv_S[i].GetAt(7 + j));
					strData += strTemp2;
				}
			}
			else if (szType == 'R')
			{
				for (int j = 0; j < nDatalenth; j++)
				{
					szData[j] = strRecv_S[i].GetAt(9 + j);

					strTemp2.Format(_T("%c"), strRecv_S[i].GetAt(9 + j));
					strData += strTemp2;
				}
			}


			CString strLogTemp = _T("");
			strLogTemp.Format(_T("[Recv] [%d Tester] %s"),m_nId, strMsg);
			//COMMON->Write_Comm_Log(strLogTemp);

					// return 반환
			if (szType == 'S')
			{
				Send_Return_s(szExtCode, szTesterNum_small);

				Request_Recv(szExtCode, szData, nDatalenth);
			}

			//if (szType == 'R' || szType == 'r' || strRecv_R != _T(""))
			if (szType == 'R' || strRecv_R != _T(""))
			{
				Send_Return(szExtCode, szTesterNum_small);		// Return 데이터 가 1 또는 0 일경우
			}

		}

		m_nLastEtxCnt = 0;
		::ZeroMemory(szBuffer_Save, sizeof(szBuffer_Save));
		//send_msg_target(FLAG_SOCKET_RECEIVE, SOCKET_READ_FC);
	}
	else
	{
		//send_msg_target(FLAG_SOCKET_ERROR, sock->GetLastError());
	}
}

void CSYSocket::ParsingData_HandlerToHandler(CString strData)
{
	m_strReciveBarcode = _T("");
	m_bConnection_Live = FALSE;
	m_bRecive_StartSignal = FALSE;
	
	
	// Sample:
	// @001,1,			// LIVE PING  무조건 2번째 파라가 1이 와야된다.
	// @002,Barcode,    // Barcode
	// @003,1,			// 1 : START, 0 : STOP

	CString strParsing[2] = { _T(""), };
	CString strRecv = _T("");
	TCHAR tchSep = _T(',');
	TCHAR tchSep1 = _T('@');
	CString strLogTemp = _T("");

	AfxExtractSubString(strRecv, strData, 1, tchSep1);

	for (int i = 0; i < 2; i++)
	{
		AfxExtractSubString(strParsing[i], strRecv, i, tchSep);
	}

	strLogTemp.Format(_T("[Recv] [%d Handler] %s"), m_nId, strRecv);
	//COMMON->Write_Comm_Log(strLogTemp);
	
	if (strParsing[0] == _T("001"))
	{
		int nTemp = 0;
		nTemp = _ttoi(strParsing[1]);

		if (nTemp == 1)
			m_bConnection_Live = TRUE;		   
	}
	else if (strParsing[0] == _T("002"))
	{
		int nCount = 0;
		if (strParsing[1].GetLength() == 8)		// 예를 들어 바코드 길이가 맞지 않으면 잘못된 데이터로 간주한다.
		{
			m_strReciveBarcode = strParsing[1];
		}
	}
	else if (strParsing[0] == _T("003"))
	{
		int nTemp = 0;
		nTemp = _ttoi(strParsing[1]);

		if (nTemp == 1)
			m_bRecive_StartSignal = TRUE;
	}
}


void CSYSocket::OnTimer(UINT_PTR nIDEvent)
{
	CWnd::OnTimer(nIDEvent);

	switch (nIDEvent)
	{
		//Receive Data 마지막이 CR 인지 확인 용 Timer
	case TIMER_RECIEVE:
		KillTimer(TIMER_RECIEVE);

		//if (COMMON->Wait_Time(RECIEVE_WAIT_TIME, &m_bRecieveCR_OK) == FALSE)
		{
			send_msg_target(FLAG_SOCKET_RECEIVE, SOCKET_READ_ERROR_FC);
		}
		break;

	default:
		break;
	}

}
void CSYSocket::CreateSocket(DWORD server_ip, UINT server_port, CString strIP, int flag)
{
#ifdef SOCKET_USE
	SOCKADDR_IN saddr;
	CString addr;
	struct tcp_keepalive keepalive = { 0 }, returned = { 0 };
	DWORD dwBytes;

	m_bReturn = FALSE;

	/* 0 - passive, 1 - active */
	keepalive.onoff = 1;
	keepalive.keepalivetime = 10000;
	keepalive.keepaliveinterval = 1000;

	
	//Client
	if (flag == 1)
	{
		saddr.sin_family = AF_INET;
		saddr.sin_addr.s_addr = htonl(server_ip);
		saddr.sin_port = htons(server_port);

		if (strIP.Left(3) == _T("127"))
		{
			if (m_socket.Create(0, SOCK_STREAM, 63L, _T("127.0.0.100")) == FALSE)
			{
				CloseSocket();
				return;
			}
		}
		else if (strIP.Left(3) == _T("192"))
		{
			if (m_socket.Create(0, SOCK_STREAM, 63L, _T("192.168.1.200")) == FALSE)
			{
				CloseSocket();
				return;
			}
		}
		WSAIoctl(m_socket.m_hSocket, SIO_KEEPALIVE_VALS, &keepalive, sizeof(struct tcp_keepalive),
			&returned, sizeof(struct tcp_keepalive), &dwBytes, NULL, NULL);

		m_socket.Connect((SOCKADDR*)&saddr, sizeof(saddr));
	}
	//Server
	else if (flag == 0)
	{
		m_server_accept = FALSE;
		m_socket_server.Create(server_port);

		WSAIoctl(m_socket_server.m_hSocket, SIO_KEEPALIVE_VALS, &keepalive, sizeof(struct tcp_keepalive),
			&returned, sizeof(struct tcp_keepalive), &dwBytes, NULL, NULL);

		if (!m_socket_server.Listen(1))
		{
			send_msg_target(FLAG_SOCKET_ERROR, m_socket_server.GetLastError());
			m_socket_server.Close();
			return;
		}
	}
#else
	send_msg_target(FLAG_SOCKET_CONNECT, 0);
#endif
}

void CSYSocket::CreateSocket(DWORD server_ip, UINT server_port, int flag)
{
#ifdef SOCKET_USE
	SOCKADDR_IN saddr;
	CString addr;
	struct tcp_keepalive keepalive = {0}, returned = {0};
	DWORD dwBytes;

	m_bReturn = FALSE;

	/* 0 - passive, 1 - active */
	keepalive.onoff				= 1;
	keepalive.keepalivetime		= 10000;
	keepalive.keepaliveinterval = 1000;

	//Client
	if(flag == 1 )
	{
		saddr.sin_family		= AF_INET;
		saddr.sin_addr.s_addr	= htonl(server_ip);
		saddr.sin_port			= htons(server_port);
		
		if(m_socket.Create(0, SOCK_STREAM, 63L, _T("127.0.0.100")) == FALSE)
		{
			CloseSocket();
			return;
		}
		WSAIoctl(m_socket.m_hSocket, SIO_KEEPALIVE_VALS, &keepalive, sizeof(struct tcp_keepalive),
					&returned, sizeof(struct tcp_keepalive), &dwBytes, NULL, NULL);

		m_socket.Connect((SOCKADDR*)&saddr, sizeof(saddr));		
	}
	//Server
	else if ( flag == 0 )
	{
		m_server_accept = FALSE;
		m_socket_server.Create(server_port);

		WSAIoctl(m_socket_server.m_hSocket, SIO_KEEPALIVE_VALS, &keepalive, sizeof(struct tcp_keepalive),
					&returned, sizeof(struct tcp_keepalive), &dwBytes, NULL, NULL);

		if(!m_socket_server.Listen(1) )
		{
			send_msg_target(FLAG_SOCKET_ERROR, m_socket_server.GetLastError());
			m_socket_server.Close();
			return;
		}
	}
#else
	send_msg_target(FLAG_SOCKET_CONNECT, 0);
#endif

}

void CSYSocket::CloseSocket()
{
	m_bConnected = FALSE;
	//pDataManager->m_bRFIDConnected = false;			//확인사항

	m_bReturn = FALSE;

#ifdef SOCKET_USE
	m_socket.Close();
	m_socket_server.Close();
#endif
}

void CSYSocket::send_msg_target(int flag, int code)
{
	CString strLog = _T("");

	switch(flag)
	{
	case FLAG_SOCKET_CONNECT:
		if(code == 0)
		{
			m_bConnected = TRUE;
			//COMMON->Write_Comm_Log(_T("Connected Sysocket"));
		}
		else
		{
			CloseSocket();
		}
		break;

	case FLAG_SOCKET_CLOSE:

		CloseSocket();

		break;

	case FLAG_SOCKET_RECEIVE:
		switch (code)
		{
		case SOCKET_READ_FC:
			if (m_nLogMode_Socket == 0)
				//COMMON->Write_Comm_Log(_T("READ_FC"));
			break;

		case SOCKET_READ_ERROR_FC:
			if (m_nLogMode_Socket == 0)
				//COMMON->Write_Comm_Log(_T("READ FC"));
			//else
				//COMMON->Write_Comm_Log(_T("READ FC ERROR"));

			break;
		}
		break;

	case FLAG_SOCKET_ERROR:
		if(code)
		{
			CloseSocket();
		}
		break;
	}

	m_nRecvPos = 0;
	m_nRecieveSetTimerFlag = 0;
	m_bRecieveCR_OK = FALSE;
		
	m_bReturn = TRUE;
	m_nMode = 0;
}


bool CSYSocket::Send_Command(CString strMsg)
{
	int nLength = 0;
	CString strTemp = _T("");
	CString strLog = _T("");
	_u8 send_buf[SOCKET_DATA_MAX_SIZE];

	///////////////////////////////////////////////////////////////////////////////
	
#ifdef SOCKET_NOT_USE
	return true;
#endif

	m_nMode = 0;
	m_bReturn = FALSE;	
	m_strRecvData = _T("");
	m_strLastError = _T("");	
	memset(send_buf, 0x00, sizeof(char) * SOCKET_DATA_MAX_SIZE);

	nLength = strMsg.GetLength();
	for(int i = 0; i < nLength; i++)
	{
		if(i == nLength - 1)
		{
			strTemp.Format(_T("%02x0D\n"), strMsg.GetAt(i));
			send_buf[i] = (BYTE)strMsg.GetAt(i);
			send_buf[i + 1] = 0x0D;
		}
		else
		{			
			strTemp.Format(_T("%02x"), strMsg.GetAt(i));
			send_buf[i] = (BYTE)strMsg.GetAt(i);
		}		

		strLog += strTemp;
	}
	strLog.MakeUpper();	

	
#ifdef SOCKET_USE
	if((m_socket.Send(send_buf, SOCKET_DATA_MAX_SIZE)) == SOCKET_ERROR)
	{
		m_strLastError.Format(_T("[SOCKET] FLAG_SOCKET_ERROR_%d [ TESTER : %d ]"), m_socket.GetLastError(), m_nId);

		m_bReturn = FALSE;
		m_strRecvData = _T("");
		m_nMode = 0;

		return false;
	}
#endif
	
	//Socket 통신 데이터 로그	

	NoatepadDebug(strLog);

	return true;
}

void CSYSocket::NoatepadDebug( __in LPCTSTR szDebugString )
{
	SYSTEMTIME _starttime;
	GetLocalTime(&_starttime);

	TCHAR szTime[MAX_PATH] = { 0, };

	_stprintf_s(szTime, MAX_PATH, _T("[%02d:%02d:%02d:%04d] ")
		, _starttime.wHour
		, _starttime.wMinute
		, _starttime.wSecond
		, _starttime.wMilliseconds);
	//  시스템 죽지않는 현상 해결 방법...............
	HWND	hNotePad(NULL);
	hNotePad = ::FindWindow(NULL, _T("notepaddebug.txt - Notepad"));
	if (hNotePad == NULL)
	{
		hNotePad = ::FindWindow(NULL, _T("notepaddebug - Notepad"));
		if (hNotePad == NULL)
		{
			hNotePad = ::FindWindow(NULL, _T("notepaddebug.txt - 메모장"));
			if (hNotePad == NULL)
				hNotePad = ::FindWindow(NULL, _T("notepaddebug - 메모장"));
		}
	}

	if (hNotePad != NULL)
	{
		HWND hContext = ::GetWindow(hNotePad, GW_CHILD);
		if (hContext)
		{
			LRESULT i = ::SendMessage(hContext, WM_GETTEXTLENGTH, 0, 0);
			::SendMessage(hContext, EM_SETSEL, i, i);
			::SendMessage(hContext, EM_REPLACESEL, 0, (LPARAM)(LPCTSTR)szTime);
			LRESULT j = ::SendMessage(hContext, WM_GETTEXTLENGTH, 0, 0);
			::SendMessage(hContext, EM_SETSEL, j, j);
			::SendMessage(hContext, EM_REPLACESEL, 0, (LPARAM)szDebugString);
			LRESULT k = ::SendMessage(hContext, WM_GETTEXTLENGTH, 0, 0);
			::SendMessage(hContext, EM_SETSEL, k, k);
		}
	}
}
bool CSYSocket::Send_Return_s(BYTE *byExt, BYTE byTesterNum)
{
	CString strTemp = _T("");
	CString strMsg = _T("");
	int		nReturnLength = 0;
	BOOL	bRet = FALSE;

	BYTE szBuffer[100];
	::ZeroMemory(szBuffer, sizeof(szBuffer));

	szBuffer[0] = 0x02;
	szBuffer[1] = 's';
	szBuffer[2] = 'H';
	szBuffer[3] = byTesterNum;
	szBuffer[4] = byExt[0];
	szBuffer[5] = byExt[1];
	szBuffer[6] = byExt[2];
	szBuffer[7] = '1';			// 0 fail, 1 ok
	szBuffer[8] = 0x03;


	for (int i = 0; i < 9; i++)
	{
		strTemp.Format(_T("%c"), szBuffer[i]);
		strMsg += strTemp;
	}


#ifdef SOCKET_USE
	if ((m_socket.Send(szBuffer, SOCKET_DATA_MAX_SIZE)) == SOCKET_ERROR)
	{
		m_bReturn = FALSE;
		m_strRecvData = _T("");
		m_nMode = 0;

		return false;
	}
#endif

	strTemp.Format(_T("[%d Tester] "), m_nId);
	strTemp = strTemp + strMsg;
	//COMMON->Write_Comm_Log(strTemp);
	return bRet;
}

bool CSYSocket::Send_Return(BYTE *byExt, BYTE byTesterNum)
{
	// 검사기에서 'R' 로 요청이 왔을때 Return 해야한다.
	CString strTemp = _T("");
	CString strMsg = _T("");
	int		nReturnLength = 0;
	BOOL	bRet = FALSE;

	BYTE szBuffer[100];
	::ZeroMemory(szBuffer, sizeof(szBuffer));

	szBuffer[0] = 0x02;
	szBuffer[1] = 'r';
	szBuffer[2] = 'H';
	szBuffer[3] = byTesterNum;
	szBuffer[4] = byExt[0];
	szBuffer[5] = byExt[1];
	szBuffer[6] = byExt[2];
	szBuffer[7] = '1';
	szBuffer[8] = 0x03;


	for (int i = 0; i < 9; i++)
	{
		strTemp.Format(_T("%c"), szBuffer[i]);
		strMsg += strTemp;
	}

#ifdef SOCKET_USE
	if ((m_socket.Send(szBuffer, SOCKET_DATA_MAX_SIZE)) == SOCKET_ERROR)
	{
		m_bReturn = FALSE;
		m_strRecvData = _T("");
		m_nMode = 0;

		return false;
	}
#endif
	//COMMON->Write_Comm_Log(strMsg);
	return bRet;
}

bool CSYSocket::Send_Return_LongData(BYTE *byExt, BYTE *byData, int nDataLength, BYTE byTesterNum)
{
	// 검사기에서 'R' 로 요청이 왔을때 Return 해야한다.
	CString strTemp = _T("");
	CString strMsg = _T("");
	int		nReturnLength = 0;
	BOOL	bRet = FALSE;

	BYTE szBuffer[100];
	::ZeroMemory(szBuffer, sizeof(szBuffer));

	szBuffer[0] = 0x02;
	szBuffer[1] = 'r';
	szBuffer[2] = 'H';
	szBuffer[3] = byTesterNum;
	szBuffer[4] = byExt[0];
	szBuffer[5] = byExt[1];
	szBuffer[6] = byExt[2];

	for (int i = 0; i < nDataLength; i++)
	{
		szBuffer[7 + i] = byData[i];
	}

	szBuffer[7 + nDataLength] = 0x03;


	for (int i = 0; i < 8 + nDataLength; i++)
	{
		strTemp.Format(_T("%c"), szBuffer[i]);
		strMsg += strTemp;
	}

#ifdef SOCKET_USE
	if ((m_socket.Send(szBuffer, SOCKET_DATA_MAX_SIZE)) == SOCKET_ERROR)
	{
		m_bReturn = FALSE;
		m_strRecvData = _T("");
		m_nMode = 0;

		return false;
	}
#endif
	//COMMON->Write_Comm_Log(strMsg);

	return bRet;
}

bool CSYSocket::Request_Recv(BYTE *byExt, BYTE *byData, int nLength)
{
	bool bRet = FALSE;
	CString strLog = _T("");

	if (byExt[0] == 'S')
	{
		if (byData[0] == '1')
		{
			// show_msg_box 검사 가능 창 띄어주기

			m_bFlag_Status = TRUE;
			bRet = TRUE;
			//COMMON->Write_Comm_Log(_T("Recv S00 1"));
		}
		else
		{
			m_strTester_ReturnFail.Format(_T("%c"), byData[0]);
			// 검사기 프로그램 확인 요청 and 장비 센서류 확인.
			m_bFlag_Status = FALSE;
			strLog.Format(_T("Recv S00 [%c]. Status is not Ready."), byData[0]);
			//COMMON->Write_Comm_Log(strLog);
		}
	}
	else if (byExt[0] == 'C')
	{
		if (byExt[2] == '1')
		{
			m_strTester_ReturnFail.Format(_T("%c"), byData[0]);

			if (byData[0] == 1)
			{

				m_bFlag_Tester_Start = TRUE;
				bRet = TRUE;
			}
			else
			{
				strLog.Format(_T("Recv S00 [%c]. Status is not Ready."), byData[0]);
				//COMMON->Write_Comm_Log( strLog);

				m_bFlag_Tester_Start = FALSE;
				bRet = TRUE;
			}
		} 
		else if (byExt[2] == '2')
		{
			if (byData[0] == 1)
				m_bFlag_Tester_Stop = TRUE;

			bRet = TRUE;
		}
		else if (byExt[2] == '3')
		{
			m_bFlag_Tester_Result = TRUE;

			CString strTest = _T("");
			strTest = char(byData[0]);

			if (_ttoi(strTest) == 1)
				m_strTester_Result = _T("OK");
			else
				m_strTester_Result = _T("NG");

			bRet = TRUE;
		}
		else if (byExt[2] == '4')
		{
			if (byData[0] == 1)
				m_bFlag_Emg = TRUE;

			bRet = TRUE;
		}
		else if (byExt[2] == '5')
		{
			if (byData[0] == 1)
				m_bFlag_Origin = TRUE;

			bRet = TRUE;
		}
	}
	else if (byExt[0] == 'L')
	{
		if (byExt[1] == 'C')
		{
			// if( byData[0] == 1 ) 
			m_bFlag_ChartLight = TRUE;
			bRet = TRUE;
		}
		else if (byExt[1] == 'B')
		{
			// if( byData[0] == 1 ) 
			m_bFlag_BlemishLight = TRUE;
			bRet = TRUE;
		}
		else if (byExt[1] == '0')
		{
			if (byExt[2] == '0')
			{
				// if( byData[0] == 1 ) 
				// LOT 시작
				m_bFlag_Lot_Start = TRUE;
				bRet = TRUE;
			}
			else if (byExt[2] == '1')
			{
				// if( byData[0] == 1 ) 
				// LOT 종료
				m_bFlag_Lot_Stop = TRUE;
				bRet = TRUE;
			}
			else if (byExt[2] == '2')
			{
				// if( byData[0] == 1 ) 
				// LOT Info
				m_bFlag_Lot_Info = TRUE;
				bRet = TRUE;
			}
		}
	}
	else if (byExt[0] == 'M')
	{
		if (byExt[2] == '0')
		{
			if (SEQUENCE->GetRunMode() == eStop || SEQUENCE->GetRunMode() == ePause)
				return FALSE;
			else //if (//COMMON->m_bAutoRun == FALSE)
				//bRet = FALSE;
			//else
			{ 
				CString strTest = _T("");

				strTest = char(byData[0]);
				strTest += char(byData[1]);					

				// 1 : Stage Move
				// 10 : 90도 각도 Move
				// Vabration A
				m_nData_MoveStage_TeachingPosition = _ttoi(strTest);
				//COMMON->m_bRecvMotorMove[m_nId] = m_nData_MoveStage_TeachingPosition;
					
				m_bFlag_Move_Stage = TRUE;

				strLog.Format(_T("[Recv] TestNum[%d] TeachingNum[%s]"), m_nId, strTest);

				if( m_nId < 4)
					//COMMON->Write_Comm_Log(strLog);

				bRet = TRUE;
			}
		}
		else if (byExt[2] == '1' || byExt[2] == '2')
		{
			/*
			if (COMMON->m_bAutoRun == FALSE)
				return FALSE;
				*/

			BYTE szData[1024];
			int	nCount = 0;
			int nPos[12] = { 0, };
			CString strTemp = _T("");
			CString strTemp2 = _T("");

			// char -> CString
			for (int i = 0; i < nLength; i++)
			{
				szData[i] = byData[i];

				strTemp2.Format(_T("%c"), byData[i]);
				strTemp += strTemp2;
			}

			// CString 끝에 ';'를 붙인다.
			strTemp += ';';

			// ';'위치를 찾는다.
			for (int i = 0; i < strTemp.GetLength(); i++)
			{
				if (strTemp.GetAt(i) == ';')
				{
					nPos[nCount] = i;
					nCount++;
				}
			}

			// 배열 초기화. 
			CString strParsing_Num[MOTOR_MAX_COUNT] = { _T(""), };
			CString strParsing_Dist[MOTOR_MAX_COUNT] = { _T(""), };
			TCHAR tchSep = _T(';');

			// ';' 2개를 한 묶음으로 분리를 하자.
			// 모터번호;이동량;모터번호;이동량 으로 수신되기 때문.
			for (int j = 0; j <= nCount; j += 2)
			{
				// 모터번호
				AfxExtractSubString(strParsing_Num[j / 2], strTemp, j, tchSep);
				// 이동량 (단위:마이크로)
				AfxExtractSubString(strParsing_Dist[j / 2], strTemp, j + 1, tchSep);
			}

			// ';' 전체 갯수에서 2로 나누어야 버퍼의 갯수와 맞게 된다.
			int nEnd = (nCount / 2);
			if (nEnd == 0) nEnd = 1;

			// 모터와 이동량을 실제 사용할 버퍼에 저장시키자.
			for (int i = 0; i < nEnd; i++)
			{
				if (strParsing_Num[i] == _T(""))
				{
					m_nData_Motor[i] = -1;
					m_nData_MoveDistance[i] = 0;
				}
				else
				{
					m_nData_Motor[i] = _ttoi(strParsing_Num[i]);
					m_nData_MoveDistance[i] = _ttoi(strParsing_Dist[i]);
				}
			}


			if (byExt[2] == '1')
			{
				// 테스터연결이 싱글모드일 경우.
				m_bFlag_Move_Motor = TRUE;
			}
			else if (byExt[2] == '2')
			{
				// 테스터연결이 듀얼모드일 경우.
				m_bFlag_Move_Motor_Abs = TRUE;
			}

			bRet = TRUE;

		}
	}
	else if (byExt[0] == 'V')
	{
		if (byExt[2] == '0')
		{
			/*
			if (COMMON->m_bAutoRun == FALSE)
				return FALSE;
			*/

			BYTE szData[1024];
			int	nCount = 0;
			int nPos[12] = { 0, };
			CString strTemp = _T("");
			CString strTemp2 = _T("");

			// char -> CString
			for (int i = 0; i < nLength; i++)
			{
				szData[i] = byData[i];

				strTemp2.Format(_T("%c"), byData[i]);
				strTemp += strTemp2;
			}

			// CString 끝에 ';'를 붙인다.
			strTemp += ';';

			// ';'위치를 찾는다.
			for (int i = 0; i < strTemp.GetLength(); i++)
			{
				if (strTemp.GetAt(i) == ';')
				{
					nPos[nCount] = i;
					nCount++;
				}
			}

			// 배열 초기화. 
			CString strAngle = _T("");
			CString strHz = _T("");
			TCHAR tchSep = _T(';');

			// ';' 2개를 한 묶음으로 분리를 하자.
			// 각도/hz
			for (int j = 0; j < nCount; j += 2)
			{
				// 각도
				AfxExtractSubString(strHz, strTemp, j, tchSep);
				// 속도
				AfxExtractSubString(strAngle, strTemp, j + 1, tchSep);
			}

			m_bVaibration_Run = TRUE;
			m_nVaibration_Hz = _ttoi(strHz);
			m_nVaibration_Angle = (_ttoi(strAngle) / 10 );
		}
		else if (byExt[2] == '1')
		{
			// 테스터연결이 듀얼모드일 경우.
			m_bVaibration_Stop = TRUE;
		}
	}
	else if (byExt[0] == 'I')
	{
		BYTE szData[1024];
		int	nCount = 0;
		int nPos[12] = { 0, };
		CString strTemp = _T("");
		CString strTemp2 = _T("");

		// char -> CString
		for (int i = 0; i < nLength; i++)
		{
			szData[i] = byData[i];

			strTemp2.Format(_T("%c"), byData[i]);
			strTemp += strTemp2;
		}

		// CString 끝에 ';'를 붙인다.
		strTemp += ';';

		// ';'위치를 찾는다.
		for (int i = 0; i < strTemp.GetLength(); i++)
		{
			if (strTemp.GetAt(i) == ';')
			{
				nCount++;
			}
		}

		m_bLightChangeCommandRecv = TRUE;

		// 배열 초기화. 
		CString strParsing_LightNum[MOTOR_MAX_COUNT] = { _T(""), };
		CString strParsing_LightValue[MOTOR_MAX_COUNT] = { _T(""), };
		TCHAR tchSep = _T(';');

		// ';' 2개를 한 묶음으로 분리를 하자.
		// 모터번호;이동량;모터번호;이동량 으로 수신되기 때문.
		for (int j = 0; j <= nCount; j += 2)
		{
			// 조명번호
			AfxExtractSubString(strParsing_LightNum[j / 2], strTemp, j, tchSep);
			// 조명레벨?
			AfxExtractSubString(strParsing_LightValue[j / 2], strTemp, j + 1, tchSep);
		}

		// ';' 전체 갯수에서 2로 나누어야 버퍼의 갯수와 맞게 된다.
		int nEnd = (nCount / 2);
		if (nEnd == 0) nEnd = 1;

		// 모터와 이동량을 실제 사용할 버퍼에 저장시키자.
		for (int i = 0; i < nEnd; i++)
		{
			if (strParsing_LightNum[i] == _T(""))
			{
				m_nData_LightNum[i] = -1;
				m_nData_LightValue[i] = 0;
			}
			else
			{
				m_nData_LightNum[i] = _ttoi(strParsing_LightNum[i]);
				m_nData_LightValue[i] = _ttoi(strParsing_LightValue[i]);
			}
		}
	}
	return bRet;
}

void CSYSocket::Reset_LightController_FlagData()
{
	m_bLightChangeCommandRecv = FALSE;
	for (int i = 0; i < 5; i++)
	{
		m_nData_LightNum[i] = -1;
		m_nData_LightValue[i] = 0;
	}
}

void CSYSocket::Reset_MotorMove_FlagData()
{
	m_bFlag_Move_Stage = FALSE;
	m_bFlag_Move_Motor = FALSE;
	m_bFlag_Move_Motor_Abs = FALSE;

	for (int i = 0; i < (int)MOTOR_MAX_COUNT; i++)
	{
		m_nData_Motor[MOTOR_MAX_COUNT] = -1;
		m_nData_MoveDistance[MOTOR_MAX_COUNT] = 0;
	}
}

void CSYSocket::Reset_FlagData()
{
	m_bFlag_Status = FALSE;
	m_bFlag_Tester_Start = FALSE;
	m_bFlag_Tester_Stop = FALSE;
	m_bFlag_Tester_Result = FALSE;
	m_bFlag_Emg = FALSE;

	m_bFlag_Origin = FALSE;
	m_bFlag_Lot_Start = FALSE;
	m_bFlag_Lot_Stop = FALSE;
	m_bFlag_Lot_Info = FALSE;

	m_bFlag_Move_Stage = FALSE;
	m_bFlag_Move_Motor = FALSE;
	m_bFlag_Move_Motor_Abs = FALSE;

	m_bFlag_ChartLight = FALSE;
	m_bFlag_BlemishLight = FALSE;

	m_nData_MoveStage_TeachingPosition = 0;
	m_nData_MoveStage_TeachingPositionBK = 100000;

	m_strTester_Result = _T("");
	m_strTester_ReturnFail = _T("");

	for (int i = 0; i < (int)MOTOR_MAX_COUNT; i++)
	{
		m_nData_Motor[MOTOR_MAX_COUNT] = -1;
		m_nData_MoveDistance[MOTOR_MAX_COUNT] = 0;
	}

	m_bLightChangeCommandRecv = FALSE;
	for (int i = 0; i < 5; i++)
	{
		m_nData_LightNum[i] = -1;
		m_nData_LightValue[i] = 0;
	}

	m_bVaibration_Run = FALSE;
	m_bVaibration_Stop = FALSE;
	m_nVaibration_Angle = 0;
	m_nVaibration_Hz = 0;
}

//=================================================================================================
// Response
bool CSYSocket::Response_Vaibration_Stop(int nPos)
{
	// 검사기로 검사시작 요청시 사용.
	CString strTemp = _T("");
	CString strMsg = _T("");
	int		nReturnLength = 0;
	BOOL	bRet = FALSE;

	BYTE szBuffer[100];
	::ZeroMemory(szBuffer, sizeof(szBuffer));


	szBuffer[0] = 0x02;
	szBuffer[1] = 'R';
	szBuffer[2] = 'H';

	strTemp.Format(_T("%d"), nPos);

	szBuffer[3] = strTemp.GetAt(0);

	szBuffer[4] = 'V';
	szBuffer[5] = '0';
	szBuffer[6] = '1';
	szBuffer[7] = '1';
	szBuffer[8] = 0x03;


	for (int i = 0; i < 9; i++)
	{
		strTemp.Format(_T("%c"), szBuffer[i]);
		strMsg += strTemp;
	}

#ifdef SOCKET_USE
	if ((m_socket.Send(szBuffer, SOCKET_DATA_MAX_SIZE)) == SOCKET_ERROR)
	{

		m_bReturn = FALSE;
		m_strRecvData = _T("");
		m_nMode = 0;

		return false;
	}
#endif
	//COMMON->Write_Comm_Log(strMsg);
	return bRet;
}

bool CSYSocket::Response_Vaibration(int nPos)
{
	// 검사기로 검사시작 요청시 사용.
	CString strTemp = _T("");
	CString strMsg = _T("");
	int		nReturnLength = 0;
	BOOL	bRet = FALSE;

	BYTE szBuffer[100];
	::ZeroMemory(szBuffer, sizeof(szBuffer));

	szBuffer[0] = 0x02;
	szBuffer[1] = 'R';
	szBuffer[2] = 'H';

	strTemp.Format(_T("%d"), nPos);

	szBuffer[3] = strTemp.GetAt(0);

	szBuffer[4] = 'V';
	szBuffer[5] = '0';
	szBuffer[6] = '0';
	szBuffer[7] = '1';
	szBuffer[8] = 0x03;


	for (int i = 0; i < 9; i++)
	{
		strTemp.Format(_T("%c"), szBuffer[i]);
		strMsg += strTemp;
	}

#ifdef SOCKET_USE
	if ((m_socket.Send(szBuffer, SOCKET_DATA_MAX_SIZE)) == SOCKET_ERROR)
	{

		m_bReturn = FALSE;
		m_strRecvData = _T("");
		m_nMode = 0;

		return false;
	}
#endif
	//COMMON->Write_Comm_Log(strMsg);
	return bRet;
}

bool CSYSocket::Response_StageMove(int nPos)
{
	// 검사기로 검사시작 요청시 사용.
	CString strTemp = _T("");
	CString strMsg = _T("");
	int		nReturnLength = 0;
	BOOL	bRet = FALSE;

	BYTE szBuffer[100];
	::ZeroMemory(szBuffer, sizeof(szBuffer));


	szBuffer[0] = 0x02;
	szBuffer[1] = 'R';
	szBuffer[2] = 'H';

	strTemp.Format(_T("%d"), nPos);

	szBuffer[3] = strTemp.GetAt(0);

	szBuffer[4] = 'M';
	szBuffer[5] = '0';
	szBuffer[6] = '0';
	szBuffer[7] = '1';
	szBuffer[8] = 0x03;


	for (int i = 0; i < 9; i++)
	{
		strTemp.Format(_T("%c"), szBuffer[i]);
		strMsg += strTemp;
	}

#ifdef SOCKET_USE
	if ((m_socket.Send(szBuffer, SOCKET_DATA_MAX_SIZE)) == SOCKET_ERROR)
	{

		m_bReturn = FALSE;
		m_strRecvData = _T("");
		m_nMode = 0;

		return false;
	}
#endif
	//COMMON->Write_Comm_Log(strMsg);
	return bRet;
}

bool CSYSocket::Response_MotorMove_Abs(int nPos)
{
	// 검사기로 검사시작 요청시 사용.
	CString strTemp = _T("");
	CString strMsg = _T("");
	int		nReturnLength = 0;
	BOOL	bRet = FALSE;

	BYTE szBuffer[100];
	::ZeroMemory(szBuffer, sizeof(szBuffer));


	szBuffer[0] = 0x02;
	szBuffer[1] = 'R';
	szBuffer[2] = 'H';
	if (nPos == 0)
		szBuffer[3] = '0';
	else if (nPos == 1)
		szBuffer[3] = '1';
	else if (nPos == 2)
		szBuffer[3] = '2';
	szBuffer[4] = 'M';
	szBuffer[5] = '0';
	szBuffer[6] = '2';
	szBuffer[7] = '1';
	szBuffer[8] = 0x03;


	for (int i = 0; i < 9; i++)
	{
		strTemp.Format(_T("%c"), szBuffer[i]);
		strMsg += strTemp;
	}

#ifdef SOCKET_USE
	if ((m_socket.Send(szBuffer, SOCKET_DATA_MAX_SIZE)) == SOCKET_ERROR)
	{

		m_bReturn = FALSE;
		m_strRecvData = _T("");
		m_nMode = 0;

		return false;
	}
#endif
	//COMMON->Write_Comm_Log(strMsg);
	return bRet;
}

bool CSYSocket::Response_MotorMove_Rel(int nPos)
{
	// 검사기로 검사시작 요청시 사용.
	CString strTemp = _T("");
	CString strMsg = _T("");
	int		nReturnLength = 0;
	BOOL	bRet = FALSE;

	BYTE szBuffer[100];
	::ZeroMemory(szBuffer, sizeof(szBuffer));


	szBuffer[0] = 0x02;
	szBuffer[1] = 'R';
	szBuffer[2] = 'H';
	if (nPos == 0)
		szBuffer[3] = '0';
	else if (nPos == 1)
		szBuffer[3] = '1';
	else if (nPos == 2)
		szBuffer[3] = '2';



	szBuffer[4] = 'M';
	szBuffer[5] = '0';
	szBuffer[6] = '1';
	szBuffer[7] = '1';
	szBuffer[8] = 0x03;


	for (int i = 0; i < 9; i++)
	{
		strTemp.Format(_T("%c"), szBuffer[i]);
		strMsg += strTemp;
	}

#ifdef SOCKET_USE
	if ((m_socket.Send(szBuffer, SOCKET_DATA_MAX_SIZE)) == SOCKET_ERROR)
	{

		m_bReturn = FALSE;
		m_strRecvData = _T("");
		m_nMode = 0;

		return false;
	}
#endif
	//COMMON->Write_Comm_Log(strMsg);
	return bRet;
}
bool CSYSocket::Response_LightController(int nTester)
{
	// 검사기로 검사시작 요청시 사용.
	CString strTemp = _T("");
	CString strMsg = _T("");
	int		nReturnLength = 0;
	BOOL	bRet = FALSE;

	BYTE szBuffer[100];
	::ZeroMemory(szBuffer, sizeof(szBuffer));

	szBuffer[0] = 0x02;
	szBuffer[1] = 'R';
	szBuffer[2] = 'H';
	szBuffer[3] = '0';
	szBuffer[4] = 'I';
	szBuffer[5] = '0';
	szBuffer[6] = '0';
	szBuffer[7] = '1';
	szBuffer[8] = 0x03;

	for (int i = 0; i < 9; i++)
	{
		strTemp.Format(_T("%c"), szBuffer[i]);
		strMsg += strTemp;
	}


#ifdef SOCKET_USE
	if ((m_socket.Send(szBuffer, SOCKET_DATA_MAX_SIZE)) == SOCKET_ERROR)
	{

		m_bReturn = FALSE;
		m_strRecvData = _T("");
		m_nMode = 0;

		return false;
	}
#endif
	//COMMON->Write_Comm_Log(strMsg);
	return bRet;
}

//=======================================================================================
bool CSYSocket::Send_Status(int nTesterNum)
{
	// 검사기로 검사시작 요청시 사용.
	CString strTemp = _T("");
	CString strMsg = _T("");
	int		nReturnLength = 0;
	BOOL	bRet = FALSE;

	BYTE szBuffer[100];
	::ZeroMemory(szBuffer, sizeof(szBuffer));


	szBuffer[0] = 0x02;
	szBuffer[1] = 'S';
	szBuffer[2] = 'H';
	szBuffer[3] = 0x30 + nTesterNum;
	szBuffer[4] = 'S';
	szBuffer[5] = '0';
	szBuffer[6] = '0';
	szBuffer[7] = 0x03;

	for (int i = 0; i < 9; i++)
	{
		strTemp.Format(_T("%c"), szBuffer[i]);
		strMsg += strTemp;
	}

#ifdef SOCKET_USE
	if ((m_socket.Send(szBuffer, SOCKET_DATA_MAX_SIZE)) == SOCKET_ERROR)
	{

		m_bReturn = FALSE;
		m_strRecvData = _T("");
		m_nMode = 0;

		return false;
	}
#endif
	//COMMON->Write_Comm_Log(strMsg);
	return bRet;
}

bool CSYSocket::Send_Start_NoBarcode(int nTesterNum)
{
	// 검사기로 검사시작 요청시 사용.
	CString strTemp = _T("");
	CString strMsg = _T("");
	int		nReturnLength = 0;
	BOOL	bRet = FALSE;

	BYTE szBuffer[100];
	::ZeroMemory(szBuffer, sizeof(szBuffer));


	szBuffer[0] = 0x02;
	szBuffer[1] = 'S';
	szBuffer[2] = 'H';
	szBuffer[3] = 0x30 + nTesterNum;
	szBuffer[4] = 'C';
	szBuffer[5] = '0';
	szBuffer[6] = '1';
	szBuffer[7] = 0x03;

	for (int i = 0; i < 9; i++)
	{
		strTemp.Format(_T("%c"), szBuffer[i]);
		strMsg += strTemp;
	}

#ifdef SOCKET_USE
	if ((m_socket.Send(szBuffer, SOCKET_DATA_MAX_SIZE)) == SOCKET_ERROR)
	{

		m_bReturn = FALSE;
		m_strRecvData = _T("");
		m_nMode = 0;

		return false;
	}
#endif
	//COMMON->Write_Comm_Log(strMsg);
	return bRet;
}

bool CSYSocket::Send_Start(int nTesterNum, CString strBarcode)
{
	// 검사기로 검사시작 요청시 사용.
	CString strTemp = _T("");
	CString strMsg = _T("");
	int		nReturnLength = 0;
	BOOL	bRet = FALSE;

	BYTE szBuffer[100];
	::ZeroMemory(szBuffer, sizeof(szBuffer));


	szBuffer[0] = 0x02;
	szBuffer[1] = 'S';
	szBuffer[2] = 'H';
	szBuffer[3] = 0x30 + nTesterNum;
	szBuffer[4] = 'C';
	szBuffer[5] = '0';
	szBuffer[6] = '1';

	for (int i = 0; i < strBarcode.GetLength(); i++)
	{
		szBuffer[7 + i] = strBarcode.GetAt(i);
	}

	szBuffer[7 + strBarcode.GetLength()] = 0x03;

	for (int i = 0; i < 8 + strBarcode.GetLength(); i++)
	{
		strTemp.Format(_T("%c"), szBuffer[i]);
		strMsg += strTemp;
	}

#ifdef SOCKET_USE
	if ((m_socket.Send(szBuffer, SOCKET_DATA_MAX_SIZE)) == SOCKET_ERROR)
	{

		m_bReturn = FALSE;
		m_strRecvData = _T("");
		m_nMode = 0;

		return false;
	}
#endif
	//COMMON->Write_Comm_Log(strMsg);
	return bRet;

}
bool CSYSocket::Send_Stop(int nTesterNum)
{
	// 검사기로 검사시작 요청시 사용.
	CString strTemp = _T("");
	CString strMsg = _T("");
	int		nReturnLength = 0;
	BOOL	bRet = FALSE;

	BYTE szBuffer[100];
	::ZeroMemory(szBuffer, sizeof(szBuffer));


	szBuffer[0] = 0x02;
	szBuffer[1] = 'S';
	szBuffer[2] = 'H';
	szBuffer[3] = 0x30 + nTesterNum;
	szBuffer[4] = 'C';
	szBuffer[5] = '0';
	szBuffer[6] = '2';
	szBuffer[7] = 0x03;

	for (int i = 0; i < 9; i++)
	{
		strTemp.Format(_T("%c"), szBuffer[i]);
		strMsg += strTemp;
	}

#ifdef SOCKET_USE
	if ((m_socket.Send(szBuffer, SOCKET_DATA_MAX_SIZE)) == SOCKET_ERROR)
	{

		m_bReturn = FALSE;
		m_strRecvData = _T("");
		m_nMode = 0;

		return false;
	}
#endif
	//COMMON->Write_Comm_Log(strMsg);
	return bRet;
}
bool CSYSocket::Send_Result(int nTesterNum)
{
	// 검사기로 검사시작 요청시 사용.
	CString strTemp = _T("");
	CString strMsg = _T("");
	int		nReturnLength = 0;
	BOOL	bRet = FALSE;

	BYTE szBuffer[100];
	::ZeroMemory(szBuffer, sizeof(szBuffer));


	szBuffer[0] = 0x02;
	szBuffer[1] = 'S';
	szBuffer[2] = 0x30 + nTesterNum;
	szBuffer[3] = 'H';
	szBuffer[4] = 'C';
	szBuffer[5] = '0';
	szBuffer[6] = '3';
	szBuffer[7] = '1';
	szBuffer[8] = 0x03;

	for (int i = 0; i < 9; i++)
	{
		strTemp.Format(_T("%c"), szBuffer[i]);
		strMsg += strTemp;
	}

#ifdef SOCKET_USE
	if ((m_socket.Send(szBuffer, SOCKET_DATA_MAX_SIZE)) == SOCKET_ERROR)
	{

		m_bReturn = FALSE;
		m_strRecvData = _T("");
		m_nMode = 0;

		return false;
	}
#endif
	//COMMON->Write_Comm_Log(strMsg);
	return bRet;
}

bool CSYSocket::Send_EmgStop(int nTesterNum)
{
	// 검사기로 검사시작 요청시 사용.
	CString strTemp = _T("");
	CString strMsg = _T("");
	int		nReturnLength = 0;
	BOOL	bRet = FALSE;

	BYTE szBuffer[100];
	::ZeroMemory(szBuffer, sizeof(szBuffer));


	szBuffer[0] = 0x02;
	szBuffer[1] = 'S';
	szBuffer[2] = 'H';
	szBuffer[3] = 0x30 + nTesterNum;
	szBuffer[4] = 'C';
	szBuffer[5] = '0';
	szBuffer[6] = '4';
	szBuffer[7] = '0';
	szBuffer[8] = 0x03;

	for (int i = 0; i < 9; i++)
	{
		strTemp.Format(_T("%c"), szBuffer[i]);
		strMsg += strTemp;
	}

#ifdef SOCKET_USE
	if ((m_socket.Send(szBuffer, SOCKET_DATA_MAX_SIZE)) == SOCKET_ERROR)
	{

		m_bReturn = FALSE;
		m_strRecvData = _T("");
		m_nMode = 0;

		return false;
	}
#endif
	//COMMON->Write_Comm_Log(strMsg);
	return bRet;
}
bool CSYSocket::Send_Origin()
{
	// 검사기로 검사시작 요청시 사용.
	CString strTemp = _T("");
	CString strMsg = _T("");
	int		nReturnLength = 0;
	BOOL	bRet = FALSE;

	BYTE szBuffer[100];
	::ZeroMemory(szBuffer, sizeof(szBuffer));


	szBuffer[0] = 0x02;
	szBuffer[1] = 'S';
	szBuffer[2] = 'H';
	szBuffer[3] = '0';
	szBuffer[4] = 'C';
	szBuffer[5] = '0';
	szBuffer[6] = '5';
	szBuffer[7] = 0x03;

	for (int i = 0; i < 8; i++)
	{
		strTemp.Format(_T("%c"), szBuffer[i]);
		strMsg += strTemp;
	}

#ifdef SOCKET_USE
	if ((m_socket.Send(szBuffer, SOCKET_DATA_MAX_SIZE)) == SOCKET_ERROR)
	{

		m_bReturn = FALSE;
		m_strRecvData = _T("");
		m_nMode = 0;

		return false;
	}
#endif
	//COMMON->Write_Comm_Log(strMsg);
	return bRet;
}
bool CSYSocket::Send_LotStart(int nTesterNum)
{
	// 검사기로 검사시작 요청시 사용.
	CString strTemp = _T("");
	CString strMsg = _T("");
	int		nReturnLength = 0;
	BOOL	bRet = FALSE;

	BYTE szBuffer[100];
	::ZeroMemory(szBuffer, sizeof(szBuffer));


	szBuffer[0] = 0x02;
	szBuffer[1] = 'S';
	szBuffer[2] = 'H';
	szBuffer[3] = 0x30 + nTesterNum;
	szBuffer[4] = 'L';
	szBuffer[5] = '0';
	szBuffer[6] = '0';
	szBuffer[7] = 0x03;

	for (int i = 0; i < 9; i++)
	{
		strTemp.Format(_T("%c"), szBuffer[i]);
		strMsg += strTemp;
	}

#ifdef SOCKET_USE
	if ((m_socket.Send(szBuffer, SOCKET_DATA_MAX_SIZE)) == SOCKET_ERROR)
	{

		m_bReturn = FALSE;
		m_strRecvData = _T("");
		m_nMode = 0;

		return false;
	}
#endif
	//COMMON->Write_Comm_Log(strMsg);
	return bRet;
}
bool CSYSocket::Send_LotStop(int nTesterNum)
{
	// 검사기로 검사시작 요청시 사용.
	CString strTemp = _T("");
	CString strMsg = _T("");
	int		nReturnLength = 0;
	BOOL	bRet = FALSE;

	BYTE szBuffer[100];
	::ZeroMemory(szBuffer, sizeof(szBuffer));


	szBuffer[0] = 0x02;
	szBuffer[1] = 'S';
	szBuffer[2] = 'H';
	szBuffer[3] = 0x30 + nTesterNum;
	szBuffer[4] = 'L';
	szBuffer[5] = '0';
	szBuffer[6] = '1';
	szBuffer[7] = 0x03;

	for (int i = 0; i < 9; i++)
	{
		strTemp.Format(_T("%c"), szBuffer[i]);
		strMsg += strTemp;
	}

#ifdef SOCKET_USE
	if ((m_socket.Send(szBuffer, SOCKET_DATA_MAX_SIZE)) == SOCKET_ERROR)
	{

		m_bReturn = FALSE;
		m_strRecvData = _T("");
		m_nMode = 0;

		return false;
	}
#endif
	//COMMON->Write_Comm_Log(strMsg);
	return bRet;
}
bool CSYSocket::Send_LotInfo(CString strInfo)
{
	// 검사기로 검사시작 요청시 사용.
	CString strTemp = _T("");
	CString strMsg = _T("");
	int		nReturnLength = 0;
	BOOL	bRet = FALSE;

	BYTE szBuffer[100];
	::ZeroMemory(szBuffer, sizeof(szBuffer));


	szBuffer[0] = 0x02;
	szBuffer[1] = 'S';
	szBuffer[2] = 'H';
	szBuffer[3] = '0';
	szBuffer[4] = 'L';
	szBuffer[5] = '0';
	szBuffer[6] = '2';
	szBuffer[7] = 0x03;

	for (int i = 0; i < strInfo.GetLength(); i++)
	{
		szBuffer[8 + i] = strInfo.GetAt(i);
	}

	szBuffer[8 + strInfo.GetLength()] = 0x03;

	for (int i = 0; i < 8 + strInfo.GetLength(); i++)
	{
		strTemp.Format(_T("%c"), szBuffer[i]);
		strMsg += strTemp;
	}

#ifdef SOCKET_USE
	if ((m_socket.Send(szBuffer, SOCKET_DATA_MAX_SIZE)) == SOCKET_ERROR)
	{

		m_bReturn = FALSE;
		m_strRecvData = _T("");
		m_nMode = 0;

		return false;
	}
#endif
	//COMMON->Write_Comm_Log(strMsg);
	return bRet;
}

bool CSYSocket::Send_StageMove(int nPos)
{
	// 검사기로 검사시작 요청시 사용.
	CString strTemp = _T("");
	CString strMsg = _T("");
	int		nReturnLength = 0;
	BOOL	bRet = FALSE;

	BYTE szBuffer[100];
	::ZeroMemory(szBuffer, sizeof(szBuffer));


	szBuffer[0] = 0x02;
	szBuffer[1] = 'S';
	szBuffer[2] = 'H';
	szBuffer[3] = '0';
	szBuffer[4] = 'G';
	szBuffer[5] = '0';
	szBuffer[6] = '0';

	if (nPos == 0)
		szBuffer[7] = '0';
	else if (nPos == 1)
		szBuffer[7] = '1';
	else if (nPos == 2)
		szBuffer[7] = '2';

	szBuffer[8] = 0x03;


	for (int i = 0; i < 9; i++)
	{
		strTemp.Format(_T("%c"), szBuffer[i]);
		strMsg += strTemp;
	}

#ifdef SOCKET_USE
	if ((m_socket.Send(szBuffer, SOCKET_DATA_MAX_SIZE)) == SOCKET_ERROR)
	{

		m_bReturn = FALSE;
		m_strRecvData = _T("");
		m_nMode = 0;

		return false;
	}
#endif
	//COMMON->Write_Comm_Log(strMsg);
	return bRet;
}
bool CSYSocket::Send_MotorMove_Abs(CString strMove)
{
	// 검사기로 검사시작 요청시 사용.
	CString strTemp = _T("");
	CString strMsg = _T("");
	int		nReturnLength = 0;
	BOOL	bRet = FALSE;

	BYTE szBuffer[100];
	::ZeroMemory(szBuffer, sizeof(szBuffer));


	szBuffer[0] = 0x02;
	szBuffer[1] = 'S';
	szBuffer[2] = 'H';
	szBuffer[3] = '0';
	szBuffer[4] = 'G';
	szBuffer[5] = '0';
	szBuffer[6] = '1';

	for (int i = 0; i < strMove.GetLength(); i++)
	{
		szBuffer[7 + i] = strMove.GetAt(i);
	}

	szBuffer[7 + strMove.GetLength()] = 0x03;

	for (int i = 0; i < 8 + strMove.GetLength(); i++)
	{
		strTemp.Format(_T("%c"), szBuffer[i]);
		strMsg += strTemp;
	}

#ifdef SOCKET_USE
	if ((m_socket.Send(szBuffer, SOCKET_DATA_MAX_SIZE)) == SOCKET_ERROR)
	{

		m_bReturn = FALSE;
		m_strRecvData = _T("");
		m_nMode = 0;

		return false;
	}
#endif
	//COMMON->Write_Comm_Log(strMsg);
	return bRet;
}

bool CSYSocket::Send_MotorMove_Rel(CString strMove)
{
	// 검사기로 검사시작 요청시 사용.
	CString strTemp = _T("");
	CString strMsg = _T("");
	int		nReturnLength = 0;
	BOOL	bRet = FALSE;

	BYTE szBuffer[100];
	::ZeroMemory(szBuffer, sizeof(szBuffer));


	szBuffer[0] = 0x02;
	szBuffer[1] = 'S';
	szBuffer[2] = 'H';
	szBuffer[3] = '0';
	szBuffer[4] = 'G';
	szBuffer[5] = '0';
	szBuffer[6] = '1';

	for (int i = 0; i < strMove.GetLength(); i++)
	{
		szBuffer[7 + i] = strMove.GetAt(i);
	}

	szBuffer[7 + strMove.GetLength()] = 0x03;

	for (int i = 0; i < 8 + strMove.GetLength(); i++)
	{
		strTemp.Format(_T("%c"), szBuffer[i]);
		strMsg += strTemp;
	}

#ifdef SOCKET_USE
	if ((m_socket.Send(szBuffer, SOCKET_DATA_MAX_SIZE)) == SOCKET_ERROR)
	{

		m_bReturn = FALSE;
		m_strRecvData = _T("");
		m_nMode = 0;

		return false;
	}
#endif
	//COMMON->Write_Comm_Log(strMsg);
	return bRet;
}

bool CSYSocket::Send_ChartLight(CString strLight)
{
	// 검사기로 검사시작 요청시 사용.
	CString strTemp = _T("");
	CString strMsg = _T("");
	int		nReturnLength = 0;
	BOOL	bRet = FALSE;

	BYTE szBuffer[100];
	::ZeroMemory(szBuffer, sizeof(szBuffer));


	szBuffer[0] = 0x02;
	szBuffer[1] = 'S';
	szBuffer[2] = 'H';
	szBuffer[3] = '0';
	szBuffer[4] = 'L';
	szBuffer[5] = 'C';
	szBuffer[6] = '0';

	for (int i = 0; i < strLight.GetLength(); i++)
	{
		szBuffer[8 + i] = strLight.GetAt(i);
	}

	szBuffer[7 + strLight.GetLength()] = 0x03;

	for (int i = 0; i < 9 + strLight.GetLength(); i++)
	{
		strTemp.Format(_T("%c"), szBuffer[i]);
		strMsg += strTemp;
	}

#ifdef SOCKET_USE
	if ((m_socket.Send(szBuffer, SOCKET_DATA_MAX_SIZE)) == SOCKET_ERROR)
	{

		m_bReturn = FALSE;
		m_strRecvData = _T("");
		m_nMode = 0;

		return false;
	}
#endif
	//COMMON->Write_Comm_Log(strMsg);
	return bRet;
}

bool CSYSocket::Send_BlemishLight(CString strLight)
{
	// 검사기로 검사시작 요청시 사용.
	CString strTemp = _T("");
	CString strMsg = _T("");
	int		nReturnLength = 0;
	BOOL	bRet = FALSE;

	BYTE szBuffer[100];
	::ZeroMemory(szBuffer, sizeof(szBuffer));


	szBuffer[0] = 0x02;
	szBuffer[1] = 'S';
	szBuffer[2] = 'H';
	szBuffer[3] = '0';
	szBuffer[4] = 'L';
	szBuffer[5] = 'B';
	szBuffer[6] = '0';

	for (int i = 0; i < strLight.GetLength(); i++)
	{
		szBuffer[8 + i] = strLight.GetAt(i);
	}

	szBuffer[7 + strLight.GetLength()] = 0x03;

	for (int i = 0; i < 9 + strLight.GetLength(); i++)
	{
		strTemp.Format(_T("%c"), szBuffer[i]);
		strMsg += strTemp;
	}

#ifdef SOCKET_USE
	if ((m_socket.Send(szBuffer, SOCKET_DATA_MAX_SIZE)) == SOCKET_ERROR)
	{

		m_bReturn = FALSE;
		m_strRecvData = _T("");
		m_nMode = 0;

		return false;
	}
#endif
	//COMMON->Write_Comm_Log(strMsg);
	return bRet;
}



//==============================================================================================
// Handler to Handler Communication
bool CSYSocket::Send_Handler_SampleInfo(int nHandlerNum)
{
	BYTE szBuffer[100];
	::ZeroMemory(szBuffer, sizeof(szBuffer));
	
	CString strTemp = _T("");
	CString strMsg = _T("");

	szBuffer[0] = 0x02;
	szBuffer[1] = '@';
	szBuffer[2] = '0';
	szBuffer[3] = '0';
	szBuffer[4] = '1';
	szBuffer[5] = ',';
	szBuffer[6] = '1';
	szBuffer[7] = 0x03;

	for (int i = 0; i < 8; i++)
	{
		strTemp.Format(_T("%c"), szBuffer[i]);
		strMsg += strTemp;
	}

#ifdef SOCKET_USE
	if ((m_socket.Send(szBuffer, SOCKET_DATA_MAX_SIZE)) == SOCKET_ERROR)
	{

		m_bReturn = FALSE;
		m_strRecvData = _T("");
		m_nMode = 0;

		return false;
	}
#endif
	//COMMON->Write_Comm_Log(strMsg);
	
	return TRUE;
}
bool CSYSocket::Send_Handler_MchStart(int nHandlerNum, CString strBarcode)
{
	BYTE szBuffer[100];
	::ZeroMemory(szBuffer, sizeof(szBuffer));

	CString strTemp = _T("");
	CString strMsg = _T("");

	szBuffer[0] = 0x02;
	szBuffer[1] = '@';
	szBuffer[2] = '0';
	szBuffer[3] = '0';
	szBuffer[4] = '2';
	szBuffer[5] = ',';
	
	
	for (int i = 0; i < strBarcode.GetLength(); i++)
	{
		szBuffer[6 + i] = strBarcode.GetAt(i);
	}

	szBuffer[6 + strBarcode.GetLength()] = 0x03;

	for (int i = 0; i < 8; i++)
	{
		strTemp.Format(_T("%c"), szBuffer[i]);
		strMsg += strTemp;
	}

#ifdef SOCKET_USE
	if ((m_socket.Send(szBuffer, SOCKET_DATA_MAX_SIZE)) == SOCKET_ERROR)
	{

		m_bReturn = FALSE;
		m_strRecvData = _T("");
		m_nMode = 0;

		return false;
	}
#endif
	//COMMON->Write_Comm_Log(strMsg);

	return TRUE;
}
bool CSYSocket::Send_Handler_LivePing(int nHandlerNum)
{
	BYTE szBuffer[100];
	::ZeroMemory(szBuffer, sizeof(szBuffer));

	CString strTemp = _T("");
	CString strMsg = _T("");

	szBuffer[0] = 0x02;
	szBuffer[1] = '@';
	szBuffer[2] = '0';
	szBuffer[3] = '0';
	szBuffer[4] = '3';
	szBuffer[5] = ',';
	szBuffer[6] = '1';
	szBuffer[7] = 0x03;

	for (int i = 0; i < 8; i++)
	{
		strTemp.Format(_T("%c"), szBuffer[i]);
		strMsg += strTemp;
	}

#ifdef SOCKET_USE
	if ((m_socket.Send(szBuffer, SOCKET_DATA_MAX_SIZE)) == SOCKET_ERROR)
	{

		m_bReturn = FALSE;
		m_strRecvData = _T("");
		m_nMode = 0;

		return false;
	}
#endif
	//COMMON->Write_Comm_Log(strMsg);

	return TRUE;
}

bool CSYSocket::Return_Handler_SampleInfo(int nHandlerNum, int nState)
{
	BYTE szBuffer[100];
	::ZeroMemory(szBuffer, sizeof(szBuffer));

	CString strTemp = _T("");
	CString strMsg = _T("");

	szBuffer[0] = 0x02;
	szBuffer[1] = '@';
	szBuffer[2] = '0';
	szBuffer[3] = '0';
	szBuffer[4] = '2';
	szBuffer[5] = ',';

	if( nState == TRUE )
		szBuffer[6] = '1';
	else
		szBuffer[6] = '0';

	szBuffer[7] = 0x03;

	for (int i = 0; i < 8; i++)
	{
		strTemp.Format(_T("%c"), szBuffer[i]);
		strMsg += strTemp;
	}

#ifdef SOCKET_USE
	if ((m_socket.Send(szBuffer, SOCKET_DATA_MAX_SIZE)) == SOCKET_ERROR)
	{

		m_bReturn = FALSE;
		m_strRecvData = _T("");
		m_nMode = 0;

		return false;
	}
#endif
	//COMMON->Write_Comm_Log(strMsg);

	return TRUE;
}
bool CSYSocket::Return_Handler_MchStart(int nHandlerNum, int nState)
{
	BYTE szBuffer[100];
	::ZeroMemory(szBuffer, sizeof(szBuffer));

	CString strTemp = _T("");
	CString strMsg = _T("");

	szBuffer[0] = 0x02;
	szBuffer[1] = '@';
	szBuffer[2] = '0';
	szBuffer[3] = '0';
	szBuffer[4] = '3';
	szBuffer[5] = ',';

	if (nState == TRUE)
		szBuffer[6] = '1';
	else
		szBuffer[6] = '0';

	szBuffer[7] = 0x03;

	for (int i = 0; i < 8; i++)
	{
		strTemp.Format(_T("%c"), szBuffer[i]);
		strMsg += strTemp;
	}

#ifdef SOCKET_USE
	if ((m_socket.Send(szBuffer, SOCKET_DATA_MAX_SIZE)) == SOCKET_ERROR)
	{

		m_bReturn = FALSE;
		m_strRecvData = _T("");
		m_nMode = 0;

		return false;
	}
#endif
	//COMMON->Write_Comm_Log(strMsg);

	return TRUE;
}
bool CSYSocket::Return_Handler_LivePing(int nHandlerNum)
{
	BYTE szBuffer[100];
	::ZeroMemory(szBuffer, sizeof(szBuffer));

	CString strTemp = _T("");
	CString strMsg = _T("");

	szBuffer[0] = 0x02;
	szBuffer[1] = '@';
	szBuffer[2] = '0';
	szBuffer[3] = '0';
	szBuffer[4] = '1';
	szBuffer[5] = ',';
	szBuffer[6] = '1';
	szBuffer[7] = 0x03;

	for (int i = 0; i < 8; i++)
	{
		strTemp.Format(_T("%c"), szBuffer[i]);
		strMsg += strTemp;
	}

#ifdef SOCKET_USE
	if ((m_socket.Send(szBuffer, SOCKET_DATA_MAX_SIZE)) == SOCKET_ERROR)
	{

		m_bReturn = FALSE;
		m_strRecvData = _T("");
		m_nMode = 0;

		return false;
	}
#endif
	//COMMON->Write_Comm_Log(strMsg);

	return TRUE;
}
