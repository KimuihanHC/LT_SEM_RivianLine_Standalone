#pragma once
#include "pch.h"
#include "User_Define.h"

#include "Fastech_PR_Include\\FAS_EziMOTIONPlusR.h"

//Timer
#define TIMER_MOTION_STATUS_FASTECH				1
#define MOTOR_RESOLITION_FASTECH				10000

//Time Out
#define FASTECH_MOVING_TIME_OUT					10000

#define	IS_BIT_SET(value, index)		(value & ((DWORD)0x01 << index))

class CFastech_Motion : public CWnd, public CSington<CFastech_Motion>
{
	DECLARE_DYNAMIC(CFastech_Motion)

public:
	CFastech_Motion();
	virtual ~CFastech_Motion();

public:
	BOOL Create(CWnd* pParentWnd = NULL);

	void InitVariables();
	void InitRobotVariables();

	int m_bConnectedInitFlag;

	BYTE m_nPortNo;
	DWORD m_dwBaudrate;
	BYTE m_nSlaveNo;
	int m_nSlaveCount;
	int m_pSalveNumber[MAX_SLAVE_NUMS];
	bool m_bSalveEnable[MAX_SLAVE_NUMS];

	CString GetLastError()
	{
		return lastError;
	};
	  
	CString lastError;

protected:
	DECLARE_MESSAGE_MAP()

protected:
	BOOL		m_bThreadRun;
	CWinThread *m_pEziServoThread;
	static UINT	Thread_EziServoRead(LPVOID lpVoid);


protected:
	bool m_bConnected;

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
	bool *m_bHWHomeStatus;
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
	bool IsConnected();
	DWORD GetAxisStatus(int Index)			const { return m_dwAxisStatus[Index]; }

	long GetCommandPos(int Index)			const { return m_nCommandPos[Index]; }
	long GetActualPos(int Index)			const { return m_nActualPos[Index]; }
	long GetActualVel(int Index)			const { return m_nActualVel[Index]; }
	long GetPosError(int Index)				const { return m_nPosError[Index]; }

	//Motion 동작 관련 변수
	bool IsHoming(int Index)				const { return m_bHoming[Index]; }				//Origin 동작 중
	bool IsHomeOK(int Index)				const { return m_bHomeOK[Index]; }				//Origin 완료
	bool IsMotioning(int Index)				const { return m_bMotioning[Index]; }			//Motion 동작 중
	bool IsServoStatus(int Index)			const { return m_bServoStatus[Index]; }			//Servo On/Off 상태	
	bool IsInposStatus(int Index)			const { return m_bInposStatus[Index]; }			//Inposition 상태
	bool IsHWPlusLimitStatus(int Index)		const { return m_bHWPlusLimitStatus[Index]; }	//H/W +Limit
	bool IsHWMinusLimitStatus(int Index)	const { return m_bHWMinusLimitStatus[Index]; }	//H/W -Limit
	bool IsHWHomeStatus(int Index)			const { return m_bHWHomeStatus[Index]; }		//H/W Home
	bool IsSWPlusLimitStatus(int Index)		const { return m_bSWPlusLimitStatus[Index]; }		//S/W +Limit
	bool IsSWMinusLimitStatus(int Index)	const { return m_bSWMinusLimitStatus[Index]; }	//S/W -Limit
	bool IsJogging()						const { return m_bJogging; }					//Jog 운전 중
	bool IsStopping()						const { return m_bStopping; }					//정지
	bool IsRepeating()						const { return m_bRepeating; }					//반복 운전 중

	//Alarm 관련 변수
	bool IsAlarmStatus(int Index)			const { return m_bAlarmStatus[Index]; }			//Alarm 상태
	bool IsOverCurrentError(int Index)		const { return m_bOverCurrentError[Index]; }	//과전류 이상
	bool IsOverSpeedError(int Index)		const { return m_bOverSpeedError[Index]; }		//과속도 이상
	bool IsPostTrackingError(int Index)		const { return m_bPostTrackingError[Index]; }	//위치추종이상
	bool IsOverloadError(int Index)			const { return m_bOverloadError[Index]; }		//과부하 이상	
	bool IsOverheatError(int Index)			const { return m_bOverheatError[Index]; }		//과열 이상
	bool IsBackEMFError(int Index)			const { return m_bBackEMFError[Index]; }		//회생전압 이상
	bool IsMotorPowerError(int Index)		const { return m_bMotorPowerError[Index]; }		//모터 전압 이상
	bool IsInpositionError(int Index)		const { return m_bInpositionError[Index]; }		//인포지션 이상
	bool IsPosOverflowError(int Index)		const { return m_bPosOverflowError[Index]; }	//위치 초과오차 이상
	bool IsSlowStopError(int Index)			const { return m_bSlowStopError[Index]; }		//Slow Stop 이상


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
	bool Servo_Enable(BYTE SlaveNo);													//Servo On/Off //
	bool Servo_Enable_Init();												//Servo On/Off(Init시 사용)
	bool Servo_Off();														//Servo Off
	bool Servo_Off(BYTE SlaveNo);
	bool Servo_Alarm_Reset_All();											//Servo Alarm Reset 전축
	bool Servo_Alarm_Reset_Single(int nSlave);								//Servo Alarm Reset 단축

	double GetAxisCurrnetActualPos(int nIndex);


	//운전 제어
	long GetPulse(double CommandPos, int nAxis);
	double GetActualmm(long ActualPos, int nAxis);
	double GetVelocity(int lVelocity, int nAxis);							//모션 이동 시 필요한 속도 계산
	double GetActVelocity_Disp(long lVelocity, int nAxis);					//GUI 표시를 위한 모션 이동 속도 계산
	double GetActPosition_Disp(long lPosition, int nAxis);					//GUI 표시를 위한 모션 현 위치 값 계산

	bool SetParamer(BYTE nSlaveNo, BYTE nParaNo, long lValue);				//Parameter Setting
	bool SetIOAssignMap(BYTE nSlaveNo, BYTE bLevel);						//SetIOAssignMAp Setting
	bool SetIOAssignMap_SlowStop(BYTE nSlaveNo, BYTE bLevel);				//Slow Stop 기능 활성화/비활성화

	void InitHomeOK(int Index);												//Origin 완료
	bool MoveHome(BYTE lAxis);													//해당 축 위치로 이동
	bool MoveHome_All(BYTE PortNo);										//전 축 초기화
	int	 MoveLimit(BYTE PortNo, BYTE SlaveNo, DWORD lVelocity, int LimitPos);				//Limit위치로 이동 -0 +1
	bool MoveStop();														//1축 정지
	bool MoveStop(BYTE SlaveNo);											//선택한 1축 정지
	bool AllMoveStop();														//모든 축 정지
	bool AllEmergencyStop();												//EMG Stop
	bool SingleEmergencyStop(BYTE SlaveNo);									//1축 Stop
	bool MoveVelocity(int nAxis, DWORD lVelocity, int nDir);				//Jog 동작
	BOOL SingleAxisAbsMove(int nAxis, double dPos, DWORD lVelocity );										//abs Move	
	BOOL SingleAxisIncMove(int nAxis, double dPos, DWORD lVelocity);										//abs Move	
	bool SetResetPosition(BYTE SlaveNo);									//Position 값 초기화
};