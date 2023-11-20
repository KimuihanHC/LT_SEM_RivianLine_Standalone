#if !defined(AFX_EZDEBUGSOCK_H__AF376484_B8C8_11D4_8D51_00104B1587CE__INCLUDED_)
#define AFX_EZDEBUGSOCK_H__AF376484_B8C8_11D4_8D51_00104B1587CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EzDebugSock.h : header file
// Copyright(C) Sollae Systems Co.,Ltd.
// 11.Nov.2000 yyc  initial touch
//

#include <afxsock.h> 
/////////////////////////////////////////////////////////////////////////////
// CEzSock command target
#define WM_SOCK_RECV_MSG_LURI		(WM_USER+1020)
#define WM_SOCK_CONNECT_MSG_LURI	(WM_USER+1021)
#define WM_SOCK_CLOSE_MSG_LURI		(WM_USER+1022)
#define WM_SOCK_ACCEPT_MSG_LURI		(WM_USER+1023)

#define WM_SOCK_RECV_MSG		(WM_USER+1010)
#define WM_SOCK_CONNECT_MSG		(WM_USER+1011)
#define WM_SOCK_CLOSE_MSG		(WM_USER+1012)
#define WM_SOCK_ACCEPT_MSG		(WM_USER+1013)

class CEzSock : public CAsyncSocket
{
// Attributes
public:

// Operations
public:
	HWND m_parent_wnd;
	CEzSock();
	virtual ~CEzSock();

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEzSock)
	public:
	virtual void OnAccept(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CEzSock)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
public:
	BOOL m_fConnected;
private:
	//HWND		m_hWndOwner;
	//CVSPData*	m_pVspData;
	//CAsyncSocket m_ClientSocket;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EZSOCK_H__AF376484_B8C8_11D4_8D51_00104B1587CE__INCLUDED_)
