//*****************************************************************************
// Filename	: 	ODA_PT.h
// Created	:	2016/03/09
// Modified	:	2016/05/09
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
#ifndef ODA_PT_h__
#define ODA_PT_h__

#pragma once

#include "SerialCom_Base.h"
#include "Def_ODA_PT.h"

using namespace ODA_PT;

//=============================================================================
//
//=============================================================================
class CODA_PT : public CSerialCom_Base
{
public:
	CODA_PT();
	virtual ~CODA_PT();

	//---------------------------------------------------------
	// 재정의
	//---------------------------------------------------------
protected:
	//virtual void	OnEvent					(EEvent eEvent, EError eError);
	virtual LRESULT	OnFilterRecvData		(const char* szACK, DWORD dwAckSize);
	virtual void	OnRecvProtocol			(const char* szACK, DWORD dwAckSize);

	ST_ODA_PT_Protocol		m_stProtocol;
	ST_ODA_PT_RecvProtocol	m_stRecvProtocol;	

	void			ResetProtocol();

	FLOAT			m_fMinCurrent;
	FLOAT			m_fMaxCurrent;
	FLOAT			m_fUnitCurrent;

public:

	ST_ODA_PT_RecvProtocol*	GetAckProtocol()
	{
		return &m_stRecvProtocol;
	};

	// 최소 전압, 최대 전압 설정
	void		SetMinMaxVoltage			(__in FLOAT fMinVolt, __in FLOAT fMaxVolt);

//	void		SetMinMaxCurrent			(__in FLOAT fMinCurrent, __in FLOAT fMaxCurrent);

	// 포트 체크
	LRESULT		Send_PortCheck				();
	

// 	CMD_APPLy,
	LRESULT		Send_Apply					(__in float fVoltage, __in float fCurrent);
	LRESULT		Send_Apply					(__in float fVoltage);
// 	CMD_APPLy_Req,
	LRESULT		Send_Apply_Status			(__out float& fOutVoltage, __out float& fOutCurrent);

// 	CMD_VOLT,
// 	CMD_VOLT_Req,
	LRESULT		Send_Voltage				(__in float fVoltage);
	LRESULT		Send_Voltage_Req			(__out float& fOutVoltage);
// 	CMD_VOLT_STEP,
// 	CMD_VOLT_STEP_Req,
// 	CMD_VOLT_OVP,
// 	CMD_VOLT_OVP_Req,
// 	CMD_VOLT_OVP_TRIP_Req,
// 	CMD_VOLT_OVP_CLE_Req,
// 	CMD_VOLT_UVL,
// 	CMD_VOLT_UVL_Req,
// 	CMD_VOLT_OVL,
// 	CMD_VOLT_OVL_Req,
// 	CMD_CURR,
// 	CMD_CURR_Req,
	LRESULT		Send_Current				(__in float fCurrent);
	LRESULT		Send_Current_Req			(__out float& fOutCurrent);
// 	CMD_CURR_STEP,
// 	CMD_CURR_STEP_Req,
// 	CMD_CURR_OCP,
// 	CMD_CURR_OCP_Req,
// 	CMD_CURR_OCP_TRIP_Req,
// 	CMD_CURR_OCP_CLE_Req,
// 	CMD_CURR_UCL,
// 	CMD_CURR_UCL_Req,
// 	CMD_CURR_OCL,
// 	CMD_CURR_OCL_Req,
// 	CMD_FLOW_Req,
	LRESULT		Send_Flow_Status			(__in enCV_CC nOutCVCC);

// 	CMD_POL,
// 	CMD_POL_Req,
// 	CMD_MEAS_CURR_Req,
// 	CMD_MEAS_VOLT_Req,
// 	CMD_MEAS_ALL_Req,
	LRESULT		Send_Meas_ALL				(__out float& fOutVoltage, __out float& fOutCurrent);
// 	CMD_SYST_BEEP,

// 	CMD_SYST_ERR_Req,
	LRESULT		Send_Error_Status			(__out int& iOutErrCode, __out CString& szOutErrorDesc);

// 	CMD_SYST_VERS_Req,
	LRESULT		Send_Version_Status			(__out CString& szOutVersion);

// 	CMD_SYST_OUTP,
	LRESULT		Send_Output					(__in enSwitchOnOff nOutputOnOff);
// 	CMD_SYST_OUTP_Req,
	LRESULT		Send_Output_Status			(__out enSwitchOnOff& nOutputOnOff);

// 	CMD_SYST_KEYL,
	LRESULT		Send_KeyLock				(__in enSwitchOnOff nKeyLockOnOff);
// 	CMD_SYST_KEYL_Req,
	LRESULT		Send_KeyLock_Status			(__out enSwitchOnOff& nKeyLockOnOff);
};

#endif // ODA_PT_h__
