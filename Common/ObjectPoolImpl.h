//*****************************************************************************
// Filename	: ObjectPoolImpl.h
// Created	: 2011/1/25
// Modified	: 2011/1/25 - 16:59
//
// Author	: 
//	
// Purpose	: 메모리 단편화나 메모리 릭 때문에 new로 할당 받고, delete로 메모리 
//			해제하는것을 대체하기 위한 클래스
//*****************************************************************************
// Copyright (C) 1991 - 1999 Rational Software Corporation

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_CObjectPoolImpl_3DF6A86601FA_INCLUDED
#define _INC_CObjectPoolImpl_3DF6A86601FA_INCLUDED

//#include <afxcoll.h>
#include <afxmt.h>

template<class T> class CObjectPoolImpl
{
public:

	//Pre-allocate memory for the pool
	CObjectPoolImpl(int p_iSize)
	{
		s_iGrowSize = p_iSize;
		s_arrAllObjs = NULL;
		s_iLastObjIdx = 0;
		s_iNumOfObj = 0;
		s_iGrowSize = 10;

		increaseObjMem();
	}


	//Poll an object out of the pool
	T* getNewObj()
	{
		m_csLock.Lock();
		T* pObj = NULL;
		if (s_iLastObjIdx < 0)
		{
			//Increase memory for Obj
			increaseObjMem();		
		}

		pObj = (T*)s_arrAllObjs[s_iLastObjIdx];	
		s_iLastObjIdx --;

		m_csLock.Unlock();
		return pObj;
	}

	//Push back an object into the pool
	int deleteObj(T *pObj)
	{
		m_csLock.Lock();
		if (s_iLastObjIdx >= s_iNumOfObj-1)
		{
			m_csLock.Unlock();
			return -1;
		}
		else
		{
			s_iLastObjIdx++;
			s_arrAllObjs[s_iLastObjIdx] = (long)pObj;
		}
		m_csLock.Unlock();
		return 0;
	}

	//Empty the pool and destroy all objects
	int deleteAllObj()
	{
		m_csLock.Lock();
		for (int i=0; i<=s_iLastObjIdx; i++)
		{
			T* pObj = (T*)s_arrAllObjs[i];
			delete pObj;
		}
		delete[] s_arrAllObjs;
		m_csLock.Unlock();
		return 0;
	}



private:
	//Grow by Megabytes
	int s_iGrowSize;
	int s_iNumOfObj;
	int s_iLastObjIdx;	
	long* s_arrAllObjs;
	CCriticalSection m_csLock;

	//Renew object pool and increase the size
	BOOL increaseObjMem()
	{
		BOOL blRet = TRUE;
		long* arrOldObjs = s_arrAllObjs; //backup the pool

		int iObjSize = sizeof(T);
		int iAllSize = 1024*1024*s_iGrowSize;
		int iGrowNum = iAllSize/iObjSize;

		//Resize the pool
		s_iNumOfObj += iGrowNum;
		s_arrAllObjs = new long[s_iNumOfObj];
		s_iLastObjIdx = iGrowNum-1;

		//Renew the pool
		for (int i=0; i<iGrowNum; i++)
		{
			T* pObj = new T();
			s_arrAllObjs[i] = (long)pObj;
		}

		//Delete the backup pool
		if (arrOldObjs != NULL)
			delete[] arrOldObjs;

		return blRet;
	}


};

#endif /* _INC_CObjectPoolImpl_3DF6A86601FA_INCLUDED */


// 예제)	#define StrPool CObjectPoolImpl<CString>
//			StrPool pool(1000);	