#pragma once

#include <stdlib.h>

//////////////////////////////////////////////////////////////////////////
// Sington Template
template <typename T>

class CSington
{
private:
	static T* m_pInst;
protected:
	CSington() 
	{
		ASSERT( !m_pInst );
		__int64 offset = (__int64)(T*)1 - (__int64)(CSington<T>*)(T*)1;
		m_pInst = (T*)( (__int64)this + offset);
		::atexit( DestroyInstance );
	}
	virtual ~CSington() 
	{
		m_pInst = NULL;
	};
public:
	static T* Instance()
	{
		if( m_pInst == NULL )
			m_pInst = new T;
		ASSERT( m_pInst != NULL );
		return m_pInst;
	}
protected:
	static void DestroyInstance()
	{
		if( m_pInst )
			delete m_pInst;
	}
};
template <typename T> T* CSington<T>::m_pInst = NULL;