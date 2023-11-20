//*****************************************************************************
// Filename	: 	Def_ODA_PT.h
// Created	:	2017/11/7
// Modified	:	2017/11/7
//
// Author	:	PiRing
//	
// Purpose	:	ODA PT Seriese - Programmable DC Power Supply
//*****************************************************************************
#ifndef Def_ODA_PT_h__
#define Def_ODA_PT_h__

//=============================================================================
// ODA PT Seriese - Programmable DC Power Supply 통신 프로토콜
//=============================================================================
/*
6-1. Commands Syntax
▌영문 소문자는 생략 가능합니다.
▌입력은 영문 대/소문자 구분 없이 사용할 수 있습니다.
▌공백(20H) 또는 탭(09H)의 갯수는 제한이 없으며 최소 1개 이상만 주면 됩니다.
▌명령어는 한번에 한 명령어만 주어질 수 있습니다
▌사각 브라켓([ ]) 은 옵션 또는 parameters이며 생략 가능 합니다
▌Braces({ })안의 parameters는 생략 불가능 합니다
▌삼각 브라켓(< >)은 변경 가능한 값이나 CODE(예:MIN,MAX)로 대치할 수 있습니다.
▌분할 바( | )는 2개 또는 그 이상의 parameter에서 택일 하는 것을 의미합니다.
▌명령 종결의문자는 기본LF이며,CR,CRLF로 변경할 수 있습니다.
▌최대 한번에 전송할 수 있는 문자열은 40 Byte입니다.
▌RS485통신일 경우 "ODA" + 1byte address(01H ~ FFH) + SCPI Protocol로 이루어 집니다.
▌RS485통신의 Query(질의)문에서 Return문자열은 RS232C방식과 같습니다.(address포함안됨)
*/

#include <afxwin.h>

#pragma pack(push, 1)

namespace ODA_PT
{
	#define	ODA_PT_ETX					0x0A	// LF
	#define ODA_PT_Delimeter			','		// 파라미터 구분자
	#define ODA_PT_Space				0x20	// ' ' 공백
	#define ODA_PT_ProtoLength			40
	#define ODA_PT_MaxLength			40

	// 고정된 통신 프로토콜 선언
	static const char* g_szODA_PT_Version		= "SYST:VERS?\0A";

	// 프로토콜 커맨드 
	typedef enum enODA_PT_Command
	{
		CMD_APPLy,
		CMD_APPLy_Req,
		CMD_VOLT,
		CMD_VOLT_Req,
		CMD_VOLT_STEP,
		CMD_VOLT_STEP_Req,
		CMD_VOLT_OVP,
		CMD_VOLT_OVP_Req,
		CMD_VOLT_OVP_TRIP_Req,
		CMD_VOLT_OVP_CLE_Req,
		CMD_VOLT_UVL,
		CMD_VOLT_UVL_Req,
		CMD_VOLT_OVL,
		CMD_VOLT_OVL_Req,
		CMD_CURR,
		CMD_CURR_Req,
		CMD_CURR_STEP,
		CMD_CURR_STEP_Req,
		CMD_CURR_OCP,
		CMD_CURR_OCP_Req,
		CMD_CURR_OCP_TRIP_Req,
		CMD_CURR_OCP_CLE_Req,
		CMD_CURR_UCL,
		CMD_CURR_UCL_Req,
		CMD_CURR_OCL,
		CMD_CURR_OCL_Req,
		CMD_FLOW_Req,
		CMD_POL,
		CMD_POL_Req,
		CMD_MEAS_CURR_Req,
		CMD_MEAS_VOLT_Req,
		CMD_MEAS_ALL_Req,
		CMD_SYST_BEEP,
		CMD_SYST_ERR_Req,
		CMD_SYST_VERS_Req,
		CMD_SYST_OUTP,
		CMD_SYST_OUTP_Req,
		CMD_SYST_KEYL,
		CMD_SYST_KEYL_Req,

		CMD_MaxEnum,
	}enODA_PT_Command;

	static char* g_szCommand[CMD_MaxEnum] =
	{
		"APPLy",
		"APPLy?",
		"VOLT",
		"VOLT?",
		"VOLT:STEP",
		"VOLT:STEP?",
		"VOLT:OVP",
		"VOLT:OVP?",
		"VOLT:OVP:TRIP?",
		"VOLT:OVP:CLE?",
		"VOLT:UVL",
		"VOLT:UVL?",
		"VOLT:OVL",
		"VOLT:OVL?",
		"CURR",
		"CURR?",
		"CURR:STEP",
		"CURR:STEP?",
		"CURR:OCP",
		"CURR:OCP?",
		"CURR:OCP:TRIP?",
		"CURR:OCP:CLE?",
		"CURR:UCL",
		"CURR:UCL?",
		"CURR:OCL",
		"CURR:OCL?",
		"FLOW?",
		"POL",
		"POL?",
		"MEAS:CURR?",
		"MEAS:VOLT?",
		"MEAS:ALL?",
		"SYST:BEEP",
		"SYST:ERR?",
		"SYST:VERS?",
		"OUTP",
		"OUTP?",
		"KEYL",
		"KEYL?",
	};

	typedef enum enUpDown
	{
		STEP_UP,
		STEP_DOWN,
	}enUpDown;

	static char* g_szUpDown[] =
	{
		"UP",
		"DOWN",
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

	typedef enum enCV_CC
	{
		Const_CV,
		Const_CC,
	}enCV_CC;

	static LPCTSTR g_szCV_CC[] =
	{
		_T("CV"),
		_T("CC"),
	};

	//-------------------------------------------------------------------------
	// 전송용 프로토콜
	//-------------------------------------------------------------------------
	typedef struct _tag_ODA_PT_Protocol
	{
		CStringA	Command;
		CStringA	Parameter;
		char		ETX;

		CStringA	Protocol;

		_tag_ODA_PT_Protocol()
		{
			ETX = ODA_PT_ETX;
		};

		void CombineProtocol()
		{
			Protocol.Empty();

			Protocol.Append(Command);

			if (FALSE == Parameter.IsEmpty())
			{
				Protocol.AppendChar(ODA_PT_Space);
				Protocol.Append(Parameter);
			}

			Protocol.AppendChar(ETX);
		};

		void MakeProtocol(__in enODA_PT_Command nCommand)
		{
			Parameter.Empty();

			Command = g_szCommand[nCommand];

			// 프로토콜 결합
			CombineProtocol();
		}

		void MakeProtocol_Apply(__in float fVoltage, __in float fCurrent)
		{
			Parameter.Empty();
			
			Command = g_szCommand[CMD_APPLy];
			Parameter.Format("%.4f,%.4f", fVoltage, fCurrent);
			
			// 프로토콜 결합
			CombineProtocol();
		};

		void MakeProtocol_Apply(__in float fVoltage)
		{
			Parameter.Empty();

			Command = g_szCommand[CMD_APPLy];
			Parameter.Format("%.4f", fVoltage);

			// 프로토콜 결합
			CombineProtocol();
		};

		void MakeProtocol_Volt(__in float fVoltage)
		{
			Parameter.Empty();

			Command = g_szCommand[CMD_VOLT];
			Parameter.Format("%.4f", fVoltage);

			// 프로토콜 결합
			CombineProtocol();
		};

		void MakeProtocol_Volt(__in enUpDown nStepUpDown)
		{
			Parameter.Empty();

			Command = g_szCommand[CMD_VOLT];
			Parameter = g_szUpDown[nStepUpDown];

			// 프로토콜 결합
			CombineProtocol();
		};

		void MakeProtocol_VoltStep(__in float fCurrent)
		{
			Parameter.Empty();

			Command = g_szCommand[CMD_VOLT_STEP];
			Parameter.Format("%.4f", fCurrent);

			// 프로토콜 결합
			CombineProtocol();
		};

		void MakeProtocol_Curr(__in float fCurrent)
		{
			Parameter.Empty();

			Command = g_szCommand[CMD_CURR];
			Parameter.Format("%.4f", fCurrent);

			// 프로토콜 결합
			CombineProtocol();
		};

		void MakeProtocol_Curr(__in enUpDown nStepUpDown)
		{
			Parameter.Empty();

			Command = g_szCommand[CMD_CURR];
			Parameter = g_szUpDown[nStepUpDown];

			// 프로토콜 결합
			CombineProtocol();
		};

		void MakeProtocol_CurrStep(__in float fCurrent)
		{
			Parameter.Empty();

			Command = g_szCommand[CMD_CURR_STEP];
			Parameter.Format("%.4f", fCurrent);

			// 프로토콜 결합
			CombineProtocol();
		};

		void MakeProtocol_Output(__in enSwitchOnOff nOutOnOff)
		{
			Parameter.Empty();

			Command = g_szCommand[CMD_SYST_OUTP];
			Parameter = g_szOnOff[nOutOnOff];

			// 프로토콜 결합
			CombineProtocol();
		};

		void MakeProtocol_KeyLock(__in enSwitchOnOff nKeyOnOff)
		{
			Parameter.Empty();

			Command = g_szCommand[CMD_SYST_KEYL];
			Parameter = g_szOnOff[nKeyOnOff];

			// 프로토콜 결합
			CombineProtocol();
		};
	}ST_ODA_PT_Protocol, *PST_ODA_PT_Protocol;

	//-------------------------------------------------------------------------
	// 수신용 프로토콜
	//-------------------------------------------------------------------------
	typedef struct _tag_ODA_PT_RecvProtocol
	{
		enODA_PT_Command	CommandID;				// 프로토콜 전송 할 때 커맨드 설정용도
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

		float				fVoltageStep	= 0.0f;	// 설정된 스텝 전압값
		float				fCurrentStep	= 0.0f;	// 설정된 스텝 전류값
		
		enCV_CC				nCVCCStatus		= Const_CC;
		enSwitchOnOff		nOut_OnOff		= enSwitchOnOff::Switch_OFF;
		enSwitchOnOff		nKey_OnOff		= enSwitchOnOff::Switch_OFF;

		_tag_ODA_PT_RecvProtocol()
		{
		};

		_tag_ODA_PT_RecvProtocol& operator= (const _tag_ODA_PT_RecvProtocol& ref)
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
			case ODA_PT::CMD_APPLy_Req:	// Voltage,Current
			{
				bPass = FALSE;
				fVoltage = 0.0f;
				fCurrent = 0.0f;
				if (TokenizeProtocol())
				{
					if (2 <= szarTokenz.GetCount())
					{
						fVoltage = (float)_ttof(szarTokenz[0].GetBuffer(0));
						fCurrent = (float)_ttof(szarTokenz[1].GetBuffer(0));

						bPass = TRUE;
					}
				}
			}
				break;

			case ODA_PT::CMD_VOLT_Req:	// Voltage
			{
				bPass = TRUE;
				fVoltage = (float)_ttof(Protocol.GetBuffer(0));
			}
				break;

			case ODA_PT::CMD_VOLT_STEP_Req:
			{
				bPass = TRUE;
				fVoltageStep = (float)_ttof(Protocol.GetBuffer(0));
			}
				break;

			case ODA_PT::CMD_VOLT_OVP_Req:
				break;

			case ODA_PT::CMD_VOLT_OVP_TRIP_Req:
				break;

			case ODA_PT::CMD_VOLT_OVP_CLE_Req:
				break;

			case ODA_PT::CMD_VOLT_UVL_Req:
				break;

			case ODA_PT::CMD_VOLT_OVL_Req:
				break;

			case ODA_PT::CMD_CURR_Req:
			{
				bPass = TRUE;
				fCurrent = (float)_ttof(Protocol.GetBuffer(0));
			}
				break;

			case ODA_PT::CMD_CURR_STEP_Req:
			{
				bPass = TRUE;
				fCurrentStep = (float)_ttof(Protocol.GetBuffer(0));
			}
				break;

			case ODA_PT::CMD_CURR_OCP_Req:
				break;

			case ODA_PT::CMD_CURR_OCP_TRIP_Req:
				break;

			case ODA_PT::CMD_CURR_OCP_CLE_Req:
				break;

			case ODA_PT::CMD_CURR_UCL_Req:
				break;

			case ODA_PT::CMD_CURR_OCL_Req:
				break;

			case ODA_PT::CMD_FLOW_Req:	// CV, CC 상태
			{
				bPass = TRUE;

				if (0 == Protocol.Compare(g_szCV_CC[Const_CV]))
				{
					nCVCCStatus = Const_CC;
				}
				else if (0 == Protocol.Compare(g_szCV_CC[Const_CV]))
				{
					nCVCCStatus = Const_CV;
				}
				else
				{
					bPass = FALSE;
				}
			}
				break;

			case ODA_PT::CMD_POL_Req:
				break;

			case ODA_PT::CMD_MEAS_CURR_Req:
			{
				bPass = TRUE;
				fMeasCurrent = (float)_ttof(Protocol.GetBuffer(0));
			}
				break;

			case ODA_PT::CMD_MEAS_VOLT_Req:
			{
				bPass = TRUE;
				fMeasVoltage = (float)_ttof(Protocol.GetBuffer(0));
			}
				break;

			case ODA_PT::CMD_MEAS_ALL_Req:
			{
				bPass = FALSE;
				fMeasVoltage = 0.0f;
				fMeasCurrent = 0.0f;
				if (TokenizeProtocol())
				{
					if (2 <= szarTokenz.GetCount())
					{
						fMeasVoltage = (float)_ttof(szarTokenz[0].GetBuffer(0));
						fMeasCurrent = (float)_ttof(szarTokenz[1].GetBuffer(0));

						bPass = TRUE;
					}
				}
			}
				break;

			case ODA_PT::CMD_SYST_ERR_Req:
			{
				bPass = FALSE;
				iErrorCode = 0;
				ErrorDesc.Empty();
				if (TokenizeProtocol())
				{
					if (2 <= szarTokenz.GetCount())
					{
						iErrorCode	= _ttoi(szarTokenz[0].GetBuffer(0));
						ErrorDesc	= szarTokenz.GetAt(1);

						bPass = TRUE;
					}
				}
			}
				break;

			case ODA_PT::CMD_SYST_VERS_Req:
			{
				bPass = TRUE;
				Version = Protocol;
			}
				break;

			case ODA_PT::CMD_SYST_OUTP_Req:
			{
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
			}
				break;

			case ODA_PT::CMD_SYST_KEYL_Req:
			{
				bPass = TRUE;

				if (0 == Protocol.Compare(_T("0")))
				{
					nKey_OnOff = enSwitchOnOff::Switch_OFF;
				}
				else if (0 == Protocol.Compare(_T("1")))
				{
					nKey_OnOff = enSwitchOnOff::Switch_ON;
				}
				else
				{
					bPass = FALSE;
				}
			}
				break;

			default:
				break;
			}

			return TRUE;
		};
	}ST_ODA_PT_RecvProtocol, *PST_ODA_PT_RecvProtocol;



	/*
	= Output Setting Commands =

	APPLy {<voltage>}[,<current>] VOLT:UVL {<numeric value>}
	APPLy? VOLT:UVL?
	VOLT {<voltage>|UP|DOWN} VOLT:OVL {<numeric value>
	VOLT? VOLT:OVL?
	VOLT:STEP {<numeric value>}
	VOLT:STEP?
	VOLT:OVP {<numeric value>}
	VOLT:OVP?
	VOLT:OVP:TRIP?
	VOLT:OVP:CLE?
	CURR {<current>|UP|DOWN} CURR:UCL{<numeric value>}
	VOLT? CURR:UCL?
	CURR:STEP {<numeric value>} CURR:OCL{<numeric value>}
	CURR:STEP? CURR:OCL?
	CURR:OCP {<numeric value>}
	CURR:OCP?
	CURR:OCP:TRIP?
	CURR:OCP:CLE?
	FLOW?
	POL {P|N}
	POL?
	
	= Measurement Commands =
	MEAS:CURR?
	MEAS:VOLT?
	MEAS:ALL?
	
	= Calibration Commands =
	CAL:VOLT {voltage|MIN|MAX}
	CAL:CURR {current|MIN|MAX}

	= Factory Commands =
	FACT:USER-M {CLR}
	FACT:LAST-STA {DIS|SAF|FUL}
	FACT:LAST-STA?
	FACT:AUTO-CUR {DIS|ENA}
	FACT:AUTO-CUR?
	FACT:AUTO-LOC {DIS|ENA}
	FACT:AUTO-LOC?
	FACT:OVP {DIS|ENA}
	FACT:OVP?
	FACT:OCP {DIS|ENA}
	FACT:OCP?
	FACT:ADC {5|20|50|100|300|1300}
	FACT:ADC?
	FACT:LOAD-DEF

	= System Commands =
	SYST:BEEP
	SYST:ERR?
	SYST:VERS?
	OUTP {OFF|ON}
	OUTP?
	KEYL {OFF|ON}
	KEYL?
	*IDN?
	*SN?
	*RST
	*SAV {1|2|3|4|5|6|7|8|10}
	*RCL {1|2|3|4|5|6|7|8|10}
	*CLS

	*/



	/*
	+0,"No error"
	-10, "Invalid the DAC parameter"
	-200, "System interface error"
	-201, "ADC operating failed"
	-202, "Front panel operating failed"
	-255, "Error not define"
	-20, "Ignored min run under volt"
	-21, "Ignored min save under volt"
	-22, "Invalid min value use under volt"
	-23, "En route to cal the curr"
	-24, "Over volt min parameter"
	-25, "Under volt max parameter"
	-26, "Over volt max parameter"
	-27, "Ignored min run under curr"
	-28, "Ignored min save under curr"
	-29, "Invalid min value use under curr"
	-30, "En route to cal the curr"
	-31, "Over curr min parameter"
	-32, "Under curr max parameter"
	-33, "Over curr max parameter"
	-34, "Not allowed command under cal"
	-74, "ADC-V low limit over"
	-75, "ADC-V high limit over"
	-76, "ADC-A low limit over"
	-77, "ADC-A high limit over"
	-80, "Memory limit volt error"
	-81, "Memory limit curr error"
	-82, "Memory max volt error"
	-83, "Memory max curr error"
	-84, "Memory volt decimal error"
	-85, "Memory curr decimal error"
	-86, "Memory volt length error"
	-87, "Memory curr length error"
	-88, "Not match volt length and limit"
	-89, "Not match curr length and limit"
	-120, "Suffix too long"
	-121, "Invalid data"
	-122, "Syntax error"
	-123, "Invalid suffix"
	-124, "Undefined header"
	-220, "No execution"
	-221, "Setting conflict"
	-222, "Out of data"
	-223, "Incorret error"
	*/
};

#pragma pack (pop)

#endif // Def_ODA_PT_h__
