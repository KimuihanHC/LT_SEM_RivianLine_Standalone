//*****************************************************************************
// Filename	: 	FN300.h
// Created	:	2016/03/09
// Modified	:	2016/05/09
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
#ifndef FN300_h__
#define FN300_h__

#pragma once
#include "SerialCom_Base.h"
#include "Def_FN300.h"

using namespace FN300;

//=============================================================================
//
//=============================================================================
class CFN300 : public CSerialCom_Base
{
public:
	CFN300();
	virtual ~CFN300();

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
	ST_SendProtocol_FN300	m_stProtocol;
	ST_RecvProtocol_FN300	m_stRecvProtocol;

	void			ResetProtocol();

public:

	ST_RecvProtocol_FN300	GetAckProtocol()
	{
		return m_stRecvProtocol;
	};

	// 공용
	LRESULT			Send_ChannelData	(__in UINT nCH, __in BYTE xData);
	LRESULT			Send_ChannelOnOff	(__in UINT nCH, __in BOOL bOnOff);
	LRESULT			Send_AllChannelOnOff(__in BOOL bOnOff);
};

#endif // FN300_h__
