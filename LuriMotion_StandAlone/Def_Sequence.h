﻿#pragma once

#include "Def_CompileOption.h"

#if(GET_INSPECTOR == SYS_OQC_SFR_MULTI_CL)
typedef enum MyEnum
{
	SEQ_STEP_OQC_MULTI_STARTUP_STANDBY,
	SEQ_STEP_OQC_MULTI_STARTUP_COMPLETE,
	SEQ_STEP_OQC_MULTI_STARTUP_ERROR,
	
	// 0
	SEQ_STEP_OQC_MULTI_STARTUP_LOAD_START,
	SEQ_STEP_OQC_MULTI_STARTUP_LOAD_CAMERA_DETECT_CHECK,		// camera detect
	SEQ_STEP_OQC_MULTI_STARTUP_LOAD_CAMERA_FIX_OFF,			// camera fix off
	SEQ_STEP_OQC_MULTI_STARTUP_LOAD_CAMERA_FIX_OFF_CHECK,	// camera fix off check
	SEQ_STEP_OQC_MULTI_STARTUP_LOAD_SOCKET_DN,				// socket dn
	SEQ_STEP_OQC_MULTI_STARTUP_LOAD_SOCKET_DN_CHECK,			// socket dn check
	SEQ_STEP_OQC_MULTI_STARTUP_LOAD_SOCKET_IN,				// socket in
	SEQ_STEP_OQC_MULTI_STARTUP_LOAD_SOCKET_IN_CHECK,			// socket in check
	SEQ_STEP_OQC_MULTI_STARTUP_LOAD_SOCKET_UP,				// socket up
	SEQ_STEP_OQC_MULTI_STARTUP_LOAD_SOCKET_UP_CHECK,			// socket up check
	SEQ_STEP_OQC_MULTI_STARTUP_LOAD_CAMERA_FIX_ON,			// camera fix on
	SEQ_STEP_OQC_MULTI_STARTUP_LOAD_CAMERA_FIX_ON_CHECK,		// camera fix on check
	SEQ_STEP_OQC_MULTI_STARTUP_LOAD_PROBE_PIN_UP,			// probe pin up
	SEQ_STEP_OQC_MULTI_STARTUP_LOAD_PROBE_PIN_UP_CHECK,		// probe pin up check
	SEQ_STEP_OQC_MULTI_STARTUP_LOAD_POWER_ON,				// power on
	SEQ_STEP_OQC_MULTI_STARTUP_LOAD_PREVIEWSTART,			// preview start
	SEQ_STEP_OQC_MULTI_STARTUP_LOAD_PREVIEWSTART_ACK_CHECK,	// preview start check
	SEQ_STEP_OQC_MULTI_STARTUP_LOAD_RETRY_PREVIEWSTART,		// retry preview start
	SEQ_STEP_OQC_MULTI_STARTUP_LOAD_END,

	// 3
	SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_START,
	SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_WAIT_MOVE_Z,			// wait move z position
	SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_WAIT_MOVE_Z_CHECK,	// wait move z position moving check
	SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_WAIT_MOVE_Y,			// wait move y position
	SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_WAIT_MOVE_Y_CHECK,	// wait move y position moving check
	SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_PREVIEW_STOP,			// prewview stop
	SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_PREVIEW_STOP_CHECK,	// preview stop check
	SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_POWER_OFF,			// power off
	SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_PROBE_DN,				// probe pin dn
	SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_PROBE_DN_CHECK,		// probe pin dn check
	SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_CAMERA_FIX_OFF,		// camera fix off
	SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_CAMERA_FIX_OFF_CHECK,	// camera fix off check
	SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_SOCKET_DN,			// socket dn
	SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_SOCKET_DN_CHECK,		// socket dn check
	SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_SOCKET_OUT,			// socket out
	SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_SOCKET_OUT_CHECK,		// socket out check
	SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_SOCKET_UP,			// socket up
	SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_SOCKET_UP_CHECK,		// socket up check
	SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_END,

	// 1
	SEQ_STEP_OQC_MULTI_STARTUP_BLEMISH_START,
	SEQ_STEP_OQC_MULTI_STARTUP_BLEMISH_SOCKET_POS_CHECK,	// socket position check 
	SEQ_STEP_OQC_MULTI_STARTUP_BLEMISH_MOTOR_POS_CHECK,		// blemish position check
	SEQ_STEP_OQC_MULTI_STARTUP_BLEMISH_MOVE_Z_ZERO,			// blemish move z zero
	SEQ_STEP_OQC_MULTI_STARTUP_BLEMISH_MOVE_Z_ZERO_CHECK,	// blemish move z zero moving check
	SEQ_STEP_OQC_MULTI_STARTUP_BLEMISH_MOVE_Y,				// blemish move y position
	SEQ_STEP_OQC_MULTI_STARTUP_BLEMISH_MOVE_Y_CHECK,		// blemish move y moving check
	SEQ_STEP_OQC_MULTI_STARTUP_BLEMISH_MOVE_Z,				// blemish move z position
	SEQ_STEP_OQC_MULTI_STARTUP_BLEMISH_MOVE_Z_CHECK,		// blemish move z moving check
	SEQ_STEP_OQC_MULTI_STARTUP_BLEMISH_SEND_COMMAND,		// blemish send command
	SEQ_STEP_OQC_MULTI_STARTUP_BLEMISH_SEND_COMMAND_CHECK,	// blemish ack check
	SEQ_STEP_OQC_MULTI_STARTUP_BLEMISH_END,

	// 2
	SEQ_STEP_OQC_MULTI_STARTUP_SFR_START,
	SEQ_STEP_OQC_MULTI_STARTUP_SFR_SOCKET_POS_CHECK,	// socket position check 
	SEQ_STEP_OQC_MULTI_STARTUP_SFR_MOTOR_POS_CHECK,		// SFR position check
	SEQ_STEP_OQC_MULTI_STARTUP_SFR_MOVE_Z_ZERO,			// SFR move z zero
	SEQ_STEP_OQC_MULTI_STARTUP_SFR_MOVE_Z_ZERO_CHECK,	// SFR move z zero moving check
	SEQ_STEP_OQC_MULTI_STARTUP_SFR_MOVE_Y,				// SFR move y position
	SEQ_STEP_OQC_MULTI_STARTUP_SFR_MOVE_Y_CHECK,		// SFR move y moving check
	SEQ_STEP_OQC_MULTI_STARTUP_SFR_MOVE_Z,				// SFR move z position
	SEQ_STEP_OQC_MULTI_STARTUP_SFR_MOVE_Z_CHECK,		// SFR move z moving check
	SEQ_STEP_OQC_MULTI_STARTUP_SFR_SEND_COMMAND,		// SFR send command
	SEQ_STEP_OQC_MULTI_STARTUP_SFR_SEND_COMMAND_CHECK,	// SFR ack check
	SEQ_STEP_OQC_MULTI_STARTUP_SFR_END,
}enSeqOqcMultiStartUpStep;

static LPCTSTR g_szSeqStartUpName[] =
{
	_T("SEQ_STEP_OQC_MULTI_STARTUP_STANDBY"),
	_T("SEQ_STEP_OQC_MULTI_STARTUP_COMPLETE"),
	_T("SEQ_STEP_OQC_MULTI_STARTUP_ERROR"),

	// 0
	_T("SEQ_STEP_OQC_MULTI_STARTUP_LOAD_START"),
	_T("SEQ_STEP_OQC_MULTI_STARTUP_LOAD_CAMERA_DETECT_CHECK"),		// camera detect
	_T("SEQ_STEP_OQC_MULTI_STARTUP_LOAD_CAMERA_FIX_OFF"),			// camera fix off
	_T("SEQ_STEP_OQC_MULTI_STARTUP_LOAD_CAMERA_FIX_OFF_CHECK"),		// camera fix off check
	_T("SEQ_STEP_OQC_MULTI_STARTUP_LOAD_SOCKET_DN"),				// socket dn
	_T("SEQ_STEP_OQC_MULTI_STARTUP_LOAD_SOCKET_DN_CHECK"),			// socket dn check
	_T("SEQ_STEP_OQC_MULTI_STARTUP_LOAD_SOCKET_IN"),				// socket in
	_T("SEQ_STEP_OQC_MULTI_STARTUP_LOAD_SOCKET_IN_CHECK"),			// socket in check
	_T("SEQ_STEP_OQC_MULTI_STARTUP_LOAD_SOCKET_UP"),				// socket up
	_T("SEQ_STEP_OQC_MULTI_STARTUP_LOAD_SOCKET_UP_CHECK"),			// socket up check
	_T("SEQ_STEP_OQC_MULTI_STARTUP_LOAD_CAMERA_FIX_ON"),			// camera fix on
	_T("SEQ_STEP_OQC_MULTI_STARTUP_LOAD_CAMERA_FIX_ON_CHECK"),		// camera fix on check
	_T("SEQ_STEP_OQC_MULTI_STARTUP_LOAD_PROBE_PIN_UP"),				// probe pin up
	_T("SEQ_STEP_OQC_MULTI_STARTUP_LOAD_PROBE_PIN_UP_CHECK"),		// probe pin up check
	_T("SEQ_STEP_OQC_MULTI_STARTUP_LOAD_POWER_ON"),					// power on
	_T("SEQ_STEP_OQC_MULTI_STARTUP_LOAD_PREVIEWSTART"),				// preview start
	_T("SEQ_STEP_OQC_MULTI_STARTUP_LOAD_PREVIEWSTART_ACK_CHECK"),	// preview start check
	_T("SEQ_STEP_OQC_MULTI_STARTUP_LOAD_RETRY_PREVIEWSTART"),		// retry preview start
	_T("SEQ_STEP_OQC_MULTI_STARTUP_LOAD_END"),

	// 3
	_T("SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_START"),
	_T("SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_WAIT_MOVE_Z"),			// wait move z position
	_T("SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_WAIT_MOVE_Z_CHECK"),		// wait move z position moving check
	_T("SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_WAIT_MOVE_Y"),			// wait move y position
	_T("SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_WAIT_MOVE_Y_CHECK"),		// wait move y position moving check
	_T("SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_PREVIEW_STOP"),			// prewview stop
	_T("SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_PREVIEW_STOP_CHECK"),		// preview stop check
	_T("SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_POWER_OFF"),				// power off
	_T("SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_PROBE_DN"),				// probe pin dn
	_T("SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_PROBE_DN_CHECK"),			// probe pin dn check
	_T("SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_CAMERA_FIX_OFF"),			// camera fix off
	_T("SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_CAMERA_FIX_OFF_CHECK"),	// camera fix off check
	_T("SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_SOCKET_DN"),				// socket dn
	_T("SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_SOCKET_DN_CHECK"),		// socket dn check
	_T("SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_SOCKET_OUT"),				// socket out
	_T("SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_SOCKET_OUT_CHECK"),		// socket out check
	_T("SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_SOCKET_UP"),				// socket up
	_T("SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_SOCKET_UP_CHECK"),		// socket up check
	_T("SEQ_STEP_OQC_MULTI_STARTUP_UNLOAD_END"),

	// 1
	_T("SEQ_STEP_OQC_MULTI_STARTUP_BLEMISH_START"),
	_T("SEQ_STEP_OQC_MULTI_STARTUP_BLEMISH_SOCKET_POS_CHECK"),		// socket position check 
	_T("SEQ_STEP_OQC_MULTI_STARTUP_BLEMISH_MOTOR_POS_CHECK"),		// blemish position check
	_T("SEQ_STEP_OQC_MULTI_STARTUP_BLEMISH_MOVE_Z_ZERO"),			// blemish move z zero
	_T("SEQ_STEP_OQC_MULTI_STARTUP_BLEMISH_MOVE_Z_ZERO_CHECK"),		// blemish move z zero moving check
	_T("SEQ_STEP_OQC_MULTI_STARTUP_BLEMISH_MOVE_Y"),				// blemish move y position
	_T("SEQ_STEP_OQC_MULTI_STARTUP_BLEMISH_MOVE_Y_CHECK"),			// blemish move y moving check
	_T("SEQ_STEP_OQC_MULTI_STARTUP_BLEMISH_MOVE_Z"),				// blemish move z position
	_T("SEQ_STEP_OQC_MULTI_STARTUP_BLEMISH_MOVE_Z_CHECK"),			// blemish move z moving check
	_T("SEQ_STEP_OQC_MULTI_STARTUP_BLEMISH_SEND_COMMAND"),			// blemish send command
	_T("SEQ_STEP_OQC_MULTI_STARTUP_BLEMISH_SEND_COMMAND_CHECK"),	// blemish ack check
	_T("SEQ_STEP_OQC_MULTI_STARTUP_BLEMISH_END"),

	// 2
	_T("SEQ_STEP_OQC_MULTI_STARTUP_SFR_START"),
	_T("SEQ_STEP_OQC_MULTI_STARTUP_SFR_SOCKET_POS_CHECK"),		// socket position check 
	_T("SEQ_STEP_OQC_MULTI_STARTUP_SFR_MOTOR_POS_CHECK"),		// SFR position check
	_T("SEQ_STEP_OQC_MULTI_STARTUP_SFR_MOVE_Z_ZERO"),			// SFR move z zero
	_T("SEQ_STEP_OQC_MULTI_STARTUP_SFR_MOVE_Z_ZERO_CHECK"),		// SFR move z zero moving check
	_T("SEQ_STEP_OQC_MULTI_STARTUP_SFR_MOVE_Y"),				// SFR move y position
	_T("SEQ_STEP_OQC_MULTI_STARTUP_SFR_MOVE_Y_CHECK"),			// SFR move y moving check
	_T("SEQ_STEP_OQC_MULTI_STARTUP_SFR_MOVE_Z"),				// SFR move z position
	_T("SEQ_STEP_OQC_MULTI_STARTUP_SFR_MOVE_Z_CHECK"),			// SFR move z moving check
	_T("SEQ_STEP_OQC_MULTI_STARTUP_SFR_SEND_COMMAND"),			// SFR send command
	_T("SEQ_STEP_OQC_MULTI_STARTUP_SFR_SEND_COMMAND_CHECK"),	// SFR ack check
	_T("SEQ_STEP_OQC_MULTI_STARTUP_SFR_END"),
};

typedef enum
{
	SEQ_STEP_STANDBY = 0,
	SEQ_STEP_ERROR,

	SEQ_STEP_SFR_WIDE_STARTUPMODE_ACTION_WAIT,
	SEQ_STEP_SFR_WIDE_READY,
	SEQ_STEP_SFR_WIDE_INTERLOCK_CHECK,

	SEQ_STEP_SFR_WIDE_MOVE_SOCKET_CYLINDER_DOWN, //add
	SEQ_STEP_SFR_WIDE_MOVE_SOCKET_CYLINDER_DOWN_CHK,//add
	SEQ_STEP_SFR_WIDE_MOVE_SOCKET_CYLINDER_IN,//add
	SEQ_STEP_SFR_WIDE_MOVE_SOCKET_CYLINDER_IN_CHK,//add
	SEQ_STEP_SFR_WIDE_MOVE_SOCKET_CYLINDER_UP,//add
	SEQ_STEP_SFR_WIDE_MOVE_SOCKET_CYLINDER_UP_CHK,//add
	SEQ_STEP_SFR_WIDE_MOVE_CAMERA_FIX_CYLINDER_ON,//add
	SEQ_STEP_SFR_WIDE_MOVE_CAMERA_FIX_CYLINDER_ON_CHK,//add

	SEQ_STEP_SFR_WIDE_MOVE_PROBE_CYLINDER_UP,//add
	SEQ_STEP_SFR_WIDE_MOVE_PROBE_CYLINDER_UP_CHK,//add

	SEQ_STEP_SFR_WIDE_SEND_INSPECTION_PREVIEW_START,

	SEQ_STEP_SFR_WIDE_MOVE_Z_ZERO_2,
	SEQ_STEP_SFR_WIDE_MOVE_Z_ZERO_2_CHK,
	SEQ_STEP_SFR_WIDE_MOVE_CYLINDER_UP_Y_POS,  //Blemish 위치 이동. 이 여기를 넘여야 Start 버튼 꺼짐.
	SEQ_STEP_SFR_WIDE_MOVE_CYLINDER_UP_Y_POS_CHK,
	SEQ_STEP_SFR_WIDE_MOVE_SFR_Y_POS,
	SEQ_STEP_SFR_WIDE_MOVE_SFR_Y_POS_CHK,
	SEQ_STEP_SFR_WIDE_MOVE_SFR_Z_POS,
	SEQ_STEP_SFR_WIDE_MOVE_SFR_Z_POS_CHK,

	SEQ_STEP_SFR_WIDE_SEND_INSPECTION_PREVIEW_RESTART,
	SEQ_STEP_SFR_WIDE_SEND_INSPECTION_PREVIEW_START_CHK,

	SEQ_STEP_SFR_WIDE_INSPECTION_MEASURE_CURRENT,

	SEQ_STEP_SFR_WIDE_SEND_INSEPCTION_SFR,
	SEQ_STEP_SFR_WIDE_SEND_INSEPCTION_SFR_WAIT_RECIVE,

	SEQ_STEP_SFR_WIDE_MOVE_Z_ZERO_1,
	SEQ_STEP_SFR_WIDE_MOVE_Z_ZERO_1_CHK,
	SEQ_STEP_SFR_WIDE_MOVE_BLEMISHI_Y_POS,
	SEQ_STEP_SFR_WIDE_MOVE_BLEMISHI_Y_POS_CHK,
	SEQ_STEP_SFR_WIDE_MOVE_BLEMISHI_Z_POS,
	SEQ_STEP_SFR_WIDE_MOVE_BLEMISHI_Z_POS_CHK,

	SEQ_STEP_SFR_WIDE_SEND_INSEPCTION_BLEMISHI,
	SEQ_STEP_SFR_WIDE_SEND_INSEPCTION_BLEMISHI_WAIT_RECIVE,
	SEQ_STEP_SFR_WIDE_SEND_INSPECTION_PREVIEW_STOP,

	SEQ_STEP_SFR_WIDE_MOVE_Z_ZERO_3,
	SEQ_STEP_SFR_WIDE_MOVE_Z_ZERO_3_CHK,
	SEQ_STEP_SFR_WIDE_MOVE_Y_WAIT_POS,
	SEQ_STEP_SFR_WIDE_MOVE_Y_WAIT_POS_CHK,
	SEQ_STEP_SFR_WIDE_MOVE_Z_WAIT_POS,
	SEQ_STEP_SFR_WIDE_MOVE_Z_WAIT_POS_CHK,
	SEQ_STEP_SFR_WIDE_SEND_INSPECTION_PREVIEW_STOP_CHK,

	SEQ_STEP_SFR_WIDE_MOVE_CAMERA_FIX_CYLINDER_OFF,//add
	SEQ_STEP_SFR_WIDE_MOVE_CAMERA_FIX_CYLINDER_OFF_CHK,

	SEQ_STEP_SFR_WIDE_MOVE_CAMERA_CYLINDER_DOWN,
	SEQ_STEP_SFR_WIDE_MOVE_CAMERA_CYLINDER_DOWN_CHK,	

	SEQ_STEP_SFR_WIDE_MOVE_SOCKET_CYLINDER_OUT,//add
	SEQ_STEP_SFR_WIDE_MOVE_SOCKET_CYLINDER_OUT_CHK,//add
	SEQ_STEP_SFR_WIDE_UNLOAD_MOVE_SOCKET_CYLINDER_UP, //add
	SEQ_STEP_SFR_WIDE_UNLOAD_MOVE_SOCKET_CYLINDER_UP_CHK,//add

	SEQ_STEP_SFR_WIDE_WAIT_STOP_SWITCH_PUSH,

	SEQ_STEP_SFR_WIDE_CYCLE_COMPLEATE,
	SEQ_STEP_SFR_WIDE_DRYRUN_MODE,

	SEQ_STEP_SFR_MOVEBACK_STEP001_CHECK_BACK_COMMAND,
	SEQ_STEP_SFR_MOVEBACK_STEP002_MOVEZ_WAITPOS,
	SEQ_STEP_SFR_MOVEBACK_STEP003_MOVEZ_WAITPOS_CHK,
	SEQ_STEP_SFR_MOVEBACK_STEP004_MOVEY_WAITPOS,
	SEQ_STEP_SFR_MOVEBACK_STEP005_MOVEY_WAITPOS_CHK,
	SEQ_STEP_SFR_MOVEBACK_STEP006_COMPLEATE,
} enSeqSFRWideStep;

static LPCTSTR g_szSeqName[] =
{
	_T("SEQ_STEP_STANDBY"),
	_T("SEQ_STEP_ERROR"),

	_T("SEQ_STEP_SFR_STARTUPMODE_ACTION_WAIT"),
	_T("SEQ_STEP_SFR_READY"),
	_T("SEQ_STEP_SFR_INTERLOCK_CHECK"),

	_T("SEQ_STEP_SFR_MOVE_SOCKET_CYLINDER_DOWN"),
	_T("SEQ_STEP_SFR_MOVE_SOCKET_CYLINDER_DOWN_CHK"),
	_T("SEQ_STEP_SFR_MOVE_SOCKET_CYLINDER_IN"),
	_T("SEQ_STEP_SFR_MOVE_SOCKET_CYLINDER_IN_CHK"),
	_T("SEQ_STEP_SFR_MOVE_SOCKET_CYLINDER_UP"),
	_T("SEQ_STEP_SFR_MOVE_SOCKET_CYLINDER_UP_CHK"),
	_T("SEQ_STEP_SFR_MOVE_CAMERA_FIX_CYLINDER_ON"),
	_T("SEQ_STEP_SFR_MOVE_CAMERA_FIX_CYLINDER_ON_CHK"),

	_T("SEQ_STEP_SFR_MOVE_PROBE_CYLINDER_UP"),
	_T("SEQ_STEP_SFR_MOVE_PROBE_CYLINDER_UP_CHK"),

	_T("SEQ_STEP_SFR_SEND_INSPECTION_PREVIEW_START"),

	_T("SEQ_STEP_SFR_MOVE_Z_ZERO_2"),						// SINGLE : SKIP
	_T("SEQ_STEP_SFR_MOVE_Z_ZERO_2_CHK"),					// SINGLE : SKIP
	_T("SEQ_STEP_SFR_MOVE_CYLINDER_UP_Y_POS"),
	_T("SEQ_STEP_SFR_MOVE_CYLINDER_UP_Y_POS_CHK"),
	_T("SEQ_STEP_SFR_MOVE_SFR_Y_POS"),
	_T("SEQ_STEP_SFR_MOVE_SFR_Y_POS_CHK"),
	_T("SEQ_STEP_SFR_MOVE_SFR_Z_POS"),
	_T("SEQ_STEP_SFR_MOVE_SFR_Z_POS_CHK"),

	_T("SEQ_STEP_SFR_SEND_INSPECTION_PREVIEW_RESTART"),
	_T("SEQ_STEP_SFR_SEND_INSPECTION_PREVIEW_START_CHK"),

	_T("SEQ_STEP_SFR_WIDE_INSPECTION_MEASURE_CURRENT"),

	_T("SEQ_STEP_SFR_SEND_INSEPCTION_SFR"),
	_T("SEQ_STEP_SFR_SEND_INSEPCTION_SFR_WAIT_RECIVE"),

	_T("SEQ_STEP_SFR_MOVE_Z_ZERO_1"),
	_T("SEQ_STEP_SFR_MOVE_Z_ZERO_1_CHK"),
	_T("SEQ_STEP_SFR_MOVE_BLEMISHI_Y_POS"),
	_T("SEQ_STEP_SFR_MOVE_BLEMISHI_Y_POS_CHK"),
	_T("SEQ_STEP_SFR_MOVE_BLEMISHI_Z_POS"),
	_T("SEQ_STEP_SFR_MOVE_BLEMISHI_Z_POS_CHK"),

	_T("SEQ_STEP_SFR_SEND_INSEPCTION_BLEMISHI"),
	_T("SEQ_STEP_SFR_SEND_INSEPCTION_BLEMISHI_WAIT_RECIVE"),
	_T("SEQ_STEP_SFR_SEND_INSPECTION_PREVIEW_STOP"),

	_T("SEQ_STEP_SFR_MOVE_Z_ZERO_"),
	_T("SEQ_STEP_SFR_MOVE_Z_ZERO_3_CHK"),
	_T("SEQ_STEP_SFR_MOVE_Y_WAIT_POS"),
	_T("SEQ_STEP_SFR_MOVE_Y_WAIT_POS_CHK"),
	_T("SEQ_STEP_SFR_MOVE_Z_WAIT_POS"),
	_T("SEQ_STEP_SFR_MOVE_Z_WAIT_POS_CHK"),
	_T("SEQ_STEP_SFR_SEND_INSPECTION_PREVIEW_STOP_CHK"),

	_T("SEQ_STEP_SFR_MOVE_CAMERA_FIX_CYLINDER_OFF"),
	_T("SEQ_STEP_SFR_MOVE_CAMERA_FIX_CYLINDER_OFF_CHK"),

	_T("SEQ_STEP_SFR_MOVE_CAMERA_CYLINDER_DOWN"),
	_T("SEQ_STEP_SFR_MOVE_CAMERA_CYLINDER_DOWN_CHK"),

	_T("SEQ_STEP_SFR_MOVE_SOCKET_CYLINDER_OUT"),
	_T("SEQ_STEP_SFR_MOVE_SOCKET_CYLINDER_OUT_CHK"),
	_T("SEQ_STEP_SFR_UNLOAD_MOVE_SOCKET_CYLINDER_UP"),
	_T("SEQ_STEP_SFR_UNLOAD_MOVE_SOCKET_CYLINDER_UP_CHK"),

	_T("SEQ_STEP_SFR_WAIT_STOP_SWITCH_PUSH"),

	_T("SEQ_STEP_SFR_CYCLE_COMPLEATE"),
	_T("SEQ_STEP_SFR_DRYRUN_MODE"),

	_T("SEQ_STEP_SFR_MOVEBACK_STEP001_CHECK_BACK_COMMAND"),
	_T("SEQ_STEP_SFR_MOVEBACK_STEP002_MOVEZ_WAITPOS"),
	_T("SEQ_STEP_SFR_MOVEBACK_STEP003_MOVEZ_WAITPOS_CHK"),
	_T("SEQ_STEP_SFR_MOVEBACK_STEP004_MOVEY_WAITPOS"),
	_T("SEQ_STEP_SFR_MOVEBACK_STEP005_MOVEY_WAITPOS_CHK"),
	_T("SEQ_STEP_SFR_MOVEBACK_STEP006_COMPLEATE"),
};
#elif(GET_INSPECTOR == SYS_OQC_SFR_SINGLE_CL)

typedef enum MyEnum
{
	SEQ_STEP_OQC_SINGLE_STARTUP_STANDBY,
	SEQ_STEP_OQC_SINGLE_STARTUP_COMPLETE,
	SEQ_STEP_OQC_SINGLE_STARTUP_ERROR,

	// 0
	SEQ_STEP_OQC_SINGLE_STARTUP_LOAD_START,
	SEQ_STEP_OQC_SINGLE_STARTUP_LOAD_CAMERA_DETECT_CHECK,		// camera detect
	SEQ_STEP_OQC_SINGLE_STARTUP_LOAD_CAMERA_FIX_OFF,			// camera fix off
	SEQ_STEP_OQC_SINGLE_STARTUP_LOAD_CAMERA_FIX_OFF_CHECK,		// camera fix off check
	SEQ_STEP_OQC_SINGLE_STARTUP_LOAD_SOCKET_DN,					// socket dn
	SEQ_STEP_OQC_SINGLE_STARTUP_LOAD_SOCKET_DN_CHECK,			// socket dn check
	SEQ_STEP_OQC_SINGLE_STARTUP_LOAD_SOCKET_IN,					// socket in
	SEQ_STEP_OQC_SINGLE_STARTUP_LOAD_SOCKET_IN_CHECK,			// socket in check
	SEQ_STEP_OQC_SINGLE_STARTUP_LOAD_SOCKET_UP,					// socket up
	SEQ_STEP_OQC_SINGLE_STARTUP_LOAD_SOCKET_UP_CHECK,			// socket up check
	SEQ_STEP_OQC_SINGLE_STARTUP_LOAD_CAMERA_FIX_ON,				// camera fix on
	SEQ_STEP_OQC_SINGLE_STARTUP_LOAD_CAMERA_FIX_ON_CHECK,		// camera fix on check
	SEQ_STEP_OQC_SINGLE_STARTUP_LOAD_PROBE_PIN_UP,				// probe pin up
	SEQ_STEP_OQC_SINGLE_STARTUP_LOAD_PROBE_PIN_UP_CHECK,		// probe pin up check
	SEQ_STEP_OQC_SINGLE_STARTUP_LOAD_POWER_ON,					// power on
	SEQ_STEP_OQC_SINGLE_STARTUP_LOAD_PREVIEWSTART,				// preview start
	SEQ_STEP_OQC_SINGLE_STARTUP_LOAD_PREVIEWSTART_ACK_CHECK,	// preview start check
	SEQ_STEP_OQC_SINGLE_STARTUP_LOAD_RETRY_PREVIEWSTART,		// retry preview start
	SEQ_STEP_OQC_SINGLE_STARTUP_LOAD_END,

	// 3
	SEQ_STEP_OQC_SINGLE_STARTUP_UNLOAD_START,
	SEQ_STEP_OQC_SINGLE_STARTUP_UNLOAD_MOVE_Y,					// move y position
	SEQ_STEP_OQC_SINGLE_STARTUP_UNLOAD_MOVE_Y_CHECK,			// move y moving check
	SEQ_STEP_OQC_SINGLE_STARTUP_UNLOAD_PREVIEW_STOP,			// prewview stop
	SEQ_STEP_OQC_SINGLE_STARTUP_UNLOAD_PREVIEW_STOP_CHECK,		// preview stop check
	SEQ_STEP_OQC_SINGLE_STARTUP_UNLOAD_POWER_OFF,				// power off
	SEQ_STEP_OQC_SINGLE_STARTUP_UNLOAD_PROBE_DN,				// probe pin dn
	SEQ_STEP_OQC_SINGLE_STARTUP_UNLOAD_PROBE_DN_CHECK,			// probe pin dn check
	SEQ_STEP_OQC_SINGLE_STARTUP_UNLOAD_CAMERA_FIX_OFF,			// camera fix off
	SEQ_STEP_OQC_SINGLE_STARTUP_UNLOAD_CAMERA_FIX_OFF_CHECK,	// camera fix off check
	SEQ_STEP_OQC_SINGLE_STARTUP_UNLOAD_SOCKET_DN,				// socket dn
	SEQ_STEP_OQC_SINGLE_STARTUP_UNLOAD_SOCKET_DN_CHECK,			// socket dn check
	SEQ_STEP_OQC_SINGLE_STARTUP_UNLOAD_SOCKET_OUT,				// socket out
	SEQ_STEP_OQC_SINGLE_STARTUP_UNLOAD_SOCKET_OUT_CHECK,		// socket out check
	SEQ_STEP_OQC_SINGLE_STARTUP_UNLOAD_SOCKET_UP,				// socket up
	SEQ_STEP_OQC_SINGLE_STARTUP_UNLOAD_SOCKET_UP_CHECK,			// socket up check
	SEQ_STEP_OQC_SINGLE_STARTUP_UNLOAD_END,

	// 1
	SEQ_STEP_OQC_SINGLE_STARTUP_BLEMISH_START,
	SEQ_STEP_OQC_SINGLE_STARTUP_BLEMISH_SOCKET_POS_CHECK,		// socket position check 
	SEQ_STEP_OQC_SINGLE_STARTUP_BLEMISH_MOTOR_POS_CHECK,		// blemish position check
	SEQ_STEP_OQC_SINGLE_STARTUP_BLEMISH_MOVE_Y,					// blemish move y position
	SEQ_STEP_OQC_SINGLE_STARTUP_BLEMISH_MOVE_Y_CHECK,			// blemish move y moving check
	SEQ_STEP_OQC_SINGLE_STARTUP_BLEMISH_SEND_COMMAND,			// blemish send command
	SEQ_STEP_OQC_SINGLE_STARTUP_BLEMISH_SEND_COMMAND_CHECK,		// blemish ack check
	SEQ_STEP_OQC_SINGLE_STARTUP_BLEMISH_END,

	// 2
	SEQ_STEP_OQC_SINGLE_STARTUP_SFR_START,
	SEQ_STEP_OQC_SINGLE_STARTUP_SFR_SOCKET_POS_CHECK,		// socket position check 
	SEQ_STEP_OQC_SINGLE_STARTUP_SFR_MOTOR_POS_CHECK,		// SFR position check
	SEQ_STEP_OQC_SINGLE_STARTUP_SFR_MOVE_Y,					// SFR move y position
	SEQ_STEP_OQC_SINGLE_STARTUP_SFR_MOVE_Y_CHECK,			// SFR move y moving check
	SEQ_STEP_OQC_SINGLE_STARTUP_SFR_SEND_COMMAND,			// SFR send command
	SEQ_STEP_OQC_SINGLE_STARTUP_SFR_SEND_COMMAND_CHECK,		// SFR ack check
	SEQ_STEP_OQC_SINGLE_STARTUP_SFR_END,
}enSeqOqcSingleStartUpStep;

static LPCTSTR g_szSeqStartUpName[] =
{
	_T("SEQ_STEP_OQC_SINGLE_STARTUP_STANDBY"),
	_T("SEQ_STEP_OQC_SINGLE_STARTUP_COMPLETE"),
	_T("SEQ_STEP_OQC_SINGLE_STARTUP_ERROR"),

	// 0
	_T("SEQ_STEP_OQC_SINGLE_STARTUP_LOAD_START"),
	_T("SEQ_STEP_OQC_SINGLE_STARTUP_LOAD_CAMERA_DETECT_CHECK"),		// camera detect
	_T("SEQ_STEP_OQC_SINGLE_STARTUP_LOAD_CAMERA_FIX_OFF"),			// camera fix off
	_T("SEQ_STEP_OQC_SINGLE_STARTUP_LOAD_CAMERA_FIX_OFF_CHECK"),	// camera fix off check
	_T("SEQ_STEP_OQC_SINGLE_STARTUP_LOAD_SOCKET_DN"),				// socket dn
	_T("SEQ_STEP_OQC_SINGLE_STARTUP_LOAD_SOCKET_DN_CHECK"),			// socket dn check
	_T("SEQ_STEP_OQC_SINGLE_STARTUP_LOAD_SOCKET_IN"),				// socket in
	_T("SEQ_STEP_OQC_SINGLE_STARTUP_LOAD_SOCKET_IN_CHECK"),			// socket in check
	_T("SEQ_STEP_OQC_SINGLE_STARTUP_LOAD_SOCKET_UP"),				// socket up
	_T("SEQ_STEP_OQC_SINGLE_STARTUP_LOAD_SOCKET_UP_CHECK"),			// socket up check
	_T("SEQ_STEP_OQC_SINGLE_STARTUP_LOAD_CAMERA_FIX_ON"),			// camera fix on
	_T("SEQ_STEP_OQC_SINGLE_STARTUP_LOAD_CAMERA_FIX_ON_CHECK"),		// camera fix on check
	_T("SEQ_STEP_OQC_SINGLE_STARTUP_LOAD_PROBE_PIN_UP"),			// probe pin up
	_T("SEQ_STEP_OQC_SINGLE_STARTUP_LOAD_PROBE_PIN_UP_CHECK"),		// probe pin up check
	_T("SEQ_STEP_OQC_SINGLE_STARTUP_LOAD_POWER_ON"),				// power on
	_T("SEQ_STEP_OQC_SINGLE_STARTUP_LOAD_PREVIEWSTART"),			// preview start
	_T("SEQ_STEP_OQC_SINGLE_STARTUP_LOAD_PREVIEWSTART_ACK_CHECK"),	// preview start check
	_T("SEQ_STEP_OQC_SINGLE_STARTUP_LOAD_RETRY_PREVIEWSTART"),		// retry preview start
	_T("SEQ_STEP_OQC_SINGLE_STARTUP_LOAD_END"),

	// 3
	_T("SEQ_STEP_OQC_SINGLE_STARTUP_UNLOAD_START"),
	_T("SEQ_STEP_OQC_SINGLE_STARTUP_UNLOAD_WAIT_MOVE_Y"),			// wait move Y position
	_T("SEQ_STEP_OQC_SINGLE_STARTUP_UNLOAD_WAIT_MOVE_Y_CHECK"),		// wait move Y position moving check
	_T("SEQ_STEP_OQC_SINGLE_STARTUP_UNLOAD_PREVIEW_STOP"),			// prewview stop
	_T("SEQ_STEP_OQC_SINGLE_STARTUP_UNLOAD_PREVIEW_STOP_CHECK"),	// preview stop check
	_T("SEQ_STEP_OQC_SINGLE_STARTUP_UNLOAD_POWER_OFF"),				// power off
	_T("SEQ_STEP_OQC_SINGLE_STARTUP_UNLOAD_PROBE_DN"),				// probe pin dn
	_T("SEQ_STEP_OQC_SINGLE_STARTUP_UNLOAD_PROBE_DN_CHECK"),		// probe pin dn check
	_T("SEQ_STEP_OQC_SINGLE_STARTUP_UNLOAD_CAMERA_FIX_OFF"),		// camera fix off
	_T("SEQ_STEP_OQC_SINGLE_STARTUP_UNLOAD_CAMERA_FIX_OFF_CHECK"),	// camera fix off check
	_T("SEQ_STEP_OQC_SINGLE_STARTUP_UNLOAD_SOCKET_DN"),				// socket dn
	_T("SEQ_STEP_OQC_SINGLE_STARTUP_UNLOAD_SOCKET_DN_CHECK"),		// socket dn check
	_T("SEQ_STEP_OQC_SINGLE_STARTUP_UNLOAD_SOCKET_OUT"),			// socket out
	_T("SEQ_STEP_OQC_SINGLE_STARTUP_UNLOAD_SOCKET_OUT_CHECK"),		// socket out check
	_T("SEQ_STEP_OQC_SINGLE_STARTUP_UNLOAD_SOCKET_UP"),				// socket up
	_T("SEQ_STEP_OQC_SINGLE_STARTUP_UNLOAD_SOCKET_UP_CHECK"),		// socket up check
	_T("SEQ_STEP_OQC_SINGLE_STARTUP_UNLOAD_END"),

	// 1
	_T("SEQ_STEP_OQC_SINGLE_STARTUP_BLEMISH_START"),
	_T("SEQ_STEP_OQC_SINGLE_STARTUP_BLEMISH_SOCKET_POS_CHECK"),		// socket position check 
	_T("SEQ_STEP_OQC_SINGLE_STARTUP_BLEMISH_MOTOR_POS_CHECK"),		// blemish position check
	_T("SEQ_STEP_OQC_SINGLE_STARTUP_BLEMISH_MOVE_Y"),				// blemish move y position
	_T("SEQ_STEP_OQC_SINGLE_STARTUP_BLEMISH_MOVE_Y_CHECK"),			// blemish move y moving check
	_T("SEQ_STEP_OQC_SINGLE_STARTUP_BLEMISH_SEND_COMMAND"),			// blemish send command
	_T("SEQ_STEP_OQC_SINGLE_STARTUP_BLEMISH_SEND_COMMAND_CHECK"),	// blemish ack check
	_T("SEQ_STEP_OQC_SINGLE_STARTUP_BLEMISH_END"),

	// 2
	_T("SEQ_STEP_OQC_SINGLE_STARTUP_SFR_START"),
	_T("SEQ_STEP_OQC_SINGLE_STARTUP_SFR_SOCKET_POS_CHECK"),		// socket position check 
	_T("SEQ_STEP_OQC_SINGLE_STARTUP_SFR_MOTOR_POS_CHECK"),		// SFR position check
	_T("SEQ_STEP_OQC_SINGLE_STARTUP_SFR_MOVE_Y"),				// SFR move y position
	_T("SEQ_STEP_OQC_SINGLE_STARTUP_SFR_MOVE_Y_CHECK"),			// SFR move y moving check
	_T("SEQ_STEP_OQC_SINGLE_STARTUP_SFR_SEND_COMMAND"),			// SFR send command
	_T("SEQ_STEP_OQC_SINGLE_STARTUP_SFR_SEND_COMMAND_CHECK"),	// SFR ack check
	_T("SEQ_STEP_OQC_SINGLE_STARTUP_SFR_END"),
};

typedef enum
{
	SEQ_STEP_SFR_SINGLE_STANDBY = 0,
	SEQ_STEP_SFR_SINGLE_ERROR,

	SEQ_STEP_SFR_SINGLE_STARTUPMODE_ACTION_WAIT,
	SEQ_STEP_SFR_SINGLE_READY,
	SEQ_STEP_SFR_SINGLE_INTERLOCK_CHECK,

	SEQ_STEP_SFR_SINGLE_MOVE_PROBE_CYLINDER_UP,//add
	SEQ_STEP_SFR_SINGLE_MOVE_PROBE_CYLINDER_UP_CHK,//add
	SEQ_STEP_SFR_SINGLE_MOVE_SOCKET_CYLINDER_DOWN, //add
	SEQ_STEP_SFR_SINGLE_MOVE_SOCKET_CYLINDER_DOWN_CHK,//add
	SEQ_STEP_SFR_SINGLE_MOVE_SOCKET_CYLINDER_IN,//add
	SEQ_STEP_SFR_SINGLE_MOVE_SOCKET_CYLINDER_IN_CHK,//add
	SEQ_STEP_SFR_SINGLE_MOVE_SOCKET_CYLINDER_UP,//add
	SEQ_STEP_SFR_SINGLE_MOVE_SOCKET_CYLINDER_UP_CHK,//add
	SEQ_STEP_SFR_SINGLE_MOVE_CAMERA_FIX_CYLINDER_ON,//add
	SEQ_STEP_SFR_SINGLE_MOVE_CAMERA_FIX_CYLINDER_ON_CHK,//add

	SEQ_STEP_SFR_SINGLE_SEND_INSPECTION_PREVIEW_START,

	SEQ_STEP_SFR_SINGLE_MOVE_CYLINDER_UP_Y_POS,  //Blemish 위치 이동. 이 여기를 넘여야 Start 버튼 꺼짐.
	SEQ_STEP_SFR_SINGLE_MOVE_CYLINDER_UP_Y_POS_CHK,
	SEQ_STEP_SFR_SINGLE_MOVE_SFR_Y_POS,
	SEQ_STEP_SFR_SINGLE_MOVE_SFR_Y_POS_CHK,

	SEQ_STEP_SFR_SINGLE_SEND_INSPECTION_PREVIEW_RESTART,
	SEQ_STEP_SFR_SINGLE_SEND_INSPECTION_PREVIEW_START_CHK,

	SEQ_STEP_SFR_SINGLE_SEND_INSPECTION_MEASURE_CURRENT,

	SEQ_STEP_SFR_SINGLE_SEND_INSEPCTION_SFR,
	SEQ_STEP_SFR_SINGLE_SEND_INSEPCTION_SFR_WAIT_RECIVE,

	SEQ_STEP_SFR_SINGLE_MOVE_BLEMISHI_Y_POS,
	SEQ_STEP_SFR_SINGLE_MOVE_BLEMISHI_Y_POS_CHK,

	SEQ_STEP_SFR_SINGLE_SEND_INSEPCTION_BLEMISHI,
	SEQ_STEP_SFR_SINGLE_SEND_INSEPCTION_BLEMISHI_WAIT_RECIVE,
	SEQ_STEP_SFR_SINGLE_SEND_INSPECTION_PREVIEW_STOP,

	SEQ_STEP_SFR_SINGLE_MOVE_Y_WAIT_POS,
	SEQ_STEP_SFR_SINGLE_MOVE_Y_WAIT_POS_CHK,

	SEQ_STEP_SFR_SINGLE_SEND_INSPECTION_PREVIEW_STOP_CHK,

	SEQ_STEP_SFR_SINGLE_MOVE_CAMERA_FIX_CYLINDER_OFF,//add
	SEQ_STEP_SFR_SINGLE_MOVE_CAMERA_FIX_CYLINDER_OFF_CHK,

	SEQ_STEP_SFR_SINGLE_MOVE_CAMERA_CYLINDER_DOWN,
	SEQ_STEP_SFR_SINGLE_MOVE_CAMERA_CYLINDER_DOWN_CHK,

	SEQ_STEP_SFR_SINGLE_MOVE_SOCKET_CYLINDER_OUT,//add
	SEQ_STEP_SFR_SINGLE_MOVE_SOCKET_CYLINDER_OUT_CHK,//add
	SEQ_STEP_SFR_SINGLE_UNLOAD_MOVE_SOCKET_CYLINDER_UP, //add
	SEQ_STEP_SFR_SINGLE_UNLOAD_MOVE_SOCKET_CYLINDER_UP_CHK,//add

	SEQ_STEP_SFR_SINGLE_WAIT_STOP_SWITCH_PUSH,

	SEQ_STEP_SFR_SINGLE_CYCLE_COMPLEATE,
	SEQ_STEP_SFR_SINGLE_DRYRUN_MODE,

	SEQ_STEP_SFR_SINGLE_MOVEBACK_STEP001_CHECK_BACK_COMMAND,
	SEQ_STEP_SFR_SINGLE_MOVEBACK_STEP002_MOVEZ_WAITPOS,
	SEQ_STEP_SFR_SINGLE_MOVEBACK_STEP003_MOVEZ_WAITPOS_CHK,
	SEQ_STEP_SFR_SINGLE_MOVEBACK_STEP004_MOVEY_WAITPOS,
	SEQ_STEP_SFR_SINGLE_MOVEBACK_STEP005_MOVEY_WAITPOS_CHK,
	SEQ_STEP_SFR_SINGLE_MOVEBACK_STEP006_COMPLEATE,
} enSeqSFRWideStep;

static LPCTSTR g_szSeqName[] =
{
	_T("SEQ_STEP_SFR_SINGLE_STANDBY"),
	_T("SEQ_STEP_SFR_SINGLE_ERROR"),

	_T("SEQ_STEP_SFR_SINGLE_STARTUPMODE_ACTION_WAIT"),
	_T("SEQ_STEP_SFR_SINGLE_READY"),
	_T("SEQ_STEP_SFR_SINGLE_INTERLOCK_CHECK"),

	_T("SEQ_STEP_SFR_SINGLE_MOVE_PROBE_CYLINDER_UP"),
	_T("SEQ_STEP_SFR_SINGLE_MOVE_PROBE_CYLINDER_UP_CHK"),
	_T("SEQ_STEP_SFR_SINGLE_MOVE_SOCKET_CYLINDER_DOWN"),
	_T("SEQ_STEP_SFR_SINGLE_MOVE_SOCKET_CYLINDER_DOWN_CHK"),
	_T("SEQ_STEP_SFR_SINGLE_MOVE_SOCKET_CYLINDER_IN"),
	_T("SEQ_STEP_SFR_SINGLE_MOVE_SOCKET_CYLINDER_IN_CHK"),
	_T("SEQ_STEP_SFR_SINGLE_MOVE_SOCKET_CYLINDER_UP"),
	_T("SEQ_STEP_SFR_SINGLE_MOVE_SOCKET_CYLINDER_UP_CHK"),
	_T("SEQ_STEP_SFR_SINGLE_MOVE_CAMERA_FIX_CYLINDER_ON"),
	_T("SEQ_STEP_SFR_SINGLE_MOVE_CAMERA_FIX_CYLINDER_ON_CHK"),
		
	_T("SEQ_STEP_SFR_SINGLE_SEND_INSPECTION_PREVIEW_START"),

	_T("SEQ_STEP_SFR_SINGLE_MOVE_CYLINDER_UP_Y_POS"),
	_T("SEQ_STEP_SFR_SINGLE_MOVE_CYLINDER_UP_Y_POS_CHK"),
	_T("SEQ_STEP_SFR_SINGLE_MOVE_SFR_Y_POS"),
	_T("SEQ_STEP_SFR_SINGLE_MOVE_SFR_Y_POS_CHK"),

	_T("SEQ_STEP_SFR_SINGLE_SEND_INSPECTION_PREVIEW_RESTART"),
	_T("SEQ_STEP_SFR_SINGLE_SEND_INSPECTION_PREVIEW_START_CHK"),

	_T("SEQ_STEP_SFR_SINGLE_SEND_INSPECTION_MEASURE_CURRENT"),

	_T("SEQ_STEP_SFR_SINGLE_SEND_INSEPCTION_SFR"),
	_T("SEQ_STEP_SFR_SINGLE_SEND_INSEPCTION_SFR_WAIT_RECIVE"),

	_T("SEQ_STEP_SFR_SINGLE_MOVE_BLEMISHI_Y_POS"),
	_T("SEQ_STEP_SFR_SINGLE_MOVE_BLEMISHI_Y_POS_CHK"),

	_T("SEQ_STEP_SFR_SINGLE_SEND_INSEPCTION_BLEMISHI"),
	_T("SEQ_STEP_SFR_SINGLE_SEND_INSEPCTION_BLEMISHI_WAIT_RECIVE"),
	_T("SEQ_STEP_SFR_SINGLE_SEND_INSPECTION_PREVIEW_STOP"),

	_T("SEQ_STEP_SFR_SINGLE_MOVE_Y_WAIT_POS"),
	_T("SEQ_STEP_SFR_SINGLE_MOVE_Y_WAIT_POS_CHK"),

	_T("SEQ_STEP_SFR_SINGLE_SEND_INSPECTION_PREVIEW_STOP_CHK"),

	_T("SEQ_STEP_SFR_SINGLE_MOVE_CAMERA_FIX_CYLINDER_OFF"),
	_T("SEQ_STEP_SFR_SINGLE_MOVE_CAMERA_FIX_CYLINDER_OFF_CHK"),

	_T("SEQ_STEP_SFR_SINGLE_MOVE_CAMERA_CYLINDER_DOWN"),
	_T("SEQ_STEP_SFR_SINGLE_MOVE_CAMERA_CYLINDER_DOWN_CHK"),

	_T("SEQ_STEP_SFR_SINGLE_MOVE_SOCKET_CYLINDER_OUT"),
	_T("SEQ_STEP_SFR_SINGLE_MOVE_SOCKET_CYLINDER_OUT_CHK"),
	_T("SEQ_STEP_SFR_SINGLE_UNLOAD_MOVE_SOCKET_CYLINDER_UP"),
	_T("SEQ_STEP_SFR_SINGLE_UNLOAD_MOVE_SOCKET_CYLINDER_UP_CHK"),

	_T("SEQ_STEP_SFR_SINGLE_WAIT_STOP_SWITCH_PUSH"),

	_T("SEQ_STEP_SFR_SINGLE_CYCLE_COMPLEATE"),
	_T("SEQ_STEP_SFR_SINGLE_DRYRUN_MODE"),

	_T("SEQ_STEP_SFR_SINGLE_MOVEBACK_STEP001_CHECK_BACK_COMMAND"),
	_T("SEQ_STEP_SFR_SINGLE_MOVEBACK_STEP002_MOVEZ_WAITPOS"),
	_T("SEQ_STEP_SFR_SINGLE_MOVEBACK_STEP003_MOVEZ_WAITPOS_CHK"),
	_T("SEQ_STEP_SFR_SINGLE_MOVEBACK_STEP004_MOVEY_WAITPOS"),
	_T("SEQ_STEP_SFR_SINGLE_MOVEBACK_STEP005_MOVEY_WAITPOS_CHK"),
	_T("SEQ_STEP_SFR_SINGLE_MOVEBACK_STEP006_COMPLEATE"),
};
#elif(GET_INSPECTOR == SYS_DISTORTION_30)

typedef enum MyEnum
{
	SEQ_STEP_DISTORTION_STARTUP_STANDBY, 
	SEQ_STEP_DISTORTION_STARTUP_COMPLETE,
	SEQ_STEP_DISTORTION_STARTUP_ERROR,
	
	// wait pos
	SEQ_STEP_DISTORTION_STARTUP_LOAD_START,
	SEQ_STEP_DISTORTION_STARTUP_LOAD_COVER_OPEN_CHECK,			// cover 닫힘 확인 
	SEQ_STEP_DISTORTION_STARTUP_LOAD_CAMERA_FIX_ON,				// camera fix on
	SEQ_STEP_DISTORTION_STARTUP_LOAD_CAMERA_FIX_ON_CHECK,
	SEQ_STEP_DISTORTION_STARTUP_LOAD_PROBE_PIN_UP,
	SEQ_STEP_DISTORTION_STARTUP_LOAD_PROBE_PIN_UP_CHECK,			// camera probe pin up
	SEQ_STEP_DISTORTION_STARTUP_LOAD_POWER_ON,					// power on
	SEQ_STEP_DISTORTION_STARTUP_LOAD_PREVIEW_START,				// preview start
	SEQ_STEP_DISTORTION_STARTUP_LOAD_PREVIEW_START_ACK_CHECK,	// preview start check
	SEQ_STEP_DISTORTION_STARTUP_LOAD_RETRY_PREVIEW_START,		// retry prew start 
	SEQ_STEP_DISTORTION_STARTUP_LOAD_END,

	SEQ_STEP_DISTORTION_STARTUP_UNLOAD_START,
	SEQ_STEP_DISTORTION_STARTUP_UNLOAD_PREVIEW_STOP,				// preview stop
	SEQ_STEP_DISTORTION_STARTUP_UNLOAD_PREVIEW_STOP_ACK_CHECK,
	SEQ_STEP_DISTORTION_STARTUP_UNLOAD_POWER_OFF,					// power off
	SEQ_STEP_DISTORTION_STARTUP_UNLOAD_PROBE_PIN_DN,				// probe dn
	SEQ_STEP_DISTORTION_STARTUP_UNLOAD_PROBE_PIN_DN_CHECK,			// camera fix off
	SEQ_STEP_DISTORTION_STARTUP_UNLOAD_CAMERA_FIX_OFF,				// probe dn
	SEQ_STEP_DISTORTION_STARTUP_UNLOAD_CAMERA_FIX_OFF_CHECK,		// camera fix off
	SEQ_STEP_DISTORTION_STARTUP_UNLOAD_END,
	
	// capture corner image
	SEQ_STEP_DISTORTION_STARTUP_CAPTURE_START,
	SEQ_STEP_DISTORTION_STARTUP_MOVE_CAPTURE,
	SEQ_STEP_DISTORTION_STARTUP_MOVE_CAPTURE_CHECK,
	SEQ_STEP_DISTORTION_STARTUP_CAPTURE_CAPTURE,				// send capture command
	SEQ_STEP_DISTORTION_STARTUP_CAPTURE_CAPTURE_ACK_CHECK,		// wait for capture end ack
	SEQ_STEP_DISTORTION_STARTUP_CAPTURE_END,	

	// caculation calibration
	SEQ_STEP_DISTORTION_STARTUP_CAL_START,
	SEQ_STEP_DISTORTION_STARTUP_CAL_CACULATION,				// send caculation calibration command
	SEQ_STEP_DISTORTION_STARTUP_CAL_CACULATION_ACK_CHECK,	// wait for calibration end ack
	SEQ_STEP_DISTORTION_STARTUP_CAL_END,

}enSeqDistortionStartUpStep;

static LPCTSTR g_szSeqStartUpName[] =
{
	_T("SEQ_STEP_DISTORTION_STARTUP_STANDBY"),
	_T("SEQ_STEP_DISTORTION_STARTUP_COMPLETE"),
	_T("SEQ_STEP_DISTORTION_STARTUP_ERROR"),

	// wait pos
	_T("SEQ_STEP_DISTORTION_STARTUP_LOAD_START"),
	_T("SEQ_STEP_DISTORTION_STARTUP_LOAD_COVER_OPEN_CHECK"),			// cover 닫힘 확인 
	_T("SEQ_STEP_DISTORTION_STARTUP_LOAD_CAMERA_FIX_ON"),				// camera fix on
	_T("SEQ_STEP_DISTORTION_STARTUP_LOAD_CAMERA_FIX_ON_CHECK"),
	_T("SEQ_STEP_DISTORTION_STARTUP_LOAD_PROBE_PIN_UP"),
	_T("SEQ_STEP_DISTORTION_STARTUP_LOAD_PROBE_PIN_UP_CHECK"),		// camera probe pin up
	_T("SEQ_STEP_DISTORTION_STARTUP_LOAD_POWER_ON"),					// power on
	_T("SEQ_STEP_DISTORTION_STARTUP_LOAD_PREVIEW_START"),			// preview start
	_T("SEQ_STEP_DISTORTION_STARTUP_LOAD_PREVIEW_START_ACK_CHECK"),	// preview start check
	_T("SEQ_STEP_DISTORTION_STARTUP_LOAD_RETRY_PREVIEW_START"),		// retry prew start 
	_T("SEQ_STEP_DISTORTION_STARTUP_LOAD_END"),

	_T("SEQ_STEP_DISTORTION_STARTUP_UNLOAD_START"),
	_T("SEQ_STEP_DISTORTION_STARTUP_UNLOAD_PREVIEW_STOP"),				// preview stop
	_T("SEQ_STEP_DISTORTION_STARTUP_UNLOAD_PREVIEW_STOP_ACK_CHECK"),
	_T("SEQ_STEP_DISTORTION_STARTUP_UNLOAD_POWER_OFF"),				// power off
	_T("SEQ_STEP_DISTORTION_STARTUP_UNLOAD_PROBE_PIN_DN"),				// probe dn
	_T("SEQ_STEP_DISTORTION_STARTUP_UNLOAD_PROBE_PIN_DN_CHECK"),		// camera fix off
	_T("SEQ_STEP_DISTORTION_STARTUP_UNLOAD_CAMERA_FIX_OFF"),			// probe dn
	_T("SEQ_STEP_DISTORTION_STARTUP_UNLOAD_CAMERA_FIX_OFF_CHECK"),		// camera fix off
	_T("SEQ_STEP_DISTORTION_STARTUP_UNLOAD_END"),

	// capture corner image
	_T("SEQ_STEP_DISTORTION_STARTUP_CAPTURE_START"),
	_T("SEQ_STEP_DISTORTION_STARTUP_MOVE_CAPTURE"),						// send capture command
	_T("SEQ_STEP_DISTORTION_STARTUP_MOVE_CAPTURE_CHECK"),				// send capture command
	_T("SEQ_STEP_DISTORTION_STARTUP_CAPTURE_CAPTURE"),					// send capture command
	_T("SEQ_STEP_DISTORTION_STARTUP_CAPTURE_CAPTURE_ACK_CHECK"),		// wait for capture end ack
	_T("SEQ_STEP_DISTORTION_STARTUP_CAPTURE_END"),
																
	// caculation calibration
	_T("SEQ_STEP_DISTORTION_STARTUP_CAL_START"),
	_T("SEQ_STEP_DISTORTION_STARTUP_CAL_CACULATION"),				// send caculation calibration command
	_T("SEQ_STEP_DISTORTION_STARTUP_CAL_CACULATION_ACK_CHECK"),		// wait for calibration end ack
	_T("SEQ_STEP_DISTORTION_STARTUP_CAL_END"),
};

typedef enum
{
	SEQ_STEP_DISTORTION_STANDBY = 0,
	SEQ_STEP_DISTORTION_ERROR,

	SEQ_STEP_DISTORTION_STARTUPMODE_ACTION_WAIT,
	SEQ_STEP_DISTORTION_READY,
	SEQ_STEP_DISTORTION_INTERLOCK_CHECK,

	SEQ_STEP_DISTORTION_CAMERA_FIX_CYLINDER_ON,
	SEQ_STEP_DISTORTION_CAMERA_FIX_CYLINDER_ON_CHK,
	SEQ_STEP_DISTORTION_PROBE_PIN_UP,
	SEQ_STEP_DISTORTION_PROBE_PIN_UP_CHK,

	SEQ_STEP_DISTORTION_INSPECTION_PREVIEW_START,
	SEQ_STEP_DISTORTION_INSPECTION_MOVE_CAPTURE_INIT_POSITION,			// 230707  
	SEQ_STEP_DISTORTION_INSPECTION_PREVIEW_START_CHK,
	SEQ_STEP_DISTORTION_INSPECTION_MOVE_CAPTURE_INIT_POSITION_CHK,		// 230707   

	SEQ_STEP_DISTORTION_INSEPCTION_CORNERS,
	SEQ_STEP_DISTORTION_INSEPCTION_CORNERS_WAIT_RECIVE,
	
	SEQ_STEP_DISTORTION_INSPECTION_MOVE_CAPTURE_POSITION,				// 230707
	SEQ_STEP_DISTORTION_INSPECTION_MOVE_CAPTURE_POSITION_CHK,			// 230707

	SEQ_STEP_DISTORTION_INSEPCTION_INTRINSIC_CAL,
	SEQ_STEP_DISTORTION_INSPECTION_MOVE_UNLOAD,							// 230707
	SEQ_STEP_DISTORTION_INSEPCTION_INTRINSIC_CAL_WAIT_RECIVE,
	SEQ_STEP_DISTORTION_INSPECTION_PREVIEW_STOP,
	SEQ_STEP_DISTORTION_INSPECTION_PREVIEW_STOP_CHK,
	
	SEQ_STEP_DISTORTION_PROBE_PIN_DOWN,
	SEQ_STEP_DISTORTION_PROBE_PIN_DOWN_CHK,
	SEQ_STEP_DISTORTION_MOVE_CAMERA_FIX_CYLINDER_OFF,
	SEQ_STEP_DISTORTION_MOVE_CAMERA_FIX_CYLINDER_OFF_CHK,

	SEQ_STEP_DISTORTION_INSPECTION_MOVE_UNLOAD_CHK,						// 230707

	SEQ_STEP_DISTORTION_WAIT_STOP_SWITCH_PUSH,
	SEQ_STEP_DISTORTION_SEND_INSPECTION_WAIT_STOP_SWITCH_SIGNAL,

	SEQ_STEP_DISTORTION_CYCLE_COMPLEATE,
	SEQ_STEP_DISTORTION_DRYRUN_MODE,

} enSeqDistortionStep;

static LPCTSTR g_szSeqName[] =
{
	_T("SEQ_STEP_DISTORTION_STANDBY"),
	_T("SEQ_STEP_DISTORTION_ERROR"),

	_T("SEQ_STEP_DISTORTION_STARTUPMODE_ACTION_WAIT"),
	_T("SEQ_STEP_DISTORTION_READY"),
	_T("SEQ_STEP_DISTORTION_INTERLOCK_CHECK"),

	_T("SEQ_STEP_DISTORTION_CAMERA_FIX_CYLINDER_ON"),
	_T("SEQ_STEP_DISTORTION_CAMERA_FIX_CYLINDER_ON_CHK"),
	_T("SEQ_STEP_DISTORTION_PROBE_PIN_UP"),
	_T("SEQ_STEP_DISTORTION_PROBE_PIN_UP_CHK"),

	_T("SEQ_STEP_DISTORTION_INSPECTION_PREVIEW_START"),
	_T("SEQ_STEP_DISTORTION_INSPECTION_MOVE_CAPTURE_INIT_POSITION"),		// 230707
	_T("SEQ_STEP_DISTORTION_INSPECTION_PREVIEW_START_CHK"),
	_T("SEQ_STEP_DISTORTION_INSPECTION_MOVE_CAPTURE_INIT_POSITION_CHK"),	// 230707

	_T("SEQ_STEP_DISTORTION_INSEPCTION_CORNERS"),
	_T("SEQ_STEP_DISTORTION_INSEPCTION_CORNERS_WAIT_RECIVE"),

	_T("SEQ_STEP_DISTORTION_INSPECTION_MOVE_CAPTURE_POSITION"),				// 230707
	_T("SEQ_STEP_DISTORTION_INSPECTION_MOVE_CAPTURE_POSITION_CHK"),			// 230707

	_T("SEQ_STEP_DISTORTION_INSEPCTION_INTRINSIC_CAL"),
	_T("SEQ_STEP_DISTORTION_INSPECTION_MOVE_UNLOAD"),						// 230707
	_T("SEQ_STEP_DISTORTION_INSEPCTION_INTRINSIC_CAL_WAIT_RECIVE"),
	_T("SEQ_STEP_DISTORTION_INSPECTION_PREVIEW_STOP"),
	_T("SEQ_STEP_DISTORTION_INSPECTION_PREVIEW_STOP_CHK"),
	
	_T("SEQ_STEP_DISTORTION_PROBE_PIN_DOWN"),
	_T("SEQ_STEP_DISTORTION_PROBE_PIN_DOWN_CHK"),
	_T("SEQ_STEP_DISTORTION_MOVE_CAMERA_FIX_CYLINDER_OFF"),
	_T("SEQ_STEP_DISTORTION_MOVE_CAMERA_FIX_CYLINDER_OFF_CHK"),

	_T("SEQ_STEP_DISTORTION_INSPECTION_MOVE_UNLOAD_CHK"),					// 230707

	_T("SEQ_STEP_DISTORTION_WAIT_STOP_SWITCH_PUSH"),
	_T("SEQ_STEP_DISTORTION_SEND_INSPECTION_WAIT_STOP_SWITCH_SIGNAL"),

	_T("SEQ_STEP_DISTORTION_CYCLE_COMPLEATE"),
	_T("SEQ_STEP_DISTORTION_DRYRUN_MODE"),
};

#endif