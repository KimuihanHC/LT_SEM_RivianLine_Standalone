
#pragma once

namespace LED_PD_FN300
{
	#define  LED_ACK_STX				'R'
	#define	 LED_ACK_ETX				0x0A	// (LF)
	#define  LedPD_FN300_ProtoLength	6

	// protocol
	// <command><n1><n2><n3><CR><LF>

	typedef struct _tag_LedPD_FN300_Protocol
	{
		char		CMD;
		char		CH;
		char		Data[2];
		char		CR;
		char		LF;

		_tag_LedPD_FN300_Protocol()
		{
			Reset();
		};

		void MakeProtocol_SetChannelData(__in int channel, __in int data)
		{
			Reset();

			CMD = 'C';

			char sz[10] = { 0, };
			sprintf(sz, "%X", channel);
			CH = sz[0];

			sprintf(sz, "%02X", data);
			Data[0] = sz[0];
			Data[1] = sz[1];
		};

		void MakeProtocol_SetChannelOnOff(__in int channel, __in BOOL bOn)
		{
			Reset();

			CMD = 'H';

			char sz[10] = { 0, };
			sprintf(sz, "%X", channel);
			CH = sz[0];

			if(bOn)
				strcpy(sz, "ON");
			else
				strcpy(sz, "OF");

			Data[0] = sz[0];
			Data[1] = sz[1];
		};

		void MakeProtocol_GetChannelData(__in int channel)
		{
			Reset();

			CMD = 'S';

			char sz[10] = { 0, };
			sprintf(sz, "%X", channel);
			CH = sz[0];

			Data[0] = '0';
			Data[1] = '0';
		};

		void MakeProtocol_GetChannelOnOff(__in int channel)
		{
			Reset();

			CMD = 'S';

			char sz[10] = { 0, };
			sprintf(sz, "%X", channel);
			CH = sz[0];

			Data[0] = '0';
			Data[1] = '1';
		};

		void MakeProtocol_GetChannelCheck(__in int channel)
		{
			Reset();

			CMD = 'S';

			char sz[10] = { 0, };
			sprintf(sz, "%X", channel);
			CH = sz[0];

			Data[0] = '0';
			Data[1] = '2';
		};
		
		void Reset()
		{
			CMD = 0x00;
			CH = 0x00;
			memset(Data, 0x00, sizeof(Data));
			CR = 0x0d;
			LF = 0x0a;
		};

	}ST_LedPD_FN300_Protocol, *PST_LedPD_FN300_Protocol;

	typedef struct _tag_LedPD_FN300_Recv_Protocol
	{
		char		CMD;
		char		CH;
		CStringA	Data;
		char		CR;
		char		LF;

		int  nChLightValue;
		BOOL bChError;

		CStringA	Protocol;

		_tag_LedPD_FN300_Recv_Protocol()
		{
			Reset();
		};

		BOOL SetRecvProtocol(__in const char* pszProtocol, __in UINT_PTR nLength)
		{
			if (NULL == pszProtocol)
				return FALSE;

			CStringA strProtocol = pszProtocol;

			INT_PTR nOffset = 0;

			CMD = pszProtocol[nOffset++];
			CH = pszProtocol[nOffset++];
			Data = strProtocol.Mid((int)nOffset++, (int)2);
			nOffset++;
			CR = pszProtocol[nOffset++];
			LF = pszProtocol[nOffset++];

			return TRUE;
		}
		
		void Reset()
		{
			CMD = 0x00;
			CH = 0x00;
			Data = "";
			CR = 0x00;
			LF = 0x00;

			nChLightValue = 0;
			bChError = FALSE;
		};

	}ST_LedPD_FN300_Recv_Protocol, *PST_LedPD_FN300_Recv_Protocol;
}