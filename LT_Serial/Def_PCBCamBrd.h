//*****************************************************************************
// Filename	: 	Define_CamBrd.h
// Created	:	2016/3/8 - 13:31
// Modified	:	2016/3/8 - 13:31
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
#ifndef Define_CamBrd_h__
#define Define_CamBrd_h__

#include <afxwin.h>

//=============================================================================
// LGIT - Camera Board 통신 프로토콜
//=============================================================================
/*
Transmit from host
<STX><CMD><DATA><ETX>

Response from host
<STX><CMD><DATA><ETX>

Where:
<STX>		= 1 ASCII '!'
<CMD>		= 1 ASCII character
<DATA>		= Command Argument, up to 9 ASCII characters.
<ETX>		= 1 ASCII '@'
*/
namespace LGIT_CamBrd
{

#define	CamBrd_STX				0x21	//'!'
#define	CamBrd_ETX				0x40	//'@'
#define CamBrd_DummyChar		0x30	//'0'
#define CamBrd_ProtoLength		27
#define CamBrd_DataLength		24

	// 고정된 통신 프로토콜 선언
	static const char*	g_szCamBrd_BoardCheck		= "!0100000000000000000000000@";
	static const char*	g_szCamBrd_VoltOn			= "!V100000000000000000000000@";
	static const char*	g_szCamBrd_VoltOff			= "!V000000000000000000000000@";
	static const char*	g_szCamBrd_GetInCurrent		= "!P000000000000000000000000@";
	static const char*	g_szCamBrd_CheckOverCurrent	= "!D000000000000000000000000@";
	static const char*	g_szCamBrd_LEDDutyControl	= "!R000000000000000000000000@";
	static const char*	g_szCamBrd_LED_ViselOn		= "!L100000000000000000000000@";
	static const char*	g_szCamBrd_LED_ViselOff		= "!L000000000000000000000000@";

	// 프로토콜 커맨드 
	typedef enum enCamBrdCmd
	{
		CMD_BoardCheck		= '0',
		CMD_SetVoltage		= 'V',
		CMD_GetInCurrent	= 'P',
		CMD_OverCurrent		= 'D',
		CMD_LEDDutyControl	= 'R',
		CMD_SetLED			= 'L',
	}enCamBrdCmd;

	typedef enum enCamCurrentCmd
	{
		CurrentCh1 = 0,
		CurrentCh2,
		CurrentCh3,
		CurrentCh4,
		CurrentCh5,
		CurrentMax,
	}enCamCurrentCmd;

	typedef struct _tag_CamBrdCurrent
	{
		float fOutCurrent[CurrentMax];

		_tag_CamBrdCurrent()
		{
			for (UINT nCh = 0; nCh < CurrentMax; nCh++)
				fOutCurrent[nCh] = FALSE;
		}

	}ST_CamBrdCurrent, *PST_CamBrdCurrent;


	typedef struct _tag_CamBrdProtocol
	{
		char		STX;
		char		CMD;
		char		Data[CamBrd_DataLength];
		char		ETX;

		_tag_CamBrdProtocol()
		{
			STX = CamBrd_STX;
			CMD = CamBrd_DummyChar;
			ETX = CamBrd_ETX;
			memset(Data, CamBrd_DummyChar, CamBrd_DataLength);
		};

		void MakeProtocol_SetVolt(__in BOOL bOnOff)
		{
			CMD = CMD_SetVoltage;

			if (bOnOff == TRUE)
			{
				Data[0] = '1';
			}
			else
			{
				Data[0] = '0';
			}
		};

		void MakeProtocol_SetLED(__in BOOL bOnOff)
		{
			CMD = CMD_SetLED;

			if (bOnOff == TRUE)
			{
				Data[0] = '1';
			}
			else
			{
				Data[0] = '0';
			}
		};

		void MakeProtocol_EtcSetLEDDutyControl(__in double dLEDDuty)
		{
			//ASSERT((Slot_All <= nSlotNo) && (nSlotNo <= Slot_E + 1));

			CMD = CMD_LEDDutyControl;
			sprintf_s(&(Data[0]), 5, "%04.0f", dLEDDuty * 100.0f);
			Data[4] = CamBrd_DummyChar;
		};

	}ST_CamBrdProtocol, *PST_CamBrdProtocol;

	typedef struct _tag_CamBrdRecvProtocol
	{
		char		STX;
		char		CMD;
		CStringA	Data;
		char		ETX;

		CStringA	Protocol;

		float		fCurrent[CurrentMax];
		BOOL		nOverCurrent;
		BOOL		bResult;

		_tag_CamBrdRecvProtocol()
		{
			for (UINT nCh = 0; nCh < CurrentMax; nCh++)
				fCurrent[nCh] = 0;

			nOverCurrent	= FALSE;
		};

		_tag_CamBrdRecvProtocol& operator= (_tag_CamBrdRecvProtocol& ref)
		{
			STX = ref.STX;
			CMD = ref.CMD;
			Data = ref.Data;
			ETX = ref.ETX;

			Protocol = ref.Protocol;

			return *this;
		};

		BOOL SetRecvProtocol(__in const char* pszProtocol, __in UINT_PTR nLength)
		{
			if (NULL == pszProtocol)
				return FALSE;

			if (nLength != CamBrd_ProtoLength)
				return FALSE;

			CStringA strProtocol = pszProtocol;

			INT_PTR nDataLength = nLength - 3; // STX, ETX, CMD
			INT_PTR nOffset = 0;

			STX  = pszProtocol[nOffset++];
			CMD  = pszProtocol[nOffset++];
			Data = strProtocol.Mid((int)nOffset++, (int)nDataLength);
			ETX  = pszProtocol[nOffset];

			switch (CMD)
			{
			case CMD_BoardCheck:
				AnalyzeData_BoardCheck();
				break;

			case CMD_SetVoltage:
				AnalyzeData_SetVolt();
				break;

			case CMD_GetInCurrent:
				AnalyzeData_Current();
				break;

			case CMD_OverCurrent:
				AnalyzeData_OverCurrent();
				break;
	
			default:
				break;
			}	

			// Check STX
			if (CamBrd_STX != STX)
				return FALSE;

			// Check ETX
			if (CamBrd_ETX != ETX)
				return FALSE;

			return TRUE;
		};

		void AnalyzeData_BoardCheck()
		{
			Data.ReleaseBuffer();
		};

		void AnalyzeData_SetVolt()
		{
			Data.ReleaseBuffer();
		};

		void AnalyzeData_Current()
		{
			// 기존 1234 mA -> 수정 123.4 mA (2018.01.29)

			CStringA szBuff;

			szBuff = Data.Left(4);
			fCurrent[CurrentCh1] = (float)atof(szBuff) / 10.0f;

			szBuff = Data.Mid(4, 4);
			fCurrent[CurrentCh2] = (float)atol(szBuff) / 10.0f;
			Data.ReleaseBuffer();

			szBuff = Data.Mid(8, 4);
			fCurrent[CurrentCh3] = (float)atol(szBuff) / 1000.0f;
			Data.ReleaseBuffer();

			szBuff = Data.Mid(12, 4);
			fCurrent[CurrentCh4] = (float)atol(szBuff) / 10.0f;
			Data.ReleaseBuffer();

			szBuff = Data.Mid(16, 4);
			fCurrent[CurrentCh5] = (float)atol(szBuff) / 10.0f;
			Data.ReleaseBuffer();
		};

		void AnalyzeData_OverCurrent()
		{
			CStringA szBuff;

			szBuff = Data.Left(1);

			if (1 == atoi(szBuff))
				nOverCurrent = TRUE;
			else
				nOverCurrent = FALSE;

			Data.ReleaseBuffer();
		};

	}ST_CamBrdRecvProtocol, *PST_CamBrdRecvProtocol;

};

#endif // Define_CamBrd_h__
