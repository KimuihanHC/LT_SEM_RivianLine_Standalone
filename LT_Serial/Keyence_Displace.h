//*****************************************************************************
// Filename	: 	Keyence_Displace.h
// Created	:	2016/5/9 - 23:33
// Modified	:	2016/5/9 - 23:33
//
// Author	:	PiRing
//	
// Purpose	:	KEYENCE SR-710 바코드 리더기 통신 클래스
//*****************************************************************************
#ifndef Keyence_Displace_h__
#define Keyence_Displace_h__

#pragma once

#include "SerialCom_Base.h"
#include "Def_Keyence_Displace.h"

using namespace Keyence_Displace;

//=============================================================================
//
//=============================================================================
class CKeyence_Displace : public CSerialCom_Base
{
public:
	CKeyence_Displace();
	virtual ~CKeyence_Displace();

	//---------------------------------------------------------
	// 재정의
	//---------------------------------------------------------
protected:
	virtual void	OnEvent					(EEvent eEvent, EError eError);
	virtual LRESULT	OnFilterRecvData		(const char* szACK, DWORD dwAckSize);
	virtual void	OnRecvProtocol			(const char* szACK, DWORD dwAckSize);
			
	ST_RecvProtocol		m_stRecvProtocol;

public:
	LRESULT			Send_OutputData			(__out double& dOutData);

};

#endif // Keyence_Displace_h__

