//*****************************************************************************
// Filename	: 	File_WatchList.h
// Created	:	2016/1/29 - 16:24
// Modified	:	2016/1/29 - 16:24
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
#ifndef File_WatchList_h__
#define File_WatchList_h__

#pragma once

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
class CFile_WatchList
{
public:
	CFile_WatchList();
	~CFile_WatchList();

protected:
	CStringList		m_lstFile;

	CString			m_strFilePath;
	CString			m_strExt;
	UINT			m_WM_Refresh;

	HWND			m_hOwner;
	// 목록 변경 감시 쓰레드	
	HANDLE			m_hThrWatchDir;
	HANDLE			m_hEndWatchEvent;
	
	static	UINT WINAPI ThrWatchDirectory(LPVOID lpV);
	
	// 목록 지움
	void			ResetList			();
	// 설정된 폴더에서 모델 파일을 찾아 목록에 추가함.
	void			SearchFileDir		();

	
public:
	void			SetOwner			(__in HWND hOwner, __in UINT nWinMsg);
	
	void			SetWatchOption		(__in LPCTSTR lpszDir, __in LPCTSTR lpszExt);	

	void			BeginWatchThrFunc	();
	void			EndWatchThrFunc		();
	// 목록 갱신
	void			RefreshList			();

	CStringList*	GetFileList			()
	{
		return &m_lstFile;
	};

	virtual BOOL	SaveFile			(__in LPCTSTR lpszFilePath, __in const VOID* pIn);
	virtual BOOL	LoadFile			(__in LPCTSTR lpszFilePath, __out VOID* pOut);


	BOOL			IsThreadAlive();
};

#endif // File_WatchList_h__

