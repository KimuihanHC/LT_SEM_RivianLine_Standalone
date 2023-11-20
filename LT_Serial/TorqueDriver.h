//*****************************************************************************
// Filename	: 	TorqueDriver.h
// Created	:	2018/03/09
// Modified	:	2018/03/09
//
// Author	:	KHO
//	
// Purpose	:	
//*****************************************************************************
#ifndef TorqueDriver_h__
#define TorqueDriver_h__

#pragma once

#include "SerialCom_Base.h"
#include "Def_TorqueDrvier.h"

using namespace TorqueDriver;

//=============================================================================
//
//=============================================================================
class CTorqueDriver : public CSerialCom_Base
{
public:
	CTorqueDriver();
	virtual ~CTorqueDriver();

	//---------------------------------------------------------
	// 재정의
	//---------------------------------------------------------
protected:
	//virtual void	OnEvent					(EEvent eEvent, EError eError);
	virtual LRESULT	OnFilterRecvData		(const char* szACK, DWORD dwAckSize);
	virtual void	OnRecvProtocol			(const char* szACK, DWORD dwAckSize);

	//-----------------------------------------------------
	// 통신 모듈
	//-----------------------------------------------------
	ST_TorqueProtocol			m_stSendProtocol;
	ST_TorqueProtocol_Recv		m_stRecvProtocol;
	
	void	ResetProtocol		();

public:

	BOOL	Send_VersionCheck	();

	ST_TorqueMonitor	GetAckProtocol()
	{
		return m_stRecvProtocol.stMonitor;
	};


};

#endif // TorqueDriver_h__
