//*****************************************************************************
// Filename	: 	IndicatorSensor.h
// Created	:	2017/1/2 - 17:58
// Modified	:	2017/1/2 - 17:58
//
// Author	:	KHO
//	
// Purpose	:	
//*****************************************************************************

#ifndef IndicatorSensor_h__
#define IndicatorSensor_h__

#pragma once

#include "SerialCom_Base.h"
#include "Def_IndicatorSensor.h"

static const char* g_szReadIndicatorData = "";

//=============================================================================
//
//=============================================================================
class CIndicatorSensor : public CSerialCom_Base
{
public:
	CIndicatorSensor();
	virtual ~CIndicatorSensor();

	//---------------------------------------------------------
	// 재정의
	//---------------------------------------------------------
protected:
	virtual void	OnEvent(EEvent eEvent, EError eError);
	virtual LRESULT	OnFilterRecvData(const char* szACK, DWORD dwAckSize);
	virtual void	OnRecvProtocol(const char* szACK, DWORD dwAckSize);

	IndicatorSensor::ST_Protocol_Request		m_stSendProtocol;
	IndicatorSensor::ST_Protocol_Response		m_stRecvProtocol;

public:

	BOOL Send_SetZero				();
	BOOL Send_PortCheck				(__out CStringA& strData);
	BOOL Send_ReadIndicatorData		(__in BYTE byCommand, __out float& fValue);
};

#endif // IndicatorSensor

