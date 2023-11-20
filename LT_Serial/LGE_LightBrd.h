//*****************************************************************************
// Filename	: 	LGE_LightBrd.h
// Created	:	2016/03/09
// Modified	:	2016/05/09
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
#ifndef LGE_LightBrd_h__
#define LGE_LightBrd_h__

#pragma once

#include "SerialCom_Base.h"
#include "Def_LGE_LightBrd.h"

using namespace LGE_LightControl;

//=============================================================================
//
//=============================================================================
class CLGE_LightBrd : public CSerialCom_Base
{
public:
	CLGE_LightBrd();
	virtual ~CLGE_LightBrd();

	//---------------------------------------------------------
	// 재정의
	//---------------------------------------------------------
protected:
	//virtual void	OnEvent					(EEvent eEvent, EError eError);
	virtual LRESULT	OnFilterRecvData		(const char* szACK, DWORD dwAckSize);
	virtual void	OnRecvProtocol			(const char* szACK, DWORD dwAckSize);

	ST_LightBrdProtocol		m_stProtocol;
	ST_LightBrdRecvProtocol	m_stRecvProtocol;	

	void			ResetProtocol();

	FLOAT			m_fMinCurrent;
	FLOAT			m_fMaxCurrent;
	FLOAT			m_fUnitCurrent;

public:

	ST_LightBrdRecvProtocol	GetAckProtocol()
	{
		return m_stRecvProtocol;
	};

	// 최소 전압, 최대 전압 설정
	void		SetMinMaxVoltage			(__in FLOAT fMinVolt, __in FLOAT fMaxVolt);

	void		SetMinMaxCurrent			(__in FLOAT fMinCurrent, __in FLOAT fMaxCurrent);

	// 포트 체크
	LRESULT		Send_PortCheck				(__out BYTE& byPortNo);
	// 개별 전압 설정
	LRESULT		Send_EtcSetVolt				(__in UINT nSlotNo, __in float fVolt);
	// 전압 측정
	LRESULT		Send_DetectVolt				(__out ST_SlotInfo& stOutSlotVolt);
	// 개별 전류 설정
	LRESULT		Send_EtcSetCurrent			(__in UINT nSlotNo, __in float fCurrent);
	// 전류 측정
	LRESULT		Send_DetectCurrent			(__out ST_SlotInfo& stOutSlotVolt);
	// 자동/수동 모드 체크
	LRESULT		Send_ControlCheck			(__out BOOL& bAuto, __out BOOL& bManual);

	// 밝기 단계를 이용한 광원 제어, 전압
	LRESULT		Send_AmbientVoltOn_All		(__in float fVolt);
	// 전체 광원 끄기, 전압
	LRESULT		Send_AmbientVoltOff_All		();

	// 밝기 단계를 이용한 광원 제어, 전류
	LRESULT		Send_AmbientCurrentOn_All	(__in WORD wValue);
	// 전체 광원 끄기, 전류
	LRESULT		Send_AmbientCurrentOff_All	();

};

#endif // LGE_LightBrd_h__
