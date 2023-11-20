//*****************************************************************************
// Filename	: SortableStringArray.h
// Created	: 2012/11/16
// Modified	: 2012/11/16 - 16:08
//
// Author	: PiRing
//	
// Purpose	: 
//*****************************************************************************
#pragma once

typedef int (__cdecl *GENERICCOMPAREFN)(const void * elem1, const void * elem2);
typedef int (__cdecl *STRINGCOMPAREFN)(const CString * elem1, const CString * elem2);

//=============================================================================
//
//=============================================================================
class CSortableStringArray : public CStringArray
{
public:
	CSortableStringArray(void){};
	virtual ~CSortableStringArray(void){};

protected:
	static int __cdecl Compare(const CString * pstr1, const CString * pstr2)
	{
		ASSERT(pstr1);
		ASSERT(pstr2);
		return pstr1->Compare(*pstr2);
	};

public:
	void Sort(STRINGCOMPAREFN pfnCompare = Compare)
	{
		CString * prgstr = GetData();
		qsort(prgstr, GetSize(), sizeof(CString), (GENERICCOMPAREFN)pfnCompare);
	};
};

//=============================================================================
//
//=============================================================================
//class CSortableStringList : public CStringList
//{
//public:
//	CSortableStringList(void){};
//	virtual ~CSortableStringList(void){};
//
//protected:
//	static int __cdecl Compare(const CString * pstr1, const CString * pstr2)
//	{
//		ASSERT(pstr1);
//		ASSERT(pstr2);
//		return pstr1->Compare(*pstr2);
//	};
//
//public:
//	void Sort(STRINGCOMPAREFN pfnCompare = Compare)
//	{
//		CString * prgstr = GetHead();
//		qsort(prgstr, GetSize(), sizeof(CString), (GENERICCOMPAREFN)pfnCompare);
//	};
//};