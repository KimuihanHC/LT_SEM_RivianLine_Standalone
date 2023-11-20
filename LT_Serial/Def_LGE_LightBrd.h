//*****************************************************************************
// Filename	: 	Def_LGE_LightBrd.h
// Created	:	2016/3/8 - 13:31
// Modified	:	2016/3/8 - 13:31
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
#ifndef Def_LGE_LightBrd_h__
#define Def_LGE_LightBrd_h__

//=============================================================================
// 광원 제어 보드 통신 프로토콜
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

#include <afxwin.h>

#pragma pack(push, 1)

namespace LGE_LightControl
{
	#define	LightBrd_STX				0x21	//'!'
	#define	LightBrd_ETX				0x40	//'@'
	#define LightBrd_DummyChar			0x30	//'0'
	#define LightBrd_ProtoLength		23
	#define LightBrd_DataLength			20

	// 고정된 통신 프로토콜 선언
	static const char* g_szLightBrd_PortCheck		= "!000000000000000000000@";
	static const char* g_szLightBrd_DetectVolt		= "!W00000000000000000000@";
	static const char* g_szLightBrd_DetectCurrent	= "!400000000000000000000@";
	static const char* g_szLightBrd_ControlCheck	= "!C00000000000000000000@";

	// 프로토콜 커맨드 
	typedef enum enLightBrdCmd
	{
		CMD_PortCheck		= '0',
		CMD_EtcSetVolt		= 'U',
		CMD_DetectVolt		= 'W',
		CMD_EtcSetCurrent	= '5',
		CMD_DetectCurrent	= '4',
		CMD_ControlCheck	= 'C',
	}enLightBrdCmd;

	typedef enum enSlot
	{
		Slot_All,
		Slot_A,
		Slot_B,
		Slot_C,
		Slot_D,
		Slot_E,
		Slot_Max = Slot_E,
	}enSlot;

	typedef struct _tag_SlotInfo
	{
		float fVolt[Slot_Max];
		float fCurrent[Slot_Max];

		_tag_SlotInfo()
		{
			Reset();
		};

		_tag_SlotInfo& operator= (_tag_SlotInfo& ref)
		{
			for (UINT nIdx = 0; nIdx < Slot_Max; nIdx++)
			{
				fVolt[nIdx]		= ref.fVolt[nIdx];
				fCurrent[nIdx]	= ref.fCurrent[nIdx];
			}

			return *this;
		};

		void Reset()
		{
			for (UINT nIdx = 0; nIdx < Slot_Max; nIdx++)
			{
				fVolt[nIdx] = 0.0f;
				fCurrent[nIdx] = 0.0f;
			}
		};

	}ST_SlotInfo;

	typedef struct _tag_LightBrdProtocol
	{
		char		STX;
		char		CMD;
		char		Data[LightBrd_DataLength];
		char		ETX;

		_tag_LightBrdProtocol()
		{
			STX = LightBrd_STX;
			CMD = LightBrd_DummyChar;
			ETX = LightBrd_ETX;
			memset(Data, LightBrd_DummyChar, LightBrd_DataLength);
		};

		void MakeProtocol_EtcSetVolt(__in UINT nSlotNo, __in float fVolt)
		{
			ASSERT((Slot_All <= nSlotNo) && (nSlotNo <= Slot_E+1));

			CMD = CMD_EtcSetVolt;
			Data[0] = nSlotNo + 0x30;
			sprintf_s(&(Data[1]), 5, "%04.0f", fVolt * 100.0f);
			Data[5] = LightBrd_DummyChar;
		};

		void MakeProtocol_EtcSetCurrent(__in UINT nSlotNo, __in float fCurrent)
		{
			ASSERT((Slot_All <= nSlotNo) && (nSlotNo <= Slot_E + 1));

			CMD = CMD_EtcSetCurrent;
			Data[0] = nSlotNo + 0x30;
			sprintf_s(&(Data[1]), 5, "%04.0f", fCurrent * 1.0f);
			Data[5] = LightBrd_DummyChar;
		};

	}ST_LightBrdProtocol, *PST_LightBrdProtocol;

	typedef struct _tag_LightBrdRecvProtocol
	{
		char		STX;
		char		CMD;
		CStringA	Data;
		char		ETX;

		CStringA	Protocol;

		ST_SlotInfo RecvedSlotVolt;

		BOOL		bAck;
		BOOL		bAutomatic;
		BOOL		bManual;

		_tag_LightBrdRecvProtocol()
		{
			bAck		= TRUE;
			bAutomatic	= FALSE;
			bManual		= FALSE;
		};

		_tag_LightBrdRecvProtocol& operator= (_tag_LightBrdRecvProtocol& ref)
		{
			STX				= ref.STX;
			CMD				= ref.CMD;
			Data			= ref.Data;
			ETX				= ref.ETX;

			Protocol		= ref.Protocol;

			RecvedSlotVolt	= ref.RecvedSlotVolt;
			bAck			= ref.bAck;
			bAutomatic		= ref.bAutomatic;
			bManual			= ref.bManual;

			return *this;
		};

		BOOL SetRecvProtocol(__in const char* pszProtocol, __in UINT_PTR nLength)
		{
			if (NULL == pszProtocol)
				return FALSE;

			if (nLength != LightBrd_ProtoLength)
				return FALSE;

			CStringA strProtocol = pszProtocol;

			INT_PTR nDataLength = nLength - 3; // STX, ETX, CMD
			INT_PTR nOffset = 0;

			STX		= pszProtocol[nOffset++];
			CMD		= pszProtocol[nOffset++];
			Data	= strProtocol.Mid((int)nOffset++, (int)nDataLength);
			ETX		= pszProtocol[nOffset];

			switch (CMD)
			{
			case CMD_PortCheck:
				AnalyzeData_PortCheck();
				break;

			case CMD_EtcSetVolt:
				AnalyzeData_PortCheck();
				break;

			case CMD_DetectVolt:
				AnalyzeData_DetectVolt();
				break;

			case CMD_EtcSetCurrent:
				AnalyzeData_PortCheck();
				break;

			case CMD_DetectCurrent:
				AnalyzeData_DetectCurrent();
				break;

			case CMD_ControlCheck:
				AnalyzeData_ControlCheck();
				break;
			}

			// Check STX
			if (LightBrd_STX != STX)
				return FALSE;

			// Check ETX
			if (LightBrd_ETX != ETX)
				return FALSE;

			return TRUE;
		};

		void AnalyzeData_PortCheck()
		{
			if ('1' == Data.GetAt(0))
			{
				bAck = TRUE;
			}
			else
			{
				bAck = FALSE;
			}
		};

		void AnalyzeData_DetectVolt()
		{
			CStringA szBuff;

			UINT nOffset = 0;
			for (UINT nSlotIdx = 0; nSlotIdx < Slot_E; nSlotIdx++)
			{
				szBuff = Data.Mid(nOffset, nOffset + 4);

				RecvedSlotVolt.fVolt[nSlotIdx] = (float)atof(szBuff.GetBuffer());
				szBuff.ReleaseBuffer();
			}
		};

		void AnalyzeData_DetectCurrent()
		{
			CStringA szBuff;

			UINT nOffset = 0;
			for (UINT nSlotIdx = 0; nSlotIdx < Slot_E; nSlotIdx++)
			{
				szBuff = Data.Mid(nOffset, nOffset + 4);

				RecvedSlotVolt.fCurrent[nSlotIdx] = (float)atof(szBuff.GetBuffer());
				szBuff.ReleaseBuffer();
			}
		};

		void AnalyzeData_ControlCheck()
		{
			if ('1' == Data.GetAt(0))
			{
				bAutomatic = TRUE;
			}
			else
			{
				bAutomatic = FALSE;
			}

			if ('1' == Data.GetAt(1))
			{
				bManual = TRUE;
			}
			else
			{
				bManual = FALSE;
			}
		};
	}ST_LightBrdRecvProtocol, *PST_LightBrdRecvProtocol;

};

#pragma pack (pop)

#endif // Def_LGE_LightBrd_h__
