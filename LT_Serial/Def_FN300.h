//*****************************************************************************
// Filename	: 	Def_Keyence_Displace.h
// Created	:	2017/10/29 - 15:32
// Modified	:	2017/10/29 - 15:32
//
// Author	:	PiRing
//	
// Purpose	:	FN300 13CH 광원컨트롤러
//*****************************************************************************
#ifndef Def_FN300_h__
#define Def_FN300_h__

#include <afxwin.h>


namespace FN300
{
	typedef enum _ConstVar
	{
		FN300_CR			= 0x0D,	// CR
		FN300_LF			= 0x0A,	// LF
		FN300_DummyChar		= 0x30,	//'0'
		FN300_DataLength	= 3,
		FN300_ProtoLength	= 6,
	}_ConstVar;

	typedef enum _Command
	{
		FN300_Recieve	= 'R',	// CR
		FN300_DATA		= 'C',
		FN300_OnOff		= 'H',
	}_Command;

	// 고정 프로토콜
	static const char* g_szChannelData	= "CT00" + FN300_CR + FN300_LF; // All Channel 
	static const char* g_szChannelOn	= "HTON" + FN300_CR + FN300_LF; // All Channel 
	static const char* g_szChannelOff	= "HTOF" + FN300_CR + FN300_LF; // All Channel 

	//-------------------------------------------------------------------------
	// 전송용 프로토콜
	//-------------------------------------------------------------------------

	typedef struct _tag_SendProtocol_FN300
	{
		char		CMD;
		char		DATA[FN300_DataLength];
		char		CR;
		char		LF;

		_tag_SendProtocol_FN300()
		{
			CMD = FN300_DATA;
			CR	= FN300_CR;
			LF	= FN300_LF;
			memset(DATA, FN300_DummyChar, FN300_DataLength);
		};

		void MakeProtocol_ChannelData(__in UINT nCH, __in BYTE xData)
		{
			CMD = FN300_DATA;

			char aData[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
			
			// Channel
			DATA[0] = aData[nCH];

			// Data
			BYTE xBuf1 = (xData & 0xf0) >> 4;
			BYTE xBuf2 = xData & 0x0f;


			DATA[1] = aData[xBuf1];
			DATA[2] = aData[xBuf2];

		};

		void MakeProtocol_ChannelOnOff(__in UINT nCH, __in BOOL bOnOff)
		{
			CMD = FN300_OnOff;

			char aData[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

			// Channel
			DATA[0] = aData[nCH];

			// Data
			if (bOnOff == TRUE)
			{
				DATA[1] = 'O';
				DATA[2] = 'N';
			}
			else
			{
				DATA[1] = 'O';
				DATA[2] = 'F';
			}
		};

		

	}ST_SendProtocol_FN300, *PST_SendProtocol_FN300;


	typedef struct _tag_RecvProtocol_FN300
	{
		CStringA Dataz;
		BOOL bResult;

		_tag_RecvProtocol_FN300()
		{
			bResult = FALSE;
		};

		_tag_RecvProtocol_FN300& operator= (_tag_RecvProtocol_FN300& ref)
		{
			Dataz = ref.Dataz;
			bResult = ref.bResult;

			return *this;
		};

		BOOL SetRecvProtocol(__in const char* pszProtocol, __in UINT_PTR nLength)
		{
			// ER 이면 에러
			if (NULL == pszProtocol)
				return bResult = FALSE;

			if (FN300_ProtoLength != nLength)
				return bResult = FALSE;

			CStringA strProtocol = pszProtocol;
			Dataz = strProtocol.Mid(2, 2);

			if (Dataz == "ER")
			{
				return bResult = FALSE;
			}
			
			return bResult = TRUE;
		};

	}ST_RecvProtocol_FN300, *PST_RecvProtocol_FN300;
};




#endif // Def_FN300_h__
