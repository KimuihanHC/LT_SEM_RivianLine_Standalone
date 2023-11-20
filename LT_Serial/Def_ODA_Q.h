//*****************************************************************************
// Filename	: 	Def_ODA_Q.h
// Created	:	2017/11/7
// Modified	:	2017/11/7
//
// Author	:	KHO
//	
// Purpose	:	ODA Q Seriese - Programmable DC Power Supply
//*****************************************************************************
#ifndef Def_ODA_PT_h__
#define Def_ODA_PT_h__

//=============================================================================
// ODA Q Seriese - Programmable DC Power Supply 통신 프로토콜
//=============================================================================
/*
4-1. Commands Syntax
▌입력은 영문 대/소문자 구분 없이 사용할 수 있습니다.
▌공백(20H) 또는 탭(09H)의 갯수는 제한이 없으며 최소 1개 이상만 주면 됩니다.
▌명령어는 한번에 한 명령어만 주어질 수 있습니다
▌Braces({ })안의 parameters는 생략 불가능 합니다
▌분할 바( | )는 2개 또는 그 이상의 parameter에서 택일 하는 것을 의미합니다.
▌삼각 브라켓(< >)은 변경 가능한 값입니다.
▌명령의 종결은 LF(0AH)입니다.
▌최대 한번에 전송할 수 있는 문자열은 50 Byte입니다.
▌RS485통신일 경우 "ODA" + 1byte address(01H ~ FFH) + SCPI Protocol로 이루어 집니다.
▌RS485통신의 Query(질의)문에서 Return문자열은 RS232C방식과 같습니다.(address포함안됨)
*/

#include <afxwin.h>

#pragma pack(push, 1)

namespace ODA_Q
{
	#define	ODA_Q_ETX				0x0A	// LF
	#define ODA_Q_Delimeter			','		// 파라미터 구분자
	#define ODA_Q_Space				0x20	// ' ' 공백
	#define ODA_Q_ProtoLength		50
	#define ODA_Q_MaxLength			50

	// 프로토콜 커맨드 
	typedef enum enODA_Q_Command
	{
		CMD_Q_VOLT,
		CMD_Q_VOLT_Req,
		CMD_Q_CURR,
		CMD_Q_CURR_Req,
		CMD_Q_MEAS_VOLT_Req,
		CMD_Q_MEAS_CURR_Req,
		CMD_Q_SYST_IDN_Req,
		CMD_Q_SYST_OUTP,
		CMD_Q_SYST_OUTP_Req,
		CMD_Q_CHANNEL,
		CMD_MaxEnum,
	}enODA_Q_Command;

	static char* g_szCommand[CMD_MaxEnum] =
	{
		"VOLT",						// CMD_Q_VOLT,
		"VOLT?",					// CMD_Q_VOLT_Req,
		"CURR",						// CMD_Q_CURR,
		"CURR?",					// CMD_Q_CURR_Req,
		"MEAS:VOLT?",				// CMD_Q_MEAS_VOLT_Req,
		"MEAS:CURR?",				// CMD_Q_MEAS_CURR_Req,
		"*IDN?",					// CMD_Q_SYST_IDN_Req,
		"OUTP",						// CMD_Q_SYST_OUTP,
		"OUTP?",					// CMD_Q_SYST_OUTP_Req,
		"Inst",						// CMD_Q_CHANNEL
	};

	typedef enum enSwitchOnOff
	{
		Switch_ON,
		Switch_OFF,
	}enSwitchOnOff;

	static char* g_szOnOff[] =
	{
		"ON",
		"OFF",
	};

	//-------------------------------------------------------------------------
	// 전송용 프로토콜
	//-------------------------------------------------------------------------
	typedef struct _tag_ODA_Q_Protocol
	{
		CStringA	Command;
		CStringA	Parameter;
		char		ETX;

		CStringA	Protocol;

		_tag_ODA_Q_Protocol()
		{
			ETX = ODA_Q_ETX;
		};

		void CombineProtocol()
		{
			Protocol.Empty();

			Protocol.Append(Command);

			if (FALSE == Parameter.IsEmpty())
			{
				Protocol.AppendChar(ODA_Q_Space);
				Protocol.Append(Parameter);
			}

			Protocol.AppendChar(ETX);
		};

		void MakeProtocol(__in enODA_Q_Command nCommand)
		{
			Parameter.Empty();

			Command = g_szCommand[nCommand];

			// 프로토콜 결합
			CombineProtocol();
		}

		void MakeProtocol_Channel(__in UINT nChannel)
		{
			Parameter.Empty();

			Command = g_szCommand[CMD_Q_CHANNEL];
			Parameter.Format("OUTP%d", nChannel);

			// 프로토콜 결합
			CombineProtocol();
		};

		void MakeProtocol_Volt(__in float fVoltage)
		{
			Parameter.Empty();

			Command = g_szCommand[CMD_Q_VOLT];
			Parameter.Format("%.1f", fVoltage);

			// 프로토콜 결합
			CombineProtocol();
		};

		void MakeProtocol_Curr(__in float fCurrent)
		{
			Parameter.Empty();

			Command = g_szCommand[CMD_Q_CURR];
			Parameter.Format("%.1f", fCurrent);

			// 프로토콜 결합
			CombineProtocol();
		};

		void MakeProtocol_Output(__in enSwitchOnOff nOutOnOff)
		{
			Parameter.Empty();

			Command = g_szCommand[CMD_Q_SYST_OUTP];
			Parameter = g_szOnOff[nOutOnOff];

			// 프로토콜 결합
			CombineProtocol();
		};

	}ST_ODA_Q_Protocol, *PST_ODA_Q_Protocol;

	//-------------------------------------------------------------------------
	// 수신용 프로토콜
	//-------------------------------------------------------------------------
	typedef struct _tag_ODA_Q_RecvProtocol
	{
		enODA_Q_Command		CommandID;				// 프로토콜 전송 할 때 커맨드 설정용도
		CString				Protocol;

		BOOL				bPass			= TRUE;	// 수신 데이터 정상/오류 구분

		CStringArray		szarTokenz;				// 수신된 데이터를 분리

		int					iErrorCode		= 0;	// 오류 코드 : 0이면 정상
		CString				ErrorDesc;				// 오류 문자열
		CString				Version;				// Version

		float				fVoltage		= 0.0f;	// 설정된 전압값
		float				fCurrent		= 0.0f;	// 설정된 전류값
		float				fMeasVoltage	= 0.0f;	// 측정된 전압값
		float				fMeasCurrent	= 0.0f;	// 측정된 전류값

		enSwitchOnOff		nOut_OnOff		= enSwitchOnOff::Switch_OFF;
		enSwitchOnOff		nKey_OnOff		= enSwitchOnOff::Switch_OFF;

		_tag_ODA_Q_RecvProtocol()
		{
		};

		_tag_ODA_Q_RecvProtocol& operator= (const _tag_ODA_Q_RecvProtocol& ref)
		{
			CommandID	= ref.CommandID;
			Protocol	= ref.Protocol;

			return *this;
		};

		// 수신 데이터를 ',' 기준으로 분리함
		BOOL TokenizeProtocol()
		{
			CString		szTemp = Protocol;
			CString		szToken;
			int			curPos = 0;

			szarTokenz.RemoveAll();

			do
			{
				szToken = szTemp.Tokenize(_T(","), curPos);

				if (szToken != _T(""))
				{
					szarTokenz.Add(szToken);

					TRACE(_T("[%02d] %s\n"), szarTokenz.GetCount(), szToken);
				}
			} while (szToken != _T(""));

			return TRUE;
		};

		// 수신된 데이터 설정
		BOOL SetRecvProtocol(__in const char* pszProtocol, __in UINT_PTR nLength)
		{
			if (NULL == pszProtocol)
				return FALSE;

			USES_CONVERSION;
			Protocol = CA2T(pszProtocol);

			INT_PTR nDataLength = nLength - 1; // ETX
			INT_PTR nOffset = 0;

			Protocol.Remove(0x0A);
			Protocol.Remove(0x0D);

			switch (CommandID)
			{
			case ODA_Q::CMD_Q_VOLT_Req:
				bPass = TRUE;
				fVoltage = (float)_ttof(Protocol.GetBuffer(0));
				break;
			case ODA_Q::CMD_Q_CURR_Req:
				bPass = TRUE;
				fCurrent = (float)_ttof(Protocol.GetBuffer(0));
				break;
			case ODA_Q::CMD_Q_MEAS_VOLT_Req:
				bPass = TRUE;
				fMeasVoltage = (float)_ttof(Protocol.GetBuffer(0));
				break;
			case ODA_Q::CMD_Q_MEAS_CURR_Req:
				bPass = TRUE;
				fMeasCurrent = (float)_ttof(Protocol.GetBuffer(0));
				break;
			case ODA_Q::CMD_Q_SYST_IDN_Req:
				bPass = TRUE;
				Version = Protocol;
				break;
			case ODA_Q::CMD_Q_SYST_OUTP_Req:
				bPass = TRUE;

				if (0 == Protocol.Compare(_T("0")))
				{
					nOut_OnOff = enSwitchOnOff::Switch_OFF;
				}
				else if (0 == Protocol.Compare(_T("1")))
				{
					nOut_OnOff = enSwitchOnOff::Switch_ON;
				}
				else
				{
					bPass = FALSE;
				}
				break;
			default:
				break;
			}

			return TRUE;
		};
	}ST_ODA_Q_RecvProtocol, *PST_ODA_Q_RecvProtocol;

};

#pragma pack (pop)

#endif // Def_ODA_Q_h__
