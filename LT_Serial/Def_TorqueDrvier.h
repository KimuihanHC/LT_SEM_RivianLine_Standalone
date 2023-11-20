//*****************************************************************************
// Filename	: 	Def_TorqueDrvier.h
// Created	:	2016/3/8 - 13:31
// Modified	:	2016/3/8 - 13:31
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
#ifndef Def_TorqueDrvier_h__
#define Def_TorqueDrvier_h__

#pragma pack(push, 1)

namespace TorqueDriver
{
	static const char	g_szToqueDriver_Version[7] = { 0x02, 'D', '0', '0', '1', '5', 0x03 }; //"/x2D001E/x3";

	typedef enum enTorque_Cmd
	{
		Cmd_Req_Monitor = 'M',
		Cmd_Rev_Monitor = 'm',
		Cmd_Req_Param = 'D',
		Cmd_Rev_Param = 'd',
	}enTorque_Cmd;

	typedef enum enTorque_ConstVar
	{
		Torque_Sehan_STX = 0x02,				// STX			: ┐
		Torque_Sehan_ETX = 0x03,				// ETX			: └
		Torque_Sehan_Delimeter = 0x44,		// Delimeter	: ,
		Torque_Sen_Protocol_Length = 5,		// 송신 프로토콜
		Torque_Rew_Protocol_Length = 0x56,		// 수신 프로토콜
	}enTorque_ConstVar;


	typedef enum enTorqueReadDataFormatIdx
	{
		TorqueRDF_SerialNo,					// 제품 시리얼 번호
		TorqueRDF_FasteningTime,			// 회전 시간
		TorqueRDF_PresetNo,					// Preset 번호
		TorqueRDF_Torque,					// 설정 토크 값
		TorqueRDF_ConvTorque,				// 체결 토크 값
		TorqueRDF_RPM,						// 회전 속도
		TorqueRDF_A1,						// A1 회전 수
		TorqueRDF_A2,						// A2 회전 수
		TorqueRDF_A3,						// A3 회전 수
		TorqueRDF_ErrorCode,				// 에러 코드
		TorqueRDF_Count,					// 체결 수량
		TorqueRDF_FastenLoosen,				// 체결(1), 풀림(0)
		TorqueRDF_Status,					// 체결 완료(1), 미완료, 공회전(0)
		TorqueRDF_CheckSum,					// 체크섬
		TorqueRDF_MaxEnum,
	}enTorqueReadDataFormatIdx;

	typedef struct _tag_TorqueMonitor
	{
		char		SeralNo[10];		// 제품 시리얼 번호			
		char		FasteningTime[5];	// 회전 시간
		char		PresetNo;			// Preset 번호
		char		Torque[5];			// 설정 토크 값
		char		ConvTorque[5];		// 체결 토크 값
		char		RPM[4];				// 회전 속도
		char		A1[4];				// A1 회전 수
		char		A2[4];				// A2 회전 수
		char		A3[4];				// A3 회전 수
		char		ErrorCode[3];		// 에러 코드
		char		Count[2];			// 체결 수량
		char		FastenLoosen;		// 체결(1), 풀림(0)
		char		Status;				// 체결 완료(1), 미완료, 공회전(0)

		_tag_TorqueMonitor()
		{
			memset(SeralNo, 0, 10);
			memset(FasteningTime, 0, 5);
			memset(Torque, 0, 5);
			memset(ConvTorque, 0, 5);
			memset(RPM, 0, 4);
			memset(A1, 0, 4);
			memset(A2, 0, 4);
			memset(A3, 0, 4);
			memset(ErrorCode, 0, 3);
			memset(Count, 0, 2);

			PresetNo = 0;
			FastenLoosen = 0;
			Status = 0;
		};

	}ST_TorqueMonitor, *PST_TorqueMonitor;

	typedef struct _tag_Torque_Protocol_Recv
	{
		char		STX;
		char		CMD;
		CStringA	Dataz;
		char		BCC;			// 체크섬
		char		ETX;

		ST_TorqueMonitor stMonitor;

		void Reset()
		{
			STX = Torque_Sehan_STX;
			CMD = Cmd_Rev_Param;
			BCC = 0;
			ETX = Torque_Sehan_ETX;
		};

		BOOL SetRecvProtocol(__in const char* pszProtocol, __in UINT_PTR nLength)
		{
			if (NULL == pszProtocol)
				return FALSE;

			Dataz.Empty();
			CStringA strProtocol = pszProtocol;

			INT_PTR nOffset = 0;

			STX = pszProtocol[nOffset++];
			CMD = pszProtocol[nOffset++];

			for (UINT nIdx = 0; nIdx < nLength - 4; nIdx++)
			{
				Dataz.AppendChar(pszProtocol[nOffset++]);
			}

			Dataz.Remove(',');

			BCC = pszProtocol[nOffset++];
			ETX = pszProtocol[nOffset];

			switch (CMD)
			{
			case Cmd_Rev_Monitor:
				AnalyzeData_Monitor();
				break;
			case Cmd_Rev_Param:
				break;
			default:
				break;
			}

			return TRUE;
		};

		void AnalyzeData_Monitor()
		{
			TRACE(_T("%d :%d \n"), sizeof(ST_TorqueMonitor), Dataz.GetLength());
			memcpy_s(&stMonitor, sizeof(ST_TorqueMonitor), Dataz.GetBuffer(0), Dataz.GetLength());
		};

		BOOL CheckBCC()
		{
			char chBCC = CMD;

			for (int iIdx = 0; iIdx < Dataz.GetLength(); iIdx++)
			{
				chBCC += Dataz.GetAt(iIdx);
			}

			chBCC = chBCC & 0xF;

			CStringA sz;
			sz.Format("%x", chBCC);

			chBCC = sz.GetAt(0);

			if (BCC == chBCC)
				return TRUE;
			else
				return FALSE;
		};

	}ST_TorqueProtocol_Recv, *PST_TorqueReadDataFormat;


	typedef struct _tag_TorqueProtocol
	{
		char		STX;
		char		CMD;
		CStringA	Dataz;
		char		BCC;			// 체크섬
		char		ETX;

		_tag_TorqueProtocol()
		{
			STX = Torque_Sehan_STX;
			CMD = Cmd_Req_Param;
			BCC = 0;
			ETX = Torque_Sehan_ETX;
		};

		_tag_TorqueProtocol& operator= (_tag_TorqueProtocol& ref)
		{
			return *this;
		};

		BOOL MakeBCC()
		{
			char chBCC = CMD;

			for (int iIdx = 0; iIdx < Dataz.GetLength(); iIdx++)
			{
				chBCC += Dataz.GetAt(iIdx);
			}

			chBCC = chBCC & 0xF;

			CStringA sz;
			sz.Format("%x", chBCC);

			chBCC = sz.GetAt(0);

			if (BCC == chBCC)
				return TRUE;
			else
				return FALSE;
		};

	}ST_TorqueProtocol, *PST_TorqueProtocol;

};

#pragma pack(pop)

#endif // Def_TorqueDrvier_h__
