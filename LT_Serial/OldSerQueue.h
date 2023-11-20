//*****************************************************************************
// Filename	: 	OldSerQueue.h
//
// Created	:	2009/03/31
// Modified	:	2009/12/28 - 12:28
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
#pragma once

#include <deque>
using namespace std;

class COldSerQueue
{
public:
	COldSerQueue(void);
	~COldSerQueue(void);

protected:

	deque <char>::iterator m_Iter;
	deque <char> m_Queue;


public:
	char		Pop							();
	DWORD_PTR	PopData						(char* pOutBuf, DWORD_PTR dwMaxSize);
	DWORD_PTR	PopAllData					(char * pOutBuf);
	BOOL		PopDataUntilFindDelimiter	(char cDelimiter, char * pOutBuf, DWORD_PTR* pBufSize, DWORD_PTR dwMaxSize = 0);

	void		Push						(char cInChar);
	void		Push						(const char* pInBuf, DWORD_PTR InBufSize);

	INT_PTR		FindDelimiter				(char cDelimiter, UINT_PTR iStart = 0);
	INT_PTR		EraseUntilFindDelimiter		(char cDelimiter);

	void		Empty						();
	BOOL		IsEmpty						();
	DWORD_PTR	GetSize						();

	char		GetAt						(UINT_PTR nPos);
};


//assign ()		: Erases elements from a deque and copies a new set of elements to the target deque. 
//at ()			: Returns a reference to the element at a specified location in the deque. 
//back ()		: Returns a reference to the last element of the deque. 
//begin ()		: Returns an iterator addressing the first element in the deque. 
//clear ()		: Erases all the elements of a deque. 
//empty ()		: Tests if a deque is empty. 
//end ()		: Returns an iterator that addresses the location succeeding the last element in a deque. 
//erase ()		: Removes an element or a range of elements in a deque from specified positions. 
//front ()		: Returns a reference to the first element in a deque. 
//get_allocator () : Returns a copy of the allocator object used to construct the deque. 
//insert ()		: Inserts an element or a number of elements or a range of elements into the deque at a specified position. 
//max_size ()	: Returns the maximum length of the deque. 
//pop_back ()	: Deletes the element at the end of the deque. 
//pop_front ()	: Deletes the element at the beginning of the deque. 
//push_back ()	: Adds an element to the end of the deque. 
//push_front () : Adds an element to the beginning of the deque. 
//rbegin ()		: Returns an iterator to the first element in a reversed deque. 
//rend ()		: Returns an iterator that points just beyond the last element in a reversed deque. 
//resize ()		: Specifies a new size for a deque. 
//size ()		: Returns the number of elements in the deque. 
//swap ()		: Exchanges the elements of two deques. 

