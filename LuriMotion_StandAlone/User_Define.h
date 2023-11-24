#pragma once
#include <vector>
#include "Def_DigitalIO.h"

//==========================================================================
// Socket 통신용
typedef BYTE			_u8;
typedef WORD			_u16;
typedef DWORD			_u32;

struct tcp_keepalive {
	u_long  onoff;
	u_long  keepalivetime;
	u_long  keepaliveinterval;
};

// New WSAIoctl Options
#define SW_VERSION				_T("[S/W Ver]     22.0901.1")

#define SIO_RCVALL            _WSAIOW(IOC_VENDOR,1)
#define SIO_RCVALL_MCAST      _WSAIOW(IOC_VENDOR,2)
#define SIO_RCVALL_IGMPMCAST  _WSAIOW(IOC_VENDOR,3)
#define SIO_KEEPALIVE_VALS    _WSAIOW(IOC_VENDOR,4)
#define SIO_ABSORB_RTRALERT   _WSAIOW(IOC_VENDOR,5)
#define SIO_UCAST_IF          _WSAIOW(IOC_VENDOR,6)
#define SIO_LIMIT_BROADCASTS  _WSAIOW(IOC_VENDOR,7)
#define SIO_INDEX_BIND        _WSAIOW(IOC_VENDOR,8)
#define SIO_INDEX_MCASTIF     _WSAIOW(IOC_VENDOR,9)
#define SIO_INDEX_ADD_MCAST   _WSAIOW(IOC_VENDOR,10)
#define SIO_INDEX_DEL_MCAST   _WSAIOW(IOC_VENDOR,11)
//=================================================================================

// Values for use with SIO_RCVALL* options
#define RCVALL_OFF				0
#define RCVALL_ON				1
#define RCVALL_SOCKETLEVELONLY	2
#define MAX_WAITCOMM_NUM		6	
#define MAX_TESTER_NUM			8

// [21.1017.2]
#define MAX_USER_NUM			20
// [21.1021.1]
#define	MESSAGE_UI_WINDOW_TOP	(WM_USER + 100)
#define	MESSAGE_STEP_MOVE		(WM_USER + 200)

#define UM_RECV_BARCODE			(WM_USER + 300)

// MsgBox
#pragma pack(push, 1)
enum class OperationMode
{
	Auto,
	Manual,
	SemiAuto,
	StartUpCheck,
	DryRun,
	Loto,
	LightMeasMode,
};

enum class enMaintFunId
{
	Load = 0,
	Unload,
};

typedef struct _THREAD_PARAM
{
	CString strMsg;
	int nMode;
	bool bTesterAlarm;

} THREAD_PARAM;
#pragma pack(pop, 1)

enum SleepModeStatus
{
	SLEEP_OFF,
	SLEEP_ON,
	SLEEP_ENABLE,
	SLEEP_FINISH,
	SLEEP_SUCCESS,
};
enum enCV_CC
{
	Const_CV = 0,
	Const_CC,
};
//enum enSwitchOnOff
//{
//	Switch_ON = 0,
//	Switch_OFF,
//};

enum enSlot
{
	Slot_All = 0,
	Slot_A,
	Slot_B,
	Slot_C,
	Slot_D,
	Slot_E,
	Slot_Max = Slot_E,
};
// [21.1017.1]
enum enPermisionLevel
{
	LVL_OPER = 0,
	LVL_MAINT,
	LVL_ADMIN
};
// [21.1027.1]
enum enMoveMode
{
	MOVE_VELOCITY = 0,
	MOVE_DISTANCE
};

enum enEMOStep
{
	EMO_1st = 0,
	EMO_2nd,
};

#if (GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL) || (GET_INSPECTOR == SYS_DISTORTION_30)
// [22.0401.4]
enum enDoorLIst
{
	EDoor_Front = 0,
	EDoor_Left_Front,
	EDoor_Left_Rear,
	EDoor_Right_Front,
	EDoor_Right_Rear,
	EDoor_Back_Left,
	EDoor_Back_Right,
	EDoor_Max
};
#elif (GET_INSPECTOR == SYS_OQC_SFR_SINGLE_CL)
enum enDoorLIst
{
	EDoor_Front = 0,
	EDoor_Left,
	EDoor_Right,
	EDoor_Back_Left,
	EDoor_Back_Right,
	EDoor_Max
};
#elif (GET_INSPECTOR == SYS_FILM_PEEL_OFF)
enum enDoorLIst
{
	EDoor_Rear1 = 0,
	EDoor_Rear2,
	EDoor_Rear3,
	EDoor_Max
};
#endif

enum enDoorLockAction
{
	ELock = 0,
	EUnlock,
};

#pragma pack(push, 1)
typedef struct _LURI_LIGHT_RECV
{
	float fVolt[Slot_Max];
	float fCurrent[Slot_Max];

	void Reset()
	{
		for (UINT nIdx = 0; nIdx < Slot_Max; nIdx++)
		{
			fVolt[nIdx] = 0.0f;
			fCurrent[nIdx] = 0.0f;
		}
	};

}LURI_LIGHT_RECV;
#pragma pack(push, 1)

// [21.1017.3] Added
#pragma pack(push, 1)
typedef struct _EQ_USER_DATA
{

	CString strUserID;
	CString strPassword;
	int		nPermission;

	_EQ_USER_DATA()
	{
		strUserID = _T("");
		strPassword = _T("");
		nPermission = 0;
	}

}EQ_USER_DATA;
#pragma pack(pop, 1)

// [21.1017.4] Added
#pragma pack(push, 1)
typedef struct _EQ_LEVEL_DATA
{
	CString strNo;
	CString strLevel;
	CString strUserID;

	_EQ_LEVEL_DATA()
	{
		strNo = _T("");
		strLevel = _T("");
		strUserID = _T("");
	}

}EQ_LEVEL_DATA;
#pragma pack(pop, 1)

// [21.1017.5] Modified
#pragma pack(push, 1)
typedef struct _EQ_BASIC_PARAM
{
	CString strLastRecipe;
	int nLanguage;
    int nLightSelected;

	EQ_USER_DATA stUserData[MAX_USER_NUM];

	_EQ_BASIC_PARAM()
	{
		strLastRecipe = _T("");
		nLanguage = 0;
		nLightSelected = 0;

		for (int i = 0; i < MAX_USER_NUM; i++)
		{
			stUserData[i].strUserID = _T("");
			stUserData[i].strPassword = _T("");
			stUserData[i].nPermission = 0;
		}
	}

}EQ_BASIC_PARAM;
#pragma pack(pop, 1)

// RUM_MODE
enum RUN_MODE { eNone = -1, eStandby = 0, eRunning, ePause, eError, eStop, eEStop, eHome};
enum TRAY_STATUS { eTray_None, eTray_Before, eTray_Working, eTray_Done };

// TOWERLAMP
enum ETwrMode { eModeRun = 0, eModeStop, eModeError, eModeReady, eModeCall, eModePause, eModeMax };
enum ETwrLamp { eTowerRed = 0, eTowerYellow, eTowerGreen, eTowerBuzzer, eTowerMax };
enum ETwrType { eTowerLamp_Off = 0, eTowerLamp_On, eTowerLamp_Blink, eTowerLamp_Max };

#pragma pack(push, 1)
typedef struct _EQ_TOWERLAMP
{
	int nTowerLampSetting[eModeMax][eTowerMax];

	_EQ_TOWERLAMP()
	{
		for (int i = 0; i < (int)eModeMax; i++)
		{
			for (int j = 0; j < (int)eTowerMax; j++)
			{
				nTowerLampSetting[i][j] = (int)eTowerLamp_Off;
			}
		}
	}
}EQ_TOWERLAMP;
#pragma pack(pop, 1)

// Motor Speed
enum { eSpeedSlow, eSpeedMiddle, eSpeedHigh, eSpeedAcc, eSpeedDec, eSpeedMax };

#pragma pack(push, 1)
typedef struct _MOTION_PARAM
{
	int nPort[2];								// Serial 2개
	int nBaudRate[2];							// Serial 2개

	int nMotorSpeed[10][eSpeedMax];			// 속도 저장용.

	CString	strChangeTime_New;				// 혹시 사용자가 저장버튼을 눌렀는지 확인하기 위해 생성 ( 저장 버튼을 누른 가장 최근 시간 )
	CString strChangeTime_Last;				// 혹시 사용자가 저장버튼을 눌렀을 경우 가장 마지막 저장된 시간
	CString strDiff[10][eSpeedMax];			// 저장 버튼을 눌렀을 경우 변경된 값이 있다면 일단 남겨두자. (가장 최근의 것만 가지고 있는다. )

	_MOTION_PARAM()
	{
		for (int i = 0; i < 2; i++)
		{
			nPort[i] = i + 1;
			nBaudRate[i] = 9600;
		}
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < (int)eSpeedMax; j++)
			{
				strDiff[i][j] = _T("");

				if (i == eSpeedAcc || i == eSpeedDec)
					nMotorSpeed[i][j] = (int)100;
				else
					nMotorSpeed[i][j] = (int)1;
			}
		}

		strChangeTime_New = _T("");
		strChangeTime_Last = _T("");
	}
}MOTION_PARAM;
#pragma pack(pop, 1)


#pragma pack(push, 1)
typedef struct _TEACHING_PARAM
{
	int		nSocketY_Wait_Pos;
	int		nSocketY_Blemishi_Pos;
	int		nSocketY_SFR_Pos;
	int		nSocketY_Intrinsic_Pos[5];

	int		nSocketZ_Wait_Pos;
	int		nSocketZ_Blemishi_Pos;
	int		nSocketZ_SFR_825_Pos;
	int		nSocketZ_SFR_180_Pos;

	int		nChangeMode_Time;

	CString strChangeTime_New_Pos;
	CString strChangeTime_Last_Pos;

	std::vector<CString> vModel_8M_30;
	std::vector<CString> vModel_3M_180_Side;
	std::vector<CString> vModel_3M_180_Front;
	std::vector<CString> vModel_3M_180_Rear;

	_TEACHING_PARAM()
	{
		strChangeTime_New_Pos = _T("");
		strChangeTime_Last_Pos = _T("");

		nSocketY_Wait_Pos = 0;
		nSocketY_Blemishi_Pos = 0;
		nSocketY_SFR_Pos = 0;

		nSocketZ_Wait_Pos = 0;
		nSocketZ_Blemishi_Pos = 0;
		nSocketZ_SFR_825_Pos = 0;
		nSocketZ_SFR_180_Pos = 0;

		nChangeMode_Time = 0;

		for (int i = 0; i < 5; i++)
			nSocketY_Intrinsic_Pos[i] = 0;

		vModel_8M_30.clear();
		vModel_3M_180_Side.clear();
		vModel_3M_180_Front.clear();
		vModel_3M_180_Rear.clear();
	}

	int GetChangeModeTime()
	{
		return (nChangeMode_Time / (60 * 1000));
	};

}TEACHING_PARAM;
#pragma pack(pop, 1)

#pragma pack(push, 1)
typedef struct _LIGHT_DATA
{
	int nVoltage[2][13];					// 컨트롤러당 13개채널 , 저장은 INT 형으로 하지만 실제 적용할때는 나누기1000 해서 Float형으로 한다.
	int nCurrent[2][13];					// 컨트롤러당 13개채널 , 저장은 INT 형으로 하지만 실제 적용할때는 나누기1000 해서 Float형으로 한다.

	_LIGHT_DATA()
	{
		for (int j = 0; j < 13; j++)
		{
			nVoltage[0][j] = 0;
			nVoltage[1][j] = 0;
			nCurrent[0][j] = 0;
			nCurrent[1][j] = 0;
		}
	}
} LIGHT_DATA;
#pragma pack(push, 1)

typedef struct _TESTER_COMM
{
	CString			strTesterConnect[5];			// Single 5개 연결
	int				nTesterPort[5];						// Single 5개 연결
	DWORD			dwTesterIP[5];				
	CString			strWaitTimeLIst[MAX_WAITCOMM_NUM];
	CString			strSemcoVer[MAX_TESTER_NUM];
	CString			strName[MAX_TESTER_NUM];

	int nSleepModeTimer;
	int nUseMode[5];							// 사용 유무 저장용
	int nServerMode[5];							// 서버 모드 사용 유무

	int nPort[3];								// Serial 2개, 0 : Light, 1 : PowerSupply, 2: BCR
	int nBaudRate[3];							// Serial 2개
	int nDataBit[3];							// Serial 2개
	int nStopBit[3];							// Serial 2개
	int nParityBit[3];							// Serial 2개

	float fVolt;								// 사용전압
	int nCurrent;								// 전류 

	LIGHT_DATA stLightData[3];

	_TESTER_COMM()
	{
		for (int i = 0; i < MAX_WAITCOMM_NUM; i++)
			strWaitTimeLIst[i] = _T("");
		for (int i = 0; i < MAX_TESTER_NUM; i++)
			strName[i] = _T("");
		for (int i = 0; i < MAX_TESTER_NUM; i++)
			strSemcoVer[i] = _T("");

		nSleepModeTimer = 0;
		for (int i = 0; i < 5; i++)
		{
			strTesterConnect[i] = _T("");
			nTesterPort[i] = 5000 + i;
			dwTesterIP[i] = 0;

			nUseMode[i] = 0;
			nServerMode[i] = 0;
		}

		for (int i = 0; i < 4; i++)
		{
			nPort[i] = i + 1;
			nBaudRate[i] = 9600;
			nDataBit[i] = 8;
			nStopBit[i] = 0;
			nParityBit[i] = 1;
		}

		fVolt = 12.0f;
		nCurrent = 1;
	}
}TESTER_COMM;
#pragma pack(pop, 1)

#pragma pack(push, 1)
typedef struct _ERROR_LIST
{
	int nErrorCount;
	std::vector<CString> vtErrorNum;

	int nErrorNameCount_100;
	int nErrorNameCount_200;
	int nErrorNameCount_300;

	std::vector<CString> vtErr_Kor;
	std::vector<CString> vtErr_Eng;
	std::vector<CString> vtErr_Viet;

	int nSolutionNameCount_100;
	int nSolutionNameCount_200;
	int nSolutionNameCount_300;

	std::vector<CString> vtSol_Kor;
	std::vector<CString> vtSol_Eng;
	std::vector<CString> vtSol_Viet;

	_ERROR_LIST()
	{
		nErrorCount = 0;
		vtErrorNum.clear();

		nErrorNameCount_100 = 0;
		nErrorNameCount_200 = 0;
		nErrorNameCount_300 = 0;

		vtErr_Kor.clear();
		vtErr_Eng.clear();
		vtErr_Viet.clear();

		nSolutionNameCount_100 = 0;
		nSolutionNameCount_200 = 0;
		nSolutionNameCount_300 = 0;

		vtSol_Kor.clear();
		vtSol_Eng.clear();
		vtSol_Viet.clear();
	}
}ERROR_LIST;
#pragma pack(pop, 1)


//-------------------------------------------------------------------------
// LuriMotion Dlg 

static LPCTSTR m_szBottomMenuKor[] =
{
	_T("메인"),
	_T("티칭"),
	_T("유지보수"),
	_T("IO"),
	_T("통신 연결"),
	_T("숨기기"),	
	_T("숨기기"),
	_T("종료"),
};

static LPCTSTR m_szBottomMenuEng[] =
{
	_T("Main"),
	_T("Teaching"),
	_T("Maint"),
	_T("DIO"),
	_T("Lan/Serial"),
	_T("HIDE"),	
	_T("HIDE"),
	_T("EXIT"),
};

// [21.1018.7] Modyfied
static LPCTSTR m_szSideMenuKor[] =
{
	_T("오토모드"),
	_T("매뉴얼모드"),
	_T("시업점검모드"),
	_T("로토모드"),
	_T("반복 검사"),
	_T("드라이런"),
	_T(""),
	_T("장비 초기화"),
};

static LPCTSTR m_szSideMenuEng[] =
{
	_T("AUTO MODE"),
	_T("MANUAL MODE"),
	_T("STARTUP MODE"),
	_T("LOTO MODE"),
	_T("Inspection Repeat"),
	_T("Dryrun"),
	_T(""),
	_T("INITIAL"),
};

static LPCTSTR m_szTitleMenuKor[] =
{
	_T("언어 설정"),
	_T("권한 설정"),
	_T("타워 램프"),	
	_T("Tool 4"),
};

static LPCTSTR m_szTitleMenuEng[] =
{
	_T("Language"),
	_T("Permission"),
	_T("TowerLamp"),	
	_T("Tool 4"),
};

// LuriMotion Dlg End
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
// Main Dlg 
static LPCTSTR m_szMainMotorSubTitle_Kor[] =
{
	_T("모터이름"),
	_T("현재위치"),
	_T("+ 리밋"),
	_T("- 리밋"),
	_T("에러"),
	_T("정지"),
	_T("홈"),
};
static LPCTSTR m_szMainMotorSubTitle_Eng[] =
{
	_T("Motor Name"),
	_T("Current Pos"),
	_T("+ Limit"),
	_T("- Limit"),
	_T("Error"),
	_T("InPosition"),
	_T("Home"),
};

static LPCTSTR m_szMainMotorName_Kor[] =
{
	_T("소켓Y"),
	_T("소켓Z"),
	_T("-"),
	_T("-"),
	_T("-"),
	_T("-"),
};
static LPCTSTR m_szMainMotorName_Eng[] =
{
	_T("SocketY"),
	_T("SocketZ"),
	_T("-"),
	_T("-"),
	_T("-"),
	_T("-"),
};

#if(GET_INSPECTOR == SYS_DISTORTION_30)
static LPCTSTR m_szMainCommName_Kor[] =
{
	_T("검사기"),
	_T("광원"),
	_T("IO 보드"),
	_T("파워"),
	_T("핸디바코드"),
};
static LPCTSTR m_szMainCommName_Eng[] =
{
	_T("Tester"),
	_T("Light B/d"),
	_T("IO B/d"),
	_T("Power"),
	_T("Handy BCR"),
};
#else
static LPCTSTR m_szMainCommName_Kor[] =
{
	_T("검사기"),
	_T("모션보드"),
	_T("IO보드"),
	_T("파워"),
	_T("핸디바코드"),
};
static LPCTSTR m_szMainCommName_Eng[] =
{
	_T("Tester"),
	_T("Motion B/d"),
	_T("IO B/d"),
	_T("Power"),
	_T("Handy BCR"),
};
#endif

static LPCTSTR m_szMainCommIO_Kor[] =
{
	_T("도어 전면"),
	_T("도어 좌측면"),
	_T(""),

	_T("도어 후면"),
	_T("도어 후측면"),
	_T("EMO"),
};
static LPCTSTR m_szMainCommIO_Eng[] =
{
	_T("Door Front"),
	_T("Door Left"),
	_T("Light curtain"),

	_T("Door Back"),
	_T("Door Right"),
	_T("EMO"),
};

static LPCTSTR m_szMainYield_Kor[] =
{
	_T("투입 수량"),
	_T("양품 수량"),
	_T("NG 수량"),
	_T("수율"),

	_T("리셋"),
};

static LPCTSTR m_szMainYield_Eng[] =
{
	_T("Input Count"),
	_T("OK Count"),
	_T("NG Count"),
	_T("Yield"),

	_T("Reset"),
};

#if(GET_INSPECTOR == SYS_DISTORTION_30)
static LPCTSTR m_szMainStartUpMode_Kor[] =
{
	_T("투입 이동"),
	_T("Calibration 계산"),
	_T("Capture4 Corner Image"),
	_T("대기위치 이동"),

	_T("Capture1 Corner Image"),
	_T("Capture2 Corner Image"),
	_T("Capture3 Corner Image"),
	_T("Capture5 Corner Image"),
};

static LPCTSTR m_szMainStartUpMode_Eng[] =
{
	_T("Load Pos Move"),
	_T("Caculate Calibration"),
	_T("Capture4 Corner Image"),
	_T("Wait Pos Move"),

	_T("Capture1 Corner Image"),
	_T("Capture2 Corner Image"),
	_T("Capture3 Corner Image"),
	_T("Capture5 Corner Image"),
};
#else
static LPCTSTR m_szMainStartUpMode_Kor[] =
{
	_T("투입 이동"),
	_T("SFR위치 이동"),
	_T("이물검사 위치이동"),
	_T("대기위치 이동"),
};

static LPCTSTR m_szMainStartUpMode_Eng[] =
{
	_T("Load Pos Move"),
	_T("SFR Pos Move"),
	_T("Blemish Pos Move"),
	_T("Wait Pos Move"),
};
#endif

#if(GET_INSPECTOR == SYS_DISTORTION_30)
static LPCTSTR m_szMainStageFlow_Kor[] =
{
	_T("로딩"),
	_T("이미지 캡쳐"),
	_T("계산"),
	_T("언로딩")
};

static LPCTSTR m_szMainStageFlow_Eng[] =
{
	_T("Loading"),
	_T("Capture Image"),
	_T("Calculation"),
};
#else
static LPCTSTR m_szMainStageFlow_Kor[] =
{
	_T("로딩"),
	_T("이물"),
	_T("SFR"),
};

static LPCTSTR m_szMainStageFlow_Eng[] =
{
	_T("Loading"),
	_T("Blemish"),
	_T("SFR"),
};
#endif


static LPCTSTR m_szMainMonitoring_Kor[] =
{
	_T("1.바코드 입력"),
	_T("2.소켓 닫힘"),
	_T("4.H0100 수신"),
	_T("5.파워서플-Ch1 선택"),
	_T("6.파워서플-Volt 입력"),
	_T("7.파워서플-Curr 입력"),
	_T("8.파워서플 켜기"),
	_T("9.T0110 송신"),
	_T("3.프로브핀 올리기"),
	_T("15.파워서플 끄기"),
	_T("16.프로브핀 내리기"),
	_T("12.Blemish 검사송신"),
	_T("13.Blemish 검사수신"),
	_T("14.PreviewStop 송신"),
	_T("17.PreviewStop 수신"),
	_T("Blemish 결과"),
	_T("10.SFR 검사송신"),
	_T("11.SFR 검사수신"),
	_T("SFR 결과"),
};

static LPCTSTR m_szMainMonitoring_Eng[] =
{
	_T("1.Barcode Input"),
	_T("2.Socket Close"),
	_T("4.T0110 Send"),
	_T("5.PowerSupply_Ch1 Set"),
	_T("6.PowerSupply-Volt Set"),
	_T("7.PowerSupply-Curr Set"),
	_T("8.PowerSupply On"),
	_T("9.H0100 Recive"),
	_T("3.ProbePin Up"),

	_T("15PowerSupply Off"),
	_T("16.ProbePin Down"),
	_T("12.Blemish Send"),
	_T("13.Blemish Recive"),
	_T("14.PreviewStop Send"),
	_T("17.PreviewStop Recive"),
	_T("Blemish RESULT"),

	_T("10.SFR Send"),
	_T("11.SFR Recive"),
	_T("SFR RESULT"),
};


static LPCTSTR m_szMainMovingFlow_Kor[] =
{
	_T("로딩 -----------> 이물"),
	_T("이물 -----------> SFR"),
	_T("이물 <----------- SFR"),
	_T("로딩 <----------- 이물"),
};


static LPCTSTR m_szMainMovingFlow_Eng[] =
{
	_T("Loading -----------> Blemish"),
	_T("Blemish -----------> SFR"),
	_T("Blemish <----------- SFR"),
	_T("Loading <----------- Blemish"),
};


// Main Dlg End
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
// Teaching Dlg

static LPCTSTR m_szTeachTitleKor[] = 
{
	_T("모터 현재 위치값"),
	_T("조그 이동 설정"),
	_T("실린더 & 컨베어"),
	_T("티칭 포지션"),
	_T("모터 속도 설정"),
};

static LPCTSTR m_szTeachTitleEng[] =
{
	_T("Motor Curruent Pos"),
	_T("Jog Move"),
	_T("Cylinder & Convayor"),
	_T("Position Teaching"),
	_T("Motor Speed Setting"),
};

static LPCTSTR m_szTeachCurPosAxisnNameKor[] =
{
	_T("Y 축"),
	_T("Z 축"),
	_T("3번축"),
	_T("4번축"),
	_T("5번축"),
	_T("6번축"),
	_T("7번축"),
	_T("8번축"),
	_T("-"),
	_T("-"),
};

static LPCTSTR m_szTeachCurPosAxisnNameEng[] =
{
	_T("Y AXIS"),
	_T("Z Axis"),
	_T("Axis 3"),
	_T("Axis 4"),
	_T("Axis 5"),
	_T("Axis 6"),
	_T("Axis 7"),
	_T("Axis 8"),
	_T("-"),
	_T("-"),
};


static LPCTSTR m_szTeachJogKor[] =
{
	_T("모드 선택"),
	_T("축 선택"),
	_T("이동 거리(mm)"),
	_T("이동 속도"),
};

static LPCTSTR m_szTeachJogEng[] =
{
	_T("Jog Mode"),
	_T("Select Axis"),
	_T("Distance Set(mm)"),
	_T("Speed Set"),
};


static LPCTSTR m_szTeachJogBtnKor[] =
{
	_T("속도 모드"),
	_T("상대 이동 모드"),

	_T("Y 축"),
	_T("Z 축"),
	_T("축 3"),
	_T("축 4"),
	_T("축 5"),
	_T("축 6"),
	_T("축 7"),
	_T("축 8"),
	_T("축 9"),
	_T("축 10"),

	_T("1mm"),
	_T("3mm"),
	_T("5mm"),
	_T("10mm"),
	_T("이동거리 입력"),

	_T("저속"),
	_T("중속"),
	_T("고속"),

	_T("이동(-)"),
	_T("이동(+)"),

	_T("개별축 HOME 동작"),

};

static LPCTSTR m_szTeachJogBtnEng[] =
{
	_T("Velocity"),
	_T("Relative"),

	_T("Y AXIS"),
	_T("Z Axis"),
	_T("Axis 3"),
	_T("Axis 4"),
	_T("Axis 5"),
	_T("Axis 6"),
	_T("Axis 7"),
	_T("Axis 8"),
	_T("Axis 9"),
	_T("Axis 10"),

	_T("1mm"),
	_T("3mm"),
	_T("5mm"),
	_T("10mm"),
	_T("Distance Input"),

	_T("Slow"),
	_T("Middle"),
	_T("High"),

	_T("Move(-)"),
	_T("move(+)"),

	_T("Each Axis Home"),
};


static LPCTSTR m_szTeachCylinderKor[] =
{
#if (GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL) || (GET_INSPECTOR == SYS_OQC_SFR_SINGLE_CL) || (GET_INSPECTOR == SYS_DISTORTION_30)
	_T("도어 인터락 전면"),
	_T("도어 인터락 좌측1"),
	_T("도어 인터락 좌측2"),
	_T("도어 인터락 우측1"),
	_T("도어 인터락 우측2"),
	_T("도어 인터락 후면1"),
	_T("도어 인터락 후면2"),
	_T(" - "),
#elif (GET_INSPECTOR == SYS_FILM_PEEL_OFF)
	_T("도어 인터락 후면1"),
	_T("도어 인터락 후면2"),
	_T("도어 인터락 후면3"),
	_T(" - "),
#else
#endif
};

static LPCTSTR m_szTeachCylinderEng[] =
{
#if (GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL) || (GET_INSPECTOR == SYS_OQC_SFR_SINGLE_CL) || (GET_INSPECTOR == SYS_DISTORTION_30)
	_T("DoorLock Front"),
	_T("DoorLock Left1"),
	_T("DoorLock Left2"),
	_T("DoorLock Right1"),
	_T("DoorLock Right2"),
	_T("DoorLock Back1"),
	_T("DoorLock Back2"),
	_T(" - "),
#elif (GET_INSPECTOR == SYS_FILM_PEEL_OFF)
	_T("DoorLock Rear1"),
	_T("DoorLock Rear2"),
	_T("DoorLock Rear3"),
	_T(" - "),
#else
#endif
};



static LPCTSTR m_szTeachMotorSpeedKor[] =
{
	_T("모터 선택"),
	_T("모터 속도\n(RPM)"),
	
	_T("저속"),
	_T("중속"),
	_T("속도"),
	_T("가속도"),
	_T("감속도"),
};

static LPCTSTR m_szTeachMotorSpeedEng[] =
{
	_T("Motor Select"),
	_T("Motor Speed\n(RPM)"),

	_T("Slow"),
	_T("Middle"),
	_T("Velocity"),
	_T("Acc"),
	_T("Dec"),
};

static LPCTSTR m_szTeachTrinity_AxisList_Kor[] =
{
	_T("SOCKET Y"),

	_T("자재대기위치(mm)"),
	_T("이물검사위치(mm)"),
	_T("SFR검사위치180도(mm)"),       
	_T("SFR검사위치82.5도(mm)"),
};

static LPCTSTR m_szTeachTrinity_AxisList_Eng[] =
{
	_T("SOCKET Y"),

	_T("Sample Input Pos(mm)"),
	_T("Blemish Inspect(mm)"),
	_T("SFR Inspect 180 (mm)"),
	_T("SFR Inspect 82.5 (mm)"),
};

static LPCTSTR m_szTeachIntrisic_AxisList_KorEng[] =
{
	_T("SOCKET Y"),

	_T("Capture1"),
	_T("Capture2"),
	_T("Capture3"),
	_T("Capture4"),
	_T("Capture5"),
};

static LPCTSTR m_szTeachTrinity_AxisStatus_List[] =
{
	_T("Error All"),
	_T("H/W +Limit"),
	_T("H/W -Limit"),
	_T("S/W +Limit"),
	_T("S/W -Limit"),
	_T("Reserved"),
	_T("Reserved"),
	_T( "Err Pos Overflow"),
	_T("Err Over Current"),
	_T("Err Over Speed"),
	_T( "Err Pos Tracking"),
	_T("Err Over load"),
	_T("Err Over heat"),
	_T( "Err Back EMF"),
	_T("Err Motor Power"),
	_T("Err Inposition"),
	_T( "Emg Stop"),
	_T("Slow Stop"),
	_T("Org Returning"),
	_T( "Inposition"),
	_T("Servo On"),
	_T("Alarm Reset"),
	_T( "PT Stopped"),
	_T("Origin Sensor"),
	_T("Z Pulse"),
	_T( "Org Ret OK"),
	_T("Motion DIR"),
	_T("Motioning"),
	_T("Motion Pause"),
	_T("Motion Accel"),
	_T("Motion Decel"),
	_T("Motion Constant"),
};

//List Conroller
enum
{
	WAIT_COMM_LIST1,
	WAIT_COMM_LIST2,
	WAIT_COMM_LIST3,
	WAIT_COMM_LIST4,
	WAIT_COMM_LIST5,
	WAIT_COMM_LIST6,
	WAIT_COMM_MAX,
};
static LPCTSTR m_szWaitTimeList_Kor[] =
{
	_T("카메라 초기화"),
	_T("카메라 시작"),
	_T("이물검사"),
	_T("SFR검사"),
	_T("카메라 종료"),
	_T("준비 신호 시간초과"),
};
static LPCTSTR m_szWaitTimeList_Eng[] =
{
	_T("Camera Initailize"),
	_T("Camera On"),
	_T("Blemish Inspection"),
	_T("SFR Inspection"),
	_T("Camera Off"),
	_T("Ready signal Timeout"),
};

//-------------------------------------------------------------------------
// Component 
static LPCTSTR m_szComponentWaitList_Kor[] =
{
	_T("통신 이름"),
	_T("대기 시간(ms)"),
};
static LPCTSTR m_szComponentWaitList_Eng[] =
{
	_T("Communication Name"),
	_T("WAIT TIME(ms)"),
};
static LPCTSTR m_szComponentTitle_Kor[] =
{
	_T("LAN 통신 연결"),
	_T("SERIAL 통신 연결"),
	_T("통신 대기시간 설정"),
	_T("대기시간"),
	_T("슬립모드 시간(초)"),
	_T("전원 셋팅"),
};
static LPCTSTR m_szComponentTitle_Eng[] =
{
	_T("LAN Communication"),
	_T("SERIAL Communication"),
	_T("Wait Time Communication"),
	_T("WaitTime"),
	_T("SleepMode Time(Sec)"),
	_T("Power Setting"),
};

static LPCTSTR m_szComponentLan_Sub_Kor[] =
{
	_T("TESTER 연결"),
	_T("통합 프로그램 연결"),
	_T("CAMERA 연결"),
};
static LPCTSTR m_szComponentLan_Sub_Eng[] =
{
	_T("TESTER Connect"),
	_T("TOTAL Program Connect"),
	_T("CAMERA Connect"),
};

static LPCTSTR m_szComponentConnect_Kor[] =
{
	_T("연결"),
	_T("해제"),
};
static LPCTSTR m_szComponentConnect_Eng[] =
{
	_T("Connect"),
	_T("Disconnect"),
};

static LPCTSTR m_szComponentSerialSub_Kor[] =
{
	_T("FN30 조명 컨트롤러"),
	_T("FN30 조명 채널 설정"),
	_T("파워 서플라이 연결"),
	_T("핸디 바코드 설정"),
};
static LPCTSTR m_szComponentSerialSub_Eng[] =
{
	_T("FN30 Light Controller"),
	_T("FN30 Light Channel Setting"),
	_T("Connect PowerSupply"),
	_T("Handy BCR Setting"),
};

static LPCTSTR m_szComponentSerialInput_Kor[] =
{
	_T("채널"),
	_T("레벨"),
	_T("셋"),
	_T("모델"),
};
static LPCTSTR m_szComponentSerialInput_Eng[] =
{
	_T("Channel"),
	_T("Level"),
	_T("Set"),
	_T("Model"),
};

static LPCTSTR m_szComponentSerial_Param[] =
{
	_T("Com Port"),
	_T("BaudRate"),
	_T("Data Bit"),
	_T("Parity Bit"),
	_T("Stop Bit"),
};

//-------------------------------------------------------------------------
// Error List
static LPCTSTR m_szErrList_Title_Kor[] =
{
	_T("에러리스트"),
	_T("해결 방법"),
	_T("에러 발생 위치"),
};
static LPCTSTR m_szErrList_Title_Eng[] =
{
	_T("Error List"),
	_T("Solution"),
	_T("Error Position"),
};

static LPCTSTR m_szErrList_TitleSub_Kor[] =
{
	_T("번호"),
	_T("에러번호"),
	_T("에러명"),

	_T("에러번호"),
	_T("해결방법"),
};
static LPCTSTR m_szErrList_TitleSub_Eng[] =
{
	_T("Num"),
	_T("Error Num"),
	_T("Error Name"),

	_T("Error Num"),
	_T("Error Solition"),
};

//-------------------------------------------------------------------------
// Key Pad Dialog
static LPCTSTR m_szKeyPadNujm_Param[] =
{
	_T("0"),
	_T("1"),
	_T("2"),
	_T("3"),
	_T("4"),
	_T("5"),
	_T("6"),
	_T("7"),
	_T("8"),
	_T("9"),
	_T("."),
	_T("DEL"),
	_T("+ / -"),
	_T("Ok"),
	_T("Cancel"),

};

//-------------------------------------------------------------------------
// 언어 설정 Dialog

static LPCTSTR m_szSelect_Language_Kor[] =
{
	_T("한국어"),
	_T("영어"),
	_T("-"),
	_T("-"),
};

static LPCTSTR m_szSelect_Language_Eng[] =
{
	_T("Korean"),
	_T("English"),
	_T("-"),
	_T("-"),
};

//-------------------------------------------------------------------------
// 권한 설정 Dialog
static LPCTSTR m_szSelect_Admin_Kor[] =
{
	_T("작업자"),
	_T("메인터넌스"),
	_T("관리자"),
};
static LPCTSTR m_szSelect_Admin_Eng[] =
{
	_T("Operator"),
	_T("Maintenance"),
	_T("Administrator"),
};

//-------------------------------------------------------------------------
// TowerLamp
static LPCTSTR m_szTowerLamp[] =
{
	_T("RUN"),
	_T("STOP"),
	_T("ERROR"),
	_T("READY"),
	_T("CALL"),
	_T("PAUSE"),
};

static LPCTSTR m_szToweColor[] =
{
	_T("RED"),
	_T("YELLOW"),
	_T("GREEN"),
	_T("BUZZER"),
};


