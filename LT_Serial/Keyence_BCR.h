//*****************************************************************************
// Filename	: 	Keyence_BCR.h
// Created	:	2016/5/9 - 23:33
// Modified	:	2016/5/9 - 23:33
//
// Author	:	PiRing
//	
// Purpose	:	KEYENCE SR-710 바코드 리더기 통신 클래스
//*****************************************************************************
#ifndef Keyence_BCR_h__
#define Keyence_BCR_h__

#pragma once

#include "SerialCom_Base.h"
#include "Def_Keyence_BCR.h"

using namespace Keyence;

//=============================================================================
//
//=============================================================================
class CKeyence_BCR : public CSerialCom_Base
{
public:
	CKeyence_BCR();
	virtual ~CKeyence_BCR();

	//---------------------------------------------------------
	// 재정의
	//---------------------------------------------------------
protected:
	virtual void	OnEvent					(EEvent eEvent, EError eError);
	virtual LRESULT	OnFilterRecvData		(const char* szACK, DWORD dwAckSize);
	virtual void	OnRecvProtocol			(const char* szACK, DWORD dwAckSize);
			
	ST_Protocol		m_stRecvProtocol;

public:

	// 
	LRESULT		Send_VersionCheck			();

	LRESULT		Send_TimingON				(__out UINT& nRetCode);
	LRESULT		Send_TimingOFF				(__out UINT& nRetCode);

	LRESULT		Send_ReadBarcode			(__out CString& szOutBarcode);

};

#endif // Keyence_BCR_h__

