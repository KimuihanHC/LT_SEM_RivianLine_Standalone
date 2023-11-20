//*****************************************************************************
// Filename	: 	BCRCtrl.h
// Created	:	2016/5/9 - 23:33
// Modified	:	2016/5/9 - 23:33
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
#ifndef BCRCtrl_h__
#define BCRCtrl_h__

#pragma once

#include "SerialCom_Base.h"

//=============================================================================
//
//=============================================================================
class CBCRCtrl : public CSerialCom_Base
{
public:
	CBCRCtrl();
	virtual ~CBCRCtrl();

	//---------------------------------------------------------
	// 재정의
	//---------------------------------------------------------
protected:
	virtual void	OnEvent					(EEvent eEvent, EError eError);
	virtual LRESULT	OnFilterRecvData		(const char* szACK, DWORD dwAckSize);
	//virtual void	OnRecvProtocol			(const char* szACK, DWORD dwAckSize);


public:

	// 

	BOOL		Send_ReadBarcode			(__out CString& szOutBarcode);

};

#endif // BCRCtrl_h__

