#pragma once
#include "Singleton.h"
#include "Def_CompileOption.h"

//#define _SIMUL_MODE_				1
#define _DRY_RUN_MODULE_NONE_		1		// 활성시 dryrun mode, 비활성화시 양산모드

#define _MOTION_CW					1
#define _MOTION_CCW					0

#define _SENSOR_ON					1
#define _SENSOR_OFF					0

#define _CYLINDER_UP				0
#define _CYLINDER_DOWN				1
#define _CYLINDER_IN				0
#define _CYLINDER_OUT				1


#define _PROGRAM_ROOT_DIR _T("C:\\Exec_Rivian\\Luritech_Motion")
#define _PROGRAM_ERRORLIST_DIR _T("C:\\Exec_Rivian\\Luritech_Motion\\ErrorList")
#define _MCH_LOG_SYSTEM_FOLDER _T("C:\\Exec_Rivian\\Luritech_Motion\\Log")
#define _MCH_LOG_COMM_FOLDER _T("C:\\Exec_Rivian\\Luritech_Motion\\Comm_Log")
#define _MCH_LOG_SEQ_FOLDER _T("C:\\Exec_Rivian\\Luritech_Motion\\Seq_Log")
#define _MCH_LOG_BARCODE_FOLDER _T("C:\\Exec_Rivian\\Luritech_Motion\\Barcode_Log")
#define _PROGRAM_RECIPE_DIR _T("C:\\Exec_Rivian\\Recipe")

//====================================================================
#define _SYSTEM_LOG		_T("[SYSTEM]")
#define _ERROR_LOG		_T("[ERROR]")
#define _MOTION_LOG		_T("[MOTION]")
#define _THREAD_LOG		_T("[THREAD]")
#define _COMM_LOG		_T("[COMM]")
#define _SEQ_LOG		_T("[SEQ]")
#define _BARCODE_LOG	_T("[BARCODE]")
//====================================================================

#define LANGAUGE_MAX_COUNT				2		// 영어, 한국어



// 장비별 Setting 필요부분 JYS.
//====================================================================
// MOTOR DEFINE
#define SOCKET_Y						0
#define SOCKET_Z						1

#if (GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL)
#define MOTOR_MAX_COUNT					2
#elif (GET_INSPECTOR == SYS_OQC_SFR_SINGLE_CL || GET_INSPECTOR == SYS_DISTORTION_30)
#define MOTOR_MAX_COUNT					1
#elif (GET_INSPECTOR == SYS_FILM_PEEL_OFF)
#define MOTOR_MAX_COUNT					2
#else
#define MOTOR_MAX_COUNT					1
#endif

#if (GET_INSPECTOR == SYS_OQC_SFR_SINGLE_CL) || (GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL) || (GET_INSPECTOR == SYS_DISTORTION_30)
#define INPUT_IO_MAX_COUNT				32
#define OUTPUT_IO_MAX_COUNT				32
#elif (GET_INSPECTOR == SYS_FILM_PEEL_OFF)
#define INPUT_IO_MAX_COUNT				64
#define OUTPUT_IO_MAX_COUNT				64
#endif

#define INPUT_IO_READ_COUNT				32		// 한번에 읽어올 i/O 수
#define OUTPUT_IO_READ_COUNT			32		// 한번에 읽어올 I/O 수


// PC에 아진보드 연결시 INPUT IO 먼저 인식이 되게 진행하면 EZSOFT 에서 OUTPUT 보드가 인식되는 번호가 몇번쨰 인지 확인후 작성
// 아웃풋을 먼저 설치하게 디되면 INPUT I/O번호가 밀리게 된다 따라서 한가지 방법으로 PC에 연결하는걸 추천
// 무조건 I/O 보드 연결은 INPUT 모두 연결후에 OUTPUT 연결하도록 하자.
#define AJIN_OUTPUT_IO_START_NUMBER		3		


#define SOCKET_USE
#define LAN_COMMUNICATION_COUNT			5


//====================================================================
// 조명 컨트롤러 연결 UI MAX 2개 ( DlgSystem.h )
// 위의 두개를 동시에 사용은 안된다.
//====================================================================
#define LIGHT_CONTROLLER_LURITECH_BOARD_USE	
#define LIGHT_CONTROLLER_FN300_BOARD_USE
//#define LIGHT_CONTROLLER_PT_BOARD_USE
//#define LIGHT_CONTROLLER_L100_USE
#define LIGHT_CONTROLLER_TYPE_COUNT		5

//====================================================================
// 조명컨트롤러 고정 사항.
// 조명 컨트롤러 MAX Voltage, MIN Voltage
#define LIGHT_CONTROLLER_MAX_VOLTAGE	15
#define LIGHT_CONTROLLER_MIN_VOLTAGE	0

// 조명 컨트롤러 MAX Current, MIN Current
#define LIGHT_CONTROLLER_MAX_CURRENT	3000
#define LIGHT_CONTROLLER_MIN_CURRENT	0




// Msgbox 사용 Type
#define MODE_QUESTION					0
#define MODE_ERROR						1
#define MODE_INFORMATION				2
#define MODE_NO_CLOSE					3

//#define BUTTON_OK						0x1
//#define BUTTON_CANCEL					0x2
//#define BUTTON_BUZZER_OFF				0x4
//#define BUTTON_ALL						0x7

// 나중에 지우자
#define LIGHT_CONTROLLER_LURITECH_BOARD_COUNT		4
#define LIGHT_CONTROLLER_PT_BOARD_COUNT				4
#define LIGHT_CONTROLLER_IPL_L100_COUNT				4
#define LIGHT_CONTROLLER_FN1000W_13CH_COUNT			13

#pragma comment(lib, "PCOMM.lib")

// Serial
#include "LED_PD_FN300.h"
#include "LightController_Luri.h"
//#include "LightController_ODA_PT.h"
#include "LightController_IPL_L100.h"
#include "LightController_PD_FN300W_13CH.h"
#include "LightController_PD_FN1500W_13CH.h"
//#include "LightController_ODA_Q.h"
#include "LightController_YouthTech_DPS.h"

// Ajinboard 
#pragma comment(lib, "AXL.lib")
#include "AXA.h"
#include "AXC.h"
#include "AXD.h"
#include "AXDev.h"
#include "AXHD.h"
#include "AXHS.h"
#include "AXL.h"
#include "AXM.h"
#define AXL_DEFAULT_IRQNO				7

#include "DataManager.h"
#define DATAMANAGER		CDataManager::Instance()

#include "Common.h"
#define COMMON			CCommon::Instance()

// 설비별로 시퀀스를 다르게 구현하자 
#if(GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL)
#include "SeqOqcMulti.h"
#define SEQUENCE		CSeqOqcMulti::Instance()
#define FN300			CLED_PD_FN300::GetInstance()

#elif(GET_INSPECTOR == SYS_OQC_SFR_SINGLE_CL)
#include "SeqOqcSingle.h"
#define SEQUENCE		CSeqOqcSingle::Instance()
#define FN300			CLED_PD_FN300::GetInstance()

#elif(GET_INSPECTOR == SYS_DISTORTION_30)
#include "SeqIntrinsic30.h"
#define SEQUENCE		CSeqIntrinsic30::Instance()

#elif(GET_INSPECTOR == SYS_FILM_PEEL_OFF)
#include "SeqFilmPeelOff.h"
#define SEQUENCE		CFilmPeelOff::Instance()
#define FN300			CLED_PD_FN300::GetInstance()
#endif

#include "AjinBoard.h"
#define AJIN_MO			CAjinMotion::Instance()

#include "Ajin_IO.h"
#define AJIN_IO			CAjinDio::Instance()

//#if(!MOTION_FAS_NONE)
#include "Fastech_PR_Motion.h"
#define FAS_MO			CFastech_Motion::Instance()
//#endif
#include "BCRCtrl.h"
static CBCRCtrl g_HandyBCR;

#include "ODA_Q.h"

#define ODA_Q CODA_Q::GetInstance()

#define WRITE_SYSTEM_FILE		GetLogSystem().LogOut
#define WRITE_COM_FILE			GetLogComm().LogOut
#define WRITE_BARCODE_FILE		GetLogBarcode().LogOut
#define WRITE_SEQ_FILE			GetLogSeq().LogOut

#define WRITE_MAIN_LOG			CDlgMain::Get_Instance()->Write_Main_Log		// UI Log 사용하려는 파일에 dlgmain.h 를 include  해주어야 한다.
#define WRITE_COMM_LOG			CDlgMain::Get_Instance()->Write_Comm_Log			// UI Log 사용하려는 파일에 dlgmain.h 를 include  해주어야 한다.
#define WRITE_BARCODE_LOG		CDlgMain::Get_Instance()->Write_Barcode_Log			// UI Log 사용하려는 파일에 dlgmain.h 를 include  해주어야 한다.
#define WRITE_SEQ_LOG			CDlgMain::Get_Instance()->Write_Sequence_Log		// UI Log 사용하려는 파일에 dlgmain.h 를 include  해주어야 한다.

#define SHOW_ERROR				CDlgMain::Get_Instance()->ShowError

#define TIMEOUT_DELAY_100		(int)100
#define TIMEOUT_DELAY_200		(int)200
#define TIMEOUT_DELAY_500		(int)500
#define TIMEOUT_DELAY_1000		(int)1000
#define TIMEOUT_DELAY_3000		(int)3000
#define TIMEOUT_DELAY_5000		(int)5000
#define TIMEOUT_DELAY_10000		(int)10000
#define TIMEOUT_DELAY_30000		(int)30000
#define TIMEOUT_DELAY_60000		(int)60000

#define TIMEOUT_SENSOR_5000		(int)5000

#define PULSE_PER_UNIT_Y		1000.0		// 리드길이 : 1바퀴 10mm
#define PULSE_PER_UNIT_Z		10000.0		// 리드길이 : 1바퀴 1mm

#pragma warning(disable : 4995)
#pragma warning(disable : 4819)
#pragma warning(disable : 4996)
#pragma warning(disable : 4005)
