//*****************************************************************************
// Filename	: 	PCBCamBrd.h
// Created	:	2016/03/09
// Modified	:	2016/05/09
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
#ifndef PCBCamBrd_h__
#define PCBCamBrd_h__

#pragma once
#include "SerialCom_Base.h"
#include "Def_PCBCamBrd.h"

using namespace LGIT_CamBrd;

//=============================================================================
//
//=============================================================================
class CPCBCamBrd : public CSerialCom_Base
{
public:
	CPCBCamBrd();
	virtual ~CPCBCamBrd();

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
	ST_CamBrdProtocol		m_stProtocol;
	ST_CamBrdRecvProtocol	m_stRecvProtocol;	

	void			ResetProtocol();

public:

	ST_CamBrdRecvProtocol	GetAckProtocol()
	{
		return m_stRecvProtocol;
	};

	// 공용
	LRESULT			Send_BoardCheck			(__out BYTE& byBrdNo);
	LRESULT			Send_SetVolt			(__in BOOL	bOnOfff);
	LRESULT			Send_GetCurrent			(__out ST_CamBrdCurrent& stCurrent);
	LRESULT			Send_CheckOverCurrent	(__out BOOL& bOverCurrent);
	LRESULT			Send_LEDDutyControl		(__in double dLEDRMSCycle);
	LRESULT			Send_LEDViselOnOff		(__in BOOL	bOnOfff);
};

#endif // PCBCamBrd_h__
