
#pragma once

#include "SerialCom_Base.h"
#include "Def_LED_PD_FN300.h"

//#include "Logger.h"

using namespace LED_PD_FN300;

class CLED_PD_FN300 : public CSerialCom_Base
{
public:
	CLED_PD_FN300();
	virtual ~CLED_PD_FN300();

	static CLED_PD_FN300* GetInstance();

	//---------------------------------------------------------
	// ¿Á¡§¿«
	//---------------------------------------------------------
protected:
	//virtual void	OnEvent					(EEvent eEvent, EError eError);
	virtual LRESULT	OnFilterRecvData		(const char* szACK, DWORD dwAckSize);
	virtual void	OnRecvProtocol			(const char* szACK, DWORD dwAckSize);

	ST_LedPD_FN300_Protocol			m_stProtocol;
	ST_LedPD_FN300_Recv_Protocol	m_stRecvProtocol;

	void			ResetProtocol();

//	CLogger					m_Log;

public:

	BOOL SendManuallyData		(const char* lpBuffer, DWORD dwCount);

	// Channel data control
	LRESULT Send_SetChannelData(__in int ch, __in int data);

	// Channel on/off
	LRESULT Send_SetChannelOnOff(__in int ch, __in BOOL bOn);

	// Confirmation of the condition
	LRESULT Send_GetChannelData(__in int ch, __out int& data);

	//
	LRESULT	Send_GetChannelOnOff(__in int ch, __out BOOL& bOn);

	//
	LRESULT Send_GetChannelCheck(__in int ch, __out BOOL& bError);
};

