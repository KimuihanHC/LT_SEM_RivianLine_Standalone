// EzSock.cpp : implementation file
// Copyright(C) Sollae Systems Co.,Ltd.
// 11.Nov.2000 yyc  initial touch
//

#include "pch.h"
#include "EzSock.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEzSock
CEzSock::CEzSock()
{
	m_fConnected = FALSE;
}
CEzSock::~CEzSock()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CEzSock, CAsyncSocket)
	//{{AFX_MSG_MAP(CEzSock)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CEzSock member functions
void CEzSock::OnConnect(int nErrorCode) 
{
	try
	{
		::SendMessage(m_parent_wnd, WM_SOCK_CONNECT_MSG, (WPARAM)this, nErrorCode);
		CAsyncSocket::OnConnect(nErrorCode);
	}
	catch(...)
	{
		//TRACE("CEzSock::OnConnect => Exception.\r\n");
	}
}

void CEzSock::OnAccept(int nErrorCode) 
{
	try
	{
		::SendMessage(m_parent_wnd, WM_SOCK_ACCEPT_MSG, (WPARAM)this, nErrorCode);
		CAsyncSocket::OnAccept(nErrorCode);
	}
	catch(...)
	{
		//TRACE("CEzSock::OnAccept => Exception.\r\n");
	}
}

void CEzSock::OnClose(int nErrorCode) 
{	
	try
	{
		::SendMessage(m_parent_wnd, WM_SOCK_CLOSE_MSG, (WPARAM)this, nErrorCode);
		CAsyncSocket::Close();
	}
	catch(...)
	{
		//TRACE("CEzSock::OnClose => Exception.\r\n");
	}
}

void CEzSock::OnReceive(int nErrorCode) 
{
	try
	{
		::SendMessage(m_parent_wnd, WM_SOCK_RECV_MSG, (WPARAM)this, nErrorCode);
		CAsyncSocket::OnReceive(nErrorCode);
	}
	catch(...)
	{
		//TRACE("CEzSock::OnReceive => Exception.\r\n");
	}
}

void CEzSock::OnSend(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	CAsyncSocket::OnSend(nErrorCode);
}
