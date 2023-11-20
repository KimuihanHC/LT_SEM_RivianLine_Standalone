//*****************************************************************************
// Filename	: 	OldSerQueue.cpp
//
// Created	:	2009/03/31
// Modified	:	2010/01/08 - 10:43
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
#include "StdAfx.h"
#include "OldSerQueue.h"

//=============================================================================
// Method		: COldSerQueue::COldSerQueue
// Access		: public 
// Returns		: 
// Parameter	: void
// Qualifier	:
// Last Update	: 2009/12/28 - 12:59
// Desc.		: 
//=============================================================================
COldSerQueue::COldSerQueue(void)
{
}

//=============================================================================
// Method		: COldSerQueue::~COldSerQueue
// Access		: public 
// Returns		: 
// Parameter	: void
// Qualifier	:
// Last Update	: 2009/12/28 - 12:59
// Desc.		: 
//=============================================================================
COldSerQueue::~COldSerQueue(void)
{
}

//=============================================================================
// Method		: COldSerQueue::Pop
// Access		: public 
// Returns		: char
// Qualifier	:
// Last Update	: 2009/12/28
// Desc.		: 
//=============================================================================
char COldSerQueue::Pop	()
{
	if (m_Queue.empty ())
		return 0;

	char cOutChar = m_Queue.front ();
	m_Queue.pop_front ();

	return cOutChar;
}

//=============================================================================
// Method		: COldSerQueue::PopData
// Access		: public 
// Returns		: DWORD_PTR
// Parameter	: char * pOutBuf
// Parameter	: DWORD_PTR dwMaxSize
// Qualifier	:
// Last Update	: 2009/12/28
// Desc.		: 
//=============================================================================
DWORD_PTR COldSerQueue::PopData(char* pOutBuf, DWORD_PTR dwMaxSize)
{
	DWORD_PTR dwSize = m_Queue.size ();
	if (dwSize == 0)
		return 0;

	DWORD_PTR dwBufsize = 0;
	if (dwSize < dwMaxSize)
		dwBufsize = dwSize;
	else
		dwBufsize = dwMaxSize;

	if (NULL == pOutBuf)
	{
		pOutBuf = new char [dwBufsize];
		memset (pOutBuf, 0, dwBufsize);
	}
	else
		memset (pOutBuf, 0, dwMaxSize);

	for (DWORD_PTR dwIndex = 0; dwIndex < dwBufsize; dwIndex++)
	{
		pOutBuf[dwIndex] = m_Queue.front ();
		m_Queue.pop_front ();
	}

	return dwBufsize;
}

//=============================================================================
// Method		: COldSerQueue::PopAllData
// Access		: public 
// Returns		: DWORD_PTR
// Parameter	: char * pOutBuf
// Qualifier	:
// Last Update	: 2009/12/28
// Desc.		: 
//=============================================================================
DWORD_PTR COldSerQueue::PopAllData(char * pOutBuf)
{
	DWORD_PTR dwSize = m_Queue.size ();
	if (dwSize == 0)
		return 0;

	if (NULL == pOutBuf)
		pOutBuf = new char [dwSize];

	memset (pOutBuf, 0, dwSize);

	for (DWORD_PTR dwIndex = 0; dwIndex < dwSize; dwIndex++)
	{
		pOutBuf[dwIndex] = m_Queue.front ();
		m_Queue.pop_front ();
	}

	return dwSize;
}

//=============================================================================
// Method		: COldSerQueue::PopDataUntilFindDelimiter
// Access		: public 
// Returns		: BOOL
// Parameter	: char cDelimiter
// Parameter	: char * pOutBuf
// Parameter	: DWORD_PTR * pBufSize
// Parameter	: DWORD_PTR dwMaxSize
// Qualifier	:
// Last Update	: 2009/12/28
// Desc.		: 
//=============================================================================
BOOL COldSerQueue::PopDataUntilFindDelimiter(char cDelimiter, char * pOutBuf, DWORD_PTR* pBufSize, DWORD_PTR dwMaxSize)
{
	DWORD_PTR dwSize = m_Queue.size ();
	if (dwSize == 0)
	{
		*pBufSize = 0;
		return FALSE;
	}

	DWORD_PTR dwBufsize = dwSize;
	if (0 < dwMaxSize)
	{		
		if (dwSize < dwMaxSize)
			dwBufsize = dwSize;
		else
			dwBufsize = dwMaxSize;
	}

	if (NULL == pOutBuf)
		pOutBuf = new char [dwBufsize];

	memset (pOutBuf, 0, dwBufsize);

	BOOL bFind = FALSE;
	for (DWORD_PTR dwIndex = 0; dwIndex < dwBufsize; dwIndex++)
	{
		if (cDelimiter == m_Queue.front ())
			bFind = TRUE;			

		pOutBuf[dwIndex] = m_Queue.front ();
		m_Queue.pop_front ();

		if (bFind)
		{
			*pBufSize = dwIndex + 1;
			return TRUE;
		}
	}

	*pBufSize = dwBufsize;
	return FALSE;
}

//=============================================================================
// Method		: COldSerQueue::Push
// Access		: public 
// Returns		: void
// Parameter	: char cInChar
// Qualifier	:
// Last Update	: 2009/12/28
// Desc.		: 
//=============================================================================
void COldSerQueue::Push (char cInChar)
{
	m_Queue.push_back (cInChar);
}

//=============================================================================
// Method		: COldSerQueue::Push
// Access		: public 
// Returns		: void
// Parameter	: const char * pInBuf
// Parameter	: DWORD_PTR InBufSize
// Qualifier	:
// Last Update	: 2009/12/28
// Desc.		: 
//=============================================================================
void COldSerQueue::Push(const char* pInBuf, DWORD_PTR InBufSize)
{
	if (NULL == pInBuf)
	{
		ASSERT (_T("NULL == pInBuf"));
		return;
	}

	if (InBufSize <= 0)
	{
		ASSERT (_T("InBufSize is 0"));
		return;
	}

	for (DWORD_PTR dwIndex = 0; dwIndex < InBufSize; dwIndex++)
		m_Queue.push_back (pInBuf[dwIndex]);
}

//=============================================================================
// Method		: COldSerQueue::FindDelimiter
// Access		: public 
// Returns		: INT_PTR
//					 0 보다 크면 찾음
//					-1	-> 오류 (찾지 못함)
//					-2	-> 오류 (큐에 데이터가 없음)
// Parameter	: char cDelimiter
// Parameter	: UINT_PTR iStart
// Qualifier	:
// Last Update	: 2009/12/29 - 14:14
// Desc.		: 
//=============================================================================
INT_PTR COldSerQueue::FindDelimiter(char cDelimiter, UINT_PTR iStart)
{
	DWORD_PTR dwSize = m_Queue.size ();

	if (dwSize == 0)
		return -2;

	for (DWORD_PTR dwIndex = iStart; dwIndex < dwSize; dwIndex++)
	{
		if (m_Queue.at (dwIndex) == cDelimiter)
			return dwIndex;
	}

	//deque <int>::iterator Iter;
	//for ( Iter = m_Queue.begin( ); Iter != m_Queue.end( ); Iter++ )
	//	if (cDelimiter == (*Iter))
	
	return -1;
}

//=============================================================================
// Method		: COldSerQueue::EraseUntilFindDelimiter
// Access		: public 
// Returns		: INT_PTR	-> 큐 크기
// Parameter	: char cDelimiter
// Qualifier	:
// Last Update	: 2009/12/28
// Desc.		: 
//=============================================================================
INT_PTR COldSerQueue::EraseUntilFindDelimiter(char cDelimiter)
{
	while (!m_Queue.empty())
	{
		if (m_Queue.front () == cDelimiter)
			break;
		else
			m_Queue.pop_front ();
	} 
	
	return m_Queue.size();
}

//=============================================================================
// Method		: COldSerQueue::RemoveAll
// Access		: public 
// Returns		: void
// Qualifier	:
// Last Update	: 2009/12/28
// Desc.		: 
//=============================================================================
void COldSerQueue::Empty ()
{
	m_Queue.clear();
}

//=============================================================================
// Method		: COldSerQueue::IsEmpty
// Access		: public 
// Returns		: BOOL
// Qualifier	:
// Last Update	: 2009/12/28
// Desc.		: 
//=============================================================================
BOOL COldSerQueue::IsEmpty ()
{
	return m_Queue.empty ();	
}

//=============================================================================
// Method		: COldSerQueue::GetSize
// Access		: public 
// Returns		: DWORD_PTR
// Qualifier	:
// Last Update	: 2009/12/28
// Desc.		: 
//=============================================================================
DWORD_PTR COldSerQueue::GetSize()
{
	return m_Queue.size ();
}

//=============================================================================
// Method		: COldSerQueue::GetAt
// Access		: public 
// Returns		: char
// Parameter	: UINT_PTR nPos
// Qualifier	:
// Last Update	: 2010/01/08 - 10:43
// Desc.		: 
//=============================================================================
char COldSerQueue::GetAt( UINT_PTR nPos )
{
	return m_Queue.at (nPos);
}