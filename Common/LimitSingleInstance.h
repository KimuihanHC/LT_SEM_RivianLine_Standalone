//*****************************************************************************
// Filename	: LimitSingleInstance.h
// Created	: 2010/10/27
// Modified	: 2010/10/27 - 10:32
//
// Author	: PiRing
//	
// Purpose	: 
//*****************************************************************************
#ifndef LimitSingleInstance_h__
#define LimitSingleInstance_h__

#include <windows.h> 

#pragma once

//=============================================================================
// CLimitSingleInstance
//=============================================================================
class CLimitSingleInstance
{
protected:
	DWORD  m_dwLastError;
	HANDLE m_hMutex;

public:
	CLimitSingleInstance(TCHAR *strMutexName)
	{
		//Make sure that you use a name that is unique for this application otherwise
		//two apps may think they are the same if they are using same name for
		//3rd parm to CreateMutex
		m_hMutex = CreateMutex(NULL, FALSE, strMutexName); //do early
		m_dwLastError = GetLastError(); //save for use later...
	}

	~CLimitSingleInstance() 
	{
		if (m_hMutex)  //Do not forget to close handles.
		{
			CloseHandle(m_hMutex); //Do as late as possible.
			m_hMutex = NULL; //Good habit to be in.
		}
	}

	BOOL IsAnotherInstanceRunning() 
	{
		return (ERROR_ALREADY_EXISTS == m_dwLastError);
	}

};

#endif // LimitSingleInstance_h__
