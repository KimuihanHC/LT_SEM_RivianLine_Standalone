#pragma once

#include "FAS_EziMOTIONPlusR.h"

//Timer
#define TIMER_MOTION_STATUS				1


//Pitch 계산
//모터 1회전 당 실제 이동 거리 (단위 : mm)						컨베어 풀리 구조 일 경우에는 실제 1바퀴 모터 회전시 실제 움직이는 거리를 기구에 확인 하여 입력한다.
//모터 1회전 당 실제 이동 각도	(50:1 감속기)  7.2도			1 : 1/50 = 360' : X
//모터 1회전 당 실제 이동 거리  (20리드 ) (단위 : mm)			니드 피치는 그냥 적용
//모터 1회전 당 실제 이동 각도	(1:4.5 풀리 비)					1 : 4.5 = X : 360`

#define MOTOR_RESOLITION				10000

#define AXIS_LOADER1_PITCH				88						//모터 1회전 당 실제 이동 거리 (단위 : mm)
#define AXIS_LOADER2_PITCH				88						//87.575
#define AXIS_LOADER3_PITCH				88
#define AXIS_UNLOADER1_PITCH			88
#define AXIS_UNLOADER2_PITCH			88
#define AXIS_UNLOADER3_PITCH			62
#define AXIS_NGBUFFER1_PITCH			62						//62.360
#define AXIS_NGBUFFER2_PITCH			62



//Axis
#define AXIS_LOADER1					0
#define AXIS_LOADER2					1
#define AXIS_LOADER3					2
#define AXIS_UNLOADER1					3
#define AXIS_UNLOADER2					4
#define AXIS_UNLOADER3					5
#define AXIS_NGBUFFER1					6
#define AXIS_NGBUFFER2					7

#define LOADER2_POS1					1			//Rotator 부
#define LOADER2_POS2					2			//Buffer 부
#define LOADER2_POS3					3			//Loading 부

#define NG_BUFFER1_POS1					1
#define NG_BUFFER1_POS2					2

#define NG_BUFFER2_POS1					1
#define NG_BUFFER2_POS2					2

//Parameter
#define PARAMETER_SLOWDOWN_SENSOR		35
#define PARAMETER_STOP_SENSOR			36
#define PARAMETER_SLOWDOWN_SPEED		37


//Time Out
#define MOVING_TIME_OUT					10000

#define	IS_BIT_SET(value, index)		(value & ((DWORD)0x01 << index))


class CEziServo_PR : public CWnd
{
private:
	static CEziServo_PR *m_pInstance;

public:
	static CEziServo_PR* Get_Instance();
	void Delete_Instance();

	DECLARE_DYNAMIC(CEziServo_PR)

public:
	CEziServo_PR();
	virtual ~CEziServo_PR();

	BOOL Create(CWnd* pParentWnd=NULL);

	void InitVariables();
	void InitRobotVariables();
	
	bool m_bConnected;	

	BYTE m_nPortNo;
	DWORD m_dwBaudrate;
	BYTE m_nSlaveNo;
	int m_nSlaveCount;
	int m_pSalveNumber[MAX_SLAVE_NUMS];
	bool m_bSalveEnable[MAX_SLAVE_NUMS];

	CString lastError;	

protected:
	DECLARE_MESSAGE_MAP()

protected:
	BOOL		m_bThreadRun;
	CWinThread *m_pEziServoThread;
	static UINT	Thread_EziServoRead(LPVOID lpVoid);
	

protected:	
	DWORD *m_dwIOInputValue;
	DWORD *m_dwIOOutputValue;
	DWORD *m_dwAxisStatus;	

	long *m_nCommandPos;
	long *m_nActualPos;
	long *m_nActualVel;
	long *m_nPosError;

	//Motion 동작 관련 변수
	bool *m_bHoming;
	bool *m_bHomeOK;
	bool m_bJogging;
	bool m_bStopping;
	bool m_bRepeating;
	bool *m_bMotioning;
	bool *m_bServoStatus;	
	bool *m_bInposStatus;
	bool *m_bHWPlusLimitStatus;
	bool *m_bHWMinusLimitStatus;
	bool *m_bSWPlusLimitStatus;
	bool *m_bSWMinusLimitStatus;

	//Alarm 관련 변수
	bool *m_bAlarmStatus;
	bool *m_bOverCurrentError;
	bool *m_bOverSpeedError;
	bool *m_bPostTrackingError;
	bool *m_bOverloadError;
	bool *m_bOverheatError;
	bool *m_bBackEMFError;
	bool *m_bMotorPowerError;
	bool *m_bInpositionError;
	bool *m_bPosOverflowError;
	bool *m_bSlowStopError;

	WORD *wPosItemNo;

	int m_nServoEnableFlag;											//Robot 상태 값 읽어 올때 Servo 관련 Flag 처리
	int m_nORGFlag;													//원점 이동 명령시 Flag 처리

public:
	DWORD GetAxisStatus(int Index)			const {return m_dwAxisStatus[Index];}

	long GetCommandPos(int Index)			const {return m_nCommandPos[Index];}		
	long GetActualPos(int Index)			const {return m_nActualPos[Index];}
	long GetActualVel(int Index)			const {return m_nActualVel[Index];}
	long GetPosError(int Index)				const {return m_nPosError[Index];}

	//Motion 동작 관련 변수
	bool IsHoming(int Index)				const {return m_bHoming[Index];}				//Origin 동작 중
	bool IsHomeOK(int Index)				const {return m_bHomeOK[Index];}				//Origin 완료
	bool IsMotioning(int Index)				const {return m_bMotioning[Index];}				//Motion 동작 중
	bool IsServoStatus(int Index)			const {return m_bServoStatus[Index];}			//Servo On/Off 상태	
	bool IsInposStatus(int Index)			const {return m_bInposStatus[Index];}			//Inposition 상태
	bool IsHWPlusLimitStatus(int Index)		const {return m_bHWPlusLimitStatus[Index];}		//H/W +Limit
	bool IsHWMinusLimitStatus(int Index)	const {return m_bHWMinusLimitStatus[Index];}	//H/W -Limit
	bool IsSWPlusLimitStatus(int Index)		const {return m_bSWPlusLimitStatus[Index];}		//S/W +Limit
	bool IsSWMinusLimitStatus(int Index)	const {return m_bSWMinusLimitStatus[Index];}	//S/W -Limit
	bool IsJogging()						const {return m_bJogging;}						//Jog 운전 중
	bool IsStopping()						const {return m_bStopping;}						//정지
	bool IsRepeating()						const {return m_bRepeating;}					//반복 운전 중

	//Alarm 관련 변수
	bool IsAlarmStatus(int Index)			const {return m_bAlarmStatus[Index];}			//Alarm 상태
	bool IsOverCurrentError(int Index)		const {return m_bOverCurrentError[Index];}		//과전류 이상
	bool IsOverSpeedError(int Index)		const {return m_bOverSpeedError[Index];}		//과속도 이상
	bool IsPostTrackingError(int Index)		const {return m_bPostTrackingError[Index];}		//위치추종이상
	bool IsOverloadError(int Index)			const {return m_bOverloadError[Index];}			//과부하 이상	
	bool IsOverheatError(int Index)			const {return m_bOverheatError[Index];}			//과열 이상
	bool IsBackEMFError(int Index)			const {return m_bBackEMFError[Index];}			//회생전압 이상
	bool IsMotorPowerError(int Index)		const {return m_bMotorPowerError[Index];}		//모터 전압 이상
	bool IsInpositionError(int Index)		const {return m_bInpositionError[Index];}		//인포지션 이상
	bool IsPosOverflowError(int Index)		const {return m_bPosOverflowError[Index];}		//위치 초과오차 이상
	bool IsSlowStopError(int Index)			const {return m_bSlowStopError[Index];}			//Slow Stop 이상
	

public:		
	//통신 관련
	bool Connect_EziServo(BYTE PortNo, DWORD Baudrate);						//통신 연결
	void Disconnect_EziServo();												//통신 연결 해제

	//Robot 상태 관련
	void Error_Status_Display(int Return);									//Error Display
	bool Robot_Status();													//현재 1축 로봇 상태
	bool Robot_Status_All_Axis();											//현재 전축 로봇 상태
	bool GetAllStatus();													//Robot의 모든 상태 값을 읽어 온다

	//Servo 관련 제어
	bool Servo_Enable();													//Servo On/Off
	bool Servo_Enable_Init();												//Servo On/Off(Init시 사용)
	bool Servo_Off();														//Servo Off
	bool Servo_Alarm_Reset_All();											//Servo Alarm Reset 전축
	bool Servo_Alarm_Reset_Single(int nSlave);								//Servo Alarm Reset 단축



	//운전 제어
	long GetPulse(double CommandPos, int nAxis);
	double GetActualmm(long ActualPos, int nAxis);
	double GetVelocity(int lVelocity, int nAxis);							//모션 이동 시 필요한 속도 계산
	double GetActVelocity_Disp(long lVelocity, int nAxis);					//GUI 표시를 위한 모션 이동 속도 계산
	double GetActPosition_Disp(long lPosition, int nAxis);					//GUI 표시를 위한 모션 현 위치 값 계산

	bool SetParamer(BYTE nSlaveNo, BYTE nParaNo, long lValue);				//Parameter Setting
	bool SetIOAssignMap(BYTE nSlaveNo, BYTE bLevel);						//SetIOAssignMAp Setting
	bool SetIOAssignMap_SlowStop(BYTE nSlaveNo, BYTE bLevel);				//Slow Stop 기능 활성화/비활성화

	bool MoveStop();														//1축 정지
	bool MoveStop(BYTE SlaveNo);											//선택한 1축 정지
	bool AllMoveStop();														//모든 축 정지
	bool AllEmergencyStop();												//EMG Stop
	bool SingleEmergencyStop(BYTE SlaveNo);									//1축 Stop
	bool MoveVelocity(DWORD lVelocity, int nDir, int nAxis);				//Jog 동작
	bool SingleAxisAbsMove(DWORD lVelocity, int nAxis);						//Ins Move	

	bool SetResetPosition(BYTE SlaveNo);									//Position 값 초기화
};

//extern CEziServo_PR m_ctrlEziServo;