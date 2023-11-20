//*****************************************************************************
// Filename	: 	ODA_Q.h
// Created	:	2016/03/09
// Modified	:	2016/05/09
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
#ifndef ODA_Q_h__
#define ODA_Q_h__

#pragma once

#include "SerialCom_Base.h"
#include "Def_ODA_Q.h"

using namespace ODA_Q;

//=============================================================================
//
//=============================================================================
class CODA_Q : public CSerialCom_Base
{
public:
	CODA_Q();
	virtual ~CODA_Q ();

	static CODA_Q* GetInstance();

	//---------------------------------------------------------
	// 재정의
	//---------------------------------------------------------
protected:
	//virtual void	OnEvent					(EEvent eEvent, EError eError);
	virtual LRESULT	OnFilterRecvData		(const char* szACK, DWORD dwAckSize);
	virtual void	OnRecvProtocol			(const char* szACK, DWORD dwAckSize);

	ST_ODA_Q_Protocol		m_stProtocol;
	ST_ODA_Q_RecvProtocol	m_stRecvProtocol;	

	void			ResetProtocol();

	FLOAT			m_fMinCurrent;
	FLOAT			m_fMaxCurrent;
	FLOAT			m_fUnitCurrent;

public:

	ST_ODA_Q_RecvProtocol*	GetAckProtocol()
	{
		return &m_stRecvProtocol;
	};

	// 최소 전압, 최대 전압 설정
	void		SetMinMaxVoltage			(__in FLOAT fMinVolt, __in FLOAT fMaxVolt);

//	void		SetMinMaxCurrent			(__in FLOAT fMinCurrent, __in FLOAT fMaxCurrent);

	// 포트 체크
	LRESULT		Send_PortCheck				();

	LRESULT		Send_Channel				(__in UINT nChannel);

	LRESULT		Send_Voltage				(__in float fVoltage);

	LRESULT		Send_Current				(__in float fCurrent);

	LRESULT		Send_Meas_Voltage			(__out float& fOutVoltage);

	LRESULT		Send_Meas_Current			(__out float& fOutCurrent);

	LRESULT		Send_Version_Status			(__out CString& szOutVersion);

	LRESULT		Send_Output					(__in enSwitchOnOff nOutputOnOff);

	LRESULT		Send_Output_Status			(__out enSwitchOnOff& nOutputOnOff);
};

#endif // ODA_Q_h__
