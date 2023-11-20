//*****************************************************************************
// Filename	: 	Def_Keyence_BCR.h
// Created	:	2017/10/29 - 15:32
// Modified	:	2017/10/29 - 15:32
//
// Author	:	PiRing
//	
// Purpose	:	Keyence SR-710 바코드 리더기
//*****************************************************************************
#ifndef Def_Keyence_BCR_h__
#define Def_Keyence_BCR_h__

#include <afxwin.h>

// • 통신 프로토콜： 무수순, PASS / RTRY, ACK / NAK
// • 통신 데이터 포맷： 헤더, 터미네이터

//출하 시의 RS - 232C는 다음과 같이 설정되어 있습니다.
// • 전송 속도： 115200 bps
// • 데이터 길이： 8 bit
// • 패리티： 짝수
// • 스톱 비트 길이： 1 bit

/*
1. 타이밍 ON 커맨드 송신
	"LON"
2. 타이밍 ON 커맨드 회신
	"OK,LON"
3. 판독 동작
4. 판독 결과 데이터
	(코드 데이터)
*/


//  PASS, RTRY의 통신 포맷
// PASS：정상 종료
// RTRY：재송신 요구

//  송신 커맨드 포맷
// 파라미터 없음		헤더 | 커맨드 | 터미네이터
// 파라미터 1개		헤더 | 커맨드, 파라미터1 | 터미네이터
// 파라미터 2개		헤더 | 커맨드, 파라미터1, 파라미터2 | 터미네이터

//  회신 포맷
// 성공				헤더 | OK, 송신 커맨드 | 터미네이터
// 실패				헤더 | ER, 송신 커맨드, 에러 코드 | 터미네이터

namespace Keyence
{
	typedef enum _ConstVar
	{
		Keyence_STX				= 0x02,	// Header
		Keyence_ETX				= 0x03,	// Terminator
		Keyence_ProtoLength		= 23,
		Keyence_DataLength		= 20,
		Keyence_Delimeter		= ':',
	}_ConstVar;

	static const char* g_szKeyence_LON		= "\02LON\03";
	static const char* g_szKeyence_LOFF		= "\02LOFF\03";
	static const char* g_szKeyence_Ver		= "\02KEYENCE\03";
	

	typedef enum enSR700_Command
	{
		LON,		// 타이밍 ON 커맨드
		LOFF,		// 타이밍 OFF 커맨드
		PRON,		// 프리셋 데이터 등록 시작
		RCON,		// 일괄 설정 코드 판독 시작
		RCOFF,		// 일괄 설정 코드 판독 종료
		RCCHK,		// 일괄 설정 코드 판독 상황 확인
		TEST1,		// 판독률 측정 테스트 모드 시작
		TEST2,		// 택 타임 측정 테스트 모드 시작
		TEST3,		// 코드 위치 측정 테스트 모드 시작 *SR-750 시리즈
		QUIT,		// 테스트 모드 종료
		INCHK,		// 입력 단자의 상태 확인
		OUTON,		// 출력 단자의 ON 제어(개별 동작)
		OUTOFF,		// 출력 단자의 OFF 제어(개별 동작)
		ALLON,		// 출력 단자의 ON 제어(일괄 동작)
		ALLOFF,		// 출력 단자의 OFF 제어(일괄 동작)
		RESET,		// 소프트 리셋
		BCLR,		// 송신 버퍼의 클리어
		NUM,		// 판독 성공/에러 횟수 이력
		NUMB,		// 뱅크 사용 횟수 이력
		SHOT,		// 캡처의 실행
		CANCEL,		// 판독, 촬상 동작의 강제 종료
		LOCK,		// 강제 트리거 잠금
		UNLOCK,		// 강제 트리거 잠금 해제
		RLOCK,		// 강제 트리거 잠금 상태 취득
		AMON,		// 레이저 포인터 ON (SR-700)
		AMOFF,		// 레이저 포인터 OFF (SR-700)
		TUNE,		// 튜닝 시작
		TQUIT,		// 튜닝 종료
		BANKORDER,	// 얼터네이트 순서의 취득
		BCOPY,		// 파라미터 뱅크 복사
		SAVE,		// 설정 저장
		CMDSTAT,	// 설정 커맨드 상태의 취득
		DFLT,		// 설정 초기화
		BUSYSTAT,	// BUSY 상태의 취득 *SR-700만
		ERRSTAT,	// 에러 상태의 취득
		PCLR,		// PLC 링크 에러 해제
		KEYENCE,	// 모델, 본체 시스템 버전의 확인
		BSAVE,		// 설정 백업 *SR-700 시리즈
		BLOAD,		// 백업 설정의 로드 *SR-700 시리즈
		SCPTIME,	// 스크립트 처리 시간의 취득
		SCPDBG,		// 스크립트 디버그 설정
		SCPERR,		// 스크립트 에러 결과의 취득
		SCPVER,		// 스크립트 버전의 취득

		Max_Cmd_Count,
	}enSR700_Command;

	static const char* g_szKeyence_CMD[Max_Cmd_Count] =
	{
		"LON",
		"LOFF",
		"PRON",
		"RCON",
		"RCOFF",
		"RCCHK",
		"TEST1",
		"TEST2",
		"TEST3",
		"QUIT",
		"INCHK",
		"OUTON",
		"OUTOFF",
		"ALLON",
		"ALLOFF",
		"RESET",
		"BCLR",
		"NUM",
		"NUMB",
		"SHOT",
		"CANCEL",
		"LOCK",
		"UNLOCK",
		"RLOCK",
		"AMON",
		"AMOFF",
		"TUNE",
		"TQUIT",
		"BANKORDER",
		"BCOPY",
		"SAVE",
		"CMDSTAT",
		"DFLT",
		"BUSYSTAT",
		"ERRSTAT",
		"PCLR",
		"KEYENCE",
		"BSAVE",
		"BLOAD",
		"SCPTIME",
		"SCPDBG",
		"SCPERR",
		"SCPVER",
	};

	typedef struct _tag_Protocol
	{
		char		Header;
		CStringA	Dataz;
		char		Terminator;

		CStringA	Protocol;

		UINT		ErrCode;
		BOOL		bOK;

		_tag_Protocol()
		{
			Header		= Keyence_STX;
			Terminator	= Keyence_ETX;

			ErrCode		= 0;
			bOK			= FALSE;
		};

		_tag_Protocol& operator= (_tag_Protocol& ref)
		{
			Header		= ref.Header;
			Dataz		= ref.Dataz;
			Terminator	= ref.Terminator;
			Protocol	= ref.Protocol;
			ErrCode		= ref.ErrCode;
			bOK			= ref.bOK;

			return *this;
		};

		BOOL SetRecvProtocol(__in const char* pszProtocol, __in UINT_PTR nLength)
		{
			// ER 이면 에러
			if (NULL == pszProtocol)
				return FALSE;

			CStringA strProtocol = pszProtocol;

			INT_PTR nDataLength = nLength - 2; // STX, ETX
			INT_PTR nOffset = 0;

			Header		= pszProtocol[nOffset++];
			Dataz		= strProtocol.Mid((int)nOffset++, (int)nDataLength);
			Terminator	= pszProtocol[nOffset];

			// ER,
			if (0 <= Dataz.Find("ER,"))
			{
				int iOffset = Dataz.ReverseFind(',');

				CStringA szErrCode = Dataz.Right(Dataz.GetLength() - (iOffset + 1));
				
				ErrCode = (UINT)atoi(szErrCode.GetBuffer(0));

				bOK = FALSE;
			}

			bOK = TRUE;

			return TRUE;
		};

	}ST_Protocol, *PST_Protocol;

	

	// 에러 코드
	typedef enum enErrorCode
	{
		ErrCode_00 = 00, // 미정의 커맨드 수신
		ErrCode_01 = 01, // 커맨드의 포맷이 다르다. (파라미터의 수가 올바르지 않음)
		ErrCode_02 = 02, // 파라미터1의 설정 범위를 초과했다.
		ErrCode_03 = 03, // 파라미터2의 설정 범위를 초과했다.
		ErrCode_04 = 04, // 파라미터2가 HEX(16진) 코드로 지정되어 있지 않다.
		ErrCode_05 = 05, // 파라미터2가 HEX(16진) 코드이나 설정 범위를 초과했다.
		ErrCode_10 = 10, // 프리셋 데이터 내에 !가 2개 이상 존재한다. 프리셋 데이터가 올바르지 않다.
		ErrCode_11 = 11, // 영역 지정 데이터가 올바르지 않다.
		ErrCode_12 = 12, // 지정 파일이 존재하지 않는다.
		ErrCode_13 = 13, // % Tmm - ***의 mm가 31을 초과했다.
		ErrCode_14 = 14, // % Tmm - KEYENCE 커맨드에 대해 올바른 회신이 없다.
		ErrCode_20 = 20, // 현재의 동작 상태에서 실행할 수 없는 커맨드를 수신했다. (실행 에러)
		ErrCode_21 = 21, // 버퍼 오버 중이므로 커맨드를 실행할 수 없다.
		ErrCode_22 = 22, // 파라미터의 로드, 세이브 중에 에러가 발생해서 커맨드를 실행할 수 없다.
		ErrCode_23 = 23, // 설정 소프트웨어와 접속 중이므로 RS - 232C 인터페이스에서는 커맨드를 실행할 수 없다.
		ErrCode_30 = 30, // 해당 번호의 에러 화상은 저장되어 있지 않다.
		ErrCode_35 = 35, // 판독 코드가 설정되어 있지 않다.
		ErrCode_40 = 40, // 일괄적으로 송신한 커맨드의 길이가 2048 byte를 초과했다.
		ErrCode_41 = 41, // 일괄 설정으로 리스폰스의 길이가 2048 byte를 초과했다.
		ErrCode_42 = 42, // 일괄 설정으로 지정할 수 없는 커맨드가 지정되었다.
		ErrCode_99 = 99, // 기타 에러(KEYENCE로 문의해 주십시오)
	}enErrorCode;

	typedef enum enReadDataFormatIdx
	{
		RDF_Time,					  // 시간 ：
		RDF_DetailErrorCode,		  // 판독 상세에러 코드：
		RDF_CodeType,				  // 코드종류：
		RDF_SymbolID_ReadData,		  // 심벌식별자 | 판독 데이터 ：
		RDF_ParameterBankNumbers,	  // 파라미터 뱅크 번호：
		RDF_BurstNumber,			  // 버스트 번호：
		RDF_ScanCount,				  // 스캔 횟수：
		RDF_PositioningLevel,		  // 위치 측정 레벨：
		RDF_CodeVertexCoord,		  // 코드 정점 좌표：
		RDF_CodeCenterCoord,		  // 코드 중심 좌표：
		RDF_UnusedECCratio,			  // 오류 정정 미사용률：
		RDF_MatchingLevel,			  // 매칭 레벨：
		RDF_ISO_IEC15415_Result,	  // ISO / IEC15415 검증 결과：
		RDF_ISO_IECTR29158_Result,	  // ISO / IEC TR 29158 (AIM DPM - 1 - 2006) 검증 결과：
		RDF_SAE_AS9132_Result,		  // SAE AS9132 검증 결과：
		RDF_SEMI_T10_0701_Result,	  // SEMI T10 - 0701 검증 결과：
		RDF_JapanPharmaCodeResult,	  // 일본 의료용 의약 품 코드 검증 결과：
		RDF_ISO_IEC_16022,			  // ISO / IEC16022 ：
		RDF_ImageFileName,			  // 화상 파일명：
		RDF_GroupName,				  // 그룹명 ：
		RDF_MasterSlaveID,			  // 마스터 슬레이브 국ID ：
		RDF_ReadTime,				  // 판독시간
		RDF_MaxEnum,
	}enReadDataFormatIdx;

	// Read Data Format
	typedef struct _tag_ReadDataFormat
	{
		CStringA	Time;						// 	시간 ：
		CStringA	DetailErrorCode;			// 	판독 상세에러 코드：
		CStringA	CodeType;					// 	코드종류：
		CStringA	SymbolID;					// 	심벌식별자 |
		CStringA	ReadData;					// 	판독 데이터 ：
		CStringA	ParameterBankNumbers;		// 	파라미터 뱅크 번호：
		CStringA	BurstNumber;				// 	버스트 번호：
		CStringA	ScanCount;					// 	스캔 횟수：
		CStringA	PositioningLevel;			// 	위치 측정 레벨：
		CStringA	CodeVertexCoord;			// 	코드 정점 좌표：
		CStringA	CodeCenterCoord;			// 	코드 중심 좌표：
		CStringA	UnusedECCratio;				// 	오류 정정 미사용률：
		CStringA	MatchingLevel;				// 	매칭 레벨：
		CStringA	ISO_IEC15415_Result;		// 	ISO / IEC15415 검증 결과：
		CStringA	ISO_IECTR29158_Result;		// 	ISO / IEC TR 29158 (AIM DPM - 1 - 2006) 검증 결과：
		CStringA	SAE_AS9132_Result;			// 	SAE AS9132 검증 결과：
		CStringA	SEMI_T10_0701_Result;		// 	SEMI T10 - 0701 검증 결과：
		CStringA	JapanPharmaCodeResult;		// 	일본 의료용 의약 품 코드 검증 결과：
		CStringA	ISO_IEC_16022;				// 	ISO / IEC16022 ：
		CStringA	ImageFileName;				// 	화상 파일명：
		CStringA	GroupName;					// 	그룹명 ：
		CStringA	MasterSlaveID;				// 	마스터 슬레이브 국ID ：
		CStringA	ReadTime;					// 	판독시간

		BOOL SetRecvReadData(__in LPCSTR szReadData, __in UINT nDataLength)
		{
			USES_CONVERSION;
			CString			szTemp = CA2T(szReadData);
			CString			szToken;
			CStringArray	szItemz;
			int				curPos = 0;

			do
			{
				szToken = szTemp.Tokenize(_T(":"), curPos);

				if (szToken != "")
				{
					szItemz.Add(szToken);

					TRACE(_T("[%02d] %s\n"), szItemz.GetCount(), szToken);
				}
			}
			while (szToken != "");

			INT_PTR nCount = szItemz.GetCount();
			if (RDF_MaxEnum != nCount)
			{
				return FALSE;
			}

			return TRUE;
		};
	}ST_ReadDataFormat, *PST_ReadDataFormat;

	// Appending Data (헤더 | 데이터 사이즈 | 판독 데이터 + 부가 데이터 | 체크섬 | 터미네이터)
	typedef struct _tag_AppendingData
	{
		char		Header;
		CStringA	DataSize;
		CStringA	ReadData;
		char		Terminator;
	}ST_AppendingData, *PST_AppendingData;

	// 	상세 에러 코드 설명
	typedef enum enDetErrorCode
	{
		Err_ReadingSuccess						= 0,	// 	0 판독 성공
		Err_NoCode								= 1,	// 	1 코드 없음
		Err_TheSetDecodeTimeoutPeriodElapsed	= 2,	// 	2 설정된 디코드 타임 아웃 시간을 경과했다.
		Err_ScanningAborted						= 3,	// 	3 촬상을 중단했다.
		Err_ReadingImagesFailed					= 9,	// 	9 화상을 취득하지 못했다.
	}enDetErrorCode;



	// 판독 데이터의 포맷 ------------------------------------------------------
	// 	Data size |														// 	데이터사이즈 |
	// 	Time :															// 	시간 ：
	// 	Detail error code :												// 	판독 상세에러 코드：
	// 	Code type :														// 	코드종류：
	// 	Symbol ID | Read data :											// 	심벌식별자 | 판독 데이터 ：
	// 	Parameter bank numbers :										// 	파라미터 뱅크 번호：
	// 	Burst number :													// 	버스트 번호：
	// 	Scan count :													// 	스캔 횟수：
	// 	Positioning level :												// 	위치 측정 레벨：
	// 	Code vertex coordinates :										// 	코드 정점 좌표：
	// 	Code center coordinates :										// 	코드 중심 좌표：
	// 	Unused ECC ratio :												// 	오류 정정 미사용률：
	// 	Matching level :												// 	매칭 레벨：
	// 	ISO / IEC15415 verification result :							// 	ISO / IEC15415 검증 결과：
	// 	ISO / IEC TR 29158 (AIM DPM - 1 - 2006) verification result :	// 	ISO / IEC TR 29158 (AIM DPM - 1 - 2006) 검증 결과：
	// 	SAE AS9132 verification result :								// 	SAE AS9132 검증 결과：
	// 	SEMI T10 - 0701 verification result :							// 	SEMI T10 - 0701 검증 결과：
	// 	Japan pharmaceutical code verification result :					// 	일본 의료용 의약 품 코드 검증 결과：
	// 	ISO / IEC 16022 :												// 	ISO / IEC16022 ：
	// 	Image file name :												// 	화상 파일명：
	// 	Group name :													// 	그룹명 ：
	// 	Master / Slave ID :												// 	마스터 슬레이브 국ID ：
	// 	Read time														// 	판독시간
	// 	| Checksum														// 	| 체크섬
	//-------------------------------------------------------------------------



};




#endif // Def_Keyence_BCR_h__
