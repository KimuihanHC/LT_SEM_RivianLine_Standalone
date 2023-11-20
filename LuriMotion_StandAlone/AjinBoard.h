#pragma once

#include "pch.h"
#include "User_Define.h"



//Timer
#define TIMER_MOTION_STATUS				1


//Pitch 계산
//모터 1회전 당 실제 이동 거리 (단위 : mm)						컨베어 풀리 구조 일 경우에는 실제 1바퀴 모터 회전시 실제 움직이는 거리를 기구에 확인 하여 입력한다.
//모터 1회전 당 실제 이동 각도	(50:1 감속기)  7.2도			1 : 1/50 = 360' : X
//모터 1회전 당 실제 이동 거리  (20리드 ) (단위 : mm)			니드 피치는 그냥 적용
//모터 1회전 당 실제 이동 각도	(1:4.5 풀리 비)					1 : 4.5 = X : 360`


//Time Out
#define NGTRAY_WAITING_TIME_OUT			30000		// NG TRAY 교체 대기 시간.
#define MOTION_MOVING_TIME_OUT			20000		// 모터 동작 후 완료 대기 시간.
#define CYLINDER_MOVING_TIME_OUT		5000		// 실린더 동작 후 완료 대기 시간.

#define	IS_BIT_SET(value, index)		(value & ((DWORD)0x01 << index))




class CAjinMotion : public CWnd, public CSington<CAjinMotion>
{
	DECLARE_DYNAMIC(CAjinMotion)

public:
	CAjinMotion();
	virtual ~CAjinMotion();

	BOOL Create(CWnd* pParentWnd=NULL);

	void InitVariables();
	void InitRobotVariables();
	
	bool m_bConnected;	

	int m_nBdID;
	
	CString lastError;	

protected:
	DECLARE_MESSAGE_MAP()

protected:
	BOOL		m_bThreadRun;
	CWinThread *m_pMotionThread;
	static UINT	Thread_MotionRead(LPVOID lpVoid);
	

protected:	

	int m_nServoEnableFlag;											//Robot 상태 값 읽어 올때 Servo 관련 Flag 처리
	int m_nORGFlag;													//원점 이동 명령시 Flag 처리

public:		
	//통신 관련

	afx_msg bool Init_Ajin();
	afx_msg bool IsInit_Ajin();

	afx_msg bool LoadFile_Mot();
	afx_msg bool SaveFile_Mot();

	afx_msg void ServoOn(long lAxis);
	afx_msg void ServoOff(long lAxis);
	afx_msg bool IsServoOn(long lAxis);

	afx_msg bool SetErrorReset(long lAxis);

	afx_msg double GetActPos(long lAxis);
	afx_msg double GetCmdPos(long lAxis);
	afx_msg void GetPulseAndUnit(long lAxis, double *dUnit, long *dPulse);

	afx_msg bool Move_Home(long lAxis);
	afx_msg bool HomeResult(long lAxis);
	afx_msg BOOL GetHomeRate(long lAxis );
	afx_msg int  GetHomeRate_Gage(long lAxis);
	afx_msg DWORD Get_HomeRate_Sub();
	afx_msg void  Set_HomeRate_Sub_Zero();
	DWORD	m_dwSub;
	DWORD	m_dwMain;

	afx_msg bool MoveEStop(long lAxis);
	afx_msg bool MoveSStop(long lAxis);

	afx_msg void PositionClear(long lAxis);
	afx_msg bool MoveAxis_NonWait(long lAxis, double dPos, double dVel = 0.0f);
	afx_msg bool MoveAxis_Wait(long lAxis, double dPos, double dVel);
	afx_msg bool MoveAxis_Velocity(long lAxis, double dvel);
	afx_msg bool MotorMultiAxis_NonWait(long lAxisCnt, long *lpAxisNo, double *dpPos, double *dpVel);
	afx_msg void SetMotionMode( long lAxis, int nMode );

	afx_msg bool GantryEnable(long lAxisMaster, long lAxisSlave);
	afx_msg bool GantryDisEnable(long lAxisMaster, long lAxisSlave);
	afx_msg bool GetGantryModeEnable();

	afx_msg bool IsMotion(long lAxis);

	// invaild Chk 가 문제가 없을 경우에만 아래 5개 동작을 한다.
	afx_msg bool IsInvaildAxis();

	afx_msg bool IsServoAlarmOn();
	afx_msg bool IsServoCurPos();
	afx_msg bool IsMotionInpos();
	afx_msg bool IsLimitChk();
	afx_msg bool isServoHome();
	

public:

	// 여기서 확인한 Data는 이 Class에서 저장한다.
	DWORD		m_dwLastError;
	CString		m_strLastError;

	double		m_dbPos[MOTOR_MAX_COUNT];
	DWORD		m_bReadError[MOTOR_MAX_COUNT];
	DWORD		m_bReadHome[MOTOR_MAX_COUNT];
	DWORD		m_bReadLimitP[MOTOR_MAX_COUNT];
	DWORD		m_bReadLimitM[MOTOR_MAX_COUNT];
	DWORD		m_bReadInPos[MOTOR_MAX_COUNT];
};

